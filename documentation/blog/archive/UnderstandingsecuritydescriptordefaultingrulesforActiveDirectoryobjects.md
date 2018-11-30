<div id="page">

# Understanding security descriptor defaulting rules for Active Directory objects

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
8/28/2009 5:52:00 PM

-----

<div id="content">

This blog post is to help understand the defaulting rules when assigning
security descriptors to new active directory (AD) objects.

# Background

The SECURITY\_DESCRIPTOR structure defines the security attributes of an
object. For instance, some of the attributes specify the owner, the
access rights, privileges to perform operations, and level of audit
logging.

The SECURITY\_DESCRIPTOR structure is defined in \[MS-DTYP\] Section
2.4.6 <http://msdn.microsoft.com/en-us/library/cc230366.aspx>.

    typedef struct _SECURITY_DESCRIPTOR {

      UCHAR Revision;

      UCHAR Sbz1;

      USHORT Control;

      PSID Owner;

      PSID Group;

      PACL Sacl;

      PACL Dacl;

``` 
 } SECURITY_DESCRIPTOR, 
```

     *PSECURITY_DESCRIPTOR;

The Owner field identifies the security identifier (SID) of the object
owner. The Group field is the SID of group of the object. The DACL field
is the discretionary access control list that specifies the user or
group's access on the object. The SACL is the system access control list
and controls the audit logging on the object.

# Security descriptor for new objects

\[MS-DTYP\] Section 2.5.2.3 defines the CreateSecurityDescriptor
algorithm, which in general, is applicable to all types of objects, e.g.
file objects, registry objects, AD objects.

There are four main steps in the procedure of computing a security
descriptor for a new object:

Step 1: Compute the Owner field

Step 2: Compute the Group field

Step 3: Compute the DACL

Step 4: Compute the SACL

In the next sections, we review these steps for active directory
objects.

# Security descriptor for new AD objects

There are some specifics related to AD objects. \[MS-ADTS\] Section
7.1.3 defines Security Descriptor Requirements for AD objects. It
presents the parameters used by the CreateSecurityDescriptor algorithm
to compute the resultant security descriptor value of an AD object.

## Owner and Group defaulting rules

As specified in \[MS-ADTS\] <span id="_Toc238247818"></span>7.1.3.6
Owner and Group Defaulting
Rules<span id="z3ac403a64e8e488d8ef6c7fa1aa785b6"></span>, the OWNER and
GROUP fields are defaulted in the following conditions on the supplied
control flags (see \[MS-ADTS\] 7.1.3.2   SD Flags Control):

§ The security descriptor flags do not include the OWNER bit.

§ The security descriptor flags include the OWNER bit, but the OWNER
field in the supplied value is NULL.

In the preceding cases, the OWNER field is defaulted as follows:

1\. The subject is a member of the default administrator group (DAG) for
the object; the SID of this group becomes the OWNER SID in the security
descriptor.

2\. The subject's security context contains the TokenOwner field, then
the SID contained in this field becomes the OWNER SID of the security
descriptor.

3\. The subject's SID becomes the OWNER SID of the security descriptor.

In case of a domain functional level of Windows Server 2008 or higher,
if the DAG SID is used as the default OWNER field value, then the same
SID is written into the GROUP field.

In all other cases, the GROUP field is not modified before the security
descriptor value is passed to the CreateSecurityDescriptor algorithm.

Note: A subject is a thread executing in the security context provided
by an access token. The creator of the object is the subject that is
creating the object.

## Rules for Default Administrator Group (DAG)

The rules for the DAG, which is used for OWNER/GROUP defaulting and for
OWNER access checks, are defined \[MS-ADTS\] Section
<span id="_Toc238247819"></span>7.1.3.7   Default Administrators
Group<span id="z33abc2177906429eb66cdac92ce4f453"></span>. The rules
depend on the object location (naming context of domain, configuration,
schema, or application), and the requestor token (member of these groups
Schema Admins, Domain Administrators, Enterprise Administrators).  

## Computing resultant ACL and defaulting rules

When computing the resultant ACL for a new object, two important aspects
are ACL inheritance and their defaulting rules. It is important to note
these key points when an ACL is built for an AD object compared to other
types of objects:

  - Generic inheritable ACEs apply to all types of child objects.
    Object-specific inheritable ACEs apply only to a specific type of
    child object.
  - If there is no specified security descriptor, no parent-inheritable
    ACEs, the operating system uses the ACL from the
    defaultSecurityDescriptor in the classSchema object, and in the
    absence thereof, the ACL from the creator's token.

The following procedure summarizes the steps to compute the resultant
ACL for a new AD object. This procedure is based on the
CreateSecurityDescriptor algorithm with amendments pertaining to the
defaultSecurityDescriptor attribute of AD object classes.

  - Condition 1: Parent contains inheritable ACEs  
    Step 1. If the control flags on the client allow inheritance then
    the inheritable ACEs from the parent form the newly created object's
    initial DACL and SACL  
    Step 2. If the control flags on the client do not allow inheritance,
    set initial DACL and SACL to NULL  
    Step 3. If an explicit security descriptor is provided by the
    client, it is merged with the initial DACL and SACL.  
    Step 4. If an explicit security descriptor is not provided by the
    client then the DACL and SACL from the defaultSecurityDescriptor are
    merged with the initial DACL and SACL.
  - Condition 2: Parent does not contain inheritable ACEs and the
    classSchema of the object being created has a
    defaultSecurityDescriptor.  
    If the defaultSecurityDesciptor is present, then the ACLs should be
    obtained from the defaultSecurityDescriptor.
  - Condition 3: Parent does not contain inheritable ACEs and
    defaultSecurityDescriptor is absent in the classSchema of the object
    being created  
    If the defaultSecurityDescriptor is absent, then make use of Token
    as described in the ComputeACL method (MS-DTYP 2.5.2.4).

 

References

\[MS-DTYP\]: Windows Data Types
<http://msdn.microsoft.com/en-us/library/cc230273.aspx>

\[MS-ADTS\]: Active Directory Technical Specification
<http://msdn.microsoft.com/en-us/library/cc223122.aspx>

 

</div>

</div>
