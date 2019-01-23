<div id="page">

# NTLM Terminology: MS-NLMP vs. http://davenport.sourceforge.net/ntlm.html

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
11/15/2010 8:40:36 AM

-----

<div id="content">

The NTLM Authentication protocol is an old relic. Microsoft, the
inventor of the protocol, itself discourages its use and recommends
using Kerberos.

But I expect NTLM to be around for a long time. The reason is Kerberos’s
use of Key Distribution Center (KDC) and Ticket Granting Server (TGS).
These two entities are generally co-located but even then, this is a
third entity. In case of simple client and server authentication where
the server is not KDC, Kerberos can not be used. Also, another
requirement is that both the client and server should be joined to a
domain. If any one of them is not, NTLM will be used.

Recently I was working on an issue regarding NTLM session security and I
noticed that customer was using terminology for NTLM Authentication
Protocol that is not present in
[**<span style="color: #006bad;">\[MS-NLMP</span>**](http://msdn.microsoft.com/en-us/library/cc236621\(PROT.13\).aspx)\],
the official specification for the NTLM authentication protocol. The
customer was using terminology presented in
[**<span style="color: #006bad;">The NTLM Authentication Protocol and
Security Support
Provider</span>**](http://davenport.sourceforge.net/ntlm.html).

This was not the first time that this had happened. We have had similar
cases in the past. So, to make communication more efficient and
productive, I decided to write this blog where I’ll present terminology
used in [**<span style="color: #006bad;">The NTLM Authentication
Protocol and Security Support
Provider</span>**](http://davenport.sourceforge.net/ntlm.html) and its
equivalent used in \[MS-NLMP\].

For the rest of this blog, I’ll use NTLM.html as an abbreviation for
[**<span style="color: #006bad;">The NTLM Authentication Protocol and
Security Support
Provider</span>**](http://davenport.sourceforge.net/ntlm.html). I’ll not
list the terms that are not present in \[MS-NLMP\] but
intuitive.

 

<span style="text-decoration: underline;">**<span style="font-size: small;">NTLM.html</span>**</span>

</div>

</div>

<span style="text-decoration: underline;"><span style="font-size: small;">**MS-NLMP**</span></span>

Type 1 message

Negotiate message

Type 2 message

Challenge message

Type 3 message

Authenticate message

NTLM2 Session Security

Extended Session Security

NTLM Response

NTLM v1 Response

NTLM2 Session Response

There is no equivalent term in MS-NLMP. This is the LM and NTLMv1
response combined when Extended Session Security is negotiated.

Datagram

Connectionless

LM User Session Key

No Equivalent

NTLM User Session Key

SessionBaseKey

LMv2 User Session Key

No Equivalent

NTLMv2 User Session Key

SessionBaseKey

LM hash

LMOWFv1

NTLM hash

NTOWFv1

NTLMv2 hash

NTOWFv2, LMOWFv2

Lan Manager Session Key

This is calculated as follows:  

ConcatenationOf(DES(LMOWFv1\[0..6\],  
LmChallengeResponse\[0..7\]),  
DES(ConcatenationOf(LMOWFv1\[7\], 0xBDBDBDBDBDBD),  
LmChallengeResponse\[0..7\]))

*Note: I reproduced this here since at the writing of this blog, MS-NLMP
has a bug. The version here is correct and will appear in a future
release.*

This is the sessionBaseKey when only LM authentication is used. If
NTLMSSP\_NEGOTIATE\_KEY\_EXCH is set, this becomes KeyExchangeKey when
NTLMSSP\_NEGOTIATE\_LMKEY flag is set.

Master key

This term is used for the session key that will be used to sign and seal
the messages. This is 16 bit key but depending upon the flags
NTLMSSP\_NEGOTIATE\_56 and NTLMSSP\_NEGOTIATE\_128, a portion of this
key may be used to sign and seal.

secondary base key, secondary key

RandomSessionKey

Client nonce

ClientChallenge

Key Weakening

This term is not mentioned in \[MS-NLMP\] but the process is explained
in section “3.4.5.3 SEALKEY”. This is governed by the flag
NTLMSSP\_NEGOTIATE\_56.  
If set, session key is truncated to 7 bytes, else it is truncated to 5
bytes. These key are extended to 8 bytes. For details, please consult
either document.

User Session Key

This term is not used in MS-NLMP and its closest equivalent is
SessionBaseKey but it should be noted that SessionBaseKey is more
limited concept than the User Session key. User Session Key represents
the initial session key that can be modified later based on the values
of different flags. For details, please consult NTLM.html.

 

Now that you know what LM hash is, a note about session security when
NTLMSSP\_NEGOTIATE\_LMKEY is set.

As explained in Knowledge Base article 299656
([**<span style="color: #006bad;">http://support.microsoft.com/kb/299656</span>**](http://support.microsoft.com/kb/299656 "http://support.microsoft.com/kb/299656")),
newer versions of Windows do not store the LM hash of the password. This
generally does not pose a problem since most modern implementations of
NTLM do not use LAN Manager (LM) authentication. Even when an LM
response is sent, the NTLM response is also sent in the Authenticate
message; servers use that to authenticate (if not, authentication will
fail). But in a rare case, if you are sending both the LM and NTLM
responses and using NTLM session security (NTLMSSP\_NEGOTIATE\_LMKEY is
set), your authentication will succeed but you will not be able to
decrypt the messages from the server and the server will not be able to
decrypt your messages. The reason is that server will be using an LM
hash that is all zeros, since it does not store LM hashes. This will
have an effect on calculation of key exchange keys and eventually on
sealing keys. To avoid this problem, use Extended Session Security.
