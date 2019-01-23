<div id="page">

# SMB 3.1.1 Encryption in Windows 10

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
9/9/2015 1:04:00 PM

-----

<div id="content">

<span style="font-size: small">SMB 3 encryption offers data packet
confidentiality and prevents an attacker from both tampering with and
eavesdropping on any data packet. Encryption has been enhanced in SMB
3.1.1. The cipher can now be negotiated during connection establishment.
In addition to AES-128-CCM for SMB 3.0.x compatibility, Windows 10 (and
Windows Server 2016) added AES-128-GCM in SMB 3.1.1. The GCM mode offers
a significant performance gain. Both ciphers \[RFC5084\] provide
authenticated encryption, i.e. message integrity (signing). This blog
takes a protocol walk on the enhancement and provides sample test
vectors. </span>

<span style="font-size: small">NOTE: This is written based on Windows
10, and Windows Server 2016 Technical Preview 3.</span>

## Encryption capability negotiate context

  - <span style="font-size: small">SMB 3.1.1 client and server negotiate
    encryption support via SMB2\_ENCRYPTION\_CAPABILITIES negotiate
    context in the NEGOTIATE request and response. </span>
  - <span style="font-size: small">The client advertises its list of
    supported cipher IDs in the order of most preferred encryption
    algorithm. Windows 10 implements cipher IDs 0x0002 (AES-128-GCM) and
    0x0001 (AES-128-CCM). </span>
  - <span style="font-size: small">If the client sends 3.0.x in the
    dialect array and supports encryption, it must advertise the
    SMB2\_GLOBAL\_CAP\_ENCRYPTION capability flag as well since it does
    not know yet what dialect the server supports.</span>
  - <span style="font-size: small">If the server selects dialect 3.1.1
    and supports encryption, it responds with the encryption negotiate
    context. SMB2\_GLOBAL\_CAP\_ENCRYPTION capability flag should not be
    set because it is valid for the SMB 3.0 and 3.0.2 dialects.</span>
  - <span style="font-size: small">The encryption capability negotiate
    context has the same format in the Negotiate request and response,
    except that the response context must have CipherCount set to 1, and
    the cipher ID that conveys the server’s selection. </span>

## Multichannel encryption

  - <span style="font-size: small">Session binding (multichannel)
    requires that all channels bound to a given session negotiate the
    same encryption cipher as the master session’s connection.</span>
  - <span style="font-size: small">As in SMB 3.0.x, all SMB 3.1.1
    channels (primary and alternate) bound to a given master session
    share the same encryption and decryption keys.</span>
  - <span style="font-size: small">The cipher ID is kept consistent as
    multichannel traffic can be spread across channels.</span>

## Server-wide or per-share encryption

  - <span style="font-size: small">For a global level encryption,
    SessionSetup\_response.SessionFlags includes
    SMB2\_SESSION\_FLAG\_ENCRYPT\_DATA. Encryption is enforced on the
    whole server.</span>
  - <span style="font-size: small">For a per-share level encryption,
    TreeConnect\_response.ShareFlags includes
    SMB2\_SHAREFLAG\_ENCRYPT\_DATA.</span>
  - <span style="font-size: small">If server-wide encryption is
    configured, share level encryption will not have any effect. This
    means, when global encryption is enabled, you cannot selectively
    disable encryption for certain shares.</span>
  - <span style="font-size: small">Encryption enforcement can be
    bypassed if “unencrypted access” is allowed. See
    RejectUnencryptedAccess.</span>

## Configuration

<span style="font-size: small">PowerShell cmdlets can be used to
configure encryption on both Windows 10 and Windows Server 2016:</span>

<span style="font-size: small">Global level encryption on the server:
</span>

<span style="font-size: small">Set-SmbServerConfiguration -EncryptData
\<0|1\></span>

<span style="font-size: small">Share level encryption: </span>

<span style="font-size: small">Set-SmbShare -Name \<share name\>
-EncryptData \<0|1\></span>

<span style="font-size: small">Share level encryption enabled at
creation:</span>

<span style="font-size: small">New-SmbShare -Name \<share name\> -Path
\<pathname\> -EncryptData 1</span>

<span style="font-size: small">Unencrypted access:</span>

<span style="font-size: small">Set-SmbServerConfiguration
-RejectUnencryptedAccess \<0|1\></span>

<span style="font-size: small">See notes in Section “Unencrypted
access”.</span>

## Encryption keys

<span style="font-size: small">Upon successful SessionSetup
(Session.EncryptData), or successful TreeConnect (Share.EncryptData),
the server and client generate EncryptionKey and DecryptionKey as
specified in \[MS-SMB2\]. </span>

<span style="font-size: small">The key derivation in SMB 3.1.1 uses the
same function as in SMB 3.0.x with new labels and context as follows.
The context Session.PreauthIntegrityHashValue is derived from
pre-authentication integrity hashing. The respective labels are shown in
the following key formulas.</span>

  - <span style="font-size: small">EncryptionKey (Client) =
    DecryptionKey (Server) = SMB3KDF (SessionKey, "SMBC2SCipherKey\\0",
    Session.PreauthIntegrityHashValue)</span>
  - <span style="font-size: small">DecryptionKey (Client) =
    EncryptionKey (Server) = SMB3KDF ( SessionKey, "SMBS2CCipherKey\\0",
    Session.PreauthIntegrityHashValue)</span>

<span style="font-size: small">Note that SMB3KDF() and the calculation
of Session.PreauthIntegrityHashValue are described in the blog
post:</span>

<span style="font-size: small">SMB 3.1.1 Pre-authentication integrity in
Windows
10</span>

<span style="font-size: small"><http://blogs.msdn.com/b/openspecification/archive/2015/08/11/smb-3-1-1-pre-authentication-integrity-in-windows-10.aspx></span>

## Transformed message

<span style="font-size: small">A transformed message consists of a
transform\_header followed by its encrypted SMB2 message.</span>

<span style="font-size: small">A transform\_header has the same size and
most of the same fields as defined for dialect 3.0.x with two specific
changes for dialect 3.1.1:</span>

<span style="font-size: small">ProtocolId (4 bytes):  0xFD534D42 (in
network order)</span>

<span style="font-size: small">Signature (16 bytes):  Signature of the
encrypted message.</span>

<span style="font-size: small">Nonce (16 bytes):  An
implementation-specific value unique for this encrypted message. </span>

<span style="font-size: small">OriginalMessageSize (4 bytes):  The size
in bytes of the SMB2 message.</span>

<span style="font-size: small">Reserved (2 bytes):  Set to zeros and
ignored. </span>

<span style="font-size: small">Flags (2 bytes): This field indicates how
the SMB2 message was transformed.</span>

<span style="font-size: small">SessionId (8 bytes):  Uniquely identifies
the established session for the command.</span>

<span style="font-size: small">The two changes to the transform\_header
for SMB 3.1.1 are as follows:</span>

<span style="font-size: small">Nonce (16 bytes) field: If CipherId is
AES-128-GCM, the nonce used for encryption is the leftmost 12 bytes of
the Nonce field, AES128GCM\_Nonce (12 bytes), and the remaining 4 bytes
are reserved. If CipherId is AES-128-CCM, the nonce used for encryption
is the leftmost 11 bytes of the Nonce field, AES128CCM\_Nonce (11
bytes), and the remaining 5 bytes are reserved.</span>

<span style="font-size: small">Flags (2 bytes): this field repurposes
the EncryptionAlgorithm (2 bytes) field used in SMB 3.0.x. When Flags’
value is set to 0x0001, it indicates that the message is encrypted using
the negotiated cipher ID.</span>

## Encrypting the message

<span style="font-size: small">The sender encrypts the message with
these specifics:</span>

<span style="font-size: small">- The encryption algorithm specified by
Connection.CipherId (AES-GCM or AES-CCM) is called with the following
inputs:</span>

<span style="font-size: small">- AES key: Session.EncryptionKey.</span>

<span style="font-size: small">- AES-nonce or IV: AES128CCM\_Nonce for
AES-CCM, AES128GCM\_Nonce for AES-GCM.</span>

<span style="font-size: small">- Plaintext: The SMB2 message including
the header and the payload.</span>

<span style="font-size: small">- The optional authenticated data (AAD): 
The SMB2 transform\_header excluding the ProtocolId and Signature
fields; these are the 32 bytes starting from the Nonce field.</span>

<span style="font-size: small">The AES-CCM or AES-GCM outputs are:
</span>

<span style="font-size: small">- Ciphertext: the encrypted SMB2
message</span>

<span style="font-size: small">- Message authentication code: the
Signature field of the transform\_header.</span>

<span style="font-size: small">The sender appends the encrypted SMB2
message to the transform\_header and sends it to the receiver.</span>

## Decrypting the message

<span style="font-size: small">The message is decrypted using: </span>

<span style="font-size: small">- The encryption algorithm specified by
Connection.CipherId.</span>

