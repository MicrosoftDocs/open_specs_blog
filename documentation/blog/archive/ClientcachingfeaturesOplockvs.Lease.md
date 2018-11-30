<div id="page">

# Client caching features: Oplock vs. Lease

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
5/22/2009 4:57:00 PM

-----

<div id="content">

    Opportunistic locks ,or oplock is
<span style="mso-spacerun: yes"> </span>a client caching mechanism that
allows SMB/SMB2 clients to dynamically decide the client-side buffering
strategy so the network traffic can be
minimized.<span style="mso-spacerun: yes">   </span>For example, a
client can buffer <span style="mso-spacerun: yes"> </span>data for
writing <span style="mso-spacerun: yes"> </span>locally to reduce
network packets <span style="mso-spacerun: yes"> </span>if the client is
notified there are no other processes
<span style="mso-spacerun: yes"> </span>accessing
data.<span style="mso-spacerun: yes">   </span>The same buffering can be
done on reading , where data from a remote file is read ahead to reduce
the round trips between the client and the server if the client
knows<span style="mso-spacerun: yes">  </span>that there are no other
processes writing data to the file.<span style="mso-spacerun: yes">   
</span>

     In SMB2.1, which is the new SMB2 dialect implemented in Windows
7,<span style="mso-spacerun: yes">  </span>Lease is introduced as a new
type of client caching mechanism.
<span style="mso-spacerun: yes"> </span>It
<span style="mso-spacerun: yes"> </span>offers more flexibility for
controlling client caching and thus results in significant performance
improvement in high latency
network<sup><span style="FONT-SIZE: 14pt; LINE-HEIGHT: 115%; mso-bidi-font-size: 11.0pt">\[2\]</span></sup>.

In this blog, we will analyze the network behavior
of<span style="mso-spacerun: yes">  </span>oplock
<span style="mso-spacerun: yes"> </span>in SMB/SMB2
<span style="mso-spacerun: yes"> </span>and leasing
<span style="mso-spacerun: yes"> </span>in
SMB2.1.<span style="mso-spacerun: yes">     </span>We will compare lease
and oplock and discuss the advantages of lease over oplock in certain
scenarios.
<span style="mso-spacerun: yes">   </span>

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Oplocks
<span style="mso-spacerun: yes"> </span>in SMB/SMB2</span>**

<span style="mso-spacerun: yes"> </span>The following types of oplocks
are available in SMB/SMB2.<span style="mso-spacerun: yes"> 
</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Batch Oplock**

This oplock allows a client to perform batch operations to repeatedly
open, read/write and close the same
file<span style="mso-spacerun: yes">  </span>without actually closing
the file on the server.<span style="mso-spacerun: yes">  </span>This
oplock<span style="mso-spacerun: yes">  </span>is exclusive and cannot
be held by two clients at the same
time.<span style="mso-spacerun: yes">   </span>It provides read, write
and handle caching
.

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Level II Oplock**

This oplock allows multiple clients for reading, but no
writing.<span style="mso-spacerun: yes">   </span>This oplock can be
shared by multiple clients at the same
time.<span style="mso-spacerun: yes">  </span>It provides read caching
only.

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Exclusive
Oplock<span style="mso-spacerun: yes">   </span>(only in SMB2)**

This oplock allows a client to have exclusive access to do arbitrary
buffering.<span style="mso-spacerun: yes">  </span>It
<span style="mso-spacerun: yes"> </span>provides read caching and writes
caching.

 

<span style="mso-spacerun: yes">    </span>After an oplock is granted to
a client, if the server receives another operation that
<span style="mso-spacerun: yes"> </span>conflicts with the current
opLock held by the client, an oplock break notification will be sent to
the client.<span style="mso-spacerun: yes">  </span>The client should
send an <span style="mso-spacerun: yes"> </span>acknowledgement for the
oplock break to the server before the server
can<span style="mso-spacerun: yes"> 
</span>proceed.<span style="mso-spacerun: yes">  </span>For example, if
a client holds a Level II oplock and another client opens the same file
on the server for writing, the level II oplock must be broken and the
client caching has to be
disabled.

**<span class="underline"><span style="COLOR: #0070c0"></span></span>**

<span style="TEXT-DECORATION: none"> </span>

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Oplock
network traffic:<span style="mso-spacerun: yes">  
</span></span>**

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span>**

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>A client requests
<span style="mso-spacerun: yes"> </span>an
oplock<span style="mso-spacerun: yes">  </span>from
<span style="mso-spacerun: yes"> </span>a server
in

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span><span style="mso-spacerun: yes"> </span>Flag field
of<span style="mso-spacerun: yes">  </span>SMB\_COM\_NT\_CREATE\_ANDX
(SMB)<span style="mso-spacerun: yes">  </span>

