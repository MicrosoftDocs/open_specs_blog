<div id="page">

# OpenXML Styles 101 - Creating Custom Styles and Understanding Style Inheritance

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 9/16/2015
12:00:00 PM

-----

<div id="content">

<span style="font-size: large;">Introduction</span>  
This will be the first in a series of articles on various OpenXML
topics. This article provides an expanded description of how Style
Inheritance works. We will be using an example created in Microsoft Word
2016 and then manually modifying the package contents.  
  
By simply reading through this blog you should be able to grasp how to
manually create your own custom styles and how to build more complex
styles using Style Inheritance. However, nothing beats getting some
actual hands-on experience. I highly encourage you to perform the steps
as they are presented here and experiment on your own.  
  
  
<span style="font-size: large;">Resources</span>  
This article will refer exclusively to information contained in [ISO/IEC
29500-1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/ISO/IEC%2029500-1 "https://aka.ms/sl6tli").
The latest version, as of this article's release date, was published in
September of 2012 and can be downloaded from the following
location.  
  
[http://standards.iso.org/ittf/PubliclyAvailableStandards/index.html](http://standards.iso.org/ittf/PubliclyAvailableStandards/index.html "https://aka.ms/em8mad")  
  
  
<span style="font-size: large;">Getting Started</span>  
We will start by creating a very simple document that contains three
paragraphs. The actual contents of the paragraphs isn't very important,
but it makes it easier to identify each one in the XML if they are
numbered on some
way.  
  
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5076.Untitled%20picture.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5076.Untitled%20picture.png)  
*<span style="font-size: x-small;">Figure 1: Three paragraphs in the
default style.</span>*  
  
Save the document and then rename the file with a .zip extension.  
  
<span style="text-decoration: underline;">**Note:**</span> It is assumed
that you already understand that the .DOCX, .XLSX, and .PPTX file
formats are actually special .ZIP packages that contain all of the XML
files used to construct the document. [ISO/IEC
29500-1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/ISO/IEC%2029500-1 "https://aka.ms/sl6tli")
section 9 contains a summary of the package format. [ISO/IEC
29500-2](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/ISO/IEC%2029500-2 "https://aka.ms/bgdzkr")
deals exclusively with the Open Packaging Conventions (OPC) format.  
  
  
<span style="font-size: large;">Examining the Package Contents</span>  
Once you have renamed the file to have a .zip extension, go ahead and
extract the contents. Open the folder and then examine the contents of
the "word" folder. The contents should look similar to
this.  
  
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5732.Untitled%20picture.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5732.Untitled%20picture.png)  
*<span style="font-size: x-small;">Figure 2: The contents of the "word"
folder.</span>*  
  
The two files that we will be working with are document.xml and
styles.xml. Go ahead and open the document.xml and styles.xml files in a
basic text editor. The first thing you will notice is that the contents
are one big blob of text without any kind of formatting unless you are
using an application that was designed specifically for editing XML
files. There are a few different ways to fix this, but if you are a
Visual Studio user you can use the Edit -\> Advanced -\> Format Document
command.  
  
The section of the document.xml file that we will be working with is in
the middle that has our three paragraphs.  
  
<span style="text-decoration: underline;">*document.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="00BA6E33" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 1\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="009A7F68" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 2\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="009A7F68" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 3\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:bookmarkStart w:id="0" w:name="\_GoBack"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:bookmarkEnd
w:id="0"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
  
There isn't anything in the styles.xml file that we really care about
right now. The only thing worth taking note of is the "Normal" style
near the bottom of the file.  
  
<span style="text-decoration: underline;">*styles.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:style
w:type="paragraph" w:default="1" w:styleId="Normal"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">   \<w:name
w:val="Normal"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:qFormat/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:style\></span>  
  
  
<span style="font-size: large;">Creating a New Style</span>  
Additional details on the style element can be found in [ISO/IEC
29500-1](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/ISO/IEC%2029500-1 "https://aka.ms/sl6tli")
section 17.7.4.17. For now, we will create a very simple style that is
based on the Normal style, sets it to be Bold, and increases the font
size.  
  
<span style="text-decoration: underline;">*styles.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:style
w:type="paragraph" w:styleId="MyCustomStyle1"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">   \<w:name
w:val="My Custom Style 1"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:basedOn w:val="Normal"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">      \<w:sz
w:val="32"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">      \<w:b
w:val="true"/\>0</span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:style\></span>  
  