<span style="font-size: small">- The Session.DecryptionKey of the
Session that corresponds to the SessionId in the
transform\_header.</span>

<span style="font-size: small">- The AAD passed to the algorithm is the
transform\_header excluding the ProtocolId and Signature fields.</span>

<span style="font-size: small">- The nonce passed to the algorithm is
based on CipherId as previously described.</span>

<span style="font-size: small">The signature returned by the decryption
algorithm is then verified against the Signature in the
transform\_header.</span>

## Encryption clauses

<span style="font-size: small">The encryption clauses are generally the
same as for SMB 3.0.x. See the blog post at:</span>

<span style="font-size: small">Encryption in SMB 3.0: A protocol
perspective</span>

<span style="font-size: small"><http://blogs.msdn.com/b/openspecification/archive/2012/10/05/encryption-in-smb-3-0-a-protocol-perspective.aspx></span>

## Unencrypted Access: RejectUnencryptedAccess

<span style="font-size: small">Set-SmbServerConfiguration
-RejectUnencryptedAccess \<0|1\></span>

<span style="font-size: small">The behavior is mainly unchanged in
Windows Server 2016. The default value of RejectUnencryptedAccess is
TRUE. When encryption is required (per share or server wide), the server
returns ACCESS\_DENIED for an unencrypted access attempt. A value of
FALSE allows access from clients which do not support encryption or are
not encryption-capable (e.g. SMB 1, SMB2 dialects 2.02, 2.1, SMB 3.x
without encryption).</span>

<span style="font-size: small">This configuration item is meant for a
transition phase to support down-level clients which use older dialects
(SMB 2.1 or earlier) by literally “allowing any unencrypted access"
whenever the deployment scenario requires. </span>

<span style="font-size: small">Note that if RejectUnencryptedAccess is
disabled (FALSE), it opens the possibility for a man-in-the-middle
(MITM) attacker to prevent the connection from negotiating encryption.
If SMB 3.x is negotiated, the Windows 10 client will leverage negotiate
validation or pre-authentication integrity to verify the properties of
the connection with the server. In SMB 2.x.x, the encryption feature is
not available even if the Windows 10 client still performs secure
negotiate validation.</span>

<span style="font-size: small">However, setting RejectUnencryptedAccess
to FALSE makes it un-detectible if the MITM downgrades the connection's
dialect to SMB1, thus preventing encryption negotiation and allowing
“clear text” eavesdropping.  </span>

<span style="font-size: small">As a result, it is recommended to disable
SMB1 on the client as soon as it is no longer needed.</span>

## Conclusion

<span style="font-size: small">Encryption is a very important feature
whenever data confidentiality is required. This is the case when
transferring high business impact data over untrusted networks. The
addition of encryption negotiate context in SMB 3.1.1 is an enhancement
that would interest many implementers. It also makes the feature
extensible as new cryptographic ciphers become available and adopted.
Some benchmark testing showed that AES-128-GCM provides as much as two
times performance improvement over AES-128-CCM, while providing
authenticated encryption at the same time.</span>

## Appendix A. Test vector for SMB 3.1.1 encryption

<span style="font-size: small">This sample data should be considered
“as-is”. It should also be noted that examples do not replace
normative protocol specifications. The authoritative reference is
\[MS-SMB2\].</span>

<span style="font-size: small">The test client negotiates SMB 3.1.1 and
communicates with a Windows 2016 server. It opens a file and WRITEs the
following content. It then READs back the file. </span>

<span style="font-size: small">This is the content written and
read:</span>

<span style="font-size: small">Smb3 encryption testing</span>

<span style="font-size: small">Hex
value:</span>

<span style="font-size: small">536D623320656E6372797074696F6E2074657374696E67</span>

<span style="font-size: small">These outputs show pre-authentication
integrity phase for key derivation, then the encryption and decryption
of the WRITE and READ commands.</span>

<span style="font-size: small">The decrypted content is verified to be
same at the end of the SMB2 READ response.</span>

### <span>Appendix A.1 Test vector with AES-GCM</span>

<span style="font-size: small">--- Key derivation ---</span>

<span style="font-size: small">Header.Command 0x0000 NEGOTIATE</span>

<span style="font-size: small">Preauth integrity hash ---</span>

<span style="font-size: small">PreauthIntegrityCaps.HashAlgorithmCount
0x1</span>

<span style="font-size: small">PreauthIntegrityCaps.SaltLength
0x20</span>

<span style="font-size: small">PreauthIntegrityCaps.HashAlgorithms
0x0001</span>

<span style="font-size: small">PreauthIntegrityCaps.Salt
</span>

<span style="font-size: small">D1709D7196E1BD0B6EBF95213D76553435763514392649FD6F216ED8BF269CD8</span>

<span style="font-size: small">Encryption capabilites ---</span>

<span style="font-size: small">EncryptionCaps.CipherCount 0x2</span>

<span style="font-size: small">EncryptionCaps.Ciphers\[0\] 0x0002</span>

<span style="font-size: small">EncryptionCaps.Ciphers\[1\] 0x0001</span>

<span style="font-size: small">Connection.PreauthIntegrityHashId
0x0001</span>

<span style="font-size: small">NEGOTIATE Request</span>

<span style="font-size: small">Preauth integrity hash ---</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">Negotiate request packet
</span>

<span style="font-size: small">FE534D4240000100000000000000010000000000000000000000000000000000FFFE000000000000</span>

<span style="font-size: small">0000000000000000000000000000000000000000000000002400050001000000660000004F0D7FA0</span>

<span style="font-size: small">09F5B246B2EF62551D7D7C0970000000020000000202100200030203110300000100260000000000</span>

<span style="font-size: small">010020000100D1709D7196E1BD0B6EBF95213D76553435763514392649FD6F216ED8BF269CD80000</span>

<span style="font-size: small">0200060000000000020002000100</span>

<span style="font-size: small">Concatenate
Connection.PreauthIntegrityHashValue and Negotiate request packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000FE534D42400001000000000000000100</span>

<span style="font-size: small">00000000000000000000000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">00000000000000002400050001000000660000004F0D7FA009F5B246B2EF62551D7D7C0970000000</span>

<span style="font-size: small">020000000202100200030203110300000100260000000000010020000100D1709D7196E1BD0B6EBF</span>

<span style="font-size: small">95213D76553435763514392649FD6F216ED8BF269CD800000200060000000000020002000100</span>

<span style="font-size: small">New </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">550442DAF311412870AD9E58E602B0312D61328D6B1AC28F22AF46D6EA581F23A9BFABE0CC041197</span>

<span style="font-size: small">6BF3F9DA23D3433352CB48CF00B8659BC1A3695E1B1A52A8</span>

<span style="font-size: small">NEGOTIATE Response</span>

<span style="font-size: small">Updating Preauth integrity hash
---</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">550442DAF311412870AD9E58E602B0312D61328D6B1AC28F22AF46D6EA581F23A9BFABE0CC041197</span>

<span style="font-size: small">6BF3F9DA23D3433352CB48CF00B8659BC1A3695E1B1A52A8</span>

<span style="font-size: small">Negotiate response packet
</span>

<span style="font-size: small">FE534D4240000100000000000000010001000000000000000000000000000000FFFE000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000410001001103020039CBCAF329714942</span>

<span style="font-size: small">BDCE5D60F09AB3FB27000000000080000000800000008000D1168E69CDAED00109094AB095AED001</span>

<span style="font-size: small">80004001C00100006082013C06062B0601050502A08201303082012CA01A3018060A2B0601040182</span>

<span style="font-size: small">3702021E060A2B06010401823702020AA282010C048201084E45474F455854530100000000000000</span>

<span style="font-size: small">6000000070000000807CC0FD06D6362D02DDE1CF343BFE29C16AA4EA4741FB0EF645DC5C5D3C3E6A</span>

<span style="font-size: small">8DE5D0BAEF7A06DC070076174356EDA0000000000000000060000000010000000000000000000000</span>

<span style="font-size: small">5C33530DEAF90D4DB2EC4AE3786EC3084E45474F4558545303000000010000004000000098000000</span>

<span style="font-size: small">807CC0FD06D6362D02DDE1CF343BFE295C33530DEAF90D4DB2EC4AE3786EC3084000000058000000</span>

<span style="font-size: small">3056A05430523027802530233121301F06035504031318546F6B656E205369676E696E6720507562</span>

<span style="font-size: small">6C6963204B65793027802530233121301F06035504031318546F6B656E205369676E696E67205075</span>

<span style="font-size: small">626C6963204B65790100260000000000010020000100B51C002C28941192737A08344B05CE90786E</span>

<span style="font-size: small">EC146D99CDB60AE44E5A86127D270000020004000000000001000200</span>

<span style="font-size: small">Concatenate
Connection.PreauthIntegrityHashValue and Negotiate response
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">550442DAF311412870AD9E58E602B0312D61328D6B1AC28F22AF46D6EA581F23A9BFABE0CC041197</span>

