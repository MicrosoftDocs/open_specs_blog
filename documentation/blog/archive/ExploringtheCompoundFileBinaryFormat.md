<div id="page">

# Exploring the Compound File Binary Format

[tomjebo](https://social.msdn.microsoft.com/profile/tomjebo) 7/24/2009
12:21:00 PM

-----

<div id="content">

Although the march of progress steadily tramples the old tried and true
in favor of enlightened file formats designed for the new era of the
web, some of us take joy in digging deeper into bits and bytes of binary
file formats.  I’m one of those and I can’t resist hacking my way
through one of the oldest formats used by Microsoft applications, the
Compound File Binary Format (CFBF). 

Besides having been the bread and butter for the Microsoft Office suite
of applications for many years (Visio .vsd, Publisher .pub, Outlook .msg
have not replaced CFBF files as their formats in the latest versions)
and almost all OLE applications that are capable of linking and
embedding, CFBF has been put to use in many other applications and
environments as well.   Picture It\!, Digital Image, SQL Server 2000 DTS
packages, MSI packages, the Explorer Shell thumbnail cache, Internet
Explorer RSS feeds, Rights Management add-on for Internet Explorer as
well as many third party application formats like AAF (a professional
file interchange format designed for video post production and
authoring), Kodak FlashPix photo file, CAD programs like Solid Works
just to name a few.  And you can also find the reverse engineered
implementations like Open Office designed for interoperability with
Microsoft Office.  A pure Java implementation of the OLE 2 Compound
Document named POIFS (part of the Apache POI Java API To Access
Microsoft Format Files), POLE and HPSF are also reverse engineered
implementations.   These are just a few examples the aftereffect of
which should be a sinking … er, I mean warm feeling that CFBF is kind of
ubiquitous. 

This is why we publish [\[MS-CFB\] Compound File Binary File Format
Specification](http://msdn.microsoft.com/en-us/library/cc546605.aspx),
an open specification for CFBF.  By reading this entire document, you
will thoroughly understand and be able to parse your way manually
through any compound file using this format.  Well, that’s kind of
optimistic.  Theoretically that is true.  The document is very complete
and rescues us from the days of a cryptic, hard to find document
(available in the Win32 SDK long ago) to a much happier place.  However,
there is still some complexity inherent in the underlying structure. 
Assuming you have the programmer’s perception of the compound file
already burned into the neurons of your brain, for example the Root
Storage, Storages, Streams, names like \\001CompObj, etc…, **figure 1**
below, gives the high level overview (like International Space Station’s
view) of the organization that’s manipulated by the structured storage
API’s like StgCreateDofile() and IStorage::CreateStream() on your
behalf:

[![cfbf2](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/cfbf2_thumb.png
"cfbf2")](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/cfbf2_2.png)

 

 

 

###### Figure 1: High level view of compound file

Because the details are in \[MS-CFB\], I’ll give the nickel tour here. 
FAT’s (File Allocation Table) in CFBF are pretty much like FAT’s in the
file system on disk.  The whole file is divided conveniently in to 512
byte (or 4096 byte, depending on the version of the format your
application uses) sectors.  Therefore, a FAT array maps out all the 512
byte sectors in the file.  The very first sector in the file consists of
a header record with a signature of **0xd0cf11ea1b11ae1** at offset 0,
just so you know it’s really a compound document.  Get it, docfile …
d0cf11e… ok.  This header points to everything else in the file. The
DIFAT array, the first 109 elements of which are in the header, is the
meta-FAT for FAT sectors.  Because you can have more than one sector
containing FAT array data (i.e. sector map), the DIFAT array tells which
sectors in the file are FAT’s.  The Directory Entry array simply
provides a directory of all the storages and streams in the file
including the “**Root Entry**”, a special directory entry* *which every
compound file has and which serves as the parent of all other directory
entries.  Each stream directory entry points to the first sector
containing data and the number of sectors total used by that stream. 
The mini stream is a special stream, pointed to by the Root Entry
instead of the Directory Entry array, that contains application stream
data that is less than the mini stream cutoff size (0x1000) in total
length.  These are mapped by a miniFAT array and each “mini sector” if
you will is actually only 64 bytes in length.  This saves room on
fragmentation when an application’s stream only uses a fraction of the
512 or 4096 byte sectors normally allocated for data.

In the process of manually parsing files from various applications
looking for non-conformance to the specification or possible logic bugs,
it’s expedient to use binary editors, especially ones that will support
parsing scripts.  Doing it by hand is really tedious.  In the way Netmon
supports scripts for parsing packet records on the wire, scripts can be
written to parse the structures that are described in MS-CFB.  These
parsing scripts can be very powerful and make life much easier. 
However, complexity in traversing non-contiguous sectors of a stream in
script logic can cause some to gravitate toward the pragmatism of just
preprocessing the file to rewrite streams with contiguous sectors.  This
may seem the lazy approach to script writing but it actually aids in
visual verification of the stream data as well.  For example, you’ve
found the starting sector of a stream, let’s say “**STM1**”, of interest
in an application’s data file.  It took you very little effort to get
there, you just 1) parsed the Directory Entry array, looking for the
stream’s name and 2) located the starting sector number in the stream’s
directory entry and voila (which is French for add one to the starting
sector number and multiply by 512), you’re there.  So you start reading
down the line and everything looks in order but on the 512th
(zero-based) byte you don’t see what you expect.  Well, that’s because
the next sector in the file doesn’t belong
**STM1**\! 

