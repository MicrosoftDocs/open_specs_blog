<div id="page">

# SMB3 Secure Dialect Negotiation

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
6/28/2012 12:15:00 PM

-----

<div id="content">

<span style="font-size: small"><span style="font-family: Calibri">This
blog talks about secure dialect negotiation, one of the new SMB3
security enhancements in Windows Server 2012. </span></span>

<span style="font-size: small"><span style="font-family: Calibri">Secure
dialect negotiation is introduced in SMB3 to protect against
man-in-the-middle attempt to downgrade dialect negotiation. The idea is
to prevent an eavesdropper from downgrading the initially negotiated
dialect and capabilities between the client and the server.
</span></span>

<span style="font-family: Calibri;font-size: small"> </span>

## <span style="font-size: medium"><span style="color: #4f81bd"><span style="font-family: Cambria">Summary</span></span></span>

<span style="font-size: small"><span style="font-family: Calibri">In a
nutshell, upon reception of a Tree Connect response, an SMB3-capable
client validates the original SMB2 Negotiate request by sending a signed
IOCTL, called FSCTL\_VALIDATE\_NEGOTIATE\_INFO as specified in
\[MS-SMB2\]. The server needs to reply with a signed response, and this
enables end-to-end validation of the Negotiate exchange.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">The
client sends the IOCTL only if it is implemented to request validation
of an SMB2 Negotiate via the RequireSecureNegotiate abstract data
element. Windows 8 clients configure this through the registry key
RequireSecureNegotiate.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">To
change this setting, set the following LanmanWorkstation parameter using
PowerShell cmdlet:</span></span>

Set-SmbClientConfiguration -RequireSecureNegotiate \<0|1\>

0 – Disabled

1 – Required

To check the current configuration value, use the PowerShell cmdlet:

Get-SmbClientConfiguration | fl RequireSecureNegotiate

You can also edit the DWORD value through the registry editor.
HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\RequireSecureNegotiate.

To change the default behavior, you need to define the registry key or
use the PowerShell command. If not present, its default value is
“Required” in Windows 8 clients.

Note: In Windows 8/8.1 implementation, RequireSecureNegotiate = 1 means
“Required” enabled. Any other value other than 1 (0, 2,3, …) means
“Disabled”.  Note that MS-SMB2 correctly defines
RequireSecureNegotiate as a
Boolean.

<span style="font-family: Calibri;font-size: small"> </span>

## <span style="font-size: medium"><span style="color: #4f81bd"><span style="font-family: Cambria">Secure Negotiate details</span></span></span>

<span style="font-size: small"><span style="font-family: Calibri">The
FSCTL\_VALIDATE\_NEGOTIATE\_INFO operation allows the client and server
to repeat the Negotiate process over a signed exchange. This exchange
could also be sealed if encryption is enabled, i.e. Session.EncryptData
or TreeConnect.EncryptData is TRUE.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">The
signed request allows the server to validate the client capabilities,
Guid, SecurityMode, and Dialects which could be 0x0300, 0x0210, and
0x0202 (this list includes 0x0224 if you are testing Windows 8
Beta).</span></span>

<span style="font-size: small"><span style="font-family: Calibri">The
signed response (success or error) enables the client to verify the
outcome.  On a signed success response, the client verifies that none of
these attributes of the Negotiate response have been downgraded or
manipulated: server capabilities, server Guid, server SecurityMode, and
the connection Dialect.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">If the
client receives an error response, the transport connection is
terminated if any of the following conditions is met:</span></span>

<span style="font-family: Calibri;font-size: small">-</span>       
<span style="font-size: small"><span style="font-family: Calibri">The
response is not signed. This is applied regardless of whether signing is
required or not on the connection or session.</span></span>

<span style="font-family: Calibri;font-size: small">-</span>       
<span style="font-size: small"><span style="font-family: Calibri">The
status code is STATUS\_ACCESS\_DENIED, meaning the server could not
validate the request.</span></span>

<span style="font-family: Calibri;font-size: small">-</span>       
<span style="font-size: small"><span style="font-family: Calibri">The
response values (capabilities, Guid, SecurityMode, and Dialect) do not
match the server values that the client originally stored on the
connection object. </span></span>

