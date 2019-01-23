<div id="page">

# PowerShell script for finding Microsoft Office legacy files

[Vilmos Foltenyi
MSFT](https://social.msdn.microsoft.com/profile/Vilmos%20Foltenyi%20MSFT)
4/8/2013 2:08:00 PM

-----

<div id="content">

Referenced
documents:  
[<span style="color: #0000ff;">\[MS-CFB\]</span>](http://msdn.microsoft.com/en-us/library/dd942138.aspx):
Compound File Binary File
Format  
[<span style="color: #0000ff;">\[MS-OLEPS\]</span>](http://msdn.microsoft.com/en-us/library/dd942421.aspx):
Object Linking and Embedding (OLE) Property Set Data Structures  
Windows PowerShell Cookbook, 3<sup>rd</sup> edition, by Lee Holmes

**NOTE**: Questions and comments are welcome. However, please DO NOT
post a comment using the comment tool at the end of this post. Instead,
post a new thread in the Open Specifications Forum: Office File Formats
at  
[<span style="color: #0000ff;">http://social.msdn.microsoft.com/Forums/en-US/os\_binaryfile/threads</span>](http://social.msdn.microsoft.com/Forums/en-US/os_binaryfile/threads)

 

\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#  
\# WHAT THE SCRIPT DOES  
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

This blog is complementary to the blog
“[<span style="color: #0000ff;">Determining Office Binary File Format
Types</span>](http://blogs.msdn.com/b/openspecification/archive/2013/01/16/determining-office-binary-file-format-types.aspx)”,
by JCurry (Josh). That blog describes in details how to find
PIDSI\_APPNAME, i.e. the name of the
application<span style="text-decoration: line-through;">,</span> which
created the file. For the Office legacy files it can be: “Microsoft
Office Word”, “Microsoft Excel”, or “Microsoft Office PowerPoint”. The
PowerShell (PS) script in this blog prints more information, some
information from the header, and the properties from the “Summary
Information sector”. This looks adequate to meet the requests of recent
cases on the Open Specifications Forum / Office File Formats, but, of
course, more annotation can be added if there is demand for it.

The OfficeLegacyFilter.ps1 PS script, see attached file, starts with a
comment block, which contains the disclaimer and the name, version, and
usage of the script. The script has one parameter, which can be a
directory or file name. If the parameter is a directory, then all the
files in this directory and in the subdirectories are recursively
checked.

All checks are made on the content of the file; the file name extension
(sometime called file type) is not used. If a check fails the file is
just skipped. Which means if no Office Legacy file is in scope, the
script returns nothing. This makes easier to use the script in pipe.

The checks start with the Header Signature, Minor and Major versions,
etc. until the First Directory Sector Location is reached. Currently we
are interested only in one directory sector, which has the name “Summary
Information”. At offset 0x74 of that directory sector is the Starting
Sector Location, see \[MS-CFB\] v20130118 / 2.6.1 Compound File
Directory Entry, from that value we can navigate to \[MS-OLEPS\] —
v20130118 / 2.21 PropertySetStream and to 2.20 PropertySet, where you’ll
find NumProperties; all the properties with their values are printed
out. All these steps can be easily followed in the script. 

  
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#  
\# HOW TO RUN THE SCRIPT  
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

If your machine is not set up for running a PowerShell script, the
OfficeLegacyFilter.ps1 script will not run, because scripting support is
disabled by default. To see your current execution policy setting run
the:

<span style="font-family: courier new,courier; font-size: small;">Get-ExecutionPolicy  
</span>  
cmdlet. If you have some kind of restriction, it can be lifted by
running
the  
  
<span style="font-family: courier new,courier; font-size: small;">Set-ExecutionPolicy
Unrestricted  
</span>  
cmdlet. If you try to run the script now, probably you will be prompted
for permission and the script will run.

If you want to run the script in a more secure way, set the execution
policy to the desired level. If you already have a code-signing
certificate, you can use it. To check whether you have a code-signing
certificate do the next:  
You can go to the cert: driver and look around the certificates

<span style="font-family: courier new,courier; font-size: small;">cd
cert:     \# Go to the cert driver and look around  
</span>  
You can find all code-signing certificates

<span style="font-family: courier new,courier; font-size: small;">dir
cert: –Recurse –CodeSigningCert</span>

If you have at least one, use, e.g., the first one  
  
<span style="font-family: courier new,courier; font-size: small;">$cert
= @(dir cert: -Recurse
–CodeSigningCert)\[0\]</span>

<span style="font-family: courier new,courier; font-size: small;">Set-AuthenticodeSignature
\<ps1 file\> $cert</span>

At the end of the \<ps1 file\> you should see a comment block with the
next structure:

<span style="font-family: courier new,courier; font-size: small;">\# SIG
\# Begin signature block</span>  
<span style="font-family: courier new,courier; font-size: small;">\#
\<64 base64
digits\></span>  
<span style="font-family: Courier New; font-size: x-small;">.......................</span>  
<span style="font-family: courier new,courier; font-size: small;">\#
\<64 base64 digits\></span>  
<span style="font-family: courier new,courier; font-size: small;">\#
\<base64 digits\></span>  
<span style="font-family: courier new,courier; font-size: small;">\# SIG
\# End signature block</span>

and running the script should not be a problem.

If you don’t have code-signing certificate, then you can make one. The
steps how to create a self-signed certificate are described in many
places. First you need the utility for making certificate, makecert.exe.
You can read about it
here:  
[<span style="color: #0000ff;">http://msdn.microsoft.com/en-us/library/windows/desktop/aa386968(v=vs.85).aspx</span>](http://msdn.microsoft.com/en-us/library/windows/desktop/aa386968\(v=vs.85\).aspx)

First you have to generate a local certificate authority and after to
create a self-signed certificate by using the local certification
authority.  
You can read about this, e.g., at  
Program: Create a Self-Signed Certificate  
Holmes, Lee (2012-12-21). Windows PowerShell Cookbook: The Complete
Guide to Scripting Microsoft's Command Shell (p. 522). O'Reilly Media.
Kindle Edition.

You should be able to see the properties of the newly created signing
certificate by running:  
  
<span style="font-family: courier new,courier; font-size: small;">dir
cert: -Recurse –CodeSigningCert | Format-List \*</span>

 

\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#  
\# EXAMPLE  
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

After creating two of the three test files I changed their extension to
txt. It can be anything, it is not used in the script.

 

<span style="font-family: courier new,courier;">PS
C:\\Projects\\PS\\scripts\>
C:\\Projects\\PS\\scripts\\OfficeLegacyFilter.ps1
'C:\\Test\\a'</span>

<span style="font-family: courier new,courier;">C:\\Test\\a\\x\\Autonumbering.txt</span>  
<span style="font-family: courier new,courier;">    000000 Header
Signature   .  .  .  .  : D0 CF 11 E0 A1 B1 1A E1</span>  
<span style="font-family: courier new,courier;">    000018 MinorVersion
.  .  .  .  .  .  : 62</span>  
<span style="font-family: courier new,courier;">    00001A MajorVersion
.  .  .  .  .  .  : 3</span>  
<span style="font-family: courier new,courier;">    009E94 01
CODEPAGE\_PROPERTY\_IDENTIFIER: 1252</span>  
<span style="font-family: courier new,courier;">    009E9C 02
PIDSI\_TITLE  .  .  .  .  .  : PowerPoint Presentation </span>  
<span style="font-family: courier new,courier;">    009EBC 04
PIDSI\_AUTHOR .  .  .  .  .  : Vilmos Foltenyi </span>  
<span style="font-family: courier new,courier;">    009EBC 0A
PIDSI\_EDITTIME  .  .  .  .  : 00:02:53.4790000</span>  
<span style="font-family: courier new,courier;">    009ED4 08
PIDSI\_LASTAUTHOR   .  .  .  : Vilmos Foltenyi </span>  
<span style="font-family: courier new,courier;">    009EEC 09
PIDSI\_REVNUMBER .  .  .  .  : 1   </span>  
<span style="font-family: courier new,courier;">    009EF8 12
PIDSI\_APPNAME   .  .  .  .  : Microsoft Office PowerPoint </span>  
<span style="font-family: courier new,courier;">    009F28 0C
PIDSI\_CREATE\_DTM   .  .  .  : Tuesday, 12/20/2011 3:33:05 PM</span>  
<span style="font-family: courier new,courier;">    009F34 0D
PIDSI\_LASTSAVE\_DTM .  .  .  : Thursday, 1/24/2013 12:05:04 AM</span>  
<span style="font-family: courier new,courier;">    009F40 0F
PIDSI\_WORDCOUNT .  .  .  .  : 3</span>  
<span style="font-family: courier new,courier;">    009F48 11
PIDSI\_THUMBNAIL size format : 57736 FFFFFFFF</span>

<span style="font-family: courier new,courier;">C:\\Test\\a\\x\\glow
test.txt</span>  
<span style="font-family: courier new,courier;">    000000 Header
Signature   .  .  .  .  : D0 CF 11 E0 A1 B1 1A E1</span>  
<span style="font-family: courier new,courier;">    000018 MinorVersion
.  .  .  .  .  .  : 62</span>  
<span style="font-family: courier new,courier;">    00001A MajorVersion
.  .  .  .  .  .  : 3</span>  
<span style="font-family: courier new,courier;">    0118A4 01
CODEPAGE\_PROPERTY\_IDENTIFIER: 1252</span>  
<span style="font-family: courier new,courier;">    0118AC 04
PIDSI\_AUTHOR .  .  .  .  .  : Tim </span>  
<span style="font-family: courier new,courier;">    0118AC 0A
PIDSI\_EDITTIME  .  .  .  .  : 00:00:00</span>  
<span style="font-family: courier new,courier;">    0118B8 07
PIDSI\_TEMPLATE  .  .  .  .  : Normal.dotm </span>  
<span style="font-family: courier new,courier;">    0118CC 08
PIDSI\_LASTAUTHOR   .  .  .  : Tim </span>  
<span style="font-family: courier new,courier;">    0118D8 09
PIDSI\_REVNUMBER .  .  .  .  : 2   </span>  
<span style="font-family: courier new,courier;">    0118E4 12
PIDSI\_APPNAME   .  .  .  .  : Microsoft Office Word   </span>  
<span style="font-family: courier new,courier;">    011910 0C
PIDSI\_CREATE\_DTM   .  .  .  : Wednesday, 1/16/2013 8:33:00 AM</span>  
<span style="font-family: courier new,courier;">    01191C 0D
PIDSI\_LASTSAVE\_DTM .  .  .  : Wednesday, 1/16/2013 8:33:00 AM</span>  
<span style="font-family: courier new,courier;">    011928 0E
PIDSI\_PAGECOUNT .  .  .  .  : 1</span>  
<span style="font-family: courier new,courier;">    011930 0F
PIDSI\_WORDCOUNT .  .  .  .  : 24</span>  
<span style="font-family: courier new,courier;">    011938 10
PIDSI\_CHARCOUNT .  .  .  .  : 137</span>  
<span style="font-family: courier new,courier;">    011940 13
PIDSI\_DOC\_SECURITY .  .  .  :
0</span>

<span style="font-family: courier new,courier;">C:\\Test\\a\\y\\Acronyms.xls</span>  
<span style="font-family: courier new,courier;">    000000 Header
Signature   .  .  .  .  : D0 CF 11 E0 A1 B1 1A E1</span>  
<span style="font-family: courier new,courier;">    000018 MinorVersion
.  .  .  .  .  .  : 62</span>  
<span style="font-family: courier new,courier;">    00001A MajorVersion
.  .  .  .  .  .  : 3</span>  
<span style="font-family: courier new,courier;">    009474 01
CODEPAGE\_PROPERTY\_IDENTIFIER: 1252</span>  
<span style="font-family: courier new,courier;">    00947C 04
PIDSI\_AUTHOR .  .  .  .  .  :     </span>  
<span style="font-family: courier new,courier;">    009488 08
PIDSI\_LASTAUTHOR   .  .  .  :     </span>  
<span style="font-family: courier new,courier;">    009494 12
PIDSI\_APPNAME   .  .  .  .  : Microsoft Excel </span>  
<span style="font-family: courier new,courier;">    0094AC 0C
PIDSI\_CREATE\_DTM   .  .  .  : Friday, 9/15/2006 5:00:00 PM</span>  
<span style="font-family: courier new,courier;">    0094B8 0D
PIDSI\_LASTSAVE\_DTM .  .  .  : Thursday, 2/14/2013 11:49:09 AM</span>  
<span style="font-family: courier new,courier;">    0094C4 13
PIDSI\_DOC\_SECURITY .  .  .  :
0</span>

 

[OfficeLegacyFilter.ps1](images/OfficeLegacyFilter.ps1)

</div>

</div>