<span style="mso-spacerun: yes">                          
</span>or

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>RequestedOplockLevel field in
SMB2\_CREATE(SMB2)

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The server grants
<span style="mso-spacerun: yes"> </span>oplock to the client
in

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>opLockLevel of<span style="mso-spacerun: yes"> 
</span>SMB\_COM\_NT\_CREATE\_ANDX response(SMB)

<span style="mso-spacerun: yes">                        
</span>or

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>opLockLevel of<span style="mso-spacerun: yes"> 
</span>SMB2\_CREATE<span style="mso-spacerun: yes"> 
</span>response(SMB2)

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The server sends an opLock break
<span style="mso-spacerun: yes"> </span>to the client, if the condition
for oplock break is met.

There is a difference between SMB and SMB2 for how an oplock break is
sent to a
client

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>For SMB, the server sends an oplock break to the
client using SMB\_LOCK\_ANDX.<span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes"> </span>LockType<span style="mso-spacerun: yes"> 
</span>field is the<span style="mso-spacerun: yes">  </span>level
that<span style="mso-spacerun: yes">  </span>the oplock is broken
into.<span style="mso-spacerun: yes">   
</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>For SMB2, there is a specific command
(SMB2\_OPLOCK\_BREAK,0x12 ) used to send the oplock break notification
to the client.<span style="mso-spacerun: yes"> 
</span>

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The client sends
the<span style="mso-spacerun: yes"> 
</span>acknowledgment<span style="mso-spacerun: yes">  </span>for the
opLock break to the<span style="mso-spacerun: yes">  </span>server

 

<span style="mso-spacerun: yes"> </span>SMB and SMB2 clients use
different commands to acknowledge oplock
breaks.

 

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>For SMB, the
client<span style="mso-spacerun: yes">  </span>sends an oplock break
acknowledgement to the server<span style="mso-spacerun: yes"> 
</span>using SMB\_LOCK\_ANDX.<span style="mso-spacerun: yes">   
</span>LockType is the type of oplock client
holds.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>For SMB2, the client sends
SMB2\_OPLOCK\_BREAK\_ACKNOWLEDGEMENT to the
server.<span style="mso-spacerun: yes">  </span>OplockType is the type
of oplock client holds.

<span style="COLOR: #002060"><span style="mso-spacerun: yes">  
</span></span>

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Lease
in SMB 2.1<span style="mso-spacerun: yes">  </span>(Windows 7)</span>**

<span style="mso-spacerun: yes">         </span>In Windows 7, a new
caching mechanism named Lease is
introduced.<span style="mso-spacerun: yes">    </span>It shares the same
purpose as an oplock, which allows clients to adjust their buffering
policy to increase performance and to reduce network
use.<span style="mso-spacerun: yes">   </span>Actually, the newly added
lease types correspond to the new oplock types introduced in Windows 7
file system <sup>\[1\]</sup>.<span style="mso-spacerun: yes">  
</span>SMB2.1 just gives it a different name to distinguish it from the
existing oplock
functionality.

**<span style="FONT-SIZE: 12pt; LINE-HEIGHT: 115%"><span style="mso-spacerun: yes">        
</span></span>**The primary types of leases available
are:

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Read-caching
lease**:<span style="mso-spacerun: yes"> 
</span>allows<span style="mso-spacerun: yes">  </span>caching
<span style="mso-spacerun: yes"> </span>reads and can be shared by
multiple
clients.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Write-caching lease**: allows caching writes and
<span style="mso-spacerun: yes"> </span>is exclusive to only one
client.

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**Handle-caching lease**: allows caching
<span style="mso-spacerun: yes"> </span>handles and can be shared by
multiple clients**<span style="FONT-SIZE: 12pt">.</span>**

