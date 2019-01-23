<div id="page">

# Exploring the CFB File Format 8

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/9/2011 9:52:34 PM

-----

<div id="content">

**Exploring the CFB File Format 8**

*Range Lock Sectors*

A range lock sector is a part of a CFB file that is used for concurrency
and transactions. The range sector will only appear in files greater
than 2 GB. As we have seen

in previous posts, there are files with 512-byte sectors, and others
with 4096-byte sectors. A 512-byte sector CFB file is limited to 2 GB
and \*MUST\* never be larger

than this size. Conversely, a 4096-byte file can grow as large as 16
terabytes. Therefore, only a 4096-byte CFB file

contains a range lock sector and it will ALWAYS appear at file offsets
0x7FFFFF00-0x7FFFFFFF (inclusive).Therefore, is merely only a small feat
to consume the

bytes in question:

  
\# ff bytes are found in the range: 0x7FFFFF00-0x7FFFFFFF (inclusive)  
for i in range(0, 255):  
<span> </span>  cur\_byte = disk\[i\]  
  
It is merely a good idea just to skip this sector as it will not contain
any user-defined data. No other data structure contained in a CFB file
points to this sector.The

information found in this segment will be subject to the producer, and
\*not\* the consumer. The producer will use this for various
transactions when it comes to the

process of obtaining objects. The way this sector is \*actually\* used
is considered an implementation detail and is constructed differently
from app to app as each

one will use this space in a different manner.

 

Please note this sector is \*not\* important when parsing a cfb file, it
is only important for the producer that created the

file. Thus, you could always jump to the offset past the sector in
question:

 

file.seek(0x7FFFFFFF)

 

Subsequently, the producer can continue consuming sectors immediately
following the range lock sector per the FAT.

</div>

</div>
