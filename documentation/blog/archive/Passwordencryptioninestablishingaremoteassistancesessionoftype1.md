<div id="page">

# Password encryption in establishing a remote assistance session of type 1

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
10/31/2011 9:02:00 AM

-----

<div id="content">

This blog provides details on how the **PassStub** is used when
establishing a **remote assistance session of type 1**. It presents the
**password encryption flow** and illustrates with Windows APIs and
sample data.

 

**Remote assistance overview**

 

Remote Assistance (RA) was introduced in Windows XP and enables one
computer user to remotely help another computer user over the Internet
(KB 300546). The basic concept is that one user, called the
"**Expert**," can view the desktop of another user, the "**Novice**".
This allows the Expert to share control of the Novice's computer to
remotely resolve issues.  
The main steps of Remote Assistance can be described as follows:

  - The Novice invites the Expert to connect to their computer: there
    are three available options, Messenger, email or **saving the
    invitation as a file**. With the email or Save as Invitation
    methods, the Novice will be given the opportunity to **protect the
    session with a password**.
  - The Expert receives the invitation and can initiate the Remote
    Assistance session by supplying a password which the Novice set. The
    password is relayed out-of-band to the Expert.
  - The Novice receives the session request and verifies the password
    that the Expert provided. The Novice also checks the validity period
    of the invitation.
  - If the invitation is open, i.e. not expired, and the password is
    correct, the Novice can then start the Remote Assistance session. At
    this point, the Expert sees the desktop of the Novice’s computer.
    This happens in fact through Terminal Services / Remote Desktop
    Services.

 

**Remote Assistance Invitation file**

The Remote Assistance Initiation Protocol \[MS-RAI\] documents the
format of the Remote Assistance Invitation (RAI) file in Section 6
Appendix A. KB300692 also describes the content of the **RAI file** in
more informative details. The purpose of MS-RAI protocol is to obtain
the Remote Assistance Connection String. **The Remote Assistance
Connection String 1** is used on **Windows XP or Windows Server 2003
Novice**. The Connection String 1 contains the Remote Desktop Protocol
parameters required to establish a Remote Assistance connection, as
documented in MS-RAI Section 2.2.1.

As previously introduced, Windows provides the option to obtain the
connection string by transmitting the RAI file over e-mail as an XML
file. The expert computer parses the RAI File to extract the Remote
Assistance Connection String and other information.

On Windows implementation, an RAI file uses .MsRcIncident as file
extension. In Windows XP, this extension is associated with Help and
Support Center (helpcrt.exe). When the Expert executes the .MsRcIncident
file, Help and Support starts and passes the information to Remote
Assistance, which in turn parses the file and starts the process of
initiating a Remote Assistance session with the Novice computer as
instructed by the parameters in the file.

 

The following is an example of RAI file of the first type:

\<?xml version="1.0" encoding="Unicode" ?\>

\<UPLOADINFO
TYPE="Escalated"\>

\<UPLOADDATA

USERNAME="Administrator"

RCTICKET="65538,1,10.0.3.105:3389;winxpsp3.contoso3.com:3389,\*,rb+v0oPmEISmi8N2zK/vuhgul/ABqlDt6wW0VxMyxK8=,\*,\*,IuaRySSbPDNna4+2mKcsKxsbJFI="

RCTICKETENCRYPTED="1"

DtStart="1314905741"

DtLength="180"

PassStub="RT=0PvIndan52\*"

L="0" /\>

\</UPLOADINFO\>

 

The fields in the RAI file are documented in MS-RAI. Three fields are
particularly of interest for this blog: RCTICKET, RCTICKETENCRYPTED and
PassStub. I will study the PassStub field in more detail and explain its
usage.

 

The RCTICKET field contains the RA Connection String 1 (ref. MS-RAI
Section
2.2.1).

<div>

**RCTICKET**="65538,1,10.0.3.105:3389;winxpsp3.contoso3.com:3389,\*,rb+v0oPmEISmi8N2zK/vuhgul/ABqlDt6wW0VxMyxK8=,\*,\*,IuaRySSbPDNna4+2mKcsKxsbJFI=”

RA Connection String 1 is referred to as the connection string of the
first type.

