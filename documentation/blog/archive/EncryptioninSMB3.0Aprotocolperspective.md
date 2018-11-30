<div id="page">

# Encryption in SMB 3.0: A protocol perspective

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
10/5/2012 1:26:00 PM

-----

<div id="content">

<span style="font-family: Calibri; font-size: small;">Encryption is one
of the new SMB 3.0 security enhancements in Windows Server 2012 RTM. It
can be enabled on a per-share basis, or enforced for all shares on the
server. SMB 3.0 uses AES-CCM
\[RFC5084\] </span><span style="font-size: small;"><span style="font-family: Calibri;">as
encryption algorithm, and this also provides data integrity (signing).
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">This
blog takes a protocol walk on the topic through \[MS-SMB2\], and
provides a sample test vector. \[MS-SMB2\] describes SMB 2.x and SMB 3.0
dialects. Throughout this blog, I use the terms SMB2 and SMB3
interchangeably because both share the core SMB2 Packet format. Details
on encryption configuration aspects can be found in the blogs listed in
references.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Capability activation </span></span>

**<span style="font-family: Calibri; font-size: small;"> </span>**

  - <span style="font-family: Calibri;"><span style="font-size: small;">The
    client and server advertise encryption support via the
    SMB2\_GLOBAL\_CAP\_ENCRYPTION capability flag during SMB 3.0 dialect
    negotiation. Then at least one of the following two options must be
    happen:
    </span></span>
  - <span style="font-family: Calibri;"><span style="font-size: small;">For
    enabling encryption for a whole session, SessionFlags of
    SessionSetup response includes SMB2\_SESSION\_FLAG\_ENCRYPT\_DATA.
    This is the case where encryption is enforced on the whole
    server.</span></span>
  - <span style="font-size: small;"><span style="font-family: Calibri;">For
    a per-share enabled encryption, ShareFlags of TreeConnect response
    includes SMB2\_SHAREFLAG\_ENCRYPT\_DATA.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">When