<span style="mso-spacerun: yes">         </span>It is very important to
remember that a lease can be a combination of one or more of the leases
above.<span style="mso-spacerun: yes">   </span>The valid leases Windows
7 can grant are<span style="mso-spacerun: yes"> 
</span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**R**
(READ\_CACHING)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**RW**(READ\_CACHING|WRITE\_CACHING)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**RH**(READ\_CACHING|HANDLE\_CACHING)

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>**RWH**(READ\_CACHING|WRITE\_CACHING|HANDLE\_CACHING)

 

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">SMB2.1
<span style="mso-spacerun: yes"> </span>lease network
traffic:<span style="mso-spacerun: yes">  
</span></span>**<span style="COLOR: #0070c0"></span>

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>A client requests<span style="mso-spacerun: yes"> 
</span>a<span style="mso-spacerun: yes"> 
</span>lease<span style="mso-spacerun: yes">  </span>from a
server

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span><span style="mso-spacerun: yes"> </span>Set
RequestedOplockLevel<span style="mso-spacerun: yes"> 
</span>field<span style="mso-spacerun: yes">  </span>to
SMB2\_OPLOCK\_LEVEL\_LEASE(0xFF)<span style="mso-spacerun: yes"> 
</span>in SMB2\_CREATE request
.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Use SMB2\_CREATE\_REQUEST\_LEASE create context in
SMB2\_CREATE request to indicate the type of lease
requested.

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="mso-spacerun: yes"> </span>The server
grants<span style="mso-spacerun: yes">  </span>a
lease<span style="mso-spacerun: yes">  </span>to the
client<span style="mso-spacerun: yes"> 
</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Set opLockLevel<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span>field
of<span style="mso-spacerun: yes"> 
</span>SMB2\_CREATE<span style="mso-spacerun: yes">  </span>response
to<span style="mso-spacerun: yes">  </span>OPLOCK\_LEVEL\_LEASE to
indicate that a lease is
granted.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Use SMB2\_CREATE\_RESPONSE\_LEASE create
context<span style="mso-spacerun: yes">  </span>in SMB2\_CREATE response
to indicate the type of lease granted.<span style="mso-spacerun: yes">  
</span>

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The<span style="mso-spacerun: yes">  </span>server
sends<span style="mso-spacerun: yes">  </span>lease breaks to the
client, if a lease break condition is
met.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>SMB2\_LEASE\_BREAK\_NOTIFICATION is
sent<span style="mso-spacerun: yes">  </span>to client. It contains the
current lease state and the new lease state the client should change
to.<span style="mso-spacerun: yes">    </span>This is a different
structure than
opLock.

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The client sends
the<span style="mso-spacerun: yes"> 
</span>acknowledgement<span style="mso-spacerun: yes">  </span>for the
lease break to the<span style="mso-spacerun: yes"> 
</span>server

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>SMB2\_LEASE\_BREAK\_ACKNOWLEDGEMENT is sent to
server.<span style="mso-spacerun: yes">  </span>It indicates the lease
state of client currently holds.<span style="mso-spacerun: yes">   
</span>This is a different structure than
opLock.

<span style="COLOR: #002060"></span>

 

**<span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Comparison
between Lease and
Oplock</span>**

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Lease in SMB2.1 is based on the new types of
oplocks introduced in Window 7 or later file
systems.<span style="mso-spacerun: yes">    </span>The purposes for
Lease and Oplock are the same.<span style="mso-spacerun: yes"> 
</span>But Lease provides greater flexibility for clients to request
caching mechanisms.<span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes"> </span>Some new caching levels
are <span style="mso-spacerun: yes"> </span>added in leasing featuer in
Windows 7.<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">    </span>

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="mso-spacerun: yes"> </span>One of the
<span style="mso-spacerun: yes"> </span>levels added is RH lease, which
allows read caching and handle caching so multiple clients
can<span style="mso-spacerun: yes">  </span>hold on to cached data even
after an application closes the
handle.<span style="mso-spacerun: yes">   </span>Compared with Level
II oplocks, which have to be broken between closing and opening handles,
this new lease level offers a big improvement for complex I/O intensive
applications.<span style="mso-spacerun: yes"> 
</span>

 

<span style="FONT-FAMILY: Symbol; mso-fareast-font-family: Symbol; mso-bidi-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Another enhancement of lease over oplock is that
SMB2.1 in Windows 7 allows full caching when multiple handles are opened
by the same client.<span style="mso-spacerun: yes">   </span>This is
also a significant improvement over oplock semantics when any additional
handle opened by even the same client will break the existing batch
oplock and thus disable the client
caching.<span style="mso-spacerun: yes">   </span>The following is the
comparison between oplock and lease for the case of multiple handles
opened.<span style="mso-spacerun: yes"> 
</span>

<span style="COLOR: #002060"><span style="mso-spacerun: yes">                                     
</span>**<span class="underline">Oplock with multiple handles from the
same
client</span>**</span>

<span style="COLOR: #002060"><span style="mso-spacerun: yes">                     
</span>**Client**<span style="mso-spacerun: yes">                   
</span><span style="mso-spacerun: yes">       </span><span style="mso-spacerun: yes">                     </span>**Network**<span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">       </span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">         </span>**Server**<span style="mso-spacerun: yes">       
</span></span>

