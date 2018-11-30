<div id="page">

# A quick look at the new negotiation mechanism (NegoEx) used with SPNEGO in Windows 7

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
6/30/2011 8:55:00
PM

-----

<div id="content">

**<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
is NegoEx ?  Why do we need it
?</span></span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">  
Before Windows 7 was introduced, applications utilize the SPNEGO to
provide the ability to negotiate the authentication support between
Kerberos SSP and NTLM SSP.  SPNEGO itself is also implemented as a
Negotiate SSP that only manages the selection of the authentication
packages based on the configurations of clients, servers and domain
controllers.    Even there are other security packages such as Digest
and  Schannel available, Negotiate SSP was implemented to support the
negotiation only between NTLM and Kerberos.  This makes it impossible
for the application that already uses the Negotiate SSP to use
additional authentication protocols.   In Windows 7,  NegoEx SSP is
introduced to allow for the creation and installation of new negotiable
security support packages without breaking down the existing SPNEGO
negotiation mechanism.    NegoEx is a protocol specified by
</span></span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">draft-zhu-negoex</span>](http://tools.ietf.org/html/draft-zhu-negoex-04)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.  
It is implemented as a new SSP.  The Negotiate SSP was modified to
support NegoEx SSP in addition to NTLM and Kerberos SSPs.   The  NegoEx
SSP in turn loads additional security packages.   During negotiation,
Negotiate SSP will call into NegoEx SSP to find if any of its installed
security package can supported the requested authentication.     Custom
SSP can be implemented and loaded into NegoEx SSP.  In Windows 7, PKU2U
is the only such a SSP loaded by default.  The following diagram can
show the relationship between all the SSPs and
SSPI.</span></span>

![](images/0755.NegoEx.JPG)

