<div id="page">

# SMB 2 and SMB 3 security in Windows 10: the anatomy of signing and cryptographic keys

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
5/26/2017 5:11:50 AM

-----

<div id="content">

<div>

Signing is an integral security feature in SMB2 since its inception.
Encryption starts in SMB3 as an important security enhancement. This
article reviews the security evolution of the authenticated session as
well as computation of keys used in SMB 2.x through 3.1.1 dialects. It
provides test vectors for key computation in SMB 3.0 and SMB 3.1.1. The
samples illustrate the multichannel scenario and include step-by-step
key calculation with pre-authentication integrity. Finally, the article
highlights why Kerberos should be preferred over NTLMv2 whenever
possible.

</div>

<div>

</div>

<div>

Table of Contents

</div>

<div>

</div>

<div>

Introduction 2

Windows versions and chronology of dialects in MS-SMB2 3

</div>

<div style="padding-left: 30px">

Deprecating SMB1 3

Dialects in Windows 10 v1703 implementation of MS-SMB2 4

Chronology of crypto algorithms in MS-SMB2 5

</div>

<div>

Security considerations in SMB2 and SMB3 5

</div>

<div style="padding-left: 30px">

Key consideration 5

SessionKey 5

NTLM SessionKey 5

Kerberos SessionKey 6

GSS\_GetMIC() and GSS\_Wrap() or their Ex() extensions 7

</div>

<div>

MS-SMB2 message integrity 7

</div>

<div style="padding-left: 30px">

Signed session 7

Secure dialect negotiation in SMB 3.0.x 8

Pre-authentication integrity in SMB 3.1.1 8

</div>

<div>

Keys in SMB 2.x through 3.1.1 8

</div>

<div style="padding-left: 30px">

Dialects 2.0.2 and 2.1 8

Dialects 3.x 8

Dialects 3.0 and 3.0.2 9

Dialect 3.1.1 9

</div>

<div>

SMB 3.x key derivation function 9

</div>

<div style="padding-left: 30px">

Parameters for Dialects 3.0 and 3.0.2 cryptographic keys 10

Parameters for Dialect 3.1.1 cryptographic keys 10

SMB 3.x Session binding 10

SMB 3.1.1 Session binding and pre-authentication integrity 11

</div>

<div>

Signing algorithms 11

</div>

<div style="padding-left: 30px">

Signing in dialects 2.0.2 and 2.1 11

Signing in dialects 3.0, 3.0.2, 3.1.1 11

</div>

<div>

When is the message signed? 12

</div>

<div style="padding-left: 30px">

Signing negotiation 12

Signing configuration 12

Requiring signing 13

Signing the message 13

Which SigningKey is used? 13

</div>

<div>

When is the message encrypted in SMB 3.x? 14

</div>

<div style="padding-left: 30px">

Encryption negotiation in SMB 3.0 14

Encryption in SMB 3.0: A protocol perspective 14

Encryption negotiation in SMB 3.1.1 14

SMB 3.1.1 Encryption in Windows 10 14

Encryption configuration summary 14

Built-in signing in SMB 3.x encryption 15

</div>

<div>

Conclusion 15

Appendix: Key derivation examples 16

</div>

<div style="padding-left: 30px">

Key derivation example for SMB 3.0 multichannel 16

Key derivation example for SMB 3.1.1 multichannel 17

NTLMv2 SessionKey calculation example 32

</div>

<div>

References 38

</div>

<div>

</div>

# Introduction

<div>

The security model in MS-SMB2 relies upon authenticating the client-user
identity before accessing a share on the server. Once the user is
authenticated, the server may mandate message signing or encryption. The
server also controls access to the share based on which users, groups,
or claims are authorized to have various levels of access. This blog
does not discuss authorization model.

This article focuses on how MS-SMB2 achieves message integrity and
confidentially for the authenticated user. We elaborate on the link as
well as the distinction between the GSS-authenticated user session and
the SMB2 session.

SMB2 message integrity ensures that packets are not tampered or replayed
while in-flight. Signing has always been part of the SMB2 protocol since
its beginning in dialect 2.0.2 (Windows Vista). SMB 3.0 (Windows 8) adds
a new signing algorithm and key calculation.

Confidentiality provides secure data access over untrusted networks by
preventing data inspection. SMB 3.0 introduces end-to-end encryption
built in the protocol. In SMB 3.1.1 (Windows 10), the encryption
algorithm can be negotiated, and the cryptographic key computation is
enhanced with pre-authentication integrity.

This article provides a tutorial regarding SMB2 and SMB3 security
features:

</div>

  - Security considerations in the specification.
  - Signing and encryption operations.
  - Algorithms used for each security feature for a given dialect.
  - Keys in SMB2 and SMB3 protocols.
  - Specifics of key derivation in SMB 3.0.x and 3.1.1.
  - Illustration of key calculation steps for multichannel.
  - Illustration of the (out-of-favor yet) popular NTLMv2 session key
    calculation.

# Windows versions and chronology of dialects in MS-SMB2

<div>

When a new Windows version introduces a new SMB dialect, all previous
dialects are mostly available for backward compatibility with older
versions. Although SMBv1 dialect is still around, it is on the way of
being gradually phased out.

</div>

## Deprecating SMB1

<div>

We have been asking the world to stop using the legacy SMBv1. Recently,
US-CERT issued a security bulletin advising to disable SMBv1 and move to
SMB 2.x or preferably SMB 3.x.

Windows XP and Windows 2003 have SMBv1 only. As you are probably aware,
Windows XP support ended on April 8, 2014. Windows Server 2003 extended
support ended on July 14, 2015. These non-serviced operating systems
still command some footprint and everybody has not migrated yet from
Windows XP and Windows Server 2003.

It will take an industry effort to overcome some of the challenges on
the deprecation path. These challenges include old printers, scanners
and NAS devices.

How to enable and disable SMBv1, SMBv2, and SMBv3 in Windows and Windows
Server

https://support.microsoft.com/en-us/help/2696547/how-to-enable-and-disable-smbv1-smbv2-and-smbv3-in-windows-and-windows-server

The Deprecation of SMB1 – You should be planning to get rid of this old
SMB1
dialect

https://blogs.technet.microsoft.com/josebda/2015/04/21/the-deprecation-of-smb1-you-should-be-planning-to-get-rid-of-this-old-smb-dialect/

https://blogs.technet.microsoft.com/filecab/2016/09/16/stop-using-smb1/

\[US-CERT\] SMB Security Best
Practices

https://www.us-cert.gov/ncas/current-activity/2017/01/16/SMB-Security-Best-Practices

Features Removed or Deprecated in Windows Server 2012 R2

https://technet.microsoft.com/en-us/library/dn303411.aspx

</div>

## Dialects in Windows 10 v1703 implementation of MS-SMB2

<div>

In the following table, as in the rest of this article, we are only
focusing on SMB protocols version 2 and 3 specified in MS-SMB2.

Client      ----   server   ----   dialect

\---------------------------------------------

Vista       ---    2008         ---   2.0.2

7              ---    2008 R2   ---   2.1

8              ---    2012         ---   3.0

8.1          ---    2012 R2    ---   3.0.2

10           ---    2016          ---   3.1.1

For a general discussion on SMB dialects in Windows 8.1 and Server 2012
R2, we encourage the reader to consult the following
blog.

</div>

<div>

https://blogs.technet.microsoft.com/josebda/2013/10/02/windows-server-2012-r2-which-version-of-the-smb-protocol-smb-1-0-smb-2-0-smb-2-1-smb-3-0-or-smb-3-02-are-you-using/

Windows 10 v1703 sends SMB2 Negotiate with supported dialects: 2.0.2,
2.1, 3.0, 3.0.2, 3.1.1 (i.e. Dialects array on the wire: 02 02 10 02 00
03 02 03 11 03). Windows Server 2016 responds with dialect revision:
3.1.1 (i.e. network bytes 11 03), whereas Windows 2012 R2 responds with
dialect revision: 3.0.2 (i.e. 02 03), and so on for other versions.

The dialect is negotiated along with supported capabilities between the
client and server (e.g. signing, encryption) among the capabilities
defined in the scope of that dialect. SMB2 Negotiate is however unsigned
since it occurs prior to authentication. This poses the problem of a
man-in-the-middle (MITM) downgrading the dialect or tampering with the
capabilities. SMB 3.0 attempted to solve this issue with a
post-authentication validation of the Negotiate. SMB 3.1.1 integrated
pre-authentication integrity into all the keys derived for the protocol.

</div>

## Chronology of crypto algorithms in MS-SMB2

<div>

Dialect    ---   Key derivation   ---   Signing ---  
Encryption

\---------------------------------------------------------------------------------------------------------------------------------

2.0.2       ---    GSS-API SessionKey  ---   HMAC-SHA256   ---    N/A

2.1          ---    GSS-API SessionKey   ---   HMAC-SHA256   ---    N/A

3.0          ---    GSS-API SessionKey & KDF \[SP800-108\]  ---  
AES-128-CMAC ---    AES-128-CCM

3.0.2       ---    GSS-API SessionKey & KDF \[SP800-108\]  ---  
AES-128-CMAC ---    AES-128-CCM

3.1.1       ---   SHA-512 & GSS-API SessionKey & KDF \[SP800-108\] 
---   AES-128-CMAC ---  AES-128-CCM and AES-128-GCM

Ref: GSS-API \[RFC2743\]; KDF \[SP800-108\]; SHA-512 \[FIPS180-4\];
HMAC-SHA256 \[FIPS180-2\] and \[RFC2104\]; AES-128-CMAC \[RFC4493\];
AES-CCM and AES-GCM \[RFC5084\]

</div>

# Security considerations in SMB2 and SMB3

## Key consideration

<div>

All cryptographic keys used in SMB 2.x and 3.x are derived from the
SessionKey. Therefore, the security of SMB 2/3 signing and encryption
relies in part on the session key. This key must be unique, kept secret,
and genuinely impossible to guess.

The server should choose an authentication mechanism that provides
unique and randomly generated session keys to ensure the security of the
signing key, encryption key, and decryption key.

