<div id="page">

# Common mistakes when reading TS\_ICON\_INFO in \[MS-RDPERP\]

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
6/25/2010 1:47:17 PM

-----

<div id="content">

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Hi,
today I’ll brief you about some common mistakes when reading the
TS\_ICON\_INFO structure. This structure is defined in section 2.2.1.2.3
of \[MS-RDPERP\].</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">I’ve
been working on a case lately that (I can tell the sin but not the
sinner) will be a good example for this post.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">It
happens that the Icons that were shown on the taskbar after successfully
creating a RAIL session were both “upside down” AND “with wrong
coloring”.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">These
effects together with “truncated icons” are usually related to either or
all of these 3
situations:</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin;"><span style="mso-list: Ignore;"><span style="font-family: Calibri; font-size: small;">1)</span><span style="font: 7pt &#39;Times New Roman&#39;;">     
</span></span></span><span style="font-size: small;"><span style="font-family: Calibri;">CbColorTable
and ColorTable are expected but the Bpp is not 1, 4 nor
8.</span></span>

<span style="mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin;"><span style="mso-list: Ignore;"><span style="font-family: Calibri; font-size: small;">2)</span><span style="font: 7pt &#39;Times New Roman&#39;;">     
</span></span></span><span style="font-size: small;"><span style="font-family: Calibri;">The
RGB being used is 565 instead of
555.</span></span>

<span style="mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin;"><span style="mso-list: Ignore;"><span style="font-family: Calibri; font-size: small;">3)</span><span style="font: 7pt &#39;Times New Roman&#39;;">     
</span></span></span><span style="font-size: small;"><span style="font-family: Calibri;">The
data from the bitmap is being read upside
down.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="text-decoration: underline;"><span style="font-size: small;"><span style="font-family: Calibri;">CbColorTable
and ColorTable are expected but the Bpp is not 1, 4 nor
8.</span></span></span>**

<span style="font-size: small;"><span style="font-family: Calibri;">\[MS-RDPERP\]
2.2.1.2.3<span style="mso-spacerun: yes;">   </span>Icon Info
(TS\_ICON\_INFO) defines the following
structure:</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">0</span></span></span>

</div>

</div>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">1</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">2</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">3</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">4</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">5</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">6</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">7</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">8</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">9</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">1  
0</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">1</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">2</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">3</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">4</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">5</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">6</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">7</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">8</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">9</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">2  
0</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">1</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">2</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">3</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">4</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">5</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">6</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">7</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">8</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">9</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;"><span style="font-size: x-small;"><span style="font-family: Verdana;">3  
0</span></span></span>

<span style="mso-bidi-font-size: 9.0pt;">  
<span style="font-size: x-small;"><span style="font-family: Verdana;">1</span></span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">CacheEntry</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">CacheId</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">Bpp</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">Width</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">Height</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;"><span style="background: yellow; mso-highlight: yellow;">CbColorTable
(optional)</span></span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">CbBitsMask</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">CbBitsColor</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">BitsMask
(variable)</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">...</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;"><span style="background: yellow; mso-highlight: yellow;">ColorTable
(variable)</span></span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">...</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">BitsColor
(variable)</span></span>

<span style="font-size: x-small;"><span style="font-family: Verdana;">...</span></span>

