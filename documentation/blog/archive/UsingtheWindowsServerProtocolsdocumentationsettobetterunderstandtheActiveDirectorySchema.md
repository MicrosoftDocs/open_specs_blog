<div id="page">

# Using the Windows Server Protocols documentation set to better understand the Active Directory Schema

[Pyralanna](https://social.msdn.microsoft.com/profile/Pyralanna)
6/26/2009 3:41:00 PM

-----

<div id="content">

Implementing an interoperable directory service requires an LDAP
compliant backend.<span style="mso-spacerun: yes">  </span>At some point
you will need to load the targeted version of
a <span class="MsoCommentReference"><span style="FONT-SIZE: 8pt; LINE-HEIGHT: 115%"><span style="mso-special-character: comment"> </span></span></span>schema
from Windows into your implementation.<span style="mso-spacerun: yes"> 
</span>Presented here are some of the concepts, documentation and tools
to consider when working with the
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>Active
Directory schema - Directory Services (AD -DS).

Let’s start with some basic concepts about the Active Directory
schema.<span style="mso-spacerun: yes">  </span>The schema keeps track
of things such as classes, class attributes, and relationships between
objects such as subclasses and super classes. A super class is a parent
while a child class inherits its attributes from the super class. The
schema<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span>also enforces rules that
govern both the structure and the content of the directory.

A <span class="Italic">*class* </span>is a category of objects that
share a set of common characteristics. It is a formal description of an
object that can be stored in the directory. Each object in the directory
is an instance of one or more classes in the schema.

<span style="mso-spacerun: yes"> </span>Attributes define the types of
information that an object can hold. For each class, the schema
specifies the mandatory attributes and optional attributes that
constitute the set of shared characteristics of the class.

The syntax is the data type of a particular attribute. Syntaxes
determine what data type an attribute can have. Active Directory uses a
set of predefined syntaxes. The predefined syntaxes do not actually
appear in the directory, and new syntaxes cannot be added.

The schema is represented in Active Directory by a set of objects known
as <span class="Italic">*schema objects*</span>. This discussion will
focus mainly on examining
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>these
objects
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>in
order to understand how they relate to their classes, attributes and
syntaxes in an actual schema file. I’ve included a schema file derived
from the MS-AD\* documentation in the protocols test suite that can be
examined to see the various classes and attributes included in the
schema for Windows 2008 Server SP1.
<span style="mso-spacerun: yes"> </span>The LDIFDE utility can be used
to import and export Active Directory objects. It is shipped with
Windows 2008 Server and beyond, or as part of the Windows 2003 resource
kit. The full name for this utility is the LDAP Data Interchange Format
Data Exchange tool. <span style="mso-spacerun: yes"> </span>Although I
will use the abbreviation LDIFDE throughout this
discussion,<span style="mso-spacerun: yes">  </span>its full name
underlies the fact that Active Directory is based on the LDAP Data
Interchange Format standard which is a file format used in LDAP
implementations. For more information regarding the LDAP Format see RFC
2849 - <http://www.ietf.org/rfc/rfc2849.txt> .

I’ve included a schema file derived from the MS-AD\* documentation in
the protocols test suite that can be examined to see the various classes
and attributes included in the schema for Windows 2008 Server SP1. You
can also generate a schema file using LDIFDE which will contain
information about your current installation.

 

The documents that we will be using to help us understand how this all
fits together are part of the Windows Server Protocols documentation set
- <http://msdn.microsoft.com/en-us/library/cc216517(PROT.10).aspx> . The
particular documents from set that we will use
are:

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-ADTS\]: Active Directory Technical
Specification --
<http://msdn.microsoft.com/en-us/library/cc223122(PROT.13).aspx>

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-ADSC\]: Active Directory Schema Classes --
<http://msdn.microsoft.com/en-us/library/cc221630(PROT.13).aspx>

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-ADA1\]: Active Directory Schema Attributes A-L
--
<http://msdn.microsoft.com/en-us/library/cc219751(PROT.13).aspx>

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-ADA2\]: Active Directory Schema Attributes M
--
<http://msdn.microsoft.com/en-us/library/cc220154(PROT.13).aspx>

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>\[MS-ADA3\]: Active Directory Schema Attributes N-Z
-- <http://msdn.microsoft.com/en-us/library/cc220699(PROT.13).aspx>

