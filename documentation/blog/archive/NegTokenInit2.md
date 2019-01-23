<div id="page">

# NegTokenInit2

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
7/6/2009 4:51:00 PM

-----

<div id="content">

**J**ust when you think that you have figured out SPNEGO, boom\!
NegTokenInit2 hits you. At least, that was the case with yours truly.

NegTokenInit2 is a part of extension to SPNEGO introduced by Microsoft.
SPNEGO stands for Simple and Protected Generic Security Service
Application Program Interface Negotiation Mechanism. The discussion in
this blog assumes familiarity with SPNEGO. Interested readers can find
information about SPNEGO in RFC 4178. Microsoft’s extension to SPNEGO is
documented in
[\[MS-SPNG\]](http://msdn.microsoft.com/en-us/library/cc247021\(PROT.13\).aspx).

SPNEGO is transported only when encapsulated in an application protocol.
As such, we will use Server Message Block (SMB) protocol as application
protocol for examples in this blog. The following example does not delve
into the details of SMB protocol. I have simplified the discussion based
on what we see in the network protocol analyzer. Readers who are
interested in the details of SMB protocol can find them in
[\[MS-SMB\]](http://msdn.microsoft.com/en-us/library/cc246231\(PROT.13\).aspx)
and
[\[MS-SMB2\]](http://msdn.microsoft.com/en-us/library/cc246482\(PROT.13\).aspx)

Before a client and a server can use SMB/SMB2, they need to determine
the version of SMB/SMB2 that both can use. The client, as the first
message, sends a negotiate request message to the server. In the
negotiate request message, a list of SMB versions is included that the
client supports.

  Frame: Number = 21, Captured Frame Length = 202, MediaType =
ETHERNET  
\+ Ethernet: Etype = Internet IP
(IPv4),DestinationAddress:\[00-0F-FE-9A-2C-91\],SourceAddress:\[00-1E-4F-F3-4B-55\]  
\+ Ipv4: Src = 65.53.76.218, Dest = 65.53.73.99, Next Protocol = TCP,
Packet ID = 9773, Total IP Length = 188  
\+ Tcp: Flags=...AP..., SrcPort=54094, DstPort=NETBIOS Session
Service(139), PayloadLen=148, Seq=2774588991 - 2774589139,
Ack=4119913195, Win=513 (scale factor 0x8) = 131328  
\+ Nbtss: SESSION MESSAGE, Length =144  
\- SMB: C; Negotiate, Dialect = PC NETWORK PROGRAM 1.0, LANMAN1.0,
Windows for Workgroups 3.1a, LM1.2X002, LANMAN2.1, NT LM 0.12, SMB
2.002  
    Protocol: SMB  
    Command: Negotiate 114(0x72)  
  + NTStatus: 0x0, Facility = FACILITY\_SYSTEM, Severity =
STATUS\_SEVERITY\_SUCCESS, Code = (0) STATUS\_SUCCESS  
  + SMBHeader: Command, TID: 0xFFFF, PID: 0xFEFF, UID: 0x0000, MID:
0x0000  
  - CNegotiate:  
     WordCount: 0 (0x0)  
     ByteCount: 109 (0x6D)  
   + Dialect: PC NETWORK PROGRAM 1.0  
   + Dialect: LANMAN1.0  
   + Dialect: Windows for Workgroups 3.1a  
   + Dialect: LM1.2X002  
   + Dialect: LANMAN2.1  
   + Dialect: NT LM 0.12  
   + Dialect: SMB 2.002

The server responds with a negotiate response message that contains the
preferred version of SMB. The server also includes a security blob in
the negotiate response message. This security blob contains SPNEGO
NegTokenInit2 message. Here is what a NegTokenInit2 message looks like:

  Frame: Number = 23, Captured Frame Length = 294, MediaType =
ETHERNET  
\+ Ethernet: Etype = Internet IP
(IPv4),DestinationAddress:\[00-1E-4F-F3-4B-55\],SourceAddress:\[00-0F-FE-9A-2C-91\]  
\+ Ipv4: Src = 65.53.73.99, Dest = 65.53.76.218, Next Protocol = TCP,
Packet ID = 27970, Total IP Length = 280  
\+ Tcp: Flags=...AP..., SrcPort=NETBIOS Session Service(139),
DstPort=54094, PayloadLen=240, Seq=4119913195 - 4119913435,
Ack=2774589139, Win=256 (scale factor 0x8) = 65536  
\+ Nbtss: SESSION MESSAGE, Length =236  
\- SMB2: R  NEGOTIATE (0x0),
GUID={673BA0F4-D52B-42B3-4841-AD794E6E8177}, Mid = 0  
    SMBIdentifier: SMB  
  + SMB2Header: R NEGOTIATE (0x0)  
  - RNegotiate:  
     StructureSize: 65 (0x41)  
   + SecurityMode: 1 (0x1)  
     DialectRevision: (0x202) - SMB 2.002 dialect revision number.  
     Reserved: 0 (0x0)  
     ServerGuid: {673BA0F4-D52B-42B3-4841-AD794E6E8177}  
   + Capabilities: 0x1  
     MaxTransactSize: 65536 (0x10000)  
     MaxReadSize: 65536 (0x10000)  
     MaxWriteSize: 65536 (0x10000)  
     SystemTime: 03/30/2009, 04:47:22 PM  
     SystemStartTime: 03/27/2009, 11:26:31 PM  
     SecurityBufferOffset: 128 (0x80)  
     SecurityBufferLength: 108 (0x6C)  
     Reserved2: 541936672 (0x204D4C20)  
   - securityBlob:  
    - GSSAPI:  
     - InitialContextToken:  
      + ApplicationHeader:  
      + ThisMech: SpnegoToken (1.3.6.1.5.5.2)  
      - InnerContextToken: 0x1  
       - SpnegoToken: 0x1  
        + ChoiceTag:  
        - NegTokenInit:  
         + SequenceHeader:  
         + Tag0:  
         - MechTypes: Prefer MsKerberosToken (1.2.840.48018.1.2.2)  
          + SequenceHeader:  
          + MechType: MsKerberosToken (1.2.840.48018.1.2.2)  
          + MechType: KerberosToken (1.2.840.113554.1.2.2)  
          + MechType: UnknownOidExtension (1.2.840.113554.1.2.2.3)  
          + MechType: NLMP (1.3.6.1.4.1.311.2.2.10)  
         + Tag3:  
         - NegHints:  
          + SequenceHeader:  
          + Tag0:  
          + hintName: not\_defined\_in\_RFC4178@please\_ignore

The server has selected SMB 2.002 from the list sent by the client as
the version of SMB to use for subsequent message exchange. Also there is
a security blob in the response that contains NegTokenInit2. The parser
displays it as NegTokenInit. In Tag0, the server lists the
authentication and security mechanisms that it supports.

Tag3 is an interesting one. As you can see, hintName is
“<not_defined_in_RFC4178@please_ignore>”. In Windows 2003 and earlier,
this used to be service principle name (SPN). For details, please see
[\[MS-SPNG\]](http://msdn.microsoft.com/en-us/library/cc247021\(PROT.13\).aspx)

Now that we know the whereabouts of NegTokenInit2, what is it used for?
One obvious use could be to select one of the supported mechanisms of
the server and use it in NegTokenInit. This also seem to be the
intention of the designer(s) of this message. In Appendix A, section
\<10\> Section 3.3.5.1 of
[\[MS-SPNG\]](http://msdn.microsoft.com/en-us/library/cc247021\(PROT.13\).aspx),
it is stated that:

> This message is supported on Windows 2000, Windows XP, and Windows
> Server 2003, Windows 7, and Windows Server 2008 R2. Windows Vista does
> not generate this message.  
> All versions of Windows that support this message also accept the
> mechList message from the server as described. All versions of Windows
> that support this message treat the server name as an optional
> indicator of the domain of the server only and
> <span class="underline">use this message to select among possible sets
> of credentials</span>. There is no guarantee that any Windows-based
> client will use this value in any discernable way. For example, if the
> client and server support the same set of security mechanisms, the
> client will continue to produce the same list as if the server had
> provided no list at all. All Windows versions ignore the server name
> in the NegHint structure by default; it was used as a development aid
> during Windows 2000 and is only used when an undocumented local
> configuration value is set. The hint support was retained in
> anticipation of cross-forest scenarios, but it was never used.
> Therefore, the removal of the hint in Windows Vista. The Windows
> implementation where this message is supported never alters the target
> name that is expressed by the application protocol above SPNEGO.

The important point to note in this excerpt is that the client
implementation can safely ignore this message and send NegTokenInit as
if it does not know anything about the mechanisms that the server
supports.

On the flip side, this message can be used to shorten the message
exchange for authentication. I have yet to see an example of that but
here is how I think it can be achieved. Before proceeding, I would like
to clarify that this is my personal theory and Microsoft does not
endorse it in any way. Use it at your own risk.

RFC4178, in section 5, states that:

> If the mechanism selected by the negotiation does not support
> integrity protection, then no mechlistMIC token is used.
> 
> Otherwise, if the accepted mechanism is the most preferred mechanism
> of both the initiator and the acceptor, then the MIC token exchange,
> as described later in this section, is OPTIONAL. A mechanism is the
> acceptor's most preferred mechanism if there is no other mechanism
> that the acceptor would have preferred over the accepted mechanism had
> it been present in the mechanism list.
> 
> In all other cases, MIC tokens MUST be exchanged after the mechanism
> context is fully established.
> 
> . . .

By analyzing NegTokenInit2, a client can determine the most preferred
mechanism of the server. Even if the client supports a more advanced
mechanism, by making the most preferred method the same as server’s,
client and server can avoid the exchange of mechListMIC token.

To elaborate, let’s consider a hypothetical example. Assume that the
server supports mechanisms ProtocolB, ProtocolC and ProtocolD. The
server communicates the supported mechanisms through NegTokenInit2 by
sending a list of MechTypes:

1.  ProtocolB
2.  ProtocolC
3.  ProtocolD

According to this list, the most preferred mechanism for the server is
ProtocolB.

Let’s assume that the client supports ProtocolA, ProtocolB and
ProtocolC. ProtocolA is a more advanced protocol then protocolB. If the
client send the following list in NegTokenInit, ignoring NegTokenInit2:

1.  ProtocolA
2.  ProtocolB
3.  ProtocolC

the server will select ProtocolB. This will result in a situation where
the exchange of mechListMIC tokens will become necessary, since
ProtocolB is not client’s most preferred.

On the other hand, if the client analyzes NegTokenInit2 and instead
sends the following list in the NegTokenInit:

1.  ProtocolB
2.  ProtocolC

the server will still select ProtocolB but the exchange of mechListMIC
will be optional.

The protocol selected is same in both cases (ProtocolB) but using
NegTokenInit2 helps to avoid mechListMIC token exchange.

I used “client analyzes” pretty loosely in the discussion above. What
should happen is that NegTokenInit2 should be passed to
GSS\_Init\_sec\_context() on the client side. The implementation of
GSS\_Init\_sec\_context() can then make the decision as to completely
ignore NegTokenInit2 or use it as I have described above. More details
on GSS-API can be found in RFC2743.

</div>

</div>
