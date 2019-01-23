<div id="page">

# Hitchhiker’s Guide to Debugging RDP protocols: Part 2

[Bryan S.
Burgin](https://social.msdn.microsoft.com/profile/Bryan%20S.%20Burgin)
7/24/2012 3:17:00 PM

-----

<div id="content">

Hitchhiker’s Guide to Debugging RDP protocols: Part 2

**NOTE: Questions and comments are welcome.  However, please DO NOT post
a comment using the comment tool at the end of this post.  Instead, post
a new thread in the Open Specifications Forum: Windows Protocols at
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols>.**

\-- Introduction  
\-- Create your own certificate for RDP communications  
\-- Setting up the server machine (RDP remote system).  
\-- Setting up the RDP client  
\-- Installing Network Monitor and Capturing, Decrypting and Parsing RDP
Traffic  
   
\==============================================  
INTRODUCTION  
\==============================================

The purpose of this paper is to provide a guide to capture and decrypt
RDP traffic.  RDP traffic is generally encrypted and compressed, making
it difficult to see.  The technique discussed here is to provide a
self-created certificate for encryption and to disable compression.  By
making your own certificate, you also have the keys to decrypt the
traffic.

This information was originally published in a much larger work that was
specific to debugging \[MS-RDPEUSB\] (USB device redirection over a RDP
connection) as “Hitchhiker’s Guide to Debugging RDP protocols: Part 1
\[MS-RDPEUSB\]” at
<http://blogs.msdn.com/b/openspecification/archive/2012/05/24/hitchhiker-s-guide-to-debugging-rdp-protocols-part-1-ms-rdpeusb.aspx>. 
This pared down version removes all the USB device information to make
the process more readable in generic RDP protocol cases.

The audience for this paper is the RDP protocol implementer.  That is, a
third-party that is developing a non-Window RDP client.  The technique
provided here will enable such a developer to set up a
Windows-to-Windows environment and observe the protocols (RDP is a
family of protocols) in action without being obscured by compression or
encryption.  This will provide a basis for comparison to the developer’s
own RDP client implementation.

WHAT YOU NEED:

\-- Server side machine: This is the machine that is the “Remote”
machine.  
\-- Client side machine: This is the machine that is the RDP client
running mstsc.exe (Remote Desktop Connection)  
Note: Either or both of the above can be virtual systems, unless part of
your test involves remoting hardware (smart cards, etc.), in which case
you may need a physical machine.  
\-- Microsoft Network Monitor 3.4.  See
<http://www.microsoft.com/en-us/download/details.aspx?id=4865>  
\-- Updated Network Monitor Parsers (October 2011 or newer).  See
<http://nmparsers.codeplex.com>.  
\-- Network Monitor Decryption Expert NmExpert (October 2011 or newer). 
See <http://nmdecrypt.codeplex.com>.  
\-- The tool MAKECERT.EXE to make your own certificate.  See
<http://msdn.microsoft.com/en-US/library/bfsktky3(v=VS.80).aspx> for
more information.  I obtain this from the Windows Driver Kit (WDK)
version 7.1.0.  See
<http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=11800>.  
But there are other sources including the Software Developers Kit (SDK)
at <http://msdn.microsoft.com/en-us/windowsserver/bb980924.aspx> and
others.

MAKE A CERTIFICATE FOR RDP COMMUNICATION  
Make a certificate acceptable for message exchange.  This certificate
will be used by RDP to encrypt all communications between the server and
the client.  Since this is a certificate you create, you will possess
its private key.  Obtain the tool MAKECERT (see “What you need”,
above).  If you obtained the tool by installing the WDK, it is located
at C:\\WinDDK\\7600.16385.1\\bin\\amd64\\makecert.exe.  Run:

“makecert -r -pe -n "CN=Contoso.com(testcert)"  -eku 1.3.6.1.5.5.7.3.1
-ss my -sr LocalMachine -sky exchange -sp "Microsoft RSA SChannel
Cryptographic Provider" -sy 12”  
The name “Contoso.com(testcert)” is arbitrary; you can specify any
name.  These instructions will assume “Contoso.com(testcert)”.  It
places the certificate in this machine’s Certificate Store under
Certificates (Local Computer), Personal, Certificates.

We must export it so we can port the certificate around as a file. 
Do:  
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
amend .pfx automatically.  Save it somewhere convenient.  
You can use this certificate on all RDP servers.  I did the steps above
just once and continuously use the same certificate on every RDP test
engagement.

\==============================================  
SERVER-SIDE MACHINE (RDP REMOTE MACHINE)  
\==============================================  
Enable RDP:  
Before you can remote into this machine, you must enable Remote
Desktop.  To facilitate decrypting packets, you must use TLS
(specifically, TLS 1.0 or 1.1 -- not TLS 1.2).  Further, you must
install the certificate you created for RDP communications, give NETWORK
SERVICE access to it and instruct RDP to use that certificate instead of
its default certificate.  You must also disable server-to-client
compression.  Lastly, there are some special platform-specific
instructions.

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
If you ran MAKECERT on this machine, the certificate is already in the
right place.  However, if you ran MAKECERT on a different machine you
must import it to this server test system.  The technique to import the
certificate is different between Windows 8 and Windows 7.  See the
instructions below that apply to you.  
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
WINDOWS 7 INSTRUCTIONS:  
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

GIVE NETWORK SERVICE ACCESS TO THE CERTIFICATE; GET THE CERTIFICATE’S
THUMBPRINT (HASH):  
You must give the Network Service access to read the certificate.  You
must do this even if you ran MAKECERT on this machine.  Do the
following:  
\-- From an elevated command prompt, run MMC.  
\-- Select File | Add/Remove Snap-in from the menu bar.  
\-- Select Certificates and then Add.  
\-- Specify Computer Account and then Local Computer.  
\-- Expand Console Root, Certificates (Local Computer), Personal,
Certificates and the find the Contoso.com(testcert).  
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

PLATFORM-SPEIFIC STEPS:

WINDOWS 7:  
\-- If the server-side machine is Windows 7, edit
HKEY\_LOCAL\_MACHINE\\System\\CurrentControlSet\\Control\\Terminal
Server\\WinStations\\RDP-Tcp:  
MinEncryptionLavel = (DWORD) 3  
SecurityLayer = (DWORD) 2

WINDOWS 8:  
\-- If the both the client machine AND the server machine is Windows 8,
you must disable RDP UDP.  RDP8 (the RDP communications used if both
endpoints are Windows 8) will make liberal use of UDP in addition to
TCP.  This will make capturing and decrypting the traffic difficult as
Network Monitor, which decrypts traffic per “conversation”, will see
these as separate conversations.   Do the following:  
\-- Run Regedit  
\-- Create HKLM\\Software\\Microsoft\\Terminal Server
Client\\DisableUDPTransport (DWORD) = 1  
\-- Close Regedit  
\==============================================  
CLIENT-SIDE MACHINE (RDP CLIENT MACHINE)  
\==============================================

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

EXECUTE REMOTE DESKTOP (MSTSC), DISABLE CLIENT-TO-SERVER COMPRESSION:

\-- Run MSTSC.EXE (Start, All Programs, Accessories, Remote Desktop
Connection).  
\-- Specify the remote system’s name or IP Address.  
\-- Save the RDP connection details somewhere (i.e. the desktop) as a
.RDP file.  
\-- Exit MSTSC (do not press connect).  
\-- Find the .RDP file created above, right-click, select “Open with”
and specify Notepad (also un-tick the option “Always use the selected
program to open this kind of file”).  
\-- In Notepad, find the entry “compression:i:1” and change it to
“compression:i:0”  
\-- Save the file (as test.rdp, for example).

CONNECT TO THE RDP SERVER  
To connect to the remote system, just double-click on the .RDP file (in
the above example, just double-click on test.rdp).  Remote Desktop will
complain that the certificate is not trusted.  This is because you are
using a self-created certificate.  Just acknowledge these warnings.

\==============================================  
INSTALLING NETWORK MONITOR AND  
CAPTURING, DECRYPTING AND PARSING RDP TRAFFIC  
\==============================================

SETTING UP NETMON:

You can install Network Monitor (Netmon) on either the client side or
server side.  I install it on the client side.  The important point is
that you must capture the RDP trace from the beginning of the RDP
session so that the TLS handshake is captured.

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
capturing driver; thereafter, administrator is not necessary).  On
Windows 8, Netmon must be run as an Administrator every time.  
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
as the filter and press Apply.  If you only want to see only a specific
RDP protocol (RDPEUSB, for example), enter “RDPEUSB” as the Display
Filter and press Apply (you can drop “DecryptedPayloadHeader” from your
filter when a specific protocol is cited).