<span class="msoDel"></span>

 

<span class="msoDel"></span>

 

 

Let’s talk about how you can use these documents to obtain a better
understanding of what an Active Directory schema looks like and how the
objects, classes and attributes in the
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>schema
relate to one another. Using the following command from a command prompt
I generated an ldf file for one of my installed Active Directories by
running the command:<span style="mso-spacerun: yes">  </span>‘ldifde –f
myschema.ldf’  from a command prompt.
<span style="mso-spacerun: yes"> </span>For clarity I have included
this file along with two others<span style="mso-spacerun: yes"> 
</span>in this blog. Here is a description of the included files:

Myschema.ldf – this is the schema file for the contoso.com domain
generated with the previous ldifde command. This is an installation of
Active Directory that I use for testing purposes only.

MS-AD\_Schema\_2K8\_SP1\_Consolidated-sorted.ldf – This file contains a
non installation specific<span style="mso-spacerun: yes">  </span>Active
Directory schema for Windows 2008 Server SP1.

Computers.ldf – This file contains the computer objects for the
contoso.com Active Directory.

Note that all of these files can be opened as text file. I used notepad
to view them.

An example of an alternate usage of ldifde that exports the schema from
a particular server is:

<span style="mso-spacerun: yes"> </span>ldifde –f myschema.ldf –s
<span style="mso-spacerun: yes"> </span>JD-SERVER01.contoso.com

where

\-f filename<span style="mso-tab-count: 1">          </span>Output file
name

\- s servername<span style="mso-tab-count: 1">  </span>The server to
bind to.

Full command arguments can be found with “ldifde -?”

 

 

This file, myschema.ldf,
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>has
a much larger amount of data specific to the contoso.com domain’s Active
Directory as compared to the data in the
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>file
MS-AD\_Schema\_2K8\_SP1\_Consolidated-sorted.ldf. Both of these files
are attached to this blog along with additional file, computers.ldf that
shows computer objects in my Active Directory installation Please note
that the schema file for the contoso.com domain is
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>a
completely fictitious Active Directory schema populated for testing
purposes only. The Microsoft file is intended as a template containing
most classes and attributes that you would find in a Windows 2008 Server
SP1 Active Directory.

Before we look at some data that you can find in an actual schema file,
I wanted to mention more about the schema syntax. While discussing the
schema syntax, you will also get a taste of how classes and attributes
are defined, and how to use the \[MS-AD\*\] documents
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>to
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>view
both class and attribute definitions.<span class="msoDel"> ~~~~ </span>

The schema syntax is covered in the \[MS-ADTS\] document. The below
description of LDAP Representations and the associated table are taken
directly from that
document:

<span id="_Toc227006867"></span>**<span class="msoDel">~~“~~</span>3.1.1.2.2.2<span style="mso-spacerun: yes">  
</span>LDAP
Representations**<span id="z7cda533ed7a44aeca51791d02ff4a1aa"></span>

The LDAP syntaxes supported by DCs are as shown in the following table.
The set of syntaxes supported is not extensible by schema modifications.
Each syntax is identified by the combination of the attributeSyntax,
oMSyntax <span style="mso-spacerun: yes"> </span>and, in select cases,
oMObjectClass <span style="mso-spacerun: yes"> </span>attributes of an
attributeSchema object. The cases for which oMObjectClass is not used
are indicated by the presence of a hyphen in the oMObjectClass column in
the table. The combinations shown in the following table are exhaustive;
this table is consistent and identical for all versions of Windows
Server.”

 

