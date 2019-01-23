<div id="page">

# SMB 2.x and SMB 3.0 Timeouts in Windows

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
3/27/2013 1:20:00 PM

-----

<div id="content">

<span style="font-size: small;">This blog talks about common timeouts
for SMB dialects 2.x and 3.0 \[MS-SMB2\] in Windows. It also covers
continuous availability timeout, witness keep alive \[MS-SWN\], and some
SMB-Direct timers \[MS-SMBD\]. The behaviors are generally
version-specific and therefore may change in future Windows releases or
fixes.</span>

<span style="font-size: small;">A previous blog discusses “CIFS and SMB
Timeouts in
Windows”:    
<http://blogs.msdn.com/b/openspecification/archive/2013/03/19/cifs-and-smb-timeouts-in-windows.aspx>  
 </span>  
<span style="font-size: small;">NOTE: For questions on MS-SMB2, MS-SWN,
MS-SMBD documents, please post in the Open Specifications Forum: Windows
Protocols at
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols>.</span>

<span style="font-size: small;">Given a SMB2 file sharing scenario,
these are frequent troubleshooting questions on timeouts:</span>  
<span style="font-size: small;">- What timeouts are involved?</span>  
<span style="font-size: small;">- What are the related Windows
behaviors?</span>  
<span style="font-size: small;">- What timers are configurable and what
are their settings in Windows?</span>  
<span style="font-size: small;">Just as a refresher, the following are
the Windows SKUs where SMB dialects 2.x and 3.0 were
introduced.</span>  
<span style="font-size: small;">Dialect 2.002, Windows Vista and Windows
Server 2008.</span>  
<span style="font-size: small;">Dialect 2.1, Windows 7 and Windows
Server 2008 R2.</span>  
<span style="font-size: small;">Dialect 3.0, Windows 8 and Windows
Server 2012.</span>  
<span style="font-size: small;">All these SMB 2.x and 3.0 dialects share
the same core SMB2 Packet format \[MS-SMB2\].</span>

**<span style="font-size: small;">Request Expiration Timer
\[MS-SMB2\]</span>**

<span style="font-size: small;">This is the amount of time the client
waits for the server to respond to an outstanding request. This timeout
value can be adjusted through the following registry
setting:</span>  
<span style="font-size: small;">\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: SessTimeout</span>  
<span style="font-size: small;">Default:    60 seconds (Windows
Vista)</span>

<span style="font-size: small;">When the client does not receive the
response to a request before the Request Expiration Timer expires, it
will reset the connection because the operation is considered
blocked.</span>

<span style="font-size: small;">NOTE: The client may choose this timeout
based on local policy, the type of request, and network characteristics.
One such example is the implementation choice introduced in Windows 8
for the SMB2 Negotiate when continuously availability (CA) is active.
When negotiating with CA cluster servers, the Negotiate request timeout
is set to a smaller value, e.g. the maximum of 10 seconds and
SessTimeout/6. This is to allow a fast failover, so that when a CA
server is not responding, the SMB 3.0 client can expedite failover to
the other node. Recall that CA requires SMB 3.0 and onward.</span>

<span style="font-size: small;">If a request is being processed
asynchronously, i.e. the server sends an interim response with Status
set to STATUS\_PENDING and SMB2\_FLAGS\_ASYNC\_COMMAND bit set in Flags,
Windows clients extend the time-out as follows:</span>

<span style="font-size: small;">• If the asynchronous operation is SMB2
Directory Change Notification, the client will not enforce a timeout. 
</span>  
<span style="font-size: small;">• Otherwise, if the client is running at
least Windows 7 and ExtendedSessTimeout is configured, the timeout is
extended to the value of ExtendedSessTimeout:
</span>  
<span style="font-size: small;">\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: ExtendedSessTimeout
</span>  
<span style="font-size: small;">• Otherwise, if the client is running at
least Windows 7 and ExtendedSessTimeout is not configured, the timeout
is extended to four times the value of SessTimeout (4 \* SessTimeout).
By default, ExtendedSessTimeout is not configured.</span>

<span style="font-size: small;">For example, it is typical that an
asynchronous write operation expires if a backend Windows 2008 R2-based
storage server is taking over 4 minutes (4 \* 60 sec default SessTimeout
plus the scanning time to detect that the request expired) to complete
the operation. Increasing SessTimeout would effectively extend the time
for asynchronous operations.</span>

