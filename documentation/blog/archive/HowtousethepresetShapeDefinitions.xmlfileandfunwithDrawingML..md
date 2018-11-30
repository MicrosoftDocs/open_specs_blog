<div id="page">

# How to use the presetShapeDefinitions.xml file and fun with DrawingML.

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 11/14/2011
7:31:00
AM

-----

<div id="content">

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">This
article deals with the content contained in ECMA-376 Part 1. The 3rd
edition of ECMA-376 was released in June of 2011 and can be downloaded
from the following location:
<http://www.ecma-international.org/publications/standards/Ecma-376.htm>  
</span>

**<span style="font-family: arial,helvetica,sans-serif; font-size: medium;">  
Where can I get the presetShapeDefinitions.xml
    file?</span>**

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;"><span style="font-style: normal; font-weight: normal;">Download
    </span>[ECMA-376 3rd edition
    Part 1](http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-376,%20Third%20Edition,%20Part%201%20-%20Fundamentals%20And%20Markup%20Language%20Reference.zip)<span style="font-style: normal; font-weight: normal;">
    and unzip the contents into an empty folder. You should see a .pdf
    file and 5 more .zip
    files.</span></span>

2.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Unzip
    the contents of the OfficeOpenXML-DrawingMLGeometries.zip file into
    the same
    folder.</span>

3.  
    
    <div style="margin-top: 0px; margin-bottom: 0px; vertical-align: middle;">
    
    <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Open
    the presetShapeDefinitions.xml file in the XML file editor of your
    choice. If you don't have a dedicated XML file editor, most basic
    text editors (Notepad) should be able to open and display the file
    contents.</span>
    
    </div>

 

<span style="font-family: arial,helvetica,sans-serif; font-size: medium; font-weight: bold;">What
am I supposed to do with
this?  
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">That's
completely up to you. But here's a few
    ideas…</span>

1.  
    
    <div style="margin-top: 0px; margin-bottom: 0px; vertical-align: middle;">
    
    <span style="font-family: arial,helvetica,sans-serif; font-size: small; font-style: normal; font-weight: normal;">Maybe
    you are writing an open source word processor for another platform
    that can read .docx files and you need to be able to draw the shape
    associated with a
    <span style="font-family: courier new,courier;">**\<prstGeom\>**</span>
    (§20.1.9.18) element. The shape doesn't look right and you are
    questioning whether or not there is a problem with the
    definition.</span>
    
    </div>

2.  
    
    <div style="margin-top: 0px; margin-bottom: 0px; vertical-align: middle;">
    
    <span style="font-family: arial,helvetica,sans-serif; font-size: small;">You
    could be creating a custom template for PowerPoint and you want to
    modify one of the existing shapes beyond what is available in the
    format
options.</span>
    
    </div>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">For
the purposes of this blog, I will focus on the second
scenario.  
</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: medium; font-weight: bold;">  
How do I do
that?  
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">In
short, any shape definition from the presetShapeDefinitions.xml file can
be used within a
**<span style="font-family: courier new,courier;">\<custGeom\></span>**
(§20.1.9.8) element. Sounds easy, right? Well, it is\! Here's how to do
it:</span>

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small; font-style: normal; font-weight: normal;">Create
    a new Microsoft PowerPoint 2010 presentation, delete the 2 title
    placeholders, and drop any shape you want onto the slide. I will use
    the 'Moon' shape for demonstration
purposes.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small; font-style: normal; font-weight: normal;">[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5241.pic1.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5241.pic1.png)</span>

2.  <span style="font-family: arial,helvetica,sans-serif; font-size: small; font-style: normal; font-weight: normal;"></span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">Save
    the presentation as
    </span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">'MyShapeTest.pptx'.</span>
3.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;"></span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">Close
    Microsoft
    PowerPoint.</span>
4.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;"></span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">Rename
    the file to
    </span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">'MyShapeTest.zip'.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Wait
a second\! Why
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">are
we renaming the file with a .zip extension? You may have heard that
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">starting
with Office 2007 the files that end with 'x', such as .docx, .xlsx,
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">and
.pptx are actually zip-compressed packages containing a collection of
XML
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">files.
Well, you can take that at face value and rename any of those file types
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">with
a .zip extension and then open it in your un-zipper of choice and browse
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">the
contents, which is exactly what we are going to
do.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Extract
the contents of the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">MyShapeTest.zip
file.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Open
the file
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">"ppt\\slides\\slide1.xml"
in a text editor or a dedicated xml
file</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> editor.</span>

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Note:
    If the
    file</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> opens
    automatically in Internet Explorer or another browser you may
    need</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">
    to use the "Open with…"
