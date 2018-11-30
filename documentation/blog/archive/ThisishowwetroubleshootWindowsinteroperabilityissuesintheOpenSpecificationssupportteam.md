<div id="page">

# This is how we troubleshoot Windows interoperability issues in the Open Specifications support team

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
6/28/2011 12:39:00
PM

-----

<div id="content">

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Hi
y’all, Sebastian from Texas
here\!</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">I’ve
been at the File Sharing
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">Plugfest</span>](http://www.microsoft.com/openspecifications/en/us/applied-interoperability/testing/plugfests-and-events/default.aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
last
week.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">This
time around I’ve had the chance to give a presentation on how we use the
different sets of tools that we have at hand (both internal and external
ones) when working on support cases related to the open specifications
world.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Attendees
showed good interest on the presentation, so I decided to write a blog
about
it.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Even
though the original presentation was more focused on SMB, this
adaptation applies for any of the protocols we
support.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">In
this post I will talk about the “What”. I will later work on one or more
posts about the
“How”.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Here
we
go\!</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

**<span style="font-family: Calibri; font-size: medium;" data-face="Calibri">Introduction</span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">When
working with wire protocols, a network capture is almost always involved
in the troubleshooting process; however, sometimes we need more tools to
look at the problem from a different
perspective.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Memory
dumps, debugging, logging and tracing are often key tools used in
finding the root cause of a
problem.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: medium;">**<span style="font-family: Calibri;" data-face="Calibri">Network
Monitor
3.X</span>**</span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Network
captures are often enough to find protocol
issues.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">For
those that are not familiar with the tool, Network Monitor has full
parsers that are based on the
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">MCPP</span>](http://msdn.microsoft.com/en-us/library/cc216513\(v=prot.10\).aspx)<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">
and
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">WSPP</span>](http://msdn.microsoft.com/en-us/library/cc964399\(v=prot.13\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
documents as well as RFCs and other normative
documentation.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">For
certain scenarios, there are “Experts” that expand the tool’s
functionality:</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">Nmdecrypt</span>](http://nmdecrypt.codeplex.com/)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
(SSL
decryption)</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">Top
Users</span>](http://nmtopusers.codeplex.com/)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
(shows the heavy talkers in a
trace)</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">Others…
(Experts and parsers can be downloaded from
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">www.codeplex.com</span>](http://www.codeplex.com/)<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">
and are open GPL, Experts SDK available at:
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">http://nmexperts.codeplex.com/</span>](http://nmexperts.codeplex.com/)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
)</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: medium;">**<span style="font-family: Calibri;" data-face="Calibri">Debugging
Tools for
Windows</span>**</span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">There
are several tools to debug in Windows but,
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">Windbg</span>](http://msdn.microsoft.com/en-us/library/ff561300\(v=vs.85\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
is the most common debugger in use for customer
issues.</span></span>

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">Windbg
is a GUI debugging tool that counts with many features
including:</span></span>

<span style="font-size: small;" size="3">–</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Ability
to do live kernel/user level and local/remote debugging</span></span>

<span style="font-size: small;" size="3">–</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Ability
to load post mortem memory dumps</span></span>

<span style="font-size: small;" size="3">–</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Ability
to work with Time Travel Tracing dumps</span></span>

<span style="font-size: small;" size="3">–</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Comes
in 32 & 64-bit versions</span></span>

<span style="font-size: small;" size="3">–</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Can
make use of the Microsoft web symbols server:
**SRV\*DownstreamStore\*http://msdl.microsoft.com/download/symbols**</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">For
an overview of the tool and to download the installable packet, click
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">here</span>](http://msdn.microsoft.com/en-us/library/ff561300\(v=vs.85\).aspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

**<span style="font-family: Calibri; font-size: medium;" data-face="Calibri">Time
Travel
Tracing</span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
is Time Travel
Tracing?</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Time
Travel Tracing (also known as iDNA tracing) consists of two
elements:</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">an
utility to capture TTT trace (available to
everyone)</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">an
extension to Windbg that allows to load the trace for analysis
(available only
internally)</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
novelty of this tool is that when the trace is loaded into WinDbg, it
allows the user to move back and forth in the process execution
time.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Thanks
to this capability, it helps us debug the Windows interoperability
behaviors without the need to reproduce the problem or remote live
debugging.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">As
of now, the tool only allows to capture user mode
processes.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: medium;">**<span style="font-family: Calibri;" data-face="Calibri">ETW
Tracing</span>**</span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
is ETW
Tracing?</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">It’s
a tracing facility widely used by Windows OS that is available in user
and kernel mode
components.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
event provider is registered with ETW using unique GUID and can be
dynamically enabled /
disabled.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">These
traces can even be captured with Perfmon however the results have to be
sent to us for analysis since the decoding of a log file requires
private TMF
files.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: medium;">**<span style="font-family: Calibri;" data-face="Calibri">Windows
Resource
Kit</span>**</span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">The
Resource Kit is a set of tools and documentation available for
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">download</span>](http://www.microsoft.com/download/en/confirmation.aspx?displaylang=en&id=17657)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">These
tools can provide lots of insight on what the OS is doing and how the
environment is
configured.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">These
tools are standalone (no installation needed) and are mostly command
window
based.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">We
generally request logs and outputs from these tools in order to have the
most information possible out of the Windows environment we are working
with.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">As
these tools are public, anyone troubleshooting a Windows issue can make
use of
them.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
ones we use the most
are:</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">Klist</span>:
Lists the active Kerberos tickets in the
cache</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">SrvInfo</span>:
Provides loads of information about the OS and its running processes +
configuration.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">Oh</span>:
Shows open
handles.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">SrvCheck</span>:
Shows information about the shares and their
permissions.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: medium;">**<span style="font-family: Calibri;" data-face="Calibri">Sysinternals
Utilities</span>**</span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">These
tools were created (at least most of them) by
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">Mark
Russinovich</span>](http://www.microsoft.com/presspass/exec/techfellow/Russinovich/default.mspx)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">They
can be downloaded for free from
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">http://</span>](http://technet.microsoft.com/en-us/sysinternals)[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">technet.microsoft.com/en-us/sysinternals</span>](http://technet.microsoft.com/en-us/sysinternals)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">What
makes these tools great is that they provided a humongous amount of
information about IOs, networks, Processes, Services, Registry,
etc.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">In
their vast majority they are standalone tools so they do not need
installation.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
most common tools we use
are:</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">Process
Monitor</span>: It allows users to track IO and Registry activity from
the processes perspective. Can also track the activity that happens at
the boot
process</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">Process
Explorer</span>: It’s a task manager replacement tool that provides
loads of information about each process. It can even drill down to the
stack level and provide insight about each running
thread.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">DiskMon</span>:
Provides information about disk activity in great
detail.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">-</span>         
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"><span style="text-decoration: underline;">AccessChk</span>:
Quick way to understand what types of access a user or group has over a
specific
resource</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> I
truly hope this introduction enables you to have a better set of tools
to work both on your own and with us when troubleshooting
interoperability
issues.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">In
upcoming posts, I will provide more information on how to use these
tools to get the best out of
them.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">Until
next
time\!</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

</div>

</div>
