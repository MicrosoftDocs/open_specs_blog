<div id="page">

# MS-PST - Parsing a Heap-on-Node Property Context Block

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 5/30/2014
12:12:00 PM

-----

<div id="content">

Summary

This Blog will use the sample Heap-on-Node (HN) from section
[3.8](http://msdn.microsoft.com/en-us/library/ff386697\(v=office.12\).aspx)
of
[MS-PST](http://msdn.microsoft.com/en-us/library/ff385210\(v=office.12\).aspx)
and walk through the process of how to read a property from it. The
current version of the MS-PST open specification document can be found
here: <http://msdn.microsoft.com/en-us/library/ff385210(office.12).aspx>

 

 

Introduction

First, it's important to understand that there are several layers and
structures involved here. A basic understanding of the NDB and LTP
layers is extremely helpful, but not required.

 

The construction of the Heap-on-Node can be summarized as
    follows:

1.  <span style="font-family: Calibri; font-size: 11.0pt; font-weight: normal; font-style: normal;">The
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">Block</span>](http://msdn.microsoft.com/en-us/library/ff387148\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt; font-weight: normal; font-style: normal;">
    is a
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">Heap-on-Node</span>](http://msdn.microsoft.com/en-us/library/ff386518\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt; font-weight: normal; font-style: normal;">.</span>
2.  <span style="font-family: Calibri; font-size: 11.0pt;">The
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">BTree-on-Heap</span>](http://msdn.microsoft.com/en-us/library/ff385560\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt;">
    is built on top of the
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">Heap-on-Node</span>](http://msdn.microsoft.com/en-us/library/ff386518\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt;">.</span>
3.  <span style="font-family: Calibri; font-size: 11.0pt;">The
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">Property
    Context</span>](http://msdn.microsoft.com/en-us/library/ff385509\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt;">
    is built on top of the
    </span>[<span style="font-family: Calibri; font-size: 11.0pt;">BTree-on-Heap</span>](http://msdn.microsoft.com/en-us/library/ff385560\(v=office.12\).aspx)<span style="font-family: Calibri; font-size: 11.0pt;">.</span>

 

 

The HNHDR

First, we will take a look at the
[HNHDR](http://msdn.microsoft.com/en-us/library/ff386774\(v=office.12\).aspx)
which is the first 12 bytes of the
block.

 

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4645.F1.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4645.F1.png)

Figure 1: The HNHDR.

 

For now, we really only care about 2 of the properties here. The first 2
bytes which is the <span style="font-weight: bold;">ibHnpm</span>
property with a value of <span style="font-weight: bold;">0x00EC</span>.
This is the offset from the beginning of the block to the beginning of
the
[HNPAGEMAP](http://msdn.microsoft.com/en-us/library/ff385507\(v=office.12\).aspx).
The 4th byte is the <span style="font-weight: bold;">bClientSig</span>
with a value of <span style="font-weight: bold;">0xBC</span>. This tells
us that this block contains a [Property
Context](http://msdn.microsoft.com/en-us/library/ff385509\(v=office.12\).aspx)
which is built on top of a
[BTree-on-Heap](http://msdn.microsoft.com/en-us/library/ff385560\(v=office.12\).aspx).
This will be important later.

 

 

The HNPAGEMAP

Looking at the offset in the
[Block](http://msdn.microsoft.com/en-us/library/ff387148\(v=office.12\).aspx)
specified by the ibHnpm property, it's not possible to know the length
of the
[HNPAGEMAP](http://msdn.microsoft.com/en-us/library/ff385507\(v=office.12\).aspx)
until you read the first property,
<span style="font-weight: bold;">cAlloc</span>, which in this case is
<span style="font-weight: bold;">0x0008</span>. This tells us that the
<span style="font-weight: bold;">rgibAlloc</span> table will contain 9
(8 + 1) entries. Each entry is a 2 byte WORD value. So, the length of
the rgibAlloc table is 18 (9 \* 2) bytes in length. Add the 2 bytes each
for cAlloc and cFree and you get the total length of the
[HNPAGEMAP](http://msdn.microsoft.com/en-us/library/ff385507\(v=office.12\).aspx)
which is 22 bytes. Reading all 22 bytes we have the
following.

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/2234.F2.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/2234.F2.png)

Figure 2: The HNPAGEMAP.

 

Starting at the 5th byte, read the next 18 bytes as a series of 2-byte
values to get a list of the offsets for the starting location of each
allocation in the block. The 9th offset is a place holder that tells an
application where the next allocation should start, it is not an
allocation that contains
    data.

 

1.  <span style="font-family: Calibri; font-size: 11.0pt; font-weight: normal; font-style: normal;">0x000C</span>
2.  <span style="font-family: Calibri; font-size: 11.0pt;">0x0014</span>
3.  <span style="font-family: Calibri; font-size: 11.0pt;">0x006C</span>
4.  <span style="font-family: Calibri; font-size: 11.0pt;">0x007C</span>
5.  <span style="font-family: Calibri; font-size: 11.0pt;">0x008C</span>
6.  <span style="font-family: Calibri; font-size: 11.0pt;">0x00A4</span>
7.  <span style="font-family: Calibri; font-size: 11.0pt;">0x00BC</span>
8.  <span style="font-family: Calibri; font-size: 11.0pt;">0x00D4</span>
9.  <span style="font-family: Calibri; font-size: 11.0pt;">0x00EC</span>

 

The first allocation starting at offset 0x000C will be the
[BTHHEADER](http://msdn.microsoft.com/en-us/library/ff386157\(v=office.12\).aspx)
which is 8 bytes in length.

 

 

The
BTHHEADER

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/0513.F3.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/0513.F3.png) 

Figure 3: The BTHHEADER.

 

We care about 3 values from the
[BTHHEADER](http://msdn.microsoft.com/en-us/library/ff386157\(v=office.12\).aspx);
<span style="font-weight: bold;">cbKey</span>,
<span style="font-weight: bold;">cbEnt</span>, and
<span style="font-weight: bold;">hidRoot</span>.

 

<div style="direction: ltr;">

cbKey

</div>

</div>

</div>

0x02

cbEnt

0x06

hidRoot

0x0040, 0x0000

 

It's VERY important to understand that the hidRoot is a
[HID](http://msdn.microsoft.com/en-us/library/ff386694\(v=office.12\).aspx)
structure that should be read as 2 separate 2-byte values and NOT as a
single 4-byte value.

 

The combination of the cbKey and cbEnt tells us how long each [PC BTH
Record](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
will be. Remember back when we looked at the
[HNHDR](http://msdn.microsoft.com/en-us/library/ff386774\(v=office.12\).aspx)
and the bClientSig value was 0xBC (bTypePC)? That's how we know that the
records will be [PC BTH
Records](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx).
In this case, each record will be 8 bytes in length.

 

 

Locating The hidRoot

The hidRoot is worth spending a little bit more time talking about. The
current version of the
[MS-PST](http://msdn.microsoft.com/en-us/library/ff385210\(v=office.12\).aspx)
document (v2.1, 2/10/2014) is missing some vital information for working
with HID structures. The current version of the documentation was valid
for older versions of Outlook, however the current version of Outlook
handles this structure differently. The exact version of Outlook that
first handled it this way is not known.

 

Looking at the hidRoot we know that the hidBlockIndex is 0x0000 which
means that the heap item is contained in the same block we are currently
looking at. The hidIndex has a value of 0x0040 which seems strange
because it's supposed to be the 1-based index to the heap item where we
can look to start reading some [PC BTH
Records](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx).
Since this
[BTree-on-Heap](http://msdn.microsoft.com/en-us/library/ff385560\(v=office.12\).aspx)
node only contains 8 allocations, it couldn’t possibly be the 64th
allocation.

 

In order to get the correct allocation index we need to bit shift the
hidIndex 5 places to the right. This will result in a value of 0x0002.
Therefore, the [PC BTH
Records](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
begin at the second allocation. This information should be included in a
future release of the documentation and is vital in obtaining the
correct hidIndex value.

 

 

Reading The PC BTH Records

We now have enough information to read the collection of [PC BTH
Records](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
from the second allocation. First, in order to determine the length of
the 2nd allocation we subtract the offset of the 3rd allocation from the
offset of the 2nd. 0x6C - 0x14 = 0x58 (88
bytes).

 

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8475.F4.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8475.F4.png)

 

Figure 4: The PC BTH Records.

 

We also know that each [PC BTH
Record](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
is 8 bytes. Dividing 88 bytes by 8 bytes each gives us 11 records.

 

  - <span style="font-family: Consolas; font-size: 11.0pt;">34 0E 02 01
    A0 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">38 0E 03 00
    00 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">F9 0F 02 01
    60 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">01 30 1F 00
    80 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">DF 35 03 00
    89 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">E0 35 02 01
    C0 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">E3 35 02 01
    00 01 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">E7 35 02 01
    E0 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">33 66 0B 00
    01 00 00 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">FA 66 03 00
    0D 00 0E 00 </span>
  - <span style="font-family: Consolas; font-size: 11.0pt;">FF 67 03 00
    00 00 00 00</span>

 

Each [PC BTH
Record](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
starts with a 2-byte <span style="font-weight: bold;">wPropId</span> and
a 2-byte <span style="font-weight: bold;">wPropType</span>. The list of
possible wPropType values can be found in
[MS-OXCDATA](http://msdn.microsoft.com/en-us/library/cc425496\(v=exchg.80\).aspx)
section
[2.11.1](http://msdn.microsoft.com/en-us/library/ee157583\(v=exchg.80\).aspx)
and
[MS-OXPROPS](http://msdn.microsoft.com/en-us/library/cc433490\(v=exchg.80\).aspx)
contains the Master Property List which is probably the best place to
look for Property ID values. However, you will also find Property ID
values scattered throughout other documents where those properties are
used.

 

 

Getting The Property

We will use the 4th [PC BTH
Record](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
for demonstration purposes because we can easily look up what the
Property ID and type is. The wPropId is 0x3001 and the wPropType is
0x001F. Searching in
[MS-OXPROPS](http://msdn.microsoft.com/en-us/library/cc433490\(v=exchg.80\).aspx)
we find that it belongs to the
[PidTagDisplayName](http://msdn.microsoft.com/en-us/library/ee158398\(v=exchg.80\).aspx)
property and that the type is PtypString. According to
[MS-OXCDATA](http://msdn.microsoft.com/en-us/library/cc425496\(v=exchg.80\).aspx)
section
[2.11.1](http://msdn.microsoft.com/en-us/library/ee157583\(v=exchg.80\).aspx)
the PtypString is <span style="font-style: italic;">"Variable size; a
string of Unicode characters in UTF-16LE format encoding with
terminating null character (0x0000)." </span>

 

Now that we know what the property and the type is we can go retrieve
it. The 3rd property of the [PC BTH
Record](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
is the <span style="font-weight: bold;">dwValueHnid</span> which can
represent a few different things. Refer to
[MS-PST](http://msdn.microsoft.com/en-us/library/ff385210%28v=office.12%29.aspx)
section
[2.3.3.3](http://msdn.microsoft.com/en-us/library/ff386558\(v=office.12\).aspx)
on how to determine what the type of data stored in it. In this case,
it's a
[HID](http://msdn.microsoft.com/en-us/library/ff386694\(v=office.12\).aspx)
structure. Reading the hidIndex value of 0x0080 and bitshifting it 5
places to the right (like we did for the hidRoot earlier) we get an
index value of 0x0004. That means that the string value that belongs to
the PidTagDisplayName is stored in the 4th allocation of this block.
Looking back at the list of offsets from the
[HNPAGEMAP](http://msdn.microsoft.com/en-us/library/ff385507\(v=office.12\).aspx)
we can see that the 4th allocation starts at offset 0x007C and is 0x10
(16 bytes) in
length.

 

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/7220.F5.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/7220.F5.png)

Figure 5: pidTagDisplayName.

 

This represents the string "UNICODE1". Unfortunately, this block does
not contain any other meaningful properties that we can look up in
[MS-OXPROPS](http://msdn.microsoft.com/en-us/library/cc433490\(v=exchg.80\).aspx),
but if there were you would follow the same
process.

 

 

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5468.F6.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5468.F6.png)

Figure 6: Complete Heap-On-Node Property Context Block.
