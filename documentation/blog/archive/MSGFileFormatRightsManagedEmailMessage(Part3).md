<div id="page">

# .MSG File Format, Rights Managed Email Message (Part 3)

[tomjebo](https://social.msdn.microsoft.com/profile/tomjebo) 6/14/2011
1:24:58 PM

-----

<div id="content">

In Part 2, I concluded my dissection of the rights managed email message
example, with locating the DRMTransform\\Primary stream. At the time, I
didn’t go into much detail because I was focused on a) finding the
message.rpmsg attachment and b) showing how to decompress the binary
data to yield the storage container in which that stream lives. To pick
things up where I left off, I refer back to \[MS-OXORMMS\] 3.1.4.1.3,
the diagram of the storage container. The storage and stream listing
below shows recognizable names but some of these are actually shared
storage structures defined in
\[MS-OFFCRYPTO\].

[![clip\_image002](images/3377.clip_image002_thumb.jpg
"clip_image002")](images/0825.clip_image002_2.jpg)

Figure 1 Showing the storage and stream layout of the message storage
container.

So for the scope of this storage container that we’ve decompressed, you
will see that the following layered specifications are used by Outlook
(from bottom to top):

|                  |
| ---------------- |
| \[MS-CFB\]       |
| \[MS-OFFCRYPTO\] |
| \[MS-OXORMMS\]   |

\[MS-OFFCRYPTO\] defines the DataSpaces and TransformInfo storages. The
DataSpacesMap stream contains entries that name the streams containing
the content being transformed as well as the names of DataSpacesInfo
based streams that point to TransformInfo references which describe the
transforms, in order, that are applied to the corresponding content in
the map. This is the general structure used by Office applications for
consistency when doing protection and transformations on
content.

[![clip\_image004](images/1643.clip_image004_thumb.jpg
"clip_image004")](images/1234.clip_image004_2.jpg)

Figure 2 DataSpaceMap stream, showing one entry that consists of the
content stream “DRMContent” and the data space information stream
“DRMDataSpace”.

<span class="underline">[![clip\_image006](images/4370.clip_image006_thumb.jpg
"clip_image006")](images/0247.clip_image006_2.jpg)</span>

Figure 3 DRMDataSpace stream showing a DataSpaceDefinition structure
with one TransformReferences struct pointing to “DRMTransform”.

What this means is that any Office application that transforms or
protects part or all of its content must first build a structure around
\[MS-OFFCRYPTO\] 2.1. On top of that, the application then defines one
or more transforms that can be applied to the content. In the case of
Outlook and it’s rights managed message, DRMTransform specifies the
transformation that was or will be done on the content in DRMContent.
Although \[MS-OXORMMS\] specified the names DRMTransform and DRMContent,
interoperating applications will know to find these transformations by
looking at the DataSpaceMap and DataSpaceInfo written according to
\[MS-OFFCRYPTO\]. This allows for consistency across
applications.

[![clip\_image008](images/7534.clip_image008_thumb.png
"clip_image008")](images/8203.clip_image008_2.png)

Figure 4 DRMTransform\\Primary stream showing

The “Primary” stream is named by \[MS-OFFCRYPTO\] 2.1.1 as the stream
containing the transformation algorithm used to transform the content.
There can be other streams besides “Primary” if needed by the
application. The first part of the Primary stream is a required
TransformInfoHeader struct. This contains two strings, an id (partly
shown above) and a name for the transfom. In this case, it’s called
“Microsoft Metadata DRMTransform”. After that, the stream can contain
other structures and data that define the algorithm.

In the previous blog entry, I pointed out the “XrML…” string and
identified this as XrML. This is the transform. It’s a format based on
XML that tells the application how to get access to the original email
message and what rights to apply have once decoded. XrML is a standard
language used to specify rights on many kinds of digital content and an
overview of the language can be found in the
[XrMLTechnicalOverview](http://www.xrml.org/Reference/XrMLTechnicalOverviewV1.pdf)
PDF file.

For this and the next several installments, I will go a little further
explaining the relationship between \[MS-OXORMMS\] and \[MS-OFFCRYPTO\]
and investigate the various elements and attributes in this XrML string
so that the reader can get some understanding of how XrML works and it’s
power. But to wrap up this session, some history on XrML.

###### Xerox PARC, where it all began

In 1994, a patent was filed by Xerox which contained a language called
DPRL for Digital Property Rights Language. This was based on research
being done by Xerox PARC’s Mark Stefik, who was working on trusted
systems for secure digital commerce. DPRL, originally, was expressed in
the LISP programming language. It wasn’t until 1998 that Xerox released
version 2.0 of DPRL that was expressed in XML. This version of DPRL was
licensed to ContentGuard which was a company founded by Microsoft and
Xerox. In 2001, DPRL evolved into the published XrML 1.0.

</div>

</div>