option.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Locate
the **<span style="font-family: courier new,courier;">\<a:prstGeom
prst="moon"\></span>** element and its associated
**<span style="font-family: courier new,courier;">\</a:prstGeom\></span>**
element.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Make
the following
    changes:</span>

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Replace
    **<span style="font-family: courier new,courier;">\<a:prstGeom</span>**</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">**<span style="font-family: courier new,courier;"> prst="moon"\></span>**
    with
    **<span style="font-family: courier new,courier;">\<a:custGeom\>.</span>**</span>
2.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Replace
    **<span style="font-family: courier new,courier;">\</a:prstGeom\></span>**
    with
    **<span style="font-family: courier new,courier;">\</a:custGeom\>.</span>**</span>
3.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Replace
    the
    **<span style="font-family: courier new,courier;">\<a:avLst/\></span>**
    element that exists between the two custGeom
    elements</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> with
    the contents of the Moon definition from
    the</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> presetShapeDefinitions.xml
    file. DO NOT include the
    **<span style="font-family: courier new,courier;">\<moon\></span>**
    and
    **<span style="font-family: courier new,courier;">\</moon\></span>**
    elements.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Save
and close the slide1.xml
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">file.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Replace
the slide1.xml file
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">in
the zip-compressed package with the modified version of the
    file.</span>

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Note: DO
    NOT</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> re-zip
    the contents of the MyShapeTest folder. You MUST replace
    the</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> modified
    file in the original .zip
file.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Rename
the file back
to</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> 'MyShapeTest.pptx'
and open it with Microsoft
    PowerPoint.</span>

1.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">Note:
    If you
    get</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> an
    error when opening the presentation you may have accidently
    changed</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> something
    that you shouldn't have. Start over and try
again.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">When
you open the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">file
it should look exactly like it did before, but that's exactly the point.
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">You
replaced the reference to the shape with the actual definition. It's
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">supposed
to look the
same.  
</span>

**<span style="font-family: arial,helvetica,sans-serif; font-size: medium;">  
What's the
point?</span>**

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Remember
our 2
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">scenarios
from the "What an I </span><span style="font-size: small;">supposed to
do
with</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">
this" section
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">above?
Well, we've answered the first question of whether or not the definition
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">is
'good'. If it wasn't, the shape would not have rendered correctly or you
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">would
have received an error stating something along the lines of
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">"PowerPoint
found a problem with content in MyShapeTest.pptx" when
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">you
tried to open the
file.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">If
you're familiar
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">with
DrawingML you probably already realized that you can now modify the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">definition
in the slide1.xml file. For those who are not familiar with
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">DrawingML,
here's something that you can change in the definition to make the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">moon
a little more
interesting.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Follow
steps 3 - 5 from
above</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Locate
the following
    line.</span>

1.  **<span style="font-family: courier new,courier; font-size: small;">\<pathLst
    xmlns="http://schemas.openxmlformats.org/drawingml/2006/main"\></span>**
2.  <span style="font-family: arial,helvetica,sans-serif; font-size: small;">This
    is the
    List</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> of
    Path Shapes, or the actual lines that are drawn to make the
    shape.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Remove
the following
line</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;"> from
within the pathLst
    block.</span>

1.  **<span style="font-family: courier new,courier;"><span style="font-size: small;">\<arcTo
    wR="g18w"
    hR="dy1"</span><span style="font-size: small;"> stAng="stAng1"
    swAng="swAng1"
/\></span></span>**

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Replace
it with
    this.</span>

1.  **<span style="font-family: courier new,courier; font-size: small;">\<lnTo\>   </span>**  
    **<span style="font-family: courier new,courier;"><span style="font-size: small;">    
    \<pt x="wd2" y="hd2"
/\>  
    </span><span style="font-size: small;">\</lnTo\></span></span>**

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Follow
steps 8-10 from
above.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">When
you open the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">file
you should see that the inside arc has been replaced with 2 straight
lines
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">from
the moon's points to the center of the shape. In step 4 we only drew the
</span><span style="font-family: arial,helvetica,sans-serif; font-size: small;">first
line. The second one is drawn automatically by the
**<span style="font-family: courier new,courier;">\<close /\></span>**
element.</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/0636.pic2.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/0636.pic2.png)</span>

<span style="font-family: arial,helvetica,sans-serif; font-size: small;">Refer
to
[ECMA-376](http://www.ecma-international.org/publications/standards/Ecma-376.htm) Part
1, Chapter 14 on DrawingML for more information on how to manipulate and
draw your own custom shapes.</span>

</div>

</div>