<span style="font-size: small">6BF3F9DA23D3433352CB48CF00B8659BC1A3695E1B1A52A8FE534D42400001000000000000000100</span>

<span style="font-size: small">01000000000000000000000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">0000000000000000410001001103020039CBCAF329714942BDCE5D60F09AB3FB2700000000008000</span>

<span style="font-size: small">0000800000008000D1168E69CDAED00109094AB095AED00180004001C00100006082013C06062B06</span>

<span style="font-size: small">01050502A08201303082012CA01A3018060A2B06010401823702021E060A2B06010401823702020A</span>

<span style="font-size: small">A282010C048201084E45474F4558545301000000000000006000000070000000807CC0FD06D6362D</span>

<span style="font-size: small">02DDE1CF343BFE29C16AA4EA4741FB0EF645DC5C5D3C3E6A8DE5D0BAEF7A06DC070076174356EDA0</span>

<span style="font-size: small">0000000000000000600000000100000000000000000000005C33530DEAF90D4DB2EC4AE3786EC308</span>

<span style="font-size: small">4E45474F4558545303000000010000004000000098000000807CC0FD06D6362D02DDE1CF343BFE29</span>

<span style="font-size: small">5C33530DEAF90D4DB2EC4AE3786EC30840000000580000003056A05430523027802530233121301F</span>

<span style="font-size: small">06035504031318546F6B656E205369676E696E67205075626C6963204B6579302780253023312130</span>

<span style="font-size: small">1F06035504031318546F6B656E205369676E696E67205075626C6963204B65790100260000000000</span>

<span style="font-size: small">010020000100B51C002C28941192737A08344B05CE90786EEC146D99CDB60AE44E5A86127D270000</span>

<span style="font-size: small">020004000000000001000200</span>

<span style="font-size: small">New </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">ABE4DA6E875F6FB05033AF04DCC38C92888B4E13D1EAB7AA05CADE142064974CB3EAB0782600549B</span>

<span style="font-size: small">A27207AA213B0D190B9950FA36D45BE32A888BFEE8389B74</span>

<span style="font-size: small">Add NEW SessionId 0x100000000025 to
Preauth Integrity hash table with value </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">ABE4DA6E875F6FB05033AF04DCC38C92888B4E13D1EAB7AA05CADE142064974CB3EAB0782600549B</span>

<span style="font-size: small">A27207AA213B0D190B9950FA36D45BE32A888BFEE8389B74</span>

<span style="font-size: small">SESSION SETUP Request </span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000025</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">ABE4DA6E875F6FB05033AF04DCC38C92888B4E13D1EAB7AA05CADE142064974CB3EAB0782600549B</span>

<span style="font-size: small">A27207AA213B0D190B9950FA36D45BE32A888BFEE8389B74</span>

<span style="font-size: small">SessionSetup request packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800000000000000000000100000000000000FFFE000000000000</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000019000001010000000000000058004A00</span>

<span style="font-size: small">0000000000000000604806062B0601050502A03E303CA00E300C060A2B06010401823702020AA22A</span>

<span style="font-size: small">04284E544C4D5353500001000000978208E200000000000000000000000000000000060380250000</span>

<span style="font-size: small">000F</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup request
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data 
</span>

<span style="font-size: small">ABE4DA6E875F6FB05033AF04DCC38C92888B4E13D1EAB7AA05CADE142064974CB3EAB0782600549B</span>

<span style="font-size: small">A27207AA213B0D190B9950FA36D45BE32A888BFEE8389B74FE534D42400001000000000001008000</span>

<span style="font-size: small">00000000000000000100000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000019000001010000000000000058004A000000000000000000604806062B060105</span>

<span style="font-size: small">0502A03E303CA00E300C060A2B06010401823702020AA22A04284E544C4D53535000010000009782</span>

<span style="font-size: small">08E200000000000000000000000000000000060380250000000F</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A5E8AB87E2ADB8FA5F4545D20F1FD2019D66CCD0F4DFD1F762F1DFC8DCB15B98D0BD1F1450F6A0AF</span>

<span style="font-size: small">C70F80B353C2D959217681949CF22DF35F31257A281C6A80</span>

<span style="font-size: small">SESSION SETUP Response</span>

<span style="font-size: small"> --- STATUS\_MORE\_PROCESSING\_REQUIRED -
Updating Preauth integrity hash ---</span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000025</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A5E8AB87E2ADB8FA5F4545D20F1FD2019D66CCD0F4DFD1F762F1DFC8DCB15B98D0BD1F1450F6A0AF</span>

<span style="font-size: small">C70F80B353C2D959217681949CF22DF35F31257A281C6A80</span>

<span style="font-size: small">SessionSetup response packet
</span>

<span style="font-size: small">FE534D4240000100160000C00100010001000000000000000100000000000000FFFE000000000000</span>

<span style="font-size: small">250000000010000000000000000000000000000000000000090000004800B300A181B03081ADA003</span>

<span style="font-size: small">0A0101A10C060A2B06010401823702020AA281970481944E544C4D53535000020000000C000C0038</span>

<span style="font-size: small">00000015828AE25FC0CB7F886E93D6000000000000000050005000440000000A0092270000000F53</span>

<span style="font-size: small">005500540033003100310002000C0053005500540033003100310001000C00530055005400330031</span>

<span style="font-size: small">00310004000C0053005500540033003100310003000C005300550054003300310031000700080024</span>

<span style="font-size: small">8D5C6CCDAED00100000000</span>

<span style="font-size: small">SessionSetup response header signature
0x00000000000000000000000000000000</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup response
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">A5E8AB87E2ADB8FA5F4545D20F1FD2019D66CCD0F4DFD1F762F1DFC8DCB15B98D0BD1F1450F6A0AF</span>

<span style="font-size: small">C70F80B353C2D959217681949CF22DF35F31257A281C6A80FE534D4240000100160000C001000100</span>

<span style="font-size: small">01000000000000000100000000000000FFFE00000000000025000000001000000000000000000000</span>

<span style="font-size: small">0000000000000000090000004800B300A181B03081ADA0030A0101A10C060A2B0601040182370202</span>

<span style="font-size: small">0AA281970481944E544C4D53535000020000000C000C003800000015828AE25FC0CB7F886E93D600</span>

<span style="font-size: small">0000000000000050005000440000000A0092270000000F53005500540033003100310002000C0053</span>

<span style="font-size: small">005500540033003100310001000C0053005500540033003100310004000C00530055005400330031</span>

<span style="font-size: small">00310003000C0053005500540033003100310007000800248D5C6CCDAED00100000000</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">9A095455244172898902B0FBDF5FEFAFD8435BB66A47EB55CB7542732A423F58B12B3ED698BEF387</span>

<span style="font-size: small">8D8A346FD9F5CC882DA37AAF2A939290E98B935FC72B3944</span>

<span style="font-size: small">SESSION SETUP Request </span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000025</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">9A095455244172898902B0FBDF5FEFAFD8435BB66A47EB55CB7542732A423F58B12B3ED698BEF387</span>

<span style="font-size: small">8D8A346FD9F5CC882DA37AAF2A939290E98B935FC72B3944</span>

<span style="font-size: small">SessionSetup request packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800000000000000000000200000000000000FFFE000000000000</span>

<span style="font-size: small">2500000000100000000000000000000000000000000000001900000101000000000000005800CF01</span>

<span style="font-size: small">0000000000000000A18201CB308201C7A0030A0101A28201AA048201A64E544C4D53535000030000</span>

<span style="font-size: small">001800180090000000EE00EE00A80000000C000C00580000001A001A0064000000120012007E0000</span>

<span style="font-size: small">001000100096010000158288E2060380250000000FA5E34268EF143BE5816251D02C564E9B530055</span>

<span style="font-size: small">005400330031003100610064006D0069006E006900730074007200610074006F0072004400520049</span>

<span style="font-size: small">005600450052003300310031000000000000000000000000000000000000000000000000002C263D</span>

<span style="font-size: small">A5C2D54785E8EDA0552472D3A30101000000000000248D5C6CCDAED001BEA7A53E2DC098EB000000</span>

<span style="font-size: small">0002000C0053005500540033003100310001000C0053005500540033003100310004000C00530055</span>

<span style="font-size: small">00540033003100310003000C0053005500540033003100310007000800248D5C6CCDAED001060004</span>

<span style="font-size: small">00020000000800300030000000000000000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B</span>

<span style="font-size: small">8E0EBA5A6DFD9D07A31D11B548F8C9D0CC0A00100000000000000000000000000000000000090016</span>

<span style="font-size: small">0063006900660073002F00530055005400330031003100000000000000000000000000133FA6EA15</span>

<span style="font-size: small">4880BB44576C6E2490BDE7A31204100100000067890BD408F5680D00000000</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup request
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data 
</span>