**<span style="LETTER-SPACING: -0.1pt">LDAP syntax name </span>**

</div>

</div>

**<span style="LETTER-SPACING: -0.1pt">attributeSyntax</span>**

**<span style="LETTER-SPACING: -0.1pt">oMSyntax</span>**

**<span style="LETTER-SPACING: -0.1pt">oMObjectClass</span>**

Boolean

2.5.5.8

1

\-

Enumeration

2.5.5.9

10

\-

Integer

2.5.5.9

2

\-

LargeInteger

2.5.5.16

65

\-

Object(Access-Point)

2.5.5.14

127

0x2B 0x0C 0x02 0x87 0x73 0x1C 0x00 0x85 0x3E (1.3.12.2.1011.28.0.702)

Object(DN-String)

2.5.5.14

127

0x2A 0x86 0x48 0x86 0xF7 0x14 0x01 0x01 0x01 0x0C
(1.2.840.113556.1.1.1.12)

Object(OR-Name)

2.5.5.7

127

0x56 0x06 0x01 0x02 0x05 0x0B 0x1D (2.6.6.1.2.5.11.29)

Object(DN-Binary)

2.5.5.7

127

0x2A 0x86 0x48 0x86 0xF7 0x14 0x01 0x01 0x01 0x0B
(1.2.840.113556.1.1.1.11)

Object(DS-DN)

2.5.5.1

127

0x2B 0x0C 0x02 0x87 0x73 0x1C 0x00 0x85 0x4A (1.3.12.2.1011.28.0.714)

Object(Presentation-Address)

2.5.5.13

127

0x2B 0x0C 0x02 0x87 0x73 0x1C 0x00 0x85 0x5C (1.3.12.2.1011.28.0.732)

Object(Replica-Link)

2.5.5.10

127

0x2A 0x86 0x48 0x86 0xF7 0x14 0x01 0x01 0x01 0x06
(1.2.840.113556.1.1.1.6)

String(Case)

2.5.5.3

27

\-

String(IA5)

2.5.5.5

22

\-

String(NT-Sec-Desc)

2.5.5.15

66

\-

String(Numeric)

2.5.5.6

18

\-

String(Object-Identifier)

2.5.5.2

6

\-

String(Octet)

2.5.5.10

4

\-

String(Printable)

2.5.5.5

19

\-

String(Sid)

2.5.5.17

4

\-

String(Teletex)

2.5.5.4

20

\-

String(Unicode)

2.5.5.12

64

\-

String(UTC-Time)

2.5.5.11

23

\-

String(Generalized-Time)

2.5.5.11

24

\-

<span class="msoDel"></span>

 

<span class="msoDel"></span>

 

<span class="msoDel"></span>

 

<span class="msoDel"></span>

 

 

 

