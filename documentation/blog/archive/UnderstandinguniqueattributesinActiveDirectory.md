<div id="page">

# Understanding unique attributes in Active Directory

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
7/10/2009 2:57:00 PM

-----

<div id="content">

In this blog, I would like to help the reader understand the rules that
govern unique attributes in Active Directory (AD) along with the Open
Protocols documentation set. I provide examples for user naming
attributes. I also clarify common misunderstandings about attribute
uniqueness and attribute indexing.

This blog post does not intend to provide a tutorial on LDAP. I assume
the reader is familiar with   directory services concepts as detailed in
IETF LDAP specifications (RFC2251 through RFC2256, RFC3377, or the
latest RFC4510 through 4519, etc). The reader is also assumed to have
hands-on experience with Active Directory.

## The common issue                                      

You may have encountered some of the following LDAP errors as results of
attempts to add or modify entries in an Active Directory server:

  - constraintViolation (19) \[RFC4511\]
  - attributeOrValueExists (20) \[RFC4511\]
  - entryAlreadyExists (68) \[RFC4511\]

Now you wonder the intricacies of the processing behind the operation
and why it didn't succeed, even though you have attempted to add the
entry in a newly created sub-tree. You decide to probe the schema, and
pull out the relevant classSchema objects and attributeSchema objects.
But still, there is no conclusive information. I will try to discuss
this in the next paragraphs.

## Attribute uniqueness in AD

Attribute uniqueness is enforced by constraints or pre-operation
triggers that apply associated checks. The directory server performs
checks on all update operations before it makes changes to the
directory. Examples of update operations are Add, Modify, and Rename.

Active Directory does not provide any means for identifying unique
attributes. Some attributes are unique as a result of their specific
processing rules defined by the protocols. In a broader AD view, this
translates into associated constraints and triggers applied to objects
during replica update operations. These rules depend on objects and may
apply to a naming context (NC), a container, a distinguished name (DN),
a domain, or an entire forest, etc.

## Clarifying attribute indexing and uniqueness

Attribute indexing and attribute value uniqueness are not interrelated
concepts in AD. A single-valued or multi-valued attribute can be
indexed. And the values may require uniqueness or not, depending on
their rules as we previously introduced. For each attribute, the scope
of uniqueness is defined by the processing rules and constraints
specified by the protocol.

## Key references in protocol specifications