<span style="font-size: small;">The client does not enforce this timer
for the following commands:</span>  
<span style="font-size: small;">• Named Pipe Read</span>  
<span style="font-size: small;">• Named Pipe Write</span>  
<span style="font-size: small;">• Asynchronous Directory Change
Notifications</span>  
<span style="font-size: small;">• Blocking byte range lock
requests</span>  
<span style="font-size: small;">• FSCTLs: FSCTL\_PIPE\_PEEK,
FSCTL\_PIPE\_TRANSCEIVE, FSCTL\_PIPE\_WAIT</span>

<span style="font-size: small;">Note that SessTimeout and
ExtendedSessTimeout also apply to Windows-based CIFS/SMB as well, see
[previous
blog](http://blogs.msdn.com/b/openspecification/archive/2013/03/19/cifs-and-smb-timeouts-in-windows.aspx).
However, the use of ExtendedSessTimeout in SMB is controlled by client
configuration of ServersWithExtendedSessTimeout rather a server
response.</span>

**<span style="font-size: small;">Session Expiration Timer
\[MS-SMB2\]</span>**

<span style="font-size: small;">This timer is used as a frequency to
scan and mark sessions as expired when their specific expiration time is
reached. This timer value is 45 seconds in Windows-based
servers.</span>  
<span style="font-size: small;">If a session is in expired state and a
request is received, the server should return
STATUS\_NETWORK\_SESSION\_EXPIRED and the client must re-authenticate.
However, while a session is in expired state, the server processes
requests in the following cases:</span>  
<span style="font-size: small;">- LOGOFF, CLOSE, and LOCK (unlock) which
would allow to gracefully teardown. </span>  
<span style="font-size: small;">- SESSION\_SETUP for re-authentication.
</span>  
<span style="font-size: small;">- Windows releases prior to Windows 8 do
not fail a signed request, i.e. the SMB2 header has SMB2\_FLAGS\_SIGNED
set in the Flags field, and the request is not an SMB2 LOCK. </span>  
<span style="font-size: small;">Authentication-specific expiration is
driven by the authentication package. See [previous
blog](http://blogs.msdn.com/b/openspecification/archive/2013/03/19/cifs-and-smb-timeouts-in-windows.aspx)
on “CIFS and SMB Timeouts in Windows” for more details.
Session.ExpirationTime is set to the value returned by SSPI
AcceptSecurityContext. </span>  
<span style="font-size: small;">Note that for a given connection object,
if the SessionTable remains empty between two cycles of session
expiration timer, Windows-based servers will scavenge and disconnect the
connection.</span>

**<span style="font-size: small;">Resilient Open Scavenger Timer
\[MS-SMB2\]</span>**  
<span style="font-size: small;">This feature was introduced with SMB 2.1
in Windows 7.</span>  
<span style="font-size: small;">This timer is started when the transport
connection associated with a resilient handle is lost. It controls the
amount of time the server keeps a resilient handle active after the
transport connection to the client is lost.</span>  
<span style="font-size: small;">A resilient handle/open is meant to
survive temporary transport network disruption. If the client
re-establishes connection in a reasonable time after the connection was
lost, the client can reconnect to the handle. A client marks a handle
resilient via SMB2 IOCTL with CtlCode FSCTL\_LMR\_REQUEST\_RESILIENCY.
Note that Windows does not check the negotiated dialect when processing
this FSCTL.</span>  
<span style="font-size: small;">The Open.ResiliencyTimeout is set as
follows:</span>  
<span style="font-size: small;">- Either a non-zero value is supplied in
the Timeout field of the NETWORK\_RESILIENCY\_REQUEST request. If the
requested timeout is greater than MaxResiliencyTimeout, the server
returns STATUS\_INVALID\_PARAMETER.</span>  
<span style="font-size: small;">- Otherwise, an implementation-specific
value is used for resiliency timeout. Windows 7 and Windows Server 2008
R2 servers keep the resilient handle open indefinitely when the Timeout
value (requested in NETWORK\_RESILIENCY\_REQUEST) is equal to zero.
Windows 8 and Windows Server 2012 set a default value of 120 seconds.
</span>  
<span style="font-size: small;">The MaxResiliencyTimeout value can be
configured
through:</span>  
<span style="font-size: small;">\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: ResilientTimeout</span>  
<span style="font-size: small;">Default:    300 seconds (Windows 7,
Server 2008 R2, 8, Server 2012)</span>

**<span style="font-size: small;">Durable Open Scavenger Timer
\[MS-SMB2\]</span>**  
<span style="font-size: small;">This feature was introduced with SMB 2.1
in Windows 7.</span>  
<span style="font-size: small;">This timer is started when the transport
connection associated with a durable handle is lost. It controls the
amount of time the server keeps a durable handle active after the
transport connection to the client is lost.</span>  
<span style="font-size: small;">A durable handle/open allows the client
to attempt to preserve and reestablish a file handle after a network
disconnection. A client requests an open to be durable through one of
the create contexts SMB2\_CREATE\_DURABLE\_HANDLE\_REQUEST or
SMB2\_CREATE\_DURABLE\_HANDLE\_REQUEST\_V2 (v2 requires SMB 3.0
dialect).</span>  
<span style="font-size: small;">The durability timeout is set as
follows:</span>  
<span style="font-size: small;">- For
SMB2\_CREATE\_DURABLE\_HANDLE\_REQUEST, Windows 7 and Windows 2008 R2
set this timeout to 16 minutes, Windows 8 and Windows Server 2012 set
the value to 2 minutes.  </span>  
<span style="font-size: small;">- For
SMB2\_CREATE\_DURABLE\_HANDLE\_REQUEST\_V2, the timeout is set in the
following order:</span>  
<span style="font-size: small;">  a) A non-zero value is supplied in the
Timeout field of the durable v2 create context request. The Timeout in
the response is set to the minimum between the durable-v2-create-context
requested timeout and an implementation-specific maximum value \<Windows
8.1 and Server 2012 R2 set this maximum to 300 seconds. Windows 8 and
Server 2012 set this value to the Timeout of the request\>. </span>  
<span style="font-size: small;">  b) A non-zero value is configured on
the share’s CATimeout property.</span>  
<span style="font-size: small;">  c) the server’s implementation
specific value; Windows-based servers use the value of the registry
setting:</span>  
<span style="font-size: small;">    
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\</span>  
<span style="font-size: small;">     Value type: Dword</span>  
<span style="font-size: small;">     Value name:
DurableHandleV2TimeoutInSecond</span>  
<span style="font-size: small;">     Default:    60 seconds (Windows 8,
Windows Server 2012)  
     Default:    180 seconds (Windows 8.1, Windows Server 2012
R2)</span>  
<span style="font-size: small;">     Maximum: 300 seconds</span>  
                