</div>

## SessionKey

<div>

The SessionKey is set to the first 16 bytes of the cryptographic key
queried from the GSS protocol (e.g. Kerberos \[MS-KILE\], NTLM
\[MS-NLMP\]) for this authenticated context. If the cryptographic key is
less than 16 bytes, it is right-padded with zero bytes.

Extracting the session key from GSS-API is implementation-dependent. In
SSPI (Microsoft’s implementation of GSS-API), the session key is
extracted by using QueryContextAttributes with
SECPKG\_ATTR\_SESSION\_KEY.

</div>

## NTLM SessionKey

<div>

In NTLM security provider, the SessionKey is the ExportedSessionKey
described in MS-NLMP.

NTLM is a challenge response protocol and its session key is based on
password hashing that relies on weak cryptography.

</div>

  - NTLM does not support any modern cryptographic methods, such as AES
    or SHA-256. Its key derivation from the password is based on MD4
    \[RFC1320\], DES \[FIPS46-2\], HMAC\_MD5, and RC4.
  - NTLM does not use any salt. It is possible to observe a network
    packet trace and produce the same key if the attacker knows
    password, as shown in the appendix by an example of NTLMv2 session
    key calculation.
  - NTLM does not offer mutual authentication between client and server.
    The is no so such flag in the protocol.
  - The password length and complexity should be chosen carefully to
    mitigate brute force attacks.
  - At a minimum, NTLMv2 should be used, and auditing should be in
    place.

<div>

Despite its weakness, NTLM remains a widely-used authentication
mechanism in non-domain environments.

Auditing and restricting NTLM usage guide

https://technet.microsoft.com/en-us/library/jj865674.aspx

See the following blog post for a discussion NTLM
keys.

</div>

<div>

https://blogs.msdn.microsoft.com/openspecification/2010/04/19/ntlm-keys-and-sundry-stuff/

Note: Extended protection for authentication exists for NTLM over TLS.
This binds the TLS session to the NTLM session. Unfortunately, SMB2 does
use extended protection authentication because there is no outer
authenticated session.

NTLM and Channel Binding Hash (aka Extended Protection for
Authentication)

https://blogs.msdn.microsoft.com/openspecification/2013/03/26/ntlm-and-channel-binding-hash-aka-extended-protection-for-authentication/

</div>

## Kerberos SessionKey

<div>

In Kerberos \[RFC4120\] security provider, the SessionKey is either the
sub-session key (subkey) if it was negotiated during
KRB\_AP\_REQ/KRB\_AP\_REP exchange, or the session key from the ticket
if no subkey was negotiated.

When the KDC issues a service ticket, it generates a random session key.
Rather than using a pseudo-random number generator (PRNG), the session
key should use a truly random number generator, such as one based on
measurements of random physical phenomena.  See \[RFC4086\] for an
in-depth discussion of randomness.

For the subkey, it is also strongly encouraged that any key derived by
an application for subsequent use include the full key entropy derived
from the KDC-generated session key carried in the ticket.

The KDC issues tickets with the strongest encryption type (etype) to
protect the session key. The etype is based on configuration of what the
KDC enforces and what client and server principals can support
(AES256\_HMAC\_SHA1, AES128\_HMAC\_SHA1, RC4\_HMAC\_MD5, DES\_CBC\_MD5,
DES\_CBC\_CRC).

</div>

<div>

https://blogs.msdn.microsoft.com/openspecification/2010/11/17/encryption-type-selection-in-kerberos-exchanges/

</div>

<div>

https://blogs.msdn.microsoft.com/openspecification/2011/05/30/windows-configurations-for-kerberos-supported-encryption-type/

https://blogs.technet.microsoft.com/petergu/2013/04/14/interpreting-the-supportedencryptiontypes-registry-key/.

Beyond the etype, the security of the Kerberos SessionKey is
substantially strengthened if all preceding Kerberos exchanges
(\[RFC4120\] or \[MS-KILE\]) that lead to the service ticket have
reinforced protection. For instance, the authentication service may
leverage public key cryptography to protect the initial authentication
(PKInit), as specified in \[RFC4556\], \[MS-PKCA\], e.g. for smart card
authentication.

In its basic form, the security of the Kerberos authentication service
depends on the strength of the password used to protect it.

</div>

## GSS\_GetMIC() and GSS\_Wrap() or their Ex() extensions

<div>

Signing in SMB2 and SMB3 uses their respective algorithms HMAC-SHA256
and AES-128-CMAC. SMB2/3 signing does not use GSS-API GSS\_GetMIC() or
GSS\_GetMICEx().

Likewise, SMB 3.x encryption uses its own encryption algorithms, i.e.
AES-128-CCM and AES-128-GCM as introduced earlier in this article.
GSS-API GSS\_Wrap() or GSS\_WrapEx() are not pertinent for SMB3
encryption.

The GSS-API keys should not be confused with SMB2/3 keys, though the
latter are derived from the session key. In case of NTLM authentication,
NTLM has his own signing and sealing keys, which serve for
GSS\_GetMICEx(), GSS\_VerifyMICEx(), GSS\_WrapEX(), GSS\_UnwrapEX(). The
same remarks apply for Kerberos GSS-API keys, and Kerberos GSS
functions.

</div>

# MS-SMB2 message integrity

<div>

SMB2 message integrity manifests on a signed session where signed
packets flow from client to server. What happens to message integrity
until the session is established?

</div>

## Signed session

<div>

On a signed session, every signed packet includes a signature that the
receiver can validate. Unlike SMB1 signing which uses MD5 \[RFC1321\] as
hashing algorithm, SMB2 uses a better hashing for signing. SMB 2.02 and
SMB 2.1 use HMAC SHA-256, whereas SMB 3.0 upgrades to AES-128-CMAC for
its signing.

Before signing can happen, the client and server need to negotiate
signing; and the client needs authenticate first to have a SessionKey
from which a SigningKey is derived (more details on this later). SMB 2.x
did not have a mechanism to detect MITM downgrading of negotiated
capabilities.

</div>

## Secure dialect negotiation in SMB 3.0.x

<div>

Dialect 3.0 implements secure dialect negotiation (SMB 3.0, 3.02) to
protect against security-downgrade attacks. When SMB 3.0 is negotiated,
the client must send a mandatory signed request to validate the
Negotiate information. The validation occurs post-authentication and
after the Tree Connect
exchange.

http://blogs.msdn.com/b/openspecification/archive/2012/06/28/smb3-secure-dialect-negotiation.aspx

SMB 3.1.1 introduces pre-authentication integrity which supersedes the
SMB 3.0.x secure dialect negotiation.

</div>

## Pre-authentication integrity in SMB 3.1.1

<div>

SMB 3.1.1 pre-authentication integrity enhances protection against
security-downgrade attacks by verifying the integrity of all messages
preceding the session establishment. This mandatory feature protects
against any tampering with Negotiate and Session Setup messages by
leveraging cryptographic hashing SHA-512 \[FIPS180-4\].
Pre-authentication integrity also integrates a salt transmitted on the
wire as part of the hash calculation. In Windows implementation, the
pre-auth-integrity negotiate context has a salt of 32 bytes. The salt is
randomly generated (e.g. PRNG) and not reused, otherwise it would allow
surface
attacks.

https://blogs.msdn.microsoft.com/openspecification/2015/08/11/smb-3-1-1-pre-authentication-integrity-in-windows-10/

</div>

# Keys in SMB 2.x through 3.1.1

## Dialects 2.0.2 and 2.1

## 

<div>

Prior to SMB 3.0, the 16-bytes SessionKey was the primary key.

SigningKey: Session.SigningKey is the same as Session.SessionKey.

Application’s session key for an authenticated context: the client
returns Session.SessionKey.

</div>

## Dialects 3.x

<div>

In SMB 3.x, two new keys are added to serve for encryption. The
application’s session key for an authenticated context has become a key
on its own.

The SMB 3.x keys have the following properties:

</div>

  - Key size: 128-bit i.e. 16 bytes each.

  - Session.SigningKey: Key used for signing the SMB2 messages on this
    session.

  - Channel.SigningKey: Key used for signing the SMB2 messages on this
    channel when session binding is used for multiple channels.
    
    o If the session is setup with SMB2\_SESSION\_FLAG\_BINDING,
    Channel.SigningKey must be generated. Both master session and
    binding session must be from the same security principal, i.e. same
    security identifier (SID).
    
    o If SMB2\_SESSION\_FLAG\_BINDING is not set, Channel.SigningKey is
    set to Session.SigningKey. Note that two security principals can
    still multiplex separate channels on the same connection.

  - Session.ApplicationKey: Key mainly queried by higher-layer
    applications. An example is SAMR protocol over named pipe SMB2
    transport \[MS-SAMR\]. ApplicationKey is generated for non-binding
    sessions.

  - Session.EncryptionKey: Key used by the sender for encrypting
    messages. There is one encryption key for all sessions bound to a
    master session. This enables to send encrypted messages on any
    channel.

  - Session.DecryptionKey: Key used by the receiver for decrypting
    messages. Likewise, the decryption key is the same for all sessions
    bound to a master session. If a decrypted message contains
    compounded related SMB2 messages, these later must all belong to the
    same SessionId.

<div>

Upon authentication, SigningKey, and ApplicationKey are calculated. If
encryption is supported (even if not negotiated), EncryptionKey and
DecryptionKey are computed.

All these keys are derived using the SP800-108 key derivation function
(KDF) (see more details

</div>

<div>

later in this article). SessionKey is the key derivation key (KDK), a
parameter to the KDF.

</div>

## Dialects 3.0 and 3.0.2

<div>

Apart from the KDK input, i.e. SessionKey, the other parameters to the
KDF are constants defined for each type of key. (more details in
subsequent Sections).

</div>

## Dialect 3.1.1

<div>

SessionKey remains the KDF’s KDK. The context parameter is no longer a
constant, instead it is PreauthIntegrityHashValue (see blog “SMB 3.1.1
Pre-authentication Integrity”). The remaining parameters to the KDF are
constant, but they differ from the constants used in SMB 3.0.x key
derivation. (see specifics in subsequent Sections).