<span style="font-size: small">9A095455244172898902B0FBDF5FEFAFD8435BB66A47EB55CB7542732A423F58B12B3ED698BEF387</span>

<span style="font-size: small">8D8A346FD9F5CC882DA37AAF2A939290E98B935FC72B3944FE534D42400001000000000001008000</span>

<span style="font-size: small">00000000000000000200000000000000FFFE00000000000025000000001000000000000000000000</span>

<span style="font-size: small">00000000000000001900000101000000000000005800CF010000000000000000A18201CB308201C7</span>

<span style="font-size: small">A0030A0101A28201AA048201A64E544C4D53535000030000001800180090000000EE00EE00A80000</span>

<span style="font-size: small">000C000C00580000001A001A0064000000120012007E0000001000100096010000158288E2060380</span>

<span style="font-size: small">250000000FA5E34268EF143BE5816251D02C564E9B530055005400330031003100610064006D0069</span>

<span style="font-size: small">006E006900730074007200610074006F007200440052004900560045005200330031003100000000</span>

<span style="font-size: small">0000000000000000000000000000000000000000002C263DA5C2D54785E8EDA0552472D3A3010100</span>

<span style="font-size: small">0000000000248D5C6CCDAED001BEA7A53E2DC098EB0000000002000C005300550054003300310031</span>

<span style="font-size: small">0001000C0053005500540033003100310004000C0053005500540033003100310003000C00530055</span>

<span style="font-size: small">00540033003100310007000800248D5C6CCDAED00106000400020000000800300030000000000000</span>

<span style="font-size: small">000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B8E0EBA5A6DFD9D07A31D11B548F8C9D0</span>

<span style="font-size: small">CC0A001000000000000000000000000000000000000900160063006900660073002F005300550054</span>

<span style="font-size: small">00330031003100000000000000000000000000133FA6EA154880BB44576C6E2490BDE7A312041001</span>

<span style="font-size: small">00000067890BD408F5680D00000000</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">B23F3CBFD69487D9832B79B1594A367CDD950909B774C3A4C412B4FCEA9EDDDBA7DB256BA2EA30E9</span>

<span style="font-size: small">77F11F9B113247578E0E915C6D2A513B8F2FCA5707DC8770</span>

<span style="font-size: small">SESSION SETUP Response </span>

<span style="font-size: small">SessionId 0x100000000025 COMPLETED</span>

<span style="font-size: small">SessionSetup response packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800009000000000000000200000000000000FFFE000000000000</span>

<span style="font-size: small">25000000001000006B85A4519A0F3EEA35BA946DD3AFE6B80900000048001D00A11B3019A0030A01</span>

<span style="font-size: small">00A3120410010000003932A87523AB660100000000</span>

<span style="font-size: small">SessionSetup response header signature
0x6B85A4519A0F3EEA35BA946DD3AFE6B8</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">B23F3CBFD69487D9832B79B1594A367CDD950909B774C3A4C412B4FCEA9EDDDBA7DB256BA2EA30E9</span>

<span style="font-size: small">77F11F9B113247578E0E915C6D2A513B8F2FCA5707DC8770</span>

<span style="font-size: small">Input cryptographicKey (SessionKey)
0x419FDDF34C1E001909D362AE7FB6AF79</span>

<span style="font-size: small">(queried from GSS authenticated
context)</span>

<span style="font-size: small">--- Dialect 0x0311 ---</span>

<span style="font-size: small">preauthIntegrityHashValue
</span>

<span style="font-size: small">B23F3CBFD69487D9832B79B1594A367CDD950909B774C3A4C412B4FCEA9EDDDBA7DB256BA2EA30E9</span>

<span style="font-size: small">77F11F9B113247578E0E915C6D2A513B8F2FCA5707DC8770</span>

<span style="font-size: small">CypherId 0x0002</span>

<span style="font-size: small">SessionKey
0x419FDDF34C1E001909D362AE7FB6AF79</span>

<span style="font-size: small">SigningKey
0x8765949DFEAEE105CE9118B45BE988F0</span>

<span style="font-size: small">EncryptionKey
0xA2F5E80E5D59103034F32E52F698E5EC</span>

<span style="font-size: small">DecryptionKey
0x748C50868C90F302962A5C35F5F9A8BF</span>

<span style="font-size: small">ApplicationKey
0x099D610789FBE82055B313601C3E8CC4</span>

<span style="font-size: small">--- Encryption ---</span>

<span style="font-size: small">SessionId 0x100000000025</span>

<span style="font-size: small">SessionKey
0x419FDDF34C1E001909D362AE7FB6AF79</span>

<span style="font-size: small">SigningKey
0x8765949DFEAEE105CE9118B45BE988F0</span>

<span style="font-size: small">EncryptionKey
0xA2F5E80E5D59103034F32E52F698E5EC</span>

<span style="font-size: small">DecryptionKey
0x748C50868C90F302962A5C35F5F9A8BF</span>

<span style="font-size: small">ApplicationKey
0x099D610789FBE82055B313601C3E8CC4</span>

<span style="font-size: small">Header.Command 0x0009 WRITE</span>

<span style="font-size: small">Encryption of the request ---</span>

<span style="font-size: small">Key
0xA2F5E80E5D59103034F32E52F698E5EC</span>

<span style="font-size: small">Nonce Length 0xc</span>

<span style="font-size: small">AES-128-GCM nonce
0xC7D6822D269CAF48904C664C</span>

<span style="font-size: small">SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000900010008000000000000000500000000000000FFFE000001000000</span>

<span style="font-size: small">25000000001000000000000000000000000000000000000031007000170000000000000000000000</span>

<span style="font-size: small">0600000004000000010000000400000000000000000000007000000000000000536D623320656E63</span>

<span style="font-size: small">72797074696F6E2074657374696E67</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0xBD73D97D2BC9001BCAFAC0FDFF5FEEBC</span>

<span style="font-size: small">transform\_header.Nonce
0xC7D6822D269CAF48904C664C00000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x87</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000025</span>

<span style="font-size: small">Encrypted message
</span>

<span style="font-size: small">6ECDD2A7AFC7B47763057A041B8FD4DAFFE990B70C9E09D36C084E02D14EF247F8BDE38ACF6256F8</span>

<span style="font-size: small">B1D3B56F77FBDEB312FEA5E92CBCC1ED8FB2EBBFAA75E49A4A394BB44576545567C24D4C014D47C9</span>

<span style="font-size: small">FBDFDAFD2C4F9B72F8D256452620A299F48E29E53D6B61D1C13A19E91AF013F00D17E3ABC2FC3D36</span>

<span style="font-size: small">C8C1B6B93973253852DBD442E46EE8</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D42BD73D97D2BC9001BCAFAC0FDFF5FEEBCC7D6822D269CAF48904C664C0000000087000000</span>

<span style="font-size: small">0000010025000000001000006ECDD2A7AFC7B47763057A041B8FD4DAFFE990B70C9E09D36C08<span style="font-size: small">4E02</span></span>

<span style="font-size: small">D14EF247F8BDE38ACF6256F8B1D3B56F77FBDEB312FEA5E92CBCC1ED8FB2EBBFAA75E49A4A39<span style="font-size: small">4BB4</span></span>

<span style="font-size: small">4576545567C24D4C014D47C9FBDFDAFD2C4F9B72F8D256452620A299F48E29E53D6B61D1C13A<span style="font-size: small">19E9</span></span>

<span style="font-size: small">1AF013F00D17E3ABC2FC3D36C8C1B6B93973253852DBD442E46EE8</span>

<span style="font-size: small">Decryption of the response ---</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D42ACBE1CB7ED343ADF1725EF144D90D4B0E06831DD2E8EB7B4000000000000000050000000</span>

<span style="font-size: small">00000100250000000010000026BBBF949983A6C1C796559D0F2C510CB651D1F7B6AC8DED32A2A0B8</span>

<span style="font-size: small">F2D793A815C6F6B848D69767A215841A42D400AE6DDB5F0B44173A014973321FDD7950DA6179159B</span>

<span style="font-size: small">82E03C9E18A050FF0EA1C967</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0xACBE1CB7ED343ADF1725EF144D90D4B0</span>

<span style="font-size: small">transform\_header.Nonce
0xE06831DD2E8EB7B40000000000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x50</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000025</span>

<span style="font-size: small">Key
0x748C50868C90F302962A5C35F5F9A8BF</span>

<span style="font-size: small">Nonce Length 0xc</span>

<span style="font-size: small">AES-128-GCM nonce
0xE06831DD2E8EB7B400000000</span>

<span style="font-size: small">Decrypted SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000900010001000000000000000500000000000000FFFE000001000000</span>

<span style="font-size: small">25000000001000000000000000000000000000000000000011000000170000000000000000000000</span>

<span style="font-size: small">Header.Command 0x0008 READ</span>