CreateFile with READ and
WRITE<span style="mso-spacerun: yes">                         
  </span>--\><span style="mso-spacerun: yes">                
</span>First handle on the
file<span style="mso-spacerun: yes">                                                                                  
</span>

<span style="mso-spacerun: yes">                                                                                
</span>\<--<span style="mso-spacerun: yes">                 
</span>Batch oplock granted<span style="mso-spacerun: yes">            
</span>

<span style="mso-spacerun: yes">   
</span>ReadFile<span style="mso-spacerun: yes">                                                                 
</span>- -\><span style="mso-spacerun: yes">                 
</span>

<span style="mso-spacerun: yes">                                                                                   
</span>\<--<span style="mso-spacerun: yes">                  </span>read
data from file on server

<span style="mso-spacerun: yes">   
</span>ReadFile….<span style="mso-spacerun: yes">  </span>(From cached
data)<span style="mso-spacerun: yes">               </span>No network
packets

<span style="mso-spacerun: yes">   
</span>WriteFile…<span style="mso-spacerun: yes">  </span>(From cached
data)<span style="mso-spacerun: yes">               </span>No network
packets

<span style="mso-spacerun: yes">    </span>CreatFile with
READ<span style="mso-spacerun: yes">                                           
</span><span style="mso-spacerun: yes"> </span>--\><span style="mso-spacerun: yes">                  
</span>Second handle on the
file<span style="mso-spacerun: yes">            </span>

<span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes"> </span>***No more data caching
allowed*** ** <span style="mso-spacerun: yes">                         
</span>\<--<span style="mso-spacerun: yes">      
</span><span style="mso-spacerun: yes">            </span>***Batch
oplock broken***<span style="mso-spacerun: yes">       </span>

<span style="mso-spacerun: yes">   
</span>ReadFile<span style="mso-spacerun: yes">                                                               
</span><span style="mso-spacerun: yes"> </span>--\><span style="mso-spacerun: yes">                 
</span>File I/O on the
server<span style="mso-spacerun: yes">                 
</span>

<span style="mso-spacerun: yes">  
</span>WriteFile<span style="mso-spacerun: yes">                                                              
</span><span style="mso-spacerun: yes"> </span>--\><span style="mso-spacerun: yes">    
</span><span style="mso-spacerun: yes">              </span>File I/O on
the
server

 

<span style="COLOR: #002060"><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">                                    
</span>**<span class="underline">Lease
<span style="mso-spacerun: yes"> </span>with multiple handles from the
same
client</span>**</span>

<span style="COLOR: #002060"><span style="mso-spacerun: yes">                     
</span>**Client**<span style="mso-spacerun: yes">                                               
</span>**Network**<span style="mso-spacerun: yes">                    
</span>**Server**<span style="mso-spacerun: yes">        </span></span>

<span style="COLOR: #002060"><span style="mso-spacerun: yes">   
</span></span>CreateFile with READ and
WRITE<span style="mso-spacerun: yes">              
</span><span style="mso-spacerun: yes">             </span>--\>
<span style="mso-spacerun: yes">                 </span>First handle on
the file<span style="mso-spacerun: yes"> 
</span>

<span style="mso-spacerun: yes">                                                                                    
</span>\<--<span style="mso-spacerun: yes">                
</span><span style="mso-spacerun: yes">  </span>Lease<span style="mso-spacerun: yes"> 
</span>granted<span style="mso-spacerun: yes">             </span>

<span style="mso-spacerun: yes">   
</span>ReadFile<span style="mso-spacerun: yes">                                                    
</span><span style="mso-spacerun: yes">              </span>--\>
<span style="mso-spacerun: yes">                </span>

<span style="mso-spacerun: yes">                                                                                    
</span>\<--<span style="mso-spacerun: yes">                
</span><span style="mso-spacerun: yes">  </span>read data from file on
server

<span style="mso-spacerun: yes">   
</span>ReadFile….<span style="mso-spacerun: yes">  </span>(From cached
data)<span style="mso-spacerun: yes">              </span>No network
packets<span style="mso-spacerun: yes">   </span>

<span style="mso-spacerun: yes">   
</span>WriteFile…<span style="mso-spacerun: yes">  </span>(From cached
data)<span style="mso-spacerun: yes">              </span>No network
packets

<span style="mso-spacerun: yes">    </span>CreatFile with
READ<span style="mso-spacerun: yes">                                           
</span><span style="mso-spacerun: yes">     </span>--\>
<span style="mso-spacerun: yes">                  </span>***Lease is not
broken on the second handle***<span style="mso-spacerun: yes">          
</span>