</div>

# SMB 3.x key derivation function

<div>

Ko = SMB3KDF (Ki, Label, Context)

SMB3KDF() is defined as the key derivation function (KDF) in Counter
Mode, as specified in \[SP800-108\] section 5.1, with 'r' value of 32
and 'L' value of 128, and HMAC-SHA256 as the PRF.

Ki – The SessionKey is the key derivation key (KDK), used as an input to
the KDF, for SMB 3.x.

Label – the purpose of this derived key, encoded as string and length
for SMB 3.x.

Context – the context information of this derived key, encoded as string
and length for SMB 3.x.

L – An integer that specifies the length of the derived keying material
Ko, L is 128 bits for SMB 3.x cryptographic keys. Note that L is a
constant since all SMB 3.x keys are 16 bytes in length.

Ko – Keying material output from the KDF, a binary string of length L,
where Ko is the leftmost L bits of KDF result.

</div>

## Parameters for Dialects 3.0 and 3.0.2 cryptographic keys

<div>

Label and context are constants for each type of cryptographic key.

</div>

  - SigningKey = SMB3KDF ( SessionKey, "SMB2AESCMAC\\0", "SmbSign\\0")
  - ApplicationKey = SMB3KDF (SessionKey, "SMB2APP\\0", "SmbRpc\\0")
  - EncryptionKey (Client) = DecryptionKey (Server) =  SMB3KDF
    (SessionKey, "SMB2AESCCM\\0", "ServerIn \\0")
  - EncryptionKey (Server) = DecryptionKey (Client) = SMB3KDF
    (SessionKey, "SMB2AESCCM\\0", "ServerOut\\0")

## Parameters for Dialect 3.1.1 cryptographic keys

<div>

A new label is defined for each type of cryptographic key. The context
is Session.PreauthIntegrityHashValue, as described in the “SMB 3.1.1
pre-authentication integrity” blog.

</div>

  - SigningKey = SMB3KDF (SessionKey, "SMBSigningKey\\0",
    Session.PreauthIntegrityHashValue)
  - ApplicationKey = SMB3KDF (SessionKey, "SMBAppKey\\0",
    Session.PreauthIntegrityHashValue)
  - EncryptionKey (Client) = DecryptionKey (Server) = SMB3KDF
    (SessionKey, "SMBC2SCipherKey\\0",
    Session.PreauthIntegrityHashValue)
  - DecryptionKey (Client) = EncryptionKey (Server) = SMB3KDF
    (SessionKey, "SMBS2CCipherKey\\0",
    Session.PreauthIntegrityHashValue)

## SMB 3.x Session binding

<div>

SMB2\_SESSION\_FLAG\_BINDING bit is set in the Flags field of the
Session Setup request.

For session binding to multiple connections (multichannel), the server
needs to call GSS\_Accept\_sec\_context for each additional channel.
This requires creating a new-temporary security context to have a
SessionKey and derive a SigningKey for the binding channel. Note that
the session binding request must be signed with the master session’s
SigningKey, as described later in this article.

However, EncryptionKey, DecryptionKey, and ApplicationKey remain the
same from the main channel, i.e. remain the same for all related
channels.

</div>

## SMB 3.1.1 Session binding and pre-authentication integrity

<div>

If this is a master or binding session setup and the dialect is 3.1.1 or
higher, we allocate and initialize the session's pre-authentication
integrity hash value starting with the connection's pre-authentication
integrity hash. We always start with the pre-authentication hash
resulting from processing Negotiate on that connection.

The client side logic is summarized below.

Set Session.PreauthIntegrityHashValue to
Connection.PreauthIntegrityHashValue.

Update Session.PreauthIntegrityHashValue with the SessionSetup request.

if it's STATUS\_MORE\_PROCESSING\_REQUIRED, update
Session.PreauthIntegrityHashValue with the response, and process
subsequent session setup exchange leg.

if it's STATUS\_SUCCESS, use Session.PreauthIntegrityHashValue to
generate cryptographic keys, then verify the signature of the
SessionSetup response.

For the server side, the logic is similar except that we use
Session.PreauthIntegrityHashValue when SMB2\_SESSION\_FLAG\_BINDING is
NOT set, and

PreauthSession.PreauthIntegrityHashValue when
SMB2\_SESSION\_FLAG\_BINDING is set.

This is important because there is no guarantee in which order session
bindings will be processed. That way, each session setup processing is
isolated from the other, but all use the connection's pre-authentication
hash as the starting point.

</div>

# Signing algorithms

<div>

For a signed message, the sender sets the Signature field and the
SMB2\_FLAGS\_SIGNED bit in the Flags field of the SMB2 header. The
sender calculates the signature as follows.

</div>

## Signing in dialects 2.0.2 and 2.1

<div>

The signer computes a 32-byte hash by using HMAC-SHA256 over the entire
message, beginning with the SMB2 header with the signature field zeroed,
and using the provided SigningKey. The HMAC-SHA256 hash is specified in
\[FIPS180-2\] and \[RFC2104\]

The signature is the first 16-bytes of the hash.

</div>

## Signing in dialects 3.0, 3.0.2, 3.1.1

<div>

The signer computes the 16-byte signature by using AES-128-CMAC over the
entire message, beginning with the SMB2 header with the signature field
zeroed, and using the SigningKey provided. The AES-128-CMAC is specified
in \[RFC4493\].

For dialect 3.x family, the server must sign the final SessionSetup
Response. This excludes guest and anonymous sessions since there is no
SigningKey available in those types of contexts.

Signing of the final SMB 3.x SessionSetup Response:

</div>

  - Connection.Dialect belongs to the 3.x dialect family:
  - If SMB2\_SESSION\_FLAG\_BINDING is set in the SessionSetup, use
    Channel.SigningKey.
  - Otherwise, use Session.SigningKey.

<div>

For Dialect SMB 3.x, the session binding request is signed with the
master session’s SigningKey.

</div>

# When is the message signed?

## Signing negotiation

<div>

The client and server negotiate signing by using the SecurityMode field
in the SMB2 Negotiate request and response exchange. The client also
communicates the SecurityMode in the SessionSetup Request.

SMB2\_NEGOTIATE\_SIGNING\_ENABLED (0x0001)

SMB2\_NEGOTIATE\_SIGNING\_REQUIRED (0x0002)

Windows SMB 2/3 clients always set SMB2\_NEGOTIATE\_SIGNING\_ENABLED
flag in the negotiate request and Windows-based servers set it likewise
in the response since they all support signing.

MS-SMB2 require all SMB 2/3 servers to implement signing, regardless of
whether they set the SMB2\_NEGOTIATE\_SIGNING\_ENABLED flag.

</div>

## Signing configuration

<div>

By default, Windows client and server have the following settings.

Get-SmbClientConfiguration  | fl
EnableSecuritySignature,RequireSecuritySignature

EnableSecuritySignature  : True

RequireSecuritySignature : False

The corresponding registry keys on the client are under
HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters

Get-SmbServerConfiguration | fl
EnableSecuritySignature,RequireSecuritySignature

EnableSecuritySignature  : False

RequireSecuritySignature : False

The corresponding registry keys on the server are under
HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters

For additional details on signing configuration and effective behavior,
please consult the following blog:

The Basics of SMB Signing (covering both SMB1 and
SMB2)

https://blogs.technet.microsoft.com/josebda/2010/12/01/the-basics-of-smb-signing-covering-both-smb1-and-smb2/

</div>

## Requiring signing

<div>

The session will have Session.SigningRequired = TRUE if one the
following conditions is met:

</div>

  - Signing has been required by negotiation.
  - The server configuration requires message signing.
  - The SessionSetup requires signing.

<div>

NOTE: Signing cannot be required for guest or anonymous sessions as they
do not have proper security context.

</div>

## Signing the message

<div>

To sign a message, the first two prerequisites are:

</div>

  - Existence of the signing key: SessionId field has a non-zero value,
    otherwise there would be no possible signing key. The exception is a
    guest or anonymous session, which does not have a signing key even
    there’s a SessionId.
  - Signing requirement: SessionId requires signing, i.e. its
    Session.SigningRequired is equal to TRUE as outlined previously.

<div>

If those two prerequisites are met, the sender signs the message under
these specific conditions:

</div>

  - It’s a TREE\_CONNECT or the message belongs to an un-encrypted
    TreeId, i.e. its TreeConnect.EncryptData is equal to FALSE. If we
    are not encrypting, we begin to sign messages as soon the session is
    established, i.e. the signing key is available and the session
    requires signing.
  - Connection.Dialect is "3.1.1", and it’s a TREE\_CONNECT and
    SessionId is not an encrypted session, i.e. its Session.EncryptData
    is equal to FALSE. If 3.1.1 dialect is negotiated, the protocol
    requires signing or encrypting all TREE\_CONNECT sent in non-guest,
    non-anonymous sessions. When the whole session is not encrypted, we
    always sign the TREE\_CONNECT since all SMB2/3 servers support
    signing.

<div>

There is one notable exception to the “Session.SigningRequired is equal
to TRUE” prerequisite. Irrespective of Session.SigningRequired,
FSCTL\_VALIDATE\_NEGOTIATE\_INFO should always be signed. Note that if
Session.EncryptData is equal to TRUE, the server would not need to
verify the signature of FSCTL\_VALIDATE\_NEGOTIATE\_INFO message (see
Section “Built-in signing in SMB 3.x encryption”).

</div>

## Which SigningKey is used?

<div>

With SMB 2.x, the signing key is Session.SessionKey.

With SMB 3.x, the notions of multichannel and session binding introduce
new rules for whichever of Session.SessionKey or Channel.SigningKey is
used.

</div>

  - For SessionSetup request, the signing key is Session.SessionKey. For
    instance, the signing key for session setup request for session
    binding (SMB2\_SESSION\_FLAG\_BINDING) is Session.SessionKey.
  - For session set up response, if the status code is NOT equal to
    STATUS\_SUCCESS, the signing key is Session.SessionKey. If the
    session setup response has STATUS\_SUCCESS, the signing key is
    Channel.SigningKey.
  - For any other command, the signing key is Channel.SigningKey. Note
    that the session can be multiplexed on multiple channels across
    multiple connections. Since, each channel has their signing key, the
    sender looks up the Channel in Session.ChannelList that matches the
    same connection.