**<span style="font-size: small;">Continuous Availability
Timeout</span>**  
<span style="font-size: small;">This feature was introduced with SMB 3.0
in Windows 8.</span>  
<span style="font-size: small;">With SMB 3.0, each share has a CATimeout
property which defines the minimum time the server should hold a
persistent handle on a continuously available share before closing the
handle if it is un-reclaimed. By default, Windows 8 and Windows Server
2012 set CATimeout to zero.</span>  
<span style="font-size: small;">CATimeout can be set or retrieved using
PowerShell command Set-SmbShare or Get-SmbShare.</span>  
<span style="font-size: small;">Each share’s CATimeout needs to be
configured to a sensible value to enable the SMB 3 client to perform
transparent file handle recovery during server failovers.</span>  
<span style="font-size: small;">In the event of server failover, the
persistent handle may have timed out before the client reconnects to the
clustered server and attempts to reclaim the handle. If that occurs, the
client may replay an outstanding Read, Write, or IOCTL operation by
using a stale handle which no longer exists on the server side. 
</span>  
<span style="font-size: small;">Ideally, if a persistent handle times
out, the client should abandon the outstanding operation and return an
error to the application.</span>

**<span style="font-size: small;">Witness keep-alive interval
\[MS-SWN\]</span>**  
<span style="font-size: small;">This functionality was introduced for
SMB 3.0 in Windows 8.</span>  
<span style="font-size: small;">The witness protocol is used to
explicitly notify a client of resource changes that have occurred on a
highly available cluster server. This enables faster recovery from
unplanned failures, so that the client does not need to wait for TCP
timeouts.</span>  
<span style="font-size: small;">The server advertises the support of
witness protocol monitoring through the SMB2 TREE\_CONNECT response
capability flag SMB2\_SHARE\_CAP\_CLUSTER. The client instructs its
witness client to register for asynchronous notifications for desired
resources on the cluster node it is not connected to. The witness
(server) service listens and reports cluster events related to the
clustered file server that the client is connected to.</span>  
<span style="font-size: small;">When the client registers (i.e.
WintnessrRegister), the server assigns a registration key – a unique UID
– that is used for subsequent requests on that context handle. A normal
client shutdown (e.g. LanmanWorkstation) would trigger
WintnessrUnregister and clear the associated state information on both
sides.</span>  
<span style="font-size: small;">However, if the client crashes or gets
disconnected, the witness service gets notified by RPC runtime for the
disconnection. The witness service uses a default RPC keep-alive
interval that can be configured via the following registry
setting:</span>  
<span style="font-size: small;">\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\SMBWitness\\Parameters\\</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: KeepAliveInterval</span>  
<span style="font-size: small;">Default:    20 minutes (Windows 8,
Windows Server 2012)</span>  
<span style="font-size: small;">Upon receipt of disconnection
notification, the witness service will implicitly unregister the
client.</span>  
<span style="font-size: small;">When the client comes back online after
it crashed, it will register again since it has lost its state
information.</span>  
<span style="font-size: small;">If the client simply lost the
connection, and reconnected before the server noticed, the client
cancels any outstanding WitnessrAsyncNotify just in case RPC runtime is
still holding its state and then re-issues a new RPC call.</span>

