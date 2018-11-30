<div id="page">

# CIFS and SMB Timeouts in Windows

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
3/19/2013 12:36:00 PM

-----

<div id="content">

This blog gives a consolidated overview of the most common SMB timeouts
in Windows and their behaviors. Some of these legacy timeouts or timers
are optional, implementation specific, not defined or not required by
the protocol specifications. Let’s recall that MS-CIFS documents the
protocol implemented in Windows NT and Windows 98, whereas MS-SMB
describes the extensions added in Windows 2000 and beyond.

A subsequent blog discusses “SMB 2.x and SMB 3.0 Timeouts in
Windows”:   
<http://blogs.msdn.com/b/openspecification/archive/2013/03/27/smb-2-x-and-smb-3-0-timeouts-in-windows.aspx>

NOTE: For questions on MS-CIFS and MS-SMB documents, please post in the
Open Specifications Forum: Windows Protocols at
<http://social.msdn.microsoft.com/Forums/en-US/os_windowsprotocols>.

**SMB exchange timeout**

Here, I am using the term “exchange timeout”. There is no such
definition in the specifications.  
The exchange timeout can be defined as the maximum amount of time that
the client redirector waits for a server to respond to a timed SMB
message. There are three variants of SMB exchange timeout that are
observed commonly in Windows: client session timeout, extended session
timeout, offline file timeout.

**Client session timeout**  
It is the most common exchange timeout. This is defined in \[MS-CIFS\]
as a system-wide parameter Client.SessionTimeoutValue. This value can be
configured through the SessTimeout registry key
\[KB102067\].  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\  
Value type: Dword  
Value name: SessTimeout  
Default:    45 seconds (Windows NT)  
Default:    60 seconds (Windows 2000)

**Extended session timeout**  
This optional timeout may be useful in high latency networks. It applies
to a timed exchange with a server listed in
ServersWithExtendedSessTimeout.  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\  
Value type: Dword  
Value name: ExtendedSessTimeout  
Default:    1000 (seconds)  
Value type: Multi-string  
Value name: ServersWithExtendedSessTimeout  
Default:    \<null\>  
These were introduced in Windows XP.

**Offline file timeout**  
The client uses this optional timeout for a timed operation on an
offline file, normally indicated by the FILE\_ATTRIBUTE\_OFFLINE
attribute. This timeout can be configured with the registry
parameter  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\Parameters\\  
Value type: Dword  
Value name: OffLineFileTimeoutIntervalInSeconds  
Default:    1000  
This was introduced in Windows 2000.

**Request Expiration Timer**

This optional timer controls the scanning interval of outstanding
commands. It is used as a scavenger cycle for outstanding commands that
met their exchange timeout deadlines. Windows NT and Windows 98 CIFS
clients implement a default value of 30 seconds for the Request
Expiration Timer.  
When an outstanding command has passed the exchange timeout, the client
should close the connection if this remains idle. The time the client
effectively closes the connection will vary, but bounded to a minimum of
Client.SessionTimeoutValue.  
For example, let’s assume that at a 30 seconds scanning interval, an
outstanding command has a 60 seconds session timeout due to expire in 5
seconds, the redirector will close the connection at the next scavenger
cycle. The effective connection closing time would be around:  
Session timeout (60 sec) – time due at the scavenger timer (5 sec) +
next scavenger time (30 sec) = 85 sec.  
Commands exempt from the Request Expiration Timer  
The list of exempt commands is defined in MS-CIFS 3.2.6.1 Request
Expiration Timer Event.  
• The NT\_TRANSACT\_NOTIFY\_CHANGE subcommand  
• Read and write commands issued on an Open to a named pipe via the
following commands:  
• SMB\_COM\_READ  
• SMB\_COM\_WRITE  
• SMB\_COM\_READ\_ANDX  
• SMB\_COM\_WRITE\_ANDX  
• SMB\_COM\_WRITE\_AND\_CLOSE  
• TRANS\_READ\_NMPIPE  
• TRANS\_WRITE\_NMPIPE  
• TRANS\_RAW\_READ\_NMPIPE  
• TRANS\_TRANSACT\_NMPIPE  
• TRANS\_RAW\_WRITE\_NMPIPE  
• TRANS\_CALL\_NMPIPE  
• TRANS\_WAIT\_NMPIPE  
• SMB\_COM\_LOCKING\_ANDX Request with the Timeout field set to a
nonzero value