# When is the message encrypted in SMB 3.x?

<div>

A detailed discussion is provided in the blogs mentioned in the
following sub-sections.

</div>

## Encryption negotiation in SMB 3.0

<div>

SMB 3.0.x client and server advertise encryption support via the
SMB2\_GLOBAL\_CAP\_ENCRYPTION capability flag during the NEGOTIATE. If
encryption is enforced on the whole server, each session enables
encryption by setting SMB2\_SESSION\_FLAG\_ENCRYPT\_DATA in the
SessionFlags of SessionSetup response. For a per-share enabled
encryption, ShareFlags of TreeConnect response includes
SMB2\_SHAREFLAG\_ENCRYPT\_DATA.

SMB 3.0 and SMB 3.0.2 support only the encryption algorithm
AES-128-CCM.

</div>

## Encryption in SMB 3.0: A protocol perspective

<div>

http://blogs.msdn.com/b/openspecification/archive/2012/10/05/encryption-in-smb-3-0-a-protocol-perspective.aspx

</div>

## Encryption negotiation in SMB 3.1.1

<div>

SMB 3.1.1 client and server negotiate encryption support by exchanging
supported cipher IDs 0x0002 (AES-128-GCM) and 0x0001 (AES-128-CCM) in
SMB2\_ENCRYPTION\_CAPABILITIES negotiate context in the NEGOTIATE
request and
response.

</div>

## SMB 3.1.1 Encryption in Windows 10

<div>

https://blogs.msdn.microsoft.com/openspecification/2015/09/09/smb-3-1-1-encryption-in-windows-10/

</div>

## Encryption configuration summary

<div>

As discussed in the abovementioned blogs, the configuration items that
affect encryption on Windows 8 / Windows Server 2012 and onward are:

Global level encryption on the server:

Set-SmbServerConfiguration -EncryptData \<0|1\>

If enabled, SessionSetup.Response.SessionFlags includes
SMB2\_SESSION\_FLAG\_ENCRYPT\_DATA.

Share level encryption:

New-SmbShare -Name \<share name\> -Path \<pathname\> -EncryptData 1

Set-SmbShare -Name \<share name\> -EncryptData \<0|1\>

If enabled, TreeConnect.Response.ShareFlags sets
SMB2\_SHAREFLAG\_ENCRYPT\_DATA.

Server-wide encryption has precedence over share level encryption. If
global encryption is enabled, you cannot selectively disable encryption
for certain shares.

Keep in mind that a dynamic change of the share’s EncryptData property
would mean different encryption experience if existing TreeIds prior to
the change remain connected. New TreeId connections after the change
would reflect the new value of the share’s EncryptData property. Thus,
when the administrator reconfigures Share.EncryptData, is it recommended
to force disconnection of all current connections to allow clients to
pick the new configuration.

Unencrypted access:

Set-SmbServerConfiguration -RejectUnencryptedAccess \<0|1\>

This is for down-level clients. See discussion on security impact in
Section “Unencrypted Access: RejectUnencryptedAccess” of blog entry “SMB
3.1.1 Encryption in Windows 10”.

</div>

## Built-in signing in SMB 3.x encryption

<div>

Integrity (message authentication) is built into the algorithms used for
encryption in SMB 3.x, i.e. AES-128-CCM and AES-128-GCM. These
encryption algorithms incorporate the benefit of a message
authentication code. Successful decryption means that SMB3 crypto
processing has checked the integrity of the encrypted message. The
receiver does not need to validate the signature of the inner SMB2
message itself. In general, signing the SMB2 message before encrypting
it would be a waste of CPU cycles without any additional security.

SMB 3.x encryption supplants regular signing: it does not and should not
turn off message signing. When SMB 3.x encryption is in use, we do not
need regular signing. However, “RejectUnencryptedAccess set to FALSE”
should not disable signing, i.e. does not mean bypass
RequireMessageSigning. A message that is not encrypted shall be subject
to the rules of regular signing.

</div>

# Conclusion

<div>

We have discussed the anatomy of signing and cryptographic keys used in
MS-SMB2 protocol for all dialects supported by Windows 10. These keys –
directly or indirectly – support the following security features:

In SMB 2.0.2 and 2.1 dialects:

</div>

  - Message integrity across an authenticated session.

<div>

In SMB 3.0 dialect:

</div>

  - Message integrity (with a stronger algorithm) across an
    authenticated session.
  - Encryption of traffic between client and server.
  - Session binding to multiple connections (multichannel).
  - Validation of negotiated information.

In SMB 3.1.1 dialect:

  - All the above 3.0 dialect security features, except the validation
    of negotiated information.
  - Negotiation of encryption and integrity algorithms.
  - Protection of negotiation and session establishment.

# Appendix: Key derivation examples

<div>

This sample data should be considered “as-is”. It should also be noted
that examples do not replace normative protocol specifications. The
reference must be \[MS-SMB2\].

</div>

## Key derivation example for SMB 3.0 multichannel

<div>

\--- Master session (first channel) ---

</div>

<div>

Input cryptographicKey 0x7CD451825D0450D235424E44BA6E78CC

(queried from GSS authenticated context)

</div>

<div>

\--- Dialect 0x0300 ---

SessionKey 0x7CD451825D0450D235424E44BA6E78CC

SigningKey 0x0B7E9C5CAC36C0F6EA9AB275298CEDCE

EncryptionKey 0xFAD27796665B313EBB578F388632B4F7

DecryptionKey 0xB0F0427F7CEB416D1D9DCC0CD4F99447

ApplicationKey 0xBB23A4575AA26C721AF525AF15A87B4F

</div>

<div>

\--- Second channel (session binding) ---

</div>

<div>

Input cryptographicKey 0x4E01A2B313BCF660CC250BEF021AEDE6

(queried from GSS authenticated context)

Alternate channel: generate Channel.SigningKey.

Alternate channel: Copy encryption/decryption keys from main channel

</div>

<div>

\--- Dialect 0x0300 ---

SessionKey 0x4E01A2B313BCF660CC250BEF021AEDE6

SigningKey 0xBA1A17DBBFEC349BCA105563D598952F

EncryptionKey 0xFAD27796665B313EBB578F388632B4F7

DecryptionKey 0xB0F0427F7CEB416D1D9DCC0CD4F99447

ApplicationKey 0xBB23A4575AA26C721AF525AF15A87B4F

</div>

<div>

</div>

## Key derivation example for SMB 3.1.1 multichannel

<div>

\--- Master session (first channel) ---

</div>

<div>

Header.Command 0x0000 NEGOTIATE

</div>

<div>

Preauth integrity hash capabilities ---

PreauthIntegrityCaps.HashAlgorithmCount 0x1

PreauthIntegrityCaps.SaltLength 0x20

PreauthIntegrityCaps.HashAlgorithms 0x0001

PreauthIntegrityCaps.Salt

FA49E6578F1F3A9F4CD3E9CC14A67AA884B3D05844E0E5A118225C15887F32FF

</div>

<div>

Encryption capabilites ---

EncryptionCaps.CipherCount 0x2

EncryptionCaps.Ciphers\[0\] 0x0002

EncryptionCaps.Ciphers\[1\] 0x0001

</div>

<div>

Connection.PreauthIntegrityHashId 0x0001

</div>

<div>

NEGOTIATE Request

</div>

<div>

Preauth integrity hash
---

Current

Connection.PreauthIntegrityHashValue

00000000000000000000000000000000000000000000000000000000000000000000000000000000

000000000000000000000000000000000000000000000000

Negotiate request
packet

FE534D4240000100000000000000800000000000000000000100000000000000FFFE000000000000

00000000000000000000000000000000000000000000000024000500000000003F000000ECD86F32

6276024F9F7752B89BB33F3A70000000020000000202100200030203110300000100260000000000

010020000100FA49E6578F1F3A9F4CD3E9CC14A67AA884B3D05844E0E5A118225C15887F32FF0000

0200060000000000020002000100

Concatenate Connection.PreauthIntegrityHashValue and Negotiate request
packet

SHA-512 Input Hash
Data

00000000000000000000000000000000000000000000000000000000000000000000000000000000

000000000000000000000000000000000000000000000000FE534D42400001000000000000008000

00000000000000000100000000000000FFFE00000000000000000000000000000000000000000000

000000000000000024000500000000003F000000ECD86F326276024F9F7752B89BB33F3A70000000

020000000202100200030203110300000100260000000000010020000100FA49E6578F1F3A9F4CD3

E9CC14A67AA884B3D05844E0E5A118225C15887F32FF00000200060000000000020002000100

New

Connection.PreauthIntegrityHashValue

DD94EFC5321BB618A2E208BA8920D2F422992526947A409B5037DE1E0FE8C7362B8C47122594CDE0

CE26AA9DFC8BCDBDE0621957672623351A7540F1E54A0426

</div>

<div>

NEGOTIATE Response

</div>

<div>

Updating Preauth integrity hash
---

Current

Connection.PreauthIntegrityHashValue

DD94EFC5321BB618A2E208BA8920D2F422992526947A409B5037DE1E0FE8C7362B8C47122594CDE0

CE26AA9DFC8BCDBDE0621957672623351A7540F1E54A0426

Negotiate response
packet

FE534D4240000100000000000000010001000000000000000100000000000000FFFE000000000000

000000000000000000000000000000000000000000000000410001001103020039CBCAF329714942

BDCE5D60F09AB3FB2F000000000080000000800000008000D8DAE5ADCBAED00109094AB095AED001

80004001C00100006082013C06062B0601050502A08201303082012CA01A3018060A2B0601040182

3702021E060A2B06010401823702020AA282010C048201084E45474F455854530100000000000000

60000000700000007C7CC0FD06D6362D02DDE1CF343BFE292900F49750B4AA97934D9C4296B26E51

FD370471B235E15A50DAE15BD5489C87000000000000000060000000010000000000000000000000

