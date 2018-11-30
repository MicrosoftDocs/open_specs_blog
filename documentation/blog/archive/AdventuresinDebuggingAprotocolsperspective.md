<div id="page">

# Adventures in Debugging – A protocols perspective

[rguthrie-msft](https://social.msdn.microsoft.com/profile/rguthrie-msft)
5/15/2009 3:31:10 PM

-----

<div id="content">

Many times when looking at the details of a network protocol you find
yourself asking questions like:

· What is that function doing to my data under the covers?

· What if I could only see the unencrypted values? Then I could figure
out what the problem is.

Using the debugging tool Windbg and public symbols along with the
protocol documentation, one can quickly answer these questions across a
broad range of scenarios. We are going to focus on the scenario of
needing to view the parameters passed to a RPC function call to the
security account manager (SAM). You can also view an online presentation
related to this subject on channel 9
at:

<https://channel9.msdn.com/posts/Will+Gregg/Troubleshooting-Issues-with-Windows/>

The scenario we are going to explore is using the debugger to evaluate a
RPC call to
[SamrLookupNamesInDomain](http://msdn.microsoft.com/en-us/library/cc245712\(PROT.13\).aspx).
This method is defined in MS-SAMR to take a security principal as input
and return the Relative ID (RID) for that principal. Let us assume that
the parameters for this call are encrypted over the network using RPC
transport layer security and we want to ensure we are passing the
correct parameters and receiving the correct response. We can use
[Windbg](http://www.microsoft.com/whdc/DevTools/Debugging/default.mspx)
with public symbols to evaluate our implementation against this
specification. I should note that some of the commands and procedures
for Windbg are covered at a fairly high level as you can find more
detailed information in the debugger help documentation. Let’s now work
through an example:

The first step is to attach to the process in which the SAM resides. We
can use tlist.exe utility found in the debugger installation directory
to determine which process the Security Account Manager resides in.

C:\\Debuggers\>**tlist -s**

0 System Process

4 System

556 smss.exe

688 csrss.exe

740 wininit.exe

784 services.exe

**800 lsass.exe Svcs: KeyIso,Netlogon,ProtectedStorage,SamSs**

808 lsm.exe

952 svchost.exe Svcs: DcomLaunch,PlugPlay

1008 nvvsvc.exe Svcs: nvsvc

1036 svchost.exe Svcs: RpcSs

1088 svchost.exe Svcs: WinDefend

1136 svchost.exe Svcs: Audiosrv,Dhcp,Eventlog,lmhosts,wscsvc

We then attach Windbg to the process and set our symbol path to the
public symbol server with a path similar to:

srv\*c:\\mysymbols\*http://msdl.microsoft.com/download/symbols;

Once attached, we need to find our breakpoint. To do this you need to
ensure you have symbols loaded. When I am debugging I like to start by
first ensuring that my symbols are good so I will first see what is
loaded using the lm command.

0:000\> **lm**

start end module name

008c0000 00925000 msvcp60 (deferred)

01000000 01006000 lsass (**deferred)**

03240000 03505000 xpsp2res (deferred)

10000000 100c8000 ws03res (deferred)

4a630000 4a704000 oakley (deferred)

4ab70000 4ac39000 lsasrv (deferred)

4b180000 4b284000 esent (deferred)

58f40000 58f59000 wlbsctrl (deferred)

5b890000 5b91d000 vssapi (deferred)

5d9f0000 5d9f9000 rassfm (deferred)

5deb0000 5deb7000 pwdssp (deferred)

5ee00000 5ef6e000 tttracewriter (deferred)

5f1d0000 5f259000 ntdsmsg (deferred)

5f270000 5f2ca000 hnetcfg (deferred)

63a80000 63ab8000 kdcsvc (deferred)

You will notice that the symbols are not currently loaded so let’s go
ahead next and do a forced reload.

0:000\> .reload /f

Once this operation completes, rerun the lm command and you should see
the debugger display a symbol path for the loaded modules.

0:000\> lm

start end module name

008c0000 00925000 msvcp60 **(pdb symbols)
c:\\pubsymbols\\msvcp60.pdb\\16717F2EEBC34947B2469E70CCD3F8F22\\msvcp60.pdb**

01000000 01006000 lsass **(pdb symbols)
c:\\pubsymbols\\lsass.pdb\\3E7FFFBA1\\lsass.pdb**

Now that we have symbols loaded we can search for our method we want to
set a breakpoint on. There are several ways to do this but I find the
easiest way is to do a wildcard search of the loaded symbols as follows:

0:000\> **x \*\!\*LookupNamesInDomain\***

4ab92ecf lsasrv\!SamrLookupNamesInDomain = \<no type information\>

4ab718a8 lsasrv\!\_imp\_\_SamrLookupNamesInDomain = \<no type
information\>

5d9f4dac rassfm\!SamrLookupNamesInDomain = \<no type information\>

5d9f10f8 rassfm\!\_imp\_\_SamrLookupNamesInDomain = \<no type
information\>

71c4ccf7 netapi32\!SamLookupNamesInDomain = \<no type information\>

71c90080 netapi32\!\_imp\_\_SamLookupNamesInDomain = \<no type
information\>

71c4e430 netapi32\!\_imp\_load\_\_SamLookupNamesInDomain = \<no type
information\>

71ce8230 kerberos\!SamrLookupNamesInDomain = \<no type information\>

71ced044 kerberos\!\_imp\_\_SamrLookupNamesInDomain = \<no type
information\>

71ce8221 kerberos\!\_imp\_load\_\_SamrLookupNamesInDomain = \<no type
information\>

7222ff28 ntdsa\!SamLookupNamesInDomain = \<no type information\>

720e1ad4 ntdsa\!\_imp\_\_SamLookupNamesInDomain = \<no type
information\>

**741d7a38 samsrv\!SamrLookupNamesInDomain = \<no type information\>**

741df09c samsrv\!SampLookupNamesInDomainGuid = \<no type information\>

77fc259c advapi32\!FnSamLookupNamesInDomain = \<no type information\>

7e0214e5 samlib\!SamLookupNamesInDomain = \<no type information\>

7e022732 samlib\!SamrLookupNamesInDomain = \<no type information\>

The command above can be read as look in all modules for a function that
is like LookupNamesInDomain. I left the **Samr** portion of the method
name off because I wanted to see all the entries in all modules that are
close to the function I am looking for. This is useful when you want to
try and find a private method, or just see what is in a given module(s).
Since the document I am looking at is dealing with SAMR, I will set a
breakpoint on this function as such:

0:000\> **bp samsrv\!SamrLookupNamesInDomain**

0:000\> **bl**

0 e 741d7a38 0001 (0001) 0:\*\*\*\* samsrv\!SamrLookupNamesInDomain

Now that our breakpoint is set we type **** **g** (for go), and make
this call from our implementation. Once our breakpoint is hit, the first
task is to examine the stack and registers as follows:

**0:001\> kb**

ChildEBP RetAddr Args to Child

**00daf91c 77c80193 0009c970 00000001 03040790
samsrv\!SamrLookupNamesInDomain**

00daf944 77ce33e1 741d7a38 00dafb28 00000005 rpcrt4\!Invoke+0x30

00dafd44 77ce35c4 00000000 00000000 00db3738 rpcrt4\!NdrStubCall2+0x299

00dafd60 77c7ff7a 00db3738 005ac750 00db3738 rpcrt4\!NdrServerCall2+0x19

00dafd94 77c8042d 741d76ef 00db3738 00dafe34
rpcrt4\!DispatchToStubInCNoAvrf+0x38

00dafde8 77c80353 00000011 00000000 7422d5a8
rpcrt4\!RPC\_INTERFACE::DispatchToStubWorker+0x11f

00dafe0c 77c68e0d 00db3738 00000000 7422d5a8
rpcrt4\!RPC\_INTERFACE::DispatchToStub+0xa3

00dafe40 77c68cb3 00db36f8 00000052 00db36c0
rpcrt4\!OSF\_SCALL::DispatchHelper+0x149

00dafe54 77c68c2b 00000000 00000011 00dbcc88
rpcrt4\!OSF\_SCALL::DispatchRPCCall+0x10d

00dafe84 77c68b5e 00dbcc88 0300006a 00000011
rpcrt4\!OSF\_SCALL::ProcessReceivedPDU+0x57f

00dafea4 77c6e8db 00000000 0000006a 00000000
rpcrt4\!OSF\_SCALL::BeginRpcCall+0x194

00daff04 77c6e7b4 00000000 00dbcc88 0000006a
rpcrt4\!OSF\_SCONNECTION::ProcessReceiveComplete+0x435

00daff18 77c7b799 005bd8e8 0000000c 00000000
rpcrt4\!ProcessConnectionServerReceivedEvent+0x21

00daff74 00000000 00db26dc 00960180 00000000
rpcrt4\!LOADABLE\_TRANSPORT::ProcessIOEvents+0x1b8

**0:001\> r**

eax=741d7a38 ebx=00dafd5c ecx=00000000 edx=00000008 esi=00dafb24
edi=00daf920

eip=741d7a38 esp=00daf920 ebp=00daf944 iopl=0 nv up ei pl nz na pe nc

cs=001b ss=0023 ds=0023 es=0023 fs=003b gs=0000 efl=00000206

samsrv\!SamrLookupNamesInDomain:

741d7a38 8bff mov edi,edi

Now that we have hit our breakpoint examining the stack using kb, we
have the first three parameters passed into the function. Looking at the
documentation these parameters are:

long SamrLookupNamesInDomain(

\[in\] SAMPR\_HANDLE DomainHandle,

\[in, range(0,1000)\] unsigned long Count,

\[in, size\_is(1000), length\_is(Count)\] RPC\_UNICODE\_STRING
Names\[\*\],

\[out\] PSAMPR\_ULONG\_ARRAY RelativeIds,

\[out\] PSAMPR\_ULONG\_ARRAY Use

);

So our first three parameters are:

DomainHandle = 0009c970

Count = 00000001

Names = 03040790

Executing a **dd** on the Names value which is a pointer to an array of
Unicode strings of length 1 (from our Count parameter) we get the
following:

0:001\> **dd 03040790**

03040790 001c001a 0301ae58 00000000 00000000

030407a0 00000000 00000000 00000000 00000000

030407b0 00000000 00000000 00000000 00000000

030407c0 00000000 00000000 00000000 00000000

030407d0 00000000 00000000 00000000 00000000

030407e0 00000000 00000000 00000000 00000000

030407f0 00000000 00000000 00000000 00000000

03040800 00000000 00000000 00000000 00000000

The first 2 bytes (001c) is the maximum length of the Unicode string,
and the second 2 bytes (001a) is the length. The next 4 bytes (0301ae58)
is a pointer to the actual buffer which is what we want to look at so we
do a du command on that pointer to observer the names of the security
principals this call is trying to resolve.

0:001\> **du 0301ae58**

0301ae58 "Administrator"

So now we know that we are trying to resolve the security principal
Administrator. We can also find our two out parameters by looking at the
stack using the **dds** command

0:001\> **dds esp**

00daf920 77c80193 rpcrt4\!Invoke+0x30

00daf924 0009c970

00daf928 00000001

00daf92c 03040790

00daf930 **00dafcd8 \<-- Our relativeIds out parameter**

00daf934 00dafce8

00daf938 02060206

00daf93c 00000005

00daf940 00db3738

00daf944 00dafd44

00daf948 77ce33e1 rpcrt4\!NdrStubCall2+0x299

00daf94c 741d7a38 samsrv\!SamrLookupNamesInDomain

00daf950 00dafb28

00daf954 00000005

00daf958 005ac750

00daf95c 00000000

00daf960 000006bb

00daf964 00de81b8

00daf968 00000000

00daf96c 00000000

00daf970 00000000

00daf974 00000000

00daf978 00000000

00daf97c 00000000

00daf980 00000000

00daf984 00db3738

00daf988 00dbccf2

00daf98c 00dbcca0

00daf990 00dbccf2

00daf994 00dbccc4

00daf998 00000000

00daf99c 00000008

We then do a **dd** on the value and observe that it is a null pointer.
We will come back to this value later.

0:001\> **dd 00dafcd8 l2**

00dafcd8 00000000 00000000

The next thing we need to do is to find a breakpoint that we can observe
the return values prior to placing the data on the wire. An easy way to
do this is to unassemble the function and find a breakpoint just a few
instructions prior to function exit. We do this with the **uf** command.

0:001\> **uf samsrv\!SamrLookupNamesInDomain**

samsrv\!SamrLookupNamesInDomain:

741d7a38 8bff mov edi,edi

741d7a3a 55 push ebp

741d7a3b 8bec mov ebp,esp

741d7a3d 83ec48 sub esp,48h **\<-- Allocation of space for local
variables**

741d7a40 53 push ebx

741d7a41 56 push esi

741d7a42 57 push edi

741d7a43 ff7508 push dword ptr \[ebp+8\]

741d7a46 33db xor ebx,ebx

741d7a48 885de8 mov byte ptr \[ebp-18h\],bl

741d7a4b e84cd7ffff call samsrv\!SampClientRevisionFromHandle (741d519c)

741d7a50 53 push ebx

741d7a51 6a12 push 12h

741d7a53 6a01 push 1

741d7a55 8945e4 mov dword ptr \[ebp-1Ch\],eax

741d7a58 e8f4d7ffff call samsrv\!SampTraceEvent (741d5251)

741d7a5d 8b7518 mov esi,dword ptr \[ebp+18h\]

741d7a60 3bf3 cmp esi,ebx

741d7a62 0f8420440000 je samsrv\!SamrLookupNamesInDomain+0x467
(741dbe88)

…..

…..

…..

…..

samsrv\!SamrLookupNamesInDomain+0x46e:

741d7ce2 ff75fc push dword ptr \[ebp-4\]

741d7ce5 6a12 push 12h

741d7ce7 6a02 push 2

741d7ce9 e863d5ffff call samsrv\!SampTraceEvent (741d5251)

741d7cee 8b45fc mov eax,dword ptr \[ebp-4\] **\<-- move return address
into eax**

741d7cf1 5f pop edi

741d7cf2 5e pop esi

741d7cf3 5b pop ebx

741d7cf4 c9 leave

741d7cf5 c21400 ret 14h ßreturn from function

…..

…..

…..

…..

samsrv\!SamrLookupNamesInDomain+0x229:

741e3c13 8b4604 mov eax,dword ptr \[esi+4\]

741e3c16 c7040708000000 mov dword ptr \[edi+eax\],8

741e3c1d 8b4514 mov eax,dword ptr \[ebp+14h\]

741e3c20 8b4004 mov eax,dword ptr \[eax+4\]

741e3c23 891c07 mov dword ptr \[edi+eax\],ebx

741e3c26 895dfc mov dword ptr \[ebp-4\],ebx

741e3c29 e95d40ffff jmp samsrv\!SamrLookupNamesInDomain+0x3f0 (741d7c8b)

samsrv\!SamrLookupNamesInDomain+0x2a3:

741e3c2e 8b4604 mov eax,dword ptr \[esi+4\]

741e3c31 c7040702000000 mov dword ptr \[edi+eax\],2

741e3c38 e91040ffff jmp samsrv\!SamrLookupNamesInDomain+0x2ad (741d7c4d)

samsrv\!SamrLookupNamesInDomain+0x3e3:

741e3c3d 817dfc620000c0 cmp dword ptr \[ebp-4\],0C0000062h

741e3c44 0f844140ffff je samsrv\!SamrLookupNamesInDomain+0x3f0
(741d7c8b)

samsrv\!SamrLookupNamesInDomain+0x3f0:

741e3c4a e907ffffff jmp samsrv\!SamrLookupNamesInDomain+0x12a (741e3b56)

Above I have pulled out some highlights of the disassembly of this
function. The first is the function entry and allocation of local
variable storage on the stack. Depending on what instructions are
executed at the return we may need to go back and look at what values
are in the local variables. The next portion shows a ret instruction. I
found this by starting at the bottom of the unassembly and working my
way up. If you look at the assembly after this you will realize that
those instructions are mostly logging, cleanup and error handling code.
So we will set a breakpoint at 741d7cee as that is where we start to
cleanup our stack. Looking back at the function prototype we know that
the parameter we are interested in is the out parameter RelativeIds.
Once we break in on our new breakpoint, we can look for this parameter
on the stack and observe its values.

0:001\> **kb**

ChildEBP RetAddr Args to Child

00daf91c 77c80193 0009c970 00000001 03040798
samsrv\!SamrLookupNamesInDomain+0x475

00daf944 77ce33e1 741d7a38 00dafb28 00000005 rpcrt4\!Invoke+0x30

00dafd44 77ce35c4 00000000 00000000 00db3738 rpcrt4\!NdrStubCall2+0x299

00dafd60 77c7ff7a 00db3738 005ac750 00db3738 rpcrt4\!NdrServerCall2+0x19

00dafd94 77c8042d 741d76ef 00db3738 00dafe34
rpcrt4\!DispatchToStubInCNoAvrf+0x38

00dafde8 77c80353 00000011 00000000 7422d5a8
rpcrt4\!RPC\_INTERFACE::DispatchToStubWorker+0x11f

00dafe0c 77c68e0d 00db3738 00000000 7422d5a8
rpcrt4\!RPC\_INTERFACE::DispatchToStub+0xa3

00dafe40 77c68cb3 00db36f8 00000052 00db36c0
rpcrt4\!OSF\_SCALL::DispatchHelper+0x149

00dafe54 77c68c2b 00000000 00000011 00dbcc88
rpcrt4\!OSF\_SCALL::DispatchRPCCall+0x10d

00dafe84 77c68b5e 00dbcc88 0300006a 00000011
rpcrt4\!OSF\_SCALL::ProcessReceivedPDU+0x57f

00dafea4 77c6e8db 00000000 0000006a 00000000
rpcrt4\!OSF\_SCALL::BeginRpcCall+0x194

00daff04 77c6e7b4 00000000 00dbcc88 0000006a
rpcrt4\!OSF\_SCONNECTION::ProcessReceiveComplete+0x435

00daff18 77c7b799 005bd8e8 0000000c 00000000
rpcrt4\!ProcessConnectionServerReceivedEvent+0x21

00daff74 00000000 00db26dc 00960180 00000000
rpcrt4\!LOADABLE\_TRANSPORT::ProcessIOEvents+0x1b8

0:001\> **r**

eax=00000000 ebx=00000000 ecx=7422d38c edx=00daf918 esi=00dafce8
edi=00000001

eip=741d7ce9 esp=00daf8bc ebp=00daf91c iopl=0 nv up ei pl nz na po nc

cs=001b ss=0023 ds=0023 es=0023 fs=003b gs=0000 efl=00000202

samsrv\!SamrLookupNamesInDomain+0x475:

741d7ce9 e863d5ffff call samsrv\!SampTraceEvent (741d5251)

Notice that the current value of eip is our breakpoint. Now let’s go
examine the stack and see if we can find the return value.

0:001\> **dds esp**

00daf8bc 00000002

00daf8c0 00000012

00daf8c4 00000000

00daf8c8 00daf920

00daf8cc 00dafb24

00daf8d0 00dafd5c

00daf8d4 00000001

00daf8d8 03040790

00daf8dc 741d339c samsrv\!\`string'+0x340

00daf8e0 00000001

00daf8e4 000003e8

00daf8e8 00daf984

00daf8ec 00dafb30

00daf8f0 00000000

00daf8f4 00daf91c

00daf8f8 77c774d6 rpcrt4\!NdrConformantVaryingArrayUnmarshall+0x7d

00daf8fc 00000001

00daf900 00000003

00daf904 5ee13000 tttracewriter\!Initialize+0x6c00

00daf908 00000024

00daf90c 00000001

00daf910 00000001

00daf914 741d336e samsrv\!\`string'+0x312

00daf918 00000000

**00daf91c 00daf944 \<--** **base pointer**

00daf920 **77c80193** rpcrt4\!Invoke+0x30 **\<-- return address**

00daf924 0009c970

00daf928 00000001

00daf92c 03040798

00daf930 **00dafcd8** **\<--RelativeIds parameter**

00daf934 00000000

00daf938 02060206

So now we look at our relativeIds parameter and observe if the value has
changed - and it has.

0:001\> **dd 00dafcd8 l2**

00dafcd8 00000001 **0301a9c8 \<-- the buffer for our relativeIds**

0:001\> **dd 0301a9c8 l1**

0301a9c8 **000001f4**

Dumping the buffer shows us a value of 1f4 which in decimal is 500.

0:001\> **?1f4**

Evaluate expression: 500 = 000001f4

So our relativeId for Administrator is 500. Since this is a known
relativeId, this is valid and we know we are getting proper values
returned for our implementation. As you can see, using Windbg to aid in
protocol development can greatly increase your understanding of what the
behavior of an api can be.

</div>

</div>
