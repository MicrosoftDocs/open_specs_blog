<div id="page">

# An ActiveSync Primer

[doms](https://social.msdn.microsoft.com/profile/doms) 9/18/2009 1:20:00
PM

-----

<div id="content">

# An ActiveSync Primer

<span class="MsoSubtleEmphasis">*Dominic Salemno*</span>

## What is ActiveSync?

When I speak of ActiveSync, I am speaking in regards to the protocol
itself, not the application. In any communications protocol, there is
always a purpose for the transmission. In the case of ActiveSync, it
provides a means of ensuring that a specific mobile device is consistent
in data, e-mail, and contacts with an exchange account. Ultimately, the
goal is for the mobile device to contain the most up-to-date
information.

## How does it work?

The ActiveSync protocol uses request and response commands transported
over HTTPS (HTTP Secured by Secure Sockets Layer). Each ActiveSync
command is formatted in binary-encoded XML (WBXML) and sent via an HTTP
POST (See \[MS-ASWBXML\]:
<http://msdn.microsoft.com/en-us/library/dd299442.aspx>). This WBXML
corresponds directly to XML elements and attributes by a tokenization
process. The purpose behind this encoding is for efficient bandwidth
usage due to the low-bandwidth nature of mobile clients employing the
ActiveSync protocol.

Traversing down the stack, by undoing the tokenization process, you will
see the XML Message. Each XML Message will vary in size and structure
based upon the type of communications being done. However, the basic
principle remains the same. Each XML Message will either be a request or
a
response.

## An analysis of a sample ActiveSync Packet

### <span style="COLOR: #943634; mso-themecolor: accent2; mso-themeshade: 191">An ActiveSync XML Message</span>

<span style="FONT-FAMILY: Consolas; COLOR: black"></span>

 

<span style="FONT-FAMILY: Consolas; COLOR: black">\<?xml version="1.0"
encoding="utf-8"?\> </span>

<span style="FONT-FAMILY: Consolas; COLOR: black">\<FolderSync
xmlns="FolderHierarchy:"\>
</span>

<span style="FONT-FAMILY: Consolas; COLOR: black"><span style="mso-spacerun: yes"> 
</span>\<SyncKey\>2\</SyncKey\></span>

<span style="FONT-FAMILY: Consolas; COLOR: black">\</FolderSync\>
</span>

<span style="FONT-FAMILY: Consolas; COLOR: black"></span>

 

**<span style="COLOR: #943634; mso-themecolor: accent2; mso-themeshade: 191">Binary-Encoded
XML (WBXML) Representation of the XML
Message</span>**

<span style="LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Lucida Console&#39;; COLOR: black; FONT-SIZE: 10pt; mso-bidi-font-family: &#39;Lucida Console&#39;">03
01 6A 00 00 07 56 52 03 32 00 01 01
</span>

**<span style="COLOR: #943634; mso-themecolor: accent2; mso-themeshade: 191">HTTP
POST Header and Body  
</span>**<span class="MsoSubtleEmphasis">*See \[MS-ASHTTP\]:*
</span>[<span class="MsoSubtleEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">*http://msdn.microsoft.com/en-us/library/dd299446.aspx*</span></span>](http://msdn.microsoft.com/en-us/library/dd299446.aspx)**<span style="COLOR: #943634; mso-themecolor: accent2; mso-themeshade: 191"></span>**

POST
/Microsoft-Server  
ActiveSync?Cmd=FolderSync\&User=fakename\&DeviceId=v140Device\&DeviceType=SmartPhone
HTTP/1.1  
Content-Type: application/vnd.ms-sync.wbxml  
MS-ASProtocolVersion: 14.0  
User-Agent: ASOM  
Host:
Contoso.com  
  
**<span style="COLOR: #943634; mso-themecolor: accent2; mso-themeshade: 191"></span>**

<span style="LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Lucida Console&#39;; COLOR: black; FONT-SIZE: 10pt; mso-bidi-font-family: &#39;Lucida Console&#39;">03
01 6A 00 00 07 56 52 03 32 00 01 01</span>

## An Overview of the WBXML Encoding

The following table presents how the WBXML encoding has
happened

**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-SIZE: 14pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Bytes</span>**

</div>

</div>

**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-SIZE: 14pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Description</span>**

03

Version number – WBXML version 1.3

01

Unknown public identifier

6A

**<span style="COLOR: #00b050">Charset</span>** = UTF-8

00

String table length

00 07

Select code page 7 (FolderHierarchy)

56

\<FolderSync\>, with content (0x16 + 0x40)

52

\<SyncKey\>, with content

03

Inline string follows

32 00

2

01

\</SyncKey\>

01

\</FolderSync\>

## 

 

## Summary

This blog was intended to present an introduction to the ActiveSync
protocol. Although the example was fairly simple, this is the first step
in grasping the foundation for the subsequent information in the series
to this entry. Stay-tuned for more articles getting more in-depth to the
intricate details surrounding
ActiveSync\!

[ActiveSync\_Primer.pdf](images/ActiveSync_Primer.pdf)
