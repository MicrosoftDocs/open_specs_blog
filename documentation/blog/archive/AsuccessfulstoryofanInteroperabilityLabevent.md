<div id="page">

# A successful story of an Interoperability Lab event

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
10/14/2009 4:06:00 PM

-----

<div id="content">

<span style="mso-spacerun: yes"></span> 

     As the protocol documentation support team, we have the
responsibility of helping the users of our published Microsoft Open
Protocol Documentation achieve successful interoperability with
Windows.    There's more to interoperability than just good technical
documentation; engagement with partners is essential. 

   One of our team's most active partners is the Samba team, whose main
areas are remote file system and Active Directory.  Recently, I had the
chance to attend the second Samba IO Lab hosted in the [Microsoft
Enterprise Engineering
Center](http://www.microsoft.com/windowsserver/evaluation/eec/default.mspx)
(ECC), an event dedicated to testing the Samba implementation against
Windows.   It was an incredible experience for me to support the testing
and debugging activities during the event.   We achieved very positive
results at the end of week in a pretty dramatic fashion, as described in
[Andrew Bartlett's blog](http://people.samba.org/people/2009/10/05).  In
this blog, I would like to share my experiences and thoughts from the
perspective of a supporting Escalation Engineer.

   The Samba IO event was held in the ECC from 09/21 to 09/25.   Before
the event, the ECC already set up the testing environment that consists
of four domains with different domain functional levels.  Everyone in
the event felt that the lab environment was excellent.  A group of five
Samba developers arrived after attending another interoperability
event.  The Samba team was prepared to test protocols they implemented
or changed lately, including DRSR, LDAP with complete schema and AES
support in Netlogon.  The main focus was to have a Windows DC join to a
Samba domain that holds all FSMO roles and perform a complete
replication.  

   After the initial meeting, it was decided that I would work with
Tridge (Andrew Tridgell) on the main issue of domain join and
replication, while other Samba team members started working on protocol
testing using the Microsoft Protocol Documentation test suites.  At the
time, the AD Domain Service <span class="underline"></span> could not be
installed due to an internal error  when dcPromo was run to promote the
Windows  Server 2008 R2 to a DC in a Samba domain.  Our first approach
was to check log files generated in domain join and the DC promotion
processes and the very detailed logs produced by Samba.  We identified
that the problem happened somewhere after domain join, but before any
replication was attempted.    The error can be linked to many possible
root causes.   We tried several approaches associated with the error and
nothing worked.   We finally decided that we would capture a Time Travel
Trace and run live debugging with Windows source attached.  We also
captured ETW trace to assist in identifying the module for debugging.
 (Even though I used private symbols to assist in the debugging, any
developer can still [utilize the Windows debugger and public symbols, to
gain more
information](https://channel9.msdn.com/posts/Darryl/Troubleshooting-Windows-SMBSMB2-Issues/)
about the parameters passed into and returned from the interface
functions.)  In this way, I could run debugging to figure out the real
reason behind the behavior and Samba team could make changes based on
the information I provided and also continue their debugging and code
modification in parallel.  After a whole day of debugging , we finally
found out that the root cause of problem was that the RootDomainSID
returned from a Samba DC through LDAP query of attribute ncName uses
upper case letters (A to F) for the string representation of SID.  But
Windows only processes the string representation of SID in lower
case.    We were very excited, thinking that replication would start
and everything should work right away.   Just as Andrew documented in
his blog, the moment didn't come for another four days, with many traces
analyzed and more than ten problems identified and fixed.  This proved
that tracing through the source code for the logic at this detailed
level can be very useful for the debugging of interoperability
solutions.    

   After a  long week (\> 60 hours) of working side by side with Samba
team,  I was really impressed by the Samba team's professionalism,
passion, dedication, protocol knowledge,  programming  and debugging
skills and team work spirit and really enjoyed working with the entire
team.  Many times, they identified problems quickly and made correct
changes on the fly.  We shared many happy and frustrated moments and
everyone agreed that we had a very productive week.  Without the joint
debugging effort between the Samba and Microsoft teams, it would not be
possible to achieve so much while they were here.  Hopefully this can
become a model for the future [Interoperability lab
events](http://www.microsoft.com/protocols/resources/plugfests-and-interop-labs/)
for Samba and other partners.

   I am looking forward to working on the Samba IO event again in the
future\!  

</div>

</div>
