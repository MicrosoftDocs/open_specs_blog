<div id="page">

# MS-PST - The Relationship Between Nodes and Blocks

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 2/11/2011
8:18:00 AM

-----

<div id="content">

The current version of the MS-PST open specification document can be
found here:
<http://msdn.microsoft.com/en-us/library/ff385210(office.12).aspx>

 

<span style="font-weight: bold;">Introduction</span>

The PST is a structured binary file format that contains messages,
folders, and attachments. The file structure can be logically separated
into 3 layers: the Node Database (NBD) layer, the Lists, Tables, and
Properties (LTP) layer, and the Messaging layer. Each layer builds on
the one before and because of this it is necessary to completely
understand the NDB layer before the LTP layer, and to understand the LTP
layer before the Messaging layer. Like building a house, you cannot
start framing the walls until you pour the foundation. If you've browsed
through the MS-PST file format specification document you might recall
seeing this figure from section
[1.3.1.1](http://msdn.microsoft.com/en-us/library/ff387767\(office.12\).aspx).

 

 

![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/1256.F1.jpg)

 

Figure 1: Relationship between nodes and blocks.

 

 

This figure provides an overview on how to navigate both the Node and
Block BTrees in order to find the data block that belongs to the node
with ID (NID) 100. I am going to expand on this relatively simple figure
and show you exactly how to do it. If you haven't already done so. You
might want to review my first article: [MS-PST - How to Navigate the
Node
BTree](http://blogs.msdn.com/b/openspecification/archive/2010/11/30/ms-pst-how-to-navigate-the-node-btree.aspx).

 

 

<span style="font-weight: bold;">Structures that you should be familiar
with</span>

[HEADER](http://msdn.microsoft.com/en-us/library/ff387474\(office.12\).aspx):
The structure at the beginning of the file that contains meta-data about
the PST
file.

[ROOT](http://msdn.microsoft.com/en-us/library/ff385608\(office.12\).aspx):
A structure in the HEADER that contains file state
information.

[BREF](http://msdn.microsoft.com/en-us/library/ff386638\(office.12\).aspx):
A structure that contains a BID/IB
pair.

[BID](http://msdn.microsoft.com/en-us/library/ff387585\(office.12\).aspx):
Block ID, a unique
identifier.

[NID](http://msdn.microsoft.com/en-us/library/ff385251\(office.12\).aspx):
Node ID, a unique
identifier

[IB](http://msdn.microsoft.com/en-us/library/ff386553\(office.12\).aspx):
Byte Index, an unsigned 64-bit value that represents an absolute file
position.

[BTree](http://msdn.microsoft.com/en-us/library/ff386554\(office.12\).aspx):
Generic binary tree structure.

NBT: The Node BTree.

BBT: The Block
BTree.

[Page](http://msdn.microsoft.com/en-us/library/ff386116\(office.12\).aspx):
A generic 512 byte section of the
file.

[PageTrailer](http://msdn.microsoft.com/en-us/library/ff387943\(office.12\).aspx):
A 16 byte structure at the end of each Page that contains meta-data
about the
page.

[BTPage](http://msdn.microsoft.com/en-us/library/ff385991\(office.12\).aspx):
A 512 byte page that is part of the Node or Block
BTree.

[BTEntry](http://msdn.microsoft.com/en-us/library/ff387350\(office.12\).aspx):
Generic BTree
entry.

[NBTEntry](http://msdn.microsoft.com/en-us/library/ff385505\(office.12\).aspx):
Leaf node entries in the Node
BTree.

[BBTEntry](http://msdn.microsoft.com/en-us/library/ff386084\(office.12\).aspx):
Leaf node entries in the Block BTree.

 

 

<span style="font-weight: bold;">Pick a NID, any NID</span>

I will be using the same sample PST file that I used in my first article
and randomly (sort of) pick a NID that I know exists. How about 0xC01? I
think that will work. Now that we know the NID that we want to find, we
will search the Node BTree for it. Our NBT Root page is located at
0x9000 and contains 3 BTENTRY
structures.

 

 

![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/1464.F2.jpg)

 

Figure 2: The NBT Root BTPAGE.

 

 

We want to look at the first 8 bytes of each entry, or the btKey value.
The 3 values we see above are 0x21, 0x60F, and 0x2226. Per section
[2.2.2.7.7.2](http://msdn.microsoft.com/en-us/library/ff387350\(office.12\).aspx),
"<span style="font-style: italic;">All the entries in the child BTPAGE
referenced by BREF have key values greater than or equal to this key
value.</span>" Based on that, we can see that the next page we need to
look at is the second one because 0xC01 is greater than 0x60F but less
than 0x2226. The last 8 bytes of each BTENTRY entry represents the IB of
the BREF structure. The IB (Byte Index) is an unsigned 64-bit value that
represents the absolute file offset where we can find the child BTPAGE.
In this case, the next 512 byte page we want to look at is located at
offset
0x8600.

 

 

![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4426.F3.jpg)

 

Figure 3: A BTPAGE containing NBTENTRY structures.

 

 

The above BTPAGE contains 13 NBTENTRY structures. Each one is 32 bytes
in length and the first 8 bytes contains the NID value that we are
searching for. We can see that the NID we are looking for is the 10th
item in the rgentries array. The next 8 bytes in the structure contains
the bidData value, or the Data BID from Figure 1. That value is what we
need to begin our search in the Block BTree. In this case it's 0x48.

 

 

<span style="font-weight: bold;">Second verse, same as the first…</span>

Now that we know the BID to look for in the Block BTree we can start
navigating it in much the same way as the Node BTree. Like the Node
BTree, the location of the Root page of the Block BTree is stored in the
ROOT structure of the Header. See section
[2.2.2.5](http://msdn.microsoft.com/en-us/library/ff385608\(office.12\).aspx)
for more information about the ROOT structure. The value is stored in
the IB value of the BREFBBT structure. In our sample PST file the
location of the Block BTree Root page is at 0x8000. Here are the
contents of the BTPage at that
location.

 

 

![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4774.F4.jpg)

 

Figure 4: The BBT Root BTPAGE

 

 

Like the Node BTree Root page, the Block BTree Root page has an array of
BTENTRY structures. Each entry is 24 bytes in length and the first 8
bytes is the btkey value. Using the same logic as before we can see that
the entry we are looking for is going to be down the first branch of the
tree since 0x48 is less than 0x80 but greater than 0x04. As before, the
last 8 bytes tells us where to look in the file for the next BTPage. In
this case it's located at
0x6A00.

 

 

 ![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/3644.F5.jpg)

 

Figure 5: A BTPAGE containing BBTENTRY structures.

 

 

Unlike the NBTENTRY structure, the BBTENTRY structure is 24 bytes.
Similarly though, the first 8 bytes is the BID so it's easy to identify
the record we are looking for. In the above BTPAGE we can see that the
11th BBTENTRY matches the BID we are look for (0x48). Once we have
located the correct record we look at the second 8 bytes to find the IB,
or offset where the block of data is that we are looking for (0x4FC0).

 

Blocks are a little different from pages though in that they aren't a
fixed size. Blocks can be anywhere from 64 - 8192 bytes and are aligned
on a 64-byte boundary. The 2 bytes following the IB contains the value
that represents the number of raw bytes contained in the block. This
count does not represent the actual size of the block though. The block
also contains a 16-byte block trailer and padding to make it align to a
64-byte boundary. So, in our example, the raw byte count is 0x22, or 34
bytes, adding the 16-byte block trailer brings the count to 50. Since
the minimum size of a block is 64-bytes we need to read 64-bytes from
file location
0x4FC0.

 

 

![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/0042.F6.jpg)

 

Figure 6: The Block of data from location 0x4FC0.

 

 

The first 34 bytes of raw data is what we have been after the whole
time. We started with a NID and now we have the data that is associated
with it. So, what now? What does that data represent? That depends on a
number of other factors and will the topic of a future blog.

 

Notice the red highlighted block of zeroed out data after the 34 byte
chuck of data and before the 16-byte block trailer. This is the padding
that is added to make the block meet the 64-byte boundary requirement.
It's important to know that those bytes are NOT guaranteed to be zeroed
out and may contain random or old data. Make sure you ignore those
bytes.

</div>

</div>
