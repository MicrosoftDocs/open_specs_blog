<div id="page">

# An Exploration of CFB 5

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
4/27/2011 9:40:23
PM

-----

<div id="content">

**<span style="color: #365f91;"><span style="font-family: Cambria;"><span class="Heading1Char"><span style="line-height: 115%; font-size: 14pt;">An
Exploration of CFB
5  
  
</span></span></span></span>**

**<span style="font-size: small;"><span style="font-family: Calibri;">CFB
Mini FAT</span></span>**

<span style="font-size: small;"><span style="font-family: Calibri;">The
Mini FAT is used to allocate much smaller units of information into
equal-length sectors of 64-bytes. The first mini FAT sector is mapped by
the FAT and then the rest are found by following the chain. The offset
of any mini FAT sector can be calculated by a mere simple formula:
sector number x 64. Since the size of the mini FAT sectors depends on
the size of the FAT Sectors, the amount of mini fat sector fields per
sector relies heavily on the version of the file. Version 3 results in
128 fields while version 4 contains 1024 fields.</span></span>

*<span style="font-size: small;"><span style="font-family: Calibri;">if
majver ==
3:</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">               
</span>for i in range( 0, 127
):</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">               
</span><span style="mso-tab-count: 1;">                </span>f.read ( 4
)</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;">if
majver ==
4:</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 1;">               
</span>for i in range ( 0, 1023
):</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-tab-count: 2;">                               
</span>f.read ( 4 )</span></span>*

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
mini FAT sector is used to find the relevant mini streams. These streams
are found as subunits underneath the stream sectors. The streams
themselves are also 64 bytes long. They are chained exactly the same way
in the FAT as a normal stream. Reading this data can be done in the
following:</span></span>

*<span style="font-size: small;"><span style="font-family: Calibri;">f.seek
( MINISTREAMSECLOC
)</span></span>*

*<span style="font-size: small;"><span style="font-family: Calibri;">f.read
( 64
)</span></span>*

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">There
is no real way to parse the information found in the stream without the
given context. This is because the Mini Stream contains user-defined
data that can take on any particular form. Thus, without knowing what is
in the stream, we can simply read the data block-by-block if we are
merely making a general parser.</span></span>

</div>

</div>
