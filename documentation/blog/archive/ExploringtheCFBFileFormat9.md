<div id="page">

# Exploring the CFB File Format 9

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/10/2011 1:00:27
PM

-----

<div id="content">

# <span style="color: #365f91;"><span style="font-family: Cambria;">Exploring the CFB File Format 9</span></span>

# <span style="color: #365f91;"><span style="font-family: Cambria;"></span></span><span style="font-size: medium;"><span style="color: #4f81bd;"><span style="font-family: Cambria;">File Security  
</span></span></span><span style="font-size: small;"><span style="font-family: Calibri;">  
Due to the nature of a compound file, a single file in a file-system, the operating system security mechanisms is ultimately what controls the security regarding the file in question. Using the ACL, you can control who can read and/or write to the file. Using file-system encryption, you can also secure the contents of the file.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Since
this is ultimately covered by the \[MS-CFB\] Open Specification
Document, I will present a means of examples of using python to alter
Windows specific file-system security.</span></span>

<span style="font-family: Calibri; font-size: small;">Python provides a
means for native file manipulation using the win32security module. This
module allows the application in question call all relevant Win32
Security API functions. These functions will correlate to the exact
security API function names found in the Windows API. These functions
can be found on MSDN at the following URL:
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">http://msdn.microsoft.com/en-us/library/aa375742(v=VS.85).aspx</span>](http://msdn.microsoft.com/en-us/library/aa375742\(v=VS.85\).aspx)<span style="font-family: Calibri; font-size: small;">.
However, the module in question only implements a subset of these
functions, and the entire list of supported functions can be found here:
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">http://docs.activestate.com/activepython/2.4/pywin32/win32security.html</span>](http://docs.activestate.com/activepython/2.4/pywin32/win32security.html)<span style="font-size: small;"><span style="font-family: Calibri;">.  
  
</span></span><span style="font-size: medium;"><span style="color: #4f81bd;"><span style="font-family: Cambria;">Displaying
the current file-system controls of a
file  
  
</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">import
win32security</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">\#
obtain security descriptor of said owner of file  
secdesc = win32security.GetFileSecurity("C:\\Temp\\VerifySchTask.txt",
win32security.OWNER\_SECURITY\_INFORMATION)</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">\#
the sec\_desc object now contains everything we need in regards to file
permissions.  
\# we can now call any of the various win32 security api
functions.</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">\#
for example, let's get the owner's sid:  
osid = secdesc.GetSecurityDescriptorOwner()  
\#display owner sid  
print
osid</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">\#
now let’s just dump the security descriptor object to view  
print
secdesc  
  
</span></span></span><span style="font-size: medium;"><span style="color: #4f81bd;"><span style="font-family: Cambria;">Altering
the ACL of a
File  
</span></span></span>  
<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">import
win32security</span></span></span>

<span style="font-size: small;"><span style="color: #00008b;"><span style="font-family: Calibri;">\#
obtain security descriptor of said owner of file  
secdesc = win32security.GetFileSecurity("C:\\Temp\\VerifySchTask.txt",
win32security.OWNER\_SECURITY\_INFORMATION)</span></span></span>

<span style="font-size: small;"><span style="font-family: Calibri;"><span style="color: #00008b;">\#
apply the security descriptor to the aforementioned file.  
SetFileSecurity( FILE,</span>
<span style="color: #00008b;">DACL\_SECURITY\_INFORMATION  ,
secdesc)  
</span></span></span><span style="font-size: medium;"><span style="color: #4f81bd;"><span style="font-family: Cambria;"><span style="font-family: Calibri; color: #000000; font-size: x-small;">  
</span>Encryption</span></span></span>

<span style="font-size: small;">The contents of the file can be
encrypted and stored in user-defined streams. In fact, if you review
\[MS-OFFCRYPTO\], you will see this is how the latest version of Office
2010 works. It takes the XML content and stores it in a special stream
that contains the encrypted-form of the working document. However, it is
much easier to present/store the file on a file-system that is encrypted
(e.g. on Windows you have BitLocker, EFS, etc…).</span>  
  
<span style="font-size: small;">However, if you wish to perform your own
security mechanism, you can review the \\EncryptedPackage stream
structure as defined in Section 2.3.4.4 of \[MS-OFFCRYPTO\]. This stream
contains the entire office document in compressed  & encrypted
form.</span>

</div>

</div>