5C33530DEAF90D4DB2EC4AE3786EC3084E45474F4558545303000000010000004000000098000000

7C7CC0FD06D6362D02DDE1CF343BFE295C33530DEAF90D4DB2EC4AE3786EC3084000000058000000

3056A05430523027802530233121301F06035504031318546F6B656E205369676E696E6720507562

6C6963204B65793027802530233121301F06035504031318546F6B656E205369676E696E67205075

626C6963204B6579010026000000000001002000010060A3C3B95C3C7CCD51EC536648D9B3AC74C4

83CA5B65385A251117BEB30712E50000020004000000000001000200

Concatenate Connection.PreauthIntegrityHashValue and Negotiate response
packet

SHA-512 Input Hash
Data

DD94EFC5321BB618A2E208BA8920D2F422992526947A409B5037DE1E0FE8C7362B8C47122594CDE0

CE26AA9DFC8BCDBDE0621957672623351A7540F1E54A0426FE534D42400001000000000000000100

01000000000000000100000000000000FFFE00000000000000000000000000000000000000000000

0000000000000000410001001103020039CBCAF329714942BDCE5D60F09AB3FB2F00000000008000

0000800000008000D8DAE5ADCBAED00109094AB095AED00180004001C00100006082013C06062B06

01050502A08201303082012CA01A3018060A2B06010401823702021E060A2B06010401823702020A

A282010C048201084E45474F45585453010000000000000060000000700000007C7CC0FD06D6362D

02DDE1CF343BFE292900F49750B4AA97934D9C4296B26E51FD370471B235E15A50DAE15BD5489C87

0000000000000000600000000100000000000000000000005C33530DEAF90D4DB2EC4AE3786EC308

4E45474F45585453030000000100000040000000980000007C7CC0FD06D6362D02DDE1CF343BFE29

5C33530DEAF90D4DB2EC4AE3786EC30840000000580000003056A05430523027802530233121301F

06035504031318546F6B656E205369676E696E67205075626C6963204B6579302780253023312130

1F06035504031318546F6B656E205369676E696E67205075626C6963204B65790100260000000000

01002000010060A3C3B95C3C7CCD51EC536648D9B3AC74C483CA5B65385A251117BEB30712E50000

020004000000000001000200

New

Connection.PreauthIntegrityHashValue

324BFA92A4F3A190E466EBEA08D9C110DC88BFED758D9846ECC6F541CC1D02AE3C94A79F36011E99

7E13F841B91B50957AD07B19C8E2539C0B23FDAE09D2C513

</div>

<div>

Add NEW SessionId 0x100000000019 to Preauth Integrity hash table with
value

Connection.PreauthIntegrityHashValue

324BFA92A4F3A190E466EBEA08D9C110DC88BFED758D9846ECC6F541CC1D02AE3C94A79F36011E99

7E13F841B91B50957AD07B19C8E2539C0B23FDAE09D2C513

</div>

<div>

SESSION SETUP Request

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

324BFA92A4F3A190E466EBEA08D9C110DC88BFED758D9846ECC6F541CC1D02AE3C94A79F36011E99

7E13F841B91B50957AD07B19C8E2539C0B23FDAE09D2C513

SessionSetup request
packet

FE534D4240000100000000000100800000000000000000000200000000000000FFFE000000000000

00000000000000000000000000000000000000000000000019000001010000000000000058004A00

0000000000000000604806062B0601050502A03E303CA00E300C060A2B06010401823702020AA22A

04284E544C4D5353500001000000978208E200000000000000000000000000000000060380250000

000F

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
request packet

SHA-512 Input Hash
Data

324BFA92A4F3A190E466EBEA08D9C110DC88BFED758D9846ECC6F541CC1D02AE3C94A79F36011E99

7E13F841B91B50957AD07B19C8E2539C0B23FDAE09D2C513FE534D42400001000000000001008000

00000000000000000200000000000000FFFE00000000000000000000000000000000000000000000

000000000000000019000001010000000000000058004A000000000000000000604806062B060105

0502A03E303CA00E300C060A2B06010401823702020AA22A04284E544C4D53535000010000009782

08E200000000000000000000000000000000060380250000000F

PreauthSession.PreauthIntegrityHashValue

AC0B0F2B9986257700365E416D142A6EDC96DF03594A19E52A15F6BD0D041CD5D432F8ED42C55E33

197A50C9EC00F1462B50C592211B1471A04B56088FDFD5F9

</div>

<div>

SESSION SETUP Response

</div>

<div>

 --- STATUS\_MORE\_PROCESSING\_REQUIRED - Updating Preauth integrity
hash ---

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

AC0B0F2B9986257700365E416D142A6EDC96DF03594A19E52A15F6BD0D041CD5D432F8ED42C55E33

197A50C9EC00F1462B50C592211B1471A04B56088FDFD5F9

SessionSetup response
packet

FE534D4240000100160000C00100010001000000000000000200000000000000FFFE000000000000

190000000010000000000000000000000000000000000000090000004800B300A181B03081ADA003

0A0101A10C060A2B06010401823702020AA281970481944E544C4D53535000020000000C000C0038

00000015828AE20D1D8BA31179D008000000000000000050005000440000000A0092270000000F53

005500540033003100310002000C0053005500540033003100310001000C00530055005400330031

00310004000C0053005500540033003100310003000C0053005500540033003100310007000800A1

A1F5ADCBAED00100000000

SessionSetup response header signature
0x00000000000000000000000000000000

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
response packet

SHA-512 Input Hash
Data

AC0B0F2B9986257700365E416D142A6EDC96DF03594A19E52A15F6BD0D041CD5D432F8ED42C55E33

197A50C9EC00F1462B50C592211B1471A04B56088FDFD5F9FE534D4240000100160000C001000100

01000000000000000200000000000000FFFE00000000000019000000001000000000000000000000

0000000000000000090000004800B300A181B03081ADA0030A0101A10C060A2B0601040182370202

0AA281970481944E544C4D53535000020000000C000C003800000015828AE20D1D8BA31179D00800

0000000000000050005000440000000A0092270000000F53005500540033003100310002000C0053

005500540033003100310001000C0053005500540033003100310004000C00530055005400330031

00310003000C0053005500540033003100310007000800A1A1F5ADCBAED00100000000

PreauthSession.PreauthIntegrityHashValue

2729E3440DFDDD839E37193F6E8F20C20CEFB3469E453A70CD980EEC06B8835740A7376008563336

4C8989895ECE81BF102DEEB14D4B7D48AFA76901A7A38387

</div>

<div>

SESSION SETUP Request

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

2729E3440DFDDD839E37193F6E8F20C20CEFB3469E453A70CD980EEC06B8835740A7376008563336

4C8989895ECE81BF102DEEB14D4B7D48AFA76901A7A38387

SessionSetup request
packet

FE534D4240000100000000000100800000000000000000000300000000000000FFFE000000000000

1900000000100000000000000000000000000000000000001900000101000000000000005800CF01

0000000000000000A18201CB308201C7A0030A0101A28201AA048201A64E544C4D53535000030000

001800180090000000EE00EE00A80000000C000C00580000001A001A0064000000120012007E0000

001000100096010000158288E2060380250000000FECAC77A5F385A8BF9C38C706EEEDDCD3530055

005400330031003100610064006D0069006E006900730074007200610074006F0072004400520049

0056004500520033003100310000000000000000000000000000000000000000000000000063078E

B639FE03E20A231C3AE3BF23080101000000000000A1A1F5ADCBAED001BC4AD05F223CC90F000000

0002000C0053005500540033003100310001000C0053005500540033003100310004000C00530055

00540033003100310003000C0053005500540033003100310007000800A1A1F5ADCBAED001060004

00020000000800300030000000000000000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B

8E0EBA5A6DFD9D07A31D11B548F8C9D0CC0A00100000000000000000000000000000000000090016

0063006900660073002F005300550054003300310031000000000000000000000000003B9BDFF38F

5EE8F9663F11A0F4C03A78A31204100100000063775A9A5FD97F0600000000

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
request packet

SHA-512 Input Hash
Data

2729E3440DFDDD839E37193F6E8F20C20CEFB3469E453A70CD980EEC06B8835740A7376008563336

4C8989895ECE81BF102DEEB14D4B7D48AFA76901A7A38387FE534D42400001000000000001008000

00000000000000000300000000000000FFFE00000000000019000000001000000000000000000000

00000000000000001900000101000000000000005800CF010000000000000000A18201CB308201C7

A0030A0101A28201AA048201A64E544C4D53535000030000001800180090000000EE00EE00A80000

000C000C00580000001A001A0064000000120012007E0000001000100096010000158288E2060380

250000000FECAC77A5F385A8BF9C38C706EEEDDCD3530055005400330031003100610064006D0069

006E006900730074007200610074006F007200440052004900560045005200330031003100000000

00000000000000000000000000000000000000000063078EB639FE03E20A231C3AE3BF2308010100

0000000000A1A1F5ADCBAED001BC4AD05F223CC90F0000000002000C005300550054003300310031

0001000C0053005500540033003100310004000C0053005500540033003100310003000C00530055

00540033003100310007000800A1A1F5ADCBAED00106000400020000000800300030000000000000

000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B8E0EBA5A6DFD9D07A31D11B548F8C9D0

CC0A001000000000000000000000000000000000000900160063006900660073002F005300550054

003300310031000000000000000000000000003B9BDFF38F5EE8F9663F11A0F4C03A78A312041001

00000063775A9A5FD97F0600000000

PreauthSession.PreauthIntegrityHashValue

0DD13628CC3ED218EF9DF9772D436D0887AB9814BFAE63A80AA845F36909DB7928622DDDAD522D97

51640A459762C5A9D6BB084CBB3CE6BDADEF5D5BCE3C6C01

</div>

<div>

SESSION SETUP Response

SessionId 0x100000000019 COMPLETED

SessionSetup response
packet

FE534D4240000100000000000100800009000000000000000300000000000000FFFE000000000000

1900000000100000EBE146DA120BA25FC3376A49DFE31BC10900000048001D00A11B3019A0030A01