<span style="font-size: small">Encryption of the request ---</span>

<span style="font-size: small">Key
0xA2F5E80E5D59103034F32E52F698E5EC</span>

<span style="font-size: small">Nonce Length 0xc</span>

<span style="font-size: small">AES-128-GCM nonce
0xD7AA8C6D36859243B715E0A6</span>

<span style="font-size: small">SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000800010008000000000000000600000000000000FFFE000001000000</span>

<span style="font-size: small">25000000001000000000000000000000000000000000000031000000170000000000000000000000</span>

<span style="font-size: small">060000000400000001000000040000000000000000000000000000000000000000</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0x6DAC0B6FD85A3ED42BB917DA38FE0386</span>

<span style="font-size: small">transform\_header.Nonce
0xD7AA8C6D36859243B715E0A600000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x71</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000025</span>

<span style="font-size: small">Encrypted message
</span>

<span style="font-size: small">88A47BF09CA3C3141CDD7306BE9D9475AB24FCCB833D77461C041F8FB983D0C188F0729272B31D9D</span>

<span style="font-size: small">3D0DC6B687C069EEE0CC8EACA2C536D019ACC9E185D1EB630E0FCB793EEECEB06D82A1D77706E700</span>

<span style="font-size: small">DBEBFB4FEB54D7AD2D97E7288804F90757FE4D08D6A84A3FF433E7451E768E4699</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D426DAC0B6FD85A3ED42BB917DA38FE0386D7AA8C6D36859243B715E0A60000000071000000</span>

<span style="font-size: small">00000100250000000010000088A47BF09CA3C3141CDD7306BE9D9475AB24FCCB833D77461C04<span style="font-size: small">1F8F</span></span>

<span style="font-size: small">B983D0C188F0729272B31D9D3D0DC6B687C069EEE0CC8EACA2C536D019ACC9E185D1EB630E0F<span style="font-size: small">CB79</span></span>

<span style="font-size: small">3EEECEB06D82A1D77706E700DBEBFB4FEB54D7AD2D97E7288804F90757FE4D08D6A84A3FF433<span style="font-size: small">E745</span></span>

<span style="font-size: small">1E768E4699</span>

<span style="font-size: small">Decryption of the response ---</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D427F714B3B9D8FA1198584E71C2BAA1CB6E16831DD2E8EB7B4000000000000000067000000</span>

<span style="font-size: small">000001002500000000100000FECEDF4D03BB11A6CC5D8A53BE33D6D8701986342B4197D306E16F9C</span>

<span style="font-size: small">BB218E92F7F8281F51CE68BB85A20D87DE90EBBF80538066D1C37513C0A58D70936D537B624F5500</span>

<span style="font-size: small">202A612B6CD30D448A82791A0B2E049ED512AFAEFB06E98AB3D6F931D7D50DB2DBD36A</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0x7F714B3B9D8FA1198584E71C2BAA1CB6</span>

<span style="font-size: small">transform\_header.Nonce
0xE16831DD2E8EB7B40000000000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x67</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000025</span>

<span style="font-size: small">Key
0x748C50868C90F302962A5C35F5F9A8BF</span>

<span style="font-size: small">Nonce Length 0xc</span>

<span style="font-size: small">AES-128-GCM nonce
0xE16831DD2E8EB7B400000000</span>

<span style="font-size: small">Decrypted SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000800010001000000000000000600000000000000FFFE000001000000</span>

<span style="font-size: small">25000000001000000000000000000000000000000000000011005000170000000000000000000000</span>

<span style="font-size: small">536D623320656E6372797074696F6E2074657374696E67</span>

### Appendix A.2 Test vector with AES-CCM

<span style="font-size: small">--- Key derivation ---</span>

<span style="font-size: small">Header.Command 0x0000 NEGOTIATE</span>

<span style="font-size: small">Preauth integrity hash ---</span>

<span style="font-size: small">PreauthIntegrityCaps.HashAlgorithmCount
0x1</span>

<span style="font-size: small">PreauthIntegrityCaps.SaltLength
0x20</span>

<span style="font-size: small">PreauthIntegrityCaps.HashAlgorithms
0x0001</span>

<span style="font-size: small">PreauthIntegrityCaps.Salt
</span>

<span style="font-size: small">1A05A92392E1554C072AE7B186EE7DC02CB90BEF2E639CCC94B7A9DC7B393442</span>

<span style="font-size: small">Encryption capabilites ---</span>

<span style="font-size: small">EncryptionCaps.CipherCount 0x2</span>

<span style="font-size: small">EncryptionCaps.Ciphers\[0\] 0x0001</span>

<span style="font-size: small">EncryptionCaps.Ciphers\[1\] 0x0002</span>

<span style="font-size: small">Connection.PreauthIntegrityHashId
0x0001</span>

<span style="font-size: small">NEGOTIATE Request</span>

<span style="font-size: small">Preauth integrity hash ---</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">Negotiate request packet
</span>

<span style="font-size: small">FE534D4240000100000000000000010000000000000000000000000000000000FFFE000000000000</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000024000500010000006600000078EA16AC</span>

<span style="font-size: small">6877C34A95F7160F73EA377270000000020000000202100200030203110300000100260000000000</span>

<span style="font-size: small">0100200001001A05A92392E1554C072AE7B186EE7DC02CB90BEF2E639CCC94B7A9DC7B3934420000</span>

<span style="font-size: small">0200060000000000020001000200</span>

<span style="font-size: small">Concatenate
Connection.PreauthIntegrityHashValue and Negotiate request packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000FE534D42400001000000000000000100</span>

<span style="font-size: small">00000000000000000000000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000024000500010000006600000078EA16AC6877C34A95F7160F73EA377270000000</span>

<span style="font-size: small">0200000002021002000302031103000001002600000000000100200001001A05A92392E1554C072A</span>

<span style="font-size: small">E7B186EE7DC02CB90BEF2E639CCC94B7A9DC7B39344200000200060000000000020001000200</span>

<span style="font-size: small">New </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A3A8A769FEA693B3D037406EF945E115D2B7A4A9318564D2CAAA4B1FE0EC36D8D92A4802619EDCF2</span>

<span style="font-size: small">9E2410534D2D3749E71F76ADF5212F959210D291097A6355</span>

<span style="font-size: small">NEGOTIATE Response</span>

<span style="font-size: small">Updating Preauth integrity hash
---</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A3A8A769FEA693B3D037406EF945E115D2B7A4A9318564D2CAAA4B1FE0EC36D8D92A4802619EDCF2</span>

<span style="font-size: small">9E2410534D2D3749E71F76ADF5212F959210D291097A6355</span>

<span style="font-size: small">Negotiate response packet
</span>

<span style="font-size: small">FE534D4240000100000000000000010001000000000000000000000000000000FFFE000000000000</span>

<span style="font-size: small">000000000000000000000000000000000000000000000000410001001103020039CBCAF329714942</span>

<span style="font-size: small">BDCE5D60F09AB3FB27000000000080000000800000008000D04C8443CCAED00109094AB095AED001</span>

<span style="font-size: small">80004001C00100006082013C06062B0601050502A08201303082012CA01A3018060A2B0601040182</span>

<span style="font-size: small">3702021E060A2B06010401823702020AA282010C048201084E45474F455854530100000000000000</span>

<span style="font-size: small">60000000700000007F7CC0FD06D6362D02DDE1CF343BFE2973007DCF55CA793E082B7A257DEFE6E8</span>

<span style="font-size: small">E18291ABF112C0599108C772F55CBB2A000000000000000060000000010000000000000000000000</span>

<span style="font-size: small">5C33530DEAF90D4DB2EC4AE3786EC3084E45474F4558545303000000010000004000000098000000</span>

<span style="font-size: small">7F7CC0FD06D6362D02DDE1CF343BFE295C33530DEAF90D4DB2EC4AE3786EC3084000000058000000</span>

<span style="font-size: small">3056A05430523027802530233121301F06035504031318546F6B656E205369676E696E6720507562</span>

<span style="font-size: small">6C6963204B65793027802530233121301F06035504031318546F6B656E205369676E696E67205075</span>

<span style="font-size: small">626C6963204B6579010026000000000001002000010088AFA422ECC239CB16F30BA641AE4B6EE79F</span>

<span style="font-size: small">5A4AF74FE18A301E9790515D07F70000020004000000000001000100</span>

<span style="font-size: small">Concatenate
Connection.PreauthIntegrityHashValue and Negotiate response
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">A3A8A769FEA693B3D037406EF945E115D2B7A4A9318564D2CAAA4B1FE0EC36D8D92A4802619EDCF2</span>

<span style="font-size: small">9E2410534D2D3749E71F76ADF5212F959210D291097A6355FE534D42400001000000000000000100</span>

<span style="font-size: small">01000000000000000000000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">0000000000000000410001001103020039CBCAF329714942BDCE5D60F09AB3FB2700000000008000</span>

