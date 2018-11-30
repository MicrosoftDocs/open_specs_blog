<div id="page">

# Verifying the server signature in Kerberos Privilege Account Certificate

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
12/31/2009 8:33:00 PM

-----

<div id="content">

This blog post focuses on understanding how a server signature is
verified in a Kerberos Privilege Account Certificate (PAC). A PAC
contains two signatures: a server signature and a KDC signature. In a
[previous
blog](http://blogs.msdn.com/openspecification/archive/2009/04/24/understanding-microsoft-kerberos-pac-validation.aspx),
I introduced PAC validation, whereby the server requests the KDC to
verify the PAC. In this blog, I will talk about the verification of PAC
server signature, more specifically how the verification is done even
when the encryption/hashing types are different for the service ticket
and the PAC server signature.

 

## Background

The Privilege Account Certificate (PAC) is an extension element of the
authorization-data field contained in the clients' Kerberos ticket.  The
PAC structure is defined in
\[[MS-PAC](http://msdn.microsoft.com/en-us/library/cc237917.aspx)\] and
conveys authorization data provided by domain controllers (DCs) in an
Active Directory-enabled domain. It contains information such as
security identifiers, group membership, user profile information, and
password credentials.

More details can be found in \[MS-PAC\] Section 2, which describes the
Encapsulation layers.

One key reason why a PAC should be verified as unaltered is to ensure
that no privileges have been maliciously added to - or removed from -
the ticket; that is to prevent a man-in-the-middle from spoofing and
tampering the PAC.

The signatures allow the KDC or the principal verifying the PAC to
determine if the contents have been modified. The first signature is the
server signature, or server checksum. The second is the KDC signature.

In the remainder of this post, I will focus on the server signature or
server checksum in a PAC\_INFO\_BUFFER with the ultype of 0x00000006, as
defined in \[MS-PAC\]
2.4.

## Understanding the cryptographic system used by the KDC to generate the PAC server signature

The following logic reflects the testing of
\[[MS-KILE](http://msdn.microsoft.com/en-us/library/cc233855.aspx)\] and
MS-PAC implementations in Windows-based servers 2008 and 2008 R2.

By default, HMAC-RC4 is used to compute the PAC server signature,
unless:

  - \- the server key type is one the newer encryption types i.e. AES256
    or AES128, and
  - \- the functional level of the domain NC is Windows Server 2008 or
    2008 R2, or the registry setting disables the bit of HMAC-RC4 in the
    SupportedEncryptionTypes (registry entry on the DC at the following
    location (see
    [KB977321](http://support.microsoft.com/default.aspx/kb/977321)):
    HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\Kerberos\\parameters\\)

 

This logic can be explained as follows. First, as highlighted by Windows
behavior note \<26\> in \[MS-KILE\], AES is supported in Windows 2008
and onward.

Second, there are several DCs on the field that have 2003 functional
level. Even though some of these DCs are running Windows Server 2008 or
2008 R2, domain administrators tend sometimes to be conservative and
keep the functional level to 2003.

Third, unless otherwise configured, a DC that is operating at Windows
Server 2003 functional level will use the strongest encryption type that
the client can support to issue a Kerberos service ticket (e.g. AES 256
for Windows 7). However, the PAC signature could be fairly computed with
HMAC-RC4. This would be important to support of cross-domain or
cross-forest scenarios.

 

## Excerpt from MS-KILE

This is an excerpt from MS-KILE.

 

<span id="_Toc247583957"></span>**3.3.5.3.2.3   Server
Signature**<span id="z962edb93fa3c48eaa0a6062f760eb69c"></span>****

The KDC creates a keyed hash
([\[RFC4757\]](http://go.microsoft.com/fwlink/?LinkId=90488)) of the
entire PAC message with the Signature fields of both
PAC\_SIGNATURE\_DATA structures set to zero using the server account key
with the strongest cryptography that the domain
supports<span id="z105"></span>\<26\> and populates the returned
PAC\_SIGNATURE\_DATA structure (\[MS-PAC\] section 2.8) fields as
follows:

 The SignatureType SHOULD be the value (\[MS-PAC\] section 2.8)
corresponding to the cryptographic system used to calculate the
checksum.

 The Signature field SHOULD be the keyed hash (\[RFC4757\]) of the
entire PAC message with the Signature fields of both
PAC\_SIGNATURE\_DATA structures set to zero.

<span id="_Toc247583958"></span> 

**Windows Behavior Note**

\<26\> Section 3.3.5.3.2.3: Active Directories with the
msDS-Behavior-Version attribute on a domain NC root object equal to
DS\_BEHAVIOR\_WIN2000, DS\_BEHAVIOR\_WIN20003\_WITH\_MIXED\_DOMAINS, or
DS\_BEHAVIOR\_WIN2003, cannot support AES.

## Examples

These examples assume the default settings are used on all servers and
clients.

## Example 1: Same cryptosystem

DC: Windows Server 2008 SP2, functional level Windows Server 2008

Kerberos Server: Windows Server 2008 SP1

Kerberos client: Windows 7 RTM

The Kerberos client is issued a Ticket with an encryption type Etype:
aes256-cts-hmac-sha1-96 (18). For the CIFS service, the PAC in the in
the service ticket has a SignatureType of 0x00000010
(HMAC\_SHA1\_96\_AES256), or 16. This signature type corresponds to the
aes256-cts-hmac-sha1-96 cryptosystem. The resulting checksum size is 12
bytes as defined in MS-PAC Section 2.8.1.

## Example 2: Different cryptosystems

DC: Windows Server 2008 SP2, functional level Windows Server 2003

Kerberos Server: Windows Server 2008 SP1

Kerberos client: Windows 7 RTM

The Kerberos client is issued a Ticket with an encryption type Etype:
aes256-cts-hmac-sha1-96 (18). For the CIFS service, the PAC in the in
the service ticket has a SignatureType of 0xFFFFFF76
(KERB\_CHECKSUM\_HMAC\_MD5), or a checksum type of -138. This signature
type corresponds to the rc4-hmac-md5 cryptosystem. The resulting
checksum size is 16 bytes as defined in MS-PAC Section 2.8.1.

## Algorithm used to verify the PAC server signature

When the server verifies the PAC signature, it must use the same
cryptosystem that the KDC used to generate the server checksum. The list
of signature types and corresponding cryptographic systems are
documented in MS-PAC 2.8.

The cryptosystem that is used to compute the server checksum does not
depend on the key type of the long term key that the KDC shares with the
server. For instance in the Example 2 above, the server signature type
is KERB\_CHECKSUM\_HMAC\_MD5, the same algorithm RC4-HMAC-MD5 will be
used to generate and verify the signature in case of AES 128/256
long-term keys, or RC4-HMAC-MD5 long-term key. Any attempt to use a
cryptosystem that does not correspond to the signature type will produce
a different keyed hashed server checksum.

Per HMAC [RFC 2104](http://www.ietf.org/rfc/rfc2104.txt) (referenced by
RFC 4757), B=64 bytes is used for the padding for both MD5 and SHA1. The
key can be of any length up to B, the block length of the hash function.
Since the AES256 key length is 256 bits (32 bytes), the key material
will not be truncated but appended with 32 bytes of zeroes; for AES128
the padding would have been 48 bytes of zeros.

## Conclusion

When generating the PAC server signature, the KDC uses the long-term key
that it shares with the server (\[MS-PAC\] 2.8. 1). The server must
always use the long term key (that is used to decrypt the service
ticket) as an input to the keyed hash. This allows the server to verify
the signature on receiving the PAC, based on the signature type and the
corresponding cryptographic system.

## References

\[MS-KILE\] [Kerberos Protocol
Extensions](http://msdn.microsoft.com/en-us/library/cc233855.aspx)

\[MS-PAC\] [Privilege Attribute Certificate Data
Structure](http://msdn.microsoft.com/en-us/library/cc237917.aspx)

\[RFC4120\] [The Kerberos Network Authentication Service
(V5)](http://www.ietf.org/rfc/rfc4120.txt)

\[RFC4757\] [The RC4-HMAC Kerberos Encryption Types Used by Microsoft
Windows](http://www.ietf.org/rfc/rfc4757.txt)

\[RFC2104\] [HMAC: Keyed-Hashing for Message
Authentication](http://www.ietf.org/rfc/rfc2104.txt)

 

</div>

</div>
