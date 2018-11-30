<div id="page">

# More ActiveSync

[doms](https://social.msdn.microsoft.com/profile/doms) 10/3/2009
10:21:00 AM

-----

<div id="content">

# More ActiveSync

<span class="MsoSubtleEmphasis">*Dominic Michael Salemno*</span>

## Introduction

In my previous ActiveSync blog, entitled
<span class="MsoIntenseEmphasis">***An ActiveSync Primer***</span>, I
delved into the basics of the ActiveSync Protocol. This is the second
blog in a series intended to thoroughly explain the ActiveSync Protocol.

## Communications

In my previous blog, I stated that ActiveSync uses HTTPS for its
communications channel. This is not necessarily true, only
mostly<span style="FONT-FAMILY: Wingdings; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri; mso-char-type: symbol; mso-symbol-font-family: Wingdings"><span style="mso-char-type: symbol; mso-symbol-font-family: Wingdings">J</span></span>.
The truth is that HTTP can be used in place of HTTPS. This is not as
secure as the latter. The transport method needed will depend on the
Virtual Directory (VDir) configuration on the server.

Each HTTP POST will contain a series of request and response packets.
Each command issued by the client will be in the form of a request. The
response of course is the return packet from the server containing the
results relevant to the preceding request.

Let’s look at an
example:

<div style="BORDER-BOTTOM: #4f81bd 1pt solid; BORDER-LEFT: medium none; PADDING-BOTTOM: 4pt; PADDING-LEFT: 0in; PADDING-RIGHT: 0in; MARGIN-LEFT: 0.65in; BORDER-TOP: medium none; MARGIN-RIGHT: 0.65in; BORDER-RIGHT: medium none; PADDING-TOP: 0in; mso-border-bottom-themecolor: accent1; mso-element: para-border-div; mso-border-bottom-alt: solid #4F81BD .5pt">

***This example fetches an e-mail item from the server.***

</div>

### Request

POST /Microsoft-Server-

ActiveSync?Cmd=ItemOperations\&User=deviceuser\&DeviceId=device1&

DeviceType=PocketPC HTTP/1.1

Content-Type: application/vnd.ms-sync.wbxml

MS-ASProtocolVersion: 14.0

<span style="mso-spacerun: yes"></span> 

\<?xml version="1.0" encoding="utf-8"?\>

\<ItemOperations xmlns:airsync="AirSync:"

xmlns:airsyncbase="AirSyncBase:" xmlns="ItemOperations:"\>

<span style="mso-spacerun: yes">  </span>\<Fetch\>

<span style="mso-spacerun: yes">    </span>\<Store\>Mailbox\</Store\>

<span style="mso-spacerun: yes">   
</span>\<airsync:CollectionId\>7\</airsync:CollectionId\>

<span style="mso-spacerun: yes">   
</span>\<airsync:ServerId\>7:1\</airsync:ServerId\>

<span style="mso-spacerun: yes">    </span>\<Options\>

<span style="mso-spacerun: yes">     
</span>\<airsyncbase:BodyPreference\>

<span style="mso-spacerun: yes">       
</span>\<airsyncbase:Type\>1\</airsyncbase:Type\>

<span style="mso-spacerun: yes">       
</span>\<airsyncbase:TruncationSize\>5120\</airsyncbase:TruncationSize\>

<span style="mso-spacerun: yes">       
</span>\<airsyncbase:AllOrNone\>0\</airsyncbase:AllOrNone\>

<span style="mso-spacerun: yes">     
</span>\</airsyncbase:BodyPreference\>

<span style="mso-spacerun: yes">    </span>\</Options\>

<span style="mso-spacerun: yes">  </span>\</Fetch\>

\</ItemOperations\>

### Response

HTTP/1.1 200 OK

Cache-Control: private

Content-Length: 409

Content-Type: application/vnd.ms-sync

Server: Microsoft-IIS/6.0

MS-Server-ActiveSync: 14.0

Date: Tue, 08 May 2007 17:29:52 GMT

 

\<?xml version="1.0" encoding="utf-8"?\>\<ItemOperations

xmlns:airsync="AirSync:" xmlns:email="POOMMAIL:"

xmlns="ItemOperations:"\>

<span style="mso-spacerun: yes">  </span>\<Status\>1\</Status\>

<span style="mso-spacerun: yes">  </span>\<Response\>

<span style="mso-spacerun: yes">    </span>\<Fetch\>

<span style="mso-spacerun: yes">      </span>\<Status\>1\</Status\>

<span style="mso-spacerun: yes">     
</span>\<airsync:CollectionId\>7\</airsync:CollectionId\>

<span style="mso-spacerun: yes">     
</span>\<airsync:ServerId\>7:1\</airsync:ServerId\>

<span style="mso-spacerun: yes">     
</span>\<airsync:Class\>Email\</airsync:Class\>

<span style="mso-spacerun: yes">      </span>\<Properties\>

<span style="mso-spacerun: yes">        </span>\<email:To\>"deviceuser"
\&lt;someone1@example.com\&gt;\</email:To\>

<span style="mso-spacerun: yes">        </span>\<email:Cc\>"deviceuser3"
\&lt;someone3@example.com\&gt;\</email:Cc\>

<span style="mso-spacerun: yes">       
</span>\<email:From\>"deviceuser2" \&lt;someone2@example.com\&gt;

<span style="mso-spacerun: yes">        </span>\</email:From\>

<span style="mso-spacerun: yes">       
</span>\<email:Subject\>Subject\</email:Subject\>

<span style="mso-spacerun: yes">       
</span>\<email:DateReceived\>2007-05-08T17:29:07.890Z

<span style="mso-spacerun: yes">        </span>\</email:DateReceived\>

<span style="mso-spacerun: yes">       
</span>\<email:DisplayTo\>DeviceUserDisplayName\</email:DisplayTo\>

<span style="mso-spacerun: yes">       
</span>\<email:ThreadTopic\>Subject\</email:ThreadTopic\>

<span style="mso-spacerun: yes">       
</span>\<email:Importance\>1\</email:Importance\>

<span style="mso-spacerun: yes">       
</span>\<email:Read\>0\</email:Read\>

<span style="mso-spacerun: yes">        </span>\<airsyncbase:Body\>

<span style="mso-spacerun: yes">         
</span>\<airsyncbase:Type\>1\</airsyncbase:Type\>

<span style="mso-spacerun: yes">         
</span>\<airsyncbase:EstimatedDataSize\>20

<span style="mso-spacerun: yes">         
</span>\</airsyncbase:EstimatedDataSize\>

<span style="mso-spacerun: yes">         
</span>\<airsyncbase:Data\>Body as plain text\</airsyncbase:Data\>

<span style="mso-spacerun: yes">        </span>\</airsyncbase:Body\>

<span style="mso-spacerun: yes">       
</span>\<email:MessageClass\>IPM.Note\</email:MessageClass\>

<span style="mso-spacerun: yes">       
</span>\<email:InternetCPID\>28591\</email:InternetCPID\>

<span style="mso-spacerun: yes">        </span>\<email:Flag /\>

<span style="mso-spacerun: yes">       
</span>\<email:ContentClass\>urn:content-classes:message

<span style="mso-spacerun: yes">        </span>\</email:ContentClass\>

<span style="mso-spacerun: yes">       
</span>\<airsyncbase:NativeBodyType\>1\</airsyncbase:NativeBodyType\>

<span style="mso-spacerun: yes">      </span>\</Properties\>

<span style="mso-spacerun: yes">    </span>\</Fetch\>

<span style="mso-spacerun: yes">  </span>\</Response\>

\</ItemOperations\>

 

The request illustrates using the
<span class="MsoIntenseEmphasis">***fetch***</span> command to obtain an
e-mail message from the server. Subsequently, the server sends out a
<span class="IntenseQuoteChar">***fetch response***</span> containing
the requested e-mail message.

This command as well as other commands is explained in much detail in
the
[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***\[MS-ASCMD\]***</span></span>](http://msdn.microsoft.com/en-us/library/dd299441.aspx)
document
([<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***ActiveSync
Command Reference Protocol
Specification***</span></span>](http://msdn.microsoft.com/en-us/library/dd208104\(PROT.10\).aspx)).
This document describes all of the various ActiveSync commands that can
be sent over the wire.

## WBXML

The above examples are shown using the XML structures, not WBXML. This
is to simplify matters and not in actuality. WBXML is not used for all
communications, but most. In the request above, you’ll notice that WBXML
is specified in the header:

<span class="MsoSubtleEmphasis">*Content-Type:
application/vnd.ms-sync.wbxml* </span>

 

So really, this message should be encoded using WBXML, but the XML
equivalent is shown for simplicity’s sake. This is done across the
ActiveSync specification documents found in
[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***Microsoft’s
Open
Specifications***</span></span>](http://msdn.microsoft.com/en-us/library/dd208104\(PROT.10\).aspx).

## Overview

To reiterate the concept behind ActiveSync, simplistically, ActiveSync
ensures the mobile device in question is constantly sending various
request packets to the server (usually an Exchange Server) in an effort
to maintain the most up-to-date information on the device. Whether this
is simply pulling new information from the server, or sending updated
information from the device.

All of the protocol documents describing this protocol in detail can be
found on
[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***Microsoft’s
Open Specifications
site***</span></span>](http://msdn.microsoft.com/en-us/library/dd208104\(PROT.10\).aspx).
Underneath the
[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***Exchange
Server Protocols
section***</span></span>](http://msdn.microsoft.com/en-us/library/cc307725.aspx),
one will find every document describing this protocol to begin with
\[MS-AS (AS denoting ActiveSync). All of the documents in question are
as
follows:

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***\[MS-ASAIRS\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299454.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASCAL\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299440.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASCMD\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299441.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASCNTC\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299451.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASCON\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd633488.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASDOC\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299453.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASDTYPE\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299455.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASEMAIL\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299457.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASHTTP\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299446.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASMS\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd633486.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASNOTE\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd633485.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASPROV\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299443.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASTASK\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299452.aspx)<span class="MsoIntenseEmphasis"></span>

[<span class="MsoIntenseEmphasis"><span style="TEXT-DECORATION: none; text-underline: none">***<span style="mso-spacerun: yes"> </span>\[MS-ASWBXML\].pdf***</span></span>](http://msdn.microsoft.com/en-us/library/dd299442.aspx)<span class="MsoIntenseEmphasis">
****** </span>

 

These documents can be daunting, so in an effort to simplify matters,
these series will assist those wanting to learn the protocol in more
depth. The next blog will walk through initialization and
synchronization of a simple session.

[More
ActiveSync.pdf](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/09/90/27/62/More%20ActiveSync.pdf)

</div>

</div>
