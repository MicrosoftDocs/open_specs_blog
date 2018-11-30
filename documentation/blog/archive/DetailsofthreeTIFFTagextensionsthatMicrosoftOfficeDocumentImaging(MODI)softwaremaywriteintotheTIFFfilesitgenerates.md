<div id="page">

# Details of three TIFF Tag extensions that Microsoft Office Document Imaging (MODI) software may write into the TIFF files it generates

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
12/8/2009 12:39:00 PM

-----

<div id="content">

Microsoft Office Document Imaging (MODI) software includes specific
tags/constants in the documents it generates, of which some are
enumerated
[here](http://msdn.microsoft.com/en-us/library/aa167596\(office.11\).aspx). 
In addition to these enumerated tags there are some undocumented TIFF
tag extensions that MODI may write into TIFF documents.

The following information details three of these TIFF tags (**37679**,
**37681**, **37680**), and you can find corresponding **sample source
files**
[here](http://www.microsoft.com/downloads/details.aspx?FamilyID=0dbc435d-3544-4f4b-9092-2f2643d64a39),
which are provided to aid in implementation.

**Tag** **37679** contains plain text reflecting the contents of the
page image in the TIFF file. This text can either come from an OCR
operation on the image or directly from the document content if the TIFF
file was produced by the “Microsoft Office Document Image Writer”
virtual printer.

**Tag** **37681** contains positioning information which describes where
the text from **Tag 37679** appears on the page, as well as information
about the position of other objects such as images, tables, and hyphens.
The information in this tag is used by the MODI application to enable
its text selection feature.

**Tag** **37680** contains a binary dump of an OLE Property Set Storage.
The complete specification of property set storages can be found
[here](http://msdn.microsoft.com/en-us/library/dd942421\(PROT.13\).aspx).
An OLE Property Set Storage is a particular kind of Compound File. The
complete specification of the Compound File Binary File Format can be
found
[here](http://msdn.microsoft.com/en-us/library/dd942138\(PROT.13\).aspx).

The easiest way to understand the properties that can be stored in this
tag and how to read them is to examine the provided sample source code.

The sample code makes use of the following Windows APIs to extract the
data from the file into memory and map a Property Set Storage interface
onto it. Developers who want to read this OLE property set storage on
platforms other than Windows can use the above mentioned specifications
to write their own APIs that perform equivalent functions. More
documentation for each of these APIs may be found on MSDN.

· GlobalAlloc to allocate memory (the C runtime function malloc does not
return an HGLOBAL and thus isn’t sufficient for this particular purpose.
The sample code uses malloc when it can).

· GlobalLock to get a pointer from the handle returned by GlobalAlloc.

· GlobalUnlock after reading data from the file into the pointer
returned by GlobalLock.

· CreateILockBytesOnHGlobal followed by StgOpenStorageOnILockBytes to
wrap the data in an IStorage.

· StgCreatePropSetStg to get an IPropertySetStorage interface.

A Property Set Storage contains multiple Property Storages. These
Property Storages are identified by GUIDs whose names in the code are
prefixed with the string **FMTID**.

The sample code shows how to call the IPropertySetStorage::Open method
to extract an IPropertyStorage for each FMTID. The sample code reads
each property from the IPropertyStorage using the
IPropertyStorage::ReadMultiple method.

The actual numeric values of the GUIDs are in the provided sample code.
The four FMTIDs included in this Property Set Storage are:

· FMTID\_EPAPER\_PROPSET\_DOC

· FMTID\_NBDOC\_PROPSET\_DOC

· FMTID\_EPAPER\_PROPSET\_IMAGEGLOBAL

· FMTID\_SummaryInformation

FMTID\_SummaryInformation is a standard Windows FMTID, and you can find
its definition
[here](http://msdn.microsoft.com/en-us/library/aa380376\(VS.85\).aspx).

The following information concerns the other three FMTIDs.

These <span id="_GoBack"></span>are the properties in
FMTID\_EPAPER\_PROPSET\_DOC

**Property ID**

</div>

</div>

**Type**

**Meaning**

0x0002

VT\_UI4

This value exists for backward compatibility with earlier products.
Current implementations of MODI always write 0x0000, and so should any
third-party producer of these files.

0x0006

VT\_UI2

The Unicode character to be used when representing text that has been
rejected by the OCR system, for example when coping from the MODI viewer
and pasting into a plain text editor such as Notepad.

0x000F

VT\_UI4

The number of errors found during the OCR process.

0x0010

VT\_UI4

The quality of the OCR result, from 0 to 1000. Higher numbers indicate
higher confidence.

0x0014

VT\_UI4

The LCID under which OCR was performed. You can find a list of LCID
constants [here](http://msdn.microsoft.com/en-us/library/aa912040.aspx).

0x0015

VT\_UI4

Number of paragraphs of text

0x0016

VT\_UI4

Number of lines of text

0x0017

VT\_UI4

Word count (including recognized and unrecognized words)

0x0018

VT\_UI4

Character count (including recognized and unrecognized characters)

0x001B

VT\_UI4

The major revision of the software that generated this file. MODI always
writes 0x0002.

0x001C

VT\_UI4

The minor revision of the software that generated this file. MODI always
writes 0x0000.

0x001E

VT\_STORAGE

This storage contains the contents of the File-Properties dialog from
the Microsoft Office Document Imaging application. See the sample code
functions PstgPropSetFromProp and DbgDumpStandardProps for an example of
how to extract this property set and read it.

0x01F

VT\_UI4

Status of OCR in this file. Must have one of the following values:

0: OCR was not done

1: OCR was completed

2: OCR was attempted

3: OCR found no text

The following are the properties of FMTID\_NBDOC\_PROPSET\_DOC

**Property ID**

**Type**

**Meaning**

2

VT\_BLOB

This structure is defined in the sample code, see the function
HrReadStationaryData and the structure called NB\_STATIONERY\_DATA.

3

VT\_R4

Page width, in inches

4

VT\_R4

Page height, in inches

5

VT\_UI2

Language of the document, found
[here](http://msdn.microsoft.com/en-us/library/aa912040.aspx).

6

VT\_UI4

The major revision of the software that generated this file. MODI always
writes 0x0002.

7

VT\_UI4

The minor revision of the software that generated this file. MODI always
writes 0x0000.

The following are the properties of FMTID\_EPAPER\_PROPSET\_IMAGEGLOBAL

**Property ID**

**Type**

**Meaning**

2

VT\_ISTREAM

This stream contains a dictionary that maps images used during OCR to
the text it was recognized as.

3

VT\_ISTREAM

This stream contains embedded fonts.
