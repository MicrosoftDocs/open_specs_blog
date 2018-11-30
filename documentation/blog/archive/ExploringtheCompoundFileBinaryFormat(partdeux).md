<div id="page">

# Exploring the Compound File Binary Format (part deux)

[tomjebo](https://social.msdn.microsoft.com/profile/tomjebo) 9/25/2009
1:53:00
PM

-----

<div id="content">

<span class="posttitle1"><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt">Exploring
the Compound File Binary Format (part
deux)</span></span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">In
this, part
</span><span style="FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt">[<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; COLOR: #669966; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">ni</span>](http://en.wikipedia.org/wiki/Ni_\(kana\))</span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
(pronounced ne; Japanese for deux), I pick up where we left off.  Where
were we?  I had just demonstrated that the IStorage::CopyTo() method, at
least Microsoft’s default  implementation provided in Windows’
ole32.dll, will indeed do what it claims which is to “…order the
contents of streams sequentially…".   As we discovered, however, the
data in the ministream is not ordered as the other “standard” streams
are.  As a refresher, the ministream is also known as the “Root Entry”
stream and is where all streams that contain less than 4096 bytes of
data reside.   After using the CopyTo() method, we noticed that the
ministream was still discontiguous (new word).  We reasoned that
although the streams in the ministream are written by CopyTo() in the
same manner that all the others are, the ministream needs to grow and
therefore must interleave allocations of sectors with the other streams
as well as the internal structures of the compound file like the FAT,
DiFAT, Directory, etc…  It’s growth doesn’t happen once but possibly
many times and other sector allocations happen in between these spurts
of growth.  Hence the fragmentation.
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">To
verify this behaviour, I created a sample compound file and wrote three
streams to it in such a sequence that it would most likely produce
fragmentation.  The algorithm went something like **Figure
0.**</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">1.
Open three streams: stm1, stm2,
stm3</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">2.
write 2 minisectors worth of data to
stm1</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">3.
write 1 minisector worth of data to
stm2</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">4.
delete
stm1</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">5.
commit the root storage
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">6.
write 8 minisectors worth of data to
stm3</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">7.
commit the root storage
again</span>

</div>

</div>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
0: Stream fragmentation algorithm
</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">I
wrote ‘1’s to stm1, ‘2’s to stm2, ‘3’s to stms3 so they would be easily
identifiable.  After using this algorithm, I had a ministream that
looked like **Figure
1.**</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">0C00h:
32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32  2222222222222222  
0C10h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
0C20h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
0C30h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
...  
1000h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1010h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1020h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1030h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
...  
1200h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
1210h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
1220h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
1230h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
1240h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1250h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1260h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
1270h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
… (more
33’s)</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">13F0h:
33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333</span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
1: Fragmented
streams</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
ellipses (except for the last) are segments of the file that don’t
contain valid stream data.  As you can see, stm2 and stm3 are both
fragmented.  stm2 starts at 0C00h for 64 bytes but then discontinues at
0C30h and starts again at 1200h.  Likewise, stm3 starts at 1000h and
temporarily ends at 1030h, continuing at 1240h.  Normally, the data is
not “marked” nicely like this and is usually spread across a more vast
number of sectors making it very tedious to piece together. 
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
goal: to cause the stream data to defragment or order itself
contiguously without having to read the raw bytes, traverse all the
internal structures and rewrite the entire compound file. 
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">This
is actually fairly straight forward, although the resulting code is more
complex than our
</span><span style="FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt">[<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; COLOR: #669966; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">first
attempt</span>](http://blogs.msdn.com/openspecification/archive/2009/07/24/exploring-the-compound-file-binary-format.aspx)</span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
at defragmenting the stream data in the file.  As you might remember
from last time, we simply copied the entire compound file to another one
named \<filename\>.defrag.\<ext\> using the IStorage::CopyTo().  As
explained, earlier, this left the ministream potentially fragmented.  We
can use the same approach, copying element by element to a new location
but will have to separate the copying of the ministream from the rest of
the streams, storages and all internal structures (FAT, DiFAT,
Directory…).  In order to do this, we need to save the ministream
somewhere, copy just the remaining elements and then write the
ministream elements, one by one so that each element is written in its
entirety before writing the next.  This is what ensures contiguity. 
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Refining
the algorithm to make it easier to understand resulted in the following
steps:</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">1.
Open the source compound
file</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">2.
Open the target (to be defragmented) compound
file</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">3.
Open a new temporary storage or compound file
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">4.
Copy the entire source compound file to the temporary
storage</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">5.
Copy only the non-ministream elements of the source compound file to the
target file using IStorage::CopyTo() like before
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">6.
Copy the ministream elements to the
target</span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
2: General algorithm to separate copying of ministream and
non-ministream
elements</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">This
algorithm will cause the non-ministream elements to be written first and
then the ministream elements, avoiding the interlacing allocation of
sectors.  However, in order for step \#5 to work, we need to remove the
ministream elements from the source (after saving them of course) before
the CopyTo(). Also, in order to remove the ministream elements from a
compound file, you basically need to traverse the entire tree structure,
testing streams for sizes of \< 4096 and then call
IStorage::DestroyElement() on them.  Because this is a
</span><span style="FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt">[<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; COLOR: #669966; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">tree
structure</span>](http://en.wikipedia.org/wiki/Red_black_tree)</span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">,
iterating over the tree can be made much more readable by using
recursion.  We love recursion, right?  In a recursive algorithm for a
tree we simply enumerate the elements in the current storage object, and
if the current element is a stream, we can test it for residence in the
ministream (\<4096).  If it’s not a stream, simply recurse or call the
containing function again passing this current element as the argument. 
Pretty simple. 
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Here's
the pseudo code for the
algorithm:</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span> 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">function
remove\_mini(element)  
{  
<span style="mso-spacerun: yes">     </span>while (more\_elements)  
<span style="mso-spacerun: yes">    
</span>{</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">         
element.</span>enum\_elements(\&current))</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">  <span style="mso-spacerun: yes">       
</span>if (current is a stream)  
  <span style="mso-spacerun: yes">        </span>{  
   <span style="mso-spacerun: yes">            </span>if (current \<
4096)  
    <span style="mso-spacerun: yes">                </span>delete
current;  
  <span style="mso-spacerun: yes">        </span>}  
 <span style="mso-spacerun: yes">        </span> else  
   <span style="mso-spacerun: yes">           
</span>remove\_mini(current);  
 <span style="mso-spacerun: yes">    
</span>}  
}</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">**Figure
3a: Recursive function to remove ministream elements from a storage
object**</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">****</span> 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
function to “prune” the ministreams out of a storage looks like
this:</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\#define
LESSTHAN(x,y) ((x.HighPart \<= y.HighPart) && (x.LowPart \<
y.LowPart))</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">  
HRESULT PruneStgMiniStream(LPSTORAGE lpSrcStg)  
{  
    HRESULT hr = S\_OK;  
    LPENUMSTATSTG lpEnumStatStg = NULL;  
    hr  = lpSrcStg-\>EnumElements(NULL, NULL, NULL, \&lpEnumStatStg);  
    ULONG cfch = 0;  
    STATSTG tstat;  
    while (lpEnumStatStg-\>Next(1, \&tstat, \&cfch)==S\_OK)  
    {  
        ULARGE\_INTEGER mini\_limit = {4096,0};
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">       
// if it's a stream, it's not 0 bytes and it's less than 4096 bytes,
then we delete it.  
        if ((tstat.type == STGTY\_STREAM) &&  
            ((tstat.cbSize.LowPart \> 0) || (tstat.cbSize.HighPart \>
0)) &&  
             (LESSTHAN(tstat.cbSize, mini\_limit)))  
            lpSrcStg-\>DestroyElement(tstat.pwcsName);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">       
if (tstat.type == STGTY\_STORAGE)  
        {  
            LPSTORAGE lpNewStg = NULL;  
            hr = lpSrcStg-\>OpenStorage(tstat.pwcsName, NULL,  
                STGM\_READWRITE|STGM\_SHARE\_EXCLUSIVE , NULL, NULL,
\&lpNewStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">           
PruneStgMiniStream(lpNewStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">           
lpNewStg-\>Release(); // Caller releases.  
        }  
    }
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">   
lpEnumStatStg-\>Release();  
    return
hr;  
}</span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
3b: Recursive function to remove ministream elements from a storage
object</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">PruneStgMiniStream()
is called again within itself, passing lpNewStg as the new lpSrcStg
parameter. 
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">So
once steps 1-5 (**Figure 2)** have been accomplished, the ministreams
have to be copied from the temporary storage to the target.  This is a
little more complicated because we can’t just create a ministream
element wherever we want, i.e. in the root storage of the target.  We
have to make sure that each ministream element gets copied to its proper
branch in the tree.  Thinking about this reminded me of those mechanical
tracing devices we played with as kids allowing us to copy a drawing by
having a
</span><span style="FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt">[<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; COLOR: #669966; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">mechanical
arm</span>](http://en.wikipedia.org/wiki/Planimeter)</span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
mimic each movement.  In the same way, the algorithm can simply use the
temporary storage tree structure as the traced shape, telling us where
to deposit the ministream elements in the target, which is missing
them.  I was able to use the same basic form as the
PruneStgMiniStream() function but instead of deleting the ministream
elements, it creates the element in the target and copies the data. 
Here’s the resulting function to copy the ministream elements to the
target:</span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt">Here's
the pseudo code for the
algorithm:</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">function
copy\_mini(src\_element, dst\_element)  
{  
<span style="mso-spacerun: yes">     </span>while (more\_elements)  
<span style="mso-spacerun: yes">    
</span>{</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">         
</span>src\_element.enum\_elements(\&src\_current))</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">  <span style="mso-spacerun: yes">       
</span>if (src\_current is a stream)  
  <span style="mso-spacerun: yes">        </span>{  
   <span style="mso-spacerun: yes">            </span>if (src\_current
\< 4096)  
    <span style="mso-spacerun: yes">               
</span>dst\_element.create(src\_current.name,
\&dst\_current);</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">                   
</span>src\_current.copyto(dst\_current);  
  <span style="mso-spacerun: yes">        </span>}  
 <span style="mso-spacerun: yes">       
</span> else</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">         
</span>{</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">              
</span>src\_element.open(\&src\_current);</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">              
</span>dst\_element.open(\&dst\_current);  
   <span style="mso-spacerun: yes">           
</span>copy\_mini(src\_current,
dst\_current);</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">          
</span>}  
 <span style="mso-spacerun: yes">    
</span>}  
}</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi">Figure
4a: Recursive function to remove ministream elements from a storage
object</span>**

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi"></span>** 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi"></span>

Here’s the resulting function to copy the ministream elements to the
target:

** **

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi"></span>**

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">HRESULT
CopyStgMiniStream(LPSTORAGE lpSrcStg, LPSTORAGE lpDestStg)  
{  
    HRESULT hr = S\_OK;  
    LPENUMSTATSTG lpEnumStatStg = NULL;  
    hr  = lpSrcStg-\>EnumElements(NULL, NULL, NULL, \&lpEnumStatStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">   
// The assumption here is that the two input storages are identical
except for  
    // the ministream.  Therefore, we only need to enumerate on one
since the other structure  
    // will be consistent.  However, when we recurse, we drop down a
level in the tree which means  
    // we need to do that to both storages.
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">   
ULONG cfch = 0;  
    STATSTG tstat;  
    while (lpEnumStatStg-\>Next(1, \&tstat, \&cfch)==S\_OK)  
    {  
        LPSTREAM lpStmSrcTemp = NULL;  
        LPSTREAM lpStmDestTemp = NULL;  
        ULARGE\_INTEGER nread, nwritten;  
        ULARGE\_INTEGER mini\_limit = {4096,0};
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">       
// if it's a stream, it's not 0 bytes and it's less than 4096 bytes,
then we copy it.  
        if ((tstat.type == STGTY\_STREAM) &&  
            ((tstat.cbSize.LowPart \> 0) || (tstat.cbSize.HighPart \>
0)) &&  
                (LESSTHAN(tstat.cbSize, mini\_limit)))  
        {  
            hr = lpSrcStg-\>OpenStream(tstat.pwcsName, NULL,  
                STGM\_READ|STGM\_SHARE\_EXCLUSIVE, NULL,
\&lpStmSrcTemp);  
            hr = lpDestStg-\>CreateStream(tstat.pwcsName,  
                STGM\_WRITE|STGM\_SHARE\_EXCLUSIVE, NULL, NULL,
\&lpStmDestTemp);  
            hr = lpStmSrcTemp-\>CopyTo(lpStmDestTemp, tstat.cbSize,
\&nread, \&nwritten);  
            lpStmSrcTemp-\>Release();  
            lpStmDestTemp-\>Release();  
        }  
        if (tstat.type == STGTY\_STORAGE)  
        {  
            LPSTORAGE lpNewSrcStg = NULL;  
            LPSTORAGE lpNewDestStg = NULL;             
            hr = lpSrcStg-\>OpenStorage(tstat.pwcsName, NULL,  
                STGM\_READ|STGM\_SHARE\_EXCLUSIVE , NULL, NULL,
\&lpNewSrcStg);  
            hr = lpDestStg-\>OpenStorage(tstat.pwcsName, NULL,  
                STGM\_READWRITE|STGM\_SHARE\_EXCLUSIVE , NULL, NULL,
\&lpNewDestStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">           
CopyStgMiniStream(lpNewSrcStg, lpNewDestStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">           
lpNewSrcStg-\>Release(); // Caller releases.  
            lpNewDestStg-\>Release(); // Caller releases.  
        }  
    }  
    lpEnumStatStg-\>Release();  
    return
hr;  
}</span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
4: Function to copy ministream elements to a target storage
object.</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
important functions have been written, now to just put it all
together.  The ministreams have to be deleted from either the source
or the temporary storage before copying to the target storage.  Because
we don’t want to modify the source storage, it’s better to use the
temporary storage for “pruning”, then copy all but the ministream
elements from the temporary to the target storage.   So the code uses
the algorithm but switching the source and temporary
storages: </span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">…
\<omitted redundant code from original blog\>
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">LPSTORAGE
lpTempStg =
NULL;</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= StgCreateStorageEx(lptszTemp,
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes"> </span>STGM\_CREATE|STGM\_TRANSACTED|STGM\_READWRITE|
STGM\_SHARE\_EXCLUSIVE,
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes"> </span>STGFMT\_STORAGE, 0, NULL,
NULL, IID\_IStorage, (LPVOID\*) \&lpTempStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">LPSTORAGE
lpDestStg =
NULL;</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
=
StgCreateStorageEx(lptszTarget,</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-tab-count: 2">                       
</span>STGM\_CREATE|STGM\_TRANSACTED | STGM\_READWRITE |
STGM\_SHARE\_EXCLUSIVE,</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-tab-count: 2">                       
</span>STGFMT\_STORAGE, 0, NULL, NULL, IID\_IStorage, (LPVOID\*)
\&lpDestStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">…
\<omission\>
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= lpSrcStg-\>CopyTo(NULL, NULL, NULL, lpTempStg);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">//
this deletes all the streams that live in the
ministream</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= PruneStgMiniStream(lpTempStg);<span style="mso-spacerun: yes"> 
</span></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">//
copy everything but the ministream
data</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= lpTempStg-\>CopyTo(NULL, NULL, NULL,
lpDestStg);<span style="mso-spacerun: yes"> 
</span></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">//
now copy the ministreams to the destination
storage</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= CopyStgMiniStream(lpSrcStg,
lpDestStg);<span style="mso-spacerun: yes"> 
</span></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">hr
= lpDestStg-\>Commit(STGC\_CONSOLIDATE);
</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">…
\<omission\></span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
5: mainline code
</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">I’ve
omitted code that didn’t change much.  You can see that a new call to
StgCreateStorageEx() creates the temp storage.   Also, it’s important to
Commit() the temporary storage after “pruning” the ministream elements
as well as Commit()’ing the destination storage after copying to it. 
This ensures that the tree structure and data are written to disk in
their final form.  The resulting file (**Figure 6)** has a ministream
that is contiguous and therefore easier to follow application structures
and
data:</span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">0C00h:
33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33  3333333333333333  
0C10h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C20h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C30h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C40h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C50h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C60h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C70h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C80h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0C90h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CA0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CB0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CC0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CD0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CE0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0CF0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D00h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D10h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D20h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D30h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D40h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D50h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D60h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D70h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D80h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0D90h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DA0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DB0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DC0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DD0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DE0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0DF0h: 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 
3333333333333333  
0E00h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
0E10h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
0E20h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 
2222222222222222  
0E30h: 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32  2222222222222222
</span>

**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
6: Defragmented
streams</span>**<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">I’ve
attached defrag.cpp to this blog so you can see the code in its
completeness.  Using only
</span><span style="FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt">[<span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; COLOR: #669966; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">structured
storage
API’s</span>](http://msdn.microsoft.com/en-us/library/aa380365\(VS.85\).aspx)</span><span style="FONT-FAMILY: &#39;Trebuchet MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 9pt; mso-bidi-font-size: 6.0pt; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">,
I’ve shown here that it’s possible to order the application stream data
in a compound file to be contiguous thereby making it much less
formidable to traverse. 
</span>

<span style="LINE-HEIGHT: 115%; FONT-SIZE: 16pt; mso-bidi-font-size: 11.0pt"></span>

 

[defrag.cpp](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/09/89/96/21/defrag.cpp)
