<div id="page">

# Hitchhiker’s Guide to Debugging RDP protocols: Part 1 \[MS-RDPEUSB\]

[Bryan S.
Burgin](https://social.msdn.microsoft.com/profile/Bryan%20S.%20Burgin)
5/24/2012 10:20:00 AM

-----

<div id="content">

Hitchhiker’s Guide to Debugging RDP protocols: Part 1 \[MS-RDPEUSB\]

 

**NOTE: Questions and comments are welcome.  However, please DO NOT post
a comment using  
the comment tool at the end of this post.   
Instead, post a new thread in the Open Specifications Forum: Windows  
Protocols at**
[**http://social.msdn.microsoft.com/Forums/en-US/os\_windowsprotocols**](http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols)**.**

 

\-- Introduction  
\-- Development Machine: Building the driver, creating certificates  
\-- Server Machine: Installing and testing the device; installing
certificates  
\-- Client Machine: set up the RDP client for USB device redirection  
\-- Installing Network Monitor and Capturing, Decrypting and Parsing RDP
Traffic  
   
\==============================================  
INTRODUCTION  
\==============================================

The purpose of this paper is to provide a guide to debug \[MS-RDPEUSB\]
implementations.  To illustrate \[MS-RDPEUSB\] in action, I use OSR’s
USB FX2 test board.  Alternatively, I could have selected Intel’s 82930
USB Test Board.  I make this suggestion because the driver for each
device is provided as a source code sample in the Windows Driver Kit
(WDK).  The benefit of this approach is that by having the source code
for the target device’s driver, you then “own” both endpoints of the
protocol: the driver on the RDP server-side and your \[MS-RDPEUSB\]
implementation on the client-side.  This paper will concentrate on the
OSR board.

A secondary purpose of this paper is to document how to debug various
other RDP protocols.  In our journey, we will discover how to disable
RDP compression, install a private certificate so that all RDP traffic
can be decrypted and how to capture, decrypt and parse RDP traffic. 
However, because the USB-centric instructions are so intertwined within
this paper, I wrote a second blog that excludes all the USB steps to
present a pared down reference for the generic RDP cases.  See
<http://blogs.msdn.com/b/openspecification/archive/2012/07/24/hitchhiker-s-guide-to-debugging-rdp-protocols-part-2.aspx>.

Because you will be building the driver, many of the steps in this guide
focus on building and installing the driver.  This includes installing
the WDK, building and signing the driver and installing it into the test
system.  The WDK is all-inclusive; it contains all the tools to build
the driver: compiler, linker, etc.  It also includes the utilities
Makecert and Inf2cat, both cited in this guide.  So, no other
development tools are necessary (i.e., Visual Studio is not needed).

The audience for this paper is a \[MS-RDPEUSB\] protocol implementer. 
That is, third-parties that are developing non-Windows RDP clients that
want to redirect USB devices.  The techniques provided here will enable
such a developer to set up a Windows-to-Windows \[MS-RDPEUSB\] test lab
and observe the protocol in action without being obscured by compression
or encryption.  This will provide a basis for comparison to the
developer’s own \[MS-RDPEUSB\] client implementation.

Many USB devices can be redirected over RDP using \[MS-RDPEUSB\]. 
However, keep in mind that if a device’s class has a higher-level,
class-specific protocol better suited for redirection, RDP will opt to
use the most efficient protocol possible.  For instance, Microsoft’s RDP
client will opt to use \[MS-RDPESC\] for Smartcards and \[MS-RDPEFS\]
for USB thumb drives.  It is possible to short-circuit the protocol
selection process and force the Microsoft client to use \[MS-RDPEUSB\]
by adding a key under UsbSelectDeviceByInterfaces.  See the Knowledge
Base article KB 2653326 “Some USB devices are not available via RemoteFX
USB redirection” at <http://support.microsoft.com/kb/2653326>.

While Windows comes in x86 and x64 variants, these instructions were
developed with x64 in mind.  This is the harder-case scenario as x64
requires all drivers to be signed.  Some of these instructions can be
skipped in x86, but signing drivers is still good practice for all
platforms.  By focusing on x64, this ensures that this paper is as
complete as possible.  Further, all tools cited in this guide are 64 bit
builds.  If you are using x86, adjust these instructions accordingly.

USB device redirection is possible using Windows 7 SP1 or Windows 8. 
However, these instructions assume the server-side (the machine that is
the remote machine) is Windows 8.  Prior to Windows 8, an entire
RemoteFX environment had to be constructed to enable USB device
redirection.  Therefore, just two machines are necessary: the Windows 8
remote machine and either a Windows 7 SP1 or a Windows 8 machine as the
RDP client (or, eventually, your RDP client).

While Windows 8 was the remote system, I used a Windows 7 SP1 box as my
RDP client.  If you use Windows 8 as your client, too, I add some
special notes where appropriate.  Lastly, I used physical machines for
both.  This is because I connected the OSR USB FX2 test board to the
remote machine first to ensure that it is working (driver loaded, test
application works, etc.) before moving the device to the RDP client.

WHAT YOU NEED:

\-- A development machine: Any Windows machine capable of running the
Windows Driver Kit (WDK).  This can be one of the two test machines
below (“server side” or “client side”) or a third machine.  This paper
will assume it’s a third, independent machine.  
\-- Server side machine: Windows 8 x64 Enterprise.  (I used the Windows
8 “Consumer Preview” build 8250)  
\-- Client side machine: Windows 7 SP1 x64 or Windows 8 x64.  
As noted above: I recommend that both the server-side and client-side
machines be physical boxes because these instructions will include steps
to physically attach the device to each machine.  
\-- Microsoft Network Monitor 3.4.  See
<http://www.microsoft.com/en-us/download/details.aspx?id=4865>  
\-- Updated Network Monitor Parsers (October 2011 or newer).  See
<http://nmparsers.codeplex.com>.  
\-- Network Monitor Decryption Expert NmExpert (October 2011 or newer). 
See <http://nmdecrypt.codeplex.com>.  
\-- An OSR USB FX2 test board.  See <http://osronline.com>.  Navigate to
the Store, then Hardware for Learning and then OSR USB FX2 Learning Kit
(V2).  
\-- The Windows Driver Kit (WDK) version 7.1.0.  See
<http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=11800>.

\==============================================  
DEVELOPMENT MACHINE  
BUILDING THE DRIVER, CREATING CERTIFICATES  
\==============================================

The actions in this section are to be performed on a development
machine.  This can be any Windows machine capable of running the Windows
Driver Kit (WDK).  It can be one of the two test machines (“server side”
or “client side”) or a third machine.  The WDK is all-inclusive; it
contains all the tools to build the driver: compiler, linker, etc.  It
also contains the tools Makecert and Inf2cat, both cited in this guide. 
So, no other development tools are necessary (i.e., Visual Studio is not
needed).

There are three primary actions to be performed: building the driver,
signing the driver and making the package and, lastly, unrelated to the
driver, making a private certificate for RDP communication.

BUILDING THE DRIVER:  
The source code for the OSR USB FX2 board is included as a sample driver
in the Windows Driver Kit (WDK) at
\<WinDDK\>\\\<wdkbuild\>\\src\\usb\\osrusbfx2\\kmdf.   (The Intel 82930
USB test board source is at \<WinDDK\>\\\<wdkbuild\>\\src\\usb\\usbsamp,
but these instructions only consider the OSR board).  The code contains
both the driver (under the Sys folder) and a test application that
exercises the board (under the Exe folder).  Because the sample code is
used to teach developers USB driver development, it is provided in
various stages of development (Step 1, Step 2 … Step 5) and the final
stage.  We only care about the Final stage.

\-- Download and install the WDK from
<http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=11800>. 
This paper describes using the Windows 7 SP1 WDK (WDK 7.1.0).  While
there is a Windows 8 WDK, it is quite different; these instructions do
not cover its use.  
\-- Start a Windows 7 x64 Free build environment (click Start, All
Programs, Windows Driver Kits, \<driver kit version\>, Build
Environments, Windows 7 and then x64 Free Build Environment).  This will
open a Command Prompt window.  
\-- Navigate to \<kit\>\\src\\usb\\osrusbfx2\\kmdf (“cd
src\\usb\\osrusbfx2\\kmdf”).  
\-- Build by running Build.  This will build the test application and
all versions of the driver at various stages (steps) including the Final
stage.  We only care about the Final stage.  
\-- The components of the package you want to copy to the test system
are scattered in several locations.  Make a staging folder
(“c:\\osrusbfx2”, for instance) and copy these files to that
folder:  
src\\usb\\osrusbfx2\\kmdf\\exe\\objfre\_win7\_amd64\\amd64\\osrusbfx2.exe  
src\\usb\\osrusbfx2\\kmdf\\sys\\final\\objfre\_win7\_amd64\\amd64\\osrusbfx2.sys  
src\\usb\\osrusbfx2\\kmdf\\sys\\final\\objfre\_win7\_amd64\\amd64\\osrusbfx2.inf  
\\redist\\wdf\\amd64\\WdfCoInstaller01009.dll

SIGNING THE DRIVER

All drivers for Windows x64 must be signed.  Furthermore, in Windows 8,
all driver packages must have a Catalog file.  For our purposes, we’ll
create our own test signature using Makecert, sign the driver with that
certificate, edit the drivers’ INF to specify the catalog file and then
use Inf2Cat to create the Catalog file.  In the same build environment,
do all of the following from the staging directory (“c:\\osrusbfx2”):  
\-- Edit osrusbfx2.inf.  Change the driver’s date (in DriverVer) to be
today’s date (Inf2Cat will not make a Catalog file unless the date is
changed) and specify the CatalogFile as osrusbfx2.cat:  
DriverVer=05/01/2012,6.1.7600.16385  
CatalogFile=osrusbfx2.cat  
\-- Make a test certificate (this only has to be done ONCE on any given
development machine as it will install the certificate in your
development machine’s certificate store).  Do:  
“MakeCert -r -pe -ss PrivateCertStore -n "CN=Contoso.com(Test)"
testcert.cer”  
You can use any name for the certificate.  I used “Contoso.com(Test)”,
which is installed in this development machine’s private certificate
store and yields the output file testcert.cer in your staging folder
(“c:\\osrusbfx2”).  
\-- Sign the driver using this certificate:  
“signtool sign /v /s PrivateCertStore /n Contoso.com(Test) /t
<http://timestamp.verisign.com/scripts/timestamp.dll> osrusbfx2.sys”  
\-- Create the Catalog File:  
“inf2cat /driver:c:\\osrusbfx2 /os:7\_X64 /verbose”

MAKE A CERTIFICATE FOR RDP COMMUNICATION  
The certificate made above using Makecert was created for code signing. 
You need to make a separate, special certificate acceptable for message
exchange.  This certificate will be used by RDP to encrypt all
communications between the server and the client.  Since this is a
certificate you create, you will possess its private key once you export
it (also below).  That will enable you to decrypt the traffic.  From the
same Build window, do:  
“makecert -r -pe -n "CN=Contoso.com(testcert)"  -eku 1.3.6.1.5.5.7.3.1
-ss my -sr LocalMachine -sky exchange -sp "Microsoft RSA SChannel
Cryptographic Provider" -sy 12”  
The name “Contoso.com(testcert)” is arbitrary; you can specify any
name.  These instructions will assume “Contoso.com(testcert)”.  It
places the certificate in this machine’s Certificate Store under
Certificates (Local Computer), Personal, Certificates.  We must export
it so we can port the certificate around as a file.  Do:  
\-- From an elevated command prompt, run MMC.  
\-- Select File | Add/Remove Snap-in from the menu bar.  
\-- Select Certificates and then Add.  
\-- Specify Computer Account and then Local Computer.  
\-- Expand Console Root, Certificates (Local Computer), Personal,
Certificates and the find the Contoso.com(testcert) you made above.  
\-- Right-click on the certificate, select All Tasks and then Export.  
\-- In the Export Wizard, click Next.  When asked “Export Private Key”,
select “Yes, export the private key”.  
\-- For “Export File Format”, select “Personal Information Exchange” and
tick these two items: “Include all certificates in certification” and
“Export all extended properties”  
\-- Give the file a password.  I use <“P@ssw0rd>” in this example.  
\-- Specify an output filename; I used the name
“Contoso.com(testcert).pfx".  You don’t need an extension; it will
amend .pfx automatically.  Create it in the staging folder
c:\\osrusbfx2.  
You can use this certificate on all RDP servers.  I did the steps above
just once and continuously use the same certificate on every RDP test
engagement.  
Your staging folder (“c:\\osrusbfx2”) should have the following seven
files:  
osrusbfx2.exe  
osrusbfx2.sys  
osrusbfx2.inf  
osrusbfx2.cat  
WdfCoInstaller01009.dll  
Testcert.cer  
Contoso.com(testcert).pfx

\==============================================  
SERVER-SIDE MACHINE (RDP REMOTE MACHINE)  
INSTALLING AND TESTING THE DEVICE; INSTALLING CERTIFICATES  
\==============================================

The \[MS-RDPEUSB\] server-side (RDP remote) machine is a Windows 8
physical machine.  Using a virtual machine is possible, but you won’t be
able to test the device locally and you will have to pre-stage the
driver in the Driver Store (using SetupCopyOEMInf()); this is not
discussed here.

First, copy all the files from the development machine’s staging folder
(c:\\osrusbfx2) to this machine.  Then following actions need to occur
on the RDP remote machine (in summary):  
\-- Prepare the machine by allowing test signatures.  
\-- Install the test signature.  
\-- Connect the USB device and install the driver.  
\-- Test the device.  
\-- Enable RDP, install the certificate for RDP communications and
disable compression  
Each topic is discussed below.

PREPARE THE MACHINE BY ALLOWING TEST SIGNATURES

Since we’re using a test signature, we have to enable test signing on
the Windows 8 test system.  From an elevated Command Prompt, run:

“Bcdedit /set testsigning on”  
NOTE: If this system is Bitlocked, you will need the Recovery Key when
you reboot  
Reboot the machine after installing the test signature (next section). 
Allowing test signatures will “watermark” this system in the corners of
the display indicating that the system is for testing purposes.

INSTALL THE TEST SIGNATURE  
The certificate used to sign the driver must be installed in the
machine’s Trusted Root Certification Authority store.  There are two
ways to accomplish this:

Option 1: use the Testcert.cer output file when Makecert was executed:  
From an elevated prompt, do:  
\-- Run MMC  
\-- File | Add/Remove Snap-in  
\-- Certificates, Add, specify Computer Account, Local Computer, Finish
and then OK  
\-- Under Certificates (Local Computer), navigate to Trusted Root
Certificates\\Certificates.  
\-- Right-click, All Tasks, Import.  
\-- Store Location: Local Machine (press Next)  
\-- File name: navigate to where testcert.cer was copied, select
testcert.cer and press Next.  
\-- Certificate Store: Place all certificates in the following store:
Trusted Root Certification Authorities.  Press Next  
\-- Press Finish  
Option 2: Import the certificate from the driver file OSRUSBFX2.SYS:  
\-- Find and right-click on the file OSRUSBFX2.SYS, select Properties  
\-- Go to the Digital Signatures tab.  
\-- Click on the certificate “Contoso.com(Test)” and press Details.  
\-- Under Digital Signature Details, select View Certificate.  
\-- Under Certificate Information, press Install Certificate.  
\-- In the Certificate Import Wizard, select the Store location Local
Machine, press Next.  
\-- When asked for Certificate Store, select “Place all certificates inb
the following store” and browse to Trusted Root Certification
Authorities.  Press OK, then Next and then Finish.  
Reboot the machine.

CONNECT THE USB DEVICE AND INSTALL THE DRIVER  
Connect the device.  Because the driver is not “in-box” the driver will
not be automatically installed (this can be overcome by staging the
driver package via SetupCopyOEMInf(), not discussed here).  We’ll
resolve this by installing the driver manually.  Do the following:  
\-- Run Device Manager.  The OSR USB FX2 LK device will be “yellow
banged” under Other devices.  
\-- Right-click on OSR USB FX2 LK and then select Update Driver
Software.  
\-- For “How do you want to search for driver software?” specify “Browse
my computer for driver software”.  
\-- Browse to the location where the driver package was copied.   Press
Next.  
\-- Windows Security will report that the publisher of the driver cannot
be trusted.  Select “Install anyway”.  
It will now show up under Sample Device.

Hint: to debug device installation problems, the log file at
c:\\windows\\inf\\setupapi.dev.log is very useful.  Open the log file
using Notepad.  This log file is an on-going journal, appended every
time Setup is invoked to install a device.  Since it can get quite
large, it is often helpful to delete (or rename) this file first.  If
there are any problems installing the device, look to this log.

Also: if you find that your \[MS-RDPEUSB\] Add Device call results in
silence from the server-side, it is probably because the driver did not
install, load or start. C:\\windows\\inf\\setupapi.dev.log will be very
useful in determining why.

TEST THE DEVICE:  
\-- Run the test application osrusbfx2.exe using the switch p
(“osrusbfx2.exe –p”).  
\-- It will produce a menu allowing, for instance, you to enter 3 to
light the entire bar graph (on the device) or 4 to clear the bar
graph.  
\-- Once you’re satisfied the device works, exit the application. 
Remove the device.

ENABLE RDP, INSTALL THE CERTIFICATE FOR RDP COMMUNICATIONS AND DISABLE
COMPRESSION

Enable RDP:  
Before you can remote into this machine, you must enable Remote
Desktop.  To facilitate decrypting packets, you must use TLS
(specifically, TLS 1.0 or 1.1, not TLS 1.2).  Further, you must install
the certificate you created for RDP communications, give NETWORK SERVICE
access to it and instruct RDP to use that certificate instead of its
default certificate.  You must disable server-to-client compression. 
Lastly, there are some special platform-specific instructions.

To enable Remote Desktop, do:  
\-- Right-click on My Computer, select Properties  
\-- On the left-hand menu, select Remote Settings.  
\-- Under Remote Desktop, select “Allow connections from computers
running any version of Remote Desktop (less secure)”.  Do NOT tick (or,
un-tick if necessary) “Allow connections only from computers running
Remote Desktop with Network Level Authentication”.  This will use TLS
instead of NLA (Netmon’s decryption expert, NmDecrypt, will only decrypt
TLS 1.0 and TLS 1.1 conversations.  
\-- Press OK and close System Properties.  
Install the certificate for RDP communications:  
Earlier, on the development machine, you created and exported the
certificate Contoso.com(testcert).pfx.  You must import it to this
server test system, give the NETWORK SERVICE access and instruct RDP to
use it instead of its default certificate.

Import the Private Certificate:

The technique to import the certificate is different between Windows 8
and Windows 7.  See the instructions below that apply to you.  
WINDOWS 8 INSTRUCTIONS:  
\-- Double-click on Contoso.com(testcert).pfx.  
\-- In the Certificate Import Wizard, select Local Machine and then
press Next.  
\-- Confirm the file you want to import (the \<certificate\>.pfx file)
and then press Next.  
\-- Provide the password (<“P@ssw0rd>” in my case).  Also, tick “Mark
this key as exportable”.  Then press Next.  
\-- For Certificate Store, select “Automatically select the certificate
store based on the type of certificate”.  This will put it in
Certificates (Local Computer), Personal, Certificates.  
\-- Press Finish  
WINDOWS 7 INSTRUCTIONS (this is provided for completeness; this
\[MS-RDPEUSB\] test lab assumes that the server machine is Windows 8):  
\-- Run MMC as an Administrator  
\-- Go to File | Add/Remove Snap-in  
\-- Select Certificates, then press Add  
\-- Specify Computer Account and then Local Computer  
\-- Press Finish and then OK.  
\-- In the Control Console, navigate to Console Root, Certificates
(Local Computer), Personal, Certificates.  
\-- Right-click on Certificates, select All Tasks and then Import.  
\-- In Certificate Import Wizard, specify Local Machine  
\-- Browse to the .PFX certificate file (you will have to change the
file type filter from “X.509 Certificate (\*.cer;\*.crt)” to “Personal
Information Exchange (\*.pfc;\*.p12)”.  
\-- Once selected, press Next  
\-- Enter the private key password (<“P@ssw0rd>” in this example) and
also tick “Mark this key as exportable”.  
\-- At Certificate Store, specify “Place all certificates in the
following store” and the store “Personal”.  
\-- Press Next and then Finish.

INSTALL THE CERTIFICATE AND GIVE NETWORK SERVICE ACCESS:  
\-- From an elevated command prompt, run MMC.  
\-- Select File | Add/Remove Snap-in from the menu bar.  
\-- Select Certificates and then Add.  
\-- Specify Computer Account and then Local Computer.  
\-- Expand Console Root, Certificates (Local Computer), Personal,
Certificates and the find the Contoso.com(testcert) you imported
above.  
\-- Right-click on the certificate, select All Tasks and then Manage
Private Keys.  
\-- Under Security, press Add.  Add “NETWORK SERVICE”.  Press Check
Names and then OK.  And OK again to close the “Permissions for
\<certificate\> private keys “ dialog.  
\-- Now, double-click the certificate, select the Details tab and then
scroll down and click on Thumbprint.  
\-- Write this Thumbprint value down.  Mine is 28 2f 65 ba 8a 2f d0 11
0f 1d 12 cc 55 12 f8 98 86 65 64 62.  Yours will be different (but the
same for every machine you import this key to, if you use the same
certificate).  
\-- Close MMC.

INSTRUCT RDP TO USE THIS CERTIFICATE:  
\-- Run Regedit.  
\-- Go to Computer, HKEY\_LOCAL\_MACHINE, System, CurrentControlSet,
Control, Terminal Server, WinStations, RDP-Tcp and add the key
SSLCertificateSHA1Hash (BINARY) and enter the certificate Thumbprint you
recorded above.

DISABLE SERVER-TO-CLIENT COMPRESSION:  
\-- Run GPEdit as an Administrator.  
\-- Navigate to Local Computer Policy, Computer Configuration,
Administrative Templates, Windows Components, Remote Desktop Services,
Remote Desktop Session Host, Remote Session Environment and then click
on the setting “Configure compression for RemoteFX data”.  Edit the
policy.  Set it to Enabled and select the RDP compression algorithm “Do
not use an RDP compression algorithm”.  
\-- Say OK and then exit GPEdit.

OPTIONAL PLATFORM-SPECIFIC STEPS:  
\-- If this server-side machine is Windows 7 (this is provided for
completeness; this \[MS-RDPEUSB\] test lab assumes that the server
machine is Windows 8), also edit these two values also at
HKEY\_LOCAL\_MACHINE\\System\\CurrentControlSet\\Control\\Terminal
Server\\WinStations\\RDP-Tcp:  
MinEncryptionLavel = (DWORD) 3  
SecurityLayer = (DWORD) 2

\-- If the client-side machine is Windows 8, you must disable RDP UDP. 
RDP8 (the RDP communications used if both endpoints are Windows 8+) will
make liberal use of UDP in addition to TCP.  This will make capturing
and decrypting the traffic difficult as Network Monitor, which decrypts
traffic per “conversation”, will see these as separate conversations.  
Therefore, do the following if the RDP client will be Windows 8:  
\-- Run Regedit  
\-- Add HKLM\\Software\\Microsoft\\Terminal Server
Client\\DisableUDPTransport (DWORD) = 1  
\-- Close Regedit

  

\==============================================  
CLIENT-SIDE MACHINE (RDP CLIENT MACHINE)  
SET UP THE RDP CLIENT FOR USB DEVICE REDIRECTION  
\==============================================

Important: Do NOT connect the device yet.

The \[MS-RDPEUSB\] client-side (RDP client running MSTSC) machine can be
Windows 7 SP1 or Windows 8.  It must be a physical machine as you will
be connecting the OSR USB FX2 device to it.  The following actions need
to occur on the RDP remote machine (in summary):  
\-- If the client is Windows 8, disable TLS 1.2.  
\-- Enable USB Device Redirection in GPEdit (reboot).  
\-- Connect the device  
\-- Execute Remote Desktop (MSTSC) to specify the device and disable
client-to-server compression  
\-- Connect to the RDP Server and test the device.

DISABLE TLS 1.2 (WINDOWS 8 CLIENTS ONLY)  
\-- If your client machine is Windows 8, you must disable TLS 1.2. 
Windows 8 uses TLS 1.2 by default (whereas it was optional and disabled
by default in Windows 7).  Netmon’s NmDecrypt decryption expert cannot
decrypt TLS 1.2.  The TLS version (1.0, 1.1 or 1.2) is negotiated by the
client, so disabling TLS 1.2 only needs to be performed on the Windows 8
client.  I disable TLS 1.2 and 1.1 (so that TLS 1.0 is used).  
To disable TLS 1.2, add the following registry
keys:

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.2\]

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.2\\Client\]  
"Enabled"=dword:00000000  
"DisabledByDefault"=dword:00000001

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.2\\Server\]  
"Enabled"=dword:00000000  
"DisabledByDefault"=dword:00000001

To disable TLS 1.1, add the following registry
keys:

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.1\]

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.1\\Client\]  
"Enabled"=dword:00000000  
"DisabledByDefault"=dword:00000001

