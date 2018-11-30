<div id="page">

# .MSG File Format, Rights Managed Email Message (Part 2)

[tomjebo](https://social.msdn.microsoft.com/profile/tomjebo) 6/20/2010
12:32:11 PM

-----

<div id="content">

In my [last
blog](http://blogs.msdn.com/b/openspecification/archive/2009/11/06/msg-file-format-part-1.aspx),
I discussed the general format for an email message saved to disk by
Outlook or a compatible email client. The .msg file format explained in
[MS-OXMSG](http://msdn.microsoft.com/en-us/library/cc463912\(v=EXCHG.80\).aspx),
is a Compound File with storages and streams containing properties and
data describing the email message. I referred to a type of email message
called a “rights managed email message”. This is an email message that
has been encrypted and compressed in such a way that any email client
receiving the message via normal communication with an Exchange server,
will not be able to perform certain operations on the contents unless
the user logged into that client is authorized.

In this discussion, I will be giving a brief description of a rights
managed email message as specified by
[MS-OXORMMS](http://msdn.microsoft.com/en-us/library/cc463909\(v=EXCHG.80\).aspx)
and then I will show some code snippets that I used to decompress and
extract the email contents from such a message once located.

###### What is a rights managed email message?

A rights managed email message is one whose contents have been protected
by a “use license” acquired from a rights management server. I won’t go
into the details of rights management servers and the interaction
between them and client applications, but here is a quick, simple
description. The client application (Outlook in this case), initializes
a relationship with a rights management server by following the steps
outlined in [MS-RMPR “Rights Management Services (RMS): Client-to-Server
Protocol
Specification”](http://msdn.microsoft.com/en-us/library/cc243191\(v=PROT.13\).aspx).
This results in a [DRM
license](http://en.wikipedia.org/wiki/Digital_rights_management) that is
an [XrML stream](http://www.xrml.org/about.asp) to be stored in the
email message and used by the receiving email clients. This use license
determines the actions that can be done to and with the attached email
message.

In addition to the use license, the contents of the original email
message are encrypted and compressed and stored as an attachment in a
wrapper email message.

###### How do I know if a .msg represents a rights managed message?

As I alluded to just now, an email client creating a rights managed
message actually creates two messages. The first, is referred to as the
wrapper message. This message looks just like any email message when
stored in .msg format. It has storages and streams that conform to
[MS-OXMSG](http://msdn.microsoft.com/en-us/library/cc463912\(v=EXCHG.80\).aspx),
with properties that describe things like the recipient, subject and
content. For
example:

[![clip\_image002\[4\]](images/6661.clip_image002%5B4%5D_thumb.jpg
"clip_image002[4]")](images/4540.clip_image002%5B4%5D.jpg)

The tag 0x01CA001F in the \_\_substg1.0\_01CA001F stream name above
represents the value for the sending mailbox owner’s name (section
2.1120 PidTagSenderName in
MS-OXPROPS):

[![clip\_image004\[4\]](images/8662.clip_image004%5B4%5D_thumb.jpg
"clip_image004[4]")](images/3056.clip_image004%5B4%5D.jpg)

Again, that’s me. But the point is that the wrapper contains all the
required properties to be a proper email message. The one extra element
that you will find is an attachment. This attachment will have the
following properties:

*PidTagAttachLongFilename – set to the value “"message.rpmsg"*

*PidTagAttachMimeTag – set to the value
"application/x-microsoft-rpmsg-message"*

This message.rpmsg attachment should be the only attachment but if there
happen to be others, message.rpmsg must be the first one. Here’s what
the data name property looks like for this
attachment:

[![clip\_image006\[4\]](images/5428.clip_image006%5B4%5D_thumb.jpg
"clip_image006[4]")](images/4454.clip_image006%5B4%5D.jpg)

The \_substg1.0\_3707001F stream contains the value for the 0x3707001F
or PidTagAttachFilename, at offset 0x8400 which
contains:

[![clip\_image007\[4\]](images/7587.clip_image007%5B4%5D_thumb.png
"clip_image007[4]")](images/8168.clip_image007%5B4%5D.png)

So we know that we have the correct attachment.

Let’s take a look at the binary data in this
attachment.

[![clip\_image009\[4\]](images/5543.clip_image009%5B4%5D_thumb.jpg
"clip_image009[4]")](images/1323.clip_image009%5B4%5D.jpg)

Above we see the stream for property 0x37010102 or the
PidTagAttachDataBinary property which is the data for the
attachment:

[![clip\_image011\[4\]](images/6052.clip_image011%5B4%5D_thumb.jpg
"clip_image011[4]")](images/8686.clip_image011%5B4%5D.jpg)

 

-----

###### Some code to decompress the storage container

According to section 3.1.4.1.2.1 of
[MS-OXORMMS](http://msdn.microsoft.com/en-us/library/cc463909\(v=EXCHG.80\).aspx),
the message.rpmsg fundamentally has two things, a signature which looks
like this:

0x76 0xE8 0x04 0x60 0xC4 0x11 0xE3 0x86 which translates to “vè\`Äã†”.
This is garbage, I just wanted to make sure you were following me and
hadn’t fallen asleep.

The second part of this binary data is the compressed contents of
storage container with the original email message that was being
protected. The contents are compressed using the ZLIB compression
specified in [RFC1950](http://www.ietf.org/rfc/rfc1950.txt). The best
way to deal with this content is to use the freely downloaded [ZLib
library](http://zlib.net/) with your own code to decompress these
contents and find out what is stored within. Below is an example of code
that calls the [ZLib library
functions](http://zlib.net/manual.html#Basic). This is excerpt code and
I’m assuming you can interpolate the obvious parts of the code like
variable declarations not listed, etc…

First compare the bytes to make sure you have the signature, then take
one block at a time and call inflate() on it. Each block is denoted by
0x0FA0 marker:

<table>
<colgroup>
<col style="width: 100%" />
</colgroup>
<tbody>
<tr class="odd">
<td><p>#define ZLIB_BUFFER_SIZE (4 * 1024)</p>
<p>#define ZLIB_DRM_HEADER_MAGIC (0x0FA0)</p>
<p>static const char c_szCompressedDrmMessageHeader[] = &quot;\x76\xE8\x04\x60\xC4\x11\xE3\x86&quot;;</p>
<p>CHAR szHeader[sizeof(c_szCompressedDrmMessageHeader)] = {0};</p>
<p>streamIn=fopen(argv[paramFileIn], &quot;rb&quot;);</p>
<p>fread(szHeader, 1, sizeof(szHeader)-1, streamIn);</p>
<p>if (strcmp(szHeader, c_szCompressedDrmMessageHeader) != 0)</p>
<p>{</p>
<p>printf(&quot;Not a DRM message header\n&quot;</p>
<p>return -1;</p>
<p>}</p>
<p>z_stream zcpr;</p>
<p>ZLIBDRMHEADER theHeader = {0};</p>
<p>int ret=Z_OK;</p>
<p>long lOrigDone = 0;</p>
<p>int step=0;</p>
<p>memset(&amp;zcpr,0,sizeof(z_stream));</p>
<p>inflateInit(&amp;zcpr);</p>
<p>streamOut=fopen(argv[paramFileOut], &quot;wb&quot;);</p>
<p>while (true)</p>
<p>{</p>
<p>ZeroMemory(rgbOriginal, sizeof(rgbOriginal));</p>
<p>if (fread(&amp;theHeader, sizeof(theHeader), 1, streamIn) == 0)</p>
<p>{</p>
<p>printf(&quot;read failed&quot;);</p>
<p>break;</p>
<p>}</p>
<p>if (theHeader.ulCheck != ZLIB_DRM_HEADER_MAGIC)</p>
<p>{</p>
<p>printf(&quot;Header Check failed!&quot;);</p>
<p>return -3;</p>
<p>}</p>
<p>if (fread(rgbCompressed, theHeader.cbCompressed, 1, streamIn) == 0)</p>
<p>{</p>
<p>printf(&quot;read failed&quot;);</p>
<p>return -4;</p>
<p>}</p>
<p>zcpr.next_in = rgbCompressed;</p>
<p>zcpr.next_out = rgbOriginal;</p>
<p>zcpr.avail_in = theHeader.cbCompressed;</p>
<p>zcpr.avail_out = sizeof(rgbOriginal);</p>
<p>do</p>
<p>{</p>
<p>ret=inflate(&amp;zcpr,Z_SYNC_FLUSH);</p>
<p>step++;</p>
<p>} while (ret==Z_OK);</p>
<p>fwrite(rgbOriginal, 1, theHeader.cbUncompressed, streamOut);</p>
<p>}</p>
<p>}</p></td>
</tr>
</tbody>
</table>

Once you have inflated the compressed data in the attachment stream as
shown in the example code above, you will have the storage container for
the original message along with some additional properties related to
rights management. The format of the container is detailed in section
[3.1.4.1.3 of MS-OXORMMS, “Format of the Storage
Container”](http://msdn.microsoft.com/en-us/library/ee218435\(v=EXCHG.80\).aspx).
In the example rights managed email message that I’ve used for this
blog, the resulting storage container structure looks like
this:

[![clip\_image013\[4\]](images/8422.clip_image013%5B4%5D_thumb.jpg
"clip_image013[4]")](images/1641.clip_image013%5B4%5D.jpg)

Notice that this has all the storages and streams specified in section
[3.1.4.1.3 of
MS-OXORMMS](http://msdn.microsoft.com/en-us/library/ee218435\(v=EXCHG.80\).aspx).
Two of the most important are DRMContent and DRMTransform\\Primary
streams. The former is the encrypted body of the original email message
and the latter contains the transformation information on how to decode
it. Looking at the DRMTransform\\Primary stream, we
see:

[![clip\_image015\[4\]](images/6471.clip_image015%5B4%5D_thumb.jpg
"clip_image015[4]")](images/6457.clip_image015%5B4%5D.jpg)

Notice the “XrML…” string. This is XrML format XML data that tells us
how to get access to the original email message and what rights we have
once we decode it. XrML is a standard language used to specify rights on
many kinds of digital content and an overview of the language can be
found in the
[XrMLTechnicalOverview](http://www.xrml.org/Reference/XrMLTechnicalOverviewV1.pdf)
PDF file. I will reserve explanation of XrML and the subsequent decoding
and rights discernment in this sample message to another installment of
this blog. The encrypted contents of the original email message are
shown
here:

[![clip\_image016\[4\]](images/2744.clip_image016%5B4%5D_thumb.png
"clip_image016[4]")](images/1273.clip_image016%5B4%5D.png)

And as you can see this is encrypted.

</div>

</div>
