<div id="page">

# Notes on sparse files and file sharing

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
11/5/2010 12:42:00 PM

-----

<div id="content">

Sparse files are generally referred to as files, often large, that
contain only a small amount of nonzero data relative to their size. In
this blog, I would like to chat about sparse files on Windows operating
systems and the related SMB commands. From an inter-operability
perspective, I intend to discuss:

\- implications of a SMB file server reporting sparse files support;

\- considerations about a client manipulating sparse files;

\- zero filling NTFS files;

\- related Windows APIs and SMB / SMB2 commands.

A sparse file optimizes disk space consumption in the extent that large
ranges of zeroes do not require allocation on the volume. Once data is
written to a sparse file, the needed space for nonzero portions, i.e.
sparse data sets, will be allocated whereas portions that an application
designates as empty will not necessitate space allocation. The
information about the empty ranges is kept in the file metadata and
suffices to rebuild those zeros ranges.

From SMB perspective, the logic of file sparseness resides in the object
store. Related SMB commands translate directly into down-level calls to
the file system, namely NTFS for Windows-based sparse files. 

In the rest of this blog, I will introduce FILE\_SUPPORTS\_SPARSE\_FILES
and FILE\_ATTRIBUTE\_SPARSE\_FILE. Then, I will look at the file system
from the user perspective, and touch on zero filling NTFS files.
Finally, I will provide some relevant Windows APIs and SMB commands on
this topic.

  

## FILE\_SUPPORTS\_SPARSE\_FILES

 

The FILE\_SUPPORTS\_SPARSE\_FILES bit flag \[MS-SMB\] in the file system
attributes indicates whether a file system supports sparse files. If the
server sets the FILE\_SUPPORTS\_SPARSE\_FILES flag in the
SmbTrans2QueryFsInformation response (or the equivalent SMB2, see table
below), the client will expect the support of sparse files. The client
will then expect to issue requests such as FSCTL\_SET\_SPARSE,
FSCTL\_SET\_ZERO\_DATA (See \[MS-FSCC\]).

One question that arises here is whether a given SMB server could
advertise sparse file support even though the underlying file system
does not support sparse files. Technically, the server could, but there
is at least one major issue in doing so, which is excess disk space
consumption; nonetheless in general, file operations should work
correctly.

It is important to note that all Windows-based file systems properly
report file sparseness support.

 

## FILE\_ATTRIBUTE\_SPARSE\_FILE

 

The FSCTL\_SET\_SPARSE IOCTL (see \[MS-FSCC\]) is used to mark a given
file as sparse or not sparse. When a file system that supports sparse
files, i.e. NTFS, receives an FSCTL\_SET\_SPARSE IOCTL request on a
file, it should set or unset the FILE\_ATTRIBUTE\_SPARSE\_FILE attribute
of the specified file.

If you attempt to clear the FILE\_ATTRIBUTE\_SPARSE\_FILE attribute, the
operation is valid only if there is no sparse region in the file. The
FSCTL\_QUERY\_ALLOCATED\_RANGES (see \[MS-FSCC\]) control code is used
to determine whether there are sparse regions in a file.

In Windows NTFS file systems, files are not made sparse by default. The
application or user needs to explicitly mark the file sparse via the
FSCTL\_SET\_SPARSE control code.  Unlike Unix or Linux file systems,
when a file is created on Windows and some data is written at an offset,
let assume at 1 MB offset, NTFS will allocate clusters and fill in all
space from 0 to 1 MB.  This means, the file has to be explicitly marked
sparse before it becomes sparse.

 

## File system user application perspective

 

Users and applications change the sparseness of a file by setting or
clearing the FILE\_ATTRIBUTE\_SPARSE\_FILE flag through the
FSCTL\_SET\_SPARSE control code. From a file system API point of view,
this is done by a DeviceIoControl(FSCTL\_SET\_SPARSE) function call.

File system APIs can be used to copy sparse stream ranges, or query
allocated ranges. Applications then need only to read allocated ranges.