The representation for many of the preceding syntaxes is adopted from
[\[RFC2252\]](http://go.microsoft.com/fwlink/?LinkId=90326) –
Lightweight Directory Access Protocol (v3): Attribute Syntax
Definitions.

<span style="mso-spacerun: yes"> </span>Notice that the columns in the
above table; attributeSyntax, oMSyntax<span style="mso-spacerun: yes"> 
</span>and oMObjectClass<span style="mso-spacerun: yes">  </span>are all
attributes. For each object attribute in Active Directory there is an
attributeSchema object <span style="mso-spacerun: yes"> </span>and for
each class in the Active Directory there is a class Schema
object.<span class="msoDel"> ~~~~ </span>

Classes for Active Directory can be found in the \[MS-ADSC\]. One such
class, the attributeSchema class defines an attribute object in the
schema. The layout for this class, found in \[MS-ADSC\] is shown
here:

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; MARGIN-LEFT: 0.15in; BORDER-LEFT: white 3pt solid; MARGIN-RIGHT: 0in; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

cn: Attribute-Schema

ldapDisplayName: attributeSchema

governsId: 1.2.840.113556.1.3.14

objectClassCategory: 1

rdnAttId: cn

subClassOf: top

systemMustContain: schemaIDGUID, oMSyntax, lDAPDisplayName,

<span style="mso-spacerun: yes"> </span>isSingleValued, cn,
attributeSyntax, attributeID

systemMayContain: systemOnly, searchFlags, schemaFlagsEx, rangeUpper,

<span style="mso-spacerun: yes"> </span>rangeLower, oMObjectClass,
msDs-Schema-Extensions, msDS-IntId,

<span style="mso-spacerun: yes"> </span>mAPIID, linkID,
isMemberOfPartialAttributeSet, isEphemeral,

<span style="mso-spacerun: yes"> </span>isDefunct, extendedCharsAllowed,
classDisplayName,

<span style="mso-spacerun: yes"> </span>attributeSecurityGUID

systemPossSuperiors: dMD

schemaIdGuid:bf967a80-0de6-11d0-a285-00aa003049e2

defaultSecurityDescriptor: D:S:

defaultHidingValue: TRUE

systemOnly: FALSE

defaultObjectCategory:
CN=Attribute-Schema,CN=Schema,

<span style="mso-spacerun: yes"> </span>CN=Configuration,\<RootDomainDN\>

systemFlags: FLAG\_SCHEMA\_BASE\_OBJECT |
FLAG\_DOMAIN\_DISALLOW\_RENAME

 

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">The
definition of this class contains many attributes, all of which can be
found in the \[MS-ADA\*\] documentation. For example, systemMustContain
is an attribute that specifies the attributeIds of some of the mandatory
attributes of this class. Thus all attributes themselves must contain
attributeIds for schemaIDGUID, oMSyntax, lDAPDisplayName,isSingleValued,
cn, attributeSyntax, attributeID.
<span style="mso-spacerun: yes"> </span>Therefore
the<span style="mso-spacerun: yes">  </span>attributeSyntax attribute
found in the above table is a required ID for all attributes.
<span style="mso-spacerun: yes"> </span>Not all attributes have to
contain an oMObjectClass attribute as you can see from the previous
attribute syntax table . You can look for an attributes definition based
on its name in either \[MS-ADA1\], \[MS-ADA2\] or \[MS-ADA3\].
<span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span>For
example, attributeSyntax , a column
header<span style="mso-spacerun: yes">   </span>in the attribute syntax
table,<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span>can be found in
\[MS-ADA1\].</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"></span>

 

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">You
should take note of the systemFlags attribute. It contains the following
bitwise flags (further defined in \[MS-ADTS\] section
2.2.9):</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fATTINDEX:
\*
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fPDNTATTINDEX:
\*
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fANR:
Add this attribute to the ambiguous name resolution (ANR) set (if set,
then fATTINDEX <span style="mso-spacerun: yes">  </span>must be
<span style="mso-spacerun: yes"> </span>set). See \[MS-ADTS\] for ANR
search.
<span style="mso-spacerun: yes">    </span></span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fPRESERVEONDELETE:
Preserve this attribute on logical deletion. This flag is ignored on
link attributes.
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fCOPY:
Interpreted by LDAP clients, not by the server. If set, the attribute is
copied on object copy.
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fTUPLEINDEX:
\*
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fSUBTREEATTINDEX:
\*
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fCONFIDENTIAL:
This attribute is confidential; special access check is needed. For more
information, see \[MS-ADTS\] section
3.1.1.4.3.</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fNEVERVALUEAUDIT:
\*</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">fRODCFilteredAttribute:
If set, this attribute is in the RODC filtered attribute
set.</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"></span>

 

</div>

<span id="_Toc227005909"></span>**2.75<span style="mso-spacerun: yes">  
</span>Attribute
attributeSyntax**<span id="z7f9903b0af484bca98f2dfb1cfab4d13"></span>

This attribute specifies the OID for the syntax for this
attribute.

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; MARGIN-LEFT: 0.15in; BORDER-LEFT: white 3pt solid; MARGIN-RIGHT: 0in; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

cn: Attribute-Syntax

ldapDisplayName: attributeSyntax

attributeId: 1.2.840.113556.1.2.32

attributeSyntax: 2.5.5.2

omSyntax: 6

isSingleValued: TRUE

schemaIdGuid: bf967925-0de6-11d0-a285-00aa003049e2

systemOnly: TRUE

searchFlags:fPRESERVEONDELETE

systemFlags: FLAG\_SCHEMA\_BASE\_OBJECT

schemaFlagsEx: FLAG\_ATTR\_IS\_CRITICAL

</div>

Version-Specific Behavior: Implemented on Windows 2000 Server, Windows
Server 2003, Windows Server 2003 R2, Windows Server 2008, and Windows
Server
7.

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; MARGIN-LEFT: 0.15in; BORDER-LEFT: white 3pt solid; MARGIN-RIGHT: 0in; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

The
[schemaFlagsEx](http://blogs.msdn.com/tiny_mce/jscripts/tiny_mce/%5bMS-ADA3%5d.pdf)
attribute was added to this attribute definition in Windows
Server 2008

</div>

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; BORDER-LEFT: white 3pt solid; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Let’s
look at some of the attributes that you see above,then we will move on
to an example from the schema file. All of the below definitions come
from the \[MS-ADA\*\]
documents:</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"></span>

 

</div>

cn (ADA1) -- This attribute specifies the name that represents an
object. It is used to perform searches.

ldapDisplayName<span style="mso-spacerun: yes">  </span>(ADA1) -- This
attribute specifies the name used by LDAP clients, such as the ADSI LDAP
provider, to read and write the attribute by using the LDAP protocol.

attributeId<span style="mso-spacerun: yes">  </span>(ADA1) -- This
attribute specifies the unique X.500 object identifier (OID) for
identifying an attribute

schemaIdGuid (ADA3) -- This attribute specifies a unique GUID that
identifies this attribute, and is used in security descriptors. It is
required on an attributeSchema object. If omitted during Add, the server
will auto-generate a random GUID

systemOnly (ADA3) -- This attribute specifies a Boolean value that
specifies whether only Active Directory can modify the class.
System-Only classes can be created or deleted only by the directory
system agent.

Now let’s look at an actual object in the schema file for my domain. You
can do text sorts on this file in various ways. For example you can
search for objects by searching for the string “objectClass:
computer”.<span style="mso-spacerun: yes">  </span>Alternatively you
can create a file using LDIFDE and its search filter capability. For
example you could search for all of the computers in the directory with
the following command:

 

Ldifde <span style="mso-spacerun: yes"> </span>-f computers.ldf
–s<span style="mso-spacerun: yes">  </span>JD-SERVER01.contoso.com
–r<span style="mso-spacerun: yes">  </span>objectClass=computer

Switches: -f to specify the file to export to, -s to specify the server
to bind to, and –r the filter

You can use any of the classes found in \[MS-ADSC\] for this filter.

 

Below is one of the computer objects in my domain as seen in the schema
file:

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; MARGIN-LEFT: 0.15in; BORDER-LEFT: white 3pt solid; MARGIN-RIGHT: 0in; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

dn: CN=FILESERVER01,OU=Servers,DC=contoso,DC=com

changetype: add

objectClass: top

objectClass: person

objectClass: organizationalPerson

objectClass: user

objectClass: computer

cn: FILESERVER01

distinguishedName: CN=FILESERVER01,OU=Servers,DC=contoso,DC=com

instanceType: 4

whenCreated: 20081027221316.0Z

whenChanged: 20081027221316.0Z

uSNCreated: 12806

uSNChanged: 12810

name: FILESERVER01

objectGUID:: o2nB9yVP306ZYUYlMs0NVA==

userAccountControl: 4128

badPwdCount: 0

codePage: 0

countryCode: 0

badPasswordTime: 0

lastLogoff: 0

lastLogon: 0

localPolicyFlags: 0

pwdLastSet: 128696191965312500

primaryGroupID: 515

objectSid:: AQUAAAAAAAUVAAAATP4fPlGgN5m+BTa/VgQAAA==

accountExpires: 9223372036854775807

logonCount: 0

sAMAccountName: FILESERVER01$

sAMAccountType: 805306369

objectCategory: CN=Computer,CN=Schema,CN=Configuration,DC=contoso,DC=com

isCriticalSystemObject: FALSE

dSCorePropagationData: 16010101000000.0Z

 

</div>

 

There are a couple of things to note in this example. The objectClass
top is the top-level class from which all classes are derived. The
object classes in the example appear in their correct
<span class="msoDel">~~<span style="mso-spacerun: yes"> </span>~~</span>order
with respect to the object hierarchy. Computer is a subClassof User
which is a subClassof organizationalPerson etc. Keep in mind also that
the object instance inherits attributes from its parent up to the top
class. This means that you may need to look at all the different classes
involved in the hierarchy to find which class actually contains that
attribute. For example, the codePage attribute is part of the User
Class.

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; MARGIN-LEFT: 0.15in; BORDER-LEFT: white 3pt solid; MARGIN-RIGHT: 0in; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

 

 

 

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-size: 8.0pt; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">I
leave it as exercise for the reader to use the \[MS-AD\*\] documentation
to dig digger into this object to learn more about what the various
attributes <span style="mso-spacerun: yes"> </span>in this object
<span style="mso-spacerun: yes"> </span>represent and how they are
derived.</span><span class="msoDel">
</span>

<span style="FONT-SIZE: 11pt; COLOR: windowtext; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-bidi-theme-font: minor-bidi; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-no-proof: no"></span>

 

</div>

 

<div style="BORDER-RIGHT: white 3pt solid; PADDING-RIGHT: 6pt; BORDER-TOP: white 3pt solid; PADDING-LEFT: 6pt; PADDING-BOTTOM: 5pt; BORDER-LEFT: white 3pt solid; PADDING-TOP: 4pt; BORDER-BOTTOM: white 3pt solid; mso-element: para-border-div">

 

</div>

<div style="mso-element: comment-list">

-----

<div style="mso-element: comment">

<div id="_com_1" class="msocomtxt" data-language="JavaScript" onmouseover="msoCommentShow(&#39;_anchor_1&#39;,&#39;_com_1&#39;)" onmouseout="msoCommentHide(&#39;_com_1&#39;)">

<span style="mso-comment-author: &#39;Bill Wesse&#39;"><span id="_msocom_1"></span></span>

<span class="MsoCommentReference"><span style="FONT-SIZE: 8pt"><span style="mso-special-character: comment"> [\[WEW1\]](http://blogs.msdn.com/tiny_mce/jscripts/tiny_mce/blank.htm#_msoanchor_1)</span></span></span>Inserted
new
text

</div>

</div>

<div style="mso-element: comment">

<div id="_com_2" class="msocomtxt" data-language="JavaScript" onmouseover="msoCommentShow(&#39;_anchor_2&#39;,&#39;_com_2&#39;)" onmouseout="msoCommentHide(&#39;_com_2&#39;)">

<span style="mso-comment-author: &#39;Bill Wesse&#39;"><span id="_msocom_2"></span></span>

<span class="MsoCommentReference"><span style="FONT-SIZE: 8pt"><span style="mso-special-character: comment"> [\[WEW2\]](http://blogs.msdn.com/tiny_mce/jscripts/tiny_mce/blank.htm#_msoanchor_2)</span></span></span>Inserted
a close
apostrophe

</div>

</div>

</div>

[Schema.zip](images/Schema.zip)
