<div id="page">

# NTLM V1… no, excuse me… NTLM V2… oh, no, you were right… it’s V1…

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
5/3/2010 8:55:00 PM

-----

<div id="content">

… and the discussion goes like that for a couple hours.

Have you been in that situation before?

If the answer is no… then you probably have something better to do than
reading this blog. May I suggest [Dilbert](http://www.dilbert.com/)? I’m
a longtime fan.

If the answer is yes, then you will probably like this short tip.

It is easy to understand that
[NTLM](http://msdn.microsoft.com/en-us/library/cc236621\(PROT.13\).aspx)
is the authentication method being used between two computers when
capturing data over the wire but, how can we distinguish if the version
being used is
[V1](http://msdn.microsoft.com/en-us/library/cc236699\(v=PROT.13\).aspx)
or
[V2](http://msdn.microsoft.com/en-us/library/cc236700\(v=PROT.13\).aspx)?

****

 

**Well, the only way to tell is by looking into the following details:**

<span style="COLOR: #1f497d">3489       1:50:07 AM 3/19/2010   
143.9069739                       ENDPOINT01      SUT01   SMB      
SMB:C; Negotiate, Dialect = NT LM 0.12        {SMBOverTCP:148, TCP:147,
IPv4:3}</span>

<span style="COLOR: #1f497d">3490       1:50:07 AM 3/19/2010   
143.9077536                       SUT01   ENDPOINT01      SMB      
SMB:R; Negotiate, Dialect is NT LM 0.12 (\#0)             
{SMBOverTCP:148, TCP:147, IPv4:3}</span>

<span style="COLOR: #1f497d">3491       1:50:07 AM 3/19/2010   
143.9168036                       ENDPOINT01      SUT01   SMB      
SMB:C; Session Setup Andx, NTLM NEGOTIATE MESSAGE   {SMBOverTCP:148,
TCP:147, IPv4:3}</span>

<span style="COLOR: #1f497d">3492       1:50:07 AM 3/19/2010   
143.9174079                       SUT01   ENDPOINT01      SMB      
SMB:R; Session Setup Andx, NTLM CHALLENGE MESSAGE - NT Status: System -
Error, Code = (22) STATUS\_MORE\_PROCESSING\_REQUIRED               
{SMBOverTCP:148, TCP:147,
IPv4:3}</span>

<span style="BACKGROUND: yellow; COLOR: #1f497d; mso-highlight: yellow">3493      
1:50:07 AM 3/19/2010    143.9396336                       ENDPOINT01 
    SUT01   SMB       SMB:C; Session Setup Andx, NTLM AUTHENTICATE
MESSAGE, Domain:  , User: Administrator, Workstation: ENDPOINT01   
{SMBOverTCP:148, TCP:147,
IPv4:3}</span><span style="COLOR: #1f497d"></span>

<span style="COLOR: #1f497d">3495       1:50:07 AM 3/19/2010   
143.9414495                       SUT01   ENDPOINT01      SMB      
SMB:R; Session Setup Andx                {SMBOverTCP:148, TCP:147,
IPv4:3}</span>

 

 

 

**Looking into the highlighted message:**

<span style="COLOR: #1f497d"></span>

 

<span style="COLOR: #1f497d">  Frame: Number = 3493, Captured Frame
Length = 282, MediaType = ETHERNET</span>

<span style="COLOR: #1f497d">+ Ethernet: Etype = Internet IP
(IPv4),DestinationAddress:\[00-15-5D-2E-39-5A\],SourceAddress:\[00-15-5D-2E-39-5B\]</span>

<span style="COLOR: #1f497d">+ Ipv4: Src = 192.168.0.101, Dest =
192.168.0.1, Next Protocol = TCP, Packet ID = 10822, Total IP Length =
268</span>

<span style="COLOR: #1f497d">+ Tcp: Flags=...AP..., SrcPort=1085,
DstPort=Microsoft-DS(445), PayloadLen=228, Seq=2086951204 - 2086951432,
Ack=2573578059, Win=16308 (scale factor 0x2) = 65232</span>

<span style="COLOR: #1f497d">+ SMBOverTCP: Length = 224</span>

<span style="COLOR: #1f497d">- Smb: C; Session Setup Andx, NTLM
AUTHENTICATE MESSAGE, Domain:  , User: Administrator, Workstation:
ENDPOINT01</span>

<span style="COLOR: #1f497d">    Protocol: SMB</span>

<span style="COLOR: #1f497d">    Command: Session Setup Andx
115(0x73)</span>

<span style="COLOR: #1f497d">  + NTStatus: 0x0, Facility =
FACILITY\_SYSTEM, Severity = STATUS\_SEVERITY\_SUCCESS, Code = (0)
STATUS\_SUCCESS</span>

<span style="COLOR: #1f497d">  + SMBHeader: Command, TID: 0x0000, PID:
0x0000, UID: 0x1000, MID: 0x0001</span>

<span style="COLOR: #1f497d">  - CSessionSetupAndXNTLMESS: </span>

<span style="COLOR: #1f497d">     WordCount: 12 (0xC)</span>

<span style="COLOR: #1f497d">     ANDXCommand: No Secondary Command
255(0xFF)</span>

<span style="COLOR: #1f497d">     AndXReserved: 0 (0x0)</span>

<span style="COLOR: #1f497d">     ANDXOffset: 224 (0xE0)</span>

<span style="COLOR: #1f497d">     MaxBufferSize: 16644 (0x4104)</span>

<span style="COLOR: #1f497d">     MaxMpxCount: 0 (0x0)</span>

<span style="COLOR: #1f497d">     VcNumber: 0 (0x0)</span>

<span style="COLOR: #1f497d">     SessionKey: 0 (0x0)</span>

<span style="COLOR: #1f497d">     SecurityBlobLength: 160 (0xA0)</span>

<span style="COLOR: #1f497d">     Reserved: 0 (0x0)</span>

<span style="COLOR: #1f497d">   + Capabilities: 0x8001E3FC</span>

<span style="COLOR: #1f497d">     ByteCount: 165 (0xA5)</span>

<span style="COLOR: #1f497d">   - SecurityBlob: </span>

<span style="COLOR: #1f497d">    - GSSAPI: </span>

<span style="COLOR: #1f497d">     - Token: NTLM AUTHENTICATE MESSAGE,
Domain:  , User: Administrator, Workstation: ENDPOINT01</span>

<span style="COLOR: #1f497d">      - NLMP: NTLM AUTHENTICATE MESSAGE,
Domain:  , User: Administrator, Workstation: ENDPOINT01</span>

<span style="COLOR: #1f497d">         Signature: NTLMSSP</span>

<span style="COLOR: #1f497d">         MessageType: Authenticate Message
(0x00000003)</span>

<span style="COLOR: #1f497d">       - LmChallengeResponse: Length: 24,
Offset: 112</span>

<span style="COLOR: #1f497d">          Length: 24 (0x18)</span>

<span style="COLOR: #1f497d">          MaximumLength: 24 (0x18)</span>

<span style="COLOR: #1f497d">          BufferOffset: 112 (0x70)</span>

<span style="COLOR: #1f497d">      
<span style="BACKGROUND: yellow; mso-highlight: yellow">-
NtChallengeResponse: Length: 24, Offset:
136</span></span>

<span style="BACKGROUND: yellow; COLOR: #1f497d; mso-highlight: yellow">         
Length: 24 (0x18)</span><span style="COLOR: #1f497d"></span>

<span style="COLOR: #1f497d">          MaximumLength: 24 (0x18)</span>

<span style="COLOR: #1f497d">          BufferOffset: 136 (0x88)</span>

<span style="COLOR: #1f497d">       + DomainName: Length: 1, Offset:
88</span>

<span style="COLOR: #1f497d">       + UserName: Length: 13, Offset:
89</span>

<span style="COLOR: #1f497d">       + Workstation: Length: 10, Offset:
102</span>

<span style="COLOR: #1f497d">       + SessionKey: Length: 0, Offset:
160</span>

<span style="COLOR: #1f497d">       + AuthenticateFlags: 0x0280A206
(NTLM v1No encryption, Always Sign)</span>

<span style="COLOR: #1f497d">       + Version: Windows 6.0 Build 6002
NLMPv15</span>

<span style="COLOR: #1f497d">       + MessageIntegrityCheckNotPresent:
7B17C94546AB0475161B66A23214803D</span>

<span style="COLOR: #1f497d">         DomainNameStringA:  </span>

<span style="COLOR: #1f497d">         UserNameStringA:
Administrator</span>

<span style="COLOR: #1f497d">         WorkstationStringA:
ENDPOINT01</span>

<span style="COLOR: #1f497d">       + LmChallengeResponseString:
3C3EBA89185188ED468BFF010611B4852B6B2BF5A01DA154</span>

<span style="COLOR: #1f497d">       + NTLMV1ChallengeResponse:
84BF3BFBEBA1D5F4CF7171EF716EEF8FF7167E47A0EB4128</span>

<span style="COLOR: #1f497d">   + Align: 1 Bytes</span>

<span style="COLOR: #1f497d">     NativeOS: </span>

<span style="COLOR: #1f497d">     Null: 0 (0x0)</span>

 

The **highlighted field** is the only one that will clear our doubt.

If its value is **24 bytes** long, then the version being used is
**V1**.

If its value is **larger than 24 (variable size)** then the version
being used is **V2**.

Well, I told you it was a short tip… now you can click on the
[Dilbert](http://www.dilbert.com/) link and laugh at a couple strips.

*Disclaimer: it may be addictive so, set a limit up front and be
strong\!\!*
*<span style="FONT-FAMILY: Wingdings; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-char-type: symbol; mso-symbol-font-family: Wingdings"><span style="mso-char-type: symbol; mso-symbol-font-family: Wingdings">J</span></span>*

I hope you liked the post\!

 

Regards,<span id="_GoBack"></span>

 

Sebastian

 

 

</div>

</div>
