<div id="page">

# SMB Maximum Transmit Buffer Size and Performance Tuning

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
4/10/2009 11:55:00
AM

-----

<div id="content">

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Background<span style="mso-spacerun: yes"> 
</span></span></span>**

<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"><span style="mso-spacerun: yes"></span></span>

<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"><span style="mso-spacerun: yes"></span></span>

The performance of remote file operations, such as read/write, using SMB
protocol can be affected by the size of buffers allocated by servers and
clients.<span style="mso-spacerun: yes">  </span>The buffer size
determines the number of round trips needed to send a fixed amount of
data.<span style="mso-spacerun: yes">     </span>Every time when
requests and responses are sent between client and server, the amount of
time taken is equal to at least the latency between both sides, which
could be very significant in the case of Wide Area Network
(WAN).<span style="mso-spacerun: yes">      </span>Therefore it is
important to understand<span style="mso-spacerun: yes">   </span>how
Windows clients and servers select buffer size while you are working on
SMB performance monitoring and
tuning. <span style="mso-spacerun: yes"> </span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">MaxBufferSize
negotiation on SMB traffic<span style="mso-spacerun: yes"> 
</span></span></span>**

   1) SMB servers return a maximum SERVER buffer size
(MaxTransmotBufferSize) in the SMB negotiation response
(SMB\_COM\_NEGOTIATE server response). This size is not the size of the
complete network packet; it is just the SMB portion of the packet
starting from SMB header.

> \- Smb: R; Negotiate, Dialect is NT LM 0.12 (\#5),
> SpnegoNegTokenInit  
>     Protocol: SMB  
>     Command: Negotiate 114(0x72)  
>   + NTStatus: 0x0, Facility = FACILITY\_SYSTEM, Severity =
> STATUS\_SEVERITY\_SUCCESS, Code = (0) STATUS\_SUCCESS  
>   + SMBHeader: Response, TID: 0xFFFF, PID: 0xFEFF, UID: 0x0000, MID:
> 0x0000  
>   - RNegotiateDialectNTLM:  
>      WordCount: 17 (0x11)  
>      DialectIndex: 5 (0x5)  
>    + SecurityMode: 15 (0xF)  
>      MaxMpxCount: 50 (0x32)  
>      MaxCountVCs: 1 (0x1)  
>      **MaxTransmitBufferSize: 16644 (0x4104)**  
>      MaxRawSize: 65536 (0x10000)  
>      SessionKey: 0
(0x0)

  <span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2)<span style="FONT: 7pt &#39;Times New Roman&#39;">   </span></span></span>The
SMB client sends<span style="mso-spacerun: yes">  </span>a maximum
CLIENT buffer size <span style="mso-spacerun: yes"> </span>to the server
in<span style="mso-spacerun: yes">  </span>SMB Session Setup request
(SMB\_COM\_SESSION\_SETUPANDX request)
<span style="mso-spacerun: yes"> </span>telling the sever what the
maximum buffer size the client can support.

> \- Smb: C; Session Setup Andx, Krb5ApReq (0x100)  
>     Protocol: SMB  
>     Command: Session Setup Andx 115(0x73)  
>   + NTStatus: 0x0, Facility = FACILITY\_SYSTEM, Severity =
> STATUS\_SEVERITY\_SUCCESS, Code = (0) STATUS\_SUCCESS  
>   + SMBHeader: Command, TID: 0xFFFF, PID: 0xFEFF, UID: 0x0000, MID:
> 0x0040  
>   - CSessionSetupAndXNTLMESS:  
>      WordCount: 12 (0xC)  
>      ANDXCommand: No Secondary Command 255(0xFF)  
>      AndXReserved: 0 (0x0)  
>      ANDXOffset: 0 (0x0)  
>      **MaxBufferSize: 16644 (0x4104)**  
>      MaxMpxCount: 50
(0x32)  

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>** 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">How
is MaxBufferSize selected?
<span style="mso-spacerun: yes">  </span></span></span>**

The MaxBufferSize can be configured through the following registry
setting:

**<span style="COLOR: black"><span style="mso-spacerun: yes"> </span>*HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\SizeReqBuf*</span>**

**<span style="COLOR: black">Data Type:<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span></span>**<span style="COLOR: black">REG\_DWORD****</span>

**<span style="COLOR: black">Range</span>**<span style="COLOR: black">:
<span style="mso-spacerun: yes">   </span><span style="mso-spacerun: yes">      </span>1024
to 65535</span>

**<span style="COLOR: black">Default Values
(</span>**<span style="COLOR: black">If the key is not
present):<span style="mso-spacerun: yes">   </span></span>