<span style="font-size: small">0000800000008000D04C8443CCAED00109094AB095AED00180004001C00100006082013C06062B06</span>

<span style="font-size: small">01050502A08201303082012CA01A3018060A2B06010401823702021E060A2B06010401823702020A</span>

<span style="font-size: small">A282010C048201084E45474F45585453010000000000000060000000700000007F7CC0FD06D6362D</span>

<span style="font-size: small">02DDE1CF343BFE2973007DCF55CA793E082B7A257DEFE6E8E18291ABF112C0599108C772F55CBB2A</span>

<span style="font-size: small">0000000000000000600000000100000000000000000000005C33530DEAF90D4DB2EC4AE3786EC308</span>

<span style="font-size: small">4E45474F45585453030000000100000040000000980000007F7CC0FD06D6362D02DDE1CF343BFE29</span>

<span style="font-size: small">5C33530DEAF90D4DB2EC4AE3786EC30840000000580000003056A05430523027802530233121301F</span>

<span style="font-size: small">06035504031318546F6B656E205369676E696E67205075626C6963204B6579302780253023312130</span>

<span style="font-size: small">1F06035504031318546F6B656E205369676E696E67205075626C6963204B65790100260000000000</span>

<span style="font-size: small">01002000010088AFA422ECC239CB16F30BA641AE4B6EE79F5A4AF74FE18A301E9790515D07F70000</span>

<span style="font-size: small">020004000000000001000100</span>

<span style="font-size: small">New </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A21419AD43D5A4975326E07142734EADA33D0927738F3C1B05A65B003CCAAAE225B547045260356C</span>

<span style="font-size: small">2014A21E0A3DFA9EF7B192C375BFFC5F5E766AC3261F0457</span>

<span style="font-size: small">Add NEW SessionId 0x100000000021 to
Preauth Integrity hash table with value </span>

<span style="font-size: small">Connection.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A21419AD43D5A4975326E07142734EADA33D0927738F3C1B05A65B003CCAAAE225B547045260356C</span>

<span style="font-size: small">2014A21E0A3DFA9EF7B192C375BFFC5F5E766AC3261F0457</span>

<span style="font-size: small">SESSION SETUP Request </span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000021</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">A21419AD43D5A4975326E07142734EADA33D0927738F3C1B05A65B003CCAAAE225B547045260356C</span>

<span style="font-size: small">2014A21E0A3DFA9EF7B192C375BFFC5F5E766AC3261F0457</span>

<span style="font-size: small">SessionSetup request packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800000000000000000000100000000000000FFFE000000000000</span>

<span style="font-size: small">00000000000000000000000000000000000000000000000019000001010000000000000058004A00</span>

<span style="font-size: small">0000000000000000604806062B0601050502A03E303CA00E300C060A2B06010401823702020AA22A</span>

<span style="font-size: small">04284E544C4D5353500001000000978208E200000000000000000000000000000000060380250000</span>

<span style="font-size: small">000F</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup request
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data 
</span>

<span style="font-size: small">A21419AD43D5A4975326E07142734EADA33D0927738F3C1B05A65B003CCAAAE225B547045260356C</span>

<span style="font-size: small">2014A21E0A3DFA9EF7B192C375BFFC5F5E766AC3261F0457FE534D42400001000000000001008000</span>

<span style="font-size: small">00000000000000000100000000000000FFFE00000000000000000000000000000000000000000000</span>

<span style="font-size: small">000000000000000019000001010000000000000058004A000000000000000000604806062B060105</span>

<span style="font-size: small">0502A03E303CA00E300C060A2B06010401823702020AA22A04284E544C4D53535000010000009782</span>

<span style="font-size: small">08E200000000000000000000000000000000060380250000000F</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">FD10D68FFBB5D94DD483DE14DC8AF92B4D2D8517A5D245FE091C93050AC56239B3B829F74CB25451</span>

<span style="font-size: small">276248F12279DCC027C9B53841A67052A617C32C93CBA8C2</span>

<span style="font-size: small">SESSION SETUP Response</span>

<span style="font-size: small"> --- STATUS\_MORE\_PROCESSING\_REQUIRED -
Updating Preauth integrity hash ---</span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000021</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">FD10D68FFBB5D94DD483DE14DC8AF92B4D2D8517A5D245FE091C93050AC56239B3B829F74CB25451</span>

<span style="font-size: small">276248F12279DCC027C9B53841A67052A617C32C93CBA8C2</span>

<span style="font-size: small">SessionSetup response packet
</span>

<span style="font-size: small">FE534D4240000100160000C00100010001000000000000000100000000000000FFFE000000000000</span>

<span style="font-size: small">210000000010000000000000000000000000000000000000090000004800B300A181B03081ADA003</span>

<span style="font-size: small">0A0101A10C060A2B06010401823702020AA281970481944E544C4D53535000020000000C000C0038</span>

<span style="font-size: small">00000015828AE29296836B33F712E0000000000000000050005000440000000A0092270000000F53</span>

<span style="font-size: small">005500540033003100310002000C0053005500540033003100310001000C00530055005400330031</span>

<span style="font-size: small">00310004000C0053005500540033003100310003000C005300550054003300310031000700080019</span>

<span style="font-size: small">C69C43CCAED00100000000</span>

<span style="font-size: small">SessionSetup response header signature
0x00000000000000000000000000000000</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup response
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data
</span>

<span style="font-size: small">FD10D68FFBB5D94DD483DE14DC8AF92B4D2D8517A5D245FE091C93050AC56239B3B829F74CB25451</span>

<span style="font-size: small">276248F12279DCC027C9B53841A67052A617C32C93CBA8C2FE534D4240000100160000C001000100</span>

<span style="font-size: small">01000000000000000100000000000000FFFE00000000000021000000001000000000000000000000</span>

<span style="font-size: small">0000000000000000090000004800B300A181B03081ADA0030A0101A10C060A2B0601040182370202</span>

<span style="font-size: small">0AA281970481944E544C4D53535000020000000C000C003800000015828AE29296836B33F712E000</span>

<span style="font-size: small">0000000000000050005000440000000A0092270000000F53005500540033003100310002000C0053</span>

<span style="font-size: small">005500540033003100310001000C0053005500540033003100310004000C00530055005400330031</span>

<span style="font-size: small">00310003000C005300550054003300310031000700080019C69C43CCAED00100000000</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">2AA0A0D736D4A3BE4A2FA06B20EEBF02635543C0310F72595ACEAF9893BBE647D9C753175215BB24</span>

<span style="font-size: small">71DF365D4FC77AB8D168ECC91ABC02C4611D2AAC33181967</span>

<span style="font-size: small">SESSION SETUP Request </span>

<span style="font-size: small">PreauthSession.SessionId
0x100000000021</span>

<span style="font-size: small">Current </span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">2AA0A0D736D4A3BE4A2FA06B20EEBF02635543C0310F72595ACEAF9893BBE647D9C753175215BB24</span>

<span style="font-size: small">71DF365D4FC77AB8D168ECC91ABC02C4611D2AAC33181967</span>

<span style="font-size: small">SessionSetup request packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800000000000000000000200000000000000FFFE000000000000</span>

<span style="font-size: small">2100000000100000000000000000000000000000000000001900000101000000000000005800CF01</span>

<span style="font-size: small">0000000000000000A18201CB308201C7A0030A0101A28201AA048201A64E544C4D53535000030000</span>

<span style="font-size: small">001800180090000000EE00EE00A80000000C000C00580000001A001A0064000000120012007E0000</span>

<span style="font-size: small">001000100096010000158288E2060380250000000F3E492B87B2606D263031D0D12B6AD267530055</span>

<span style="font-size: small">005400330031003100610064006D0069006E006900730074007200610074006F0072004400520049</span>

<span style="font-size: small">005600450052003300310031000000000000000000000000000000000000000000000000009AEF57</span>

<span style="font-size: small">4DBD2E8A323B017ED361EEA14B010100000000000019C69C43CCAED00176AC9CBD38378531000000</span>

<span style="font-size: small">0002000C0053005500540033003100310001000C0053005500540033003100310004000C00530055</span>

<span style="font-size: small">00540033003100310003000C005300550054003300310031000700080019C69C43CCAED001060004</span>

<span style="font-size: small">00020000000800300030000000000000000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B</span>

<span style="font-size: small">8E0EBA5A6DFD9D07A31D11B548F8C9D0CC0A00100000000000000000000000000000000000090016</span>

<span style="font-size: small">0063006900660073002F005300550054003300310031000000000000000000000000005E621187A7</span>

<span style="font-size: small">5CC18E3982494ECC4793B7A3120410010000005C661B9E6BE0F1E500000000</span>

<span style="font-size: small">Concatenate
PreauthSession.PreauthIntegrityHashValue and Session Setup request
packet</span>

<span style="font-size: small">SHA-512 Input Hash Data 
</span>