00A3120410010000003B453CDC3524164200000000

SessionSetup response header signature
0xEBE146DA120BA25FC3376A49DFE31BC1

PreauthSession.PreauthIntegrityHashValue

0DD13628CC3ED218EF9DF9772D436D0887AB9814BFAE63A80AA845F36909DB7928622DDDAD522D97

51640A459762C5A9D6BB084CBB3CE6BDADEF5D5BCE3C6C01

</div>

<div>

Input cryptographicKey (SessionKey) 0x270E1BA896585EEB7AF3472D3B4C75A7

(queried from GSS authenticated context)

</div>

<div>

\--- Dialect 0x0311
---

preauthIntegrityHashValue

0DD13628CC3ED218EF9DF9772D436D0887AB9814BFAE63A80AA845F36909DB7928622DDDAD522D97

51640A459762C5A9D6BB084CBB3CE6BDADEF5D5BCE3C6C01

CypherId 0x0002

SessionKey 0x270E1BA896585EEB7AF3472D3B4C75A7

SigningKey 0x73FE7A9A77BEF0BDE49C650D8CCB5F76

EncryptionKey 0x629BCBC54422A0F572B97F45989B6073

DecryptionKey 0xE2AF0DCEFAC68DA71A0DFBD0D1350D74

ApplicationKey 0x6D7AD7954E9EC61E907B4D473DC178FF

</div>

<div>

\--- second channel (session binding) ----

</div>

<div>

Header.Command 0x0000 NEGOTIATE

</div>

<div>

Preauth integrity hash capabilities ---

PreauthIntegrityCaps.HashAlgorithmCount 0x1

PreauthIntegrityCaps.SaltLength 0x20

PreauthIntegrityCaps.HashAlgorithms 0x0001

PreauthIntegrityCaps.Salt

3E38B8B81A9AFB172278056F4898FD09A1B9976A49B2D771161FCD4D40DF7D7D

</div>

<div>

Encryption capabilites ---

EncryptionCaps.CipherCount 0x2

EncryptionCaps.Ciphers\[0\] 0x0002

EncryptionCaps.Ciphers\[1\] 0x0001

</div>

<div>

Connection.PreauthIntegrityHashId 0x0001

</div>

<div>

NEGOTIATE Request

</div>

<div>

Preauth integrity hash
---

Current

Connection.PreauthIntegrityHashValue

00000000000000000000000000000000000000000000000000000000000000000000000000000000

000000000000000000000000000000000000000000000000

Negotiate request
packet

FE534D4240000100000000000000800000000000000000000100000000000000FFFE000000000000

00000000000000000000000000000000000000000000000024000500010000003F000000ECD86F32

6276024F9F7752B89BB33F3A70000000020000000202100200030203110300000100260000000000

0100200001003E38B8B81A9AFB172278056F4898FD09A1B9976A49B2D771161FCD4D40DF7D7D0000

0200060000000000020002000100

Concatenate Connection.PreauthIntegrityHashValue and Negotiate request
packet

SHA-512 Input Hash
Data

00000000000000000000000000000000000000000000000000000000000000000000000000000000

000000000000000000000000000000000000000000000000FE534D42400001000000000000008000

00000000000000000100000000000000FFFE00000000000000000000000000000000000000000000

000000000000000024000500010000003F000000ECD86F326276024F9F7752B89BB33F3A70000000

0200000002021002000302031103000001002600000000000100200001003E38B8B81A9AFB172278

056F4898FD09A1B9976A49B2D771161FCD4D40DF7D7D00000200060000000000020002000100

New

Connection.PreauthIntegrityHashValue

F035C2B2BAB116E0DCF6A74E26670604D1BF6DDA065913AF7C30E93C1F025AC3CE2DD44D4DE26524

A785E5D8E06AF0BE1C74296FEF05B045C3793A12B32C49DF

</div>

<div>

NEGOTIATE Response

</div>

<div>

Updating Preauth integrity hash
---

Current

Connection.PreauthIntegrityHashValue

F035C2B2BAB116E0DCF6A74E26670604D1BF6DDA065913AF7C30E93C1F025AC3CE2DD44D4DE26524

A785E5D8E06AF0BE1C74296FEF05B045C3793A12B32C49DF

Negotiate response
packet

FE534D4240000100000000000000010001000000000000000100000000000000FFFE000000000000

000000000000000000000000000000000000000000000000410001001103020039CBCAF329714942

BDCE5D60F09AB3FB2F000000000080000000800000008000A76533AECBAED00109094AB095AED001

80004001C00100006082013C06062B0601050502A08201303082012CA01A3018060A2B0601040182

3702021E060A2B06010401823702020AA282010C048201084E45474F455854530100000000000000

60000000700000007E7CC0FD06D6362D02DDE1CF343BFE29172BA8482F8B7BE4DBA9F69A8DEBBA83

36E453F3EE58983492F66354ED01A5BE000000000000000060000000010000000000000000000000

5C33530DEAF90D4DB2EC4AE3786EC3084E45474F4558545303000000010000004000000098000000

7E7CC0FD06D6362D02DDE1CF343BFE295C33530DEAF90D4DB2EC4AE3786EC3084000000058000000

3056A05430523027802530233121301F06035504031318546F6B656E205369676E696E6720507562

6C6963204B65793027802530233121301F06035504031318546F6B656E205369676E696E67205075

626C6963204B657901002600000000000100200001002521FE7288989F816EE6D3D6C55921FBEF2C

C239789E83B8A0EC29E8D84EBAB80000020004000000000001000200

Concatenate Connection.PreauthIntegrityHashValue and Negotiate response
packet

SHA-512 Input Hash
Data

F035C2B2BAB116E0DCF6A74E26670604D1BF6DDA065913AF7C30E93C1F025AC3CE2DD44D4DE26524

A785E5D8E06AF0BE1C74296FEF05B045C3793A12B32C49DFFE534D42400001000000000000000100

01000000000000000100000000000000FFFE00000000000000000000000000000000000000000000

0000000000000000410001001103020039CBCAF329714942BDCE5D60F09AB3FB2F00000000008000

0000800000008000A76533AECBAED00109094AB095AED00180004001C00100006082013C06062B06

01050502A08201303082012CA01A3018060A2B06010401823702021E060A2B06010401823702020A

A282010C048201084E45474F45585453010000000000000060000000700000007E7CC0FD06D6362D

02DDE1CF343BFE29172BA8482F8B7BE4DBA9F69A8DEBBA8336E453F3EE58983492F66354ED01A5BE

0000000000000000600000000100000000000000000000005C33530DEAF90D4DB2EC4AE3786EC308

4E45474F45585453030000000100000040000000980000007E7CC0FD06D6362D02DDE1CF343BFE29

5C33530DEAF90D4DB2EC4AE3786EC30840000000580000003056A05430523027802530233121301F

06035504031318546F6B656E205369676E696E67205075626C6963204B6579302780253023312130

1F06035504031318546F6B656E205369676E696E67205075626C6963204B65790100260000000000

0100200001002521FE7288989F816EE6D3D6C55921FBEF2CC239789E83B8A0EC29E8D84EBAB80000

020004000000000001000200

New

Connection.PreauthIntegrityHashValue

E267AB1AA0403082AA2A9FEB0224AF3EA92E53CAA50A893A9635F0659F93591F81391737E68DB0C9

AD878C56449C36A6895EBCF435A7D97072C7B596B8AF3817

</div>

<div>

Add NEW SessionId 0x100000000019 to Preauth Integrity hash table with
value

Connection.PreauthIntegrityHashValue

E267AB1AA0403082AA2A9FEB0224AF3EA92E53CAA50A893A9635F0659F93591F81391737E68DB0C9

AD878C56449C36A6895EBCF435A7D97072C7B596B8AF3817

</div>

<div>

SESSION SETUP Request

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

E267AB1AA0403082AA2A9FEB0224AF3EA92E53CAA50A893A9635F0659F93591F81391737E68DB0C9

AD878C56449C36A6895EBCF435A7D97072C7B596B8AF3817

SessionSetup request
packet

FE534D4240000100000000000100800008000000000000000200000000000000FFFE000000000000

190000000010000068BD0C58F613CE1334B1EB51C68D39EA19000101010000000000000058004A00

0000000000000000604806062B0601050502A03E303CA00E300C060A2B06010401823702020AA22A

04284E544C4D5353500001000000978208E200000000000000000000000000000000060380250000

000F

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
request packet

SHA-512 Input Hash
Data

E267AB1AA0403082AA2A9FEB0224AF3EA92E53CAA50A893A9635F0659F93591F81391737E68DB0C9

AD878C56449C36A6895EBCF435A7D97072C7B596B8AF3817FE534D42400001000000000001008000

08000000000000000200000000000000FFFE000000000000190000000010000068BD0C58F613CE13

34B1EB51C68D39EA19000101010000000000000058004A000000000000000000604806062B060105

0502A03E303CA00E300C060A2B06010401823702020AA22A04284E544C4D53535000010000009782

08E200000000000000000000000000000000060380250000000F

PreauthSession.PreauthIntegrityHashValue

8346469934A59E951A3F2DA7FA4C2C29F0F6B13A6B0951D4CD5279F8D40FD84FF98157937613C6BE

9514582E44344B1710DD5BFCE3BB023D28C6EA512E0ADEBD

</div>

<div>

SESSION SETUP Response

</div>

<div>

 --- STATUS\_MORE\_PROCESSING\_REQUIRED - Updating Preauth integrity
hash ---

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

8346469934A59E951A3F2DA7FA4C2C29F0F6B13A6B0951D4CD5279F8D40FD84FF98157937613C6BE

9514582E44344B1710DD5BFCE3BB023D28C6EA512E0ADEBD

SessionSetup response
packet

FE534D4240000100160000C00100010009000000000000000200000000000000FFFE000000000000

1900000000100000014A71CAE724995E612430E2BE87578C090000004800B300A181B03081ADA003

0A0101A10C060A2B06010401823702020AA281970481944E544C4D53535000020000000C000C0038

00000015828AE23823DA68E32D2FE1000000000000000050005000440000000A0092270000000F53

