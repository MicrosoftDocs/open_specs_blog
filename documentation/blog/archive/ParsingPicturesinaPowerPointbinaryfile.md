<div id="page">

# Parsing Pictures in a PowerPoint binary file

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
9/4/2009 9:47:00 AM

-----

<div id="content">

<span style="COLOR: black"></span> 

<span style="COLOR: black">In this blog I’d like to cover every aspect
of parsing Office binary documents, and do it in less than a thousand
words.  But, what follows is more realistic, thus more narrow in focus. 
Specifically, </span>I’ll examine the PowerPoint binary format from the
point of view of parsing/enumerating “Pictures”. 
<span style="COLOR: black">PowerPoint Pictures are found in the
**Pictures Stream** (as you will see below).  By contrast, Shape objects
(rectangles, squares, lines, etc) do not exist in the Pictures Stream. 
So the discussion and details of the enumeration of Shape objects may be
the subject of a future
blog.</span>

***<span style="COLOR: #1f497d"> </span>***<span style="COLOR: #1f497d"></span>

There are several Office binary file formats defined as part of the Open
Specification document set and you can find them detailed
[here](http://msdn.microsoft.com/en-us/library/cc313105.aspx).  The one
for which I’ll focus my angle of approach is the PowerPoint binary file
format specification
([MS-PPT](http://msdn.microsoft.com/en-us/library/cc313106.aspx)).  And,
since you can’t find Office pictures in a binary document without
referring to the MS-ODRAW specification, much of the details of this
blog will derive from definitions of structures in
[MS-ODRAW](http://msdn.microsoft.com/en-us/library/cc441433.aspx).
 S<span style="COLOR: black">ince this analysis is specific to
“Pictures”, which means an actual inserted picture in the PowerPoint
document, to follow along you may refer to
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-WEIGHT: normal">MS-PPT
section 2.1.3</span>**
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">Pictures
Stream</span>**: *An optional stream whose name MUST be "Pictures"*. 
The contents of the Pictures Stream are defined by the
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">OfficeArtBStoreDelay</span>**
record as specified in MS-ODRAW section 2.2.21.</span>

 

<span style="COLOR: black">Looking at
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">MS-ODRAW
section 2.2.21</span>**
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">OfficeArtBStoreDelay</span>**:
*This record specifies the delay loaded container of
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">BLIPs</span>**
in the host application. There is no OfficeArtRecordHeader for this
container.*</span>

*<span style="COLOR: black">rgfb (variable): An array of
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">OfficeArtBStoreContainerFileBlock</span>**
records that specifies BLIP data. 
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">The
array continues while the rh.recType field of the
OfficeArtBStoreContainerFileBlock record is equal to 0xF007 or between
0xF018 and 0xF117, inclusive</span>**.</span>*

<span style="COLOR: black"> </span>

<span style="COLOR: black">For an example, I created a very simple
MS-PPT document and inserted (Menu Ribbon -\> Insert Tab) a
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">Picture</span>**
and a
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">ClipArt</span>**
object.  Based on the above specification details I’ll enumerate the
image objects, as shown below (Hex file view after opening the
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">Pictures
Stream</span>** and delving to the
**OfficeArtBStoreContainerFileBlock**).</span>

<span style="COLOR: black"></span>

 

<span style="COLOR: black">You’ll find the color coded details below,
which explain this hex data
block.</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: lime; COLOR: black">A0
46</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black">
<span style="BACKGROUND: yellow">1D F0</span>
<span style="BACKGROUND: silver">AE 03 00 00</span>
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: olive; COLOR: white">B1
A6 19 08 D8 C3 0B
6F</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black">
//image data
follows</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: olive; COLOR: white">B5
6C A3 98 8C 9E F4 65 FF FF D8 FF E0 00 10
4A</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black">…//skipping
forward to the next
record</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: lime; COLOR: black">60
21</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black">
<span style="BACKGROUND: yellow">1B F0</span>
<span style="BACKGROUND: silver">38 6D 00 00</span>
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: olive; COLOR: white">27
CF 5A 3B 2E DC 9E
2E</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: white">
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black">//image
data
follows</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: white">//</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: olive; COLOR: white">16
A1 A1 59 52 1B 76 E9 AC A7 00 00 DC F7 FF
FF</span><span style="COLOR: black"></span>

<span style="COLOR: #1f497d; mso-themecolor: dark2"></span>

 

<span style="COLOR: black">The first type of image object shown above is
the JPEG I inserted, as shown in
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">MS-ODRAW
section 2.2.22 OfficeArtBStoreContainerFileBlock:
</span>**</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black"><span></span></span>

**<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">Value           
Meaning</span>**

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF007           OfficeArtFBSE
record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF018
– 0xF117  OfficeArtBlip
record</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: black"> </span>

<span style="COLOR: black">I am only dealing with
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">OfficeArtBlip</span>**
records in my example, so MS-ODRAW section 2.2.23 OfficeArtBlip (note
color coded hex dump
above):</span>

**<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">Value  Meaning</span>**

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01A
OfficeArtBlipEMF
 record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: yellow; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01B</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"> **OfficeArtBlipWMF**
 record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01C OfficeArtBlipPICT
record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: yellow; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01D</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">
**OfficeArtBlipJPEG**
record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01E OfficeArtBlipPNG
 record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF01F OfficeArtBlipDIB
 record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF029 OfficeArtBlipTIFF
record</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;">0xF02A
OfficeArtBlipJPEG
record</span><span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: black"> </span>

<span style="COLOR: black">The first record is the
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">OfficeArtBlipJPEG</span>**,
MS-ODRAW section 2.2.27
OfficeArtBlipJPEG:</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt">rh.recVer      MUST
be <span style="BACKGROUND: lime">0x0</span>.
</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">rh.recInstance <span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: lime; COLOR: black; FONT-SIZE: 10pt; mso-highlight: lime">Specified
in the following table</span>.
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">rh.recType     MUST
be
**<span style="FONT-FAMILY: &#39;Courier New&#39;">0x<span style="BACKGROUND: yellow">F01D</span></span>**.
</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

**<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: silver; COLOR: black; FONT-SIZE: 10pt">rh.recLen</span>**<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">      An
unsigned integer that specifies the number of bytes following the
header.</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">MUST
be the size of BLIPFileData plus 17 bytes if recInstance is 0x46A or
0x6E2 or the size of BLIPFileData plus 33 bytes if recInstance is 0x46B
or
0x6E3.</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="COLOR: black"> </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">0x46A   
JPEG in RGB color
space</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; BACKGROUND: lime; COLOR: black; FONT-SIZE: 10pt">0x46B</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">   
JPEG in RGB color
space</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">0x6E2   
JPEG in CMYK color
space</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">0x6E3   
JPEG in CMYK color
space</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="COLOR: black"> </span>

<span style="COLOR: black">Since
<span style="BACKGROUND: silver">0x3AE</span> =
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">942
bytes</span>** you would read the header record and note the type of the
record (0x<span style="BACKGROUND: yellow">F01D</span>) and skip forward
942 bytes to the next 8 byte header record and repeat while you had
records to read in the Pictures Stream.  In this example, I have only
two records, and would read the next record as
0x<span style="BACKGROUND: yellow">F01B</span> OfficeArtBlipWMF, which
is length of 0x<span style="BACKGROUND: silver">6D38</span> = 27,960
bytes.</span>

<span style="COLOR: black"> </span>

<span style="COLOR: black">The following sample code snippet
demonstrates how to delve the stream structures with IStorage.  You may
use this as a simple starting point in your investigation of the Office
binary file structures.  (Note, of course, you will want to implement
with error handling and other coding best practices.  This is only a
small snippet I used in this example to investigate the stream
structures with
IStorage)</span>

<span style="COLOR: black"> </span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">int</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
main(</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">int</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
argc,
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">char</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">\*
argv\[\])</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">{</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
HRESULT
hr;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
IStorage \*pStg = NULL;
</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
CoInitialize(NULL);</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt"> </span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
hr =
StgOpenStorage(</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
L</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: #a31515; FONT-SIZE: 10pt">"\<path
to
file\>\\\\powerpoint-file.ppt”</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">,</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
NULL,STGM\_READ |
STGM\_SHARE\_EXCLUSIVE,NULL,NULL,\&pStg);</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt"> </span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">if</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
(\!FAILED(hr))</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
{</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
IEnumSTATSTG \*pEnumStat =
NULL;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
pStg-\>EnumElements(NULL,NULL,NULL,\&pEnumStat);</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
DWORD
dwFetched;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
STATSTG
stat;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt"> </span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">while</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
(pEnumStat-\>Next(1,\&stat,\&dwFetched) ==
S\_OK)</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
{</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
     
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">if</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
(\!wcscmp(L</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: #a31515; FONT-SIZE: 10pt">"**Pictures**"</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">,<span style="BACKGROUND: yellow">stat</span>.pwcsName))</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                 
{</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                       
IStream \*pStm =
NULL;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                       
hr =
pStg-\>OpenStream(</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                             
<span style="BACKGROUND: yellow">stat</span>.pwcsName,</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                             
NULL,STGM\_READ |
STGM\_SHARE\_EXCLUSIVE,NULL,\&pStm);</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt"> </span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                       
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: green; FONT-SIZE: 10pt">//
add processing
here</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: green; FONT-SIZE: 10pt"> </span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                       
pStm-\>Release();</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">                 
}</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
}</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
pEnumStat-\>Release();</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">           
pStg-\>Release();</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
}</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
CoUninitialize();</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">     
</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: blue; FONT-SIZE: 10pt">return</span><span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">
0;</span><span style="COLOR: black"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">}</span><span style="COLOR: black"></span>

<span style="COLOR: black"> </span>

<span style="COLOR: black">Note the details of the
**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; BACKGROUND: yellow">stat</span>**
structure in the code snippet
above:</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">Stat             
{pwcsName=0x006d3708 "Pictures" type=2 cbSize={...} ...}
tagSTATSTG</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">pwcsName         
0x006d3708
"**<span style="FONT-FAMILY: &#39;Courier New&#39;">Pictures</span>**"               wchar\_t
\*</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">type             
2                                   unsigned
long</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">cbSize            {**<span style="FONT-FAMILY: &#39;Courier New&#39;">28918</span>**}                             \_ULARGE\_INTEGER</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">mtime            
{dwLowDateTime=0 dwHighDateTime=0
} \_FILETIME</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">ctime            
{dwLowDateTime=0 dwHighDateTime=0
} \_FILETIME</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">atime            
{dwLowDateTime=0 dwHighDateTime=0
} \_FILETIME</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">grfMode           0                                   unsigned
long</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">grfLocksSupported 0                                  
unsigned
long</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">clsid            
{GUID\_NULL}                         \_GUID</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">grfStateBits      0                                  
unsigned
long</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; COLOR: black; FONT-SIZE: 10pt">reserved         
0                                   unsigned
long</span><span style="COLOR: black; FONT-SIZE: 10pt"></span>

<span style="COLOR: black; FONT-SIZE: 10pt"></span>

 

<span style="COLOR: black">To expand on the preceding code, since you
have the length of each picture and the image type, you could read the
bytes into a byte array and write them out to disk, or whatever your
requirements may entail.</span>

<span style="COLOR: black"></span>

 

<span style="COLOR: black">I hope this helps shed some light on parsing
Office binary files in general and how you might approach parsing
PowerPoint binary files for pictures.</span>

</div>

</div>