<span style="font-size: small">2AA0A0D736D4A3BE4A2FA06B20EEBF02635543C0310F72595ACEAF9893BBE647D9C753175215BB24</span>

<span style="font-size: small">71DF365D4FC77AB8D168ECC91ABC02C4611D2AAC33181967FE534D42400001000000000001008000</span>

<span style="font-size: small">00000000000000000200000000000000FFFE00000000000021000000001000000000000000000000</span>

<span style="font-size: small">00000000000000001900000101000000000000005800CF010000000000000000A18201CB308201C7</span>

<span style="font-size: small">A0030A0101A28201AA048201A64E544C4D53535000030000001800180090000000EE00EE00A80000</span>

<span style="font-size: small">000C000C00580000001A001A0064000000120012007E0000001000100096010000158288E2060380</span>

<span style="font-size: small">250000000F3E492B87B2606D263031D0D12B6AD267530055005400330031003100610064006D0069</span>

<span style="font-size: small">006E006900730074007200610074006F007200440052004900560045005200330031003100000000</span>

<span style="font-size: small">0000000000000000000000000000000000000000009AEF574DBD2E8A323B017ED361EEA14B010100</span>

<span style="font-size: small">000000000019C69C43CCAED00176AC9CBD383785310000000002000C005300550054003300310031</span>

<span style="font-size: small">0001000C0053005500540033003100310004000C0053005500540033003100310003000C00530055</span>

<span style="font-size: small">0054003300310031000700080019C69C43CCAED00106000400020000000800300030000000000000</span>

<span style="font-size: small">000000000000300000B61FEFCAA857EA57BF1EDCEBF8974B8E0EBA5A6DFD9D07A31D11B548F8C9D0</span>

<span style="font-size: small">CC0A001000000000000000000000000000000000000900160063006900660073002F005300550054</span>

<span style="font-size: small">003300310031000000000000000000000000005E621187A75CC18E3982494ECC4793B7A312041001</span>

<span style="font-size: small">0000005C661B9E6BE0F1E500000000</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">DECF98A420718718F22090D3580FCC5E484BD310FA1268210C6E86335A8891E767F5BCD99FA5A785</span>

<span style="font-size: small">9D665AD07A73EA94E1BCDB7CFA69A6962A28A244138340B1</span>

<span style="font-size: small">SESSION SETUP Response </span>

<span style="font-size: small">SessionId 0x100000000021 COMPLETED</span>

<span style="font-size: small">SessionSetup response packet
</span>

<span style="font-size: small">FE534D4240000100000000000100800009000000000000000200000000000000FFFE000000000000</span>

<span style="font-size: small">21000000001000003676196AEE8CA17E5D50A53642EF2BE40900000048001D00A11B3019A0030A01</span>

<span style="font-size: small">00A3120410010000000F57444342A2717E00000000</span>

<span style="font-size: small">SessionSetup response header signature
0x3676196AEE8CA17E5D50A53642EF2BE4</span>

<span style="font-size: small">PreauthSession.PreauthIntegrityHashValue
</span>

<span style="font-size: small">DECF98A420718718F22090D3580FCC5E484BD310FA1268210C6E86335A8891E767F5BCD99FA5A785</span>

<span style="font-size: small">9D665AD07A73EA94E1BCDB7CFA69A6962A28A244138340B1</span>

<span style="font-size: small">Input cryptographicKey (SessionKey)
0x07B7F69C1E2581662DF6987E88F9E891</span>

<span style="font-size: small">(queried from GSS authenticated
context)</span>

<span style="font-size: small">--- Dialect 0x0311 ---</span>

<span style="font-size: small">preauthIntegrityHashValue
</span>

<span style="font-size: small">DECF98A420718718F22090D3580FCC5E484BD310FA1268210C6E86335A8891E767F5BCD99FA5A785</span>

<span style="font-size: small">9D665AD07A73EA94E1BCDB7CFA69A6962A28A244138340B1</span>

<span style="font-size: small">CypherId 0x0001</span>

<span style="font-size: small">SessionKey
0x07B7F69C1E2581662DF6987E88F9E891</span>

<span style="font-size: small">SigningKey
0x3DCC82C5795AE27F383242761078C59B</span>

<span style="font-size: small">EncryptionKey
0xDFAAA31AAE40A2485D47AC4DF09FDA1D</span>

<span style="font-size: small">DecryptionKey
0x95C544AEF6072680DA1CE49A68A97FA6</span>

<span style="font-size: small">ApplicationKey
0x7A2F0F73EC2D530879B2913BBFCE242F</span>

<span style="font-size: small">--- Encryption ---</span>

<span style="font-size: small">SessionId 0x100000000021</span>

<span style="font-size: small">SessionKey
0x07B7F69C1E2581662DF6987E88F9E891</span>

<span style="font-size: small">SigningKey
0x3DCC82C5795AE27F383242761078C59B</span>

<span style="font-size: small">EncryptionKey
0xDFAAA31AAE40A2485D47AC4DF09FDA1D</span>

<span style="font-size: small">DecryptionKey
0x95C544AEF6072680DA1CE49A68A97FA6</span>

<span style="font-size: small">ApplicationKey
0x7A2F0F73EC2D530879B2913BBFCE242F</span>

<span style="font-size: small">Header.Command 0x0009 WRITE</span>

<span style="font-size: small">Encryption of the request ---</span>

<span style="font-size: small">Key
0xDFAAA31AAE40A2485D47AC4DF09FDA1D</span>

<span style="font-size: small">Nonce Length 0xb</span>

<span style="font-size: small">AES-128-CCM nonce
0x9F6F1EAAD7E9F24AACD38F</span>

<span style="font-size: small">SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000900010008000000000000000500000000000000FFFE000001000000</span>

<span style="font-size: small">21000000001000000000000000000000000000000000000031007000170000000000000000000000</span>

<span style="font-size: small">0500000004000000010000000400000000000000000000007000000000000000536D623320656E63</span>

<span style="font-size: small">72797074696F6E2074657374696E67</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0xE89551D666DAB8993488F5A97103116C</span>

<span style="font-size: small">transform\_header.Nonce
0x9F6F1EAAD7E9F24AACD38F0000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x87</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000021</span>

<span style="font-size: small">Encrypted message
</span>

<span style="font-size: small">56A74778199A9D2B6E9C3A376FD88D27680694FED253A313BEB07381AE8689F973ACDB8D716E4477</span>

<span style="font-size: small">803BCE53A92E1B81FA3E965AD9AF2C89C08CE66A344664453B8FC88118EDC9814CF58E92AA465E6E</span>

<span style="font-size: small">FB09958A9FDAD96FBD55B36A710C30D5E7C64AD7B9449F9F17EDD024FE8BA79154F340A82740D1D5</span>

<span style="font-size: small">180C69B0A2DE6A4BA893BD55D3210E</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D42E89551D666DAB8993488F5A97103116C9F6F1EAAD7E9F24AACD38F000000000087000000</span>

<span style="font-size: small">00000100210000000010000056A74778199A9D2B6E9C3A376FD88D27680694FED253A313BEB0<span style="font-size: small">7381</span></span>

<span style="font-size: small">AE8689F973ACDB8D716E4477803BCE53A92E1B81FA3E965AD9AF2C89C08CE66A344664453B8F<span style="font-size: small">C881</span></span>

<span style="font-size: small">18EDC9814CF58E92AA465E6EFB09958A9FDAD96FBD55B36A710C30D5E7C64AD7B9449F9F17ED<span style="font-size: small">D024</span></span>

<span style="font-size: small">FE8BA79154F340A82740D1D5180C69B0A2DE6A4BA893BD55D3210E</span>

<span style="font-size: small">Decryption of the response ---</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D42DD33EC41A927DD51476FE887C2D3C136D96831DD2E8EB7B4000000000000000050000000</span>

<span style="font-size: small">000001002100000000100000F783157E0F6F1C055D746753CA16D20C21088E2A67564E056C2F68A7</span>

<span style="font-size: small">F14F226C3BD809B7A2D52E5FE4ECF49821BC6001733430CF174E2764B3CCB213AAD8BB9FBAF6C15E</span>

<span style="font-size: small">13D9120965390E004A96A3F7</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0xDD33EC41A927DD51476FE887C2D3C136</span>

<span style="font-size: small">transform\_header.Nonce
0xD96831DD2E8EB7B40000000000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x50</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000021</span>

<span style="font-size: small">Key
0x95C544AEF6072680DA1CE49A68A97FA6</span>

<span style="font-size: small">Nonce Length 0xb</span>

<span style="font-size: small">AES-128-CCM nonce
0xD96831DD2E8EB7B4000000</span>

<span style="font-size: small">Decrypted SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000900010001000000000000000500000000000000FFFE000001000000</span>

<span style="font-size: small">21000000001000000000000000000000000000000000000011000000170000000000000000000000</span>