encryption is active for a given exchange, it is applied before
submission to the transport. An exchange could consist of a single
request or response, or a series of compounded chain operations. The
sender performs encryption as an outer layer after the SMB2 messages in
a given submission have been assembled.
</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Cryptographic keys</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Ko =
SMB3KDF (Ki, Label,
Context)</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">SMB3KDF()
is defined as the KDF algorithm in Counter Mode, as specified in
\[SP800-108\] section 5.1, with 'r' value of 32 and 'L' value of 128,
and HMAC-SHA256 as the PRF.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Ki –
Key derivation key, used as an input to the KDF. For SMB 3.0, Ki is the
SessionKey.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Label
– the purpose of this derived key, encoded as string and length for SMB
3.0.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Context
– the context information of this derived key, encoded as string and
length for SMB 3.0.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">L –
An integer that specifies the length of the derived keying material Ko,
L is 128 bits for SMB 3.0 cryptographic keys. Note that L is a constant
since all SMB 3.0 keys are 16 bytes in length (SigningKey,
EncryptionKey, DecryptionKey, and ApplicationKey).</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Ko –
Keying material output from the KDF, a binary string of length L, where
Ko is the leftmost L bits of KDF
    result.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

  - <span style="font-size: small;"><span style="font-family: Calibri;">ServerInKey 
    (ClientToServer) =  SMB3KDF (SessionKey, "SMB2AESCCM\\0", "ServerIn
    \\0")</span></span>

<span style="font-family: Calibri;"><span style="font-size: small;">EncryptionKey
(Client) = DecryptionKey (Server) = ServerInKey (ClientToServer)
</span></span>

  - <span style="font-size: small;"><span style="font-family: Calibri;">ServerOutKey
    (ServerToClient) = SMB3KDF ( SessionKey, "SMB2AESCCM\\0",
    "ServerOut\\0")</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">EncryptionKey
(Server) = DecryptionKey (Client) = ServerOutKey
(ServerToClient)</span></span>

<span style="font-family: Calibri;"> </span>

<span style="font-family: Calibri;"><span style="line-height: 115%; font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-ascii-theme-font: minor-latin; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA;">The
server and client generate the encryption keys upon session
establishment of the primary channel. If the server is configured for
encryption (i.e. SmbServerConfiguration.EncryptData) and the
Connection.ClientCapabilities includes the
SMB2\_GLOBAL\_CAP\_ENCRYPTION, the server generates the EncryptionKey
and DecryptionKey and sets SMB2\_SESSION\_FLAG\_ENCRYPT\_DATA flag in
the SessionFlags field of the SessionSetup response; the client must
also generate its encryption and decryption keys.</span></span>

 

<span style="font-size: small;"><span style="font-family: Calibri;">Upon
successful TreeConnect, if Session.EncryptData is FALSE and
Share.EncryptData (e.g. the share is configured to require encryption),
the server generates its EncryptionKey and DecryptionKey and sets the
SMB2\_SHAREFLAG\_ENCRYPT\_DATA bit in the ShareFlags field of the
TreeConnect response; the client must also generate its encryption and
decryption
keys.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Note:
These cryptographic keys are all derived from the SessionKey. As a
result, SMB 3.0 signing and encryption is as secure as the session key.
Not only must this key be unique and very random, but also it needs be
kept secret.
</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Transformed message</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">A
transformed message consists of a transform\_header followed by its
encrypted SMB2 message.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
transform\_header has the following
fields:</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">ProtocolId
(4 bytes):  0xFD, 'S', 'M', and 'B' (in network
order).</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Signature
(16 bytes):  Signature of the encrypted
message.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Nonce
(16 bytes):  An implementation-specific value that is unique for every
encrypted message within a session.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">OriginalMessageSize
(4 bytes):  The size in bytes of the SMB2
message.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Reserved
(2 bytes):  Set to zeros and ignored.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">EncryptionAlgorithm
(2 bytes):  the value 0x0001 is for
SMB2\_ENCRYPTION\_AES128\_CCM.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">SessionId
(8 bytes):  Uniquely identifies the established session for the
command.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">With
the SMB2\_ENCRYPTION\_AES128\_CCM algorithm, SMB 3.0 encryption
implementation uses the leftmost 11 bytes of the Nonce field in the
transform\_header, let’s call this Aes128Nonce. Per \[RFC3610\], the
nonce passed to AES-CCM can be between 7 and 13 bytes. Windows SMB 3.0
made the trade-off choice of 11 bytes. </span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Note
that it would almost be impossible that all possible nonce values can be
consumed on a channel, because it would take over several centuries
before a sender could recycle a nonce on a given
channel. </span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Encrypting the message</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">The sender builds
the transformed message with these specifics. AES-CCM
</span>[<span style="font-family: Verdana; font-size: small;">\[RFC5084\]</span>](http://go.microsoft.com/fwlink/?LinkId=229742)<span style="font-size: small;"><span style="font-family: Calibri;">
is called with the following inputs:</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">AES
key: Session.EncryptionKey.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">Aes-nonce:
Aes128Nonce must be used.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">Plaintext:
The SMB2 message including the header and the payload.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">The
optional authenticated data (AAD):  The SMB2 transform\_header excluding
the ProtocolId and Signature fields; these are the 32 bytes starting
from the Nonce field. </span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
AES-CCM outputs are: </span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">Ciphertext:
the encrypted SMB2 message</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">Message
authentication code: the Signature field of the
transform\_header.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
sender appends the encrypted SMB2 message to the transform\_header and
sends it to the
receiver.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Decrypting the message</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
message is decrypted using: </span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">The
EncryptionAlgorithm in the transform\_header.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">The
Session.DecryptionKey of the Session that corresponds to the SessionId
in the transform\_header.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">The
AAD passed to AES-CCM is the transform\_header excluding the ProtocolId
and Signature fields.</span></span>

<span style="font-family: Calibri; font-size: small;">-</span>       
<span style="font-size: small;"><span style="font-family: Calibri;">The
nonce passed to AES-CCM is the Aes128Nonce; that is the first 11 bytes
of the Nonce field.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
signature returned by the decryption algorithm is then verified against
the Signature in the
transform\_header.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Encryption clauses</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
sender encrypts the message if any of the following conditions is
satisfied:</span></span>

  - If the sender is sending a response to an encrypted request.
  - If Session.EncryptData is TRUE and the request or response being
    sent is not NEGOTIATE.
  - If Session.EncryptData is FALSE, the request or response being sent
    is not NEGOTIATE or SESSION\_SETUP or TREE\_CONNECT, and
    \<TreeConnect|Share\>.EncryptData is TRUE.

 

Note: TreeConnect.EncryptData is on the client side. Share.EncryptData
is on the server
side.

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Review of encryption clauses</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

  - <span style="font-family: Calibri;"><span style="font-size: small;">All
    clauses exclude any operation which does use a SessionId. A
    SessionId is needed to find the Session object and derive the
    encryption and decryption keys from its session key. For example, if
    the client sends a non-encrypted ECHO, Windows 8 server will respond
    with a non-encrypted response.
    </span></span>
  - <span style="font-family: Calibri;"><span style="font-size: small;">Clause
    “response to an encrypted request”: if the sender encrypts the
    request, the receiver will respond in the same way by encrypting the
    response. There is however a prerequisite that encryption is active,
    i.e. encryption keys have been generated. For example, if the client
    encrypts an ECHO, Windows 8 server responds in-kind by encrypting
    the response.  </span></span>
  - <span style="font-family: Calibri;">Clause "Session.EncryptData is
    TRUE":
    <span style="line-height: 107%; font-family: &#39;Calibri&#39;,sans-serif; font-size: 11pt; mso-ascii-theme-font: minor-latin; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-theme-font: minor-bidi; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-bidi-font-family: &#39;Times New Roman&#39;;">SMB
    3 session setup encryption goes as
follows:</span></span>

<span style="font-family: Calibri; font-size: small;"><span style="line-height: 107%; font-family: &#39;Calibri&#39;,sans-serif; mso-ascii-theme-font: minor-latin; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-theme-font: minor-bidi; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-bidi-font-family: &#39;Times New Roman&#39;;">--        
Initial session setup messages are un-encrypted as there is no session
object (key etc).  
\--         Session binding requests must be signed, but not encrypted.
Note:  Windows-based server does not encrypt session setup responses for
session binding, regardless whether the client encrypts the session
binding request. On the other side, Windows client does not encrypt
session binding requests when Session.EncryptData is TRUE.  
\--         Spontaneous re-authentication of a valid session must be
encrypted, otherwise the server returns STATUS\_ACCESS\_DENIED.  
\--         Re-authentication of an expired session is encrypted as
well.  
\--         For re-authentication at reconnection after a broken
connection, if the client lost all of its connections to the server,
then the client would reconnect and create a new session (initial
session setup). If the client lost some of its connections but not all,
then the client would reconnect and bind to the existing session
(session setup
    binding).  
</span></span>

  - <span style="font-size: small;"><span style="font-family: Calibri;">Clause
    “Session.EncryptData is FALSE  and \<TreeConnect|Share\>.EncryptData
    is TRUE”: </span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">If
the client performs TREE\_DISCONNECT before a LOGOFF, the logoff will
not be encrypted.
</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Compounded messages</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">It
is expected that all operations in an encrypted message belong to the
SessionId in the transform\_header. The encrypted message is sent as a
single submission to the underlying transport, there is no provision for
a next transformed message in the
transform\_header.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

### <span style="font-size: small;"><span style="color: #4f81bd;">Related compounded messages</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">In
case of related compounded requests, all the requests/responses belong
to the same SessionId. The sender indicates to the receiver that it is
using the SessionId, TreeId, and FileId of the previous operation or
resulted from the receiver processing that previous
operation.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

### <span style="font-size: small;"><span style="color: #4f81bd;">Unrelated compounded messages</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">From
a broader scope, the expected protocol behaviors for sending an
encrypted message can be summarized as follows, be it for compounded
requests/responses or not.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
sender should not send an encrypted request separately or as part of a
compounded chain (related or unrelated) that contains a SessionId
different from the session used for encryption. In other words, the
sender should not use a SessionId to convoy the encrypted traffic of
other authenticated sessions.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
receiver should not send an encrypted response separately or as part of
a compounded chain (related or unrelated) that contains a SessionId
different from the session used for encryption.   Windows-based servers
will respond in-kind to what the client sends. If the client violates
the above statement regarding the SessionId, a Windows 8-based server
will currently send the response as
such.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Oplock and Lease Breaks</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Oplock
break notifications/acknowledgments/responses must be encrypted when
encryption is active. For an Oplock, the FileID is used to derive the
SessionId which is set in the notification/acknowledgement/response. See
more details in MS-SMB2.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Lease
break notifications - sent by the server - do not have a SessionId, and
as a result, are neither signed nor encrypted.  Lease keys are not tied
to a particular session from the
client.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">However,
Lease break acknowledgements sent by the client - and their responses
sent by the server - must be encrypted when encryption is active. The
client is responsible for selecting a session associated with one of the
existing opens associated with that Lease Key. The response is sent on
the session that receives the acknowledgment.
</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

## <span style="font-size: medium;"><span style="color: #4f81bd;">Test vector</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">This
sample data should be considered “as-is”. It should also be noted that
examples do not replace normative protocol specifications. The reference
must be \[MS-SMB2\]. </span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
test program running on a Windows 8 client negotiates SMB3 and
communicates with a Windows 2012 server. It opens a file and WRITEs the
following content. It then READs back the file. </span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">This
is the content written and read:</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Smb3
encryption testing</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Hex
value:</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">536D623320656E6372797074696F6E2074657374696E67</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">These
outputs show the encryption and decryption of the WRITE and READ
commands.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
decrypted content is verified to be same at the end of the SMB2 READ
response.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

SessionId 0x8e40014000011  
SessionKey 0xB4546771B515F766A86735532DD6C4F0  
SigningKey 0xF773CD23C18FD1E08EE510CADA7CF852  
EncryptionKey (Client) 0x261B72350558F2E9DCF613070383EDBF  
DecryptionKey (Client) 0x8FE2B57EC34D2DB5B1A9727F526BBDB5  
ApplicationKey 0x77432F808CE99156B5BC6A3676D730D1

 

Header.Command 0x0009 WRITE  
Encryption of the request ---  
Key 0x261B72350558F2E9DCF613070383EDBF  
AES-128-CCM nonce 0x66E69A111892584FB5ED52  
AAD  
0x  
66E69A111892584FB5ED524A744DA3EE87000000000001001100001400E40800  
SMB2 packet  
0x  
FE534D4240000100000000000900400008000000000000000400000000000000  
FFFE0000010000001100001400E4080000000000000000000000000000000000  
3100700017000000000000000000000015010000390000020100000039020000  
00000000000000007000000000000000536D623320656E6372797074696F6E20  
74657374696E67  
transform\_header.Signature 0x81A286535415445DAE393921E44FA42E  
transform\_header.Nonce 0x66E69A111892584FB5ED524A744DA3EE  
transform\_header.OriginalMessageSize 0x87  
transform\_header.SessionId 0x8e40014000011  
Encrypted message  
0x  
25C8FEE16605A437832D1CD52DA9F4645333482A175FE5384563F45FCDAFAEF3  
8BC62BA4D5C62897996625A44C29BE5658DE2E6117585779E7B59FFD971278D0  
8580D7FA899E410E910EABF5AA1DB43050B33B49182637759AC15D84BFCDF5B6  
B238993C0F4CF4D6012023F6C627297075D84B7803912D0A9639634453595EF3  
E33FFE4E7AC2AB  
Transformed message  
0x  
FD534D4281A286535415445DAE393921E44FA42E66E69A111892584FB5ED524A  
744DA3EE87000000000001001100001400E4080025C8FEE16605A437832D1CD5  
2DA9F4645333482A175FE5384563F45FCDAFAEF38BC62BA4D5C62897996625A4  
4C29BE5658DE2E6117585779E7B59FFD971278D08580D7FA899E410E910EABF5  
AA1DB43050B33B49182637759AC15D84BFCDF5B6B238993C0F4CF4D6012023F6  
C627297075D84B7803912D0A9639634453595EF3E33FFE4E7AC2AB  
Decryption of the response ---  
Key 0x8FE2B57EC34D2DB5B1A9727F526BBDB5  
Transformed message  
0x  
FD534D42A6015530A18F6D9AFFE22AFAE8E66484860000000000000011000014  
00E4080050000000000001001100001400E40800DBF46435C5F14169293CE079  
E344479BF670227E49873F458672C3098DAC467DD5809F369D67409166515787  
1483E01F7BECD02064EAC3E235F913668BBC2F097980D4B378F1993EFF6E60D1  
77309E5B  
transform\_header.Signature 0xA6015530A18F6D9AFFE22AFAE8E66484  
transform\_header.Nonce 0x86000000000000001100001400E40800  
transform\_header.OriginalMessageSize 0x50  
transform\_header.SessionId 0x8e40014000011  
AES-128-CCM nonce 0x8600000000000000110000  
AAD  
0x  
86000000000000001100001400E4080050000000000001001100001400E40800  
Decrypted SMB2 packet  
0x  
FE534D4240000100000000000900210009000000000000000400000000000000  
FFFE0000010000001100001400E4080000000000000000000000000000000000  
11000000170000000000000000000000

 

Header.Command 0x0008 READ  
Encryption of the request ---  
Key 0x261B72350558F2E9DCF613070383EDBF  
AES-128-CCM nonce 0xA5123A25F983E245983F41  
AAD  
0x  
A5123A25F983E245983F413B8B429AF271000000000001001100001400E40800  
SMB2 packet  
0x  
FE534D4240000100000000000800400008000000000000000500000000000000  
FFFE0000010000001100001400E4080000000000000000000000000000000000  
3100000017000000000000000000000015010000390000020100000039020000  
0000000000000000000000000000000000  
transform\_header.Signature 0xE93601498B76D6F7A72D5EF9B6C79FAF  
transform\_header.Nonce 0xA5123A25F983E245983F413B8B429AF2  
transform\_header.OriginalMessageSize 0x71  
transform\_header.SessionId 0x8e40014000011  
Encrypted message  
0x  
9A464F709AA663F8C2FC3907D63CBF6F98B1E3DD649ED366009FD0B40A365224  
718E5440E053F6E01AE462FDB721BF91C3A6E52E14F9EFF005F445761289FF12  
72908B52754C8FCB949F228AC104A66204289A205BCBC47509D04AF9A907002B  
96863358B3B7CBA5E377930074FCDF3550  
Transformed message  
0x  
FD534D42E93601498B76D6F7A72D5EF9B6C79FAFA5123A25F983E245983F413B  
8B429AF271000000000001001100001400E408009A464F709AA663F8C2FC3907  
D63CBF6F98B1E3DD649ED366009FD0B40A365224718E5440E053F6E01AE462FD  
B721BF91C3A6E52E14F9EFF005F445761289FF1272908B52754C8FCB949F228A  
C104A66204289A205BCBC47509D04AF9A907002B96863358B3B7CBA5E3779300  
74FCDF3550  
Decryption of the response ---  
Key 0x8FE2B57EC34D2DB5B1A9727F526BBDB5  
Transformed message  
0x  
FD534D42ABD518B68C2F04D7879F482B689EB83F870000000000000011000014  
00E4080067000000000001001100001400E40800493D6FE2BDBEB435CF5F5469  
70C7BB57BF20E713C75A3D045507E0D68E5C0346659D6FFB8AC1504A786CA2BB  
89C9E7FE4F313E910A04180D2D0EA7DF636329E5A3285984500EF86FE9D55DA4  
FAB9531CFDD4C551D47F3C73124BB4590A45052B694048B991CCF5  
transform\_header.Signature 0xABD518B68C2F04D7879F482B689EB83F  
transform\_header.Nonce 0x87000000000000001100001400E40800  
transform\_header.OriginalMessageSize 0x67  
transform\_header.SessionId 0x8e40014000011  
AES-128-CCM nonce  0x8700000000000000110000  
AAD  
0x  
87000000000000001100001400E4080067000000000001001100001400E40800  
Decrypted SMB2
packet  
0x  
FE534D4240000100000000000800210009000000000000000500000000000000  
FFFE0000010000001100001400E4080000000000000000000000000000000000  
11005000170000000000000000000000536D623320656E6372797074696F6E20  
74657374696E67

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">References:</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri;">\[MS-SMB2\]: Server Message Block
(SMB) Protocol Versions 2 and 3 Specification</span>

<span style="font-family: Calibri;">\[SP800-108\] National Institute of
Standards and Technology. "Special Publication 800-108, Recommendation
for Key Derivation Using Pseudorandom Functions", October 2009,
</span>[<span style="font-family: Verdana;">http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf</span>](http://go.microsoft.com/fwlink/?LinkId=186039)

\[RFC5084\] Housley, R., "Using AES-CCM and AES-GCM Authenticated
Encryption in the Cryptographic Message Syntax (CMS)", RFC 5084,
November 2007,
[http://www.ietf.org/rfc/rfc5084.txt](http://go.microsoft.com/fwlink/?LinkId=229742)

<span style="font-family: Calibri;">SMB 3 Security Enhancements in
Windows Server
2012</span>

[<span style="font-family: Verdana;">http://blogs.technet.com/b/filecab/archive/2012/05/03/smb-3-security-enhancements-in-windows-server-2012.aspx</span>](http://blogs.technet.com/b/filecab/archive/2012/05/03/smb-3-security-enhancements-in-windows-server-2012.aspx)

<span style="font-family: Calibri;">Encryption in
SMB3</span>

[<span style="font-family: Verdana;">http://blogs.msdn.com/b/openspecification/archive/2012/06/08/encryption-in-smb3.aspx</span>](http://blogs.msdn.com/b/openspecification/archive/2012/06/08/encryption-in-smb3.aspx)

<span style="font-family: Calibri;"> </span>

</div>

</div>
