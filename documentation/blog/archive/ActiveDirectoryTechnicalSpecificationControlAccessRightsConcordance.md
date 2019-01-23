<div id="page">

# Active Directory Technical Specification Control Access Rights Concordance

[Bill Wesse](https://social.msdn.microsoft.com/profile/Bill%20Wesse)
8/19/2009 9:37:00
AM

-----

<div id="content">

**<span style="LINE-HEIGHT: 115%; COLOR: #404040; FONT-SIZE: 16pt; mso-themecolor: text1; mso-themetint: 191">Active
Directory Technical Specification Control Access Rights
Concordance</span>**

The attached PDF document provides a concordance for the Microsoft
Active Directory ‘Control Access Rights’ documented in the Microsoft
[Open
Specifications](http://msdn.microsoft.com/en-us/library/dd208104.aspx),
as of 10 August 2009. The main reference for these rights is \[MS‑ADTS\]
[5.1.3.2.1](http://msdn.microsoft.com/en-us/library/cc223512.aspx)
Control Access Rights.

I created the document as a useful reference for locating the various
‘Control Access Rights’ in the (currently 10, listed at the end of
this entry) specification documents to assist myself in understanding
them.

The following text is based on \[MS‑ADTS\]
[5.1.3.2.1](http://msdn.microsoft.com/en-us/library/cc223512.aspx)
Control Access Rights:

All objects in Active Directory Domain Services support a standard set
of access rights defined in the
[ADS\_RIGHTS\_ENUM](http://msdn.microsoft.com/en-us/library/aa772285.aspx)
enumeration. These access rights can be used in the Access Control
Entries (ACEs) of an object's security descriptor to control access to
the object; that is, to control who can perform standard operations,
such as creating and deleting child objects, or reading and writing the
object attributes. However, for some object classes, it may be desirable
to control access in a way not supported by the standard access rights.
To facilitate this, Active Directory Domain Services allow the standard
access control mechanism to be extended through the
[controlAccessRight](http://msdn.microsoft.com/en-us/library/cc221827.aspx)
object.

An Active Directory control access right is not identified by a specific
bit in an access mask as the standard access rights are. Instead, each
control access right is identified by a GUID.

Control access rights are used in three
ways:

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>For extended rights, which are special operations
not covered by the standard set of access rights. For example, the user
class can be granted a "Send As" right that can be used by Exchange,
Outlook, or any other mail application, to determine whether a
particular user can have another user send mail on their behalf.
Extended rights are created on
[controlAccessRight](http://msdn.microsoft.com/en-us/library/cc221827.aspx)
objects by setting the
[validAccesses](http://msdn.microsoft.com/en-us/library/cc220991.aspx)
attribute to equal the ADS\_RIGHT\_DS\_CONTROL\_ACCESS (256) access
right.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>For defining property sets, to enable controlling
access to a subset of an object's attributes, rather than just to the
individual attributes. Using the standard access rights, a single ACE
can grant or deny access to all of an object's attributes or to a single
attribute. Control access rights provide a way for a single ACE to
control access to a set of attributes. For example, the user class
supports the Personal-Information property set that includes attributes
such as street address and telephone number. Property set rights are
created on
[controlAccessRight](http://msdn.microsoft.com/en-us/library/cc221827.aspx)
objects by setting the
[validAccesses](http://msdn.microsoft.com/en-us/library/cc220991.aspx)
attribute to contain both the ACTR\_DS\_READ\_PROP (16) and the
ACTRL\_DS\_WRITE\_PROP (32) access
rights.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>For validated writes, to require that the system
perform value checking, or validation, beyond that which is required by
the schema, before writing a value to an attribute on a DS object. This
ensures that the value entered for the attribute conforms to required
semantics, is within a legal range of values, or undergoes some other
special checking that would not be performed for a simple low-level
write to the attribute. A validated write is associated to a special
permission that is distinct from the "Write \<attribute\>" permission
that would allow any value to be written to the attribute with no value
checking performed. The validated write is the only one of the three
control access rights that cannot be created as a new control access
right for an application. This is because the existing system cannot be
programmatically modified to enforce validation. If a control access
right was set up in the system as a validated write, the
[validAccesses](http://msdn.microsoft.com/en-us/library/cc220991.aspx)
attribute on the
[controlAccessRight](http://msdn.microsoft.com/en-us/library/cc221827.aspx)
objects will contain the ADS\_RIGHT\_DS\_SELF (8) access right. There
are only three validated writes defined in the Windows 2000 Active
Directory
schema:

 

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>[Self-Membership](http://msdn.microsoft.com/en-us/library/ms684405.aspx)
permission on a Group object, which allows the caller's account, but no
other account, to be added or removed from a group's
membership.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>[Validated-DNS-Host-Name](http://msdn.microsoft.com/en-us/library/ms684416.aspx)
permission on a Computer object, which allows a DNS host name attribute
that is compliant with the computer name and domain name to be
set.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>[Validated-SPN](http://msdn.microsoft.com/en-us/library/ms684417.aspx)
permission on a Computer object, which allows an SPN attribute which is
compliant with the DNS host name of the computer to be set.

Open Specification documents referenced in the attachment:

[\[MS-ADA1\]: Active Directory Schema Attributes
A-L](http://msdn.microsoft.com/en-us/library/cc219751.aspx)

[\[MS-ADA2\]: Active Directory Schema Attributes
M](http://msdn.microsoft.com/en-us/library/cc220154.aspx)

[\[MS-ADA3\]: Active Directory Schema Attributes
N-Z](http://msdn.microsoft.com/en-us/library/cc220699.aspx)

[\[MS-ADLS\]: Active Directory Lightweight Directory Services
Schema](http://msdn.microsoft.com/en-us/library/cc221082.aspx)

[\[MS-ADSC\]: Active Directory Schema
Classes](http://msdn.microsoft.com/en-us/library/cc221630.aspx)

[\[MS-ADTS\]: Active Directory Technical
Specification](http://msdn.microsoft.com/en-us/library/cc223122.aspx)

[\[MS-DRSR\]: Directory Replication Service (DRS) Remote Protocol
Specification](http://msdn.microsoft.com/en-us/library/cc228086.aspx)

[\[MS-DTYP\]: Windows Data
Types](http://msdn.microsoft.com/en-us/library/cc230273.aspx)

[\[MS-KILE\]: Kerberos Protocol
Extensions](http://msdn.microsoft.com/en-us/library/cc233855.aspx)

[\[MS-SAMR\]: Security Account Manager (SAM) Remote Protocol
Specification
(Client-to-Server)](http://msdn.microsoft.com/en-us/library/cc245476.aspx)

[\[MS-SFU\]: Kerberos Protocol Extensions: Service for User and
Constrained Delegation Protocol
Specification](http://msdn.microsoft.com/en-us/library/cc246071.aspx)

[Control Access Rights
Concordance.pdf](images/Control%20Access%20Rights%20Concordance.pdf)

</div>

</div>
