<div id="page">

# Authentication 101

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
6/24/2011 9:52:27
AM

-----

<div id="content">

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">I
am writing this blog in response to a need I felt when I was new to
authentication in Windows. The blog does not intend to reinvent the
wheel, instead to help someone new to Windows authentication to get
started and find the resources one needs to understand authentication.
I’ll briefly describe concepts; especially the ones that I felt were
not described in plain English and then direct the reader to the
appropriate resource. I am only going to describe network authentication
and will not make any attempt to describe local authentication or
Windows logon process.
</span></span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">What is authentication?</span></span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">In
a nutshell, to prove to a service you really are who you claim to be. A
service is typically running on a separate computer where you want to
access the resources offered by the service, e.g. a file server is a
service and the files are resources. If the owner of the file(s)
restricts the access to a select few, the owner will put in place
restrictions as to who can access the file(s) and what they are allowed
to do with the files. The “who” question is answered by authentication
and the “what” question is answered by
authorization.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">To
make this blog more hands on and less boring, let’s look how
authentication is performed in SMB/SMB2. In SMB/SMB2, authentication
takes place in SESSION SETUP message. If you look at session setup
message in a network trace, you will find something called security
blob. I’ll use netmon trace for this blog (wireshark trace will also
show this info albeit in a different format). </span></span>

  Frame: Number = 137, Captured Frame Length = 1514, MediaType =
ETHERNET

\+ Ethernet: Etype = Internet IP
(IPv4),DestinationAddress:\[00-15-5D-49-34-12\],SourceAddress:\[00-15-5D-49-34-11\]

\+ Ipv4: Src = 192.168.0.201, Dest = 192.168.0.1, Next Protocol = TCP,
Packet ID = 270, Total IP Length = 1500

\+ Tcp: Flags=...A...., SrcPort=49163, DstPort=Microsoft-DS(445),
PayloadLen=1460, Seq=368368040 - 368369500, Ack=2088786732, Win=512
(scale factor 0x8) = 131072

\+ SMBOverTCP: Length = 3148

\- SMB2: C   SESSION SETUP (0x1)

    SMBIdentifier: SMB

  + SMB2Header: C SESSION SETUP (0x1),TID=0x0000, MID=0x0001,
PID=0xFEFF, SID=0x0000

  - CSessionSetup:

     StructureSize: 25 (0x19)

     VcNumber: 0 (0x0)

   + SecurityMode: 2 (0x2)

   + Capabilities: 0x1

     Channel: 0 (0x0)

     SecurityBufferOffset: 88 (0x58)

     SecurityBufferLength: 3060 (0xBF4)

     PreviousSessionId: 0 (0x0)

   +
securityBlob:

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">By
expanding securityBlob, one can see the details of authentication
protocol. As we will see
shortly.</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Authentication
generally does not cause network traffic on its own (Kerberos is a
notable exception and I’ll describe that in a little bit).
Authentication is also optional and only depends on service, e.g. the
most used service in the world, HTTP, does not use authentication
normally. You don’t need to provide a username/password for
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">www.yahoo.com</span>](http://www.yahoo.com/)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">,
for
example.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">As
shown above, securityBlob is an adjunct to a protocol message. When
looking at a network trace of a protocol, authentication can be found in
a message somewhere at the beginning part of the message exchange. A
little knowledge of the protocol is very helpful as every protocol sends
security blob in a message unique to the protocol. For example, in LDAP,
the authentication takes place in bind message.
</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Let’s
expand securityBlob and see what’s inside:</span></span>

<span style="font-family: Courier New;" data-face="Courier New">   -
securityBlob: </span>

