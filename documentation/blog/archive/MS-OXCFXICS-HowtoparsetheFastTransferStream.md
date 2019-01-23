<div id="page">

# MS-OXCFXICS - How to parse the FastTransfer Stream

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 9/16/2015
11:59:00 AM

-----

<div id="content">

<span style="text-decoration: underline;">**Note:**</span> This article
was written using version 16.2 (10/30/2014) of the
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
document as reference and all links contained in this article reference
sections of that version of the document. The current version of the
MS-OXCFXICS document can be found here:
[https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx](https://msdn.microsoft.com/en-us/library/cc463916\(v=exchg.80\).aspx "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")  
  
  
<span style="font-size: large;">Resources:</span>

  - [\[MS-OXCFXICS\]](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/%5BMS-OXCFXICS%5D "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
    - Bulk Data Transfer
    Protocol

  - [\[MS-OXPROPS\]](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/%5BMS-OXPROPS%5D "https://msdn.microsoft.com/en-us/library/cc433490(v=exchg.80).aspx")
    - Exchange Server Protocols Master Property
    List

  - [\[MS-OXCDATA\]](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/%5BMS-OXCDATA%5D "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
    - Data Structures

  
<span style="font-size: large;">Introduction:</span>  
The [FastTransfer
Stream](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/FastTransfer%20Stream "https://msdn.microsoft.com/en-us/library/ee160399(v=exchg.80).aspx")
as described in
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
section
[2.2.4](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.4 "https://msdn.microsoft.com/en-us/library/ee160399(v=exchg.80).aspx")
and its related subsections can be an intimidating adversary if you are
trying to implement a mail client or server that uses RPC as its primary
method of communication (as opposed to ActiveSync or Exchange Web
Services). The important thing to understand is that it is nothing more
than a serialized collection of markers, properties, and values.  
  
  
<span style="font-size: large;">Sample Data:</span>  
The following sample data is the first part of a much larger stream of
bytes. However, for the purposes of this article, it contains enough
different property types to give you a good idea of how it all works.  
  
<span style="font-family: &#39;courier new&#39;, courier;">0x0000: | 03
00 12 40 02 01 E1 65 00 00 00 00 02 01 E0 65 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0010: | 16
00 00 00 52 F6 85 EC 7D 43 2E 4A A9 60 34 50 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0020: | 88
53 D9 0A 00 00 00 00 03 F5 40 00 08 30 00 D0 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0030: | 9E
A8 81 85 D0 01 02 01 E2 65 16 00 00 00 52 F6 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0040: | 85
EC 7D 43 2E 4A A9 60 34 50 88 53 D9 0A 00 00 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0050: | 00
00 20 7C 02 01 E3 65 17 00 00 00 16 52 F6 85 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0060: | EC
7D 43 2E 4A A9 60 34 50 88 53 D9 0A 00 00 00 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0070: | 00
20 7C 1F 00 01 30 0C 00 00 00 49 00 4E 00 42 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0080: | 00
4F 00 58 00 00 00 14 00 49 67 01 00 00 00 00 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0090: | 00
03 F4 03 00 39 66 FB 03 00 00 40 00 07 30 80 </span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x00A0: | 39
06 A8 81 85 D0 01 0B 00 F4 10 00 00</span>  
  
  
<span style="font-size: large;">Breaking it Down:</span>  
The Lexical Structure of the FastTransfer Stream might look complicated
at first but we can summarize it as follows:  
  

The stream consists of elements. 

Each element is either a marker or a propValue. 

1.  If the element matches an entry from the table in section
    [2.2.4.1.4](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.4.1.4 "https://msdn.microsoft.com/en-us/library/ee220023(v=exchg.80).aspx"),
    it’s a Marker. If not, it's a propValue.

propValues are either fixed or variable length.

The first 2 bytes of the propValue represent the data type. Reference
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.11.1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.11.1 "https://msdn.microsoft.com/en-us/library/ee157583(v=exchg.80).aspx")
to find out what type it is and whether it's fixed or variable length.

1.  If it's fixed length, read the next x number of bytes as indicated
    by the type.
2.  If it's variable length, follow the description provided to
    correctly read the value.

  
  
<span style="font-size: large;">Markers:</span>  
Markers are the easiest to understand and come in 2 types. They are
either stand-alone, or consist of a pair of start and end markers. They
are nothing more than a 4-byte value that tells you something about the
properties that come after it in the stream (unless it's an end marker).
Taking a look at our example data stream from above let's look at the
first element and try to determine if it's a marker or a
propValue.  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[0\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0000: |
<span style="background-color: #00ff00;">03 00 12 40</span> 02 01 E1 65
00 00 00 00 02 01 E0 65</span>  
  
The value of the first element is **0x40120003** (remember that int32
type values are transmitted in little-endian format). From looking at
the table in
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.2.4.1.4](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.4.1.4 "https://msdn.microsoft.com/en-us/library/ee220023(v=exchg.80).aspx")
we can see that this represents the **IncrSyncChange** Marker which
*"signifies the start of ICS information pertaining to the message."*
That's all there is to it. Moving on…  
  
  
<span style="font-size: large;">propValues:</span>  
If the element isn't a Marker (the element value is NOT in the table in
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
section
[2.2.4.1.4](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.4.1.4 "https://msdn.microsoft.com/en-us/library/ee220023(v=exchg.80).aspx")),
it's a propValue. There are 2 types of propValues, fixed and variable
length. The table in
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.11.1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.11.1 "https://msdn.microsoft.com/en-us/library/ee157583(v=exchg.80).aspx")
contains all of the possible types. The list is fairly long so I will
only talk about the ones that appear in our sample data. Let's take a
look at the first
one…  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[1\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0000: | 03
00 12 40 <span style="background-color: #00ff00;">02 01 E1 65</span>
<span style="background-color: #00ccff;">00 00 00 00</span> 02 01 E0
65</span>  
  
The element value is **0x65E10102**, which is not a Marker (check the
table\!), which means it's a propValue. There are 2 things that you need
to do with this. First, look up the type in
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.11.1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.11.1 "https://msdn.microsoft.com/en-us/library/ee157583(v=exchg.80).aspx").
In this case, the type 0x0102 means that the property is a
**PtypBinary**. According to the description, this type is "Variable
size; a COUNT field followed by that many bytes." The COUNT field is 4
bytes in length and should be read as an integer, which in this case is
the value **0x00000000**, or simply 0. This also means that this is the
end of this propType. Because the COUNT value is 0, no bytes follow it
that belong to this property. But what does the property represent?
That's the second thing to do, look up the Property ID in
[MS-OXPROPS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXPROPS "https://msdn.microsoft.com/en-us/library/cc433490(v=exchg.80).aspx").
In this case, the Property ID 0x65E1 belongs to the
**PidTagParentSourceKey** property (section
[2.850](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.850 "https://msdn.microsoft.com/en-us/library/ee159636(v=exchg.80).aspx")).  
  
Taking a look at the next
element…  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[2\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0000: | 03
00 12 40 02 01 E1 65 00 00 00 00
<span style="background-color: #00ff00;">02 01 E0 65</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0010: |
<span style="background-color: #00ccff;">16 00 00 00</span>
<span style="background-color: #ffff00;">52 F6 85 EC 7D 43 2E 4A A9 60
34 50</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0020: |
<span style="background-color: #ffff00;">88 53 D9 0A 00 00 00 00 03
F5</span> 40 00 08 30 00 D0</span>  
  
You can probably tell by how many bytes I've highlighted above that
there is definitely more going on with this next one. Following the same
process as before, get the element (0x65E00201) which is a propValue.
This is also a PtypBinary type so you know that the next thing you have
to do is get the COUNT, which in this case is 0x00000016, or 22 in
decimal format. This tells us that the next 22 bytes should be read as a
byte array. That's easy, but what does that data actually represent?
Going into that much depth with each property would be beyond the scope
of this article, but here's a hint: Look up the Property ID (0x65E0) in
[MS-OXPROPS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXPROPS "https://msdn.microsoft.com/en-us/library/cc433490(v=exchg.80).aspx")
(PidTagSourceKey) and then check the defining reference
([MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
section
[2.2.1.2.5](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.1.2.5 "https://msdn.microsoft.com/en-us/library/ee220110(v=exchg.80).aspx")).  
  
Let's parse a few more
propValues…  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[3\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0020: | 88
53 D9 0A 00 00 00 00 03 F5 <span style="background-color: #00ff00;">40
00 08 30</span> <span style="background-color: #ffff00;">00
D0</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0030: |
<span style="background-color: #ffff00;">9E A8 81 85 D0 01</span> 02 01
E2 65 16 00 00 00 52 F6</span>  
  
propValue: 0x30080040  
Property type: 0x0040 - PtypTime  
Property ID: 0x3008 - PidTagLastModificationTime  
Value: 05/03/2015 09:15:12  
<span style="text-decoration: underline;">**Note:**</span> See
[MS-DTYP](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-DTYP "https://msdn.microsoft.com/en-us/library/cc230273.aspx")
section
[2.3.3](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.3.3 "https://msdn.microsoft.com/en-us/library/cc230324.aspx")
on how to interpret this type of
value.  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[4\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0030: | 9E
A8 81 85 D0 01 <span style="background-color: #00ff00;">02 01 E2
65</span> <span style="background-color: #00ccff;">16 00 00 00</span>
<span style="background-color: #ffff00;">52 F6</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0040: |
<span style="background-color: #ffff00;">85 EC 7D 43 2E 4A A9 60 34 50
88 53 D9 0A 00 00</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0050: |
<span style="background-color: #ffff00;">00 00 20 7C</span> 02 01 E3 65
17 00 00 00 16 52 F6 85</span>  
  
propValue: 0x65E20102  
Property type: 0x0102 - PtypBinary  
Property ID: 0x65E2 - PidTagChangeKey  
Value: See
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
section
[2.2.1.2.7](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.1.2.7 "https://msdn.microsoft.com/en-us/library/ee178773(v=exchg.80).aspx")  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[5\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0050: | 00
00 20 7C <span style="background-color: #00ff00;">02 01 E3 65</span>
<span style="background-color: #00ccff;">17 00 00 00</span>
<span style="background-color: #ffff00;">16 52 F6 85</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0060: |
<span style="background-color: #ffff00;">EC 7D 43 2E 4A A9 60 34 50 88
53 D9 0A 00 00 00</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0070: |
<span style="background-color: #ffff00;">00 20 7C</span> 1F 00 01 30 0C
00 00 00 49 00 4E 00 42</span>  
  
propValue: 0x65E30102  
Property type: 0x0102 - PtypBinary  
Property ID: 0x65E3 - PidTagPredecessorChangeList  
Value: See
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
section
[2.2.1.2.8](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.1.2.8 "https://msdn.microsoft.com/en-us/library/ee202808(v=exchg.80).aspx")  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[6\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0070: | 00
20 7C <span style="background-color: #00ff00;">1F 00 01 30</span>
<span style="background-color: #00ccff;">0C 00 00 00</span>
<span style="background-color: #ffff00;">49 00 4E 00 42</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0080: |
<span style="background-color: #ffff00;">00 4F 00 58 00 00 00</span> 14
00 49 67 01 00 00 00 00</span>  
  
propValue: 0x3001001F  
Property type: 0x001F - PtypString  
Property ID: 0x3001 - PidTagDisplayName  
Value: "INBOX"  
Note: Per
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.11.1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.11.1 "https://msdn.microsoft.com/en-us/library/ee157583(v=exchg.80).aspx")
this *"a string of Unicode characters in UTF-16LE format encoding with
terminating null character
(0x0000)."*  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[7\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0080: | 00
4F 00 58 00 00 00 <span style="background-color: #00ff00;">14 00 49
67</span> <span style="background-color: #ffff00;">01 00 00 00 00
</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0090: |
<span style="background-color: #ffff00;">00 03 F4</span> 03 00 39 66 FB
03 00 00 40 00 07 30 80</span>  
  
propValue: 0x67490014  
Property type: 0x0014 - PtypInteger64  
Property ID: 0x6749 - PidTagParentFolderId  
Value:
-863846703525003263  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[8\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0090: | 00
03 F4 <span style="background-color: #00ff00;">03 00 39 66</span>
<span style="background-color: #ffff00;">FB 03 00 00</span> 40 00 07 30
80</span>  
  
propValue: 0x66390003  
Property type: 0x0003 - PtypInteger32  
Property ID: 0x6639 - PidTagRights  
Value: 0x000003FB  
<span style="text-decoration: underline;">**Note:**</span> See
[MS-OXCPERM](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCPERM "https://msdn.microsoft.com/en-us/library/cc463904(v=exchg.80).aspx")
section
[2.2.7](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.7 "https://msdn.microsoft.com/en-us/library/ee201699(v=exchg.80).aspx")
on how to interpret these
flags.  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[9\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x0090: | 00
03 F4 03 00 39 66 FB 03 00 00
<span style="background-color: #00ff00;">40 00 07 30</span>
<span style="background-color: #ffff00;">80</span></span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x00A0: |
<span style="background-color: #ffff00;">39 06 A8 81 85 D0 01</span> 0B
00 F4 10 00 00</span>  
  
propValue: 0x30070040  
Property type: 0x0040 - PtypTime  
Property ID: 0x3007 - PidTagCreationTime  
Value: 05/03/2015 09:15:11  
<span style="text-decoration: underline;">**Note:**</span> See
[MS-DTYP](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-DTYP "https://msdn.microsoft.com/en-us/library/cc230273.aspx")
section
[2.3.3](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.3.3 "https://msdn.microsoft.com/en-us/library/cc230324.aspx")
on how to interpret this type of
value.  
  
  
<span style="font-family: &#39;courier new&#39;, courier;">Element\[10\]:</span>  
<span style="font-family: &#39;courier new&#39;, courier;">0x00A0: | 39
06 A8 81 85 D0 01 <span style="background-color: #00ff00;">0B 00 F4
10</span> <span style="background-color: #ffff00;">00 00</span></span>  
  
propValue: 0x10F4000B  
Property type: 0x000B - PtypBoolean  
Property ID: 0x3007 - PidTagAttributeHidden  
Value: 0x0000 (False)  
<span style="text-decoration: underline;">**Note**</span>: If you look
at the table in
[MS-OXCDATA](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCDATA "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx")
section
[2.11.1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.11.1 "https://msdn.microsoft.com/en-us/library/ee157583(v=exchg.80).aspx")
you will see that the description for PtypBoolean states that it is *"1
byte; restricted to 1 or 0."* However, you can see that I have 2 bytes
highlighted. This is because PtypBoolean is serialized differently than
is described there. This difference is described in
[MS-OXCFXICS](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/MS-OXCFXICS "https://msdn.microsoft.com/en-us/library/cc463916(v=exchg.80).aspx")
in section
[2.2.4.1.3](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/2.2.4.1.3 "https://msdn.microsoft.com/en-us/library/ee201502(v=exchg.80).aspx")
where it states that the PtypBoolean type is *"2-bytes in FastTransfer
streams, instead of 1-byte as specified in
[\[MS-OXCDATA\]](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/%5BMS-OXCDATA%5D "https://msdn.microsoft.com/en-us/library/cc425496(v=exchg.80).aspx").
Using little-endian byte ordering, "01 00" for TRUE and "00 00" for
FALSE."*  
  
  
I hope this helps you to understand how the Fast Transfer stream is
serialized. Even though we only looked at a few of the elements in this
stream, it should give you a good idea on how to proceed to parse a
complete FastTransfer stream.

</div>

</div>
