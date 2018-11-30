<div id="page">

# About the ACCESS\_MASK Structure

[Bill Wesse](https://social.msdn.microsoft.com/profile/Bill%20Wesse)
4/1/2010 7:07:00 AM

-----

<div id="content">

Unless otherwise specified, references in this document are from the
[Windows Software Development Toolkit
(SDK)](http://go.microsoft.com/fwlink/?LinkID=89050) (v7.0) and the
[Windows Driver Kit (WDK)](http://go.microsoft.com/fwlink/?LinkID=84091)
(v7600.16385.0), which defines at least 46 mask sets and 500+ bit flag
value declarations. The document attached to this blog entry contains
extensive toolkit header extracts and [MSDN](http://msdn.microsoft.com/)
Library cross-references.

The
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_MASK</span></span>
structure is defined in
<http://msdn.microsoft.com/en-us/library/aa374892.aspx>, and is declared
in WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)).

The
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_MASK</span></span>
data type is a 32-bit (double word) flag set that is the primary means
of specifying (encoding) the requested or granted access to a
(securable) object by a user. Specifically, it is a value that defines
standard, specific, and generic rights used in [\[Access Control
Entries\]](http://msdn.microsoft.com/en-us/library/aa374868.aspx)
<span style="mso-spacerun: yes"> </span>(ACEs) as well as parameters to
Windows API functions.

A securable object is an object that can have a security descriptor
([\[Security
Descriptors\]](http://msdn.microsoft.com/en-us/library/aa379563.aspx)).
All named Windows objects are securable. Some unnamed objects, such as
process and thread objects, can have security descriptors too. For most
securable objects, you can specify an object's security descriptor in
the function call that creates the object. For example, you can specify
a security descriptor in the
[CreateFile](http://msdn.microsoft.com/en-us/library/aa363858.aspx) and
[CreateProcess](http://msdn.microsoft.com/en-us/library/ms682425.aspx)
functions.

Each type of securable object defines its own set of specific access
rights and its own mapping of generic access rights. For information
about the specific and generic access rights for each type of securable
object, see the overview for that type of object [\[Securable
Objects\]](http://msdn.microsoft.com/en-us/library/aa379557.aspx).

The following is a list of some common securable objects, from "Windows
Internals<sup><span style="LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Arial Unicode MS&#39;,&#39;sans-serif&#39;; FONT-SIZE: 12pt">®</span></sup>,
Fifth Edition", Microsoft Press, 2009, Library of Congress Control
Number 2009927697,
p458:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Files, directories and volumes (NTFS file
system)

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Devices

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">3.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Mailslots

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">4.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Named and anonymous
pipes

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">5.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Jobs

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">6.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Processes

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">7.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Threads

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">8.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Events, keyed events and event
pairs

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">9.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Mutexes,
semaphores

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">10.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Shared memory
sections

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">11.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>I/O completion
ports

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">12.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>LPC
ports

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">13.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Waitable
timers

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">14.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Access
tokens

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">15.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Windows
stations

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">16.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Desktops

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">17.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Network
shares

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">18.<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Services

# <span id="_Toc257628127"></span><span id="_Toc257628091"></span><span id="_Toc257628064"></span><span id="_Toc257627909"></span><span style="mso-bookmark: _Toc257628064"><span style="mso-bookmark: _Toc257628091"><span style="mso-bookmark: _Toc257628127"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">1<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>ACCESS\_MASK</span></span></span>

The general rule – for proper Windows executable citizenship – when
setting
<span style="mso-bidi-font-family: &#39;Courier New&#39;">ACCESS\_MASK
flags, is to always use as few flags as possible. Securable object
creation is, of course, a case where this rule may not apply. Other
popular (and no doubt necessary) exceptions involve temporary files,
configuration file management, backup operations and so on.</span>

<span style="mso-bidi-font-family: &#39;Courier New&#39;">Of special
note is the
</span><span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">MAXIMUM\_ALLOWED</span></span>
<span style="mso-bidi-font-family: &#39;Courier New&#39;">bit, which is
</span>generally used with the
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
function to determine whether a security descriptor grants a specified
set of access rights to the client identified by an access token.
Typically, server applications use this function to check access to a
private object.

As noted previously, each type of securable object has its own defined
set of *specific access rights*; in almost all cases, these rights
include a definition for all bits valid on the underlying object. These
should ideally match the result of a call to
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
– if and only if the caller has full control permissions on the
object. For example, [File Security and Access
Rights](http://msdn.microsoft.com/en-us/library/aa364399.aspx) defines
the following.

\#define FILE\_ALL\_ACCESS (STANDARD\_RIGHTS\_REQUIRED | SYNCHRONIZE |
0x1FF)

<span class="Code1Char"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">STANDARD\_</span></span><span class="Code1Char"><span style="LINE-HEIGHT: 115%; mso-ansi-font-size: 11.0pt">RIGHTS</span></span><span class="Code1Char"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">\_REQUIRED</span></span>
is a mask meant to be used when defining access masks for object types –
that is to say, the set of access masks that all securable objects must
support (this value is the set of flags defined as
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">DELETE</span></span>,
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">READ\_CONTROL</span></span>,
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">WRITE\_DAC</span></span>
and
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">WRITE\_OWNER</span></span>).

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)) (with
modifications for clarity, and to show the ‘MA’ bit):

//

//<span style="mso-spacerun: yes">  </span>Define the access mask as a
longword sized structure divided up as

//<span style="mso-spacerun: yes">  </span>follows:

//

//<span style="mso-spacerun: yes">    </span>3 3 2 2 2 2 2 2 2 2 2 2 1 1
1 1 1 1 1 1 1 1

//<span style="mso-spacerun: yes">    </span>1 0 9 8 7 6 5 4 3 2 1 0 9 8
7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

//<span style="mso-spacerun: yes">  
</span>+-------+-------+---------------+-------------------------------+

//<span style="mso-spacerun: yes">   </span>|Generic|Special|
StandardRights|<span style="mso-spacerun: yes">     
</span><span style="mso-spacerun: yes">   </span>SpecificRights<span style="mso-spacerun: yes">       
</span>|

//<span style="mso-spacerun: yes">   </span>|Rights |Rights
|<span style="mso-spacerun: yes">  
</span>Rights<span style="mso-spacerun: yes">     
</span>|<span style="mso-spacerun: yes">                              
</span>|

//<span style="mso-spacerun: yes">  
</span>+-------+-------+---------------+-------------------------------+

//<span style="mso-spacerun: yes">  
</span>|G|G|G|G|Res|M|A|Res’d|S|W|W|R|D|<span style="mso-spacerun: yes">                              
</span>|

//<span style="mso-spacerun: yes">   </span>|R|W|E|A|’d
|A|S|Res’d|Y|O|D|C|E|<span style="mso-spacerun: yes">                              
</span>|

//<span style="mso-spacerun: yes">  
</span>+-------+-------+---------------+-------------------------------+

//

//<span style="mso-spacerun: yes">      </span>typedef struct
\_ACCESS\_MASK {

//<span style="mso-spacerun: yes">         
</span>WORD<span style="mso-spacerun: yes">  </span>SpecificRights;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>StandardRights;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>AccessSystemAcl : 1;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>Reserved
<span style="mso-spacerun: yes">       </span>: 3;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>GenericAll
<span style="mso-spacerun: yes">     </span>: 1;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>GenericExecute
<span style="mso-spacerun: yes"> </span>: 1;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>GenericWrite
<span style="mso-spacerun: yes">   </span>: 1;

//<span style="mso-spacerun: yes">         
</span>BYTE<span style="mso-spacerun: yes">  </span>GenericRead
<span style="mso-spacerun: yes">    </span>: 1;

//<span style="mso-spacerun: yes">      </span>} ACCESS\_MASK;

//<span style="mso-spacerun: yes">      </span>typedef ACCESS\_MASK
\*PACCESS\_MASK;

//

//<span style="mso-spacerun: yes">  </span>but to make life simple for
programmer's we'll allow them to specify

//<span style="mso-spacerun: yes">  </span>a desired access mask by
simply OR'ing together multiple single rights

//<span style="mso-spacerun: yes">  </span>and treat an access mask as a
DWORD.<span style="mso-spacerun: yes">  </span>For example

//

//<span style="mso-spacerun: yes">      </span>DesiredAccess = DELETE |
READ\_CONTROL

//

//<span style="mso-spacerun: yes">  </span>So we'll declare ACCESS\_MASK
as DWORD

//

// begin\_wdm

typedef DWORD ACCESS\_MASK;

typedef ACCESS\_MASK \*PACCESS\_MASK;

The bits in positions 28 through 31 are generic rights that can be
mapped to object-specific user rights by the resource manager for the
requested object. The mapping of these rights is
implementation-specific.

The bits in positions 24 and 25 are for maximum allowed and access
system security rights.

The bits in positions 0 through 15 are standard rights that are common
to all objects.

The document attached to this blog entry contains a variant of the
above, with all flags
defined.

# <span id="_Toc257628128"></span><span id="_Toc257628092"></span><span id="_Toc257628065"></span><span id="_Toc257627910"></span><span style="mso-bookmark: _Toc257628065"><span style="mso-bookmark: _Toc257628092"><span style="mso-bookmark: _Toc257628128"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">2<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>Generic Access Rights</span></span></span>

The four high-order bits of an
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_MASK</span></span>
specify generic access rights. Each type of securable object maps these
bits to a set of its standard and object-specific access rights.

For example, a Windows file object maps the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">GENERIC\_READ</span></span>
bit to the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">READ\_CONTROL</span></span>
and
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SYNCHRONIZE</span></span>
standard access rights and to the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">FILE\_READ\_DATA</span></span>,
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">FILE\_READ\_EA</span></span>,
and
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">FILE\_READ\_ATTRIBUTES</span></span>
object-specific access rights. Other types of objects map the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">GENERIC\_READ</span></span>
bit to whatever set of access rights is appropriate for that type of
object.

You can use generic access rights to specify the type of access you need
when you are opening a handle to an object. This is typically simpler
than specifying all the corresponding standard and specific rights.

Applications that define private securable objects can also use the
generic access rights.

**Constant**

</div>

</div>

**Value**

**Description**

GENERIC\_ALL

0x10000000

The right to read, write, and execute the object.

GENERIC\_EXECUTE

0x20000000

The right to execute or alternatively look into the object.

GENERIC\_WRITE

0x40000000

The right to write the information maintained by the object.

GENERIC\_READ

0x80000000

The right to read the information maintained by the object.

 

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)):

//

//<span style="mso-spacerun: yes">  </span>These are the generic Rights

//

\#define
GENERIC\_READ<span style="mso-spacerun: yes">                    
</span>(0x80000000L)

\#define
GENERIC\_WRITE<span style="mso-spacerun: yes">                   
</span>(0x40000000L)

\#define
GENERIC\_EXECUTE<span style="mso-spacerun: yes">                 
</span>(0x20000000L)

\#define
GENERIC\_ALL<span style="mso-spacerun: yes">                     
</span>(0x10000000L)

# <span id="_Toc257628129"></span><span id="_Toc257628093"></span><span id="_Toc257628066"></span><span id="_Toc257627911"></span><span style="mso-bookmark: _Toc257628066"><span style="mso-bookmark: _Toc257628093"><span style="mso-bookmark: _Toc257628129"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">3<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>ACCESS\_SYSTEM\_SECURITY</span></span></span>

Access system security
(<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_SYSTEM\_SECURITY</span></span>)
is used to request or indicate access to a system access control list
(SACL). This is also known as the SACL Access Right ([\[SACL Access
Right\]](http://msdn.microsoft.com/en-us/library/aa379321.aspx)).

This type of access requires the calling process to have the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SE\_SECURITY\_NAME</span></span>
(Manage auditing and security log) privilege. If this flag is set in the
access mask of an audit access ACE (successful or unsuccessful access),
the SACL access will be audited
([\[ACCESS\_MASK\]](http://msdn.microsoft.com/en-us/library/aa374892.aspx)).

The system grants this access right only if the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SE\_SECURITY\_NAME</span></span>
privilege is enabled in the access token of the requesting thread
([\[SACL Access
Right\]](http://msdn.microsoft.com/en-us/library/aa379321.aspx)).

If a backup application must have access to the system-level access
control settings, the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_SYSTEM\_SECURITY</span></span>
flag must be specified in the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">dwDesiredAccess</span></span>
parameter value passed to
[CreateFile](http://msdn.microsoft.com/en-us/library/aa363858.aspx)
([\[File Access
Rights\]](http://msdn.microsoft.com/en-us/library/aa364399.aspx)).

Privileges determine the type of system operations that a user account
can perform. An administrator assigns privileges to user and group
accounts. Each user's privileges include those granted to the user and
to the groups to which the user belongs. The
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SE\_BACKUP\_NAME</span></span>
and
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SE\_RESTORE\_NAME</span></span>
privileges are required to grant
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_SYSTEM\_SECURITY</span></span>
for backup operations ([\[Privilege
Constants\]](http://msdn.microsoft.com/en-us/library/bb530716.aspx)).

To read the SACL from a security descriptor, the calling process must
have been granted
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_SYSTEM\_SECURITY</span></span>
access when the handle was opened. The proper way to get this access is
to enable the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SE\_SECURITY\_NAME</span></span>
privilege ([\[Privilege
Constants\]](http://msdn.microsoft.com/en-us/library/bb530716.aspx)) in
the caller's current token, open the handle for
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_SYSTEM\_SECURITY</span></span>
access, and then disable the privilege
([\[GetSecurityInfo\]](http://msdn.microsoft.com/en-us/library/aa446654.aspx)).

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)):

//

// AccessSystemAcl access type

//

\#define
ACCESS\_SYSTEM\_SECURITY<span style="mso-spacerun: yes">          
</span>(0x01000000L)

# <span id="_Toc257628130"></span><span id="_Toc257628094"></span><span id="_Toc257628067"></span><span id="_Toc257627912"></span><span style="mso-bookmark: _Toc257628067"><span style="mso-bookmark: _Toc257628094"><span style="mso-bookmark: _Toc257628130"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">4<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>MAXIMUM\_ALLOWED</span></span></span>

The
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">MAXIMUM\_ALLOWED</span></span>
access type is generally used with the
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
function to determine whether a security descriptor grants a specified
set of access rights to the client identified by an access token.
Typically, server applications use this function to check access to a
private object. Note that
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">MAXIMUM\_ALLOWED</span></span>
cannot be used in an ACE (see access control entries).

<span style="mso-spacerun: yes"> </span>When using
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
for this purpose, perform the following
steps:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Obtain a security descriptor that has owner, group,
and DACL information.

If you are not impersonating a client, obtain an impersonation token by
calling
[ImpersonateSelf](http://msdn.microsoft.com/en-us/library/aa378729.aspx)(…).
This token is passed as the client token in the
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
call.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Create a generic mapping structure
([\[GENERIC\_MAPPING\]](http://msdn.microsoft.com/en-us/library/aa446633.aspx)).
The contents of this structure will vary depending on the object being
used.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">3.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>Call
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
and request
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">MAXIMUM\_ALLOWED</span></span>
as the desired access.

If the
[AccessCheck](http://msdn.microsoft.com/en-us/library/aa374815.aspx)(…)
call succeeds after the above steps have been completed, the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">GrantedAccess</span></span>
parameter contains a mask of the object-specific rights that are granted
by the security descriptor.

When used in an Access Request operation, the Maximum Allowed bit grants
the requestor the maximum permissions allowed to the object through the
Access Check Algorithm. This bit can only be requested, it cannot be set
in an ACE
([\[MS-DTYP 2.4.3\]](http://msdn.microsoft.com/en-us/library/cc230294.aspx)).

When used to set the Security Descriptor on an object, the Maximum
Allowed bit in the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SECURITY\_DESCRIPTOR</span></span>
has no meaning. The MA bit SHOULD NOT be set and SHOULD be ignored when
part of a
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">SECURITY\_DESCRIPTOR</span></span>
structure
([\[MS-DTYP 2.4.3\]](http://msdn.microsoft.com/en-us/library/cc230294.aspx)).

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)):

//

// MaximumAllowed access type

//

\#define
MAXIMUM\_ALLOWED<span style="mso-spacerun: yes">                 
</span>(0x02000000L)

# <span id="_Toc257628131"></span><span id="_Toc257628095"></span><span id="_Toc257628068"></span><span id="_Toc257627913"></span><span style="mso-bookmark: _Toc257628068"><span style="mso-bookmark: _Toc257628095"><span style="mso-bookmark: _Toc257628131"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">5<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>Standard Access Rights</span></span></span>

Standard access rights are those rights corresponding to operations
common to most types of securable objects.

**Access Right**

**Value**

**Description**

DELETE

0x00010000

The right to delete the object.

READ\_CONTROL

0x00020000

The right to read the information in the file or directory object's
security descriptor. This does not include the information in the SACL.

WRITE\_DAC

0x00040000

The right to modify the DACL in the object's security descriptor.

WRITE\_OWNER

0x00080000

The right to change the owner in the object's security descriptor.

SYNCHRONIZE

0x00100000

The right to use the object for synchronization. This enables a thread
to wait until the object is in the signaled state. Some object types do
not support this access right.

STANDARD\_RIGHTS\_REQUIRED

0x000F0000

Combines DELETE, READ\_CONTROL, WRITE\_DAC, and WRITE\_OWNER
access.<span class="Code1Char">**<sup><span style="COLOR: red; FONT-SIZE: 9pt">
1</span></sup>**</span>

STANDARD\_RIGHTS\_READ

READ\_CONTROL

Currently defined to equal READ\_CONTROL.

STANDARD\_RIGHTS\_WRITE

READ\_CONTROL

Currently defined to equal READ\_CONTROL.

STANDARD\_RIGHTS\_EXECUTE

READ\_CONTROL

Currently defined to equal READ\_CONTROL.

STANDARD\_RIGHTS\_ALL

0x001F0000

Combines DELETE, READ\_CONTROL, WRITE\_DAC, WRITE\_OWNER, and
SYNCHRONIZE
access.

<span class="Code1Char">**<sup><span style="LINE-HEIGHT: 115%; COLOR: red; FONT-SIZE: 9pt">1</span></sup>**</span><span class="Code1Char"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt"><span style="mso-tab-count: 1"> 
</span>STANDARD\_</span></span><span class="Code1Char"><span style="LINE-HEIGHT: 115%; mso-ansi-font-size: 11.0pt">RIGHTS</span></span><span class="Code1Char"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">\_REQUIRED</span></span>
is a mask meant to be used when defining access masks for object types -
it's the set of access masks that all securable objects must support.

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)):

//

//<span style="mso-spacerun: yes">  </span>The following are masks for
the predefined standard access types

//

\#define
DELETE<span style="mso-spacerun: yes">                          
</span>(0x00010000L)

\#define
READ\_CONTROL<span style="mso-spacerun: yes">                    
</span>(0x00020000L)

\#define
WRITE\_DAC<span style="mso-spacerun: yes">                       
</span>(0x00040000L)

\#define
WRITE\_OWNER<span style="mso-spacerun: yes">                     
</span>(0x00080000L)

\#define
SYNCHRONIZE<span style="mso-spacerun: yes">                     
</span>(0x00100000L)

 

\#define
STANDARD\_RIGHTS\_REQUIRED<span style="mso-spacerun: yes">        
</span>(0x000F0000L)

 

\#define
STANDARD\_RIGHTS\_READ<span style="mso-spacerun: yes">            
</span>(READ\_CONTROL)

\#define
STANDARD\_RIGHTS\_WRITE<span style="mso-spacerun: yes">           
</span>(READ\_CONTROL)

\#define
STANDARD\_RIGHTS\_EXECUTE<span style="mso-spacerun: yes">         
</span>(READ\_CONTROL)

 

\#define
STANDARD\_RIGHTS\_ALL<span style="mso-spacerun: yes">             
</span>(0x001F0000L)

# <span id="_Toc257628132"></span><span id="_Toc257628096"></span><span id="_Toc257628069"></span><span id="_Toc257627914"></span><span style="mso-bookmark: _Toc257628069"><span style="mso-bookmark: _Toc257628096"><span style="mso-bookmark: _Toc257628132"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6<span style="FONT: 7pt &#39;Times New Roman&#39;">         </span></span></span>Specific Access Rights</span></span></span>

Each type of securable object has a set of access rights that correspond
to operations specific to that type of object. These rights occupy the
low order 16 bits of the
<span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_MASK</span></span>
data type.

From WinNT.h ([\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)):

\#define
SPECIFIC\_RIGHTS\_ALL<span style="mso-spacerun: yes">             
</span>(0x0000FFFFL)

<span id="_Toc257628133"></span><span id="_Toc257628097"></span><span id="_Toc257628070"></span><span id="_Toc257627915"></span><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">The
remainder of this text lists a quorum of
</span></span></span><span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133"><span class="CodeChar"><span style="LINE-HEIGHT: 115%; FONT-SIZE: 9pt">ACCESS\_MASK</span></span>
definition sets available online
(</span></span></span></span>[<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">MSDN</span></span></span></span><span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133"></span></span></span></span>](http://msdn.microsoft.com/)<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">),
as well as in the
</span></span></span></span>[<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">Windows
SDK</span></span></span></span><span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133"></span></span></span></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">
and the
</span></span></span></span>[<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">Windows
Driver
Kit</span></span></span></span><span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133"></span></span></span></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133">).</span></span></span></span>

## <span style="mso-bookmark: _Toc257627915"><span style="mso-bookmark: _Toc257628070"><span style="mso-bookmark: _Toc257628097"><span style="mso-bookmark: _Toc257628133"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Administrations and Management</span></span></span></span>

### <span id="_Toc257628098"></span><span id="_Toc257627916"></span><span style="mso-bookmark: _Toc257628098"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.1.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Active Directory Access Rights and Service Interfaces (ADSI)</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>See this blog entry: [Active Directory Technical
Specification Control Access Rights
Concordance](http://blogs.msdn.com/openspecification/archive/2009/08/19/active-directory-technical-specification-control-access-rights-concordance.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ADS\_RIGHTS\_ENUM
Enumeration](http://msdn.microsoft.com/en-us/library/aa772285.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Directory Services Access
Rights](http://msdn.microsoft.com/en-us/library/aa446614.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-ADTS 5.1.3.2 Access
Rights](http://msdn.microsoft.com/en-us/library/cc223511.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
NtDsAPI.h

## <span id="_Toc257628134"></span><span id="_Toc257628099"></span><span id="_Toc257628071"></span><span id="_Toc257627917"></span><span style="mso-bookmark: _Toc257628071"><span style="mso-bookmark: _Toc257628099"><span style="mso-bookmark: _Toc257628134"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Component Development (COM)</span></span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ACTRL\_ACCESS\_ENTRY
Structure](http://msdn.microsoft.com/en-us/library/ms692524.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
AccCtrl.h

## <span id="_Toc257628135"></span><span id="_Toc257628100"></span><span id="_Toc257628072"></span><span id="_Toc257627918"></span><span style="mso-bookmark: _Toc257628072"><span style="mso-bookmark: _Toc257628100"><span style="mso-bookmark: _Toc257628135"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.3<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Diagnostics</span></span></span>

### <span id="_Toc257628101"></span><span id="_Toc257627919"></span><span style="mso-bookmark: _Toc257628101"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.3.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>WMI Access Rights</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[EventAccessControl
Function](http://msdn.microsoft.com/en-us/library/aa363717.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
wmistr.h

### <span id="_Toc257628102"></span><span id="_Toc257627920"></span><span style="mso-bookmark: _Toc257628102"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.3.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Namespace Access Rights</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Namespace Access Rights
Constants](http://msdn.microsoft.com/en-us/library/aa392710.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WbemCli.h

## <span id="_Toc256074721"></span><span id="_Toc257628136"></span><span id="_Toc257628103"></span><span id="_Toc257628073"></span><span id="_Toc257627921"></span><span style="mso-bookmark: _Toc257628073"><span style="mso-bookmark: _Toc257628103"><span style="mso-bookmark: _Toc257628136"><span style="mso-bookmark: _Toc256074721"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.4<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Networking</span></span></span></span><span style="mso-bookmark: _Toc256074721"></span>

### <span style="mso-bookmark: _Toc256074721"><span id="_Toc257628104"></span><span id="_Toc257627922"></span><span style="mso-bookmark: _Toc257628104"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.4.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Fax Service</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Fax Client User Access
Rights](http://msdn.microsoft.com/en-us/library/ms692351.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Specific Fax Access
Rights](http://msdn.microsoft.com/en-us/library/ms692302.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Generic Fax Access
Rights](http://msdn.microsoft.com/en-us/library/ms691834.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Required Fax Access Rights by
Function](http://msdn.microsoft.com/en-us/library/ms692330.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091)
WinFax.h

### <span id="_Toc257628105"></span><span id="_Toc257627923"></span><span id="_Toc256074722"></span><span style="mso-bookmark: _Toc257627923"><span style="mso-bookmark: _Toc257628105"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.4.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Windows Filtering Platform (WFP)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[WFP Access
Control](http://msdn.microsoft.com/en-us/library/bb442405.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
fwpmk.h

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
fwpmu.h

### <span id="_Toc257628106"></span><span id="_Toc257627924"></span><span id="_Toc256074723"></span><span style="mso-bookmark: _Toc257627924"><span style="mso-bookmark: _Toc257628106"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.4.3<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Wireless Networking</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[WlanGetSecuritySettings
Function](http://msdn.microsoft.com/en-us/library/ms706746.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[WlanSetSecuritySettings
Function](http://msdn.microsoft.com/en-us/library/ms706819.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091)
wlanapi.h

## <span id="_Toc256074725"></span><span id="_Toc257628137"></span><span id="_Toc257628107"></span><span id="_Toc257628074"></span><span id="_Toc257627925"></span><span style="mso-bookmark: _Toc257628074"><span style="mso-bookmark: _Toc257628107"><span style="mso-bookmark: _Toc257628137"><span style="mso-bookmark: _Toc256074725"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Authorization</span></span></span></span><span style="mso-bookmark: _Toc256074725"></span>

### <span style="mso-bookmark: _Toc256074725"><span id="_Toc257628108"></span><span id="_Toc257627926"></span><span style="mso-bookmark: _Toc257628108"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Token</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Access Rights for Access-Token
Objects](http://msdn.microsoft.com/en-us/library/aa374905.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=89050),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=84091)
WinNT.h

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[](http://go.microsoft.com/fwlink/?LinkID=89050)[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
ntifs.h

### <span id="_Toc257628109"></span><span id="_Toc257627927"></span><span id="_Toc256074726"></span><span style="mso-bookmark: _Toc257627927"><span style="mso-bookmark: _Toc257628109"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Access Control Entry (ACE)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ACE](http://msdn.microsoft.com/en-us/library/aa374912.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ACE
Strings](http://msdn.microsoft.com/en-us/library/aa374928.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-DTYP\] 2.4.4
ACE](http://msdn.microsoft.com/en-us/library/cc230295\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091)
Iads.h

### <span id="_Toc257628110"></span><span id="_Toc257627928"></span><span id="_Toc256074727"></span><span style="mso-bookmark: _Toc257627928"><span style="mso-bookmark: _Toc257628110"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5.3<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Audit</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[AuditSetSystemPolicy
Function](http://msdn.microsoft.com/en-us/library/aa375712.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
NTSecAPI.h

### <span id="_Toc257628111"></span><span id="_Toc257627929"></span><span id="_Toc256074728"></span><span style="mso-bookmark: _Toc257627929"><span style="mso-bookmark: _Toc257628111"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5.4<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Local Security Authority (LSA)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Security Management
Objects](http://msdn.microsoft.com/en-us/library/ms721854.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Account Object Access
Rights](http://msdn.microsoft.com/en-us/library/ms721750.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Policy Object Access
Rights](http://msdn.microsoft.com/en-us/library/ms721916.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[TrustedDomain Object Access
Rights](http://msdn.microsoft.com/en-us/library/ms722466.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Private Data Object Access
Rights](http://msdn.microsoft.com/en-us/library/ms722417.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
ntlsa.h

### <span id="_Toc257628112"></span><span id="_Toc257627930"></span><span id="_Toc256074734"></span><span id="_Toc255968540"></span><span style="mso-bookmark: _Toc256074734"><span style="mso-bookmark: _Toc257627930"><span style="mso-bookmark: _Toc257628112"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.5.5<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Security Accounts Manager Alias Specific Access Rights</span></span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\]: Security Account Manager (SAM) Remote
Protocol Specification
(Client-to-Server)](http://msdn.microsoft.com/en-us/library/cc245476\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\] 2.2.1.6 Alias ACCESS\_MASK
Values](http://msdn.microsoft.com/en-us/library/cc245524\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\] 2.2.1.4 Domain ACCESS\_MASK
Values](http://msdn.microsoft.com/en-us/library/cc245522\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\] 2.2.1.5 Group ACCESS\_MASK
Values](http://msdn.microsoft.com/en-us/library/cc245523\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\] 2.2.1.3 Server ACCESS\_MASK
Values](http://msdn.microsoft.com/en-us/library/cc245521\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-SAMR\] 2.2.1.7 User ACCESS\_MASK
Values](http://msdn.microsoft.com/en-us/library/cc245525\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
ntsam.h

## <span id="_Toc257628138"></span><span id="_Toc257628113"></span><span id="_Toc257628075"></span><span id="_Toc257627931"></span><span style="mso-bookmark: _Toc257628075"><span style="mso-bookmark: _Toc257628113"><span style="mso-bookmark: _Toc257628138"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>System Services</span></span></span>

#### <span id="_Toc257627932"></span><span id="_Toc256074741"></span><span style="mso-bookmark: _Toc257627932"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.1.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Console*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Console Buffer Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms682062.aspx)

### <span id="_Toc257628114"></span><span id="_Toc257627933"></span><span style="mso-bookmark: _Toc257628114"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>DLLs, Processes and Threads</span>

#### <span id="_Toc257627934"></span><span id="_Toc256074743"></span><span style="mso-bookmark: _Toc257627934"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.2.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Process*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Process Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms684880.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

#### <span id="_Toc257627935"></span><span id="_Toc256074744"></span><span style="mso-bookmark: _Toc257627935"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.2.2*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Job*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Job Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms684164.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

#### <span id="_Toc257627936"></span><span id="_Toc256074745"></span><span style="mso-bookmark: _Toc257627936"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.2.3*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Thread*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Thread Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686769.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=89050),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=84091)
WinNT.h

### <span id="_Toc257628115"></span><span id="_Toc257627937"></span><span id="_Toc256074746"></span><span style="mso-bookmark: _Toc257627937"><span style="mso-bookmark: _Toc257628115"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.3<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Window Station</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Window Station Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms687391.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinUser.h

### <span id="_Toc257628116"></span><span id="_Toc257627938"></span><span id="_Toc256074747"></span><span style="mso-bookmark: _Toc257627938"><span style="mso-bookmark: _Toc257628116"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.4<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Desktop</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Desktop Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms682575.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinUser.h

### <span id="_Toc257628117"></span><span id="_Toc257627939"></span><span style="mso-bookmark: _Toc257628117"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.5<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Services</span>

#### <span id="_Toc257627940"></span><span id="_Toc256074749"></span><span style="mso-bookmark: _Toc257627940"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.5.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Service Control Manager*</span>

<span id="_Toc256074750"></span><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[<span style="mso-bookmark: _Toc256074750">Service
Security and Access
Rights</span><span style="mso-bookmark: _Toc256074750"></span>](http://msdn.microsoft.com/en-us/library/ms685981.aspx)<span style="mso-bookmark: _Toc256074750"></span>

<span style="mso-bookmark: _Toc256074750"><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span></span>[<span style="mso-bookmark: _Toc256074750">\[SDK\]</span><span style="mso-bookmark: _Toc256074750"></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc256074750">,
</span>[<span style="mso-bookmark: _Toc256074750">\[WDK\]</span><span style="mso-bookmark: _Toc256074750"></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc256074750">
WinSvc.h</span>

#### <span style="mso-bookmark: _Toc256074750"><span id="_Toc257627941"></span><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.5.2*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Service*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Service Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms685981.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinSvc.h

### <span id="_Toc257628118"></span><span id="_Toc257627942"></span><span id="_Toc256074751"></span><span style="mso-bookmark: _Toc257627942"><span style="mso-bookmark: _Toc257628118"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.6<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Synchronization Objects</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Synchronization Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686670.aspx)

#### <span id="_Toc257627943"></span><span id="_Toc256074752"></span><span style="mso-bookmark: _Toc257627943"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.6.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Event*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Synchronization Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686670.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

#### <span id="_Toc257627944"></span><span id="_Toc256074753"></span><span style="mso-bookmark: _Toc257627944"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.6.2*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Mutex*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Synchronization Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686670.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
winbase.h

#### <span id="_Toc257627945"></span><span id="_Toc256074754"></span><span style="mso-bookmark: _Toc257627945"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.6.3*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Semaphore*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Synchronization Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686670.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

#### <span id="_Toc257627946"></span><span id="_Toc256074755"></span><span style="mso-bookmark: _Toc257627946"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.6.4*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Timer*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Synchronization Object Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms686670.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

### <span id="_Toc257628119"></span><span id="_Toc257627947"></span><span style="mso-bookmark: _Toc257628119"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.7<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>File Services</span>

#### <span id="_Toc257627948"></span><span id="_Toc256074757"></span><span id="_Toc255970404"></span><span style="mso-bookmark: _Toc256074757"><span style="mso-bookmark: _Toc257627948"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.7.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*File Access Rights*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[File Security and Access
Rights](http://msdn.microsoft.com/en-us/library/aa364399.aspx)

<span id="_Toc256074758"></span><span id="_Toc255970405"></span><span style="mso-bookmark: _Toc256074758"><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span></span>[<span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758">\[SDK\]</span></span><span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758"></span></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758">,
</span></span>[<span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758">\[WDK\]</span></span><span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758"></span></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758">
WinNT.h</span></span>

#### <span style="mso-bookmark: _Toc255970405"><span style="mso-bookmark: _Toc256074758"><span id="_Toc257627949"></span><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.7.2*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*File Mapping*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[File Mapping Security and Access
Rights](http://msdn.microsoft.com/en-us/library/aa366559.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ZwCreateSection](http://msdn.microsoft.com/en-us/library/ms804361.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

#### <span id="_Toc257627950"></span><span id="_Toc256074759"></span><span style="mso-bookmark: _Toc257627950"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.7.3*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Pipes*</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[About
Pipes](http://msdn.microsoft.com/en-us/library/aa365780.aspx)

##### <span id="_Toc257627951"></span><span id="_Toc256074760"></span><span style="mso-bookmark: _Toc257627951"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.7.3.1<span style="FONT: 7pt &#39;Times New Roman&#39;">        </span></span></span>Anonymous Pipes</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Anonymous Pipe Security and Access
Rights](http://msdn.microsoft.com/en-us/library/aa365142.aspx)

##### <span id="_Toc257627952"></span><span id="_Toc256074761"></span><span id="_Toc255970406"></span><span style="mso-bookmark: _Toc256074761"><span style="mso-bookmark: _Toc257627952"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.7.3.2<span style="FONT: 7pt &#39;Times New Roman&#39;">        </span></span></span>Named Pipe</span></span><span style="mso-bookmark: _Toc256074761"><span style="mso-bookmark: _Toc257627952">s</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Named Pipe Security and Access
Rights](http://msdn.microsoft.com/en-us/library/aa365600.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

### <span id="_Toc257628120"></span><span id="_Toc257627953"></span><span id="_Toc256074762"></span><span style="mso-bookmark: _Toc257627953"><span style="mso-bookmark: _Toc257628120"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.8<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Registry</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Registry Key Security and Access
Rights](http://msdn.microsoft.com/en-us/library/ms724878.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

### <span id="_Toc257628121"></span><span id="_Toc257627954"></span><span id="_Toc256074763"></span><span style="mso-bookmark: _Toc257627954"><span style="mso-bookmark: _Toc257628121"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.9<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Kernel Transaction Manager (KTM)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Kernel Transaction Manager
Constants](http://msdn.microsoft.com/en-us/library/aa366267.aspx)

#### <span id="_Toc257627955"></span><span id="_Toc256074764"></span><span id="_Toc255970812"></span><span style="mso-bookmark: _Toc256074764"><span style="mso-bookmark: _Toc257627955"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.9.1*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Enlistment*</span></span><span style="mso-bookmark: _Toc256074764"><span style="mso-bookmark: _Toc257627955"> *(KTM)*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Enlistment Access
Masks](http://msdn.microsoft.com/en-us/library/aa366021.aspx)

<span id="_Toc256074765"></span><span id="_Toc255970813"></span><span style="mso-bookmark: _Toc256074765"><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span></span>[<span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765">\[SDK\]</span></span><span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765"></span></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765">,
</span></span>[<span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765">\[WDK\]</span></span><span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765"></span></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765">
WinNT.h</span></span>

#### <span style="mso-bookmark: _Toc255970813"><span style="mso-bookmark: _Toc256074765"><span id="_Toc257627956"></span><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.9.2*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Resource Manager*</span></span><span style="mso-bookmark: _Toc256074765"><span style="mso-bookmark: _Toc257627956"> *(KTM)*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Resource Manager Access
Masks](http://msdn.microsoft.com/en-us/library/aa366359.aspx)

<span id="_Toc256074766"></span><span id="_Toc255970814"></span><span style="mso-bookmark: _Toc256074766"><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span></span>[<span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766">\[SDK\]</span></span><span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766"></span></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766">,
</span></span>[<span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766">\[WDK\]</span></span><span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766"></span></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766">
WinNT.h</span></span>

#### <span style="mso-bookmark: _Toc255970814"><span style="mso-bookmark: _Toc256074766"><span id="_Toc257627957"></span><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.9.3*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Transaction*</span></span><span style="mso-bookmark: _Toc256074766"><span style="mso-bookmark: _Toc257627957"> *(KTM)*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Transaction Access
Masks](http://msdn.microsoft.com/en-us/library/aa366384.aspx)

<span id="_Toc256074767"></span><span id="_Toc255970815"></span><span style="mso-bookmark: _Toc256074767"><span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span></span>[<span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767">\[SDK\]</span></span><span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767"></span></span>](http://go.microsoft.com/fwlink/?LinkID=84091)<span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767">,
</span></span>[<span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767">\[WDK\]</span></span><span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767"></span></span>](http://go.microsoft.com/fwlink/?LinkID=89050)<span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767">
WinNT.h</span></span>

#### <span style="mso-bookmark: _Toc255970815"><span style="mso-bookmark: _Toc256074767"><span id="_Toc257627958"></span><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">*6.6.9.4*<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>*Transaction Manager*</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Transaction Manager Access
Masks](http://msdn.microsoft.com/en-us/library/aa366390.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

### <span id="_Toc257628122"></span><span id="_Toc257627959"></span><span id="_Toc256074768"></span><span style="mso-bookmark: _Toc257627959"><span style="mso-bookmark: _Toc257628122"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.10<span style="FONT: 7pt &#39;Times New Roman&#39;">  </span></span></span>Memory Management</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[ZwCreateSection (Section Access
Rights)](http://msdn.microsoft.com/en-us/library/ms804361.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Section Objects and
Views](http://msdn.microsoft.com/en-us/library/ms796304.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinNT.h

### <span id="_Toc257628123"></span><span id="_Toc257627960"></span><span id="_Toc256074769"></span><span style="mso-bookmark: _Toc257627960"><span style="mso-bookmark: _Toc257628123"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.6.11<span style="FONT: 7pt &#39;Times New Roman&#39;">  </span></span></span>Installable File System Drivers (Windows Driver Kit)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[FltBuildDefaultSecurityDescriptor (FLT Access
Rights)](http://msdn.microsoft.com/en-us/library/aa488551.aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
fltKernel.h

## <span id="_Toc257628139"></span><span id="_Toc257628124"></span><span id="_Toc257628076"></span><span id="_Toc257627961"></span><span style="mso-bookmark: _Toc257628076"><span style="mso-bookmark: _Toc257628124"><span style="mso-bookmark: _Toc257628139"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.7<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Open Specifications</span></span></span>

### <span id="_Toc257628125"></span><span id="_Toc257627962"></span><span id="_Toc256074771"></span><span style="mso-bookmark: _Toc257627962"><span style="mso-bookmark: _Toc257628125"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.7.1<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Printing (Windows Communication Protocols (MCPP))</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RPRN\]: Print System Remote Protocol
Specification](http://msdn.microsoft.com/en-us/library/cc244528\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[Using the Windows
Headers](http://msdn.microsoft.com/en-us/library/aa383745\(VS.85\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RPRN\] 2.2.3.1 Access
Values](http://msdn.microsoft.com/en-us/library/cc244650\(PROT.10\).aspx)
(Print
Jobs)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RPRN\] 2.2.3.1 Access
Values](http://msdn.microsoft.com/en-us/library/cc244650\(PROT.10\).aspx)
(Print Server
Printer)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RPRN\] 2.2.3.1 Access
Values](http://msdn.microsoft.com/en-us/library/cc244650\(PROT.10\).aspx)
(Print Server Remote
Protocol)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[SDK\]](http://go.microsoft.com/fwlink/?LinkID=84091),
[\[WDK\]](http://go.microsoft.com/fwlink/?LinkID=89050)
WinSpool.h

### <span id="_Toc257628126"></span><span id="_Toc257627963"></span><span id="_Toc256074775"></span><span style="mso-bookmark: _Toc257627963"><span style="mso-bookmark: _Toc257628126"><span style="mso-fareast-font-family: Cambria; mso-bidi-font-family: Cambria; mso-bidi-theme-font: major-latin; mso-fareast-theme-font: major-latin"><span style="mso-list: Ignore">6.7.2<span style="FONT: 7pt &#39;Times New Roman&#39;">       </span></span></span>Windows Internet Naming Service (WINS)</span></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RAIW\]: Remote Administrative Interface: WINS
Specification](http://msdn.microsoft.com/en-us/library/dd240484\(PROT.10\).aspx)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>[\[MS-RAIW\] 2.1.1 Server Security
Settings](http://msdn.microsoft.com/en-us/library/dd240499\(PROT.10\).aspx)

 

[Access Mask
Blog.zip](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/09/98/88/74/Access%20Mask%20Blog.zip)