005500540033003100310002000C0053005500540033003100310001000C00530055005400330031

00310004000C0053005500540033003100310003000C005300550054003300310031000700080041

3B42AECBAED00100000000

SessionSetup response header signature
0x014A71CAE724995E612430E2BE87578C

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
response packet

SHA-512 Input Hash
Data

8346469934A59E951A3F2DA7FA4C2C29F0F6B13A6B0951D4CD5279F8D40FD84FF98157937613C6BE

9514582E44344B1710DD5BFCE3BB023D28C6EA512E0ADEBDFE534D4240000100160000C001000100

09000000000000000200000000000000FFFE0000000000001900000000100000014A71CAE724995E

612430E2BE87578C090000004800B300A181B03081ADA0030A0101A10C060A2B0601040182370202

0AA281970481944E544C4D53535000020000000C000C003800000015828AE23823DA68E32D2FE100

0000000000000050005000440000000A0092270000000F53005500540033003100310002000C0053

005500540033003100310001000C0053005500540033003100310004000C00530055005400330031

00310003000C0053005500540033003100310007000800413B42AECBAED00100000000

PreauthSession.PreauthIntegrityHashValue

6DAD1BA61CAF5FDFBB46D995463FF5780F7248D692E70CE87D8B58B2FBEFD438937E1BCBEC3676F2

6F7EE374E169F8AFB17671FB9A47AB88EE2C079DB2B2C7D3

</div>

<div>

SESSION SETUP Request

PreauthSession.SessionId
0x100000000019

Current

PreauthSession.PreauthIntegrityHashValue

6DAD1BA61CAF5FDFBB46D995463FF5780F7248D692E70CE87D8B58B2FBEFD438937E1BCBEC3676F2

6F7EE374E169F8AFB17671FB9A47AB88EE2C079DB2B2C7D3

SessionSetup request
packet

FE534D4240000100000000000100800008000000000000000300000000000000FFFE000000000000

19000000001000003CA64529ACAE57B26DCEC17B14E477D71900010101000000000000005800CF01

0000000000000000A18201CB308201C7A0030A0101A28201AA048201A64E544C4D53535000030000

001800180090000000EE00EE00A80000000C000C00580000001A001A0064000000120012007E0000

001000100096010000158288E2060380250000000FED00E0F3D901F7B77E98071474F83009530055

005400330031003100610064006D0069006E006900730074007200610074006F0072004400520049

00560045005200330031003100000000000000000000000000000000000000000000000000A46794

DA5D2033965D5339E1A675B79B0101000000000000413B42AECBAED0018E4D343661BA6A75000000

0002000C0053005500540033003100310001000C0053005500540033003100310004000C00530055

00540033003100310003000C0053005500540033003100310007000800413B42AECBAED001060004

00020000000800300030000000000000000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B

8E0EBA5A6DFD9D07A31D11B548F8C9D0CC0A00100000000000000000000000000000000000090016

0063006900660073002F00530055005400330031003100000000000000000000000000C8C61D67CE

697693891336917655DDD3A3120410010000005122F070A7314A9B00000000

Concatenate PreauthSession.PreauthIntegrityHashValue and Session Setup
request packet

SHA-512 Input Hash
Data

6DAD1BA61CAF5FDFBB46D995463FF5780F7248D692E70CE87D8B58B2FBEFD438937E1BCBEC3676F2

6F7EE374E169F8AFB17671FB9A47AB88EE2C079DB2B2C7D3FE534D42400001000000000001008000

08000000000000000300000000000000FFFE00000000000019000000001000003CA64529ACAE57B2

6DCEC17B14E477D71900010101000000000000005800CF010000000000000000A18201CB308201C7

A0030A0101A28201AA048201A64E544C4D53535000030000001800180090000000EE00EE00A80000

000C000C00580000001A001A0064000000120012007E0000001000100096010000158288E2060380

250000000FED00E0F3D901F7B77E98071474F83009530055005400330031003100610064006D0069

006E006900730074007200610074006F007200440052004900560045005200330031003100000000

000000000000000000000000000000000000000000A46794DA5D2033965D5339E1A675B79B010100

0000000000413B42AECBAED0018E4D343661BA6A750000000002000C005300550054003300310031

0001000C0053005500540033003100310004000C0053005500540033003100310003000C00530055

00540033003100310007000800413B42AECBAED00106000400020000000800300030000000000000

000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B8E0EBA5A6DFD9D07A31D11B548F8C9D0

CC0A001000000000000000000000000000000000000900160063006900660073002F005300550054

00330031003100000000000000000000000000C8C61D67CE697693891336917655DDD3A312041001

0000005122F070A7314A9B00000000

PreauthSession.PreauthIntegrityHashValue

EA3BF912B11CBFEC5B1889E8209614218687F82FA5294521AD3063425E49E88A10BD022124CE2512

3BC9111F52D9566BA88BF46344E6063DC5E3FF0389026F6C

</div>

<div>

SESSION SETUP Response

SessionId 0x100000000019 COMPLETED

SessionSetup response
packet

FE534D4240000100000000000100800009000000000000000300000000000000FFFE000000000000

19000000001000008D604217FFBACF40635BF9D8729921500900000048001D00A11B3019A0030A01

00A312041001000000CFB046FC5D368E2200000000

SessionSetup response header signature
0x8D604217FFBACF40635BF9D872992150

PreauthSession.PreauthIntegrityHashValue

EA3BF912B11CBFEC5B1889E8209614218687F82FA5294521AD3063425E49E88A10BD022124CE2512

3BC9111F52D9566BA88BF46344E6063DC5E3FF0389026F6C

</div>

<div>

Input cryptographicKey (SessionKey) 0x84B9DBB730116A8FA6E9889555C265F9

(queried from GSS authenticated context)

</div>

<div>

Alternate channel: generate Channel.SigningKey.

Alternate channel: Copy encryption/decryption keys from main channel

</div>

<div>

\--- Dialect 0x0311
---

preauthIntegrityHashValue

EA3BF912B11CBFEC5B1889E8209614218687F82FA5294521AD3063425E49E88A10BD022124CE2512

3BC9111F52D9566BA88BF46344E6063DC5E3FF0389026F6C

CypherId 0x0002

SessionKey 0x84B9DBB730116A8FA6E9889555C265F9

SigningKey 0xC962BCA1A9DD1697B030644199705431

EncryptionKey 0x629BCBC54422A0F572B97F45989B6073

DecryptionKey 0xE2AF0DCEFAC68DA71A0DFBD0D1350D74

ApplicationKey 0x6D7AD7954E9EC61E907B4D473DC178FF

</div>

<div>

</div>

## NTLMv2 SessionKey calculation example

<div>

These steps show how the SessionKey is calculated for the master session
used in the SMB 3.1.1 multichannel example.

These steps show that with the knowledge of the password and a packet
sniff, one can derive the NTLM SessionKey.

The steps follow MS-NLMP, compute NTLMv2 SessionKey, and verify that it
matches the ExportedSessionKey.

Recall the cryptographicKey (SessionKey)
0x270E1BA896585EEB7AF3472D3B4C75A7

(queried from GSS authenticated context)

</div>

<div>

Inputs ==========

</div>

<div>

ChallengeMessage.ServerChallenge

</div>

<div>

       - ResponseToken: NTLM CHALLENGE MESSAGE

\- NLMP: NTLM CHALLENGE MESSAGE

Signature: NTLMSSP

MessageType: Challenge Message (0x00000002)

\+ TargetNameFields: Length: 12, Offset: 56

\+ NegotiateFlags: 0xE28A8215 (NTLM v2, 128-bit encryption, Always Sign)

\+ ServerChallenge: 0D1D8BA31179D008

Reserved: Binary Large Object (8 Bytes)

\+ TargetInfoFields: Length: 80, Offset: 68

\+ Version: Windows 10.0 Build 10130 NLMPv15

TargetNameString: SUT311

\- AvPairs: 6 pairs

\+ AvPair: SUT311 (Server NetBIOS domain name)

\+ AvPair: SUT311 (Server NetBIOS computer name)

\+ AvPair: SUT311 (The FQDN (2) of the domain.)

\+ AvPair: SUT311 (The fully qualified domain name (FQDN (1)) of the
computer.)

\+ AvPair: 10:18:21 PM 6/24/2015 (Server local time)

\+ AvPair: NULL

</div>

<div>

ChallengeMessage.ServerChallenge = "0D 1D 8B A3 11 79 D0 08 "

</div>

<div>

From the NtChallengeResponse, the Response needs to match the calculated
NTProofStr .

NtChallengeResponse = ConcatenationOf(NTProofStr, temp)

</div>

<div>

       - ResponseToken: NTLM AUTHENTICATE MESSAGE Version:NTLM v2,
Domain: SUT311, User: administrator, Workstation: DRIVER311

\- NLMP: NTLM AUTHENTICATE MESSAGE Version:NTLM v2, Domain: SUT311,
User: administrator, Workstation: DRIVER311

Signature: NTLMSSP

MessageType: Authenticate Message (0x00000003)

\+ LmChallengeResponseFields: Length: 24, Offset: 144

\+ NtChallengeResponseFields: Length: 238, Offset: 168

\+ DomainNameFields: Length: 12, Offset: 88

\+ UserNameFields: Length: 26, Offset: 100

\+ WorkstationFields: Length: 18, Offset: 126

\+ EncryptedRandomSessionKeyFields: Length: 16, Offset: 406

\+ NegotiateFlags: 0xE2888215 (NTLM v2, 128-bit encryption, Always Sign)

\+ Version: Windows 6.3 Build 9600 NLMPv15

\+ MessageIntegrityCheck: ECAC77A5F385A8BF9C38C706EEEDDCD3

DomainNameString: SUT311

UserNameString: administrator

WorkstationString: DRIVER311

\+ LmChallengeResponseStruct:
000000000000000000000000000000000000000000000000

\- NTLMV2ChallengeResponse: 63078EB639FE03E20A231C3AE3BF2308

\+ Response: 63078EB639FE03E20A231C3AE3BF2308

ResponseVersion: 1 (0x1)

