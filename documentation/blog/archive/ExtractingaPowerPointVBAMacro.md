<div id="page">

# Extracting a PowerPoint VBA Macro

[Tom Jebo](https://social.msdn.microsoft.com/profile/Tom%20Jebo)
6/20/2013 3:42:00 PM

-----

<div id="content">

## Abstract

This post of my blog responds to a request by a customer to find and
extract a VBA macro in a PowerPoint file, specifically one stored in the
older binary file format, also known to many as BFF.

## Introduction

This post will take you through steps outlined in [\[MS-PPT\]
“PowerPoint (.ppt) Binary File
Format”](http://msdn.microsoft.com/en-us/library/cc313106\(v=office.12\).aspx)
section 2.1.2 “PowerPoint Document Stream”. In this section is an
algorithm for finding various important structures and objects in the
PowerPoint format. Because I only needed the VbaProjectStgCompressedAtom
object, specified in 2.10.42, which contains the storage for the VBA
macro, I didn’t need to go through the entire algorithm. Instead, the
approach I used was to look for the part of the algorithm which most
directly leads to the object (or its immediate parent), and then work
backward in the steps. This is not actually that hard to do, but care
must when  
moving backward in the algorithm not to miss important prerequisite
structures.

Because of this, it is also important for me to note that taking this
approach is not a substitute for reading the specification and
understanding the overall structure of a PowerPoint document format.

Also, what you see here is an example process based on one PowerPoint
document that was created specifically for this exercise.

The document used in this example was created in PowerPoint 2013 by
adding a VBA macro, placing password protection on it, and then saving
it as PowerPoint 97-2003 format (i.e. BFF). Below are screen shots for
this first
part.

 

[![](images/8156.1.png)

*Figure 1.0*

 

 

The Developer tab you see in Figure 1.0, can be accessed after going to
File | Options…, selecting Customize Ribbon and checking the Developer
box on the right like in Figure
1.1.

 

*[![](images/8284.2.png)*

*Figure 1.1*

 

Once you have the Developer tab, click the Visual Basic button and you
will be able to add code like in Figure
1.2.

 

[![](images/0131.3.png)

*Figure 1.2*

 

Then, to password protect this sample macro, right click on “Module 1”
in the left hand tree view and select VBA Project Properties… In the
resulting dialog you will be able to enable protection and set the
password like in Figure
1.3.

 

[![](images/6153.4.png)

*Figure 1.3*

 

Now exit out, saving the VBA macro. Then save the file as PowerPoint
97-2003 and you have a similar sample file to what I’m using for the
rest of this blog. Now the fun begins while we follow the steps in
\[MS-PPT\]\!

As in my other blogs on binary formats like this one, I use
[SweetScape’s 010 editor](http://sweetscape.com/) to display the data
in  structures along the way. I mention this because in my previous
posts, I’ve been asked what I used. [\[MS-PPT\] “PowerPoint (.ppt)
Binary File
Format”](http://msdn.microsoft.com/en-us/library/cc313106\(v=office.12\).aspx)
is hyperlinked here for reference but I will not include links for all
the sections as I walk through the steps below.

From here down to \[5. Read the VbaProjectStgrecord…\] I will merely
list the steps in section 2.1.2 followed by the resulting data found at
required locations. The steps and the highlighting speak for
themselves.  The excerpts from \[MS-PPT\] will be in *italics*. My
story line is in normal font and the hex output will be in
<span style="font-family: Courier New;">courier font</span>.  I’ve also
tried to <span style="color: #00ff00;">highlight using color</span> to
make it easier to match fields with my comments and calculations.

 

## \[MS-PPT\] 2.1.2 PowerPoint Document Stream

######  

 

*Part 1: Construct the persist object directory.*

*1. Read the CurrentUserAtomrecord (section 2.3.2) from the Current User
Stream(section* *2.1.1). All seek operations in the steps that follow
this step are in the PowerPoint Document*
*Stream.*

 

[![](images/5822.5.png)

 

<span style="font-family: Courier New;">1000h: 00 00 F6 0F 20 00 00 00
14 00 00 00 5F C0 91 E3  ..ö. .......\_À‘ã </span>

<span style="font-family: Courier New;">1010h:
<span style="color: #00ff00;">**1A 99 00 00**</span> 08 00 F4 03 03 00
62 1C 54 6F 6D 20  .™....ô...b.Tom   
</span>

<span style="font-family: Courier New;">1020h: 4A 65 62 6F 08 00 00 00
54 00 6F 00 6D 00 20 00  Jebo....T.o.m. . </span>

<span style="font-family: Courier New;">1030h: 4A 00 65 00 62 00 6F
00                          J.e.b.o.</span>

 

*2. Seek, in the PowerPoint Document Stream, to the offset specified by
the* *offsetToCurrentEditfield of the CurrentUserAtomrecord identified
in step 1.*

 

 

CurrentUserAtomrecord.offsetToCurrentEdit
=<span style="color: #00ff00;"> **1A 99 00 00**</span>

 

*3. Read the UserEditAtomrecord at the current offset. Let this record
be a live record.*

 

0x1200 + 0x991A = 0xAB1A

 

<span style="font-family: Courier New;">AB10h: 00 00 F3 8D 00 00 D3 8A
00 00 <span style="color: #00ff00;">**00 00 F5 0F 1C 00**</span>  ..ó
</span>

<span style="font-family: Courier New;"><span style="font-family: Courier New;">AB20h:
<span style="color: #00ff00;">**00 00 00 00 00 00 81 11 00 03 00 00 00
00 FE 98**</span>  ......</span></span>

<span style="font-family: Courier New;">AB30h:
<span style="color: #00ff00;">**00 00 <span style="color: #ff80ff;">01
00 00 00</span>**</span> 04 00 00 00 01 00 62 1C 00 00 
............b...</span>

 

*Part 2: Identify the document persist object.*

*1. Read the docPersistIdReffield of the UserEditAtomrecord first
identified in step 3 of Part 1,* *that is, the UserEditAtomrecord
closest to the end of the stream.*

  

docPersistIdReffield =
<span style="color: #ff80ff;">**0x00000001**</span>

 

*2. Lookup the value of the docPersistIdReffield in the persist object
directory constructed in step* *8 of Part 1 to find the stream offset of
a persist object.*

  

PersistDirectoryAtomrecord @ 0xAAFE

offsetLastEdit is 0x00000000

 

2.3.5 PersistDirectoryEntry bytes:

 

<span style="font-family: Courier New;">AAF0h: 83 87 97 5F 7A 7B 23 3A
FC 07 00 00 FF FF <span style="color: #00ff00;">**00 00**</span> 
ƒ‡—\_z{\#:ü...ÿÿ.. </span>

<span style="font-family: Courier New;">AB00h:
<span style="color: #00ff00;">**72 17 14 00 00 00 01 00 40 00
<span style="color: #ff0000;">00 00 00 00</span> F6 0B**
</span></span>[<span style="font-family: Courier New;">r.......@.....ö</span>](mailto:r.......@.....ö)<span style="font-family: Courier New;">.  
</span>

<span style="font-family: Courier New;">AB10h:
<span style="color: #00ff00;">**00 00 F3 8D 00 00 D3 8A 00 00**</span>
00 00 F5 0F 1C 00  ..ó</span> 

 

PersistOffsetEntry = <span style="color: #ff0000;">**00 00 00
00**</span>

 

 

*3. Seek to the stream offset specified in step 2.*

*4. Read the*
<span style="color: #0080c0;">**DocumentContainerrecord**</span> *at the
current offset. Let this record be a live record.*

 

<span style="font-family: Courier New;">1200h:
<span style="color: #0080c0;">**0F 00 E8 03**</span> EE 0B 00 00 01 00
E9 03 28 00 00 00  ..è.î.....é.(...  
</span>

<span style="font-family: Courier New;">1210h: 00 1E 00 00 E0 10 00 00
E0 10 00 00 80 16 00 00  ....à...à...€... </span>

<span style="font-family: Courier New;">1220h: 05 00 00 00 0A 00 00 00
00 00 00 00 00 00 00 00  ................</span>

 

*Part 11: Identify the VBA project persist object.*

*1. Read the DocInfoListContainerrecord (section 2.4.4), if present,
specified by the docInfoList* *field of the DocumentContainerrecord
identified in step 4 of Part 2. If not present, skip to step* *6.*

  

<span style="color: #ff8000;">**docInfoList**</span> @0x148C

 

<span style="font-family: Courier New;">1480h: 00 00 00 00 00 00 00 80
00 00 00 00 <span style="color: #ff8000;">**0F 00 D0 07**</span> 
.......€......Ð. </span>

<span style="font-family: Courier New;">1490h: 44 01 00 00 1F 00 14 04
1C 00 00 00 00 00 15 04  D............... </span>

<span style="font-family: Courier New;">14A0h: 14 00 00 00 85 D1 F0 08
00 CA 9A 3B AD 07 94 C7  ....…Ñð..Êš;­.”Ç </span>

<span style="font-family: Courier New;">14B0h: 00 CA 9A 3B 01 02 00 00
1F 00 13 04 3C 00 00 00  .Êš;........\<... </span>

<span style="font-family: Courier New;">14C0h: 00 00 FD 03 34 00 00 00
01 00 00 00 01 00 00 00  ..ý.4........... </span>

<span style="font-family: Courier New;">14D0h: 01 00 00 00 01 00 00 00
54 B5 C6 00 DC AF C6 00  ........TµÆ.Ü¯Æ. </span>

<span style="font-family: Courier New;">14E0h: 08 00 00 00 18 B0 C6 00
68 B2 C6 00 1C B2 C6 00  .....°Æ.h²Æ..²Æ. </span>

<span style="font-family: Courier New;">14F0h: 00 00 00 00 00 00 00 00
00 01 C6 00 0F 00 FA 03  ..........Æ...ú. </span>

<span style="font-family: Courier New;">1500h: 47 00 00 00 00 00 FE 03
03 00 00 00 00 00 00 00  G.....þ......... </span>

<span style="font-family: Courier New;">1510h: 00 FD 03 34 00 00 00 72
00 00 00 64 00 00 00 72  .ý.4...r...d...r </span>

<span style="font-family: Courier New;">1520h: 00 00 00 64 00 00 00 3C
AF C6 00 08 00 00 00 30  ...d...\<¯Æ.....0 </span>

<span style="font-family: Courier New;">1530h: B0 C6 00 03 00 00 00 7C
B2 C6 00 4C AF C6 00 86  °Æ.....|²Æ.L¯Æ.† </span>

<span style="font-family: Courier New;">1540h: 01 00 00 66 00 00 00 01
00 C6 00 <span style="color: #00ff00;">**1F 00 FF 03 14**</span> 
...f.....Æ...ÿ..  
</span>

<span style="font-family: Courier New;">1550h:
<span style="color: #00ff00;">**00 00 00 02 00 00 04 0C 00 00 00 03 00
00 00 01**</span>  ................ </span>

<span style="font-family: Courier New;">1560h:
<span style="color: #00ff00;">**00 00 00 02 00 00 00 0F 00 88 13 69 00
00 00 0F**</span>  .........ˆ.i.... </span>

 

 *2. Read the*
<span style="color: #00ff00;">**VBAInfoContainer**</span>*(section
2.4.10) child record, if present, of the DocInfoListContainerrecord
identified in step 1. If no such child record exists, skip to step 6.*

  

<span style="font-family: Courier New;">1540h: 01 00 00 66 00 00 00 01
00 C6 00 <span style="color: #00ff00;">**1F 00 FF 03 14**</span> 
...f.....Æ...ÿ..  
</span>

<span style="font-family: Courier New;">1550h:
<span style="color: #00ff00;">**00 00 00 02 00 00 04 0C 00 00 00 03 00
00 00 01**</span>  ................ </span>

<span style="font-family: Courier New;">1560h:
<span style="color: #00ff00;">**00 00 00 02 00 00 00 0F 00 88 13 69 00
00 00 0F**</span>  .........ˆ.i.... </span>

  

*3. Lookup the value of the vbaInfoAtom.persistIdReffield of the
VBAInfoContainerrecord* *identified in step 2 in the persist object
directory constructed in step 8 of Part 1 to find the* *stream offset of
a persist object.*

  

vbaInfoAtom.persistIdReffield  = <span style="color: #00ff00;">**03 00
00 00**</span> = 3

 

 *4. Seek to the stream offset specified in step 3.*

  

From the persist object directory above:
<span style="color: #00ff00;">**F3 8D 00 00**</span> = offset 0x8DF3

PowerPoint Document stream @ 0x1200

So VbaProjectStorage record is at 0x1200 + 0x8DF3 = 0x9FF3

  

*5. Read the VbaProjectStgrecord at the current offset. Let this record
be a live record.*

 

I copied the VbaProjectStgCompressedAtom bytes from 010 editor into
another file.  This new file, then became a self-contained compound file
by the nature of its internal organization.

 

<span style="font-family: Courier New;">9FF0h: 00 00 00
<span style="color: #00ff00;">**10 00 11 10
<span style="color: #8000ff;">03 0B 00 00</span> 00 22 00 00
78**</span>  ............"..x </span>

<span style="font-family: Courier New;">A000h:
<span style="color: #00ff00;">**01 EC 59 7D 6C 1C C5 15 7F BB 77 B6 CF
97 D8 B9**</span>  .ìY}l.Å.»w¶Ï—Ø¹ </span>

<span style="font-family: Courier New;">A010h:
**<span style="color: #00ff00;">18 13 12 93 C2 C6 06 E3 24 B6 D9 DD 3B
DF 07 B1</span>**  ...“ÂÆ.ã$¶ÙÝ;ß.± </span>

<span style="font-family: Courier New;">A020h:
**<span style="color: #00ff00;">69 6E EF C3 71 64 63 2B 46 71 A5 1E 84
B3 6F 8D</span>**  inïÃqdc+Fq¥.„³o</span>

…

…

…

Size of the compressed storage is: <span style="color: #8000ff;">03 0B
00 00</span> = 0xB03

 

I copied this entire block of bytes into a separate file and
decompressed them to obtain a fully independent compound file.

The decompression algorithm is referenced in 2.10.42
“VbaProjectStgCompressedAtom” and says:

*The original bytes of the storage are compressed by the algorithm
specified in \[RFC1950\] and are decompressed by the algorithm specified
in \[RFC1951\].*

So at this point, you will need to write some code to decompress this
using the [zlib compression](http://zlib.net/) library. There is example
code shown in Figure 1.4 at the end of this blog.

Back to the block of bytes in a separate file; this is indeed a
[\[MS-CFB\]](http://msdn.microsoft.com/en-us/library/dd942138.aspx)
conforming document with the VBA storages as specified in
[\[MS-OVBA\]](http://msdn.microsoft.com/en-us/library/cc313094\(v=office.12\).aspx).
Opening that in 010 or Offvis, you can see the
storages:

 

[![](images/0602.6.png)

 

Now we can look at some of the stream data to convince ourselves that we
have a valid VBA macro storage here. Since it is password protected,
some of the fields are encrypted and look like just random data. But we
can look at, for instance, a stream named “PROJECT” which according to
\[MS-OVBA\]:

  

*2.3.1 PROJECT Stream: Project Information*

*The PROJECT stream (1) specifies properties of the VBA project.*

  

So, looking at the bytes at the beginning of it:

 

<span style="font-family: Courier New;">1AC0h: 49 44 3D 22 7B 30 30 30
30 30 30 30 30 2D 30 30  ID="{00000000-00 </span>

<span style="font-family: Courier New;">1AD0h: 30 30 2D 30 30 30 30 2D
30 30 30 30 2D 30 30 30  00-0000-0000-000 </span>

<span style="font-family: Courier New;">1AE0h: 30 30 30 30 30 30 30 30
30 7D 22 0D 0A 4D 6F 64  000000000}"..Mod </span>

<span style="font-family: Courier New;">1AF0h: 75 6C 65 3D 4D 6F 64 75
6C 65 31 0D 0A 48 65 6C  ule=Module1..Hel </span>

<span style="font-family: Courier New;">1B00h: 70 46 69 6C 65 3D 22 22
0D 0A 4E 61 6D 65 3D 22  pFile=""..Name=" </span>

<span style="font-family: Courier New;">1B10h: 56 42 41 50 72 6F 6A 65
63 74 22 0D 0A 48 65 6C  VBAProject"..Hel </span>

<span style="font-family: Courier New;">1B20h: 70 43 6F 6E 74 65 78 74
49 44 3D 22 30 22 0D 0A  pContextID="0".. </span>

<span style="font-family: Courier New;">1B30h: 56 65 72 73 69 6F 6E 43
6F 6D 70 61 74 69 62 6C  VersionCompatibl </span>

<span style="font-family: Courier New;">1B40h: 65 33 32 3D 22 33 39 33
32 32 32 30 30 30 22 0D  e32="393222000". </span>

<span style="font-family: Courier New;">1B50h: 0A 43 4D 47 3D 22 45 42
45 39 34 37 37 45 33 36  .CMG="EBE9477E36 </span>

<span style="font-family: Courier New;">1B60h: 38 32 33 36 38 32 33 32
38 36 33 32 38 36 22 0D  82368232863286". </span>

<span style="font-family: Courier New;">1B70h: 0A 44 50 42 3D 22 44 36
44 34 37 41 36 42 38 45  .DPB="D6D47A6B8E </span>

<span style="font-family: Courier New;">1B80h: 42 44 36 34 44 41 36 34
44 41 39 42 32 36 36 35  BD64DA64DA9B2665 </span>

<span style="font-family: Courier New;">1B90h: 44 41 31 37 43 37 39 36
32 31 44 46 44 31 30 37  DA17C79621DFD107 </span>

<span style="font-family: Courier New;">1BA0h: 31 31 46 39 39 33 36 31
44 30 39 32 44 43 39 31  11F99361D092DC91 </span>

<span style="font-family: Courier New;">1BB0h: 45 46 41 42 37 36 34 34
43 37 35 42 33 41 38 30  EFAB7644C75B3A80 </span>

<span style="font-family: Courier New;">1BC0h: 35 39 36 42 22 0D 0A 47
43 3D 22 43 31 43 33 36  596B"..GC="C1C36 </span>

<span style="font-family: Courier New;">1BD0h: 44 36 45 36 45 36 45 36
45 36 45 22 0D 0A 0D 0A  D6E6E6E6E6E".... </span>

<span style="font-family: Courier New;">1BE0h: 5B 48 6F 73 74 20 45 78
74 65 6E 64 65 72 20 49  \[Host Extender I </span>

<span style="font-family: Courier New;">1BF0h: 6E 66 6F 5D 0D 0A 26 48
30 30 30 30 30 30 30 31  nfo\]..\&H00000001 </span>

<span style="font-family: Courier New;">1C00h: 3D 7B 33 38 33 32 44 36
34 30 2D 43 46 39 30 2D  ={3832D640-CF90- </span>

<span style="font-family: Courier New;">1C10h: 31 31 43 46 2D 38 45 34
33 2D 30 30 41 30 43 39  11CF-8E43-00A0C9 </span>

<span style="font-family: Courier New;">1C20h: 31 31 30 30 35 41 7D 3B
56 42 45 3B 26 48 30 30  11005A};VBE;\&H00 </span>

<span style="font-family: Courier New;">1C30h: 30 30 30 30 30 30 0D 0A
0D 0A 5B 57 6F 72 6B 73  000000....\[Works </span>

<span style="font-family: Courier New;">1C40h: 70 61 63 65 5D 0D 0A 4D
6F 64 75 6C 65 31 3D 32  pace\]..Module1=2 </span>

<span style="font-family: Courier New;">1C50h: 36 2C 20 32 36 2C 20 31
34 32 38 2C 20 36 37 30  6, 26, 1428, 670 </span>

<span style="font-family: Courier New;">1C60h: 2C 20 0D
0A                                      , ..</span>

 

From this, we see properties like Name="VBAProject” and Module=Module1
which gives us some confidence that we have indeed the properties for a
VBA macro. For questions or comments about the contents of this blog,
please contact [Interoperability Documentation
Help](mailto:dochelp@microsoft.com).

 

*Figure 1.4 Example code to decompress the
VbaProjectStgCompressedAtom*

<span style="font-family: Courier New;">'===================================================================  
</span>

<span style="font-family: Courier New;">' DISCLAIMER:
</span>

<span style="font-family: Courier New;">'-------------------------------------------------------------------  
</span>

<span style="font-family: Courier New;">' </span>

<span style="font-family: Courier New;">' This sample is provided as is
and is not meant for use on a </span>

<span style="font-family: Courier New;">' production environment. It is
provided only for illustrative </span>

<span style="font-family: Courier New;">' purposes. The end user must
test and modify the sample to suit </span>

<span style="font-family: Courier New;">' their target environment.
</span><span style="font-family: Courier New;">' </span>

<span style="font-family: Courier New;">' Microsoft can make no
representation concerning the content of </span>

<span style="font-family: Courier New;">' this sample. Microsoft is
providing this information only as a </span>

<span style="font-family: Courier New;">' convenience to you. This is to
inform you that Microsoft has not </span>

<span style="font-family: Courier New;">' tested the sample and
therefore cannot make any representations </span>

<span style="font-family: Courier New;">' regarding the quality, safety,
or suitability of any code or </span>

<span style="font-family: Courier New;">' information found here.
</span>

<span style="font-family: Courier New;">'
</span>

<span style="font-family: Courier New;">'===================================================================</span>

<span style="font-family: Courier New;">// assumptions for the input
file: </span>

<span style="font-family: Courier New;">// 1. it has nothing in it but
the a) 4 byte uncompressed length and b) compressed data</span>

<span style="font-family: Courier New;">// 2. it is small enough to read
in and process in a reasonable buffer and processing time</span>

<span style="font-family: Courier New;">// </span>

<span style="font-family: Courier New;">int main(int argc, char
\*argv\[\])</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">int
BlockSizeUncompress=0x1000;</span>

<span style="font-family: Courier New;">int
cprLevel=Z\_BEST\_COMPRESSION;</span>

<span style="font-family: Courier New;">FILE\* streamIn = NULL;</span>

<span style="font-family: Courier New;">FILE\* streamOut = NULL;</span>

<span style="font-family: Courier New;">enum</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">paramExe,</span>

<span style="font-family: Courier New;">paramFileIn,</span>

<span style="font-family: Courier New;">paramFileOut,</span>

<span style="font-family: Courier New;">paramBlockSizeUncompress,</span>

<span style="font-family: Courier New;">paramCompressLevel</span>

<span style="font-family: Courier New;">};</span>

<span style="font-family: Courier New;">if (argc\<paramFileOut)</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">printf("run decompress
\<FileIn\> \<FileOut\> \[BlockSizeUncompress\] \[compres.
level\]\\n"</span>

<span style="font-family: Courier New;">"For example: decompress
inputfile.bin uncompressed.data.bin");</span>

<span style="font-family: Courier New;">return 0;</span>

<span style="font-family: Courier New;">}</span>

<span style="font-family: Courier New;">if
(argc\>paramBlockSizeUncompress)</span>

<span style="font-family: Courier New;">BlockSizeUncompress=atol(argv\[paramBlockSizeUncompress\]);</span>

<span style="font-family: Courier New;">if
(argc\>paramCompressLevel)</span>

<span style="font-family: Courier New;">cprLevel=(int)atol(argv\[paramCompressLevel\]);</span>

<span style="font-family: Courier New;">byte\* pbOriginal; </span>

<span style="font-family: Courier New;">byte\* pbCompressed; </span>

<span style="font-family: Courier New;">// open the input file for
reading binary
data</span>

<span style="font-family: Courier New;">streamIn=fopen(argv\[paramFileIn\],
"rb");</span>

<span style="font-family: Courier New;">if (streamIn==NULL)</span>

<span style="font-family: Courier New;">return 0;</span>

<span style="font-family: Courier New;">// open the output file for
writing binary
data</span>

<span style="font-family: Courier New;">streamOut=fopen(argv\[paramFileOut\],
"wb");</span>

<span style="font-family: Courier New;">if (streamOut==NULL)</span>

<span style="font-family: Courier New;">return 0;</span>

<span style="font-family: Courier New;">// Beginning of main
decompression functionality</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">z\_stream zcpr;</span>

<span style="font-family: Courier New;">int ret=Z\_OK;</span>

<span style="font-family: Courier New;">long lOrigDone = 0;</span>

<span style="font-family: Courier New;">int
step=0;</span>

<span style="font-family: Courier New;">memset(\&zcpr,0,sizeof(z\_stream));</span>

<span style="font-family: Courier New;">inflateInit(\&zcpr);</span>

<span style="font-family: Courier New;">struct \_stat buf;</span>

<span style="font-family: Courier New;">// find out how big the input
file is</span>

<span style="font-family: Courier New;">if (\_stat(argv\[paramFileIn\],
\&buf))</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">printf("failed to read file
size");</span>

<span style="font-family: Courier New;">return -4;</span>

<span style="font-family: Courier New;">}</span>

<span style="font-family: Courier New;">// allocate enough bytes for the
whole file.  
</span>

<span style="font-family: Courier New;">pbCompressed = (byte\*)  
malloc(buf.st\_size);</span>

<span style="font-family: Courier New;">size\_t corig = 0; </span>

<span style="font-family: Courier New;">long lorig = 0; </span>

<span style="font-family: Courier New;">// read in the first 4 bytes
which is the size of the uncompressed</span>

<span style="font-family: Courier New;">// data</span>

<span style="font-family: Courier New;">if (fread(\&lorig, 1, 4,
streamIn) == 0)</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">printf("failed to read
uncompressed size");</span>

<span style="font-family: Courier New;">return -4;</span>

<span style="font-family: Courier New;">}</span>

<span style="font-family: Courier New;">corig = lorig;</span>

<span style="font-family: Courier New;">pbOriginal = (byte\*)
malloc(corig); </span>

<span style="font-family: Courier New;">ZeroMemory(pbOriginal,
sizeof(pbOriginal));</span>

<span style="font-family: Courier New;">// read in the compressed data
from the input file</span>

<span style="font-family: Courier New;">if (fread(pbCompressed, 1, 4096,
streamIn) == 0)</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">printf("failed to read
compressed data");</span>

<span style="font-family: Courier New;">return -4;</span>

<span style="font-family: Courier New;">}</span>

<span style="font-family: Courier New;">zcpr.next\_in =
pbCompressed;</span>

<span style="font-family: Courier New;">zcpr.next\_out =
pbOriginal;</span>

<span style="font-family: Courier New;">zcpr.avail\_in =
buf.st\_size;</span>

<span style="font-family: Courier New;">zcpr.avail\_out = lorig;</span>

<span style="font-family: Courier New;">// call zlib to uncompress or
inflate the
data</span>

<span style="font-family: Courier New;">do</span>

<span style="font-family: Courier New;">{</span>

<span style="font-family: Courier New;">ret=inflate(\&zcpr,Z\_SYNC\_FLUSH);</span>

<span style="font-family: Courier New;">step++;</span>

<span style="font-family: Courier New;">} while (ret==Z\_OK);</span>

<span style="font-family: Courier New;">// write out the uncompressed
data to the output file.</span>

<span style="font-family: Courier New;">fwrite(pbOriginal, 1, lorig,
streamOut);</span>

<span style="font-family: Courier New;">inflateEnd(\&zcpr);</span>

<span style="font-family: Courier New;">}</span>

<span style="font-family: Courier New;">// clean up buffers and file
pointers</span>

<span style="font-family: Courier New;">free(pbCompressed);</span>

<span style="font-family: Courier New;">free(pbOriginal);</span>

<span style="font-family: Courier New;">fclose(streamIn);</span>

<span style="font-family: Courier New;">fclose(streamOut);</span>

<span style="font-family: Courier New;">return 0;</span>

<span style="font-family: Courier New;">}</span>

</div>

</div>
