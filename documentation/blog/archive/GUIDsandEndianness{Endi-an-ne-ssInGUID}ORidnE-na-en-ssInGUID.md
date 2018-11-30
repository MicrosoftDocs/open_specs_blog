<div id="page">

# GUIDs and Endianness: {Endi-an-ne-ssInGUID} OR idnE-na-en-ssInGUID?

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
10/8/2013 10:17:00 AM

-----

<div id="content">

 

Hi all\!

I have recently received a couple inquiries regarding the  
way in which GUIDs are represented, how they are stored, how they are  
transferred over the wire and how endianness impacts on them so I
decided to  
post a little blog entry to share a couple details and examples.

GUIDs are described in [\[MS-DTYP\] Section  
2.3.4](http://msdn.microsoft.com/en-us/library/cc230326.aspx) and that’s
where the 3 representations for GUIDs are explained as  
follows:

 

<span style="text-decoration: underline;">RPC IDL Representation:</span>

<div>

*typedef struct {*

*  unsigned long Data1;*

*  unsigned short Data2;*

*  unsigned short Data3;*

*  byte Data4\[8\];*

*} GUID,*

* UUID,*

* \*PGUID;*

* *

</div>

*Data1: * *This member is generally treated as an opaque value. This
member is  
equivalent to the time\_low field of a DCE UUID
(*[*\[C706\]*](http://go.microsoft.com/fwlink/?LinkId=89824) *section
A.1).*

*Data2: * *This member is generally treated as an opaque value. This
member is  
equivalent to the time\_mid field of a DCE UUID
(*[*\[C706\]*](http://go.microsoft.com/fwlink/?LinkId=89824) *section
A.1).*

*Data3: * *This member is generally treated as an opaque value. This
member is  
equivalent to the time\_hi\_and\_version field of a DCE UUID
(*[*\[C706\]*](http://go.microsoft.com/fwlink/?LinkId=89824) *section
A.1).*

*Data4: * *This array is generally treated as a sequence of opaque
values. This  
member is equivalent to the following sequence of fields of a DCE UUID
(*[*\[C706\]*](http://go.microsoft.com/fwlink/?LinkId=89824) *section
A.1) in  
this order: clock\_seq\_hi\_and\_reserved, clock\_seq\_low, and the
sequence of bytes  
in the node field.*

 

 

<span style="text-decoration: underline;">Packet Representation:</span>

*Data1  
(4 bytes): * *The value of the* *Data1* *member (section*
[*2.3.4*](#z4926e530816e41c2b251ec5c7aca018a)*), in little-endian byte
order.*

*Data2  
(2 bytes): * *The value of the* *Data2* *member (section*
[*2.3.4*](#z4926e530816e41c2b251ec5c7aca018a)*), in little-endian byte
order.*

*Data3  
(2 bytes): * *The value of the* *Data3* *member (section*
[*2.3.4*](#z4926e530816e41c2b251ec5c7aca018a)*), in little-endian byte
order.*

*Data4  
(8 bytes): * *The value of the* *Data4* *member (section*
[*2.3.4*](#z4926e530816e41c2b251ec5c7aca018a)*), in little-endian byte
order.*

** 

** 

 

<span style="text-decoration: underline;">Curly Braced String
Representation:</span>

*{f81d4fae-7dec-11d0-a765-00a0c91e6bf6}*

 

 

If you are using GUIDs for your own client/server  
application development, you can transfer/store them in any way, shape
or form  
since you are in charge of the encoding/decoding at both ends and you
can  
manipulate the data with your own rules.

It is always a good idea to follow standards but that’s a  
personal call that you, as the developer, have to make.

However, if you are planning to interoperate with other  
systems, you need to be aware of the representation that different
protocols  
and implementations are using when dealing with GUIDs.

Because of that, things may get a little confusing when  
storing those GUIDs into a file or transferring them through the network
since  
the endianness varies depending on the GUID representation being used.

 

These examples should help you out in representing the GUIDs  
in the right way:

 

<span style="text-decoration: underline;">When using “Curly Braced
String Representation”:</span>

*Strings are not  
affected by endianness since they are represented as an array of
individual  
bytes.*

* *

Big endian: {f81d4fae-7dec-11d0-a765-00a0c91e6bf6}

Little endian: {f81d4fae-7dec-11d0-a765-00a0c91e6bf6}

 

 

<span style="text-decoration: underline;">When using “RPC IDL
Representation” OR “Packet  
Representation”:</span>

*As GUIDs in these  
representations are composed of 4 different data elements, we need to
be  
careful and treat each one accordingly.*

* Data1 (4 bytes)*

*Data2 (2 bytes)*

*Data3 (2 bytes)*

*Data4 (array of \[8\]  
one byte elements)*

 

Big endian: f81d4fae-7dec-11d0-a765-00a0c91e6bf6

Little endian: ae4f1df8-ec7d-d011-a765-00a0c91ebf6

 

Note that each data element is treated individually.

Data1 being 4 bytes long goes from Byte1Byte2Byte3Byte4 to  
Byte4Byte3Byte2Byte1.

Data2 being 2 bytes long goes from Byte1Byte2 to Byte2Byte1.

Data3 being 2 bytes, is treated in the same way as Data2.

Data4 stays unaltered as it is represented as an array of 8  
individual bytes.

 

 

I hope that these examples can save you some time and debugging  
efforts\!

\*//

</div>

</div>