</div>

The values for the Remote Assistance Connection String 1 entities in the
preceding example are as follows.

<div>

ProtocolVersion =  65538

protocolType = 1

machineAddressList = 10.0.3.105:3389;winxpsp3.contoso3.com:3389

assistantAccountPwd = \*

RASessionID = rb+v0oPmEISmi8N2zK/vuhgul/ABqlDt6wW0VxMyxK8=

RASessionName = \*

RASessionPwd = \*

protocolSpecificParms = IuaRySSbPDNna4+2mKcsKxsbJFI=

</div>

 

**RCTICKETENCRYPTED**="1"  
This field indicates whether or not the Novice set a password during the
creation of the invitation. If it is set to 1, Help and Support center
prompts the Expert for a password. The password entered by the Expert is
sent to the Novice's computer (I will discuss this in more detail). If
it set to "0", the Expert is not prompted for a password.

 

**PassStub=**"RT=0PvIndan52\*"

The field is used for encrypting the password set by the novice
computer. When the RA connection string is sent as a file over e-mail, a
password is used to provide additional security.

 

**PassStub Generation on Windows XP**

 

The function used to generate the Novice PassStub is meant to make its
value unpredictable. For all practical means, this could be arbitrarily
random. MS-RAI originally suggested using CryptEncrypt API
\[MSDN-CRYPTO\] with RA SessionID encrypted with the password key. In
fact, the way the PassStub string is generated does not affect the flow
of the protocol.

The key point here is that, with the same PassStub and same password
key, both RA Novice and RA Expert will produce the same blob.  Recall
that the Expert’s operator must get the password from the Novice user
that is requesting assistance, i.e. by an out-of-band means, e.g. by
phone. In the example of this blog, the password is “Password1“.

Windows XP Novice uses a randomly generated PassStub of 14 ASCII
characters. There are no binary characters. As I mentioned previously, a
Novice should be able to use an arbitrary string for this.

In the example, the RAI file contains PassStub="RT=0PvIndan52\*". How is
this generated?

Helpctr.exe on Windows XP follows these steps to generate the PassStub:

All of the 14 characters are ASCII characters.

Characters 1-2 and 7-14 are from the set A-Z a-z 0-9 \* \_

Character 3 is from the set \!@\#$&^\*()-+=

Character 4 is from the set 0-9

Character 5 is from the set A-Z

Character 6 is from the set a-z

Each character is randomly selected from the respective sets. Using this
model provides certain guarantees about the diversity of resulting
characters in the PassStub string. Keep in mind that the PassStub string
represents the RA Novice PassStub that is sent to the Expert as part of
the RAI file.

 

**Session initialization sequence** **from the Expert**

 

The expert does the following:

  - Obtain the Connection String 1, as specified in MS-RAI (Section
    2.2). In this blog, the file is copied from the Windows XP novice
    computer to a Windows 7 expert computer.
  - Establish TCP connection by using the port and IP Address
    information in Connection String 1; in case of multiple pairs of
    port and IP address, the first successful TCP connection is used.
  - Establish basic connection by using MS-RDPBCGR (Section 3.2) and RDP
    extensions in MS-RA (Section 2.2.7).
  - Perform RA session initialization as documented in MS-RA (Section
    3.3).

The RA session initialization occurs over the RC\_CTL virtual channel.
The RC\_CTL virtual channel persists throughout the duration of the RA
connection. As documented in MS-RA Section 2.2.1.4, the
REMOTEDESKTOP\_CTL\_AUTHENTICATE\_PACKET is the expert authentication
packet. It is only used when the novice or expert is using version 1.
The authentication packet includes the RA connection string and is sent
to the novice.

The Expert encrypts and sends the PassStub in the **expertBlob** field
of the authentication packet. The expertBlob is a Unicode-based set of
PropertyName, PropertyValue pairs, as shown in the following example.
Note that each pair is prefixed with the length of the characters in the
pair, including the equal (=) sign. "NAME" and "**PASS**" are the only
two properties used in expertBlob.

20;NAME=Edgar
Olougouna69;PASS=3C9CAE0BCE7AB15C8AAC01D676045EDF3FFAF092E2DE368A2017E68A0DED7C90