**<span style="font-size: small;">SMB-Direct timers \[MS-SMBD\]</span>**

<span style="font-size: small;">SMB-Direct is a new transport supported
in Windows 8. It is designed to carry SMB2 over Remote Direct Memory
Access (RDMA) Transport Protocol.</span>

**<span style="font-size: small;">Negotiation Timer</span>**  
<span style="font-size: small;">This timer is per-connection. It
controls the amount of time to:</span>  
<span style="font-size: small;">- Establish a connection and complete
negotiation. ConnectTimeoutInMs is the deadline for the remote peer to
accept the connection request and complete SMB Direct negotiation.
</span>  
<span style="font-size: small;">- Accept negotiation: The SMB Direct
Negotiate request should be received before AcceptTimeoutInMs expires.
The servers starts this timer as soon as it accepted the connection.
</span>  
<span style="font-size: small;">\\HKLM\\System\\CurrentControlSet\\Services\\SmbDirect\\Parameters</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: ConnectTimeoutInMs </span>  
<span style="font-size: small;">Default: 120 seconds (Windows
8)</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name: AcceptTimeoutInMs</span>  
<span style="font-size: small;">Default: 5 seconds (Windows 8)</span>

**<span style="font-size: small;">Idle Connection Timer</span>**  
<span style="font-size: small;">This timer is per-connection. It is the
amount of time the connection can be idle without receiving a message
from the remote peer. Before the local peer terminates the connection,
it sends a keep alive request to the remote peer and applies a keep
alive
timer.</span>  
<span style="font-size: small;">\\HKLM\\System\\CurrentControlSet\\Services\\SmbDirect\\Parameters</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name:
IdleConnectionTimeoutInMs</span>  
<span style="font-size: small;">Default: 120 seconds (Windows 8)</span>

**<span style="font-size: small;">Keep alive interval</span>**  
<span style="font-size: small;">This attribute is per-connection. It
defines the timeout to wait for the peer response for a keep-alive
message on an idle RDMA
connection.</span>  
<span style="font-size: small;">\\HKLM\\System\\CurrentControlSet\\Services\\SmbDirect\\Parameters</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name:
KeepaliveResponseTimeoutInMs</span>  
<span style="font-size: small;">Default: 5 seconds (Windows 8)</span>

**<span style="font-size: small;">Send Credit Grant Timer</span>**  
<span style="font-size: small;">This timer is per-connection.  It
regulates the amount of time that the local peer waits for the remote
peer to grant Send credits before disconnecting the connection. This
timer is started when the local peer runs out of Send
credits.</span>  
<span style="font-size: small;">\\HKLM\\System\\CurrentControlSet\\Services\\SmbDirect\\Parameters</span>  
<span style="font-size: small;">Value type: Dword</span>  
<span style="font-size: small;">Value name:
CreditGrantTimeoutInMs</span>  
<span style="font-size: small;">Default: 5 seconds (Windows 8)</span>

<span style="font-size: small;">If any of these SMB-Direct timers
expires, the local peer terminates the connection, then signals the
connection loss to the RDMA provider.</span>

**<span style="font-size: small;">References</span>**  
<span style="font-size: small;">\[MS-SMB2\]: Server Message Block (SMB)
Protocol Versions 2 and
3</span>  
<span style="font-size: small;"><http://msdn.microsoft.com/en-us/library/cc246482.aspx></span>  
<span style="font-size: small;">\[MS-SWN\]: Service Witness
Protocol</span>  
<span style="font-size: small;"><http://msdn.microsoft.com/en-us/library/hh536748.aspx></span>  
<span style="font-size: small;">\[MS-SMBD\]: SMB2 Remote Direct Memory
Access (RDMA) Transport
Protocol</span>  
<span style="font-size: small;"><http://msdn.microsoft.com/en-us/library/hh536346.aspx></span>

 

</div>

</div>