**<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
is PKU2U ? 
</span></span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">  
The PKU2U (Public Key Cryptography Based User-to-User) protocol in
Windows 7 and Windows Server 2008 R2 is implemented as a SSP that can be
loaded by NegoEx SSP. The SSP enables peer-to-peer authentication using
online ID, particularly through the Windows 7 media and file sharing
feature called Homegroup, which permits sharing between computers that
are not members of a domain.   Users have to set up online ID
integration in Windows 7 by explicitly linking their Windows user
account to an Online
ID.  </span></span><span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> The
more information about the PKU2U and Online ID can be
found:</span></span>

  - [<span style="color: #0000ff;" color="#0000ff">Draft-zhu-pku2u-09</span>](http://tools.ietf.org/html/draft-zhu-pku2u-09)
  - [<span style="color: #0000ff;" color="#0000ff">Introducing PKU2U in
    Windows</span>](http://technet.microsoft.com/en-us/library/dd560634\(WS.10\).aspx)
  - [<span style="color: #0000ff;" color="#0000ff">Introducing Online
    Identity
    Integration</span>](http://blogs.msdn.com/tiny_mce/plugins/paste/•%09http:/technet.microsoft.com/en-us/library/dd560662\(WS.10\).aspx)
  - [<span style="color: #0000ff;" color="#0000ff">Online Identity
    Integration in Windows
    7</span>](http://technetnepal.net/blogs/gandip/archive/2010/03/02/online-identity-integration-in-windows-7.aspx)

**<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"></span></span>** 

**<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Network
Packets
Analysis:</span></span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
message exchanges of NegoEx are explained in details in 7.1 and 7.2 of 
draft-zhu-negoex-04.  The following are the security tokens in network
packets showing in a typical flow for the authentication used in SMB2. 
I will not repeat in details what have been explained in the
specification, but show what a normal Negoex SSP negotiation will look
like in a network
trace.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">1. 
  The initial token from the acceptor contains meta data and the SSPs it
supports.  In this case, the Kerberos is not available since it is not
in a domain environment, so only NTLM and NegoEx SSP are available for
negotiation.   NegoEx will have the preference.</span></span>

<span style="font-family: Calibri;" data-face="Calibri">SMB2: R  
NEGOTIATE (0x0), GUID={3521747B-D33A-1DBB-468F-AB41EB2D075F}</span>

<span style="font-family: Calibri;" data-face="Calibri">-
securityBlob:   </span>

<span style="font-family: Calibri;" data-face="Calibri">    - GSSAPI:
</span>

<span style="font-family: Calibri;" data-face="Calibri">     -
InitialContextToken: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ApplicationHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ThisMech: SpnegoToken (1.3.6.1.5.5.2)</span>

<span style="font-family: Calibri;" data-face="Calibri">      -
InnerContextToken: 0x1</span>

<span style="font-family: Calibri;" data-face="Calibri">       -
SpnegoToken: 0x1</span>

<span style="font-family: Calibri;" data-face="Calibri">        +
ChoiceTag: </span>

<span style="font-family: Calibri;" data-face="Calibri">        -
NegTokenInit: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">         + Tag0:
</span>

<span style="font-family: Calibri;" data-face="Calibri">         -
MechTypes: Prefer Negoex (1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Calibri;" data-face="Calibri">             +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">             +
MechType: Negoex (1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Calibri;" data-face="Calibri">             +
MechType: NLMP (1.3.6.1.4.1.311.2.2.10)</span>

<span style="font-family: Calibri;" data-face="Calibri">         + Tag2:
</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
OctetStringHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">         -
MechToken: MessageType = MESSAGE\_TYPE\_ACCEPTOR\_META\_DATA,
MessageLength = 152</span>

<span style="font-family: Calibri;" data-face="Calibri">          +
Negoex: MessageType = MESSAGE\_TYPE\_ACCEPTOR\_META\_DATA, MessageLength
= 152</span>

<span style="font-family: Calibri;" data-face="Calibri"> </span>

<span style="font-family: Calibri;" data-face="Calibri">2.      The
initiator also prefers NegoEx and it sends the
 MESSAGE\_TYPE\_INITIATOR\_NEGO, MESSAGE\_TYPE\_INITIATOR\_META\_DATA
and  MESSAGE\_TYPE\_AP\_REQUEST message exchanges to the
acceptor.</span>

<span style="font-family: Calibri;" data-face="Calibri">  SMB2: C  
SESSION SETUP (0x1)</span>

<span style="font-family: Calibri;" data-face="Calibri">   -
securityBlob:   </span>

<span style="font-family: Calibri;" data-face="Calibri">    - GSSAPI:
</span>

<span style="font-family: Calibri;" data-face="Calibri">     -
InitialContextToken: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ApplicationHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ThisMech: SpnegoToken (1.3.6.1.5.5.2)</span>

<span style="font-family: Calibri;" data-face="Calibri">      -
InnerContextToken: 0x1</span>

<span style="font-family: Calibri;" data-face="Calibri">       -
SpnegoToken: 0x1</span>

<span style="font-family: Calibri;" data-face="Calibri">        +
ChoiceTag: </span>

<span style="font-family: Calibri;" data-face="Calibri">        -
NegTokenInit: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">         + Tag0:
</span>

<span style="font-family: Calibri;" data-face="Calibri">         -
MechTypes: Prefer Negoex (1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Calibri;" data-face="Calibri">          +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">          +
MechType: Negoex (1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Calibri;" data-face="Calibri">          +
MechType: NLMP (1.3.6.1.4.1.311.2.2.10)</span>

<span style="font-family: Calibri;" data-face="Calibri">         + Tag2:
</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
OctetStringHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">         -
MechToken: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
1414</span>

<span style="font-family: Calibri;" data-face="Calibri">          -
Negoex: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
1414</span>

<span style="font-family: Calibri;" data-face="Calibri">           +
MessageHeader: MessageType = MESSAGE\_TYPE\_INITIATOR\_NEGO,
MessageLength = 112</span>

<span style="font-family: Calibri;" data-face="Calibri">           +
NegoMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">           +
Payload: </span>

<span style="font-family: Calibri;" data-face="Calibri">           +
MessageHeader: MessageType = MESSAGE\_TYPE\_INITIATOR\_META\_DATA,
MessageLength = 232</span>

<span style="font-family: Calibri;" data-face="Calibri">           +
ExchangeMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">           +
Payload: </span>

<span style="font-family: Calibri;" data-face="Calibri">           +
MessageHeader: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
1414</span>

<span style="font-family: Calibri;" data-face="Calibri">           +
ExchangeMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">           +
Payload:</span>

<span style="font-family: Calibri;" data-face="Calibri"> </span>

<span style="font-family: Calibri;" data-face="Calibri">3.  The acceptor
finally agrees on NegoEx.  And it also sends MESSAGE\_TYPE\_CHALLENGE
and MESSAGE\_TYPE\_ALERT for more round trips of exchanges.</span>

<span style="font-family: Calibri;" data-face="Calibri">SMB2: R  - NT
Status: System - Error, Code = (22) STATUS\_MORE\_PROCESSING\_REQUIRED 
SESSION SETUP (0x1), SessionFlags=0x0</span>

<span style="font-family: Calibri;" data-face="Calibri">   -
securityBlob:   </span>

<span style="font-family: Calibri;" data-face="Calibri">    - GSSAPI:
</span>

<span style="font-family: Calibri;" data-face="Calibri">     -
NegotiationToken: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ChoiceTag: </span>

<span style="font-family: Calibri;" data-face="Calibri">      -
NegTokenResp: </span>

<span style="font-family: Calibri;" data-face="Calibri">       +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag0:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
NegState: accept-incomplete (1)</span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag1:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
SupportedMech: Negoex (1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag2:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
OctetStringHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       -
ResponseToken: MessageType = MESSAGE\_TYPE\_ALERT, MessageLength =
92</span>

<span style="font-family: Calibri;" data-face="Calibri">        -
Negoex: MessageType = MESSAGE\_TYPE\_ALERT, MessageLength = 92</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
MessageHeader: MessageType = MESSAGE\_TYPE\_CHALLENGE, MessageLength =
162</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
ExchangeMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Payload: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
MessageHeader: MessageType = MESSAGE\_TYPE\_ALERT, MessageLength =
92</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Alert: 0x00000001 - STATUS\_WAIT\_1</span>

<span style="font-family: Calibri;" data-face="Calibri">          
Padding: Binary Large Object (4 Bytes)</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Payload:</span>

<span style="font-family: Calibri;" data-face="Calibri"> </span>

<span style="font-family: Calibri;" data-face="Calibri">4.   The
initiator sends  MESSAGE\_TYPE\_AP\_REQUEST to the acceptor to continue
the message exchanges.</span>

<span style="font-family: Calibri;" data-face="Calibri">   SMB2: C  
SESSION SETUP (0x1)</span>

<span style="font-family: Calibri;" data-face="Calibri">   -
securityBlob:   </span>

<span style="font-family: Calibri;" data-face="Calibri">    - GSSAPI:
</span>

<span style="font-family: Calibri;" data-face="Calibri">     -
NegotiationToken: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ChoiceTag: </span>

<span style="font-family: Calibri;" data-face="Calibri">      -
NegTokenResp: </span>

<span style="font-family: Calibri;" data-face="Calibri">       +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag0:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
NegState: accept-incomplete (1)</span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag2:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
OctetStringHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       -
ResponseToken: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
2248</span>

<span style="font-family: Calibri;" data-face="Calibri">        -
Negoex: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
2248</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
MessageHeader: MessageType = MESSAGE\_TYPE\_AP\_REQUEST, MessageLength =
2248</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
ExchangeMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Payload:</span>

<span style="font-family: Calibri;" data-face="Calibri"> </span>

<span style="font-family: Calibri;" data-face="Calibri">5.  These
message exchanges can continue until the authentication is finalized
when MESSAGE\_TYPE\_VERIFY can be generated using a shared key. 
</span>

<span style="font-family: Calibri;" data-face="Calibri"> </span><span style="font-family: Calibri;" data-face="Calibri">SMB2:
R   SESSION SETUP (0x1), SessionFlags=0x0</span>

<span style="font-family: Calibri;" data-face="Calibri">   -
securityBlob:   </span>

<span style="font-family: Calibri;" data-face="Calibri">    - GSSAPI:
</span>

<span style="font-family: Calibri;" data-face="Calibri">     -
NegotiationToken: </span>

<span style="font-family: Calibri;" data-face="Calibri">      +
ChoiceTag: </span>

<span style="font-family: Calibri;" data-face="Calibri">      -
NegTokenResp: </span>

<span style="font-family: Calibri;" data-face="Calibri">       +
SequenceHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag0:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
NegState: accept-completed (0)</span>

<span style="font-family: Calibri;" data-face="Calibri">       + Tag2:
</span>

<span style="font-family: Calibri;" data-face="Calibri">       +
OctetStringHeader: </span>

<span style="font-family: Calibri;" data-face="Calibri">       -
ResponseToken: MessageType = MESSAGE\_TYPE\_VERIFY, MessageLength =
92</span>

<span style="font-family: Calibri;" data-face="Calibri">        -
Negoex: MessageType = MESSAGE\_TYPE\_VERIFY, MessageLength = 92</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
MessageHeader: MessageType = MESSAGE\_TYPE\_CHALLENGE, MessageLength =
215</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
ExchangeMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Payload: </span>

<span style="font-family: Calibri;" data-face="Calibri">         +
MessageHeader: MessageType = MESSAGE\_TYPE\_VERIFY, MessageLength =
92</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
VerifyMessage: </span>

<span style="font-family: Calibri;" data-face="Calibri">          
Padding: Binary Large Object (4 Bytes)</span>

<span style="font-family: Calibri;" data-face="Calibri">         +
Payload:</span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">  
In this blog,  we took a pratical look at the new authentication
negotiation mechanism used in conjuntion with SPENGO.  Hopefully it can
help you understand how the NegoEx is used while analyzing a network
trace. </span></span>

</div>

</div>