HiResponseVersion: 1 (0x1)

\+ Z1:

Time: 06/24/2015, 22:18:21.389456 UTC

\+ ClientChallenge: BC4AD05F223CC90F

\+ Z2:

\- AvPairs: 10 pairs

\+ AvPair: SUT311 (Server NetBIOS domain name)

\+ AvPair: SUT311 (Server NetBIOS computer name)

\+ AvPair: SUT311 (The FQDN (2) of the domain.)

\+ AvPair: SUT311 (The fully qualified domain name (FQDN (1)) of the
computer.)

\+ AvPair: 10:18:21 PM 6/24/2015 (Server local time)

\+ AvPair: 0x2 (Server configuration)

\+ AvPair: 0x3000 (Token restrictions)

\+ AvPair: 0x3000 (A channel bindings hash)

\+ AvPair: cifs/SUT311 (The SPN of the target server)

\+ AvPair: NULL

Padding: Binary Large Object (8 Bytes)

\+ SessionKeyString: 3B9BDFF38F5EE8F9663F11A0F4C03A78

\+ Tag3:

\+ OctetStringHeader:

\+ MechListMic: Version: 1

</div>

<div>

Based on NtChallengeResponseFields: Length: 238, Offset: 168, extract
the bytes:

AuthenticateMessage: Ntlmv2ChallengeResponse

63 07 8E B6 39 FE 03 E2 0A 23 1C 3A E3 BF 23 08 01 01 00 00 00 00 00 00
A1 A1 F5 AD CB AE D0

01 BC 4A D0 5F 22 3C C9 0F 00 00 00 00 02 00 0C 00 53 00 55 00 54 00 33
00 31 00 31 00 01 00

0C 00 53 00 55 00 54 00 33 00 31 00 31 00 04 00 0C 00 53 00 55 00 54 00
33 00 31 00 31 00 03

00 0C 00 53 00 55 00 54 00 33 00 31 00 31 00 07 00 08 00 A1 A1 F5 AD CB
AE D0 01 06 00 04 00

02 00 00 00 08 00 30 00 30 00 00 00 00 00 00 00 00 00 00 00 00 30 00 00
B6 1F EF CA A8 57 EA

57 BF 1E DC EB F8 97 4B 8E 0E BA 5A 6D FD 9D 07 A3 1D 11 B5 48 F8 C9 D0
CC 0A 00 10 00 00

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 09 00 16 00 63 00 69 00 66
00 73 00 2F 00 53 00

55 00 54 00 33 00 31 00 31 00 00 00 00 00 00 00 00 00 00 00 00 00

</div>

<div>

AuthenticateMessage: Ntlmv2ChallengeResponse.Response:

63 07 8E B6 39 FE 03 E2 0A 23 1C 3A E3 BF 23 08

( or for future reference 63078EB639FE03E20A231C3AE3BF2308  )

</div>

<div>

From Ntlmv2ChallengeResponse of AuthenticateMessage, strip off the
Response field:

temp =

01 01 00 00 00 00 00 00 A1 A1 F5 AD CB AE D0 01 BC 4A D0 5F 22 3C C9 0F
00 00 00

00 02 00 0C 00 53 00 55 00 54 00 33 00 31 00 31 00 01 00 0C 00 53 00 55
00 54 00

33 00 31 00 31 00 04 00 0C 00 53 00 55 00 54 00 33 00 31 00 31 00 03 00
0C 00 53

00 55 00 54 00 33 00 31 00 31 00 07 00 08 00 A1 A1 F5 AD CB AE D0 01 06
00 04 00

02 00 00 00 08 00 30 00 30 00 00 00 00 00 00 00 00 00 00 00 00 30 00 00
B6 1F EF

CA A8 57 EA 57 BF 1E DC EB F8 97 4B 8E 0E BA 5A 6D FD 9D 07 A3 1D 11 B5
48 F8 C9

D0 CC 0A 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 09 00
16 00 63

00 69 00 66 00 73 00 2F 00 53 00 55 00 54 00 33 00 31 00 31 00 00 00 00
00 00 00

00 00 00 00 00 00

</div>

<div>

AuthenticateMessage.SessionKeyString

EncryptedRandomSessionKey = 3B 9B DF F3 8F 5E E8 F9 66 3F 11 A0 F4 C0 3A
78

</div>

<div>

Supply all parameters to the key calculation algorithm defined in
MS-NLMP.

Passwd = Password01\!

Username = ADMINISTRATOR

Domain = SUT311

</div>

<div>

\=== key calculation steps: \[MS-NLMP\] 3.3.2 NTLM v2 Authentication

</div>

<div>

        UNICODE(Passwd)) =
FFFE500061007300730077006F0072006400300031002100

NtHash = MD4(UNICODE(Passwd)) = 7C4FE5EADA682714A036E39378362BAB

UUserDom = UNICODE(ConcatenationOf( Uppercase(User), UserDom ) )

UUserDom = UNICODE(ADMINISTRATORSUT311) 
=

FFFE410044004D0049004E004900530054005200410054004F00520053005500540033003100

3100

NTOWFv2(Passwd, User, UserDom) = HMAC\_MD5(NtHash, UUserDom)  =

AEE3959B44A815F1EB28C9511B4F533B

ResponseKeyNT = NTOWFv2 = AEE3959B44A815F1EB28C9511B4F533B

temp = ConcatenationOf(Responserversion,

HiResponserversion, Z(6), Time, ClientChallenge, Z(4), ServerName, Z(4))

temp =
0101000000000000A1A1F5ADCBAED001BC4AD05F223CC90F0000000002000C00530055005400

33003100310001000C0053005500540033003100310004000C005300550054003300310031000

3000C0053005500540033003100310007000800A1A1F5ADCBAED001060004000200000008003

00030000000000000000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B8E0EBA5A6DF

D9D07A31D11B548F8C9D0CC0A001000000000000000000000000000000000000900160063006

900660073002F00530055005400330031003100000000000000000000000000

CHALLENGE\_MESSAGE.ServerChallenge = 0D1D8BA31179D008

NTProofStr = HMAC\_MD5(ResponseKeyNT,

ConcatenationOf(CHALLENGE\_MESSAGE.ServerChallenge, temp)

</div>

<div>

        NTProofStr = 63078EB639FE03E20A231C3AE3BF2308

SessionBaseKey = HMAC\_MD5(ResponseKeyNT, NTProofStr)

NTLMv2 KeyExchangeKey = SessionBaseKey

KeyExchangeKey= B4CF22566926B1C069ACD80E4D73C814

</div>

<div>

Because NTLMSSP\_NEGOTIATE\_KEY\_EXCH

AND (NTLMSSP\_NEGOTIATE\_SIGN OR NTLMSSP\_NEGOTIATE\_SEAL):

ExportedSessionKey = RC4.Encrypt(KeyExchangeKey,

AUTHENTICATE\_MESSAGE.EncryptedRandomSessionKey)

</div>

<div>

   ExportedSessionKey = 270E1BA896585EEB7AF3472D3B4C75A7

(this matches the key queried from GSS-API)

</div>

<div>

</div>

<div>

For completeness, we also show here the computation of the remaining
NTLM keys. Note those keys have no use in SMB2/3.

</div>

<div>

Check SIGNKEY() algorithm in MS-NLMP for specific details.

</div>

<div>

Because we have NTLMSSP\_NEGOTIATE\_EXTENDED\_SESSIONSECURITY flag set:

Client Signing key = MD5(ConcatenationOf(ExportedSessionKey,

"session key to client-to-server signing key magic constant"))

Server Signing key = MD5(ConcatenationOf(ExportedSessionKey,

"session key to server-to-client signing key magic constant"))

</div>

<div>

Server Signing key = E1BD8B416B0B709D295E12F2CF18E6C5

Client Signing key = D43F36C44BCE0630250A09EA0C2E8C2C

</div>

<div>

Check SEALKEY() algorithm in MS-NLMP for specific details.

</div>

<div>

Because NTLMSSP\_NEGOTIATE\_EXTENDED\_SESSIONSECURITY flag is set and

NTLMSSP\_NEGOTIATE\_128 is set:

Client Sealing Key = MD5(ConcatenationOf(ExportedSessionKey,

"session key to client-to-server sealing key magic constant"))

Server Sealing Key = MD5(ConcatenationOf(ExportedSessionKey,

"session key to server-to-client sealing key magic constant"))

</div>

<div>

Server Sealing Key = B0F5A0B32C81FF34A878E1409B3B0EF2

Client Sealing Key = 31E5557D99BE13F1B2665C7C7C52CE70

</div>

# References

<div>

\[MS-SMB2\]: Server Message Block (SMB) Protocol Versions 2 and 3
Specification

https://msdn.microsoft.com/en-us/library/cc246482.aspx

\[SP800-108\] National Institute of Standards and Technology. "Special
Publication 800-108, Recommendation for Key Derivation Using
Pseudorandom Functions", October 2009,
http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf

\[RFC5084\] Housley, R., "Using AES-CCM and AES-GCM Authenticated
Encryption in the Cryptographic Message Syntax (CMS)", RFC 5084,
November 2007, http://www.ietf.org/rfc/rfc5084.txt

\[FIPS180-4\] FIPS PUBS, "Secure Hash Standards (SHS)", March 2012,
http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf

\[RFC4493\]. The AES-CMAC Algorithm, http://www.ietf.org/rfc/rfc4493.txt

Windows Server 2012 R2: Which version of the SMB protocol (SMB 1.0, SMB
2.0, SMB 2.1, SMB 3.0 or SMB 3.02) are you
using?

</div>

<div>

https://blogs.technet.microsoft.com/josebda/2013/10/02/windows-server-2012-r2-which-version-of-the-smb-protocol-smb-1-0-smb-2-0-smb-2-1-smb-3-0-or-smb-3-02-are-you-using/

The Basics of SMB Signing (covering both SMB1 and
SMB2)

https://blogs.technet.microsoft.com/josebda/2010/12/01/the-basics-of-smb-signing-covering-both-smb1-and-smb2/

</div>

<div>

</div>

</div>

</div>