If a file system user copies or moves a sparse file to a volume on a
file system which does not support NTFS sparse file feature, the file
will be built to its originally specified size. This means the required
space needs to be available for the operation to succeed, and the
required space depends on whether or not sparse files are supported on
the destination volume.

It should also be noted that the FILE\_ATTRIBUTE\_SPARSE\_FILE flag
might influence write caching logic at the client side, but this is an
implementation-specific decision; a client may choose to perform only
read caching when a file is sparse. As such, there is no particular
requirement in caching of sparse files.

 

## FSCTL\_QUERY\_ALLOCATED\_RANGES

 

An application can query a sparse file to identify its allocated ranges
by using the FSCTL\_QUERY\_ALLOCATED\_RANGES control code (see MS-FSCC).
This control code returns zero or more FILE\_ALLOCATED\_RANGE\_BUFFER
data elements. Each data element corresponds to an allocated range and
contains a file offset and a length.

This allows an application to read non-zeroed ranges in a sparse stream.
When ranges are copied to a sparse file, the NTFS file system will take
care of the rest. Note that if the application simply reads a sparse
file without explicitly reading allocated ranges, NTFS will fill-in the
zeroed ranges.

 

## Zero filling NTFS files

 

Zero filling NTFS files is a common question when it comes to sparse
files and inter-operability.

Let's assume a scenario where the non-Windows client (e.g. Solaris
client) opens a file on a SMB share, then sets the file size, let's say
1 GB.

If the file was created on a Solaris CIFS server, the file would have
been sparse by default, a read returns zeroes.

Let's assume sparseness is not considered. If the client sets the size
of a file on a Windows-based SMB share, the ranges/clusters/blocks will
be allocated and reserved.

Windows will consistently return zeroes when the client reads the file.
This is covered in the algorithms described in the MS-FSA document
(Sections 3.1.1.5 Per Stream, and 3.1.5.2 Application Requests a Read).
NTFS guarantees to never expose the previous contents of a sector that
is allocated to a file.

In the context where a file is not marked as sparse, the NTFS object
store maintains a persistent attribute called ValidDataLength (VDL) for
the valid data in the stream. VDL is a high water mark of where valid
data has been written to the file. Any read between VDL and EOF is
returned as zeroes. This is similar to the concept of physical file size
and logical file size variables maintained in Unix style file systems. 

If the NTFS file is not marked sparse, when a user creates a file and
sets EOF to 1 GB, the VDL is still at zero so any read on the file
(between VDL = 0 and EOF) will return zeroes. If the user writes 1 KB
data at offset 99 KB, NTFS writes zeroes from 0 to 99 KB, writes the 1
KB data and updates the VDL to 100 KB.

 

## Related Windows APIs and SMB commands

 

### Determining if a file system supports sparse files

This is done by calling GetVolumeInformation() and then by checking if
the volume flags has the FILE\_SUPPORTS\_SPARSE\_FILES bit set.

 

### Marking a file as sparse

This can be done by calling DeviceIoControl() with the
FSCTL\_SET\_SPARSE file system control code.

 

### Determining file sparseness

One way to achieve this is to call GetFileInformationByHandle() and
check the FILE\_ATTRIBUTE\_SPARSE\_FILE of the file attributes.
GetFileInformationByHandle() works for default or alternate streams of a
files. If you detect one sparse stream, then the file is sparse.

 

### Determining allocated ranges

This can be done by calling DeviceIoControl() with the
FSCTL\_QUERY\_ALLOCATED\_RANGES control code. This is used to scans a
file or alternate stream looking for ranges that contain nonzero data.

 

The following table summarizes related SMB and SMB2 commands.

 

Windows API

</div>

</div>

SMB commands

SMB2 commands

GetVolumeInformation()

SMB Transact2, Query FS Info, Query FS Volume Info (NT)

SMB Transact2, Query FS Info, Query FS Attribute Info (NT)

SMB2 QUERY INFORMATION (0x10), Class=Query FS Volume Info