<span style="COLOR: black"></span>

 

<span style="COLOR: black">For Windows Server versions of OS (NT server,
Windows 2000, Windows 2003, Windows 2008)</span>

<span style="COLOR: black"><span style="mso-spacerun: yes">    
</span>If<span style="mso-spacerun: yes">   </span>Physical Memory \<
=512M,<span style="mso-spacerun: yes">   </span>default is 4356.</span>

<span style="COLOR: black"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes"> </span>Else<span style="mso-spacerun: yes">   
</span>default value is 16644.</span>

<span style="mso-spacerun: yes"></span> 

<span style="COLOR: black">For Windows client versions of OS (Windows
98, NT, 2000, XP and Vista) </span>

<span style="COLOR: black"><span style="mso-spacerun: yes">    
</span>Default value is
4356.</span>

<span style="COLOR: black"></span> 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Is
maximum buffer size always the value of MaxBufferSize?</span></span>**

MaxBufferSize doesn’t ALWAYS reflect the maximum buffer allowed to
transfer file between SMB servers and clients., so the answer is
no.<span style="mso-spacerun: yes">  
</span>**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>**

For normal SMB\_COM\_READ and SMB\_COM\_WRITE requests, the maximum
buffer size is always equal to MaxBufferSize.

 

If CAP\_LARGE\_READX<span style="mso-spacerun: yes">  </span>or
CAP\_LARGE\_WRITEX <span style="mso-spacerun: yes"> </span>capability is
enabled on the SMB Negotiate Server
Response,<span style="mso-spacerun: yes">   </span>the maximum buffer
size
<span style="mso-spacerun: yes"> </span>used<span style="mso-spacerun: yes"> 
</span>is<span style="mso-spacerun: yes">  </span>61440 (60K)
<span style="mso-spacerun: yes"> </span>for large read( SMB\_COM\_
READ\_ANDX ) <span style="mso-spacerun: yes"> </span>and 65535 (64K) for
large write (SMB\_COM\_WRITE\_ANDX) , regardless of
MaxBufferSize.<span style="mso-spacerun: yes">   </span>But this is only
true if the SMB signing is not turned on (we will discuss this further
in the next section).<span style="mso-spacerun: yes">   </span>

 

The data length of each Large Read and Write operation can vary based on
application and APIs used, but it should never exceed the maximum buffer
size determined as above.<span style="mso-spacerun: yes">    </span>For
example,<span style="mso-spacerun: yes">  </span>when using Windows
Explorer or cmd prompt to write a large file to a network
share,<span style="mso-spacerun: yes">  </span>data is truncated
to<span style="mso-spacerun: yes">  </span>chunks of 61440 (60K) for
transferring, even though server can
<span style="mso-spacerun: yes"> </span>support<span style="mso-spacerun: yes"> 
</span>a larger 64K buffer.

 

By default,<span style="mso-spacerun: yes">  </span>CAP\_LARGE\_READX
and CAP\_LARGE\_WRITEX are always turned on by server.
<span style="mso-spacerun: yes"> </span>But they can be turned off by
the following registry
keys.

***<span style="COLOR: black"><span style="mso-spacerun: yes"> </span></span>HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\**<span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">disablelargeread</span>***

***HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\**<span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">
disablelargewrite</span>***

 

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">How
does SMB signing affect maximum buffer size allowed?</span></span>**

When SMB signing is negotiated by servers and clients, servers
automatically disable the Large Read and Large Write
feature.<span style="mso-spacerun: yes">   </span>It means that maximum
transmit buffer size for Large Read or Large
Write<span style="mso-spacerun: yes">  </span>will fall
<span style="mso-spacerun: yes"> </span>back to MaxBufferSize as
described in the previous section.<span style="mso-spacerun: yes">  
</span>For example, the default maximum buffer size is 4356 for client
OS, when signing is enabled.<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span>One caveat is that a
server still turns on CAP\_LARGE\_READX and CAP\_LARGE\_WRITEX bits in
Server Negotiate Response in this case, even though large read and write
operation will never be performed.
<span style="mso-spacerun: yes">  </span>This is something to be aware
of when reading network packet captures of SMB
traffic.

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Conclusion</span></span>**

The maximum size of server or client transmit buffer is dependent on a
combination of the type of operation (Large Read/Write), server registry
setting and SMB signing state.<span style="mso-spacerun: yes">   
</span>Understanding how maximum buffer size is determined and how it
can be changed can help you achieve the best SMB I/O performance.

 

 

</div>

</div>
