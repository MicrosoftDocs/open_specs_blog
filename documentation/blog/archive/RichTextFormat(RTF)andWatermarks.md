<div id="page">

# Rich Text Format (RTF) and Watermarks

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
2/4/2013 12:19:00
PM

-----

<div id="content">

 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">Seldom
is the question asked, *"Is there an RTF directive that can be used to
add watermarks in RTF
documents?*"</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">One
day recently this question found me, and after delving into the world of
the
[<span style="text-decoration: underline;"><span style="color: #0072c6;"><span style="color: #0072c6;"><span lang="">Rich
Text Format
(</span></span></span></span>](http://www.microsoft.com/en-us/download/details.aspx?id=10725)[<span style="text-decoration: underline;"><span style="color: #0072c6;"><span style="color: #0072c6;"><span lang="">RTF</span></span></span></span>](http://www.microsoft.com/en-us/download/details.aspx?id=10725)[<span lang="">)
specification</span>](http://www.microsoft.com/en-us/download/details.aspx?id=10725)<span lang="">
you may in turn be interested in what I found, which includes a new
appreciation for RTF.  At risk of repeating what is stated in many blogs
in many similar ways, the RTF format affords fluidity of fidelity over
time.  What this essentially means is that RTF format provides a
significant hedge against ever changing specifications.  Thus, RTF is
kind of a time capsule document format that can be opened in the future,
with the same fidelity potential encoded at the time it was
created.</span></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">Of
course, RTF implementations do differ from application to application. 
However, since applications SHOULD ignore control words they do not
implement they will only lack the ability to render the RTF at the same
fidelity encoded in the original document.  And, since RTF is a Rich
"Text" Format, the encoding is in plain text and not encoded in a more
abstruse Binary File Format.  So reading the format and implementing the
format is far more accessible by
comparison.</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">To
delve the RTF specification, refer to this
[<span style="text-decoration: underline;"><span style="color: #0072c6;"><span style="color: #0072c6;"><span lang="">blog</span></span></span></span>](http://blogs.office.com/b/microsoft-word/archive/2008/04/17/new-version-of-the-rich-text-format-rtf-specification.aspx)<span lang="">,
which references the
</span>[<span style="text-decoration: underline;"><span style="color: #0072c6;"><span style="color: #0072c6;"><span lang="">Word
2007: Rich Text Format (RTF) Specification, version
1.9.1</span></span></span></span>](http://www.microsoft.com/en-us/download/details.aspx?id=10725)<span lang="">
(the latest version at the time of this writing).  This is **the**
essential reference for implementing the RTF
specification.</span></span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">Now
that I’ve provided a little background and the necessary references,
I’ll restate the purpose of this blog, which is to answer the
question, "*Is there an RTF directive that can be used to add watermarks
in RTF
documents?*"</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">The
answer is derived from the RTF
specification.</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">First,
nowhere in the RFT specification will you find the word "Watermark" or
an explanation for it.  That’s because there is no Watermark control
code or object in the RTF specification per-se.  Watermarking is
implemented in RFT by various control codes that compose a Watermark,
and in this case also includes a special Word Watermark object that of
course is not part of the RTF specification (see
below).</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">An
easy way to discover the control codes for implementing a Watermark in
RTF (among other things) is to create a Watermark in Word and save the
file as an RTF document.  A Watermark in Word is a Shape (i.e. a Drawing
Object).  In RTF the shape that composes the Watermark begins with the
control word
"\\shp".</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">In
the RTF specification reference, refer to the section titled,
"<span style="color: #0072c6;"><span style="color: #0072c6;">Word 97
Through Word 2007 RTF for Drawing Objects (Shapes)</span></span>", which
details the meaning of the highlighted keywords below as they relate to
"\\shp". *The first destination (**\\shp**) is always present. This
control word groups everything related to a shape together. Following
the destination change is basic information regarding the shape. The
following keywords with values can appear in any order after the
"{**\\shp**" control
word.*</span><span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">*  
*</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">The
basic syntax for drawing objects in RTF is as
follows:</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<shape\></span>

</div>

</div>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{'
**\\shp** \<shpinfo\> \<shpinst\> \<shprslt\>
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<shpinfo\></span>

 

 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\\shpleft*N*</span><span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">?
**\\shptop*N*****? **\\shpbottom*N*****? **\\shpright*N*****?
**\\shplid*N*****? **\\shpz*N*****? **\\shpfhdr*N*****? **\\shpbxpage**
? **\\shpbxmargin** ? **\\shpbxcolumn**? **\\shpbxignore**?
**\\shpbypage** ? **\\shpbymargin** ? **\\shpbypara**?
**\\shpbyignore**? **\\shpwr*N*****? **\\shpwrk*N*****?
**\\shpfblwtxt*N*****? **\\shplockanchor**?
**\\shptxt**?</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<shpinst\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{\\\*'
**\\shpinst** \<sp\>+
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<sp\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{'
**\\sp** \<sn\> \<sv\> \<hsv\>?
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<sn\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{'
**\\sn** ...
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<sv\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{'
**\\sv** ...
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<shprslt\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{\\\*'
**\\shprslt** ...
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<hsv\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{\\\*'
**\\hsv** \<accent\> & **\\ctint*N***** & **\\cshade*N*****
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<accent\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\\caccentone</span><span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">|
**\\caccenttwo** | **\\caccentthree** | **\\caccentfour** |
**\\caccentfive** |
**\\caccentsix**</span>

 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">I
won’t go into every control word definition, you can surely follow every
control word definition in the RTF specification, but I will point out
the control words germane to answering the question at hand.  The
following is a snippet from a test RTF document I used by saving a
Microsoft Word document with a Watermark as
RTF.</span> 

<span style="font-family: courier new,courier; font-size: x-small;">{\\shp{<span style="background-color: #ffff99;">\\\*</span>\\shpinst\\shpleft0\\shptop0\\shpright10557\\shpbottom2639\\shpfhdr0\\shpbxmargin\\shpbxignore\\shpbymargin\\shpbyignore\\shpwr3\\shpwrk0\\shpfblwtxt1\\shpz0\\shplid2049{\\sp{\\sn**shapeType**}{\\sv
**136**}}{\\sp{\\sn **fFlipH**}{\\sv 0}}{\\sp{\\sn **fFlipV**}{\\sv
0}} </span>

<span style="font-family: courier new,courier; font-size: x-small;">{\\sp{\\sn
**gtextUNICODE**}{\\sv **CONFIDENTIAL**}}{\\sp{\\sn gtextSize}{\\sv
65536}}{\\sp{\\sn gtextFont}{\\sv Calibri}}{\\sp{\\sn fGtext}{\\sv
1}}{\\sp{\\sn gtextFStretch}{\\sv 1}}{\\sp{\\sn**fillColor**}{\\sv
12632256}} </span>

<span style="font-family: courier new,courier; font-size: x-small;">{\\sp{\\sn**fillOpacity**}{\\sv
32768}}{\\sp{\\sn fLine}{\\sv 0}}{\\sp{\\sn
**wzName**}{\\**svPowerPlusWaterMarkObject**357476642}}{\\**sp**{\\sn
**posh**}{\\**sv** 2}}{\\sp{\\sn**posrelh**}{\\sv 0}}{\\sp{\\sn
**posv**}{\\sv 2}}{\\sp{\\sn **posrelv**}{\\sv 0}}{\\sp{\\sn dhgt}{\\sv
251659264}}… </span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">The
first thing to point out is the control symbol
"<span style="background-color: #ffff99;">\\\*</span>".  I’ve alluded to
this before, and per the RTF specification, "*Destinations added after
the *[*<span style="text-decoration: underline;"><span style="color: #0072c6;"><span style="color: #0072c6;"><span lang="">1987
RTF
Specification</span></span></span></span>*](file:///E:/Users/glueShoe/AppData/Local/Microsoft/Windows/Temporary%20Internet%20Files/Temporary%20Internet%20Files/Content.IE5/GZY35SKC/Word2007RTFSpec9.doc)*<span lang="">
may be preceded by the control symbol **\\\*** (backslash
asterisk).**This control symbol identifies destinations whose related
text should be ignored if the RTF reader does not recognize the
destination control word.**</span>*"  This means the shape will be
ignored by an RTF reader that does not implement the control word.  You
can test out a quick example of this by creating an RTF document in Word
that contains a Watermark then open the document in WordPad and you’ll
notice the Watermark will not be rendered since WordPad does not support
Watermarks.  Open the same RTF file in Word and of course you see the
Watermark.</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">In
my example file above, I used a "CONFIDENTIAL" Watermark which refers to
the Shape (control word "\\shp") and the Drawing Object
"PowerPlusWaterMarkObject").  The Drawing Object is a special object
identifier specifically for Word.  You’ll find references to this object
in API calls, macros, etc. (i.e. wherever you are dealing with a Word
Watermark).</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">You
can refer to the RTF specification and note the definitions for the
shape (position, text font/size, left, top, right, bottom, etc).  Just
follow the nested open/close braces "{ }" (the *Group* symbol), which
indicates the beginning and end of each Group (and Groups can and will
be nested).  Essentially, the entire RTF file is in a
Group.</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">An
RTF file has the following
syntax:</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">\<File\></span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">'{'
\<header\> \<document\>
'}'</span>

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">Note,
the important takeaway point in this example is that there is no
"Watermark" per-se in RTF, but rather the "\\shp" control words compose
the Watermark feature for the RTF
reader.</span> 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;">If
there is another feature you’d like to examine to see how it is
composed/represented in RTF, just open Word and save your example as RTF
and open in your editor of choice.  It’s an easy way to understand RTF
and how various features are implemented in the RTF
format.</span>

 

<span style="font-family: tahoma,arial,helvetica,sans-serif; font-size: small;"><span style="color: #1f497d;">NOTE:
Questions and comments are welcome.  However, please DO NOT post a
comment using the comment tool at the end of this post.  Instead, post a
new thread in the Open Specifications Forum at
[http://social.msdn.microsoft.com/Forums/en-US/](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/redir.aspx?C=Qgp1_VJQRE6dVF74G1qiVJPG6KVx1s8I7HPJueJilUmocAuLJVkEVsrB6BE72XYOrE2DRfW18ls.&URL=http%3a%2f%2fsocial.msdn.microsoft.com%2fForums%2fen-US%2fos_windowsprotocols)</span></span>