Updates operations and their constraints are generally defined in
[\[MS-ADTS\]](http://msdn.microsoft.com/en-us/library/cc223122\(PROT.13\).aspx)
3.1.1.5. In addition, SAM-specific objects creation and modification
apply additional constraints defined in
[\[MS-SAMR\]](http://msdn.microsoft.com/en-us/library/cc245476\(PROT.13\).aspx)
Section 3.1.1.6.

In the following examples, we introduce some unique attributes and link
them to the documentation.

## Example1: Distinguished names

Distinguished names (DNs) are unique and they unambiguously identify
objects in the directory. The directory server does not allow two
objects with the same Relative DN (RDN) under the same parent or
container. A DN is composed of a RDN, and its container's path.
Therefore the uniqueness of RDNs guarantees the uniqueness of DNs. For
more details see <span id="_Toc234619514"></span>\[MS-ADTS\]
3.1.1.3.1.2.3   Multivalued and Multiple-Attribute
RDNs<span id="z3c96b56dd7a746f198837d031f9fa01e"></span>.

A simple example of DN is: CN=username1,OU=Staff,DC=contoso,DC=com

## Example 2: sAMAccountName

\[MS-SAMR\] <span id="_Toc232189819"></span>"3.1.1   Abstract Data
Model<span id="z814e12f610374b648d288f1b899dc57f"></span>" explains
SAM-related constraints relationships between attributes and triggers
defined respectively in Sections 3.1.1.6 and 3.1.1.8. Section 3.1.1.8.4
of \[MS-SAMR\] describes the uniqueness requirements for sAMAccountName
(\[MS-ADA3\] 2.221).  

SAMAccountName is unique and also indexed. The sAMAccountName must be
unique among all security principal objects within the domain. For
instance, attempting to add the following objects will result in an LDAP
"Already exists" error.

dn: CN=username1,OU=Staff,DC=contoso,DC=com

 sAMAccountName: username1

 objectClass: user

 dn: CN=username1,OU=Managers,DC=contoso,DC=com

 sAMAccountName: username1

 objectClass: user

## Example 3: Uniqueness scope of user naming attributes

A user's RDN is the value of its canonical name or cn attribute. Like
other directory objects, a user object has names in the form of cn,
name, distinguishedName, and objectGUID. A user object is a security
principal object, and it has additional naming attributes. We summarize
the scope of uniqueness of these attributes in the following table.

**Attribute**

</div>

</div>

**Description**

**Uniqueness scope**

**userPrincipalName**

The userPrincipalName  is a single-valued and indexed attribute that is
a string that specifies the user principal name (UPN). The UPN is an
Internet-style login name for the user based on the Internet standard
[\[RFC822\]](http://go.microsoft.com/fwlink/?LinkId=90497). It should
map to the user e-mail name in the form of \<name\>@\<tree name\> where
\<name\> is the user name such as the sAMAccountName and the \<tree
name\> is the domain.

\[MS-ADA3\] 2.348 Attribute userPrincipalName

\[MS-ADTS\] 3.1.1.5.2.2   Add Operation Contraints

The UPN must be unique among all security principal objects within the
directory forest.

**objectGUID**

The objectGUID attribute is a single-valued attribute that is the unique
identifier for the object. This attribute is a Globally Unique
Identifier (GUID).

\[MS-ADA3\] 2.43 Attribute objectGUID<span id="_Toc234619438"></span>

\[MS-ADTS\] 3.1.1.1.3   Basics, objectGUID, and Special Attribute
Behavior<span id="zdd4dc725021b4c8ca44a49b3235836b7"></span>

\[MS-ADTS\] 3.1.1.5.2.2   Add Operation Contraints

The GUID is unique across the enterprise and anywhere else.

**sAMAccountName**

The sAMAccountName attribute is a single-valued attribute that is the
logon name used to support clients and servers from a previous version
of Windows (such as Windows NT 4.0 and earlier, Windows 95, Windows 98,
and LAN Manager). The sAMAccountName should be less than 20 characters
to support these clients and servers.

\[MS-ADA3\] 2.221 Attribute sAMAccountName

 \[MS-SAMR\] 3.1.1.8.4 Uniqueness requirements for sAMAccountName

The sAMAccountName must be unique among all security principal objects
within the domain.

**objectSid**

The objectSid attribute is a single-valued attribute that specifies the
security identifier (SID) of the user. The SID is a unique value used to
identify the user as a security principal. It is a binary value that is
set by the system when the user is created.

\[MS-ADA3\] 2.44 Attribute objectSid

\[MS-ADTS\] 3.1.1.1.3   Basics, objectGUID, and Special Attribute
Behavior

\[MS-ADTS\] 3.1.1.5.2.2   Add Operation Contraints

The objectSid is unique across the domain.

 

**sIDHistory**

The sIDHistory attribute is a multi-valued attribute that contains
previous SIDs used for the user object if the user was moved from
another domain.

\[MS-ADA3\] 2.266 Attribute sIDHistory

ObjectSid is unique per domain, and the sIDHistory attribute contains
SIDs from the user's previous domain moves.

Note: Naming attributes can be viewed and managed by the Active
Directory User and Computers snap-in.

## References

[\[MS-ADTS\]: Active Directory Technical
Specification](http://msdn.microsoft.com/en-us/library/cc223122\(PROT.13\).aspx)

[\[MS-ADSC\]: Active Directory Schema
Classes](http://msdn.microsoft.com/en-us/library/cc221630\(PROT.13\).aspx)

[\[MS-ADA1\]: Active Directory Schema Attributes
A-L](http://msdn.microsoft.com/en-us/library/cc219751\(PROT.13\).aspx) 

[\[MS-ADA2\]: Active Directory Schema Attributes
M](http://msdn.microsoft.com/en-us/library/cc220154\(PROT.13\).aspx)

[\[MS-ADA3\]: Active Directory Schema Attributes
N-Z](http://msdn.microsoft.com/en-us/library/cc220699\(PROT.13\).aspx)

[\[MS-SAMR\]: Security Account Manager (SAM) Remote Protocol
Specification
(Client-to-Server)](http://msdn.microsoft.com/en-us/library/cc245476\(PROT.13\).aspx)

[User Naming
Attributes](http://msdn.microsoft.com/en-us/library/ms677605.aspx)

[Indexed
attributes](http://msdn.microsoft.com/en-us/library/ms675095\(VS.85\).aspx)
