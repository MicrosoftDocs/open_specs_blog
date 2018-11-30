<div id="page">

# RDP 6.0 Bulk Compressing Data Walkthrough

[Pyralanna](https://social.msdn.microsoft.com/profile/Pyralanna)
5/26/2010 8:47:22
AM

-----

<div id="content">

<div style="border-bottom: #4f81bd 1pt solid; border-left: medium none; padding-bottom: 2pt; padding-left: 0in; padding-right: 0in; border-top: medium none; border-right: medium none; padding-top: 0in; mso-border-bottom-themecolor: accent1; mso-element: para-border-div;">

<span style="font-family: Calibri; font-size: xx-small;"> </span>

</div>

<span style="font-family: Calibri; font-size: xx-small;"> </span>

<span style="font-family: Calibri; font-size: small;">The aim of this
blog is to walk through an example of compressing a data sample using
RDP 6.0 Compression. In a future blog I will provide a walkthrough of
decoding the compressed stream back to the original data stream.
<span style="mso-spacerun: yes;"> </span>In order to better understand
the example I want to first establish some key points. Most of this blog
will center around \[MS-RDPEGDI\] Remote Desktop Protocol: Graphics
Device Interface (GDI) Acceleration Extensions Section
3.1.8<span style="mso-spacerun: yes;">   </span>Bulk Data Compression.
This blog will mainly include only details that are important to the
example walkthrough itself. For more inquisitive minds, other important
details not covered here can be found in section 3.1.8. RDP version 6.0
supports an extension to the compression techniques described in
\[MS-RDPBCGR\] Remote Desktop Protocol: Basic Connectivity and Graphics
Remoting Specification section 3.1.8. This extension is called "RDP 6.0
Bulk Compression" (RDP6.0-BC) and is only supported for server-to-client
traffic.</span>

<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes;"> </span>\[MS-RDPEGDI\]
<span style="mso-spacerun: yes;">  </span></span></span>[<span style="font-family: Calibri; font-size: small;">http://msdn.microsoft.com/en-us/library/cc241537(PROT.10).aspx</span>](http://msdn.microsoft.com/en-us/library/cc241537\(PROT.10\).aspx)

<span style="font-family: Calibri; font-size: small;"> \[MS-RDPBCGR\]  
</span>[<span style="font-family: Calibri; font-size: small;">http://msdn.microsoft.com/en-us/library/cc240445(v=PROT.10).aspx</span>](http://msdn.microsoft.com/en-us/library/cc240445\(v=PROT.10\).aspx)

<span style="font-family: Calibri; font-size: small;">Let’s take a look
at some important concepts before we look at the example. The shared
state necessary to support the transmission and reception of RDP6.0-BC
compressed data between a client and server requires a history buffer
and a current offset into the history buffer (HistoryOffset). The size
of the history buffer is 64 KB. The HistoryOffset MUST start initialized
to zero, while the history buffer MUST be filled with zeros. After it
has been initialized, the entire history buffer is immediately regarded
as valid.</span>

<span style="font-family: Calibri; font-size: small;">In addition to the
history buffer and HistoryOffset, a small cache MUST also be managed by
the client and server endpoints. This cache is referred to as the
OffsetCache and is used to store the last four unique copy-offsets
encountered during data compression (copy-offsets are described in
\[MS-RDPBCGR\] section 3.1.8.1). This saves on bandwidth in cases where
there are many repeated copy-offsets. Whenever the history buffer is
initialized or reinitialized, the OffsetCache MUST be emptied. The
uncompressed data is first inserted into the local history buffer at the
position indicated by HistoryOffset by the sender. The compressor then
runs through the length of newly added uncompressed data to be sent and
produces as output a sequence of literals (bytes to be sent
uncompressed) or copy-tuples which consists of a \<copy-offset,
length-of-match\> pair. The copy-offset component of the copy-tuple is
an index into the history buffer (counting backwards from the current
byte being compressed in the history buffer towards the start of the
buffer) where there is a match to the data to be sent. </span>

<span style="font-family: Calibri; font-size: small;">There are 6 tables
and a flowchart that are needed to walk through the sample data. These
can be found under \[MS-RDPEGDI\]
3.1.8.1.4.1<span style="mso-spacerun: yes;">   </span>Literal, EOS, and
Copy-Offset Tables and \[MS-RDPEGDI\]
3.1.8.1.4.3<span style="mso-spacerun: yes;">   </span>Encoding the
Logically Compressed Stream respectively.</span>

<span style="font-family: Calibri; font-size: small;">Here are the
tables and flowcharts that we will be using. You may find it easier to
print these off in order to follow the walkthrough.</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">Table 1:
Bit-lengths for the 294 Huffman-encoded LiteralOrEosOrCopyOffset codes
</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
2: Huffman codebook for the 294 Huffman-encoded LiteralOrEosOrCopyOffset
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">Table 3: Bit count
and base value lookup tables to encode and decode copy-offset
values</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
4: Table 4: Bit lengths for the 32 Huffman-encoded LengthOfMatch
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
5: Huffman codebook for the 32 Huffman-encoded LengthOfMatch
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">Table 6: Bit count
and base value lookup tables to encode and decode length-of-match
values</span>

<span style="font-family: Calibri; font-size: small;">Flowchart:</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
1: Bit-lengths for the 294 Huffman-encoded LiteralOrEosOrCopyOffset
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">Code
index
</span></span></span>**

</div>

</div>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">0
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">1
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">2
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">3
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">4
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">5
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">6
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">7
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">24</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">32</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">40</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">48</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">56</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">64</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">72</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">80</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">88</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">96</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">104</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">112</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">120</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">128</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">136</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">144</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">152</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">160</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">168</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xd</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">176</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xb</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">184</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">192</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">200</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">208</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">216</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">224</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">232</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">240</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">248</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">256</span>

<span style="font-family: Verdana; font-size: x-small;">0xd</span>

<span style="font-family: Verdana; font-size: x-small;">0xd</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0xa</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">264</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">272</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">280</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">288</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0xD</span>

<span style="font-family: Verdana; font-size: x-small;"> </span>

<span style="font-family: Verdana; font-size: x-small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">For
example, it can be determined from the previous table that the 0th
Huffman-encoded LiteralOrEosOrCopyOffset code has a length of 6 bits,
and the 131st Huffman-encoded LiteralOrEosOrCopyOffset code has a length
of 10 (0x0A)
bits.<span style="mso-bidi-font-size: 11.0pt; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">Table 2: Huffman
codebook for the 294 Huffman-encoded LiteralOrEosOrCopyOffset
codes</span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">Code
index
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">0
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">1
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">2
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">3
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">4
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">5
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">6
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">7
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0x0004</span>

<span style="font-family: Verdana; font-size: x-small;">0x0024</span>

<span style="font-family: Verdana; font-size: x-small;">0x0014</span>

<span style="font-family: Verdana; font-size: x-small;">0x0011</span>

<span style="font-family: Verdana; font-size: x-small;">0x0051</span>

<span style="font-family: Verdana; font-size: x-small;">0x0031</span>

<span style="font-family: Verdana; font-size: x-small;">0x0071</span>

<span style="font-family: Verdana; font-size: x-small;">0x0009</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">0x0049</span>

<span style="font-family: Verdana; font-size: x-small;">0x0029</span>

<span style="font-family: Verdana; font-size: x-small;">0x0069</span>

<span style="font-family: Verdana; font-size: x-small;">0x0015</span>

<span style="font-family: Verdana; font-size: x-small;">0x0095</span>

<span style="font-family: Verdana; font-size: x-small;">0x0055</span>

<span style="font-family: Verdana; font-size: x-small;">0x00d5</span>

<span style="font-family: Verdana; font-size: x-small;">0x0035</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">0x00b5</span>

<span style="font-family: Verdana; font-size: x-small;">0x0075</span>

<span style="font-family: Verdana; font-size: x-small;">0x001d</span>

<span style="font-family: Verdana; font-size: x-small;">0x00f5</span>

<span style="font-family: Verdana; font-size: x-small;">0x011d</span>

<span style="font-family: Verdana; font-size: x-small;">0x009d</span>

<span style="font-family: Verdana; font-size: x-small;">0x019d</span>

<span style="font-family: Verdana; font-size: x-small;">0x005d</span>

<span style="font-family: Verdana; font-size: x-small;">24</span>

<span style="font-family: Verdana; font-size: x-small;">0x000d</span>

<span style="font-family: Verdana; font-size: x-small;">0x008d</span>

<span style="font-family: Verdana; font-size: x-small;">0x015d</span>

<span style="font-family: Verdana; font-size: x-small;">0x00dd</span>

<span style="font-family: Verdana; font-size: x-small;">0x01dd</span>

<span style="font-family: Verdana; font-size: x-small;">0x003d</span>

<span style="font-family: Verdana; font-size: x-small;">0x013d</span>

<span style="font-family: Verdana; font-size: x-small;">0x00bd</span>

<span style="font-family: Verdana; font-size: x-small;">32</span>

<span style="font-family: Verdana; font-size: x-small;">0x004d</span>

<span style="font-family: Verdana; font-size: x-small;">0x01bd</span>

<span style="font-family: Verdana; font-size: x-small;">0x007d</span>

<span style="font-family: Verdana; font-size: x-small;">0x006b</span>

<span style="font-family: Verdana; font-size: x-small;">0x017d</span>

<span style="font-family: Verdana; font-size: x-small;">0x00fd</span>

<span style="font-family: Verdana; font-size: x-small;">0x01fd</span>

<span style="font-family: Verdana; font-size: x-small;">0x0003</span>

<span style="font-family: Verdana; font-size: x-small;">40</span>

<span style="font-family: Verdana; font-size: x-small;">0x0103</span>

<span style="font-family: Verdana; font-size: x-small;">0x0083</span>

<span style="font-family: Verdana; font-size: x-small;">0x0183</span>

<span style="font-family: Verdana; font-size: x-small;">0x026b</span>

<span style="font-family: Verdana; font-size: x-small;">0x0043</span>

<span style="font-family: Verdana; font-size: x-small;">0x016b</span>

<span style="font-family: Verdana; font-size: x-small;">0x036b</span>

<span style="font-family: Verdana; font-size: x-small;">0x00eb</span>

<span style="font-family: Verdana; font-size: x-small;">48</span>

<span style="font-family: Verdana; font-size: x-small;">0x0143</span>

<span style="font-family: Verdana; font-size: x-small;">0x00c3</span>

<span style="font-family: Verdana; font-size: x-small;">0x02eb</span>

<span style="font-family: Verdana; font-size: x-small;">0x01c3</span>

<span style="font-family: Verdana; font-size: x-small;">0x01eb</span>

<span style="font-family: Verdana; font-size: x-small;">0x0023</span>

<span style="font-family: Verdana; font-size: x-small;">0x03eb</span>

<span style="font-family: Verdana; font-size: x-small;">0x0123</span>

<span style="font-family: Verdana; font-size: x-small;">56</span>

<span style="font-family: Verdana; font-size: x-small;">0x00a3</span>

<span style="font-family: Verdana; font-size: x-small;">0x01a3</span>

<span style="font-family: Verdana; font-size: x-small;">0x001b</span>

<span style="font-family: Verdana; font-size: x-small;">0x021b</span>

<span style="font-family: Verdana; font-size: x-small;">0x0063</span>

<span style="font-family: Verdana; font-size: x-small;">0x011b</span>

<span style="font-family: Verdana; font-size: x-small;">0x0163</span>

<span style="font-family: Verdana; font-size: x-small;">0x00e3</span>

<span style="font-family: Verdana; font-size: x-small;">64</span>

<span style="font-family: Verdana; font-size: x-small;">0x00cd</span>

<span style="font-family: Verdana; font-size: x-small;">0x01e3</span>

<span style="font-family: Verdana; font-size: x-small;">0x0013</span>

<span style="font-family: Verdana; font-size: x-small;">0x0113</span>

<span style="font-family: Verdana; font-size: x-small;">0x0093</span>

<span style="font-family: Verdana; font-size: x-small;">0x031b</span>

<span style="font-family: Verdana; font-size: x-small;">0x009b</span>

<span style="font-family: Verdana; font-size: x-small;">0x029b</span>

<span style="font-family: Verdana; font-size: x-small;">72</span>

<span style="font-family: Verdana; font-size: x-small;">0x0193</span>

<span style="font-family: Verdana; font-size: x-small;">0x0053</span>

<span style="font-family: Verdana; font-size: x-small;">0x019b</span>

<span style="font-family: Verdana; font-size: x-small;">0x039b</span>

<span style="font-family: Verdana; font-size: x-small;">0x005b</span>

<span style="font-family: Verdana; font-size: x-small;">0x025b</span>

<span style="font-family: Verdana; font-size: x-small;">0x015b</span>

<span style="font-family: Verdana; font-size: x-small;">0x035b</span>

<span style="font-family: Verdana; font-size: x-small;">80</span>

<span style="font-family: Verdana; font-size: x-small;">0x0153</span>

<span style="font-family: Verdana; font-size: x-small;">0x00d3</span>

<span style="font-family: Verdana; font-size: x-small;">0x00db</span>

<span style="font-family: Verdana; font-size: x-small;">0x02db</span>

<span style="font-family: Verdana; font-size: x-small;">0x01db</span>

<span style="font-family: Verdana; font-size: x-small;">0x03db</span>

<span style="font-family: Verdana; font-size: x-small;">0x003b</span>

<span style="font-family: Verdana; font-size: x-small;">0x023b</span>

<span style="font-family: Verdana; font-size: x-small;">88</span>

<span style="font-family: Verdana; font-size: x-small;">0x013b</span>

<span style="font-family: Verdana; font-size: x-small;">0x01d3</span>

<span style="font-family: Verdana; font-size: x-small;">0x033b</span>

<span style="font-family: Verdana; font-size: x-small;">0x00bb</span>

<span style="font-family: Verdana; font-size: x-small;">0x02bb</span>

<span style="font-family: Verdana; font-size: x-small;">0x01bb</span>

<span style="font-family: Verdana; font-size: x-small;">0x03bb</span>

<span style="font-family: Verdana; font-size: x-small;">0x007b</span>

<span style="font-family: Verdana; font-size: x-small;">96</span>

<span style="font-family: Verdana; font-size: x-small;">0x002d</span>

<span style="font-family: Verdana; font-size: x-small;">0x027b</span>

<span style="font-family: Verdana; font-size: x-small;">0x017b</span>

<span style="font-family: Verdana; font-size: x-small;">0x037b</span>

<span style="font-family: Verdana; font-size: x-small;">0x00fb</span>

<span style="font-family: Verdana; font-size: x-small;">0x02fb</span>

<span style="font-family: Verdana; font-size: x-small;">0x01fb</span>

<span style="font-family: Verdana; font-size: x-small;">0x03fb</span>

<span style="font-family: Verdana; font-size: x-small;">104</span>

<span style="font-family: Verdana; font-size: x-small;">0x0007</span>

<span style="font-family: Verdana; font-size: x-small;">0x0207</span>

<span style="font-family: Verdana; font-size: x-small;">0x0107</span>

<span style="font-family: Verdana; font-size: x-small;">0x0307</span>

<span style="font-family: Verdana; font-size: x-small;">0x0087</span>

<span style="font-family: Verdana; font-size: x-small;">0x0287</span>

<span style="font-family: Verdana; font-size: x-small;">0x0187</span>

<span style="font-family: Verdana; font-size: x-small;">0x0387</span>

<span style="font-family: Verdana; font-size: x-small;">112</span>

<span style="font-family: Verdana; font-size: x-small;">0x0033</span>

<span style="font-family: Verdana; font-size: x-small;">0x0047</span>

<span style="font-family: Verdana; font-size: x-small;">0x0247</span>

<span style="font-family: Verdana; font-size: x-small;">0x0147</span>

<span style="font-family: Verdana; font-size: x-small;">0x0347</span>

<span style="font-family: Verdana; font-size: x-small;">0x00c7</span>

<span style="font-family: Verdana; font-size: x-small;">0x02c7</span>

<span style="font-family: Verdana; font-size: x-small;">0x01c7</span>

<span style="font-family: Verdana; font-size: x-small;">120</span>

<span style="font-family: Verdana; font-size: x-small;">0x0133</span>

<span style="font-family: Verdana; font-size: x-small;">0x03c7</span>

<span style="font-family: Verdana; font-size: x-small;">0x0027</span>

<span style="font-family: Verdana; font-size: x-small;">0x0227</span>

<span style="font-family: Verdana; font-size: x-small;">0x0127</span>

<span style="font-family: Verdana; font-size: x-small;">0x0327</span>

<span style="font-family: Verdana; font-size: x-small;">0x00a7</span>

<span style="font-family: Verdana; font-size: x-small;">0x00b3</span>

<span style="font-family: Verdana; font-size: x-small;">128</span>

<span style="font-family: Verdana; font-size: x-small;">0x0019</span>

<span style="font-family: Verdana; font-size: x-small;">0x01b3</span>

<span style="font-family: Verdana; font-size: x-small;">0x0073</span>

<span style="font-family: Verdana; font-size: x-small;">0x02a7</span>

<span style="font-family: Verdana; font-size: x-small;">0x0173</span>

<span style="font-family: Verdana; font-size: x-small;">0x01a7</span>

<span style="font-family: Verdana; font-size: x-small;">0x03a7</span>

<span style="font-family: Verdana; font-size: x-small;">0x0067</span>

<span style="font-family: Verdana; font-size: x-small;">136</span>

<span style="font-family: Verdana; font-size: x-small;">0x00f3</span>

<span style="font-family: Verdana; font-size: x-small;">0x0267</span>

<span style="font-family: Verdana; font-size: x-small;">0x0167</span>

<span style="font-family: Verdana; font-size: x-small;">0x0367</span>

<span style="font-family: Verdana; font-size: x-small;">0x00e7</span>

<span style="font-family: Verdana; font-size: x-small;">0x02e7</span>

<span style="font-family: Verdana; font-size: x-small;">0x01e7</span>

<span style="font-family: Verdana; font-size: x-small;">0x03e7</span>

<span style="font-family: Verdana; font-size: x-small;">144</span>

<span style="font-family: Verdana; font-size: x-small;">0x01f3</span>

<span style="font-family: Verdana; font-size: x-small;">0x0017</span>

<span style="font-family: Verdana; font-size: x-small;">0x0217</span>

<span style="font-family: Verdana; font-size: x-small;">0x0117</span>

<span style="font-family: Verdana; font-size: x-small;">0x0317</span>

<span style="font-family: Verdana; font-size: x-small;">0x0097</span>

<span style="font-family: Verdana; font-size: x-small;">0x0297</span>

<span style="font-family: Verdana; font-size: x-small;">0x0197</span>

<span style="font-family: Verdana; font-size: x-small;">152</span>

<span style="font-family: Verdana; font-size: x-small;">0x0397</span>

<span style="font-family: Verdana; font-size: x-small;">0x0057</span>

<span style="font-family: Verdana; font-size: x-small;">0x0257</span>

<span style="font-family: Verdana; font-size: x-small;">0x0157</span>

<span style="font-family: Verdana; font-size: x-small;">0x0357</span>

<span style="font-family: Verdana; font-size: x-small;">0x00d7</span>

<span style="font-family: Verdana; font-size: x-small;">0x02d7</span>

<span style="font-family: Verdana; font-size: x-small;">0x01d7</span>

<span style="font-family: Verdana; font-size: x-small;">160</span>

<span style="font-family: Verdana; font-size: x-small;">0x03d7</span>

<span style="font-family: Verdana; font-size: x-small;">0x0037</span>

<span style="font-family: Verdana; font-size: x-small;">0x0237</span>

<span style="font-family: Verdana; font-size: x-small;">0x0137</span>

<span style="font-family: Verdana; font-size: x-small;">0x0337</span>

<span style="font-family: Verdana; font-size: x-small;">0x00b7</span>

<span style="font-family: Verdana; font-size: x-small;">0x02b7</span>

<span style="font-family: Verdana; font-size: x-small;">0x01b7</span>

<span style="font-family: Verdana; font-size: x-small;">168</span>

<span style="font-family: Verdana; font-size: x-small;">0x03b7</span>

<span style="font-family: Verdana; font-size: x-small;">0x0077</span>

<span style="font-family: Verdana; font-size: x-small;">0x0277</span>

<span style="font-family: Verdana; font-size: x-small;">0x07ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x0177</span>

<span style="font-family: Verdana; font-size: x-small;">0x0377</span>

<span style="font-family: Verdana; font-size: x-small;">0x00f7</span>

<span style="font-family: Verdana; font-size: x-small;">0x02f7</span>

<span style="font-family: Verdana; font-size: x-small;">176</span>

<span style="font-family: Verdana; font-size: x-small;">0x01f7</span>

<span style="font-family: Verdana; font-size: x-small;">0x03f7</span>

<span style="font-family: Verdana; font-size: x-small;">0x03ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x000f</span>

<span style="font-family: Verdana; font-size: x-small;">0x020f</span>

<span style="font-family: Verdana; font-size: x-small;">0x010f</span>

<span style="font-family: Verdana; font-size: x-small;">0x030f</span>

<span style="font-family: Verdana; font-size: x-small;">0x008f</span>

<span style="font-family: Verdana; font-size: x-small;">184</span>

<span style="font-family: Verdana; font-size: x-small;">0x028f</span>

<span style="font-family: Verdana; font-size: x-small;">0x018f</span>

<span style="font-family: Verdana; font-size: x-small;">0x038f</span>

<span style="font-family: Verdana; font-size: x-small;">0x004f</span>

<span style="font-family: Verdana; font-size: x-small;">0x024f</span>

<span style="font-family: Verdana; font-size: x-small;">0x014f</span>

<span style="font-family: Verdana; font-size: x-small;">0x034f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00cf</span>

<span style="font-family: Verdana; font-size: x-small;">192</span>

<span style="font-family: Verdana; font-size: x-small;">0x000b</span>

<span style="font-family: Verdana; font-size: x-small;">0x02cf</span>

<span style="font-family: Verdana; font-size: x-small;">0x01cf</span>

<span style="font-family: Verdana; font-size: x-small;">0x03cf</span>

<span style="font-family: Verdana; font-size: x-small;">0x002f</span>

<span style="font-family: Verdana; font-size: x-small;">0x022f</span>

<span style="font-family: Verdana; font-size: x-small;">0x010b</span>

<span style="font-family: Verdana; font-size: x-small;">0x012f</span>

<span style="font-family: Verdana; font-size: x-small;">200</span>

<span style="font-family: Verdana; font-size: x-small;">0x032f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00af</span>

<span style="font-family: Verdana; font-size: x-small;">0x02af</span>

<span style="font-family: Verdana; font-size: x-small;">0x01af</span>

<span style="font-family: Verdana; font-size: x-small;">0x008b</span>

<span style="font-family: Verdana; font-size: x-small;">0x03af</span>

<span style="font-family: Verdana; font-size: x-small;">0x006f</span>

<span style="font-family: Verdana; font-size: x-small;">0x026f</span>

<span style="font-family: Verdana; font-size: x-small;">208</span>

<span style="font-family: Verdana; font-size: x-small;">0x018b</span>

<span style="font-family: Verdana; font-size: x-small;">0x016f</span>

<span style="font-family: Verdana; font-size: x-small;">0x036f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ef</span>

<span style="font-family: Verdana; font-size: x-small;">0x02ef</span>

<span style="font-family: Verdana; font-size: x-small;">0x01ef</span>

<span style="font-family: Verdana; font-size: x-small;">0x03ef</span>

<span style="font-family: Verdana; font-size: x-small;">0x001f</span>

<span style="font-family: Verdana; font-size: x-small;">216</span>

<span style="font-family: Verdana; font-size: x-small;">0x021f</span>

<span style="font-family: Verdana; font-size: x-small;">0x011f</span>

<span style="font-family: Verdana; font-size: x-small;">0x031f</span>

<span style="font-family: Verdana; font-size: x-small;">0x009f</span>

<span style="font-family: Verdana; font-size: x-small;">0x029f</span>

<span style="font-family: Verdana; font-size: x-small;">0x019f</span>

<span style="font-family: Verdana; font-size: x-small;">0x039f</span>

<span style="font-family: Verdana; font-size: x-small;">0x005f</span>

<span style="font-family: Verdana; font-size: x-small;">224</span>

<span style="font-family: Verdana; font-size: x-small;">0x004b</span>

<span style="font-family: Verdana; font-size: x-small;">0x025f</span>

<span style="font-family: Verdana; font-size: x-small;">0x015f</span>

<span style="font-family: Verdana; font-size: x-small;">0x035f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00df</span>

<span style="font-family: Verdana; font-size: x-small;">0x02df</span>

<span style="font-family: Verdana; font-size: x-small;">0x01df</span>

<span style="font-family: Verdana; font-size: x-small;">0x03df</span>

<span style="font-family: Verdana; font-size: x-small;">232</span>

<span style="font-family: Verdana; font-size: x-small;">0x003f</span>

<span style="font-family: Verdana; font-size: x-small;">0x023f</span>

<span style="font-family: Verdana; font-size: x-small;">0x013f</span>

<span style="font-family: Verdana; font-size: x-small;">0x033f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00bf</span>

<span style="font-family: Verdana; font-size: x-small;">0x02bf</span>

<span style="font-family: Verdana; font-size: x-small;">0x014b</span>

<span style="font-family: Verdana; font-size: x-small;">0x01bf</span>

<span style="font-family: Verdana; font-size: x-small;">240</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ad</span>

<span style="font-family: Verdana; font-size: x-small;">0x00cb</span>

<span style="font-family: Verdana; font-size: x-small;">0x01cb</span>

<span style="font-family: Verdana; font-size: x-small;">0x03bf</span>

<span style="font-family: Verdana; font-size: x-small;">0x002b</span>

<span style="font-family: Verdana; font-size: x-small;">0x007f</span>

<span style="font-family: Verdana; font-size: x-small;">0x027f</span>

<span style="font-family: Verdana; font-size: x-small;">0x017f</span>

<span style="font-family: Verdana; font-size: x-small;">248</span>

<span style="font-family: Verdana; font-size: x-small;">0x012b</span>

<span style="font-family: Verdana; font-size: x-small;">0x037f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x02ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ab</span>

<span style="font-family: Verdana; font-size: x-small;">0x01ab</span>

<span style="font-family: Verdana; font-size: x-small;">0x006d</span>

<span style="font-family: Verdana; font-size: x-small;">0x0059</span>

<span style="font-family: Verdana; font-size: x-small;">256</span>

<span style="font-family: Verdana; font-size: x-small;">0x17ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x0fff</span>

<span style="font-family: Verdana; font-size: x-small;">0x0039</span>

<span style="font-family: Verdana; font-size: x-small;">0x0079</span>

<span style="font-family: Verdana; font-size: x-small;">0x01ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x0005</span>

<span style="font-family: Verdana; font-size: x-small;">0x0045</span>

<span style="font-family: Verdana; font-size: x-small;">0x0034</span>

<span style="font-family: Verdana; font-size: x-small;">264</span>

<span style="font-family: Verdana; font-size: x-small;">0x000c</span>

<span style="font-family: Verdana; font-size: x-small;">0x002c</span>

<span style="font-family: Verdana; font-size: x-small;">0x001c</span>

<span style="font-family: Verdana; font-size: x-small;">0x0000</span>

<span style="font-family: Verdana; font-size: x-small;">0x003c</span>

<span style="font-family: Verdana; font-size: x-small;">0x0002</span>

<span style="font-family: Verdana; font-size: x-small;">0x0022</span>

<span style="font-family: Verdana; font-size: x-small;">0x0010</span>

<span style="font-family: Verdana; font-size: x-small;">272</span>

<span style="font-family: Verdana; font-size: x-small;">0x0012</span>

<span style="font-family: Verdana; font-size: x-small;">0x0008</span>

<span style="font-family: Verdana; font-size: x-small;">0x0032</span>

<span style="font-family: Verdana; font-size: x-small;">0x000a</span>

<span style="font-family: Verdana; font-size: x-small;">0x002a</span>

<span style="font-family: Verdana; font-size: x-small;">0x001a</span>

<span style="font-family: Verdana; font-size: x-small;">0x003a</span>

<span style="font-family: Verdana; font-size: x-small;">0x0006</span>

<span style="font-family: Verdana; font-size: x-small;">280</span>

<span style="font-family: Verdana; font-size: x-small;">0x0026</span>

<span style="font-family: Verdana; font-size: x-small;">0x0016</span>

<span style="font-family: Verdana; font-size: x-small;">0x0036</span>

<span style="font-family: Verdana; font-size: x-small;">0x000e</span>

<span style="font-family: Verdana; font-size: x-small;">0x002e</span>

<span style="font-family: Verdana; font-size: x-small;">0x001e</span>

<span style="font-family: Verdana; font-size: x-small;">0x003e</span>

<span style="font-family: Verdana; font-size: x-small;">0x0001</span>

<span style="font-family: Verdana; font-size: x-small;">288</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ed</span>

<span style="font-family: Verdana; font-size: x-small;">0x0018</span>

<span style="font-family: Verdana; font-size: x-small;">0x0021</span>

<span style="font-family: Verdana; font-size: x-small;">0x0025</span>

<span style="font-family: Verdana; font-size: x-small;">0x0065</span>

<span style="font-family: Verdana; font-size: x-small;">0x1fff</span>

<span style="font-family: Verdana; font-size: x-small;"> </span>

<span style="font-family: Verdana; font-size: x-small;"> </span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Verdana; font-size: x-small;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">For
example, it can be determined from the previous table that the 0th
Huffman-encoded LiteralOrEosOrCopyOffset code has a value of 0x0004.
Applying the bit length information from the first table in this section
(6 bits), it can be determined that the Huffman code in binary MUST be
000100.<span style="mso-bidi-font-size: 11.0pt; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">As another
example, it can be determined from the previous table that the 131st
Huffman-encoded LiteralOrEosOrCopyOffset code has a value of 0x02a7.
Applying the bit length information from the first table in this section
(10 bits), it can be determined that the Huffman code in binary MUST be
1010100111.</span>

<span style="font-family: Calibri; font-size: small;">Table 3: Bit count
and base value lookup tables to encode and decode copy-offset
values</span><span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;">.</span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">Index
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">CopyOffsetBitsLUT
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">CopyOffsetBaseLUT
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">13</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">17</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">25</span>

<span style="font-family: Verdana; font-size: x-small;">10</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">33</span>

<span style="font-family: Verdana; font-size: x-small;">11</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">49</span>

<span style="font-family: Verdana; font-size: x-small;">12</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">65</span>

<span style="font-family: Verdana; font-size: x-small;">13</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">97</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">129</span>

<span style="font-family: Verdana; font-size: x-small;">15</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">193</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">257</span>

<span style="font-family: Verdana; font-size: x-small;">17</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">385</span>

<span style="font-family: Verdana; font-size: x-small;">18</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">513</span>

<span style="font-family: Verdana; font-size: x-small;">19</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">769</span>

<span style="font-family: Verdana; font-size: x-small;">20</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">1025</span>

<span style="font-family: Verdana; font-size: x-small;">21</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">1537</span>

<span style="font-family: Verdana; font-size: x-small;">22</span>

<span style="font-family: Verdana; font-size: x-small;">10</span>

<span style="font-family: Verdana; font-size: x-small;">2049</span>

<span style="font-family: Verdana; font-size: x-small;">23</span>

<span style="font-family: Verdana; font-size: x-small;">10</span>

<span style="font-family: Verdana; font-size: x-small;">3073</span>

<span style="font-family: Verdana; font-size: x-small;">24</span>

<span style="font-family: Verdana; font-size: x-small;">11</span>

<span style="font-family: Verdana; font-size: x-small;">4097</span>

<span style="font-family: Verdana; font-size: x-small;">25</span>

<span style="font-family: Verdana; font-size: x-small;">11</span>

<span style="font-family: Verdana; font-size: x-small;">6145</span>

<span style="font-family: Verdana; font-size: x-small;">26</span>

<span style="font-family: Verdana; font-size: x-small;">12</span>

<span style="font-family: Verdana; font-size: x-small;">8193</span>

<span style="font-family: Verdana; font-size: x-small;">27</span>

<span style="font-family: Verdana; font-size: x-small;">12</span>

<span style="font-family: Verdana; font-size: x-small;">12289</span>

<span style="font-family: Verdana; font-size: x-small;">28</span>

<span style="font-family: Verdana; font-size: x-small;">13</span>

<span style="font-family: Verdana; font-size: x-small;">16385</span>

<span style="font-family: Verdana; font-size: x-small;">29</span>

<span style="font-family: Verdana; font-size: x-small;">13</span>

<span style="font-family: Verdana; font-size: x-small;">24577</span>

<span style="font-family: Verdana; font-size: x-small;">30</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">32769</span>

<span style="font-family: Verdana; font-size: x-small;">31</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">49153</span>

<span style="font-family: Verdana; font-size: x-small;">32</span>

<span style="font-family: Verdana; font-size: x-small;">15</span>

<span style="font-family: Verdana; font-size: x-small;">65537</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
4: Bit lengths for the 32 Huffman-encoded LengthOfMatch
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

**<span style="letter-spacing: -0.1pt;"></span>**

<span style="font-family: Verdana; font-size: x-small;"> </span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">0
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">1
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">2
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">3
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">4
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">5
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">6
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">7
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0x4</span>

<span style="font-family: Verdana; font-size: x-small;">0x2</span>

<span style="font-family: Verdana; font-size: x-small;">0x3</span>

<span style="font-family: Verdana; font-size: x-small;">0x4</span>

<span style="font-family: Verdana; font-size: x-small;">0x3</span>

<span style="font-family: Verdana; font-size: x-small;">0x4</span>

<span style="font-family: Verdana; font-size: x-small;">0x4</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">0x4</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">0x5</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x6</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">0x7</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x8</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">24 </span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Verdana; font-size: x-small;">0x9</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">For
example, it can be determined from the previous table that the 0th
Huffman-encoded LengthOfMatch code has a length of 4 bits while the 21st
Huffman-encoded LengthOfMatch code has a length of 8
bits.<span style="mso-bidi-font-size: 11.0pt; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">Using the
canonical Huffman algorithm (for more information, see \[CANONHUFF\]),
the Huffman codebook table shown in the following table (HuffCodeL) can
be obtained. The bit lengths in the previous table MUST be used to mask
out the appropriate
bits.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
5: Huffman codebook for the 32 Huffman-encoded LengthOfMatch
codes<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">Code
index
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">0
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">1
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">2
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">3
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">4
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">5
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">6
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">7
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0x0001</span>

<span style="font-family: Verdana; font-size: x-small;">0x0000</span>

<span style="font-family: Verdana; font-size: x-small;">0x0002</span>

<span style="font-family: Verdana; font-size: x-small;">0x0009</span>

<span style="font-family: Verdana; font-size: x-small;">0x0006</span>

<span style="font-family: Verdana; font-size: x-small;">0x0005</span>

<span style="font-family: Verdana; font-size: x-small;">0x000d</span>

<span style="font-family: Verdana; font-size: x-small;">0x000b</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">0x0003</span>

<span style="font-family: Verdana; font-size: x-small;">0x001b</span>

<span style="font-family: Verdana; font-size: x-small;">0x0007</span>

<span style="font-family: Verdana; font-size: x-small;">0x0017</span>

<span style="font-family: Verdana; font-size: x-small;">0x0037</span>

<span style="font-family: Verdana; font-size: x-small;">0x000f</span>

<span style="font-family: Verdana; font-size: x-small;">0x004f</span>

<span style="font-family: Verdana; font-size: x-small;">0x006f</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">0x002f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ef</span>

<span style="font-family: Verdana; font-size: x-small;">0x001f</span>

<span style="font-family: Verdana; font-size: x-small;">0x005f</span>

<span style="font-family: Verdana; font-size: x-small;">0x015f</span>

<span style="font-family: Verdana; font-size: x-small;">0x009f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00df</span>

<span style="font-family: Verdana; font-size: x-small;">0x01df</span>

<span style="font-family: Verdana; font-size: x-small;">24</span>

<span style="font-family: Verdana; font-size: x-small;">0x003f</span>

<span style="font-family: Verdana; font-size: x-small;">0x013f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00bf</span>

<span style="font-family: Verdana; font-size: x-small;">0x01bf</span>

<span style="font-family: Verdana; font-size: x-small;">0x007f</span>

<span style="font-family: Verdana; font-size: x-small;">0x017f</span>

<span style="font-family: Verdana; font-size: x-small;">0x00ff</span>

<span style="font-family: Verdana; font-size: x-small;">0x01ff</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">For
example, it can be determined from the previous table that the 0th
Huffman-encoded LengthOfMatch code has a value of 0x0001. Applying the
bit length information from the first table in this section (4 bits), it
can be determined that the Huffman code in binary MUST be
0001.<span style="mso-bidi-font-size: 11.0pt; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin;"></span></span></span>

<span style="font-family: Calibri; font-size: small;">As another
example, it can be determined from the previous table that the 21st
Huffman-encoded LengthOfMatch code has a value of 0x009f. Applying the
bit length information from the first table in this section (8 bits), it
can be determined that the Huffman code in binary MUST be
10011111.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Table
6: Bit count and base value lookup tables to encode and decode
length-of-match
values<span style="font-family: &#39;Verdana&#39;,&#39;sans-serif&#39;; font-size: 9pt; mso-bidi-font-size: 10.0pt; mso-fareast-font-family: SimSun; mso-font-kerning: 12.0pt;"></span></span></span>

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">Index
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">LoMBitsLUT
</span></span></span>**

**<span style="letter-spacing: -0.1pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">LoMBaseLUT
</span></span></span>**

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">5</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">7</span>

<span style="font-family: Verdana; font-size: x-small;">0</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">10</span>

<span style="font-family: Verdana; font-size: x-small;">9</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">12</span>

<span style="font-family: Verdana; font-size: x-small;">10</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">11</span>

<span style="font-family: Verdana; font-size: x-small;">1</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">12</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">18</span>

<span style="font-family: Verdana; font-size: x-small;">13</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">22</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">26</span>

<span style="font-family: Verdana; font-size: x-small;">15</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">30</span>

<span style="font-family: Verdana; font-size: x-small;">16</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">34</span>

<span style="font-family: Verdana; font-size: x-small;">17</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">42</span>

<span style="font-family: Verdana; font-size: x-small;">18</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">50</span>

<span style="font-family: Verdana; font-size: x-small;">19</span>

<span style="font-family: Verdana; font-size: x-small;">3</span>

<span style="font-family: Verdana; font-size: x-small;">58</span>

<span style="font-family: Verdana; font-size: x-small;">20</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">66</span>

<span style="font-family: Verdana; font-size: x-small;">21</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">82</span>

<span style="font-family: Verdana; font-size: x-small;">22</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">98</span>

<span style="font-family: Verdana; font-size: x-small;">23</span>

<span style="font-family: Verdana; font-size: x-small;">4</span>

<span style="font-family: Verdana; font-size: x-small;">114</span>

<span style="font-family: Verdana; font-size: x-small;">24</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">130</span>

<span style="font-family: Verdana; font-size: x-small;">25</span>

<span style="font-family: Verdana; font-size: x-small;">6</span>

<span style="font-family: Verdana; font-size: x-small;">194</span>

<span style="font-family: Verdana; font-size: x-small;">26</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">258</span>

<span style="font-family: Verdana; font-size: x-small;">27</span>

<span style="font-family: Verdana; font-size: x-small;">8</span>

<span style="font-family: Verdana; font-size: x-small;">514</span>

<span style="font-family: Verdana; font-size: x-small;">28</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span style="font-family: Verdana; font-size: x-small;">29</span>

<span style="font-family: Verdana; font-size: x-small;">14</span>

<span style="font-family: Verdana; font-size: x-small;">2</span>

<span id="zfb37ff5718264811892b42266fdd5301"></span>

**<span style="font-family: Verdana; font-size: x-small;"> </span>**

<span style="font-family: Calibri; font-size: small;">The following flow
chart describes how literals and copy-tuples in a logically compressed
stream are encoded.
</span>

<span style="mso-no-proof: yes;"><span style="font-family: Calibri; font-size: small;"></span></span>

**<span style="font-family: Verdana;">Figure 8: Encoding a logically
compressed stream</span>**

<span style="font-family: Calibri; font-size: small;">Literals are
merely encoded using the Huffman Tables HuffCodeLEC (table 2) and
HuffLengthL (table 1). Copy-tuples are encoded using the four lookup
tables in table 3 and table 6 (CopyOffsetBaseLUT, CopyOffsetBitsLUT,
LoMBaseLUT, and LoMBitsLUT) and the Huffman Tables.</span>

<span style="font-family: Calibri; font-size: small;">The flowchart
shows that the initial decision is whether we are dealing with a literal
or not. If we are working with a literal then the process is fairly
simple. Encode the literal using Huffman tables using table 1 and 2 and
output the Encoded bits. If the data does not represent a literal then
we are dealing with outputting a copy-tuple which consists of a
\<copy-offset, length-of-match\> pair, as previously mentioned.</span>

<span style="font-family: Calibri; font-size: small;">For the copy-tuple
there are two encodings that need to be performed, one for the
copy-offset and one for the length-of-match (LoM).
</span>

**<span style="font-size: small;"><span style="font-family: Calibri;">Encoding
the Length of Match:</span></span>**

<span style="font-family: Calibri; font-size: small;">Encoding of the
length-of-match is shown in the previous figure by the Action B item.
The following describes the algorithm for encoding the
length-of-match.</span>

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 0.15in; margin-right: 0in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">LUTIndex
= IndexOfEqualOrSmallerEntry(LoM,
LoMBaseLUT)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits(HuffCodeL\[LUTIndex\],
HuffLengthL\[LUTIndex\])</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"></span>

 

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBitsLength
=
LoMBitsLUT\[LUTIndex\]</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBits
= (LoM - 2) & ((2 ^ ExtraBitsLength) -
1)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits
(ExtraBits,
ExtraBitsLength</span><span style="font-size: 10pt; mso-bidi-font-size: 8.0pt;"><span style="font-family: Courier New;">)</span></span>

</div>

**<span style="font-size: small;"><span style="font-family: Calibri;">Sample
Walkthrough:</span></span>**

<span style="font-family: Calibri; font-size: small;">Now that we are
armed with all of this information and perhaps a little dangerous lets
walk through the sample data to see what data the encoder should spit
out. The sample data we will use
is:</span>

<span style="letter-spacing: 1pt; font-size: 12pt; mso-bidi-font-size: 10.0pt; mso-font-width: 80%;"><span style="font-family: Calibri;">Offset<span style="mso-spacerun: yes;">   
</span>0<span style="mso-spacerun: yes;">   
</span>1<span style="mso-spacerun: yes;">   
</span>2<span style="mso-spacerun: yes;">   
</span>3<span style="mso-spacerun: yes;">   
</span>4<span style="mso-spacerun: yes;"> 
</span><span style="mso-spacerun: yes;"> </span><span style="mso-spacerun: yes;"> </span>5<span style="mso-spacerun: yes;">   
</span>6<span style="mso-spacerun: yes;"> 
</span><span style="mso-spacerun: yes;"> </span><span style="mso-spacerun: yes;"> </span>7<span style="mso-spacerun: yes;">   
</span>8<span style="mso-spacerun: yes;">   
</span>9<span style="mso-spacerun: yes;"> 
</span>10<span style="mso-spacerun: yes;"> 
</span>11<span style="mso-spacerun: yes;"> 
</span>12<span style="mso-spacerun: yes;"> 
</span>13<span style="mso-spacerun: yes;"> 
</span>14<span style="mso-spacerun: yes;">  </span>15
</span></span>

<span style="letter-spacing: 1pt; font-size: 12pt; mso-bidi-font-size: 10.0pt; mso-font-width: 80%;"><span style="font-family: Calibri;">Bytes<span style="mso-spacerun: yes;">   
</span>**01<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>0a<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>0a<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>20<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>20<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>80<span style="mso-spacerun: yes;"> 
</span>00<span style="mso-spacerun: yes;"> 
</span>80<span style="mso-spacerun: yes;"> 
</span>00**</span></span>

**<span style="letter-spacing: 1pt; font-size: 12pt; mso-bidi-font-size: 10.0pt; mso-font-width: 80%;"></span>**

<span style="font-family: Calibri;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
1 Encoding a
literal</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">01<span style="mso-spacerun: yes;">                                
</span>For this data we use tables one and 2. We use the value of 1 to
index into both tables. Table 1 gives us a value of 6 for the length and
0x24 for the value. Doing so we get the output of
0x24:6</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">literal
6 bit code 36
(0x24)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x24:6</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
2 Encoding a
literal</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">00<span style="mso-spacerun: yes;">            
</span><span style="mso-spacerun: yes;">                  </span>Again
it’s a literal so use tables 1 and 2 in the same manner. Output is
0x04:6</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
3 Encoding the Copy-Offset for the match – Action A in the
Flowchart</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">00
00
</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Match
(non
cached)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">At
1, length
2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">(Back
1 position)<span style="mso-spacerun: yes;">                       
</span>Looking back at the history buffer we see that we have a match at
index 1 (back 1 position) and the match has a length of 2. We need to
use the algorithm for encoding a copy-tuple to obtain the copy-offset
and Extra match bits if any. Here is that algorithm:
</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="font-size: small;"><span style="font-family: Calibri;">Encoding
the
Copy-Offset:</span></span>**

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 0.15in; margin-right: 0in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">LUTIndex
= IndexOfEqualOrSmallerEntry(CopyOffset + 1,
CopyOffsetBaseLUT)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">HuffmanIndex
= LUTIndex +
257</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits(HuffCodeLEC\[HuffmanIndex\],
HuffLengthLEC\[HuffmanIndex\])</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"></span>

 

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBitsLength
=
CopyOffsetBitsLUT\[LUTIndex\]</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBits
= CopyOffset & ((2 ^ ExtraBitsLen) -
1)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits(ExtraBits,
ExtraBitsLength)</span>

</div>

****

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">The
IndexOfEqualOrSmallerEntry function searches through the specified LUT
table and returns the index of the entry that contains a value of equal
or lesser value than the first parameter. The OutputBits function
outputs the bits specified by the first parameter in the appropriate
order (the number of bits to output is given by the second parameter).
"^" is the exponentiation operator, and "&" is the bitwise AND
operator.</span>

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 24.5pt; margin-right: 0.25in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; letter-spacing: 1pt; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-font-width: 80%;">Insert
our values, note that the
</span><span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">IndexOfEqualOrSmallerEntry
function uses the amount of the offset (backup position) +1 as its first
parameter.<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span></span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; letter-spacing: 1pt; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-font-width: 80%;"><span style="mso-spacerun: yes;">                                                                                
</span></span><span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">LUTIndex
= 1 <span style="letter-spacing: 1pt; mso-font-width: 80%;">(Table
3)</span></span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">            
</span><span style="mso-spacerun: yes;">                                                                    </span><span style="mso-spacerun: yes;">                    </span>HuffmanIndex
= 1 + 257 =
258</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">                                                                                
</span><span style="mso-spacerun: yes;">                    </span>OutputBits(0x0039,
7)<span style="mso-spacerun: yes;">  </span>obtain the value and length
from Huffman tables 1 and
2</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"></span>

 

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">                                                                               
</span><span style="mso-spacerun: yes;">                    </span><span style="mso-spacerun: yes;"> </span>ExtraBitsLength
=
2</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">                                                               
</span><span style="mso-spacerun: yes;">                             </span><span style="mso-spacerun: yes;">       </span><span style="mso-spacerun: yes;"> </span>ExtraBits
= 8 & ((2 ^ 2) - 1) = 8 & 3 = bin:1000 & bin:0011 =
0</span>

</div>

<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes;">                                                                                                           
</span><span style="mso-spacerun: yes;">    </span>OutputBits(0x00,<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">ExtraMatchBits
= 0 (from table 3 for LUTIndex
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x39:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
4 Encoding the Length of the match – Action B in the
flowchart</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
LoM</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Length
=
2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes;">  
</span>Using the following algorithm we calculate
LoM:</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="font-size: small;"><span style="font-family: Calibri;">Encoding
the Length of
Match:</span></span>**

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 0.15in; margin-right: 0in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">LUTIndex
= IndexOfEqualOrSmallerEntry(LoM,
LoMBaseLUT)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits(HuffCodeL\[LUTIndex\],
HuffLengthL\[LUTIndex\])</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"></span>

 

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBitsLength
=
LoMBitsLUT\[LUTIndex\]</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">ExtraBits
= (LoM - 2) & ((2 ^ ExtraBitsLength) -
1)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">OutputBits
(ExtraBits,
ExtraBitsLength</span><span style="font-size: 10pt; mso-bidi-font-size: 8.0pt;"><span style="font-family: Courier New;">)</span></span>

<span style="font-size: 10pt; mso-bidi-font-size: 8.0pt;"></span>

<span style="font-family: Courier New;"> </span>

<span style="font-size: 10pt; mso-bidi-font-size: 8.0pt;"></span>

<span style="font-family: Courier New;"> </span>

</div>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Calculation
using our
data:</span></span></span>

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 0in; margin-right: 0.25in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; letter-spacing: 1pt; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-font-width: 80%;"><span style="mso-spacerun: yes;">    
</span></span><span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;">LUTIndex
= 0 This comes from Table 6 using out LOM of
2</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">    
</span>HuffmanCode = 0x0001 (Index 0 of Table
5)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">    
</span>LoMBits = 4 (Index of 0 Table
4)</span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">    
</span>OutputBits(0x0001, 4) Output of Huffman code and length from
Tables 5 and 4
respectively.</span>

</div>

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 24.5pt; margin-right: 0.25in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"></span>

 

</div>

<div style="padding-bottom: 5pt; padding-left: 6pt; padding-right: 6pt; margin-left: 0in; margin-right: 0.25in; padding-top: 4pt; mso-element: para-border-div; border: white 3pt solid;">

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">    
</span>ExtraBitsLength = 0
<span style="letter-spacing: 1pt; mso-font-width: 80%;">(from table 3
for LUTIndex
0)</span></span>

<span style="font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-bidi-font-size: 10.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Times New Roman&#39;;"><span style="mso-spacerun: yes;">    
</span>No extra bits to
output</span>

</div>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
5 Encode
Literal</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">0a</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes;"> </span>Using
the value a (dec 10) <span style="mso-spacerun: yes;"> </span>as the
index into Tables 1 and 2 we obtain a length of 7 and a code of
<span style="mso-spacerun: yes;"> </span>0x69</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x69:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
6 Found a Match Encode the Copy-Offset and LoM
</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">00
0a
00</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Match</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">At
3, length 3 (non
cached)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">(Back
2
positions)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
Copy-Offset</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LUTIndex
= 2 (Table
3)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanIndex
= 2 + 257 = 259
(0x103)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Index
0x103 -\> Huffman Code 0x0079 (from table
2)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Bits
for Index 259<span style="mso-spacerun: yes;">  </span>-\> 0x7 (from
table
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">ExtraMatchBits
= 0 (from Table 3 for LUTIndex
2)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x79:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
LoM</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Length
=
3</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LUTIndex
= 1 (Table
6)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanCode
for LoM 0x0000 (Index 1 Table
5)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LoMBits
= 0x2 (index 1 Table
4)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">ExtraMatchBits
= 0 (from Table 3 for LUTIndex
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x0:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
6 Encode
Literal</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">20</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">literal
8 bit code 77
(0x4d)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x4d:8</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
7 Found a Match \[Cached\] Encode the Copy-Offset and LoM
</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">00
20
00</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Match
(cached)-- Last Offset used was also back 2 postions so we have a match
with LRU cache index
0</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Looking
back at the flowchart you can see what happens when we get a cache
hit.</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">At
7, length
3</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">(Back
2
positions)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encode
OffsetCacheIndex</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Index
+ 289 = 0 + 289 = 289
(0x121)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanCode
for 289 -\> 0x0018 (table
2)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Bits
for Index 289 -\> 0x5 (from Table
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x18:5</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encode
the LoM as previously
performed.</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
LoM</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Length
=
3</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LUTIndex
= 1 (Table
6)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanCode
for LoM 0x0000 (Index 1 Table
5)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LoMBits
= 0x2 (index 1 Table
4)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">ExtraMatchBits
= 0 (from Table 3 for LUTIndex
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x0:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
8 Encode
Literal</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">80</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">literal
7 bit code 25
(0x0019)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x19:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Step
7 Found a Match \[Cached\] Encode the Copy-Offset and LoM
</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">00
80
00</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Match
(cached)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">At
11 (0xb), length
3</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">(Back
2
positions)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Last
Offset used was also back 2 postions so we have a match with LRU cache
index
0</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encode
OffsetCacheIndex</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Index
+ 289 = 0 + 289 = 289
(0x121)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanCode
for 289 -\> 0x0018 (table
2)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Bits
for Index 289 -\> 0x5 (from Table
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x18:5</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
LoM</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Length
=
3</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LUTIndex
= 1 (Table
6)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">HuffmanCode
for LoM 0x0000 (Index 1 Table
5)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">LoMBits
= 0x2 (index 1 Table
4)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">ExtraMatchBits
= 0 (from Table 3 for LUTIndex
1)</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output
0x0:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Encoding
Finished\!</span></span></span>**

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Note
Wire Format:</span></span></span>**

<span style="font-family: Calibri; font-size: small;">The compressed
stream consists of a bit-encoded sequence of literals, \<copy-offset,
length-of-match\> tuples (also known as copy-tuples), and a terminating
End-of-Stream (EOS) marker. The convention used on the stream is that
the sender MUST output bytes in little-endian order with the most
significant bit
first.</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Input
: 01 00 00 00 0a 00 0a 00 20 00 20 00 80 00 80
00</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;">Output:
0x24:6</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x04:6</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x39:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x01:4</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x69:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x79:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x00:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x4d:8</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x18:5</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x00:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x19:7</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x18:5</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">         
</span>0x00:2</span></span></span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="letter-spacing: 1pt; mso-font-width: 80%;"><span style="font-family: Calibri; font-size: small;">Output
of the Encoder:
</span></span><span style="font-family: &#39;Courier New&#39;; font-size: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;;">24
91 8b 74 9e 26 4c 06 13 be bf b6 80 06 4c 77 4c 4f 4c 2f 2c 01 d1 6b
41…</span><span style="letter-spacing: 1pt; mso-font-width: 80%;"></span>

<span style="font-family: &#39;Courier New&#39;; font-size: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;;">There
you have it\! This stream will be byte transposed into little-endian
format and then sent over the wire to the decoder. Look for **Part 2 RDP
6.0 Decompressing Data Walkthrough** coming soon to a blog near
you.</span>