NOTE: Unless you decrypt the network trace from the client, the expert
blob might not be readable in the trace since the traffic of the RDP
virtual channel RC\_CTL is normally encrypted. One alternative to get
the value is to debug the expert.

In the example, the "NAME" property has PropertyValue "Edgar Olougouna",
the encoded value of expertBlob is "20;NAME=Edgar Olougouna".

The “PASS” property is used when a version 1 expert is making a
connection with a RAI file where **RCTICKETENCRYPTED is enabled**. As
shown in the example, the PASS property value is a string and it
contains the result of encryption of the PassStub in the RAI file with
the password key (ref. \[MS-RAI\] section 6).

 

**Password encryption flow and validation for connection string type 1**

 

Here, I walk through the encryption flow and provide relevant Windows
APIs \[MSDN-CRYPTO\].

**1.     ** **Derivation of the password key** 

  - Use the CryptAcquireContext() API to acquire a handle to a key
    container within the PROV\_RSA\_FULL cryptorgtaphic service
    provider.
  - Create the MD5 hash by calling the CryptCreateHash() API with
    CALG\_MD5, the MD5 hashing as algorithm identifier.
  - Create the hash data by calling CryptHashData() API with the
    password in Unocode format. The password in my example is
    “Password1“, which would be in little endian 50 00 61 00 73 00
    73 00 77 00 6f 00 72 00 64 00 31 00.
  - Derive the key by calling CryptDeriveKey() API with CALG\_RC4, the
    RC4 encryption as algorithm identifier.

 

**2.     ** **Encryption of the PassStub data**

  - Encrypt the PassStub with the key by calling CryptEncrypt () API.
    The data to be encrypted is the Novice PassStub sent to the expert
    in the invitation file. The PassStub passed to the CryptEncrypt() is
    in Unicode. The PassStub is also prefixed with a 4 bytes data size,
    this would be 0x1C 00 00 00 for a PassStub of 14 characters (0x1C is
    28 because of the Unicode).

 In my example where PassStub="RT=0PvIndan52\*", the Unicode data passed
to the CryptEncrypt () would translate to:

 1c 00 00 00 52 00 54 00-3d 00 30 00 50 00 76 00 49 00 6e 00 64 00 61
00-6e 00 35 00 32 00 2a 00 

For visual layout:

1c 00 00 00 52 00 54 00-3d 00 30 00 50 00 76 00  ....R.T.=.0.P.v.

49 00 6e 00 64 00 61 00-6e 00 35 00 32 00 2a 00  I.n.d.a.n.5.2.\*.

The output of the CryptEncrypt () call is the blob; that is the property
value for the “PASS” property in the
expertBlob:

**PASS=**3C9CAE0BCE7AB15C8AAC01D676045EDF3FFAF092E2DE368A2017E68A0DED7C90   

 

**3.     ** **Validation ** 

  - The Novice receives the authentication packet from the Expert, it
    generates and compares its encrypted PassStub string to the one
    received in the PASS property value from the Expert. If there is a
    match, and the invitation is not expired, then the Remote Assistance
    session will be established.

 

**References:**

 

\[MS-RA\]: Remote Assistance Protocol Specification

<http://msdn.microsoft.com/en-us/library/cc240013.aspx>

\[MS-RAI\]: Remote Assistance Initiation Protocol Specification****

<http://msdn.microsoft.com/en-us/library/cc240115.aspx>

\[MS-RDPBCGR\]: Remote Desktop Protocol: Basic Connectivity and Graphics
Remoting Specification****

<http://msdn.microsoft.com/en-us/library/cc240445.aspx>****

\[MS-RDPEGDI\]: Remote Desktop Protocol: Graphics Device Interface (GDI)
Acceleration Extensions****

<http://msdn.microsoft.com/en-us/library/cc241537.aspx>

Description of the Remote Assistance Connection Process

<http://support.microsoft.com/kb/300692>

Overview of Remote Assistance in Windows XP****

<http://support.microsoft.com/kb/300546>****

Cryptography Reference \[MSDN-CRYPTO\]  

<http://msdn.microsoft.com/en-us/library/aa380256.aspx>

** **

</div>

</div>
