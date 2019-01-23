<div id="page">

# Troubleshooting with the Microsoft Exchange RPC Extractor (or, the case of the mysterious Inbox sync)

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
6/7/2011 5:01:00 PM

-----

<div id="content">

 

<span style="font-size: x-small;">If you were not already familiar with
decoding Exchange Server to Client communication, or have done so
manually in the past, I present to you
</span>[<span style="font-size: x-small;">The Microsoft Exchange RPC
Extractor</span>](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=7d9a390c-10f2-4cc0-92ea-233b4f2cc6e9)<span style="font-size: x-small;">
(RPX). RPX is a command-line driven tool that parses network captures
and decodes the RPC calls to a Microsoft Exchange Server.</span>

<span style="font-size: x-small;">Perhaps the best way to demonstrate
the RPX tool is to explain how it was used to troubleshoot a real
problem scenario.</span>

<span style="font-size: x-small;">On the Open Specifications team we are
occasionally presented with an issue that requires us to decode the RPC
wire conversation between Exchange Server and a Client (in this case
Outlook). Since Exchange Server communicates with a client via RPC, the
protocol that defines this communication at the transport layer is
</span>[<span style="font-size: x-small;">MS-OXCRPC</span>](http://msdn.microsoft.com/en-us/library/cc425493\(EXCHG.80\).aspx)<span style="font-size: x-small;">.
However, there is much more to it than that. Exchange Server
communication is obfuscated and commands called ROPs (Remote Operations)
are packed into buffers which reside in RPC over TCP traffic.
Furthermore, Exchange Server communicates with a client using several
protocol specifications,
</span>[<span style="font-size: x-small;">MS-OXCROPS</span>](http://msdn.microsoft.com/en-us/library/cc425494\(EXCHG.80\).aspx)<span style="font-size: x-small;">,
</span>[<span style="font-size: x-small;">MS-OXCFXICS</span>](http://msdn.microsoft.com/en-us/library/cc463916\(EXCHG.80\).aspx)<span style="font-size: x-small;">,
</span>[<span style="font-size: x-small;">etc</span>](http://msdn.microsoft.com/en-us/library/cc425499\(EXCHG.80\).aspx)<span style="font-size: x-small;">.
I don’t intend to go into all of the details of the communication
because that’s not the intention of this blog. And, if you are finding
you need to analyze Exchange Server RPC traffic you’re probably at least
familiar with some of those details.</span>

<span style="font-size: x-small;">Decoding Exchange Server RPC
communication traffic manually is an onerous task, especially with a
typical long network capture (many thousands of ROPs packed into many
thousands of buffers). There are tools and techniques for analyzing
Exchange Server connectivity issues, such as “rpcdump”, mentioned here,
</span>[<span style="font-size: x-small;">http://support.microsoft.com/kb/325930</span>](http://support.microsoft.com/kb/325930)<span style="font-size: x-small;">,
and the </span>[<span style="font-size: x-small;">Exchange Remote
Connectivity Analyzer
Tool</span>](http://technet.microsoft.com/en-us/library/dd439364\(EXCHG.80\).aspx)<span style="font-size: x-small;">,
but if your problem involves the analysis of the communication between
Exchange Server and a Client then a highly useful tool to simplify the
analysis is </span>[<span style="font-size: x-small;">The Microsoft
Exchange RPC
Extractor</span>](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=7d9a390c-10f2-4cc0-92ea-233b4f2cc6e9)<span style="font-size: x-small;">
(RPX).</span>

<span style="font-size: x-small;">A recent problem scenario where I
needed to analyze the Exchange Server wire communication may help
illustrate the utility of RPX.</span>

<span style="font-size: x-small;">**The case of the mysterious Inbox
sync:** The customer has a man-in-the-middle solution which
manages/optimizes traffic between an Exchange Server and Outlook clients
and during a client Inbox folder synchronization, after a few messages
are sync’d, the Inbox folder synchronization stops (with many more
messages left to sync) and the sync continues with the next folder.
There is no error message presented to the client. The game is afoot\!
And if I were a smoker this has all the makings of a three-pipe
problem.</span>

<span style="font-size: x-small;">After obtaining network captures
(concurrent client and server) I ran RPX on the network captures
(specifically the client in this example) with the following
command-line options: **rpx capture-file.cap**</span>

<span style="font-size: x-small;">There is nothing remarkable about the
command line above since I’m just taking the default options to operate
on the capture file. If you invoke RPX at the command-line with no
options, and no input file specified, you get the usage output:</span>

<span style="font-size: x-small;">Usage: rpx.exe \<capture file\> \[-L
dump limit\] \[-C\] \[-B\] \[-P\] \[-Q\]  
\<capture file\>: Netmon capture file (wildcards allowed)  
\-L: dump limit is 256 bytes by default, 0 means no limit  
\-C: send messages to console instead of log file  
\-B: save RopReadStream data to disk  
\-P: displays Aux header payloads in binary  
\-Q: quiet mode, suppresses all console output  
</span>

<span style="font-size: x-small;">The default for RPX when decoding is
to dump only the first 256 bytes of the each buffer.</span>

<span style="font-size: x-small;">Before I continue, although using
</span>[<span style="font-size: x-small;">Netmon</span>](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=983b941d-06cb-4658-b7f6-3088333d062f&displaylang=en)<span style="font-size: x-small;">
to capture data with Exchange Server was not intended to be in scope or
the subject of this blog, I should mention a few potential concerns with
capturing the data and what works best with RPX. RPX was designed and
tested with Netmon 3.x capture files. Other capture file formats will
<span style="text-decoration: underline;">not</span> work. In this
scenario I received \*.pcap files and used Netmon to convert them to
\*.cap format. Just a simple “Save-As” in Netmon 3.x. This should work
ok for you depending on the details of the file you’re working with.
Results may vary, so it’s best to capture in Netmon 3.x format. And,
just because you can load/open the capture in Netmon doesn’t necessarily
mean RPX will parse it. For RPX the file must be converted to at least
Netmon 2.x \*.cap format, or higher. If you need more information on
using Netmon 3.x with Exchange Server to capture data, refer to
</span>[<span style="font-size: x-small;">KB
2006508</span>](http://support.microsoft.com/kb/2006508)<span style="font-size: x-small;">
(enable/disable RPC encryption).</span>

<span style="font-size: x-small;">\[Pipe One\]</span>

<span style="font-size: x-small;">So I ran the above RPX command-line
options on my capture files and now I have data for all the ROPs in the
trace and the first 256 bytes of all the buffers to sift through for
what/where the problem could be occurring. Since the problem occurs on
the Inbox synchronization I know I want to look for clues at or around
the last
</span>[<span style="font-size: x-small;">RopFastTransferSourceGetBuffer</span>](http://msdn.microsoft.com/en-us/library/ee160127\(v=EXCHG.80\).aspx)<span style="font-size: x-small;">
(ropFXbuffer) operation on the Inbox folder (all this happens within a
</span>[<span style="font-size: x-small;">FastTransfer
Stream</span>](http://msdn.microsoft.com/en-us/library/ee160399\(EXCHG.80\).aspx)<span style="font-size: x-small;">).
And, since the trace was stopped right after the problem repro I know
this will also be near the end of the trace output. The reason I start
with ropFXbuffer is that this is an Inbox sync, which is
</span>[<span style="font-size: x-small;">Incremental Change
Synchronization</span>](http://msdn.microsoft.com/en-us/library/aa142535\(EXCHG.65\).aspx)<span style="font-size: x-small;">
(ICS), defined in the protocol specification
</span>[<span style="font-size: x-small;">MS-OXCFXICS</span>](http://msdn.microsoft.com/en-us/library/cc463916\(EXCHG.80\).aspx)<span style="font-size: x-small;">,
and data is requested from the server via RopFastTransferSourceGetBuffer
per
</span>[<span style="font-size: x-small;">MS-OXPROTO</span>](http://msdn.microsoft.com/en-us/library/ee178041\(EXCHG.80\).aspx)<span style="font-size: x-small;">.</span>

<span style="font-size: x-small;">Output for RPX of the near-to-last
ropFXbuffer looks like this:</span>  
<span style="font-size: x-small;">(Note, I’ve changed some strings to
protect the
innocent)</span>

<span style="font-family: Courier New; font-size: xx-small;">17:03:01.282   
\#60563    192.168.1.1:45929        192.168.2.2:3984          
TCP/IP     
MSRPC Call ID: 0x73c    RESPONSE: 0 ms    Store RPC</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60563,  
    60564,  
    60566,  
    60567,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60569,  
    60570,  
    60572,  
    60573,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60575,  
    60576,  
    60578,  
    60579,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60581,  
    60582,  
    60584,  
    60585,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60587,  
    60588,  
    60590,  
    60591,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60593,  
    60594,</span>

<span style="font-family: Courier New; font-size: xx-small;">RPC
Coalesced: 31800 bytes</span>

<span style="font-family: Courier New; font-size: xx-small;">   
EcDoRpcExt2  
     
        pcxh                     PCXH            
0x00000000-{db54d04d-2d57-4513-8505-3830d0a9d8cc}  
        \*pulFlags                unsigned long    0x00000000 (0)  
        \*pcbOut                  unsigned long    0x00007bdd (31709)  
        \*pcbAux                  unsigned long    0x00000000 (0)  
        rgbAuxOut                unsigned char\[\]  0 byte(s)  
        \*pulTransTime            unsigned long    0x00000000 (0)  
        \[Return value\]           unsigned long    0x00000000
(ecNone)  
        rgbOut                   unsigned char\[\]  31709 byte(s)  
            RPC\_HEADER\_EXT Header:     Version: 0 (0x0000), Flags: 4
(0x0004), Size: 31701 (0x7bd5), SizeActual: 31701 (0x7bd5)  \< Last \>  
             
            HSOT Table: 1 Item(s)  
                \[0\] 0x00000082     
             
           
<span style="background-color: #ffff00;">RopFastTransferSourceGetBuffer</span>:  
             
                RopId                    unsigned char    0x4e (78)  
                InputHandleIndex         unsigned long    0
(HSOT=0x00000082)  
                ReturnValue              unsigned long    0x00000000
(ecNone)  
                TransferStatus           unsigned short   0x0001 (1)  \<
Partial \>  
                InProgressCount          unsigned short   0x0001 (1)  
                TotalStepCount           unsigned long    0x0002 (2)  
                Reserved                 unsigned char    0x00 (0)  
                TransferBufferSize       unsigned short   0x7bc0
(31680)  
               
TransferBuffer:</span>

<span style="font-family: Courier New;"><span style="font-size: xx-small;">0x2cd3001e 
\<Unknown\>                             PtypString8      31680 Byte(s)  
</span><span style="font-size: xx-small;">**0000: 69 00 7A 00 61 00 74
00 69 00 6F 00 6E 00 2D 00 - i.z.a.t.i.o.n.-.  
0010: 61 00 75 00 74 00 68 00 73 00 6F 00 75 00 72 00 -
a.u.t.h.s.o.u.r.  
0020: 63 00 65 00 00 00 62 00 00 00 68 00 61 00 72 00 -
c.e**...X...X.X.X.  
0030: 69 00 2D 00 65 00 78 00 63 00 68 00 67 00 32 00 -
X.-.X.X.X.X.X.X.  
0040: 6B 00 37 00 2E 00 68 00 61 00 72 00 69 00 2D 00 -
X.X...X.X.X.X.-.  
0050: 65 00 78 00 63 00 68 00 67 00 32 00 6B 00 37 00 -
X.X.X.X.X.X.X.X.  
</span></span><span style="font-family: Courier New; font-size: x-small;">…</span>

<span style="font-size: x-small;">This particular ropFXbuffer looked
interesting to me because of the partial string “ization-authsource”. Of
course, I expect to find the first part of the string at the end of the
previous buffer.</span>

<span style="font-size: x-small;">\[Pipe Two\]</span>

<span style="font-size: x-small;">However, since the default for RPX
when decoding is to dump only the first 256 bytes of the buffers I’ll
need to run RPX again and add some options to the RPX command-line to
dump all the buffer contents of all buffers to see the end of the
previous ropFXBuffer (since it was quite a bit larger than 256 bytes).
So I ran it again on the same capture file and chose the “**-L 0**”
option. Of course, depending on the exact scenario, this typically
produces a much larger output file since it decodes all the buffers in
all Exchange Server RPC communication on the wire. In my case, this
resulted in a 65 MB text file. Usually, you don’t need to dump the
entire buffer contents for all buffers to troubleshoot an issue, but I
needed to do so in this situation.</span>

<span style="font-size: x-small;">Now I have the output for the previous
ropFXBuffer, which looks like this
(partial):</span>

<span style="font-family: Courier New; font-size: xx-small;">17:03:01.282   
\#60529    192.168.1.1:45929        192.168.2.2:3984          
TCP/IP     
MSRPC Call ID: 0x73b    RESPONSE: 0 ms    Store RPC</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60529,  
    60530,  
    60532,  
    60533,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60534,  
    60536,  
    60538,  
    60539,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60541,  
    60542,  
    60544,  
    60545,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60546,  
    60548,  
    60550,  
    60551,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60553,  
    60554,  
    60556,  
    60557,</span>

<span style="font-family: Courier New; font-size: xx-small;">Coalescing
TCP packets in frame(s):  
    60559,  
    60560,</span>

<span style="font-family: Courier New; font-size: xx-small;">RPC
Coalesced: 31800 bytes</span>

<span style="font-family: Courier New; font-size: xx-small;">   
EcDoRpcExt2  
     
        pcxh                     PCXH            
0x00000000-{db54d04d-2d57-4513-8505-3830d0a9d8cc}  
        \*pulFlags                unsigned long    0x00000000 (0)  
        \*pcbOut                  unsigned long    0x00007bdd (31709)  
        \*pcbAux                  unsigned long    0x00000000 (0)  
        rgbAuxOut                unsigned char\[\]  0 byte(s)  
        \*pulTransTime            unsigned long    0x00000000 (0)  
        \[Return value\]           unsigned long    0x00000000
(ecNone)  
        rgbOut                   unsigned char\[\]  31709 byte(s)  
RPC\_HEADER\_EXT Header:     Version: 0 (0x0000), Flags: 4 (0x0004),
Size: 31701 (0x7bd5), SizeActual: 31701 (0x7bd5)  \< Last \>  
             
            HSOT Table: 1 Item(s)  
                \[0\] 0x00000082     
             
           
<span style="background-color: #ffff00;">RopFastTransferSourceGetBuffer</span>:  
             
                RopId                    unsigned char    0x4e (78)  
                InputHandleIndex         unsigned long    0
(HSOT=0x00000082)  
                ReturnValue              unsigned long    0x00000000
(ecNone)  
                TransferStatus           unsigned short   0x0001 (1)  \<
Partial \>  
                InProgressCount          unsigned short   0x0001 (1)  
                TotalStepCount           unsigned long    0x0002 (2)  
                Reserved                 unsigned char    0x00 (0)  
                TransferBufferSize       unsigned short   0x7bc0
(31680)</span>

<span style="font-family: Courier New;"><span style="font-size: xx-small;">Raw
ROP data in binary form:  
0002: 4E 00 00 00 00 00 01 00 01 00 02 00 00 C0 7B 00 -
N.............{.  
0012: 00 34 32 00 00 35 32 00 00 36 32 00 00 37 32 00 -
.42..52..62..72.  
0022: 00 38 32 00 00 39 32 00 00 3A 32 00 00 3B 32 00 -
.82..92..:2..;2.  
…  
</span>**<span style="font-size: xx-small;">7ba2: C0 00 00 00 00 00 00
46 01 78 00 2D 00 6D 00 73 - .......F.x.-.m.s  
7bb2: 00 2D 00 65 00 78 00 63 00 68 00 61 00 6E 00 67 -
.-.e.x.c.h.a.n.g  
7bc2: 00 65 00 2D 00 6F 00 72 00 67 00 61 00 6E 00    -
.e.-.o.r.g.a.n.</span>**</span>

** **

<span style="font-size: x-small;">The first part of the string is
clearly “x-ms-exchange-organ”, so I go searching for what is
“x-ms-exchange-organization-authsource”. It turns out,
“</span>[<span style="font-size: x-small;">X-MS-Exchange-Organization-AuthSource</span>](http://technet.microsoft.com/en-us/library/bb232136.aspx)<span id="_GoBack"></span><span style="font-size: x-small;">”
is defined in
</span>[<span style="font-size: x-small;">MS-OXPROPS</span>](http://msdn.microsoft.com/en-us/library/ee625289\(EXCHG.80\).aspx)<span style="font-size: x-small;">:</span>

<span style="font-size: x-small;">**2.376 PidNameAuthenticatedSource  
**Canonical name: PidNameAuthenticatedSource  
Description:  
Property set: PS\_INTERNET\_HEADERS
{00020386-0000-0000-C000-000000000046}  
Property name: **X-MS-Exchange-Organization-AuthSource  
**Data type:PtypString, 0x001F</span>

<span style="font-size: x-small;">\[Pipe Three\]</span>

<span style="font-size: x-small;">So this string is a property called
“X-MS-Exchange-Organization-AuthSource” and type “PtypString”. </span>

<span style="font-size: x-small;">According to
</span>[<span style="font-size: x-small;">MS-OXCFXICS 2.2.4.1  Lexical
structure</span>](http://msdn.microsoft.com/en-us/library/ee236947\(EXCHG.80\).aspx)<span style="font-size: x-small;">,
A FastTransfer Stream can be larger than a single buffer. The server
MUST split the stream when it cannot fit into a single buffer. If a
split is required, the stream MUST be split either between two atoms or
at any point inside a varSizeValue.
<span style="background-color: #ffff00;">A stream MUST NOT be split
within a single atom.</span> The lexical structure of an atom is as
follows: </span>

<span style="font-size: x-small;">atom       = marker   
               / propDef   
               / fixedSizeValue   
               / length  
propDef  = ( propType propInfo )</span>

<span style="font-size: x-small;">Well, this explains it. The buffer is
invalid per protocol specification since this property was split across
two buffers. The client implementation does not consider this particular
issue a critical error and thereby continues to the next folder to sync.
The problem was identified and the man-in-the-middle code fixed. It’s
elementary with the right tools. Well, not quite. I did use some other
tools initially, and simplified the retelling of the solution to include
only RPX. And by doing so I demonstrated with a dash of intuition this
type of problem is entirely solvable with RPX and how easy it is to use.
Nevertheless, that concludes the case of the mysterious Inbox
sync.</span>

</div>

</div>