<span style="font-size: small"><span style="font-family: Calibri">Note
that the server would have also terminated the transport connection if
it could not validate the client capabilities, Guid, SecurityMode, and
Dialects.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">Because
the request is signed, the response must be signed as well, otherwise
the client would terminate the connection. In case of 3.0 dialect, the
sender computes the signature using AES\_CMAC-128. For 2.002 or 2.1
dialect, the signature is derived from an HMAC-SHA256
hash.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">Down-level
servers (pre-Windows 2012) will return STATUS\_NOT\_SUPPORTED or
STATUS\_INVALID\_DEVICE\_REQUEST since they do not allow or implement
FSCTL\_VALIDATE\_NEGOTIATE\_INFO. The client should accept the response
provided it’s properly signed. For SMB3-capable server, it is
recommended that the server implements FSCTL\_VALIDATE\_NEGOTIATE\_INFO.
On the other-hand, when a client establishes an SMB 3.x connection, it
MUST go through the FSCTL\_VALIDATE\_NEGOTIATE\_INFO phase, provided
RequireSecureNegotiate allows it.</span></span>

<span style="font-size: small"><span style="font-family: Calibri">The
protocol documents that a non-SMB3-capable (2.002 or 2.1) should respond
to VALIDATE\_NEGOTIATE\_INFO request with a status error of
STATUS\_NOT\_SUPPORTED or STATUS\_INVALID\_DEVICE\_REQUEST, the same
error as for any unsupported/non-allowed FSCTL. Windows Server 2008 (SMB
2.002) and Windows Server 2008 R2 (SMB 2.1) return STATUS\_FILE\_CLOSED,
instead.</span></span>

<span style="font-family: Calibri;font-size: small"> </span>

## <span style="font-size: medium"><span style="color: #4f81bd"><span style="font-family: Cambria">Anonymous or Guest sessions pass the mesh</span></span></span>

<span style="font-size: small"><span style="font-family: Calibri">As
previously noted, the “secure Negotiate” relies on the correct signing
of responses by all SMB 2/3 servers. This means the exchange can only
occur under a user security context that is neither anonymous nor guest.
Therefore, anonymous or guest sessions cannot use the secure
negotiation. Keep in mind that if you enable null or guest sessions to
access your shares, you leave the door open for this potential
attack.</span></span>

<span style="font-family: Calibri;font-size: small"> </span>

## <span style="font-size: medium"><span style="color: #4f81bd"><span style="font-family: Cambria">Note on downgrading to SMB1  </span></span></span>

<span style="font-family: Calibri;font-size: small">With “secure
Negotiate”, it is not required that signing is active on the connection.
It is inherently designed to work with servers that support unsolicited
signed requests. Since Windows-based SMB1 servers do not support
unsolicited signed requests, downgrading attacks from SMB 2/3 to SMB 1
cannot be detected in Windows 8. It is recommended that SMB1 is disabled
if all clients in your environments are SMB2 capable (See
</span>[<span style="font-family: Calibri;color: #0000ff;font-size: small">blog
post</span>](http://blogs.technet.com/b/filecab/archive/2012/05/03/smb-3-security-enhancements-in-windows-server-2012.aspx)<span style="font-size: small"><span style="font-family: Calibri">).
</span></span>

<span style="font-size: small"><span style="font-family: Calibri"> </span></span>

<span style="font-size: small"><span style="font-family: Calibri">**NOTE:**
At the time of writing this blog, Windows 8 and Windows Server 2012 are
Release Preview (Release Candidate) in their release
cycle.</span></span>

<span style="font-family: Calibri;font-size: small"> </span>

## <span style="font-size: medium"><span style="color: #4f81bd"><span style="font-family: Cambria">References</span></span></span>

<span style="font-size: small"><span style="font-family: Calibri">Windows
8 Beta: SMB connections fail with error "Invalid
Signature"</span></span>

[<span style="font-family: Calibri;color: #0000ff;font-size: small">http://support.microsoft.com/kb/2686098</span>](http://support.microsoft.com/kb/2686098)

<span style="font-size: small"><span style="font-family: Calibri">Overview
of Server Message Block
signing</span></span>

[<span style="font-family: Calibri;color: #0000ff;font-size: small">http://support.microsoft.com/kb/887429</span>](http://support.microsoft.com/kb/887429)

<span style="font-size: small"><span style="font-family: Calibri">SMB 3
Security Enhancements in Windows Server
2012</span></span>

[<span style="font-family: Calibri;color: #0000ff;font-size: small">http://blogs.technet.com/b/filecab/archive/2012/05/03/smb-3-security-enhancements-in-windows-server-2012.aspx</span>](http://blogs.technet.com/b/filecab/archive/2012/05/03/smb-3-security-enhancements-in-windows-server-2012.aspx)

</div>

</div>