\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SCHANNEL\\Protocols\\TLS
1.1\\Server\]  
"Enabled"=dword:00000000  
"DisabledByDefault"=dword:00000001

ENABLE USB DEVICE REDIRECTION IN GPEDIT:  
Before connecting the device and before any USB device can be
redirected, you must enable the feature in Windows.  Do the following:  
\-- Run GPEdit as an Administrator  
\-- Navigate to Local Computer Policy, Computer Configuration,
Administrative Templates, Windows Components, Remote Desktop Services,
Remote Desktop Connection Client, RemoteFX USB Device Redirection and
edit the setting “Allow RDP redirection of other supported RemoteFX USB
devices from this computer”.  
\-- Select Enabled and then specify Administrators and Users.  
\-- Reboot the machine (this will load the USB filter driver
TsUsbHub.sys that allows RDP to see connected USB devices and switch its
presence between the local machine and the remote machine).

CONNECT THE DEVICE:

After rebooting, you can now attach the device.  It is not necessary to
install the driver for the device on the client system, unless you
intend to use the device on the local system.

EXECUTE REMOTE DESKTOP (MSTSC), SPECIFY THE DEVICE AND DISABLE
CLIENT-TO-SERVER COMPRESSION:

\-- Run MSTSC.EXE (Start, All Programs, Accessories, Remote Desktop
Connection).  
\-- Specify the remote system’s name or IP Address.  
\-- Expand Options.  
\-- Go to the Local Resources Tab.  
\-- Under Local devices and resources, select More…  
\-- There should be the option (with tick mark) “Other supported
RemoteFX USB devices”.  Expand that option.  
\-- The OSR USB-FX2 LK device should also be listed.  Tick it.  If it is
not, you probably connected the device BEFORE enabling Remote USB device
redirection via GPEdit.  
\-- With OSR USB-FX2 LK ticked, press OK.  
\-- Go back to the General Tab, press Save As under Connection
Settings.  
\-- Save the RDP connection details somewhere (i.e. the desktop) as a
.RDP file.  
\-- Exit MSTSC (do not press connect).  
\-- Find the .RDP file created above, right-click, select “Open with”
and specify Notepad (also un-tick the option “Always use the selected
program to open this kind of file”.  
\-- In Notepad, find the entry “compression:i:1” and change it to
“compression:i:0”  
\-- Save the file (as test.rdp, for example).

CONNECT TO THE RDP SERVER AND TEST THE DEVICE  
To connect to the remote system with the OSR FX2 device redirected with
compression off, just double-click on the .RDP file (in the above
example, just double-click on test.rdp).  Remote Desktop will complain
that the certificate is not trusted.  This is because you are using a
self-created certificate.  Just acknowledge these warnings.

\-- Once connected to the remote system, verify that the device appears
in Device Manager (it may take a few seconds as the client sends
\[MS-RDPEUSB\] Add Device and the remote system discovers the device and
loads its driver.  
\-- You can now use the device.  Run the test application OSRUSBFX2.EXE
(osrusbfx2.exe –p).  Verify that the device behaves the same as when it
was locally connected.

\==============================================  
INSTALLING NETWORK MONITOR AND  
CAPTURING, DECRYPTING AND PARSING RDP TRAFFIC  
\==============================================

SETTING UP NETMON:

You can install Network Monitor (Netmon) on either the client side
(system running Remote Desktop hosting the device) or server side (the
remote system that hosts the device’s driver and test application).  I
install it on the client side.  The important point is that you must
capture the RDP trace from the beginning of the RDP session so that the
TLS handshake is captured.

Presently, setting up Netmon is a multi-tiered process as several fixes
were made specific to this effort.  The first step is to install the
base Network Monitor 3.4 package.  Then, you need to overlay the latest
parsers (October 2011 or newer).  And, lastly, install the Decryption
Expert, NmDecrypt (October 2011 or later).

INSTALLING NETWORK MONITOR

\-- Download Network Monitor 3.4 (nm34\_x64.exe) from
<http://www.microsoft.com/en-us/download/details.aspx?id=4865>.  
\-- Run NM34\_x64.EXE and follow the prompts to install the complete
package.  This will automatically invoke the installer for the parsers,
too (which you will see in two separate stages).  
\-- The parsers installed with Netmon 3.4 have problems with RDP; you
must update the package.  Download the updated Network Monitor Parser
package from Codeplex at <http://nmparsers.codeplex.com>.  
\-- Double-click on NetworkMonitor\_Parsers\_x64.msi to install the
package.  When asked, select Yes to overwrite the existing parser
package.  
\-- Download Network Monitor’s decryption expert, NmDecrypt at
<http://nmdecrypt.codeplex.com>.  
\-- Double-click on NetmonDecryptionExpert\_x64.msi to install the
expert.  
\-- Windows 8 Note: NmDecrypt requires .Net Framework 3.5, which is not
included automatically in Windows 8.  If you are using the decryption
expert in Windows 8, you must install this package.  There are two
options:  
Option 1: You are connected to the Internet:  
\-- Run Control Panel  
\-- Click on Programs  
\-- Click on Turn Windows Features On or Off  
\-- Check .NET Framework 3.5 (includes .NET 2.0 and 3.0).  
\-- After pressing OK, the feature will be downloaded from Windows
Update.  
Option 2: You are NOT connected to the internet:  
\-- With your Windows 8 installation media available and running an
elevated command prompt, run “dism /online /enable-feature:NetFx3 /all
/source:d:\\sources\\sxs”, assuming that your DVD media for Windows 8 is
available as D: (adjust the command line, as necessary).  
\-- By default, the Network Monitor installation placed a short-cut on
your desktop.  Run Netmon as an administrator (On Windows 7, Netmon must
be run as an administrator the first time in order to install its
capturing driver; thereafter, administrator is not necessary.  On
Windows 8, Netmon must be run as an Administrator every time).  
\-- On Netmon’s menu bar, select Parser Profiles, Network Monitor
Parsers and then select “Windows”, instead of “Default”.  This only has
to be done once.  
\-- Network Monitor is now installed and configured.

CAPTURING RDP TRAFFIC:  
\-- Run Netmon as an administrator.  
\-- On Netmon’s main screen, select “Create: Net capture tab”  
\-- Before connecting to the remote system, press Start on Netmon’s menu
bar.  You may have to wait for the capture engine to start.  
\-- Connect to the remote system (by double-clicking on the .RDP file
made previously, “test.rdp” for example).  
\-- Once you captured the events of interest, you can stop Netmon by
pressing the Stop button.  
Note: Netmon is capturing ALL RDP traffic.  Keep traffic to a minimum to
keep the trace as small as possible.  Limit mouse updates, keyboard
input and screen refreshes as much as possible.  Keeping the trace small
will shorten the time it takes to decrypt the traffic.  
\-- Save the trace (press Save As) and close Network Monitor (saving and
closing is an important step for decrypting).  Give it a name, like
“test”; it will automatically append “.cap” to make “test.cap”.

DECRYPTING TRAFFIC:  
Network Monitor tracks conversations (traffic with the same
source/destination addresses, protocols and ports).  The Decryption
Expert operates by decrypting a specific conversation.  Therefore,
before you can invoke the expert, you must identify the RDP
conversation.  The input to NmDecrypt is the raw capture.  Its output is
a new capture file.  Do the following:  
\-- Open the capture file (using the example above, double-click on
“test.cap”).  
\-- If the Network Conversations pane is not visible, select View |
Network Conversations from the menu bar.  
\-- If the trace was taken by the server-side, the RDP conversation is
probably listed under svchost.exe.  If it was captured on the client
side, it is probably listed under mstsc.exe.  Expand each conversation
(below svchost or mstsc and below IPv4 or IPv6) to the TCP leaf.  At the
bottom of Network Monitor’s screen are statistics that show how many
frames are presently displayed and how many were captured.  Assuming you
were in a private network (only these two machines), the TCP
conversation with the majority of the displayed frames will likely be
the RDP traffic of interest.  
\-- Select (single-click) that TCP conversation (in the Network
Conversations pane).  
\-- From the menu bar, select Experts, NmDecrypt and then Run Expert.  
\-- The NmDecrypt Expert dialog will appear with the Encrypted Capture
File already identified.  
\-- Enter the Server Certificate Path.  Press Browse and locate the .PFX
file that you imported to the RDP Server (“Contoso.com(testcert).pfx” in
our example).  
\-- Enter the Certificate Password (<“P@ssw0rd>” in our example).  
\-- Specify an output (Decrypted) file path.  I usually use a variation
of the input encrypted capture file name.  So, if the encrypted file is
test.cap, I’ll specify test\_decrypted.cap for the output.  
\-- You can specify a Debug Log File Path (.txt file).  I usually skip
this unless there is a failure, in which case I rerun the expert and
specify a log file for further analysis.

PARSING/VIEWING DECRYPTED RDP TRAFFIC:  
Once the Decryption Expert is finished, the trace will open in a second
instance of Network Monitor.  You will find that all traffic is
duplicated: each frame will appear as encrypted and decrypted.  The
encrypted frames will have Ethernet frame headers.  Decrypted frames
will have “DecryptedPayloadHeaders”.  To make the encrypted frames drop
out (leaving just decrypted frames), enter “DecryptedPayloadHeader” in
the Display Filter and press Apply.  If you want to refine the filter to
only show decrypted RDP traffic, enter “DecryptedPayloadHeader and RDP”
as the filter and press Apply.  If you only want to see only
\[MS-RDPEUSB\] traffic, enter “RDPEUSB” as the Display Filter and press
Apply (you can drop “DecryptedPayloadHeader” from your filter when a
specific protocol is cited).

Known issues.

\-- NmDecrypt has a limit of 2G.  If this limit is exceeded, it will use
a circular buffer, overwriting early frames.  This will appear as a
corrupted capture file.  A version of NmDecrypt is available with a
higher limit.  
\-- Not all RDP protocols have parsers.  \[MS-RDPEECO’ “Echo”, for
instance, does not.  However, “Echo”, and others, rely on
\[MS-RDPEDYC\].  Using a Display Filter using Load Filter, Standard
Filter, Search Frame and specifying the virtual channel’s name, “ECHO”
in this case, as in “ContainsBin(FrameData, ASCII, “EHCO”), you will
find the Dynamic VC Create Request PDU for that channel and its
ChannelId.  From that, you can build a filter for just that channel.

References.

\[MS-RDPEUSB\]
<http://msdn.microsoft.com/en-us/library/ff635589(v=PROT.10).aspx>  
Open Specifications Support Team Blog:
<http://blogs.msdn.com/b/openspecification/>  
Open Specifications Forums:
<http://blogs.msdn.com/b/openspecification/>  
Open Specifications Forum/Windows Protocols:
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols/threads>  
Introducing Microsoft RemoteFX USB Redirection Parts 1, 2 and 3:
<http://blogs.msdn.com/b/rds/archive/2010/06/10/introducing-microsoft-remotefx-usb-redirection-part-1.aspx>  
<http://blogs.msdn.com/b/rds/archive/2010/06/10/introducing-microsoft-remotefx-usb-redirection-part-2.aspx>  
<http://blogs.msdn.com/b/rds/archive/2010/06/10/introducing-microsoft-remotefx-usb-redirection-part-3.aspx>  
Remote Desktop Services team blog: <http://blogs.msdn.com/b/rds/>  
Network Monitor Blog: <http://blogs.technet.com/b/netmon/>

  

**NOTE: Questions and comments are welcome.  However, please DO NOT post
a comment using  
the comment tool at the end of this post.   
Instead, post a new thread in the Open Specifications Forum: Windows  
Protocols at**
[**http://social.msdn.microsoft.com/Forums/en-US/os\_windowsprotocols**](http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols)**.**

</div>

</div>