<span style="mso-spacerun: yes">   
</span>ReadFile<span style="mso-spacerun: yes">       </span>(From
cached data)<span style="mso-spacerun: yes">        
</span><span style="mso-spacerun: yes">    </span>No network
packets<span style="mso-spacerun: yes">                                                  
</span>

<span style="mso-spacerun: yes">  
</span>WriteFile<span style="mso-spacerun: yes">        </span>(From
cached data)<span style="mso-spacerun: yes">          
</span><span style="mso-spacerun: yes"> </span>No network
packets<span style="mso-spacerun: yes">                                               
</span>

<span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">  </span>From two scenarios
above, we can see that oplock doesn’t allow data caching if there are
multiple handles opened by the same client; on the other hand, the
leasing feature in Windows 7 allows<span style="mso-spacerun: yes"> 
</span>full data caching on multiple handles as long as they are opened
by the same client.<span style="mso-spacerun: yes">    </span>This
client caching enhancement can provide a further performance boost,
especially on high latency network.

For more information about client caching improvements in Windows 7, you
can review the excellent PDC presentation on the following
link<sup><span style="FONT-SIZE: 12pt; LINE-HEIGHT: 115%; mso-bidi-font-size: 11.0pt">
</span><span style="FONT-SIZE: 14pt; LINE-HEIGHT: 115%; mso-bidi-font-size: 11.0pt">\[2\]</span></sup>
<span style="mso-spacerun: yes"> </span>and the MS-SMB and MS-SMB2
documents<sup><span style="FONT-SIZE: 14pt; LINE-HEIGHT: 115%; mso-bidi-font-size: 11.0pt">\[3\]\[4\]</span></sup>.<span style="mso-spacerun: yes"> </span>

<span style="mso-spacerun: yes">  </span><span style="mso-spacerun: yes"> </span>

**<span style="FONT-SIZE: 12pt; COLOR: #002060; LINE-HEIGHT: 115%">Reference
Link:</span>**

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">\[MSDN-FSBO\]
Microsoft Corporation, "File System Behavior in the Microsoft Windows
Environment", June 2008,
</span>[<span style="COLOR: blue; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">http://download.microsoft.com/download/4/3/8/43889780-8d45-4b2e-9d3a-c696a890309f/File%20System%20Behavior%20Overview.pdf</span>](http://download.microsoft.com/download/4/3/8/43889780-8d45-4b2e-9d3a-c696a890309f/File%20System%20Behavior%20Overview.pdf)<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">
.</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>[<span style="COLOR: windowtext; TEXT-DECORATION: none; text-underline: none">Windows
7: Optimizing Applications for Remote File Services over the
WAN</span>](https://channel9.msdn.com/pdc2008/ES23/)
[<span style="COLOR: blue">https://channel9.msdn.com/pdc2008/ES23/</span>](https://channel9.msdn.com/pdc2008/ES23/)

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">3.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="COLOR: black; mso-bidi-font-weight: bold; mso-bidi-font-family: Arial">\[MS-SMB2\]:
Server Message Block (SMB) Version 2 Protocol Specification
</span>[<span style="mso-bidi-font-weight: bold; mso-bidi-font-family: Arial">http://msdn.microsoft.com/en-us/library/cc246482(PROT.13).aspx</span>](http://msdn.microsoft.com/en-us/library/cc246482\(PROT.13\).aspx)<span style="COLOR: black; mso-bidi-font-weight: bold; mso-bidi-font-family: Arial">
</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">4.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="COLOR: black; mso-bidi-font-weight: bold; mso-bidi-font-family: Arial">\[MS-SMB\]:
Server Message Block (SMB) Protocol
Specification</span><span style="FONT-SIZE: 11pt; COLOR: black; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-weight: bold; mso-fareast-font-family: Calibri; mso-bidi-font-family: Arial; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"><span style="mso-spacerun: yes"> </span></span><span style="FONT-SIZE: 11pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-fareast-font-family: Calibri; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-theme-font: minor-latin; mso-bidi-theme-font: minor-bidi; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">[<span style="mso-bidi-font-weight: bold; mso-bidi-font-family: Arial">http://msdn.microsoft.com/en-us/library/cc246231(PROT.13).aspx</span>](http://msdn.microsoft.com/en-us/library/cc246231\(PROT.13\).aspx)</span><span style="FONT-SIZE: 11pt; COLOR: black; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-weight: bold; mso-fareast-font-family: Calibri; mso-bidi-font-family: Arial; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">
</span>

</div>

</div>
