<div id="page">

# mechListMIC token

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
5/29/2009 12:52:23 PM

-----

<div id="content">

Security can be a tricky business. This is especially true when you want
security on an open network where anyone can see or modify your packet
in transit. Of course, you can encrypt your message and everything would
be fine. Right?

No. The problem is that there are more than one encryption mechanisms
and before you can use any of them, the sender and receiver must agree
on which one they will be using. But since there is no mechanism
selected yet, the negotiation for a mechanism happens in clear text.
Hence, the list sent by a client can be modified by an attacker in the
middle so that subsequent communications can be watched and possibly be
modified by the attacker.

Enter the mechListMIC token. mechListMIC stands for Mechanism List
Message Integrity Code. The purpose of a mechListMIC token is to verify
the mechanism list that was originally sent in clear text. Exchange of
mechListMIC tokens is generally optional (see RFC 4178) and therefore
they are not exchanged frequently. Microsoft’s extension to “The simple
and Protected Generic Security Service Application Program Interface
Negotiation Mechanism” aka SPNEGO makes it mandatory to exchange
mechListMIC tokens, if AES Kerberos ciphers are exchanged between the
client and server when the client/server operating system is Windows
Vista and/or Windows Server 2008. Other conditions are described in RFC
4178 that are not reproduced here for the sake of brevity. One key
condition that governs the generation of mechListMIC tokens is that the
selected mechanism must support per-message integrity protection. If all
conditions are met, until mechListMIC tokens are exchanged, the
negotiation for encryption mechanism is not complete.

AES is short for Advance Encryption System and is one of the many
Encryption types Kerberos can use. The rest of this document will assume
that the security mechanism chosen is Kerberos.

Microsoft implements RFC4537 and negotiation for encryption type (Etype)
is performed through it, where the supported etypes are sent as a part
of Kerberos AP-REQ message in the authenticator. If the encryption type
is not negotiated in AP-REQ (no authenticator is present), the etype
used is one in the ticket in AP-REQ

One scenario where mechListMIC token are exchanged goes like this:

1\. The client sends a NegTokenInit to the server with a list of
Authentication Mechanisms it supports, sorted in order of preference
with most preferred being the first one. In the following example, the
mechanisms suggested by client are

i. MsKerberos

ii. Kerberos

iii. NT LAN Manager
(NTLM)

[![image](images/image_thumb.png
"image")](images/image_2.png)

2\. If the server supports any of the authentication mechanisms, it will
choose the most preferred one that it also supports. It will also send a
NegTokenResp message to the client. In this example, the server supports
MsKerberos:

[![image](images/image_thumb_1.png
"image")](images/image_4.png)

3\. Now that client and server agree on an authentication mechanism,
they will exchange mechanism specific tokens. In this example, they have
agreed on Kerberos. So, the client will send a NegTokenResp containing a
Kerberos AP-REQ
token.

[![image](images/image_thumb_2.png
"image")](images/image_6.png)

4\. The server will process the token and, if everything goes well, will
send a NegTokenResp that will contain a Kerberos AP-REP and a
mechListMIC token - if the etype is AES128 or
AES256.

[![clip\_image008](images/clip_image008_thumb.jpg
"clip_image008")](images/clip_image008_2.jpg)

5\. At this point, Kerberos authentication is complete but mechListMIC
token exchange is not complete. A valid security context will not be
established in the client and the server if this exchange fails. The
client will send a NegTokenResp with its own generated mechListMIC token
and set the NegState to
accept-completed.

[![clip\_image010](images/clip_image010_thumb.jpg
"clip_image010")](images/clip_image010_2.jpg)

6\. This will finish the negotiation and authentication phase. The
regular message exchange between the client and server can now commence.

The client and server code that generated this message exchange is
available at msdn.microsoft.com at the following links:

<http://msdn.microsoft.com/en-us/library/aa379449(VS.85).aspx>

The code samples at the above mentioned link use SSPI with windows
sockets. Generally an application protocol, like SMB2, will send the
SSPI security BLOBs as part of its own protocol messages. In case of
SMB2, the authentication mechanism negotiation will happen in SMB2
Session Setup messages.

</div>

</div>
