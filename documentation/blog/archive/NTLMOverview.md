<div id="page">

# NTLM Overview

[Bill Wesse](https://social.msdn.microsoft.com/profile/Bill%20Wesse)
5/1/2009 6:51:00 AM

-----

<div id="content">

This blog entry is intended for those readers seeking a consolidated
reference for high-level aspects of the NTLM Protocol, as well as those
who have occasion to analyze network traffic with Wireshark (a
registered trademark of the Wireshark Foundation), Microsoft Network
Monitor (v3.3 at the initial release of this document), and so on.

A zip file (\[NTLM-OVERVIEW\].zip) is attached to this entry, containing
\[NTLM-OVERVIEW\].pdf, as well as a handful of short network traces
illustrating NTLM authentication on various protocols.

The Open Specification documents, as well as all other cited documents
are meant for in-depth reference to the protocol details.

NTLM is a Challenge/Response authentication method used across many
network protocols, and originated as a successor to LANMAN (Microsoft
LAN Manager) authentication.

The attached documentation is limited to the following
protocols:

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Remote Procedure Call (RPC)
[\[C706\]](http://www.opengroup.org/public/pubs/catalog/c706.htm)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Server Message Block (SMB) Protocol
<span style="COLOR: #0070c0">\[MS-SMB\]</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Server Message Block (SMB) Version 2 Protocol
<span style="COLOR: #0070c0">\[MS-SMB2\]</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Session Initiation Protocol (SIP)
[\[RFC3261\]](http://www.ietf.org/rfc/rfc3261.txt)

Information concerning additional protocols that use NTLM authentication
can be found in the documents listed below. Note that Microsoft Network
Monitor 3.3 includes parsers for
these.

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-MMSP\]: Microsoft Media Server (MMS) Protocol
Specification

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-NNS\]: .NET NegotiateStream Protocol
Specification

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-NNTP\]: NT LAN Manager (NTLM) Authentication:
Network News Transfer Protocol (NNTP)
Extension

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-NTHT\]: NTLM Over HTTP Protocol
Specification

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-POP3\]: NT LAN Manager (NTLM) Authentication:
Post Office Protocol - Version 3 (POP3)
Extension

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-SMTP\]: NT LAN Manager (NTLM) Authentication:
Simple Mail Transfer Protocol (SMTP)
Extension

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-TDS\]: Tabular Data Stream Protocol
Specification

<span style="mso-spacerun: yes"></span> 

Additional protocols: HTTP, LDAP, Telnet.

 

Captures:

**Capture File**

</div>

</div>

**NTLM**

**Protocol**

**Client**

**Server**

rpc\_ntlmv2.cap

v2

RPC

Windows 2003

Windows 2003

smb\_ntlmv2.cap

v2

SMB

obfuscated

Windows 2003

smb\_ntlmv2\_implicit.cap

v2

SMB

Windows Xp

obfuscated

smb\_ntlmv2\_spnego.cap

v2

SMB

Windows Xp

Windows 2003

smb2\_spnego\_ntlmv2.cap

v2

SMB2

Windows 2008

Windows 2008

smtp\_ntlmv2.cap

V2

SMTP

Windows Xp

Windows 2000

sip\_ntlmv2.cap

v2

SIP

Windows Xp

Windows
2003

 

[\[NTLM-OVERVIEW\].zip](images/%5BNTLM-OVERVIEW%5D.zip)