SMB2 QUERY INFORMATION (0x10), Class=Query FS Attribute Info

DeviceIoControl() call with the FSCTL\_SET\_SPARSE control code

SMB Nt Transact, NT\_TRANSACT\_IOCTL, FunctionCode: 0x000900C4 -
FSCTL\_SET\_SPARSE

SMB2 IOCTL (0xb), CtlCode: 0x000900C4 - FSCTL\_SET\_SPARSE

GetFileInformationByHandle()

SMB Transact2, Query File Info, Query File Basic Info

SMB2 QUERY INFORMATION (0x10), Class=FileNetworkOpenInformation (34)

DeviceIoControl() call with the FSCTL\_QUERY\_ALLOCATED\_RANGES control
code

SMB Nt Transact, NT\_TRANSACT\_IOCTL, FunctionCode: 0x000940CF -
FSCTL\_QUERY\_ALLOCATED\_RANGES

SMB2 IOCTL (0xb), CtlCode: 0x000940CF - FSCTL\_QUERY\_ALLOCATED\_RANGES

 

The following network trace snippets were captured between a Windows 7
client and Windows Server 2008 R2.

 

### <span style="text-decoration: underline;">SMB2 trace snippets</span>

 

\- SMB2: C   QUERY INFORMATION (0x10), Class=Query FS Volume Info,
FID=0xFFFFFFFF0000000D(temp@\#7231) 

     InfoType: SMB2\_0\_INFO\_FILESYSTEM - File system information is
requested.

     FileInfoClass: Query FS Volume Info

 

\- SMB2: R   QUERY INFORMATION (0x10), File=temp\\testme.txt@\#7231 

  - RQueryInfo:

   - FileSystemInfo:

    - VolumeInformation:

       VolumeCreationTime: 04/09/2010, 22:41:53.761513 UTC

       VolumeSerialNumber: 1826342482 (0x6CDBC652)

       VolumeLabelLength: 0 (0x0)

       SupportsObjects: TRUE

       Reserved: 0 (0x0)

 

\- SMB2: C   QUERY INFORMATION (0x10), Class=Query FS Attribute Info,
FID=0xFFFFFFFF0000000D(temp@\#7231) 

     InfoType: SMB2\_0\_INFO\_FILESYSTEM - File system information is
requested.

     FileInfoClass: Query FS Attribute Info

 

\- SMB2: R   QUERY INFORMATION (0x10), File=temp@\#7231 

  - RQueryInfo:

    - FileSystemInfo:

    - AttributeInformation: FileSystemName NTFS

     - FileSystemAttributes:

      - FSCCFileSystemAttribute: 13041919 (0xC700FF)

         SUPPORTS\_SPARSE\_FILES:   (.........................1......)
supports sparse files.

 

\- SMB2: C   IOCTL (0xb), FID=0xFFFFFFFF00000011

  - CIoCtl:

     CtlCode: 0x000900C4 - FSCTL\_SET\_SPARSE - Marks the indicated file
as sparse or not sparse.

   + FileId: Persistent: 0x1000000155, Volatile: 0xFFFFFFFF00000011

     Flags: (00000000000000000000000000000001) FSCTL request

 

\- SMB2: C   QUERY INFORMATION (0x10), Class=FileNetworkOpenInformation
(34), FID=0xFFFFFFFF00000011(temp\\testme.txt@\#7238) 

     InfoType: SMB2\_0\_INFO\_FILE - File information is requested.

     FileInfoClass: FileNetworkOpenInformation (34)

   + FileId: Persistent: 0x1000000155, Volatile: 0xFFFFFFFF00000011

 

\- SMB2: R   QUERY INFORMATION (0x10), File=temp\\testme.txt@\#7238  

   - FileInfo:

    - FileNetworkOpenInformation:

     - FileAttributes:

      - FSCCFileAttribute: 544 (0x220)

             Sparse:             (......................1.........)
Sparse

        

\- SMB2: C   IOCTL (0xb), FID=0xFFFFFFFF00000011

  - CIoCtl:

     CtlCode: 0x000940CF - FSCTL\_QUERY\_ALLOCATED\_RANGES - Scans a
file or alternate stream looking for ranges that contain nonzero data.

   + FileId: Persistent: 0x1000000155, Volatile: 0xFFFFFFFF00000011

     Flags: (00000000000000000000000000000001) FSCTL request

   - QueryAllocatedRanges: FileOffset: 0, Length: 841

 

\- SMB2: R   IOCTL (0xb), File=temp\\testme.txt@\#7238 

  - RIoCtl:

     CtlCode: 0x000940CF - FSCTL\_QUERY\_ALLOCATED\_RANGES - Scans a
file or alternate stream looking for ranges that contain nonzero data.

   + FileId: Persistent: 0x1000000155, Volatile: 0xFFFFFFFF00000011

     InputOffset: 112 (0x70)

     InputCount: 16 (0x10)

     OutputOffset: 128 (0x80)

     OutputCount: 16 (0x10)

     Flags: 0 (0x0)

     Reserved2: 0 (0x0)

     InputData: Binary Large Object (16 Bytes)

     QueryAllocatedRanges:

    padding: Binary Large Object (16 Bytes)

 

### <span style="text-decoration: underline;">SMB trace snippets</span>

 

\- Smb: C; Transact2, Query FS Info, Query FS Volume Info (NT)

    Command: Transact2 50(0x32)

     SubCommand: Query FS Info, 3(0x0003)

   - QueryFSInfoParameterBlock:

      QueryInfoLevel: Query FS Volume Info (NT)

 

\- Smb: R; Transact2, Query FS Info, Query FS Volume Info (NT)

    Command: Transact2 50(0x32)

   - QueryFSInfoDataBlock: Query FS Volume Info (NT)

      CreationTime: 04/09/2010, 22:41:53.761513 UTC

      VolumeSerialNumber: 1826342482 (0x6CDBC652)

      VolumeLabelLength: 0 (0x0)

      SupportsObjects: TRUE

    + VolumeLabel:

 

\- Smb: C; Transact2, Query FS Info, Query FS Attribute Info (NT)

    Command: Transact2 50(0x32)

     SubCommand: Query FS Info, 3(0x0003)

   - QueryFSInfoParameterBlock:

      QueryInfoLevel: Query FS Attribute Info (NT)

 

\- Smb: R; Transact2, Query FS Info, Query FS Attribute Info (NT), FS =
NTFS

    Command: Transact2 50(0x32)

  - RTransaction2:

   - QueryFSInfoDataBlock: Query FS Attribute Info (NT)

    - FileSystemAttributes: 13041919 (0xC700FF)

       SupportSparseFile:        (.........................1......) File
supports sparse files (FILE\_SUPPORTS\_SPARSE\_FILES)

      FileSystemNameLength: 8 (0x8)

    + FSName: NTFS

 

\- Smb: C; Nt Transact, NT\_TRANSACT\_IOCTL, FID = 0x4000
(\\temp\\testme.txt@\#7362)

    Command: Nt Transact 160(0xA0)

  - CNTTransaction:

     Function: NT\_TRANSACT\_IOCTL

   - IOCTLSetupWords:

      FunctionCode: 0x000900C4 - FSCTL\_SET\_SPARSE - Marks the
indicated file as sparse or not sparse., 590020 (0x900C4)

      FileID: 16384 (0x4000)

      IsFsctl: File system control command

    + IsFlag: 0 (0x0)

     ByteCount: 3 (0x3)

     Pad1: Binary Large Object (3 Bytes)

 

\- Smb: R; Nt Transact, NT\_TRANSACT\_IOCTL, FID = 0x4000
(\\temp\\testme.txt@\#41)

    Command: Nt Transact 160(0xA0)

  - RNTTransaction:

   - SetupWords:

      SetupWords: 0 (0x0)

     ByteCount: 3 (0x3)

     Pad1: Binary Large Object (3 Bytes)

 

\- Smb: C; Transact2, Query File Info, Query File Basic Info, FID =
0x4000 (\\temp\\testme.txt@\#7362)

    Command: Transact2 50(0x32)

  - CTransaction2:

     SubCommand: Query File Info, 7(0x0007)

     ByteCount: 7 (0x7)

     Pad1: Binary Large Object (3 Bytes)

   - QueryFileInfoParameterBlock:

      FileID: 16384 (0x4000)

      FileInfoLevel: Query File Basic Info

 

\- Smb: R; Transact2, Query File Info, Query File Basic Info, FID =
0x4000 (\\temp\\testme.txt@\#7362)

    Command: Transact2 50(0x32)

  - RTransaction2:

   - QueryFileInfoParameterBlock:

   - FileBasicInfo:

      CreationTime: 10/29/2010, 20:34:24.683222 UTC

      AccessTime: 10/29/2010, 20:34:24.683222 UTC

      LastWriteTime: 08/30/2005, 18:42:50.000000 UTC

      LastChangeTime: 10/29/2010, 20:34:24.683222 UTC

    - Attributes: 0x0220

       SparseFile:         (......................1.........) Sparse
file (FILE\_ATTRIBUTE\_SPARSE\_FILE)

 

\- Smb: C; Nt Transact, NT\_TRANSACT\_IOCTL, FID = 0x4000
(\\temp\\testme.txt@\#7362)

    Command: Nt Transact 160(0xA0)

  - CNTTransaction:

     Function: NT\_TRANSACT\_IOCTL

   - IOCTLSetupWords:

      FunctionCode: 0x000940CF - FSCTL\_QUERY\_ALLOCATED\_RANGES - Scans
a file or alternate stream looking for ranges that contain nonzero
data., 606415 (0x940CF)

      FileID: 16384 (0x4000)

      IsFsctl: File system control command

    + IsFlag: 0 (0x0)

     ByteCount: 19 (0x13)

     Pad1: Binary Large Object (3 Bytes)

   - QueryAllocatedRanges: FileOffset: 0, Length: 841

      FileOffset: 0 (0x0)

      Length: 841 (0x349)

 

\- Smb: R; Nt Transact, NT\_TRANSACT\_IOCTL, FID = 0x4000
(\\temp\\testme.txt@\#41)

    Command: Nt Transact 160(0xA0)

  - RNTTransaction:

     WordCount: 19 (0x13)

     Reserved: 0 (0x0)

     TotalParameterCount: 0 (0x0)

     TotalDataCount: 16 (0x10)

     ParameterCount: 0 (0x0)

     ParameterOffset: 76 (0x4C)

     ParamDisplacement: 0 (0x0)

     DataCount: 16 (0x10)

     DataOffset: 76 (0x4C)

     DataDisplacement: 0 (0x0)

     SetupCount: 1 (0x1)

   - SetupWords:

      SetupWords: 16 (0x10)

     ByteCount: 19 (0x13)

     Pad1: Binary Large Object (3 Bytes)

     QueryAllocatedRanges:

     Pad: Binary Large Object (16 Bytes)

 

## References

 

 

\[MS-FSCC\]: File System Control Codes

<http://msdn.microsoft.com/en-us/library/cc231987.aspx>

 

\[MS-SMB\]: Server Message Block (SMB) Protocol Specification

<http://msdn.microsoft.com/en-us/library/cc246231.aspx>

 

\[MS-SMB2\]: Server Message Block (SMB) Version 2 Protocol Specification

<http://msdn.microsoft.com/en-us/library/cc246482.aspx>

\[MS-FSA\]: File System Algorithms

<http://msdn.microsoft.com/en-us/library/ff469524.aspx>

 

Sparse File Operations

<http://msdn.microsoft.com/en-us/library/aa365566.aspx>

 

A File System for the 21st Century

<http://www.microsoft.com/msj/1198/ntfs/ntfs.aspx>