Known issues.

\-- NmDecrypt has a limit of 2G.  If this limit is exceeded, it will use
a circular buffer, overwriting early frames.  This will appear as a
corrupted capture file.  A version of NmDecrypt is available with a
higher limit.  
\-- Not all RDP protocols have parsers.  \[MS-RDPEECO\] “Echo”, for
instance, does not.  However, “Echo”, and others, rely on
\[MS-RDPEDYC\].  Using a Display Filter using Load Filter, Standard
Filter, Search Frame and specifying the virtual channel’s name, “ECHO”
in this case, as in “ContainsBin(FrameData, ASCII, “EHCO”), you will
find the Dynamic VC Create Request PDU for that channel and its
ChannelId.  From that, you can build a filter for just that channel.

References.

Open Specifications Support Team Blog:
<http://blogs.msdn.com/b/openspecification/>  
Open Specifications Forums:
<http://blogs.msdn.com/b/openspecification/>  
Open Specifications Forum/Windows Protocols:
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols/threads>  
Remote Desktop Services team blog: <http://blogs.msdn.com/b/rds/>  
Network Monitor Blog: <http://blogs.technet.com/b/netmon/>

 

**NOTE: Questions and comments are welcome.  However, please DO NOT post
a comment using the comment tool at the end of this post.  Instead, post
a new thread in the Open Specifications Forum: Windows Protocols at
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols>.**

****

</div>

</div>
