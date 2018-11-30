<div id="page">

# Beginning with the PowerPoint Document Stream

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
10/19/2009 3:11:00 PM

-----

<div id="content">

This blog will expand on my previous blog
[<span class="underline">Parsing Pictures in a PowerPoint binary
file</span>](http://blogs.msdn.com/openspecification/archive/2009/09/04/parsing-pictures-in-a-powerpoint-binary-file.aspx),
which details the Pictures Stream, and how you might parse the stream to
extract pictures contained in the PowerPoint document. I’ll extend the
concepts of the previous blog to apply to parsing the “**PowerPoint
Document**” stream.

You’ll notice as you read through the following that you could just use
the Pictures Stream, as shown previously, instead of the PowerPoint
Document stream (the OfficeArtDggContainer) and accomplish the same
thing, and in the strictest sense, yes, this is “almost” another way of
doing the same thing. However, if you are building a generalized parser
for PowerPoint binary files, and not just a one-off picture enumeration
or extraction tool, then you are expanding beyond just the Pictures
Stream and implementing a larger portion of the binary specification, so
this is an evolutionary step in that direction.

Also, having detailed in the previous blog, and in this one, how to
manually parse Office binary files with a Hex editor and code usage of
IStorage to browse over the streams, at the end of this blog I’ll
introduce a new tool by the [<span class="underline">Microsoft Security
Response Center team</span>](http://blogs.technet.com/srd/default.aspx)
(MSRC) that makes analyzing Office binary files much, much easier.

The binary file specification of course is still PowerPoint
([<span class="underline">MS-PPT</span>](http://msdn.microsoft.com/en-us/library/cc313106.aspx)),
which you may use to follow along. However, as before, much of the
parsing details will derive from definitions of structures in
[<span class="underline">MS-ODRAW</span>](http://msdn.microsoft.com/en-us/library/cc441433.aspx).

Let’s get started…

The PowerPoint Document Stream is defined in
[<span class="underline">MS-PPT</span>](http://msdn.microsoft.com/en-us/library/cc313106.aspx)
section 2.1.2:

*A required stream whose name MUST be "PowerPoint Document".*

**

*The contents of this stream are specified by a sequence of top-level
records.*

**

*Let a top-level record be specified as any one of the following:
DocumentContainer,*

*MasterOrSlideContainer, HandoutContainer, SlideContainer,
NotesContainer, ExOleObjStg,*

*ExControlStg, VbaProjectStg, PersistDirectoryAtom, or UserEditAtom
record.*

**

You may build on what we did previously by using the same code snippet
and replacing the stream name with “PowerPoint Document” and/or follow
the steps defined in
[<span class="underline">MS-PPT</span>](http://msdn.microsoft.com/en-us/library/cc313106.aspx)
**section 2.1.2** to parse the “PowerPoint Document” stream to the
**Document Container**, section 2.4.1 then to the
**DrawingGroupContainer** (OfficeArtDgg), section 2.4.3. The definition
of OfficeArtDgg is in
[<span class="underline">MS-ODRAW</span>](http://msdn.microsoft.com/en-us/library/cc441433.aspx),
section 2.2.12 and is called OfficeArtDggContainer: This record
specifies the container for *all OfficeArt file records* *containing
**document-wide data***. The OfficeArt record types are defined in
[<span class="underline">MS-ODRAW</span>](http://msdn.microsoft.com/en-us/library/cc441433.aspx)
section 2.2.

This is where we’ll start with the OfficeArtDggContainer structure
(highlighted fields follow in sequence/color with the definitions):

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73A0h:
**<span style="BACKGROUND: silver; COLOR: #00b050; mso-highlight: silver">0F
00 00 F0</span>** 30 01 00 00
<span style="BACKGROUND: olive; COLOR: white; mso-highlight: olive; mso-themecolor: background1">00
00 06 F0</span> 78 00 00 00<span style="mso-spacerun: yes"> 
</span>...ð0......ðx... </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73B0h:
01 34 00 00 0E 00 00 00 15 00 00 00 0D 00 00
00<span style="mso-spacerun: yes">  </span>.4.............. </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73C0h:
01 00 00 00 07 00 00 00 0D 00 00 00 05 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73D0h:
0C 00 00 00 01 00 00 00 0B 00 00 00 01 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73E0h:
0A 00 00 00 01 00 00 00 09 00 00 00 01 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">73F0h:
08 00 00 00 01 00 00 00 07 00 00 00 01 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7400h:
06 00 00 00 01 00 00 00 05 00 00 00 01 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7410h:
04 00 00 00 01 00 00 00 03 00 00 00 01 00 00
00<span style="mso-spacerun: yes">  </span>................ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7420h:
02 00 00 00 01 00 00 00
<span style="BACKGROUND: silver; mso-highlight: silver">2F 00 01
F0</span> 58 00 00 00<span style="mso-spacerun: yes"> 
</span>......../..ðX... </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7430h:
<span style="BACKGROUND: lime; mso-highlight: lime">5</span><span style="BACKGROUND: silver; mso-highlight: silver">2</span>
<span style="BACKGROUND: yellow; mso-highlight: yellow">00</span>
<span style="BACKGROUND: silver; mso-highlight: silver">07 F0</span> 24
00 00 00 <span style="BACKGROUND: lime; mso-highlight: lime">05</span>
05
<span style="BACKGROUND: olive; COLOR: white; mso-highlight: olive; mso-themecolor: background1">B1
A6 19 08 D8 C3</span><span style="mso-spacerun: yes"> 
</span>R..ð$.....±¦..ØÃ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7440h:
<span style="BACKGROUND: olive; COLOR: white; mso-highlight: olive; mso-themecolor: background1">0B
6F B5 6C A3 98 8C 9E F4 65</span> FF 00
<span style="BACKGROUND: yellow; mso-highlight: yellow">B6 03 00
00</span><span style="mso-spacerun: yes">  </span>.oµl£˜Œžôeÿ.¶...
</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7450h:
<span style="BACKGROUND: lime; mso-highlight: lime">01 00 00 00</span>
<span style="BACKGROUND: aqua; mso-highlight: aqua">00 00 00 00</span>
00 00 00 00
<span style="BACKGROUND: lime; mso-highlight: lime">3</span><span style="BACKGROUND: silver; mso-highlight: silver">2</span>
<span style="BACKGROUND: yellow; mso-highlight: yellow">00</span>
<span style="BACKGROUND: silver; mso-highlight: silver">07
F0</span><span style="mso-spacerun: yes"> 
</span>............2..ð</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7460h:
24 00 00 00
<span style="BACKGROUND: lime; mso-highlight: lime">03</span> 04
<span style="BACKGROUND: olive; COLOR: white; mso-highlight: olive; mso-themecolor: background1">27
CF 5A 3B 2E DC 9E 2E 16 A1</span><span style="mso-spacerun: yes"> 
</span>$.....'ÏZ;.Üž..¡ </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7470h:
<span style="BACKGROUND: olive; COLOR: white; mso-highlight: olive; mso-themecolor: background1">A1
59 52 1B 76 E9</span> FF 00
<span style="BACKGROUND: yellow; mso-highlight: yellow">40 6D 00
00</span> <span style="BACKGROUND: lime; mso-highlight: lime">01 00 00
00</span><span style="mso-spacerun: yes">  </span>¡YR.véÿ.@m......
</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">7480h:
<span style="BACKGROUND: aqua; mso-highlight: aqua">B6 03 00 00</span>
00 00 00 00<span style="mso-spacerun: yes">                         
</span>¶.......<span style="mso-spacerun: yes">  
</span></span>

****

**<span style="BACKGROUND: silver; FONT-SIZE: 10pt; mso-highlight: silver">2.2.12  
<span style="COLOR: #00b050">OfficeArtDggContainer</span></span><span style="FONT-SIZE: 10pt"></span>**

<span style="FONT-SIZE: 10pt"></span>

 

<span style="BACKGROUND: olive; COLOR: white; FONT-SIZE: 10pt; mso-highlight: olive">2.2.48  
OfficeArtFDGGBlock</span><span style="COLOR: white; FONT-SIZE: 10pt"></span>

<span style="FONT-SIZE: 10pt">Referenced by:
OfficeArtDggContainer</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span>**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: silver; FONT-SIZE: 10pt; mso-highlight: silver; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">2.2.20  
OfficeArtBStoreContainer</span>**</span>

<span style="FONT-SIZE: 10pt">Referenced by: OfficeArtDggContainer
</span>

<span style="FONT-SIZE: 10pt">This record specifies the container for
all BLIPs used in all drawings associated with the parent
OfficeArtDggContainer record.</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span style="FONT-SIZE: 10pt">**2.2.22  
OfficeArtBStoreContainerFileBlock**</span>

<span style="FONT-SIZE: 10pt">Referenced by:
**OfficeArtBStoreContainer**, OfficeArtBStoreDelay,
OfficeArtInlineSpContainer</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span>**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: silver; FONT-SIZE: 10pt; mso-highlight: silver; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">2.2.32  
OfficeArtFBSE</span>**</span>

<span style="FONT-SIZE: 10pt">Referenced by:
OfficeArtBStoreContainerFileBlock </span>

<span style="FONT-SIZE: 10pt">This record specifies a File BLIP Store
Entry (FBSE) that contains information about the BLIP.</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span style="FONT-SIZE: 10pt">**Field                      Meaning**
</span>

<span style="FONT-SIZE: 10pt">rh.recVer             MUST be
0x<span style="BACKGROUND: silver; mso-highlight: silver">2</span>.
</span>

<span style="FONT-SIZE: 10pt">rh.recInstance   MUST be the
<span style="COLOR: black">BLIP type</span>.</span>

<span style="FONT-SIZE: 10pt">\*\* section
2.4.1 <span style="BACKGROUND: lime; mso-highlight: lime">MSOBLIPTYPE</span>:</span>

<span>**<span style="mso-spacerun: yes">  
</span>msoblipJPEG**:<span style="mso-tab-count: 1">    
</span><span style="BACKGROUND: lime; mso-highlight: lime">0x05</span> 
JPEG format.</span>

<span>**<span style="mso-spacerun: yes">  
</span>msoblipWMF**:<span style="mso-tab-count: 1">   
</span><span style="BACKGROUND: lime; mso-highlight: lime">0x03</span> 
WMF format.</span>

<span style="FONT-SIZE: 10pt">rh.recType          MUST be
<span style="BACKGROUND: silver; mso-highlight: silver">0xF007</span>.
</span>

<span style="FONT-SIZE: 10pt">rh.recLen             An unsigned integer
that specifies the number of bytes following the header. MUST be the
size of nameData in bytes plus 36 if the BLIP is not embedded in this
record or the size of nameData plus **size** plus 36 if the BLIP is
embedded</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span style="FONT-SIZE: 10pt">btWin32 (1 byte): An
<span style="BACKGROUND: lime; mso-highlight: lime">MSOBLIPTYPE</span>
enumeration value that specifies the BLIP type.</span>

*<span style="FONT-SIZE: 10pt">From the example
above</span>*<span style="FONT-SIZE: 10pt"> (section 2.4.1  
<span style="BACKGROUND: lime; mso-highlight: lime">MSOBLIPTYPE</span>,
**msoblipJPEG**: 
<span style="BACKGROUND: lime; mso-highlight: lime">0x05</span> JPEG
format, and
<span style="BACKGROUND: lime; mso-highlight: lime">0x03</span> WMF
format)</span>

<span style="FONT-SIZE: 10pt">…</span>

<span style="BACKGROUND: olive; COLOR: white; FONT-SIZE: 10pt; mso-highlight: olive">rgbUid</span><span style="FONT-SIZE: 10pt">
(16 bytes): An MD4 digest, as specified in \[RFC1320\], that specifies
the unique identifier of the pixel data in the BLIP.
</span>

<span style="FONT-SIZE: 10pt">…</span>

<span><span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: yellow; FONT-SIZE: 10pt; mso-highlight: yellow; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">**size
(4 bytes): An unsigned integer that specifies the size of the BLIP in
bytes in the
stream**</span><span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: yellow; FONT-SIZE: 10pt; mso-highlight: yellow; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">.</span><span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-SIZE: 10pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA"> 
</span></span><span style="FONT-SIZE: 10pt">(0x0000<span style="BACKGROUND: yellow; mso-highlight: yellow">03B6</span>
= 950 bytes, and
0x0000<span style="BACKGROUND: yellow; mso-highlight: yellow">6D40</span>
= 27,968 bytes) \*\* You will note these are the same JPEG and WMF
pictures used in the previous blog with the Pictures Stream, as it is
the same
document.</span>

<span style="BACKGROUND: lime; FONT-SIZE: 10pt; mso-highlight: lime">cRef</span><span style="FONT-SIZE: 10pt">
(4 bytes): An unsigned integer that specifies the number of references
to the BLIP. A value of 0x00000000 specifies an empty slot in the
OfficeArtBStoreContainer.
</span>

<span style="BACKGROUND: aqua; FONT-SIZE: 10pt; mso-highlight: aqua">foDelay</span><span style="FONT-SIZE: 10pt">
(4 bytes): An MSOFO data type that specifies **the file offset into**
the associated
<span style="BACKGROUND: aqua; mso-highlight: aqua">**OfficeArtBStoreDelay**</span>
(delay stream). A value of 0xFFFFFFFF specifies that the file is not in
the delay stream and cRef MUST be 0x00000000.
</span>

<span style="FONT-SIZE: 10pt">…</span>

<span style="FONT-SIZE: 10pt"></span>

 

<span>**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: aqua; FONT-SIZE: 10pt; mso-highlight: aqua; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">2.2.21  
OfficeArtBStoreDelay</span>** </span>

<span style="FONT-SIZE: 10pt">This record specifies the delay loaded
container of BLIPs in the host application. There is no
OfficeArtRecordHeader for this container. </span>

<span style="FONT-SIZE: 10pt">rgfb (variable) </span>

<span style="FONT-SIZE: 10pt">... </span>

<span style="FONT-SIZE: 10pt">rgfb (variable): An array of
OfficeArtBStoreContainerFileBlock records that specifies BLIP data. The
array continues while the rh.recType field of the
OfficeArtBStoreContainerFileBlock record is equal to 0xF007 or between
0xF018 and 0xF117, inclusive.</span>

<span style="FONT-SIZE: 10pt"></span>

 

Having detailed, above, how to manually browse these Office binary files
with a Hex editor, yet again, the following tool I mentioned at the
beginning of this blog will make this exercise a lot easier. They’ve
done all the parsing for you\! And, the application includes file
Defragmentation and Repair tools\!

Introducing, Microsoft Security Response Center team’s
[**<span class="underline">OffVis</span>**](http://blogs.technet.com/srd/archive/2009/07/31/announcing-offvis.aspx)\!

If you followed along in a Hex editor and IStorage up to this point, I
doubt you need much help with **OffVis** since it is a drill down GUI.
Everything we did with the hex editor above in parsing you can do with
**OffVis** using a hierarchical GUI and simply drill down on the objects
and see the hex view of the data on the left side of the window.
**Note**, you can also double click on the hex view window area and it
will adjust your position automatically to the appropriate
hierarchy/record on the hierarchical view window. To get started just
pick your file from the menu ribbon (File-\> Open) then pick your
**Parser** from the list box (in this case
**PowerPoint97\_2003BinaryFormat**) and click **Parse**, then drill down
on the
structures:

<https://hec05q.bay.livefilestore.com/y1mCwVX-xWEz15_NzRizNWunQeFjUFEyc5fkoXojzOVChJ26EEMZWBuaUXnMkhcggM26_4-rn7zOnDbKI7UYsKaxo_25TJIK3Eq52sgUL8KwAshMqmaS8M_P0iglzQWx0RCmZPQcZ2ZsCV0bsfo59XjTA/Pic1.jpg>

<https://hec05q.bay.livefilestore.com/y1mPq7UWfJkjfbqFoCAM1wFyj2yxV3Lj_jjlsU6D5D2k3AwWY0RA91OouicbjWLhNQcJfzKU6D1p1LZlvzvz9JQS-kJakrJmspek-sl-UJsU_BXlu8l5lg0k64XQxiYOA42XeMfkNNuyHoVYBHZTwQtkg/Pic2.jpg>

You can find more information about **OffVis**, including the download
link, on the [<span class="underline">MSRC OffVis Blog
site</span>](http://blogs.technet.com/srd/archive/2009/07/31/announcing-offvis.aspx).

MSRC even provides a [<span class="underline">training video for
**OffVis**</span>](http://research.microsoft.com/en-us/UM/redmond/events/BH09/lecture.htm).
Although this tool is great for assisting with understanding the Office
binary file formats the original purpose of the tool, and the focus of
the video training, is for researching potential Office binary file
exploits.

I hope this helps advance your understanding as you investigate the
Office binary file formats.

Stay tuned for more…

</div>

</div>