[![discontig](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/discontig_thumb_2.png
"discontig")](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/discontig_6.png)

###### 

###### Figure 2: Non-contiguous sectors in a stream

Now you have to go back to the FAT array and find out what sector is the
next in the chain.  How inconvenient.  If they were all contiguous, you
could just keep reading.  If only you had a tool that would rearrange
the stream’s sectors to be contiguous.  There are tools that will
defragment and compact compound files but you may not have access to one
easily.  I remembered reading “Inside OLE 2” by Kraig Brockschmidt about
\<clearing throat\> years ago from cover to cover and it seemed to me
that he talked about structured storage and compound files with optimism
and enthusiasm not seen in the MSDN docs.  So I blew the dust off the
page tops and after just a few minutes found the section on
defragmentation.  A very interesting statement compelled me.  On page
398, Brockschmidt said that IStorage::CopyTo() would, “…remove all the
dead space in the process of copying the contents of one storage to
another and **will order the contents of streams sequentially,** as
shown…” and he gave a diagram to bolster my resolve.  At this point, I
was excited to have a possibly simply and elegant solution to the
non-contiguous stream sector problem.  Just use IStorage::CopyTo() to
copy the root storage from one compound document to a newly created
“processed” document.  The recursive algorithm for CopyTo() is pretty
straightforward and goes something like
this:

######                 For each source child element, create child storage/stream in target

######                 If (storage)

######                          recursively call source-\>CopyTo(new target IStorage)

######                 Else (stream)

######                          Set target stream size

######                          Copy target stream contents

This algorithm leads us to expect that the resulting file would have
good contiguity, so I immediately coded the small fragment in a console
application.   Here’s what the code looks like:

<span style="color: #0080ff;" color="#0080ff">int \_tmain(int argc,
\_TCHAR\* argv\[\])</span>

