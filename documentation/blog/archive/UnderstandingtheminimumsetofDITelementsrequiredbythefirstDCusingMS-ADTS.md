<div id="page">

# Understanding the minimum set of DIT elements required by the first DC using MS-ADTS

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
3/18/2010 1:38:00 PM

-----

<div id="content">

    Active Directory contains a group of objects that also have a group
of attributes.<span style="mso-spacerun: yes">  </span>All of them exist
for some reasons.<span style="mso-spacerun: yes">  </span>Some of them
are essential for the existence of Active Directory, while others are
stored and used by other applications, such as Exchange Server, Terminal
Server etc. <span style="mso-spacerun: yes"> </span>When a Domain
Controller is created as the first one in a new domain within a new
forest, its Active Directory DIT includes many objects and attributes.
You can view all the objects on a newly created DC using LDIFDE tool.
<span style="mso-spacerun: yes"> </span>But what is the minimum set of
DIT elements (objects and attributes) required for the proper
functioning of a DC that is the initiator of a forest? We received this
question from some protocol implementers who try to understand what
objects their AD compatible implementations have to create in order to
act as the initiator of a forest. The answer lies in section 7 of
MS-ADTS. This short blog will describe how we got a programming readable
list of objects required for being the first DC.

  
[MS-ADTS](http://msdn.microsoft.com/en-us/library/cc223122\(PROT.10\).aspx)
is the primary specification for Active Directory.
<span style="mso-spacerun: yes"> </span>In section 7, it specifies the
objects that are necessary for the proper functioning of the DCs in a
forest, and the requirements that govern the state of these objects.
<span style="mso-spacerun: yes"> </span>As a requirement of the
documentation, all the objects required in a DC should be found in this
section, but they are not listed in a machine readable format that can
be consumed by a software program. The dump of all objects in a new
Windows DC created using LDIFDE provides a good start, but it is not
exactly the minimum set of objects and attributes due to the following
reasons:

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Some objects that are created for some Windows
application
components.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Some objects are not essential for the basic
operation of Active
Directory.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Some objects or attributes exist as the choice of
the Windows implementation or configuration.

   We used the dump of Configuration NC and Domain NC from a new DC as
the base, and eliminated all the objects not covered in section 7 of
MS-ADTS. Once the list was obtained, we validated it via source code
review. <span style="mso-spacerun: yes"> </span>The list,
Windows2008R2\_Minimum\_DIT\_Consolidated.txt, is attached in this blog.
<span style="mso-spacerun: yes"> </span>It is very easy for people to
view what are in a DC initially using various AD tools, so it is worth
pointing out what objects or attributes are not essential for the proper
functioning of Active Directory when a new Windows DC creates them. The
following are some of the objects or attributes in the LDIFDE dump that
are not essential DIT
elements:

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>All objects under
CN=DisplaySpecifiers,CN=Configuration

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>All objects under CN=Public Key
Services,CN=Services,CN=Configuration

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Attributes whenCreated , whenChanged,
uSNChanged,dSCorePropagationData,uSNCreated in all objects because they
are all created in run
time.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The references to ForestDnsZones and DomainDnsZones
in all
objects.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Objects under CN=Default Domain Policy,CN=System ,
CN=Policies,CN=System.

 

  We should notice that most of the essential objects are in Domain and
Configuration NC.<span style="mso-spacerun: yes">  </span>For schema NC,
only the following two objects or containers are mandatory for a
functioning initial
DC:

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>CN=Aggregate,CN=Schema,CN=Configuration

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>CN=Schema,CN=Configuration

 

  For your reference, the complete lists of the objects removed
(Domain-NC-Removed.txt <span style="mso-spacerun: yes"> </span>&
Configuration-NC-Remove.txt ) are attached with the blog.

 

  Understanding what AD objects or attributes are essential for an
initial DC in a forest could be very important for a third party to
implement interoperable solution with Windows Active Directory and can
also help administrators

understand the impact of changing objects or attributes in Active
Directory.

[Windows2008R2\_Minimum\_DIT.zip](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/09/98/14/23/Windows2008R2_Minimum_DIT.zip)

</div>

</div>
