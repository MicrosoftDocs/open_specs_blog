<div id="page">

# Overview of Protected Office Open XML Documents

[Mark Miller
(rōnin)](https://social.msdn.microsoft.com/profile/Mark%20Miller%20\(rōnin\))
7/17/2009 9:17:00 AM

-----

<div id="content">

 

Suppose your application requirement is to
<span class="underline">programmatically</span> create password
“protected” Office Open XML (OOXML)
documents.<span style="mso-spacerun: yes">  </span>Or, perhaps the
requirement is to programmatically convert a batch of unprotected OOXML
documents to password protected ones.<span style="mso-spacerun: yes"> 
</span>The following information outlines some aspects to consider when
implementing for this requirement.

 

If you need to review the OOXML specification format you may do so here,
<span style="COLOR: black; mso-bidi-font-family: Arial; mso-bidi-font-weight: bold">[Introducing
the Office (2007) Open XML File
Formats](http://msdn.microsoft.com/en-us/library/aa338205.aspx#office2007aboutnewfileformat_structureoftheofficexmlformats)</span><span style="mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">. 
The Microsoft ECMA-376 Implementer notes are located here,
[www.documentinteropinitiative.org](http://www.documentinteropinitiative.org/)</span>

 

The Microsoft Compound File Binary File Format (MS-CFB) specification
may be found here,
[MS-CFB](http://msdn.microsoft.com/en-us/library/cc546605.aspx)<span style="mso-spacerun: yes">.  </span>Also,
you may review the Office binary file specifications for XLS, DOC and
PPT here,
<span style="COLOR: black; mso-bidi-font-family: Arial; mso-bidi-font-weight: bold">[Microsoft
Office File Format
Documents](http://msdn.microsoft.com/en-us/library/cc313105.aspx).</span> 

 

When you create a password <span class="underline">protected</span>
document in the Office User Interface (UI) you will find that it becomes
a Microsoft Compound File Binary File Format (MS-CFB) document (not a
ZIP package), though the file extension does not change (e.g., xlsx,
docx, pptx).<span style="mso-spacerun: yes">  </span>Subsequently, the
UI protected document will open within the Office UI but you cannot
rename the file extension to \*.zip and browse it with
Explorer.<span style="mso-spacerun: yes">  </span>In order to
programmatically parse the resultant protected binary document you may
refer to the CFB and Office Binary File Specification links
above.<span style="mso-spacerun: yes">  </span>You may find more
information on Office document encryption here,
<span style="COLOR: black; mso-themecolor: text1; mso-ansi-language: EN" lang="EN">[RC4
CryptoAPI Encryption Password
Verification](http://blogs.msdn.com/openspecification/archive/2009/05/08/Dominic-Salemno.aspx)</span>,
and here,
[<span style="mso-bookmark: _Toc139688862"><span style="mso-bookmark: _Toc131822189"><span style="mso-bookmark: _Toc131822147">2007
Microsoft Office System Document
Encryption</span></span></span><span style="mso-bookmark: _Toc131822147"></span><span style="mso-bookmark: _Toc131822189"></span><span style="mso-bookmark: _Toc139688862"></span>](http://download.microsoft.com/download/6/7/f/67f1ff44-f1c9-4fae-a451-4e803f7b727e/2007_Office_DocEncryption.docx)<span style="COLOR: black; mso-themecolor: text1"></span>

 

This is the header for a password protected document created in the
Excel UI (CFB document):

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 9pt">0000h:
D0 CF 11 E0 A1 B1 1A E1 00 00 00 00 00 00 00 00  ÐÏ.à¡±.á........</span>

 

This is the header for an unprotected document created in the Excel UI
(ZIP package):

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 9pt">0000h:
50 4B 03 04 14 00 06 00 08 00 00 00 21 00 9E 6F 
**PK**..........\!.žo</span>

<span style="mso-bidi-font-family: &#39;Courier New&#39;">(Note,
</span>**PK** are the initials for ‘**P**hil **K**atz’ the original
author of the **PK**ZIP
format)<span style="mso-bidi-font-family: &#39;Courier New&#39;"></span>

<span style="FONT-FAMILY: &#39;Courier New&#39;"></span>

 

To programmatically create or modify OOXML documents you may use the
System.IO.Packaging namespace in .NET to create the documents
programmatically, and an example is detailed here, [Microsoft Knowledge
Base Article 931866](http://support.microsoft.com/kb/931866).  Or, you
may use the Open XML SDK, and there are several examples here,
<span style="COLOR: black; mso-themecolor: text1; mso-font-kerning: 18.0pt">[Open
XML File Format Code
Snippets](http://www.microsoft.com/downloads/details.aspx?familyid=8d46c01f-e3f6-4069-869d-90b8b096b556&displaylang=en)</span>.<span style="mso-spacerun: yes"> 
</span>However, at this point in time the Open XML SDK does not provide
functionality to open or create files with Compound File protection (CFB
document), as detailed above.  In other words, once the document is no
longer a zip package it can no longer be opened within the Open XML
SDK.<span style="mso-spacerun: yes">  </span>We are looking into ways of
improving the SDK to support such scenarios.

 

The example and details below are specific to Excel documents.

 

To programmatically password protect Excel documents you will need to
implement the **WorkbookProtection** Class (and
f<span class="t1"><span style="COLOR: black; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">or
Word documents this is the
</span></span>**<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; COLOR: black; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">DocumentProtection
Class, refer to </span>**Open XML
SDK)**<span style="mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">,</span>**
and/or add the
<span class="t1"><span style="BACKGROUND: silver; COLOR: black; mso-highlight: silver">workbookProtection</span></span><span class="t1"><span style="COLOR: black">
element \*before\* the
<span style="BACKGROUND: yellow; mso-highlight: yellow">bookViews</span>
element and \*after\* the
<span style="BACKGROUND: lime; mso-highlight: lime">workbookPr</span>
element.<span style="mso-spacerun: yes">  </span>For
example:</span></span>

<span class="t1"><span style="COLOR: black"></span></span>

 

<div style="BORDER-BOTTOM: windowtext 1pt solid; BORDER-LEFT: windowtext 1pt solid; PADDING-BOTTOM: 1pt; PADDING-LEFT: 4pt; PADDING-RIGHT: 4pt; BACKGROUND: #f2f2f2; BORDER-TOP: windowtext 1pt solid; BORDER-RIGHT: windowtext 1pt solid; PADDING-TOP: 1pt; mso-border-alt: solid windowtext .5pt; mso-element: para-border-div; mso-shading: windowtext; mso-pattern: gray-5 auto">

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<?xml
version="1.0" encoding="utf-8" standalone="yes"
?\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:workbook</span><span style="COLOR: red; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
xmlns:x</span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">="</span>**<span style="COLOR: red; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">http://schemas.openxmlformats.org/spreadsheetml/2006/main</span>**<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">"\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:fileVersion</span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">appName</span><span style="COLOR: blue">="</span>**xl**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
lastEdited</span><span style="COLOR: blue">="</span>**4**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
lowestEdited</span><span style="COLOR: blue">="</span>**4**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
rupBuild</span><span style="COLOR: blue">="</span>**4505**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:<span style="BACKGROUND: lime; mso-highlight: lime">workbookPr</span></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">defaultThemeVersion</span><span style="COLOR: blue">="</span>**124226**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:<span style="BACKGROUND: silver; mso-highlight: silver">workbookProtection</span></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="BACKGROUND: aqua; COLOR: #990000; mso-highlight: aqua">workbookPassword</span><span style="BACKGROUND: aqua; COLOR: blue; mso-highlight: aqua">="</span>**<span style="BACKGROUND: aqua; mso-highlight: aqua">xsd:hexBinary</span>**<span style="BACKGROUND: aqua; mso-highlight: aqua">
**data**<span style="COLOR: blue">"</span></span><span style="COLOR: #990000">
lockStructure</span><span style="COLOR: blue">="</span>**1**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
lockWindows</span><span style="COLOR: blue">="</span>**1**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:<span style="BACKGROUND: yellow; mso-highlight: yellow">bookViews</span></span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:workbookView</span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">xWindow</span><span style="COLOR: blue">="</span>**600**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
yWindow</span><span style="COLOR: blue">="</span>**525**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
windowWidth</span><span style="COLOR: blue">="</span>**17895**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
windowHeight</span><span style="COLOR: blue">="</span>**4560**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
activeTab</span><span style="COLOR: blue">="</span>**1**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\</</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:bookViews</span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:sheets</span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:sheet</span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">name</span><span style="COLOR: blue">="</span>**My
Data**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
sheetId</span><span style="COLOR: blue">="</span>**1**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
r:id</span><span style="COLOR: blue">="</span>**rId1**<span style="COLOR: blue">"</span><span style="COLOR: red">
xmlns:r</span><span style="COLOR: blue">="</span>**<span style="COLOR: red">http://schemas.openxmlformats.org/officeDocument/2006/relationships</span>**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:sheet</span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">name</span><span style="COLOR: blue">="</span>**Chart**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
sheetId</span><span style="COLOR: blue">="</span>**3**<span style="COLOR: blue">"</span><span style="COLOR: #990000">
r:id</span><span style="COLOR: blue">="</span>**rId2**<span style="COLOR: blue">"</span><span style="COLOR: red">
xmlns:r</span><span style="COLOR: blue">="</span>**<span style="COLOR: red">http://schemas.openxmlformats.org/officeDocument/2006/relationships</span>**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\</</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:sheets</span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\<</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:calcPr</span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
<span style="COLOR: #990000">calcId</span><span style="COLOR: blue">="</span>**125725**<span style="COLOR: blue">"
/\></span></span>

<span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\</</span><span style="COLOR: #990000; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">x:workbook</span><span style="COLOR: blue; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\></span><span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

</div>

 

<span class="t1"><span style="COLOR: black"></span></span>

For more information about the WorkbookProtection Class refer to MSDN,
<span style="COLOR: black; mso-bidi-font-family: Arial; mso-bidi-font-weight: bold; mso-themecolor: text1">[WorkbookProtection
Class](http://msdn.microsoft.com/en-us/library/documentformat.openxml.spreadsheet.workbookprotection\(office.14\).aspx)</span><span style="COLOR: black; mso-themecolor: text1"></span>

<span class="t1"><span style="COLOR: black"></span></span>

 

<span class="t1"><span style="COLOR: black">\*
<span class="t1"><span style="COLOR: black">Also note, the
</span></span><span style="BACKGROUND: aqua; mso-highlight: aqua">workbookPassword</span>
**xsd:hexBinary**
<span style="COLOR: black; mso-bidi-font-family: Arial; mso-bidi-font-weight: bold; mso-themecolor: text1">[WorkbookPassword
Property](http://msdn.microsoft.com/en-us/library/documentformat.openxml.spreadsheet.workbookprotection.workbookpassword\(office.14\).aspx)</span></span></span>

[](http://msdn.microsoft.com/en-us/library/documentformat.openxml.spreadsheet.workbookprotection.workbookpassword\(office.14\).aspx)[](http://msdn.microsoft.com/en-us/library/documentformat.openxml.spreadsheet.workbookprotection.workbookpassword\(office.14\).aspx)

 

Note, after implementing the document protection mechanism
programmatically the document is not considered secure since the
password is stored in plain text in the OOXML document structure and can
fairly easily be obtained and/or removed by editing the “workbook.xml”
file, under the “xl” folder (or the “document.xml” file for Word, under
the “word” folder) in the ZIP package.  By comparison, a Compound File
Binary file protected document is considered more
secure<span class="MsoCommentReference"><span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-SIZE: 8pt"> </span></span>since
the password is stored in an encrypted stream in the CFB file format.

 

For more information regarding ECMA-376 document encryption approaches
refer to,
<span class="stidentifier1"><span style="COLOR: black; FONT-SIZE: 24pt; FONT-WEIGHT: normal; mso-bidi-font-weight: bold; mso-themecolor: text1">[<span style="FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; FONT-SIZE: 11pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-weight: normal">Standard
ECMA-376 Office Open XML File
Formats</span>](http://www.ecma-international.org/publications/standards/Ecma-376.htm)</span></span>,
as mentioned in [\[MS-OFFCRYPTO\]
section 1.3.3](http://msdn.microsoft.com/en-us/library/dd922354.aspx).

 

</div>

</div>