**The Echo behavior**

An Echo request is sent to determine whether or not an idle connection
has been lost. For an outstanding timed network operation, the
connection would be closed upon the expiration of the exchange timer.
The Echo mechanism does not prevent the connection from being closed if
there is no response to any outstanding command beyond the exchange
timeout. This way, the client avoids continuing Echo-probing a known
dead connection. If the server is responding to Echo requests, the
client will wait until the exchange times out, and the client will not
send any interim response to the calling application.

**OpLock Break Acknowledgment Timer**

This timer controls the time the server waits for a client to
acknowledge an OpLock break request, if an acknowledgement is required.
If an OpLock break acknowledgment is not received before the timer
elapses, the server locally breaks the lock. This is controlled through
the OplockBreakWait registry
\[KB129202\].  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\  
Value type: Dword  
Value name: OplockBreakWait  
Default:    35 (seconds)

**Idle Connection Timer**

This is the amount of time that a connection can be idle before being
disconnected. An idle connection is defined as a connection which has no
existing open handles (no open files, directories, search contexts,
etc.), and no pending operation. The Idle Connection Timer is
implementation-specific.  
When the server receives a message, Server.Connection.IdleTime is set to
the current time plus Server.AutoDisconnectTimeout \[MS-CIFS\].  
On Windows servers it can be configured through the Autodisconnect
registry key
\[KB297684\].  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\  
Value type: Dword  
Value name: Autodisconnect  
Default:    15 (minutes)  
The Autodisconnect can also be configured through group policy:  
Computer Configuration\\Windows Settings\\Security Settings\\Local
Policies\\Security Options  
"Microsoft network server: Amount of idle time required before
suspending session"

In older Windows clients (e.g. Windows 2000, Windows 2003), the client
closes an idle connection based on the KeepConn workstation parameter
\[KB819108\].  
HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanWorkstation\\parameters\\  
Value type: Dword  
Value name: KeepConn  
Default:    600 (seconds)  
KeepConn is no longer used, except in Installable File System Kit
(IFSKIT).

**Unused Open Search Timer**

This optional timer that controls the amount of time that an open search
can stay unused before the server closes the search context. It is used
as maximum age to scavenge open searches, see Server.SrvSearchMaxTimeout
\[MS-CIFS\].  
On Windows, the unused search timer can be configured
as  
\\HKLM\\SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\  
Value type: Dword  
Value name: MaxKeepSearch  
Default:   3600 (seconds)  
Range: 10 – 10,000

This legacy timer was for MS DOS applications which did not have the
capability to issue a “close search” primitive, such as Win32 FindClose,
to explicitly close a search.

**Authentication Expiration Timer**

This timer was introduced in MS-SMB. It is used to periodically scan and
mark authenticated sessions as expired when their specific expiration
time is reached. If a session is expired, SMB operations will receive
STATUS\_NETWORK\_SESSION\_EXPIRED and the client must re-authenticate.
Note that the server does not enforce session expiration on the
following commands while the authentication state of a session is
expired:  
\- SMB\_COM\_SESSION\_SETUP\_ANDX for session renewal,  
\- SMB\_COM\_CLOSE,  
\- SMB\_COM\_LOGOFF\_ANDX,  
\- SMB\_COM\_FLUSH (see KB 943459) ,  
\- SMB\_COM\_LOCKING\_ANDX as an oplock break acknowledgement (see KB
943459),  
\- SMB\_COM\_TREE\_DISCONNECT.

Note that KB 943459 does not apply to SMB 2. The fix was done on srv.sys
(SMB server) and mitigates older SMB clients that are not capable of
re-authenticating upon receiving a STATUS\_SESSION\_EXPIRED error in
response to an oplock break acknowledgment.  
More details are discussed later on session expiration and dynamic
re-authentication.

