<div id="page">

# Reflecting on another successful Interoperability Lab event

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
12/4/2009 7:24:00
AM

-----

<div id="content">

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Engagement
with partners is an integral part of achieving interoperability with
Windows. In addition to helping users of the Microsoft Open Protocol
Specifications, our team participates in a regular basis in
</span>[<span style="LINE-HEIGHT: 120%; COLOR: blue; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">interoperability
labs</span>](http://www.microsoft.com/protocols/resources/plugfests-and-interop-labs/)<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">
dedicated to specific areas of focus of our
partners.</span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Sun
Microsystems is one our team’s most active partners. Recently, I had the
opportunity to take part in the Sun’s IO lab hosted in the
</span>[<span style="LINE-HEIGHT: 120%; COLOR: blue; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Microsoft
Enterprise Engineering
Center</span>](http://www.microsoft.com/windowsserver/evaluation/eec/default.mspx)<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">
(ECC). The event focused on testing Solaris implementation against
Windows in the areas of remote file sharing with extended security
support, and Microsoft Distributed File System. It was a great adventure
for me throughout the week. In this blog, I am reflecting on the event
and sharing my experiences and story as a supporting Escalation
Engineer.<span style="mso-spacerun: yes">  
</span></span>

<span style="FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">The
Sun IO event was held at the ECC from 10/26 to 10/30. Prior to the
event, the Protocol Engineering management worked closely together with
the ECC program management to plan and set up a testing environment
agreed upon with Sun. The environment included two forests, four domains
with different domain functional levels, and a myriad of clients. A team
of five attendees came from Sun primarily to test Solaris CIFS service
with extended security support, and standalone DFS server under
development. In the file sharing space, one key objective was to test
Kerberos PAC support with Solaris (client or server) against a matrix of
Windows clients and servers. Sun also planned to leverage the Microsoft
Protocol Test suites to test their current implementation of SMB, KILE,
PAC and DFS.
</span>

<span style="FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">The
team felt pleased with the excellent lab environment setup by the EEC as
it provides the flexibility to experiment various scenarios. Solaris
servers were installed either on physical machines or on virtual
machines. On the first day, after the morning meeting, Sun’s team even
requested a third forest with a specific functional level and Windows
service pack, and it was created. This forest will be used for the
Kerberos PAC
testing.</span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Following
the initial meeting, engineers from both Microsoft and Sun worked in
configuring the testing environment. Sun’s engineers joined their
machines to the lab network dedicated for the event and began compiling
their software. From our side, we helped troubleshooting network issues
and identifying the latest patches and hot fixes required to join
Solaris to Windows domains. Others worked on compiling MIT KDC on a
Solaris server. Later during the day, I began gathering more details
about initial questions raised by Sun’s engineers regarding SMB/CIFS.
The questions aimed at clarifying certain SMB/CIFS interoperability
aspects in Windows-based implementations compared to Linux, Solaris, or
Posix-based implementations. After some quick research and engagement
with Microsoft subject matter experts, we had very interesting
discussions on the topics of interest.<span style="mso-spacerun: yes"> 
</span></span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">The
next day, the testing activities were getting substantial. I was called
upon for consulting on a pass-through authentication issue. I decided to
collect and analyze the resulting network trace, and ETW trace. As we
resolved this scenario, and ensured the correct principal was initiating
the SMB session, we ran into an interesting question on SPNEGO whereby
the negotiation hint from the Solaris server would not contain a NTLM
OID. In such a scenario, if the optimistic mechanism did not succeed,
the Windows client would close the connection but this solely depends on
the error that occurs during the authentication
initiation.<span style="mso-spacerun: yes"> 
</span></span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">From
now on, till the end of Day 3, I worked with two of Sun’s engineers and
debugged a Kerberos PAC issue. After some brainstorming, we were all
under the impression that this issue would be challenging because the
Solaris server has to use exactly the same inputs and the same
cryptographic algorithm Windows KDC used to generate the PAC server
signature. The first phase of the investigation plan was to analyze
Windows source code. The second phase was to capture Time Travel Traces
(TTT) from the Windows KDC. It has also been critical to instrument the
Solaris code and trace some key material, and encryption types passed to
different calls during the server side of PAC verification. Invariably,
I used the data from the Solaris syslog, and ran live debugging with
Windows source attached. After a few hours of analysis, I asserted that
the MIT API that was being leveraged by the Solaris server was producing
the wrong key hash for the server signature. One of Sun’s engineers
conducted code analysis and confirmed this. We then referred to the
MS-PAC and MS-KILE documents and discussed the expected
logic.</span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">On
Day 4, following the meeting with the security team, I was assigned to
test LDAP interoperability of SASL between Windows Active Directory and
Solaris LDAP clients. I worked with another Test Suites engineer to test
various scenarios. We observed the Solaris bind security layer
negotiation behavior with Windows Server 2008 R2. We also confirmed that
the default Windows DC settings would allow LDAP operations to complete
successfully via SASL.
</span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">The
final day of my week was dedicated to wrap-up the issues that I had been
tracking throughout the week. The joint debugging and testing efforts
between the Sun and Microsoft teams have been very productive, and
raised a few interoperability issues from both sides.
</span>

<span style="LINE-HEIGHT: 120%; FONT-SIZE: 12pt; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-bidi-font-family: Calibri; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">I
was really impressed by the teamwork from Sun and Microsoft engineers
and the depth of the testing activities. I will be pleased to work with
them in another interoperability event in the future.</span>

</div>

</div>
