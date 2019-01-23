<div id="page">

# BFFValidator Tool Goes Public

[tomjebo](https://social.msdn.microsoft.com/profile/tomjebo) 7/13/2011
6:54:00 AM

-----

<div id="content">

Yesterday, the Office Interoperability team announced the public release
of the Beta version of the Microsoft Office Binary File Format
Validator, also known as BFFValidator. You can read the [official
announcement](http://blogs.msdn.com/b/officeinteroperability/archive/2011/07/12/microsoft-office-binary-file-format-validator-is-now-available.aspx)
from the Office Interoperability team.

# What is BFFValidator?

A project in the Microsoft Office team that's been brewing for more than
a couple of years now, BFFValidator is based on a technology called
Gatekeeper. This technology is a homegrown system that uses rules
derived directly from the Office source and Open Specification
descriptions and grammars to create parsing code that can recognize
conformant files accepted by Microsoft Word, Excel and PowerPoint (file
types of .doc, .xls and .ppt, respectively), from current versions back
to Office 97. 

# How do I get it?

The BFFValidator tool can be downloaded from the [Microsoft Download
Center](http://www.microsoft.com/download/en/details.aspx?id=26794).

# How do I use it?

It's a lightweight, standalone command line utility with only three
accompanying dll's and very simple to use. Here's the help output from
the
tool:

<span style="font-family: courier new,courier; font-size: xx-small;">C:)\>BFFValidator.exe
/?</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Microsoft
Office Binary File Format Validator
(BETA)</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Copyright
(C) 2011 Microsoft. All rights reserved.</span>

<span style="font-family: courier new,courier; font-size: xx-small;">For
more information, visit
[http://go.microsoft.com/fwlink/?LinkId=209766](http://go.microsoft.com/fwlink/?LinkId=209766Usage)</span>

<span style="font-family: courier new,courier; font-size: xx-small;">Usage</span>

<span style="font-family: courier new,courier; font-size: xx-small;">bffvalidator.exe
\[-l log.xml\] \[-v\] filename.ext</span>

<span style="font-family: courier new,courier; font-size: xx-small;">   
-l log.xml    Optional. Specifies the location and file name for
the</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
BFFValidator log file.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
-v            Optional. The BFFValidator will display each byte that
is</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
read to the Output window. If the validated file
is</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
large, this will be time-consuming. Used
for</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
debugging.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
Filename.ext            Specifies the filename and the extension of
the</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
file to be validated.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
-?                      Displays the command syntax and options
for</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">                           
BFFValidator.exe.</span>

As you can see, there are very few options. It takes a file
specification, a "-v" option which dumps the contents out as they are
parsed, a "-?" that gives you this help information and the "-l" option
that allows you to specify the log file. This log file is where you will
find the critical information that helps you identify exactly what is
non-conforming about the input file. I recommend that if you use the
"-v" option, you pipe or redirect the output to a file. As the help
indicates, the output with this option can be prolific.

As I said, it's standalone and lightweight. You only need the following
files to validate a
file:

<span style="font-family: courier new,courier; font-size: xx-small;">C:)\>dir</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> Volume
in drive C is
OS</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> Volume
Serial Number is
F034-2612</span>

<span style="font-family: courier new,courier; font-size: xx-small;">Directory
of
C:\\bin\\BFFValidatorx64</span>

<span style="font-family: courier new,courier; font-size: xx-small;">07/12/2011 
11:07 AM    \<DIR\>         
.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/12/2011 
11:07 AM    \<DIR\>         
..</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/06/2011 
07:00 PM         1,210,880
BFFValidator.exe</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/06/2011 
07:00 PM            72,192
BFFWrapper.dll</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/06/2011 
07:12 PM         4,694,016
docValidation.dll</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/06/2011 
07:06 PM         4,177,920
pptValidation.dll</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">07/06/2011 
07:06 PM         6,482,944
xlsValidation.dll</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">              
5 File(s)     16,637,952
bytes</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">              
2 Dir(s)  891,890,511,872 bytes free</span>

# How do I report a problem or submit questions?

If you have a problem with or questions about the BFFValidator tool or a
file that it's attempting to validate, there are two channels you can
use to get assistance. Either email <dochelp@microsoft.com> or post your
query on the [Open Specifications Office File Format
forum](http://social.msdn.microsoft.com/forums/en-US/os_binaryfile/threads/).
To receive the quickest, most thorough assistance, please be as
descriptive as possible about not only what you are observing (i.e. the
problem) but also what exactly you want to see happen (for example, "I
think BFFValidator should check the value of bit A in the struct
described section 2.3.10 even though the description says 'ignore'
because Excel actually uses this bit…"). When sending to
<dochelp@microsoft.com>, you can also attach files so attach the sample
input file (e.g. testfile.xls) and the output log file from the run.

# Example output

Here's some sample output to give you an idea of what to
expect.

### Validating a passing .xls file:

<span style="font-family: courier new,courier; font-size: xx-small;">C:)\>BFFValidator.exe
-l c:\\cases\\testxls\\log.xml -v c:\\cases\\testxls\\test.xls |
more</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Opening Stream
'/?SummaryInformation'.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Opening Stream
'/?DocumentSummaryInformation'.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Opening Stream
'/Workbook'.</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x0 cb=0x2 Reading...
0xfe
0xff</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x2 cb=0x2 Reading...
0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x4 cb=0x4 Reading...
0x06 0x01 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x18 cb=0x4 Reading...
0x01 0x00 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x18 cb=0x4 Reading...
0x01 0x00 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x2c cb=0x4 Reading...
0x30 0x00 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x0 cb=0x2 Reading...
0xfe
0xff</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x2 cb=0x2 Reading...
0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '/?SummaryInformation' pos=0x4 cb=0x4 Reading...
0x06 0x01 0x02 0x00</span>

<span style="font-family: courier new,courier; font-size: xx-small;">…
\<lots more output due to –v option\>
…</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34e9 cb=0x2 Reading... 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34eb cb=0x8 Reading... 0x00 0x00 0x00 0x00
0x00 0x00 0x00
0x00</span>

<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f3 cb=0x2 Reading... 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f5 cb=0x1 Reading...
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34e7 cb=0xc Reading... 0x67 0x08 0x00 0x00
0x00 0x00 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x00
0x00 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f3 cb=0x2 Reading... 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f5 cb=0x1 Reading...
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f3 cb=0x2 Reading... 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34fa cb=0x4 Reading... 0x03 0x44 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f3 cb=0x2 Reading... 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34fa cb=0x4 Reading... 0x03 0x44 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34f3 cb=0x2 Reading... 0x02
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x34fa cb=0x4 Reading... 0x03 0x44 0x00
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">BFFValidator:
"c:\\cases\\testxls\\test.xls" PASSED at 07/12/11 11:25:14</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Log
at: c:\\cases\\testxls\\log.xml</span>

The log.xml looks like
this:

<span style="font-family: courier new,courier; font-size: xx-small;">\<BFFValidation
path="c:\\cases\\testxls\\test.xls" datetime="07/12/11 11:25:14"
result="PASSED"\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">\</BFFValidation\></span>

### Here's a sample failing .xls file:

The initial verbose output will look similar and then you'll
see:

<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3da
cb=0x0</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3fc cb=0x4 Reading... 0x1e 0x04 0x1c
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3da cb=0x22 Reading... 0x31 0x00 0x1e 0x00
0x13 0x00 0x04
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x08
0x00 0x90 0x01 0x00 0x00 0x01 0x02 0x00 0xa2 0x07 0x01 0x43 0x00 0x61
0x00 0x6c 0x00 0x69
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x62
0x00 0x72 0x00 0x69
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3de cb=0x1e Reading... 0x13 0x00 0x04 0x00
0x08 0x00 0x90
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x00
0x00 0x01 0x02 0x00 0xa2 0x07 0x01 0x43 0x00 0x61 0x00 0x6c 0x00 0x69
0x00 0x62 0x00 0x72
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x69
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3de cb=0x1e Reading... 0x13 0x00 0x04 0x00
0x08 0x00 0x90
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x00
0x00 0x01 0x02 0x00 0xa2 0x07 0x01 0x43 0x00 0x61 0x00 0x6c 0x00 0x69
0x00 0x62 0x00 0x72
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 0x69
0x00</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3de cb=0xe Reading... 0x13 0x00 0x04 0x00
0x08 0x00 0x90
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">0x00
0x00 0x01 0x02 0x00
0xa2</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">BFFValidator:
"c:\\cases\\testxls\\test.xls" FAILED at 07/12/11 11:58:26</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">Log
at:
c:\\cases\\testxls\\log.xml</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">See:
<http://msdn.microsoft.com/en-us/library/291A910C-CB69-4799-875E-A201845D4FD1>
for more information</span>

As you can see, the file validation failed on the byte at position
0x3de. The log.xml shows us the
reason:

<span style="font-family: courier new,courier; font-size: xx-small;">\<BFFValidation
path="c:\\cases\\testxls\\test.xls" datetime="07/12/11 11:58:26"
result="FAILED"\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 
\<ParseStack\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type builtinType="Docfile" docName="MS-XLS" sectionTitle="Compound
File"
msdnLink="<http://msdn.microsoft.com/en-us/library/b91df1c9-6bfa-4ab4-8218-7bb0d73624ca>"\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">     
\<Info\>Built-in type "Docfile": The root storage object of an OLE
compound file. For more information, see
[http://msdn.microsoft.com/en-us/library/dd942138.aspx.\</Info](http://msdn.microsoft.com/en-us/library/dd942138.aspx.%3C/Info)\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\</Type\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type builtinType="Stream" docName="MS-XLS" sectionTitle="Stream"
msdnLink="<http://msdn.microsoft.com/en-us/library/f67ac5ed-b0a7-4b2c-9b7a-28933eeaac7e>"
streamName="Workbook" streamOffset="0"
hexStreamOffset="0x0"\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">     
\<Info\>Built-in type "Stream": Any stream object for OLE compound
files. The entire file contents for other files.\</Info\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\</Type\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type docName="MS-XLS" sectionTitle="Substream"
msdnLink="<http://msdn.microsoft.com/en-us/library/dd947025(v=office.12).aspx>"
streamName="Workbook" streamOffset="20"
hexStreamOffset="0x14"/\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type docName="MS-XLS" sectionTitle="Record"
msdnLink="<http://msdn.microsoft.com/en-us/library/170e90ce-87d7-4758-9331-dcf14cd72388>"
streamName="Workbook" streamOffset="986"
hexStreamOffset="0x3da"/\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type docName="MS-XLS" sectionTitle="Font" sectionNumber="2.4.122"
msdnLink="<http://msdn.microsoft.com/en-us/library/291A910C-CB69-4799-875E-A201845D4FD1>"
streamName="Workbook" streamOffset="990"
hexStreamOffset="0x3de"/\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\<Type builtinType="USHORT" streamName="Workbook"
bitOffsetWithinStruct="0" hexBitOffsetWithinStruct="0x0" bitCount="16"
streamOffsetOfStruct="990" hexStreamOffsetOfStruct="0x3de"
streamOffset="990" hexStreamOffset="0x3de" childId="1"
hexChildId="0x1"\>
</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">     
\<Info\>Built-in type "USHORT": Unsigned 2-byte
integer.\</Info\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">   
\</Type\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 
\</ParseStack\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;"> 
\<LastData\>\<\!\[CDATA\[</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">13
00 04 00 08 00 90 01  00 00 01 02 00 A2 -- -- 
..............</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">\]\]\>\</LastData\></span>  
<span style="font-family: courier new,courier; font-size: xx-small;">\</BFFValidation\></span>

Notice that BFFValidator was reading the stream named "Workbook" and was
reading a "Font" record. It also shows us the section in MS-XLS which
specifies this record (2.4.122) and provides a URL to get to the
[structure definition in MSDN's Open Specfications
online](http://msdn.microsoft.com/en-us/library/291A910C-CB69-4799-875E-A201845D4FD1).
It then goes on to identify the field within the record, the offsets
into the record, the bit count and length etc… If we look at the record
in MS-XLS section 2.4.122, we see that childId 1 is the first field.
It's indeed two bytes in length. However, the value in the field
is:

<span style="font-family: courier new,courier; font-size: xx-small;">Verbose:
Reading from stream '' pos=0x3de cb=0xe Reading... 0x13 0x00 0x04 0x00
0x08 0x00 0x90
0x01</span>  
<span style="font-family: courier new,courier; font-size: xx-small;">0x00
0x00 0x01 0x02 0x00 0xa2</span>

Which, when reversed for endianness, is 0x0013 or decimal 19. The
specification clearly states this must be either 0 or between 20 and
8191 (the max for a two byte value).

 

</div>

</div>