Next, we will assign this style to the first paragraph of our
document.  
  
<span style="text-decoration: underline;">*document.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="00BA6E33" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:pStyle w:val="MyCustomStyle1"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 1\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
  
After you make those changes and save the files you will need to update
the package contents. It's important to understand that you can't just
re-zip the main folder and put a .docx extension on it. It will be
missing all of the metadata. The best way to update the package contents
is to overwrite the existing files in the original .zip package with the
updated ones and then rename back to the .docx extension. After you do
that and open the file it Word it should look like
this.  
  
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4174.Untitled%20picture.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/4174.Untitled%20picture.png)  
*<span style="font-size: x-small;">Figure 3: MyCustomStyle1 applied to
the first paragraph.</span>*  
  
  
<span style="font-size: large;">Inheriting From a Custom Style</span>  
For the second paragraph lets create a new style that inherits from the
one used in the first paragraph. We will add a new element and change an
existing one to demonstrate how the style at the leaf level overrides
the style(s) further up the inheritance tree.  
  
<span style="text-decoration: underline;">*styles.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:style
w:type="paragraph" w:styleId="MyCustomStyle2"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">   \<w:name
w:val="My Custom Style 2"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:basedOn w:val="MyCustomStyle1"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">      \<w:b
w:val="false"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:color w:val="FF0000"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:style\></span>  
  
And assign the new style to the second paragraph…  
  
<span style="text-decoration: underline;">*document.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="009A7F68" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:pStyle w:val="MyCustomStyle2"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 2\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
  
Update the package contents again and let's take a look at the
result.  
  
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8816.Untitled%20picture.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8816.Untitled%20picture.png)  
  
*<span style="font-size: x-small;">Figure 4: MyCustomStyle2 applied to
the second paragraph</span>*  
  
As you can see the text in the second paragraph is still 16pt in size
which was inherited from MyCustomStyle1, is no longer bold, and is now
red.  
  
  
<span style="font-size: large;">Overwriting Everything</span>  
Regardless of what is inherited from previous styles in the tree, it is
possible to simply overwrite everything. There is no real practical use
for this. Creating a new style that doesn't inherit from another style
is probably a better choice and will give the same results, but we are
doing this for demonstration purposes. Here is a third style that
inherits from the second style that overwrites each property set
previously.  
  
<span style="text-decoration: underline;">*styles.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:style
w:type="paragraph" w:styleId="MyCustomStyle3"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">   \<w:name
w:val="My Custom Style 3"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:basedOn w:val="MyCustomStyle2"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">      \<w:sz
w:val="64"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">      \<w:b
w:val="true"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:color w:val="0000FF"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:rPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:style\></span>  
  
And apply it to the third paragraph…  
  
<span style="text-decoration: underline;">*document.xml*</span>  
<span style="font-family: &#39;courier new&#39;, courier;">\<w:p
w:rsidR="009A7F68" w:rsidRDefault="009A7F68"\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:pStyle w:val="MyCustomStyle3"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:pPr\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">     
\<w:t\>Paragraph 3\</w:t\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \</w:r\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:bookmarkStart w:id="0" w:name="\_GoBack"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;"> 
 \<w:bookmarkEnd
w:id="0"/\></span>  
<span style="font-family: &#39;courier new&#39;, courier;">\</w:p\></span>  
  
And take a look at the
result.  
  
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/1581.Untitled%20picture.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/1581.Untitled%20picture.png)  
*<span style="font-size: x-small;">Figure 5: MyCustomStyle3 applied to
the third paragraph.</span>*  
  
As you can see the font size is now set to 32pt, bold has been turned on
again, and the text is blue.  
  
  
<span style="font-size: large;">Summary</span>  
Thank you for taking the time to read through this article. This was a
simple introduction to how Style Inheritance works in Open XML. I hope
that this was useful for you and answered any questions that you may
have had on the subject. In future articles we will dig deeper into
styles and explore other topics such as the Style Hierarchy, Linked
Styles, and more.  
  
I encourage you to send you feedback or questions regarding this
article, or any other Open XML related topics to Microsoft
Interoperability Support at
[dochelp@microsoft.com](mailto:dochelp@microsoft.com "mailto:dochelp@microsoft.com")
or posted on the Office XML, ODF, and Binary File Formats forum on MSDN.

</div>

</div>