<span style="font-family: Courier New;" data-face="Courier New">    +
GSSAPI:</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">So,
what is this GSSAPI? We explore that
next.</span></span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">GSSAPI</span></span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">GSSAPI
stands for Generic Security Services Application Programming Interface.
It is specified in RFC2078, which documents version 2 of this API.
Please consult the RFC for older or newer versions of this API at
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">http://tools.ietf.org/html/rfc2078</span>](http://tools.ietf.org/html/rfc2078)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.
There are two updates to version 2 at the writing of this blog.
</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Microsoft’s
implementation of GSSAPI is called Security Support Provider Interface
(SSPI). The details of SSPI are available at
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">http://msdn.microsoft.com/en-us/library/aa380493(VS.85).aspx</span>](http://msdn.microsoft.com/en-us/library/aa380493\(VS.85\).aspx)

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">GSSAPI
is an attempt to make the job of application designer easier. This goal
is achieved by providing function calls that are independent of the
authentication protocol.
</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">An
application programmer calls the method </span>GSS\_Init\_sec\_context
(<span style="font-size: small;" size="3"> InitializeSecurityContext aka
ISC in SSPI) with the choice of the protocol he/she wants to use e.g.
Kerberos. The call is successful if the return code is either
</span>GSS\_S\_COMPLETE  or GSS\_S\_CONTINUE\_NEEDED.
<span style="font-size: small;" size="3">If
</span>GSS\_S\_CONTINUE\_NEEDED
<span style="font-size: small;" size="3">is returned, the method also
returns a blob that is sent as securityBlob in SESSION SETUP message.
When server receives the session setup messages, it calls the GSSAPI
method </span>GSS\_Accept\_sec\_context
(<span style="font-size: small;" size="3"> AcceptSecurityContext aka ASC
in SSPI). The return code signifies if the authentication is complete.
If the return code is not zero, server sends the securityBlob that ASC
returned to client. The Client calls ISC again with the blob server send
and this goes on till the method calls returns GSS\_S\_COMPLETE which
signals the successful completion of
authentication.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">As
you may have noticed, SSPI method calls have different names than GSSAPI
names e.g. </span>GSS\_Init\_sec\_context is called
InitializeSecurityContext in SSPI. There is a very helpful list
available at [SSPI/Kerberos Interoperability with
GSSAPI](http://msdn.microsoft.com/en-us/library/ms995352.aspx) that
describe a scenario when an SSPI client interoperates with a GSSAPI
server. This does not provide a complete one to one mapping but enough
info that a designer can do the mapping on his/her
own.</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">To
understand SSPI a little better, compile and run sample code called
sample SSPI code at
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">http://msdn.microsoft.com/en-us/library/aa379449(v=VS.85).aspx</span>](http://msdn.microsoft.com/en-us/library/aa379449\(v=VS.85\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
code enables you to select Negotiate (more about this later), NTLM or
Kerberos authentication protocol. By running this application and
observing network traces, much can be learned about SSPI. Also study the
code to see how a typical application uses SSPI. Highly
recommended.</span></span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">SPNEGO</span></span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">If
we expand GSSAPI in the Netmon trace, we can see the authentication
protocol being used by GSSAPI. In this case it is SPNEGO. </span></span>

<span style="font-family: Courier New;" data-face="Courier New">   -
securityBlob: </span>

<span style="font-family: Courier New;" data-face="Courier New">    -
GSSAPI: </span>

<span style="font-family: Courier New;" data-face="Courier New">     -
InitialContextToken: </span>

<span style="font-family: Courier New;" data-face="Courier New">      +
ApplicationHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">      +
ThisMech: SpnegoToken (1.3.6.1.5.5.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">      +
InnerContextToken:
0x1</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
is SPNEGO and how did GSSAPI select this authentication
protocol?</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">Well,
in the GSSAPI call, </span>GSS\_Init\_sec\_context , there is an input
parameter called mech\_type. This specifies the authentication mechanism
(protocol) that GSSAPI will use. While GSSAPI shields the software
designer from the details of authentication protocol, it still requires
the designer to select the authentication protocol e.g. Kerberos, NTLM
etc. This has two disadvantages; the designer needs to know what
authentication protocols are available and once the software is written,
the protocol is fixed. If a new protocol becomes available, in the worst
case the software needs to be modified and recompiled. In a more
reasonable case, some configuration needed to be change to select the
new protocol, if the designer foresaw the possibility of new
protocol.</span>

<span style="font-family: Calibri;" data-face="Calibri">To make the
selection of authentication protocol dynamic, all one need to do it to
use just one protocol, SPNEGO. This seemingly self-contradictory
sentence is actually correct. SPNEGO is an authentication protocol that
is not an authentication protocol in itself. SPNEGO stands for “the
**s**imple and **p**rotected generic security service application
program interface **neg**otiation” and is specified in
</span>[<span style="font-family: Calibri; color: #0000ff;" color="#0000ff" data-face="Calibri">RFC
4178</span>](http://tools.ietf.org/html/rfc4178)<span style="font-family: Calibri;" data-face="Calibri">
.</span>

<span style="font-family: Calibri;" data-face="Calibri">To use SPNEGO,
mech\_type is set to “Negotiate” in SSPI for InitializeSecurityContext
call. In the resulting token that client sends to the server,  SPNEGO
presents a list of authentication mechanisms to the server. The list of
mechanisms is in the order of most desirable to least desirable.</span>

<span style="font-family: Calibri;" data-face="Calibri">Let’s expand the
SPNEGO token in the Netmon trace we are studying in this blog:</span>

<span style="font-family: Courier New;" data-face="Courier New">   -
securityBlob: </span>

<span style="font-family: Courier New;" data-face="Courier New">    -
GSSAPI: </span>

<span style="font-family: Courier New;" data-face="Courier New">     -
InitialContextToken: </span>

<span style="font-family: Courier New;" data-face="Courier New">      +
ApplicationHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">      +
ThisMech: SpnegoToken (1.3.6.1.5.5.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">      -
InnerContextToken: 0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">       -
SpnegoToken: 0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">       
+ ChoiceTag: </span>

<span style="font-family: Courier New;" data-face="Courier New"> 
      - NegTokenInit: </span>

<span style="font-family: Courier New;" data-face="Courier New">        
+ SequenceHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">        
+ Tag0: </span>

<span style="font-family: Courier New;" data-face="Courier New">        
- MechTypes: Prefer MsKerberosToken
(1.2.840.48018.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ SequenceHeader:
</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ MechType: MsKerberosToken
(1.2.840.48018.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ MechType: KerberosToken
(1.2.840.113554.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ MechType: Negoex
(1.3.6.1.4.1.311.2.2.30)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ MechType: NLMP (1.3.6.1.4.1.311.2.2.10)</span>

<span style="font-family: Courier New;" data-face="Courier New">        
+ Tag2: </span>

<span style="font-family: Courier New;" data-face="Courier New">        
+ OctetStringHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">        
+ MechToken:
0x1</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">As
one can see, SPNEGO presents a list of mechanisms (authentication
protocols) that client supports and lists them in the order of
preference with MsKerberos being most desirable and NLMP (NTLM) being
the least desirable in this example. To reduce the number of round
trips, SPNEGO also includes the initial token for the most desirable
mechanism in MechToken field. This is called optimistic token and is
sent in the hope that server will also select the same mechanism as
client.
</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">If
server selects the same protocol as client, the security blob in SESSION
SETUP response will contain a response token for the same protocol and
authentication will continue or complete as per selected
protocol.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">If
the server does not support the most preferred mechanism, it will send
its choice in the SESSION SETUP response and the client will send the
first token of the server selected mechanism and messages will go back
and forth till authentication is
complete.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
important point here is that as new mechanisms become available, list of
mechanism will change dynamically and there is no need to change
software or
provisioning.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">If
you look at the scheme that SPNEGO uses to make mechanism selection
dynamic, you will notice one security concern. What if a malicious
hacker proposes a list of very week authentication protocols with the
intent to authenticate with previously used token that he/she
intercepted?</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">To
make sure that the list of mechanisms is coming from a user with the
knowledge of the credentials, MechListMIC token is used. For details,
please consult the blog
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">mechListMIC
token</span>](http://blogs.msdn.com/b/openspecification/archive/2009/05/29/mechlistmic-token.aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Let’s
expand MechToken and see what’s in there:</span></span>

<span style="font-family: Courier New;" data-face="Courier New">        
- MechToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">         
- MsKerberosToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">          
- KerberosInitToken:
</span>

<span style="font-family: Courier New;" data-face="Courier New">           
+ ApplicationHeader:
</span>

<span style="font-family: Courier New;" data-face="Courier New">           
+ ThisMech: KerberosToken
(1.2.840.113554.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">           
- InnerContextToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">            
- KerberosToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">               
TokId: Krb5ApReq
(0x100)</span>

<span style="font-family: Courier New;" data-face="Courier New">             
- ApReq: KRB\_AP\_REQ
(14)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
     + ApplicationTag:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ SequenceHeader:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ Tag0:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ PvNo:
5</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ Tag1:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ MsgType: KRB\_AP\_REQ
(14)</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ Tag2:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ ApOptions:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ Tag3: </span>

<span style="font-family: Courier New;" data-face="Courier New">  
            + Ticket: Realm: NWTRADERS.MSFT, Sname:
cifs/hyper-v-dc1.nwtraders.msft</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+ Tag4:
</span>

<span style="font-family: Courier New;" data-face="Courier New">              
+
Authenticator:</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">This
is Kerberos AP\_REQ token. If the most preferred protocol were NLMP,
ThisMech would have NTLM Negotiate message in it. For GSSAPI, SPNEGO is
the authentication protocol but inside the SPNEGO token, the messages of
actual authentication protocol are sent.
</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Let’s
see how server responds to the above SESSION SETUP
request:</span></span>

<span style="font-family: Courier New;" data-face="Courier New">   -
securityBlob: </span>

<span style="font-family: Courier New;" data-face="Courier New">    -
GSSAPI: </span>

<span style="font-family: Courier New;" data-face="Courier New">     -
NegotiationToken: </span>

<span style="font-family: Courier New;" data-face="Courier New">      +
ChoiceTag: </span>

<span style="font-family: Courier New;" data-face="Courier New">      -
NegTokenResp: </span>

<span style="font-family: Courier New;" data-face="Courier New">       +
SequenceHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">       +
Tag0: </span>

<span style="font-family: Courier New;" data-face="Courier New">       +
NegState: accept-completed (0)</span>

<span style="font-family: Courier New;" data-face="Courier New">       +
Tag1: </span>

<span style="font-family: Courier New;" data-face="Courier New">       +
SupportedMech: MsKerberosToken (1.2.840.48018.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">       +
Tag2: </span>

<span style="font-family: Courier New;" data-face="Courier New">       +
OctetStringHeader: </span>

<span style="font-family: Courier New;" data-face="Courier New">       -
ResponseToken: 0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">       
- MsKerberosToken: 0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">        
- KerberosInitToken:
</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ ApplicationHeader:
</span>

<span style="font-family: Courier New;" data-face="Courier New">         
+ ThisMech: KerberosToken
(1.2.840.113554.1.2.2)</span>

<span style="font-family: Courier New;" data-face="Courier New">         
- InnerContextToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">          
- KerberosToken:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">             
TokId: Krb5ApRep
(0x200)</span>

<span style="font-family: Courier New;" data-face="Courier New">           
- ApRep: KRB\_AP\_REP
(15)</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ ApplicationTag:
</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ SequenceHeader:
</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ Tag0:
</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ PvNo:
5</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ Tag1:
</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ MsgType: KRB\_AP\_REP
(15)</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+ Tag2:
0x1</span>

<span style="font-family: Courier New;" data-face="Courier New">            
+
AuthorizationData:</span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">This
is Kerberos AP\_REP token which is wrapped in an SPNEGO token. The
response token in SPNEGO is called NegTokenResp and the state of
negotiation is accept-completed. The authentication is now
complete.</span></span>

### <span style="font-size: small;" size="3"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">SPENAGO resources</span></span></span>

[<span style="color: #0000ff;" color="#0000ff">\[RFC4178\]</span>](http://go.microsoft.com/fwlink/?LinkId=90461)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
: The Simple and Protected Generic Security Service Application Program
Interface (GSS-API) Negotiation
Mechanism</span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">\[MS-SPNG\]</span>](http://msdn.microsoft.com/en-us/library/cc247021\(v=PROT.13\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">:
Simple and Protected GSS-API Negotiation Mechanism (SPNEGO)
Extension</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">Kerberos</span></span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Kerberos
is an authentication mechanism that is extensively used in Microsoft
technologies and is the one preferred by Microsoft. Current version
ofKerberos is version 5 and is defined in
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">RFC4120</span>](http://www.ietf.org/rfc/rfc4120.txt)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">.</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">I’ll
not even try to explain Kerberos. The subject is too big and there is
tons of information available on this subject.
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">The
Evolution of Kerberos Authentication
Service</span>](http://www.cs.unibo.it/babaoglu/courses/security/resources/documents/krb_evol.pdf)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">
has a great introduction to Kerberos.  If clicking on the link does not
work, just bing the title and you will find it.
</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">You
may have noticed that the protocol in the network trace is actually
called “MsKerberos” and in the list of mechanisms there are two Kerberos
mechanisms: Kerberos and MsKerboros. So, what’s the difference? Well,
the number you see next to the name of mechanism (1.2.840.113554.1.2.2)
is called OID (object ID). For some reason Microsoft used
1.2.840.113554.1.2.2 for Kerberos instead of the official Kerberos OID
(1.2.840.113554.1.2.2). So the one Microsoft used is called MsKerberos.
By the way, both of them are same and use of either one is
okay.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">Kerberos
is unique in the way that it has its own port (port 88) and it causes
network traffic on its own. This happens when a client get a ticket
granting ticket (TGT) that is subsequently used to get service tickets
from TGS (ticket granting server). So two exchanges, AS\_REQ & AS\_RES
and TGS\_REQ and TGS\_RES are sent on their own and are not sent as a
part of other protocol
message.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">AP\_REQ
& AP\_RES are the messages that are sent as part of messages of other
protocols that use Kerberos for authentication. For example, for
CIFS//SMB/SMB2, AP\_REQ is sent in SESSION SETUP request and AP\_RES is
sent in SESSION SETUP response, as I showed
earlier.</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Microsoft
extensions of Kerberos are specified in
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-KILE</span>](http://msdn.microsoft.com/en-us/library/cc233855\(PROT.13\).aspx)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">.
Extensions are typically additions to protocols that are defined by an
RFC’s. MS-NLMP is not an extension as the document describes the whole
protocol.</span></span>

### <span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria"><span style="font-size: small;" size="3">Kerberos resources</span></span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">RFC4120</span>](http://www.ietf.org/rfc/rfc4120.txt)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;"><span style="color: #0000ff;" color="#0000ff">
</span></span>The Kerberos Network Authentication Service
(V5)</span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">\[MS-KILE\]:
Kerberos Protocol
Extensions</span>](http://msdn.microsoft.com/en-us/library/cc233855\(PROT.13\).aspx)

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">The
Evolution of Kerberos Authentication
Service</span>](http://www.cs.unibo.it/babaoglu/courses/security/resources/documents/krb_evol.pdf)

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">Designing
an Authentication System: a Dialogue in Four
Scenes</span>](http://web.mit.edu/kerberos/www/dialogue.html)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
is an interesting play that describes why’s of
Kerberos.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">There
are tons of resources available on Kerberos but my feeling is that none
of them are geared towards a novice. I found “The Evolution of Kerberos
Authentication Service” to be the most helpful for understanding.
“Designing an Authentication System” story is an honest effort to make
Kerberos easy to understand but I find it rather long. I’ll recommend
reading it at least twice to get something out of
it.</span></span>

## <span style="font-family: Cambria; color: #4f81bd; font-size: medium;" size="4" color="#4f81bd" data-face="Cambria"> </span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">NTLM</span></span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">NTLM
is an old Microsoft authentication protocol that refuses to die. One
major reason is that to use Kerberos, your server and client should be
joined to a domain (also called Kerberos realm). In a situation where
your computers are part of a workgroup, there is no KDC (key
distribution center) and Kerberos cannot be
used.</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">NTLM
stands for NT LAN Manager. NTLM has gone through several changes and as
such has multiple versions. NTLMv1, NTLM with Extended Session Security
and NTLMv2. Sometimes NTLM with extended session security is called
NTLM2 (note there is no v). Before Microsoft made NTLM specification
public as
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">MS-NLMP</span>](http://msdn.microsoft.com/en-us/library/cc236621\(PROT.13\).aspx)<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">,
people had already studied the protocol to the degree where they were
able to write applications that were able to use NTLM to authenticate
against Windows services. The study of NTLM by independent developers
resulted in some documents describing NTLM protocol and along the way
established a terminology for the protocol. This terminology is
different from the terminology that Microsoft uses. NTLM2 is a result of
that. Please consult
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">The
NTLM Authentication Protocol and Security Support
Provider</span>](http://davenport.sourceforge.net/ntlm.html)<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">
to gain an understanding of non-Microsoft NTLM terminology. Such is the
popularity of the above mentioned reference that I wrote a blog that
explains the terminology of MS-NLMP in terms of “The NTLM Authentication
Protocol and Security Support Provider”. The blog is available at
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">NTLM
Terminology: MS-NLMP vs.
http://davenport.sourceforge.net/ntlm.html</span>](file:///C:/Users/obaidf/AppData/Local/Microsoft/Windows/Temporary%20Internet%20Files/Content.Outlook/2NNIWG7I/NTLM%20Terminology:%20MS-NLMP%20vs.%20http:/davenport.sourceforge.net/ntlm.html)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">NTLM
does not rely on a third party (KDC) to authenticate a client to a
server. So, wherever Kerberos cannot be used, authentication falls back
to NTLM. NTLM relies on a shared secret (password) for authentication
that client and authenticating server
share.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">In
NTLM authentication, a client sends a NEGOTIATE\_MESSAGE to server where
client expresses its capabilities in terms of the version and options of
NTLM that it supports. In response to that, server send a
CHELLENGE\_MESSAGE to client which contains a random byte string. Client
uses that challenge in conjunction with its knowledge of user password
to create LM an NT challenge responses and send those with the options
it used to create the response to server in an AUTHENTICATE\_MESSAGE. On
the receipt of AUTHENTICATE\_MESSAGE, server calculates LM and NT
challenge responses based on its knowledge of user password, challenge
and the options client used. If the responses match, authentication
succeeds.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">The
above is a super-simplified picture of NTLM authentication. You should
consult the resources at the end of this section for a deeper
understanding of this
protocol.</span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">NTLM
is also used in domain environment where server and client don’t share a
secret. In this scenario, since server does not know the password of the
user, it sends the challenge response to the domain controller for
verification. This type of authentication is called pass-through
authentication. Pass‑through authentication is described in
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-APDS</span>](http://msdn.microsoft.com/en-us/library/cc223949\(v=PROT.13\).aspx)<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">
and
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-NRPC</span>](http://msdn.microsoft.com/en-us/library/cc237008\(PROT.10\).aspx)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">.</span></span>

### <span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria"><span style="font-size: small;" size="3">NTLM resources</span></span></span>

[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">The
NTLM Authentication Protocol and Security Support
Provider</span>](http://davenport.sourceforge.net/ntlm.html)

[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">MS-NLMP</span>](http://msdn.microsoft.com/en-us/library/cc236621\(PROT.13\).aspx)<span style="text-decoration: underline;"><span style="color: #0000ff;" color="#0000ff"><span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">:
NT LAN Manager (NTLM) Authentication Protocol
Specification</span></span></span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-APDS</span>](http://msdn.microsoft.com/en-us/library/cc223949\(v=PROT.13\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">:
Authentication Protocol Domain Support
Specification</span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-NRPC</span>](http://msdn.microsoft.com/en-us/library/cc237008\(PROT.10\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">:
Netlogon Remote Protocol
Specification</span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">NTLM
Terminology: MS-NLMP vs.
http://davenport.sourceforge.net/ntlm.html</span>](file:///C:/Users/obaidf.000/Documents/NTLM%20Terminology:%20MS-NLMP%20vs.%20http:/davenport.sourceforge.net/ntlm.html)

[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">NTLM
keys and sundry
stuff</span>](http://blogs.msdn.com/b/openspecification/archive/2010/04/20/ntlm-keys-and-sundry-stuff.aspx)

## <span style="font-family: Cambria; color: #4f81bd; font-size: medium;" size="4" color="#4f81bd" data-face="Cambria"> </span>

## <span style="font-size: medium;" size="4"><span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria">Other protocols</span></span></span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">In
addition to Negotiate (SPNEGO), Kerberos and NTLM, there are other
authentication protocols that are used in Microsoft technologies. They
are not as common as the three I mentioned.
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">MS-AUTHSO</span>](http://msdn.microsoft.com/en-us/library/ee392725\(v=PROT.10\).aspx)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">
and MS-SECO are great documents for an overview of all the
authentication protocols used by Microsoft. MS‑AUTHSO and MS-SECO are
overview documents and describe protocols briefly but every protocol has
a reference to the complete specification of the
protocol.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">To
learn about the protocol from an independent perspective, I would highly
recommend “Network Security: Private Communication in a Public World” by
Charlie Kaufman, Radia Perlman & Mike Speciner. The book explains
authentication techniques as well as encryption algorithms. The book is
mathematical in nature but there is a lot that can be learned without a
degree in
mathematics.</span></span>

### <span style="color: #4f81bd;" color="#4f81bd"><span style="font-family: Cambria;" data-face="Cambria"><span style="font-size: small;" size="3">Authentication resources</span></span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">\[KAUFMAN\]
Kaufman, C., Perlman, R., and M. Speciner, "Network Security: Private
Communication in a Public World, Second Edition", Prentice Hall, 2002,
ISBN:
0130460192.</span></span>

[<span style="font-family: Calibri; color: #0000ff; font-size: small;" size="3" color="#0000ff" data-face="Calibri">\[MS-AUTHSO\]:
Windows Authentication Services System
Overview</span>](http://msdn.microsoft.com/en-us/library/ee392725\(PROT.10\).aspx)

[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">\[MS-SECO\]:
Windows Security
Overview</span>](http://msdn.microsoft.com/en-us/library/cc246013\(v=prot.13\).aspx)

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Authentication
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">http://msdn.microsoft.com/en-us/library/aa374735(v=VS.85).aspx</span>](http://msdn.microsoft.com/en-us/library/aa374735\(v=VS.85\).aspx)

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">Authentication
Reference
</span>[<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri">http://msdn.microsoft.com/en-us/library/aa374736(v=VS.85).aspx</span>](http://msdn.microsoft.com/en-us/library/aa374736\(v=VS.85\).aspx)

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

<span style="font-family: Calibri; font-size: small;" size="3" data-face="Calibri"> </span>

</div>

</div>