<span style="color: #0080ff;" color="#0080ff">{</span>

<span style="color: #0080ff;" color="#0080ff">    CoInitialize(NULL);
</span>

<span style="color: #0080ff;" color="#0080ff">    int srclen = 0;
</span>

<span style="color: #0080ff;" color="#0080ff">    if ((argc \<
1)||((srclen = \_tcslen(argv\[1\])) \> 107))    // give 107 chars total,
the path plus 7 for ".sequen" that we'll add</span>

<span style="color: #0080ff;" color="#0080ff">        return -1;  // do
failure processing here.</span> 

    LPSTORAGE lpSrcStg = NULL;

    // pop open the docfile and suck out the root storage

    HRESULT hr = StgOpenStorage(argv\[1\], NULL, STGM\_DIRECT |
STGM\_READ | STGM\_SHARE\_DENY\_WRITE,

                                  NULL, NULL, \&lpSrcStg); 

    if (FAILED(hr))

        return hr; // do failure processing here.

    <span style="color: #0080ff;" color="#0080ff">// This part creates
the new file name, taking “file.ext” and making it
“file.sequen.ext”</span>

<span style="color: #0080ff;" color="#0080ff">    \_TCHAR\* lptszTail =
\_tcsrchr((\_TCHAR\*)argv\[1\], '.' );     // find the last dot</span>

<span style="color: #0080ff;" color="#0080ff">    // allocate the new
path buffer and clear it</span>

<span style="color: #0080ff;" color="#0080ff">    \_TCHAR\* lptszTarget
= new \_TCHAR\[108\];  // allocate enough to hold the path plus the
".sequen" insertion and a null.</span>

<span style="color: #0080ff;" color="#0080ff">    memset(lptszTarget,
0x00, 108);                // clear out the remainder of the string.
</span>

<span style="color: #0080ff;" color="#0080ff">    // copy the path up
the the extension, insert
".</span><span style="color: #0080ff;" color="#0080ff">sequen</span><span style="color: #0080ff;" color="#0080ff">",
then copy the extension.</span>

<span style="color: #0080ff;" color="#0080ff">    int posdot =
lptszTail-argv\[1\];</span>

<span style="color: #0080ff;" color="#0080ff">   
\_tcsncpy\_s(lptszTarget, posdot+1, argv\[1\],
posdot);                          // copy everything
before the dot.</span>

<span style="color: #0080ff;" color="#0080ff">   
\_tcsncpy\_s(lptszTarget+posdot, 8, \_T(".sequen"),
7);                         // insert the
".sequen"</span>

<span style="color: #0080ff;" color="#0080ff">   
\_tcscpy\_s(lptszTarget+posdot+7, \_tcslen(lptszTail)+1,
lptszTail);        // copy the rest </span>

<span style="color: #0080ff;" color="#0080ff">    </span>

<span style="color: #000000;" color="#000000">    LPSTORAGE lpDestStg =
NULL;</span>

    hr = StgCreateDocfile(lptszTarget, STGM\_TRANSACTED |
STGM\_READWRITE | STGM\_SHARE\_EXCLUSIVE, NULL, \&lpDestStg);

    if (FAILED(hr))

        return hr; // do failure processing here.

    hr = lpSrcStg-\>CopyTo(NULL, NULL, NULL, lpDestStg);

    if (FAILED(hr))

        return hr; // do failure processing here.

<span style="color: #0080ff;" color="#0080ff">   
lpSrcStg-\>Release();</span>

<span style="color: #0080ff;" color="#0080ff">    lpDestStg-\>Release();
</span>

<span style="color: #0080ff;" color="#0080ff">   
CoUninitialize();</span>

<span style="color: #0080ff;" color="#0080ff">    return
0;</span>

<span style="color: #0080ff;" color="#0080ff">}</span>

</div>

</div>

 

#### 

###### 

###### Figure 3: Code to copy a compound document, reordering the stream sectors

The active code is in black.  It’s very simple, just open the source
file and copy the root storage to the destination. 
[StgOpenStorage()](http://msdn.microsoft.com/en-us/library/aa380341\(VS.85\).aspx)
and
[StgCreateDocFile()](http://msdn.microsoft.com/en-us/library/aa380323\(VS.85\).aspx)
open and create compound files, respectively, returning an IStorage
pointer representing the root storage to the file. According to
Brockschmidt and the reference for
[IStorage::CopyTo()](http://msdn.microsoft.com/en-us/library/aa380018\(VS.85\).aspx)
this should have some positive effect and indeed, upon testing, it
does.  Verifying by hand, **some** of the streams are actually
rewritten contiguously in the process of copying .  Hm… only **some**? 
Further investigation showed me that the streams that were
non-contiguous still were the “small” streams.  Remember the ones that I
mentioned earlier that live in the ministream and are allocated with the
miniFAT array?  Well, these will be copied by the CopyTo() algorithm
just like the larger streams.  That means that from the logical
perspective of the stream being written, each 64 byte sector is written
sequentially until all the sectors for that stream are down on disk. 
However, the ministream is not just one sector and as it grows, new
sectors are allocated for it.  New sectors that are not necessarily
contiguous.  Note in **figure 4** below that the sectors for STM3 are
indeed written sequentially but **not** contiguously. Therefore, any
given small stream in the ministream is only contiguous if all of it’s
64 byte sectors reside within one of the ministream’s 512 byte sectors.
This means that at most a small stream can have eight 64-byte sectors of
data in order to fit contiguously in one ministream sector (assuming 512
byte
sectors).

[![ministreamfrag](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/ministreamfrag_thumb_1.png
"ministreamfrag")](media/TNBlogsFS/BlogFileStorage/blogs_msdn/openspecification/WindowsLiveWriter/ExploringtheCompoundFileBinaryFormat_13A40/ministreamfrag_4.png)

###### Figure 4: Non-contiguous sectors of the ministream break up the (sequential) sectors of contained streams

The code shown in **figure 3** will have the desired effect of
organizing the sectors for larger streams both sequentially and
contiguously, making them easier to read.  Although it will write
smaller streams (\< sector size) sequentially as well, it will not
guarantee that the smaller stream sectors are contiguous.  That is a
problem to solve in a later session.