<span style="font-family: Verdana;"><span class="Bold">**CacheEntry (2
bytes):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
16-bit integer. The index within an icon cache at which this icon MUST
be stored at the client. The index is unique within a given
<span class="Bold">**CacheId**</span> (see following description). The
maximum value of <span class="Bold">**CacheEntry**</span> is negotiated
between server and client through the
<span class="Bold">**NumIconCacheEntries**</span> field of the
</span>[<span style="font-family: Verdana; color: #0066ff;">Window List
Capability
Set</span>](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx?SelectedNavItem=NewPost&WeblogID=12205#z82ec7a69f7e34294830d666178b35d15)<span style="font-family: Verdana;">
during the connection establishment phase.</span>

<span style="font-family: Verdana;"><span class="Bold">**CacheId (1
byte):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
8-bit integer. The index of the icon cache at which this icon MUST be
stored at the client. If the value is 0xFFFF, the icon SHOULD NOT be
cached. The <span class="Bold">**CacheId**</span> is unique within a
remote session. </span>

<span style="font-family: Verdana;">The maximum value of
<span class="Bold">**CacheId**</span> is negotiated between server and
client through the <span class="Bold">**NumIconCaches**</span> field of
the Window List Capability Set while establishing the connection.</span>

<span style="font-family: Verdana;"><span class="Bold">**Bpp (1
byte):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
8-bit integer. The color depth of the icon. Valid values are as
follows:</span>

<span style="font-family: Verdana;">1  
4  
8  
16  
24  
32  
  
</span>

<span style="font-family: Verdana;"><span class="Bold">**Width (2
bytes):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
16-bit integer. The width, in pixels, of the icon.</span>

<span style="font-family: Verdana;"><span class="Bold">**Height (2
bytes):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
16-bit integer. The height, in pixels, of the
icon.</span>

<span style="font-family: Verdana;"><span class="Bold"><span style="background: yellow; mso-highlight: yellow;">**CbColorTable
(2 bytes):<span style="mso-spacerun: yes;"> 
</span>**</span></span><span style="background: yellow; mso-highlight: yellow;">An
unsigned 16-bit integer. The size, in bytes, of the color table data.
This field is ONLY present if the
</span><span class="Bold"><span style="background: yellow; mso-highlight: yellow;">**bits
per pixel
(Bpp)**</span></span><span style="background: yellow; mso-highlight: yellow;">
value is 1, 4, or 8.</span></span>

<span style="font-family: Verdana;"><span class="Bold">**CbBitsMask (2
bytes):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
16-bit integer. The size, in bytes, of the icon's one-bit color-depth
mask image.</span>

<span style="font-family: Verdana;"><span class="Bold">**CbBitsColor (2
bytes):<span style="mso-spacerun: yes;">  </span>**</span>An unsigned
16-bit integer. The size, in bytes, of the icon's color image.</span>

<span style="font-family: Verdana;"><span class="Bold">**BitsMask
(variable):<span style="mso-spacerun: yes;">  </span>**</span>The image
data for the 1-bpp bitmap. The length, in bytes, of this field is equal
to the value of <span class="Bold">**CbBitsMask**</span>. This field is
optional.
</span>

<span style="font-family: Verdana;"><span class="Bold"><span style="background: yellow; mso-highlight: yellow;">**ColorTable
(variable):<span style="mso-spacerun: yes;"> 
</span>**</span></span><span style="background: yellow; mso-highlight: yellow;">The
image data for the color bitmap. The length, in bytes, of this field is
equal to the value of
</span><span class="Bold"><span style="background: yellow; mso-highlight: yellow;">**CbColorTable**</span></span><span style="background: yellow; mso-highlight: yellow;">.
This field is only present if the
</span><span class="Bold"><span style="background: yellow; mso-highlight: yellow;">**Bpp**</span></span><span style="background: yellow; mso-highlight: yellow;">
value is 1, 4, or 8.</span></span>

<span style="font-family: Verdana;"><span class="Bold">**BitsColor
(variable): <span style="mso-spacerun: yes;"> </span>**</span>The image
data for the icon's color image. The length, in bytes, of this field is
equal to the value of <span class="Bold">**CbBitsColor**</span>. This
field is optional.</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">As
you can see in the highlighted information, CbColorTable’s 2 bytes are
only present if the Bpp is equals to 1, 4 or 8. With higher color
depths, these two properties are not present in the
structure.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">If
assumed present, then the CbBitsMask 2 bytes are read as the ColorTable
size, the CbBitsColor 2 bytes are read as the BitsMask size and the
first 2 bytes of the BitsMask are read as the BitsColor
size.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">You
can probably imagine that after this skewed data is read, the tables
will be interpreted with the wrong data thus garbage like icons will
appear. It may resemble the icon to some extent, but it will certainly
not be
it.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="text-decoration: underline;"><span style="font-size: small;"><span style="font-family: Calibri;">The
RGB being used is 565 instead of 555</span></span></span>**

<span style="font-size: small;"><span style="font-family: Calibri;">When
using 16 Bpp, it is very important to make sure that we use RGB555 and
not RGB565.</span></span>

<span style="font-family: Calibri; font-size: small;">Microsoft
BITAMAPINFOHEADER structure
(</span>[<span style="line-height: 115%; mso-ascii-font-family: Calibri; mso-ascii-theme-font: minor-latin; mso-hansi-font-family: Calibri; mso-hansi-theme-font: minor-latin; mso-ansi-font-size: 11.0pt; mso-bidi-font-size: 11.0pt;"><span style="font-family: Calibri; font-size: small;">http://msdn.microsoft.com/en-us/library/dd183376(v=VS.85).aspx</span></span>](http://msdn.microsoft.com/en-us/library/dd183376\(v=VS.85\).aspx)<span style="font-size: small;"><span style="font-family: Calibri;">
) defines this very
clearly:</span></span>

<span style="line-height: 115%; font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;; background: yellow; color: black; font-size: 10pt; mso-highlight: yellow;">“The
value for blue is in the least significant five bits, followed by five
bits each for green and red. The most significant bit is not
used”</span><span style="line-height: 115%; font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;; color: black; font-size: 10pt;"></span>

<span style="font-family: Calibri;"><span style="font-size: small;">Using
the wrong scheme will, of course, result on the wrong coloring of the
icon since RGB565 uses six bits for the green
component.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

**<span style="text-decoration: underline;"><span style="font-family: Calibri;"><span style="font-size: small;">The
data from the bitmap is being read upside down</span></span></span>**

<span style="font-family: Calibri;"><span style="font-size: small;">This
is usually an easy to recognize
error.</span></span>

<span style="font-family: Calibri;"><span style="font-size: small;">Whenever
your icon is looking upside down, you can be almost certain that you are
reading the structure top to bottom instead of bottom to top or
vice-versa.</span></span>

<span style="font-family: Calibri; font-size: small;">When reading a
BITMAPINFO
(</span>[<span style="line-height: 115%; mso-ascii-font-family: Calibri; mso-ascii-theme-font: minor-latin; mso-hansi-font-family: Calibri; mso-hansi-theme-font: minor-latin; mso-ansi-font-size: 11.0pt; mso-bidi-font-size: 11.0pt;"><span style="font-family: Verdana; color: #0066ff; font-size: small;">http://msdn.microsoft.com/en-us/library/dd183375(v=VS.85).aspx)</span></span>](http://msdn.microsoft.com/en-us/library/dd183375\(v=VS.85\).aspx\))<span style="font-size: small;"><span style="font-family: Calibri;">
structure, it is necessary to rely on the value of the “height” property
to be able to figure out if the bitmap should be read in one direction
or another. This is the
excerpt:</span></span>

<span style="line-height: 115%; font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;; background: yellow; color: black; font-size: 10pt; mso-highlight: yellow;">“A
DIB consists of two distinct parts: a
**<span style="font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;;">BITMAPINFO</span>**
structure describing the dimensions and colors of the bitmap, and an
array of bytes defining the pixels of the bitmap. The bits in the array
are packed together, but each scan line must be padded with zeroes to
end on a
**<span style="font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;;">LONG</span>**
data-type boundary. If the height of the bitmap is positive, the bitmap
is a bottom-up DIB and its origin is the lower-left corner. If the
height is negative, the bitmap is a top-down DIB and its origin is the
upper left
corner.</span><span style="font-size: small;"><span style="font-family: Calibri;"><span style="background: yellow; mso-highlight: yellow;">
“</span></span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">I
hope to have helped you prevent these mistakes or at least I have
provided you with an explanation of why you have found these issues when
reading the bitmaps.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">I
wish you the best of the reads\!</span></span>