Authentication-specific expiration is driven by the authentication
package. For instance, NTLM authentication has no explicit expiration
time; authentications done with NTLM do not explicitly expire.  
For Kerberos authentication package, the service ticket’s endtime is
derived from the MaxServiceTicketAge when the ticket is issued, as
documented in \[MS-KILE\]. The default MaxServiceTicketAge is 10 hours
and can be adjusted though Group Policy Management Console under Default
Domain Policy/ Account Policies / Kerberos Policy.  
In addition, even if a session did not explicitly expire, the server may
forcibly disconnect the user SMB session based upon a group policy
setting and user’s logonHours \[MS-ADA1\].  
The group policy setting is:  
Computer Configuration\\Windows Settings\\Security Settings\\Local
Policies\\Security Options  
"Microsoft network server: Disconnect clients when logon hours expire"  
This policy sets the registry
key:  
HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters\\enableforcedlogoff  
When this policy is enabled, the SMB session is disconnected when the
user's logon hours expire.  
When this policy is disabled, the session remains valid as long as it
stays connected, even after logon hours expire, provided the session did
not expire.  
This policy may be configured on the Default Domain Policy and
propagated through group policy to the computers.   
As an example, let’s assume contoso\\user1 has logonHours configured to
“Logon Denied” between 5:00 PM and 9:00 AM.  An SMB share
[\\\\server2008r2\\Share](file://\\\\server2008r2\\Share) is accessed
from a Windows XP SP3 client at 4:59:30 PM where enableforcedlogoff is
enabled. Once the clock ticks 5:00 PM, the client falls in logonHours
expiry and the session is dropped. At 5:05 PM, when user1 attempts to
access the share, a similar error is observed:

  
*[\\\\server2008r2\\Share](file://\\\\server2008r2\\Share) is not
accessible. You might not have permission to use this network resource.
Contact the administrator of this server to find out if you have access
permissions.*  
*Logon failure: account logon time restriction violation.*

**Session expiration and dynamic re-authentication**  
In Windows implementation, SMB session expiration is enforced based upon
the client’s support of dynamic re-authentication capability
\[MS-SMB\].  
If the client enables the CAP\_DYNAMIC\_REAUTH capability bit, the
server will enforce session expiration. If a client does not set
CAP\_DYNAMIC\_REAUTH, the Windows server does not return
STATUS\_NETWORK\_SESSION\_EXPIRED.   
The SMB dynamic re-authentication feature was introduced in Windows XP.
From there, Windows-based clients set the CAP\_DYNAMIC\_REAUTH
capability bit to indicate to the server that the client supports
re-authentication when the Kerberos service ticket for the session
expires.  
Windows servers do check CAP\_DYNAMIC\_REAUTH:  
• If clientCapabilities sets CAP\_DYNAMIC\_REAUTH, the server will set
Server.Session.AuthenticationExpirationTime to the expiry time returned
by AcceptSecuirtyContext.  
• If clientCapabilities does not set CAP\_DYNAMIC\_REAUTH, the server
will not set Server.Session.AuthenticationExpirationTime, basically a
CAP\_DYNAMIC\_REAUTH capability bit not set by the client means the
session will not expire on the server side.

**References:**

\[MS-CIFS\]: Common Internet File System (CIFS) Protocol  
<http://msdn.microsoft.com/en-us/library/ee442092.aspx>  
\[MS-SMB\]: Server Message Block (SMB) Protocol  
<http://msdn.microsoft.com/en-us/library/cc246231.aspx>  
\[MS-WKST\]: Workstation Service Remote Protocol  
<http://msdn.microsoft.com/en-us/library/cc250262.aspx>  
When the Kerberos ticket expires for a Kerberos-authenticated SMB
connection that is created to a Windows Server 2003-based server, the
oplock on a file cannot be broken in a timely manner  
<http://support.microsoft.com/kb/943459>  
LanmanWorkstation Parameters: SessTimeout  
<http://technet.microsoft.com/en-us/library/cc938292.aspx>  
LanmanServer Parameters: MaxKeepSearch  
<http://technet.microsoft.com/en-us/library/cc957456.aspx>  
Remote Storage and Windows 2000  
<http://technet.microsoft.com/en-us/library/cc938445.aspx>  
How to Troubleshoot Event ID 2009 Errors  
<http://support.microsoft.com/kb/165815>  
“Microsoft network server: Disconnect clients when logon hours expire”  
<http://technet.microsoft.com/en-us/library/cc758192.aspx>  
HOW TO: Limit User Logon Time in a Domain in Windows Server 2003  
<http://support.microsoft.com/kb/816666>

 

</div>

</div>