<span style="font-size: small">Header.Command 0x0008 READ</span>

<span style="font-size: small">Encryption of the request ---</span>

<span style="font-size: small">Key
0xDFAAA31AAE40A2485D47AC4DF09FDA1D</span>

<span style="font-size: small">Nonce Length 0xb</span>

<span style="font-size: small">AES-128-CCM nonce
0xA0F92E964EDC3049B86E19</span>

<span style="font-size: small">SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000800010008000000000000000600000000000000FFFE000001000000</span>

<span style="font-size: small">21000000001000000000000000000000000000000000000031000000170000000000000000000000</span>

<span style="font-size: small">050000000400000001000000040000000000000000000000000000000000000000</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0x35BF9600C841F0CDA9BD1BC3727B7E36</span>

<span style="font-size: small">transform\_header.Nonce
0xA0F92E964EDC3049B86E190000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x71</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000021</span>

<span style="font-size: small">Encrypted message
</span>

<span style="font-size: small">C4CCD3EB483A0638E69C99E391E7F64BCC10D6BEE46FEEA258C4BCAF792CB5A6E69283924081806D</span>

<span style="font-size: small">AB64827E9D14A5345D5221AB6DAFCB0E89FC2606B63D92163F4F6C93D1213D86ABF123B93EAD3AEF</span>

<span style="font-size: small">9A3471EFD68A423A00A6E0064D9AE3C842EFFFAD236A3BF25D37F4CD054C97DE18</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D4235BF9600C841F0CDA9BD1BC3727B7E36A0F92E964EDC3049B86E19000000000071000000</span>

<span style="font-size: small">000001002100000000100000C4CCD3EB483A0638E69C99E391E7F64BCC10D6BEE46FEEA258C4<span style="font-size: small">BCAF</span></span>

<span style="font-size: small">792CB5A6E69283924081806DAB64827E9D14A5345D5221AB6DAFCB0E89FC2606B63D92163F4F<span style="font-size: small">6C93</span></span>

<span style="font-size: small">D1213D86ABF123B93EAD3AEF9A3471EFD68A423A00A6E0064D9AE3C842EFFFAD236A3BF25D37<span style="font-size: small">F4CD</span></span>

<span style="font-size: small">054C97DE18</span>

<span style="font-size: small">Decryption of the response ---</span>

<span style="font-size: small">Transformed message
</span>

<span style="font-size: small">FD534D42E241A13C7E1EE42ECF1FD69F3B8668C6DA6831DD2E8EB7B4000000000000000067000000</span>

<span style="font-size: small">00000100210000000010000015D67234FC8358D7BA1BF037ABC8EFD41A0A8F9BB04B16DEB1E85606</span>

<span style="font-size: small">BD8C2770823FE6239A286CB3E3D5762ABBD53FD8DE11ED491FE905E146A8FFCE09414AB741103D63</span>

<span style="font-size: small">7E28B19C6BA759B399DCC21FAE24CF2A455A13B215FC2857ABB513927F9F271D1C208B</span>

<span style="font-size: small">transform\_header.ProtocolId
0x424d53fd</span>

<span style="font-size: small">transform\_header.Signature
0xE241A13C7E1EE42ECF1FD69F3B8668C6</span>

<span style="font-size: small">transform\_header.Nonce
0xDA6831DD2E8EB7B40000000000000000</span>

<span style="font-size: small">transform\_header.OriginalMessageSize
0x67</span>

<span style="font-size: small">transform\_header.Reserved 0x0</span>

<span style="font-size: small">transform\_header.Flags 0x0001</span>

<span style="font-size: small">transform\_header.SessionId
0x100000000021</span>

<span style="font-size: small">Key
0x95C544AEF6072680DA1CE49A68A97FA6</span>

<span style="font-size: small">Nonce Length 0xb</span>

<span style="font-size: small">AES-128-CCM nonce
0xDA6831DD2E8EB7B4000000</span>

<span style="font-size: small">Decrypted SMB2 packet
</span>

<span style="font-size: small">FE534D4240000100000000000800010001000000000000000600000000000000FFFE000001000000</span>

<span style="font-size: small">21000000001000000000000000000000000000000000000011005000170000000000000000000000</span>

<span style="font-size: small">536D623320656E6372797074696F6E2074657374696E67</span>

## Appendix B. How to disable SMB1 on Windows

<span style="font-size: small">See \[KB2696547\]
<https://support.microsoft.com/en-us/kb/2696547></span>

<span style="font-size: small">In Windows 8.1 / Server 2012 R2:</span>

<span style="font-size: small">•To remove SMB1, use the following
PowerShell cmdlet: </span>

<span style="font-size: small">Remove-WindowsFeature FS-SMB1</span>

<span style="font-size: small">•To add SMB1 feature: </span>

<span style="font-size: small">Add-WindowsFeature FS-SMB1</span>

<span style="font-size: small">Windows Client</span>

<span style="font-size: small">•To disable SMBv1 on the SMB client, run
the following commands:</span>

<span style="font-size: small">sc.exe config lanmanworkstation depend=
bowser/mrxsmb20/nsi</span>

<span style="font-size: small">sc.exe config mrxsmb10 start=
disabled</span>

<span style="font-size: small">•To enable SMBv1 on the SMB client, run
the following commands: </span>

<span style="font-size: small">sc.exe config lanmanworkstation depend=
bowser/mrxsmb10/mrxsmb20/nsi </span>

<span style="font-size: small">sc.exe config mrxsmb10 start= auto</span>

<span style="font-size: small">Windows Server</span>

<span style="font-size: small">In Windows Server 2012 R2:</span>

<span style="font-size: small">Get-SmbServerConfiguration | Select
EnableSMB1Protocol, EnableSMB2Protocol</span>

<span style="font-size: small">•To disable SMBv1 on the SMB server, run
the following cmdlet: </span>

<span style="font-size: small">Set-SmbServerConfiguration
-EnableSMB1Protocol $false</span>

<span style="font-size: small">•To enable SMBv1 on the SMB server, run
the following cmdlet:</span>

<span style="font-size: small">Set-SmbServerConfiguration
-EnableSMB1Protocol $true</span>

<span style="font-size: small">In older server versions:</span>

<span style="font-size: small">•To disable SMBv1 on the SMB server, run
the following cmdlet:</span>

<span style="font-size: small">Set-ItemProperty -Path
"HKLM:\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters"
SMB1 -Type DWORD -Value 0 –Force</span>

<span style="font-size: small">•To enable SMBv1 on the SMB server, run
the following cmdlet:</span>

<span style="font-size: small">Set-ItemProperty -Path
"HKLM:\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters"
SMB1 -Type DWORD -Value 1 –Force</span>

## \[References\]

<span style="font-size: small">Encryption in SMB 3.0: A protocol
perspective</span>

<span style="font-size: small"><http://blogs.msdn.com/b/openspecification/archive/2012/10/05/encryption-in-smb-3-0-a-protocol-perspective.aspx></span>

<span style="font-size: small">What’s new in SMB 3.1.1 in the Windows
Server 2016 Technical Preview
2</span>

<span style="font-size: small"><http://blogs.technet.com/b/josebda/archive/2015/05/05/what-s-new-in-smb-3-1-1-in-the-windows-server-technical-preview-2.aspx></span>

<span style="font-size: small">\[MS-SMB2\]: Server Message Block (SMB)
Protocol Versions 2 and 3
Specification</span>

<span style="font-size: small"><https://msdn.microsoft.com/en-us/library/cc246482.aspx> </span>

<span style="font-size: small">\[SP800-108\] National Institute of
Standards and Technology. "Special Publication 800-108, Recommendation
for Key Derivation Using Pseudorandom Functions", October 2009,
<http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf></span>

<span style="font-size: small">\[RFC5084\] Housley, R., "Using AES-CCM
and AES-GCM Authenticated Encryption in the Cryptographic Message Syntax
(CMS)", RFC 5084, November 2007,
<http://www.ietf.org/rfc/rfc5084.txt></span>

<span style="font-size: small">SMB3 Secure Dialect
Negotiation</span>

<span style="font-size: small"><http://blogs.msdn.com/b/openspecification/archive/2012/06/28/smb3-secure-dialect-negotiation.aspx></span>

<span style="font-size: small">How to enable and disable SMBv1, SMBv2,
and SMBv3 in Windows Vista, Windows Server 2008, Windows 7, Windows
Server 2008 R2, Windows 8, and Windows Server
2012</span>

<span style="font-size: small"><https://support.microsoft.com/en-us/kb/2696547></span>

<span style="font-size: small">SMB3 PowerShell changes in Windows Server
2012 R2: SMB1 can now be completely
removed</span>

<span style="font-size: small"><http://blogs.technet.com/b/josebda/archive/2014/08/11/smb3-powershell-changes-in-windows-server-2012-r2-smb1-can-now-be-completely-removed.aspx></span>

 

</div>

</div>
