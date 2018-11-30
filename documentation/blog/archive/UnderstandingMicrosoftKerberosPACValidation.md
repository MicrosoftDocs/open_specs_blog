<div id="page">

# Understanding Microsoft Kerberos PAC Validation

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
4/24/2009 6:29:00
PM

-----

<div id="content">

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span> 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">This
blog post focuses on understanding Microsoft Kerberos PAC validation. It
discusses the topic from inter-operability perspective with Windows
operating systems. A SMB session establishment scenario is used to
illustrate how PAC validation
works.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

## Background<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Impersonation
enables a trusted identity to act on behalf of another identity to
perform an action. The trusted identity is usually a service account
that is granted a set of elevated privileges to access resources and
executes tasks.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
Privilege Account Certificate
(PAC)</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">
is an extension element of the authorization-data field contained in the
client's Kerberos ticket. <span style="mso-spacerun: yes"> </span>The
PAC structure is defined in \[MS-PAC\] and conveys authorization data
provided by domain controllers (DCs) in an Active Directory-enabled
domain. It contains information such as
<span style="FONT-SIZE: 11pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">security
identifiers</span>, group membership, user profile information, and
password credentials.<span style="mso-spacerun: yes">  </span>The
illustration below shows the relationship between a Kerberos ticket and
PAC.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span> 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

Ticket

</div>

</div>

 

 

 

 

 

 

Authorization
Data

 

 

 

PAC

 

 

 

 

Signature

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
1: PAC in Kerberos Protocol
Extensions</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">[MS-KILE](http://msdn.microsoft.com/en-us/library/cc233855\(PROT.10\).aspx)
specifies extensions that enable PAC support.
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">The
KDC generates the PAC under one of the following conditions:
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-list: Ignore">-<span style="FONT: 7pt &#39;Times New Roman&#39;">         
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">During
an Authentication Service (AS) request (KERB-AS-REQ) that includes a
KERB-PA-PAC-REQUEST PA-DATA as part of the pre-authentication
data.</span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-list: Ignore">-<span style="FONT: 7pt &#39;Times New Roman&#39;">         
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">During
a Ticket Granting Service (TGS) request for a cross-realm referral TGT
([\[RFC4120\]](http://go.microsoft.com/fwlink/?LinkId=90458)) when the
client’s TGT does not include a
PAC.</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-list: Ignore">-<span style="FONT: 7pt &#39;Times New Roman&#39;">         
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">During
a TGS request when the client has domain local groups. </span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">The
KDC searches </span>Active
Directory<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">
for the user or computer account. It creates the PAC structure,
including information such as direct and transitive group membership,
and encodes it into the TGT in an AD-IF-RELEVANT element of the
authorization data
([\[RFC4120\]](http://go.microsoft.com/fwlink/?LinkId=90458) section
5.2.6). Another KDC may subsequently update the PAC when the client
requests a TGS with additional server’s domain local groups. It should
also be noted that the KDC default behavior is to generate a PAC, unless
a client explicitly requests the PAC be excluded from the ticket through
the use of
KERB-PA-PAC-REQUEST.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">PAC
validation
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">is
a security feature that addresses PAC spoofing, preventing an attacker
from gaining unauthorized access to a system or its resources by using a
tampered PAC. This is critical in applications where user impersonation
is utilized. PAC validation ensures the user presents exact
authorization data as it was granted in the Kerberos ticket.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"><span style="mso-spacerun: yes"> </span></span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">One
key reason why a PAC should be verified as unaltered is to ensure that
no additional privileges have been maliciously added to - or removed
from - the ticket. This makes the PAC validation an important
consideration when designing applications that impersonate users and
access sensitive data, or applications that grant administrative
privileges.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span> 

## Kerberos PAC validation **<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"> </span>**

**<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>**

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 ![](http://bs6yng.blu.livefilestore.com/y1pZxyl3VBYxln1gFP-3jRDHPGq9Rh91OYuWYasPjmD42Fvz4DiJRcNTiopqNiBGSc8ODx6kcIObc3l8Cmwd2Q3nOlpPrlWx1oM/PACFig2.png)

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Figure
2: Kerberos PAC
validation</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Note:
The terms client and server refer to the definitions in [Kerberos
terminology.](http://www.ietf.org/rfc/rfc4120.txt)
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">The
server’s OS performs Kerberos PAC validation to protect from a man in
the middle attack tampering with the PAC structure. PAC validation is
not required for Microsoft Kerberos Extensions \[MS-KILE\] protocol
compliance. <span style="mso-spacerun: yes"> </span>It is however an
additional security measure designed to prevent the attack described
above. In the simplest an OS can always validate the PAC signature.
Since the PAC is encoded as an
<span style="FONT-SIZE: 11pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">AD-IF-RELEVANT
</span>element of the AuthorizationData field of the ticket, the server
can ignore the data if it does not consume the information in the
PAC.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">PAC
validation occurs in the security context of the server’s process where
the user is being impersonated. An application server that is
interoperating with a Windows KDC through MS-KILE and
[MS-APDS](http://msdn.microsoft.com/en-us/library/cc223948\(PROT.10\).aspx)
will base its criteria upon which it will send PAC validation messages
to the DC.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Note
that there must be a trust relationship between the application server
and the DC. In a single forest environment, the trust implicitly exists.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">When
the PAC validation occurs, the server encodes a
<span style="FONT-SIZE: 11pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: Calibri; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">KERB\_VERIFY\_PAC\_REQUEST </span>request
message \[MS-APDS\] and transmits it to the DC through generic
pass-through mechanism
[\[MS-NRPC\]](http://msdn.microsoft.com/en-us/library/cc237008\(PROT.10\).aspx).
The DC decodes the request and extracts the server checksum and the KDC
checksum values. If the checksum verification succeeds, the DC returns a
success code to the server. An unsuccessful return code indicates that
the PAC has been
altered.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">Performing
PAC validation implies a cost in terms of response time and bandwidth
usage. It requires bandwidth usage to transmit requests and responses
between an application server and the DC. This may cause some
performance issues in high volume application servers
([KB906736](http://support.microsoft.com/kb/906736)). In such
environments, user authentication may induce
additional<span style="mso-spacerun: yes"> </span>network delay and
traffic for carrying PAC validation messages between the server and the
DC.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

## Kerberos PAC validation in Windows

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">In
Windows implementation, the application server derives the authorization
data (PAC) and requests Windows OS to generate an [access
token](http://msdn.microsoft.com/en-us/library/aa374909.aspx)*.*
<span style="mso-spacerun: yes"> </span>The access token encapsulates
account's identity, group memberships, and system privileges, and is
used to make authorization decisions.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Windows
OS uses the identity of the application server to determine when it
needs to validate the PAC. Services that are running as part of the
Windows OS do not perform PAC validation. They carry out server
signature verification on the PAC by using the long-term key that the
KDC shares with the server.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">The
OS sends PAC Validation messages to the DC (through NRPC) when it wants
to ensure the PAC has not been tampered. It involves the DC to validate
the PAC when a non-privileged application attempts to authenticate a
ticket.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Windows
OS sends the PAC validation messages to the NetLogon service of the DC
when the service does not have the TCB privilege and it is not a Service
Control Manager (SCM) service. The Local Security Authority Subsystem
Service (LSASS) process will send PAC validation messages to the DC when
the LSA client (the application server) is not running in the context of
local system, network service, or local service; or it does not have
SeTCBprivilege (Act as part of the operating system).
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

#### <span style="COLOR: black; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">[*User rights: Act as part of the operating system*](http://technet.microsoft.com/en-us/library/dd349804.aspx)</span>

<span style="COLOR: black; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">*This
policy setting determines whether a process can assume the identity of
any user and thereby gain access to the resources that the user is
authorized to access. Typically, only low-level authentication services
require this user right. Potential access is not limited to what is
associated with the user by default. The calling process may request
that arbitrary additional privileges be added to the access token. The
calling process may also build an access token that does not provide a
primary identity for auditing in the system event
logs.*</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Note:
SeTcbPrivilege enables to assign a user account the right to “Act as
Part of the operating system”. Local system, network service and local
service accounts are Windows-defined [Service User
Accounts](http://msdn.microsoft.com/en-us/library/ms686005\(VS.85\).aspx).
Each account has a set of specific privileges attached to
it.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Windows
Server 2003 SP2 has introduced the option for controlling PAC validation
on a server-wide basis for application services that are authenticating
users. The ValidateKdcPacSignature registry key was added to enable or
disable [PAC verification for services
(KB 906736).](http://support.microsoft.com/kb/906736)
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\Kerberos\\Parameters\]</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">“ValidateKdcPacSignature”=dword:00000001
</span>

*<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">When
the value of this entry is 0, Kerberos does not perform PAC validation
on a process that runs as a service. When the value of this entry is 1,
Kerberos performs PAC validation as usual. You have to restart the
computer after you modify this registry entry. When this entry is not
present, the system behaves as if the entry is present and has a value
of 1. The default value in Windows Server 2008 for this entry is 0.
</span>*

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">In
a nutshell, two main conditions prevent PAC validation from occurring in
Windows OS:<span style="mso-spacerun: yes"> 
</span></span>

<span style="mso-bidi-font-family: Calibri; mso-fareast-font-family: Calibri"><span style="mso-list: Ignore">-<span style="FONT: 7pt &#39;Times New Roman&#39;">         
</span></span></span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">the
application has the SeTcbPrivilege privilege (“Act as part of the
operating
system”);</span>

<span style="mso-bidi-font-family: Calibri; mso-fareast-font-family: Calibri"><span style="mso-list: Ignore">-<span style="FONT: 7pt &#39;Times New Roman&#39;">         
</span></span></span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">the
application is a service and the ValidateKdcPacSignature registry key is
set to disable PAC
validation.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

## Example of SMB scenario in Windows

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">The
client tries to access a Windows SMB share requiring Kerberos
authentication. It sends the security token in the KRB\_AP\_REQ during
session establishment. Windows SMB/CIFS server service requests the OS
to create an access token. Now it needs to decide how to perform PAC
validation.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Windows
SMB/CIFS server service runs under Local System Account. In terms of
Kerberos terminology, the SMB/CIFS service represents the application
server. The Local System Account has SeTCBPrivilege ([SE\_TCB\_NAME
privilege
enabled](http://msdn.microsoft.com/en-us/library/ms684190\(VS.85\).aspx)).
As a result, Windows OS will not be sending PAC validation messages to
the DC. Only the server signature in PAC\_SIGNATURE\_DATA will be
checked to determine if the PAC is valid. This verification is done
thanks to the long-term key that the KDC shares with the server.
Typically, when you establish SMB\_SESSION\_SETUP to a Windows member
server, the server signature of the PAC from KRB\_AP\_REQ in the
KerberosToken is
checked.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Note:
You can use the
[<span style="mso-fareast-font-family: &#39;Times New Roman&#39;">Process
Explorer</span>](http://technet.microsoft.com/en-us/sysinternals/bb896653.aspx)
tool to identify the service account of a running process and check if
it has the SeTcbPrivilege.
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

 

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

## Conclusion

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">PAC
validation reinforces a more secure authorization environment in
Kerberos authentication setting where impersonation occurs. From
inter-operability standpoint, an application server that is
inter-operating with a Windows DC needs to decide the criteria upon
which it requires to initiate the exchange of PAC verification messages
with the DC. There is an inherent cost of network delay and bandwidth
usage when PAC validation occurs between the server and the
DC.</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Windows
operating systems use the SeTcbPrivilege privilege or the
ValidateKdcPacSignature registry key to control PAC validation. When
deploying an application, careful assessment is needed before assigning
the SeTcbPrivilege right to an account in order to disable PAC
validation. The caveat is that the “Act as part of the operating system”
(SeTcbPrivilege) right may give excessive privileges to an application
that is impersonating clients. </span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">On
the other hand, the security context of the application – typically the
security context token that includes a service relative identifier –
determines whether the ValidateKdcPacSignature registry key can disable
PAC validation. In a Windows trusted service security context, the PAC
signature is always verified as a minimal
validation.</span>

**<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">References</span>**

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;"></span>

 

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[RFC4120\]
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">[<span style="FONT-SIZE: 11pt">The
Kerberos Network Authentication Service
(V5)</span>](http://www.ietf.org/rfc/rfc4120.txt)</span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[MS-KILE\]
[Kerberos Protocol
Extensions](http://msdn.microsoft.com/en-us/library/cc233855\(PROT.10\).aspx)
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;; mso-fareast-font-family: &#39;Times New Roman&#39;">\[MS-PAC\]
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">[Privilege
Attribute Certificate Data
Structure](http://msdn.microsoft.com/en-us/library/cc237917\(PROT.13\).aspx)
</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[MS-SECO\]
[Windows Security
Overview](http://msdn.microsoft.com/en-us/library/cc246013\(PROT.10\).aspx)</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[MS-APDS\]
</span><span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">[<span style="FONT-SIZE: 11pt">Authentication
Protocol Domain Support
Specification</span>](http://msdn.microsoft.com/en-us/library/cc223948\(PROT.10\).aspx)</span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">\[MS-NRPC\]
[<span style="COLOR: black; TEXT-DECORATION: none; text-underline: none">Netlogon
Remote Protocol
Specification</span>](http://msdn.microsoft.com/en-us/library/cc237008\(PROT.10\).aspx)</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;">Additional
details: Blog of [Tim
Springston](http://blogs.technet.com/ad/archive/2008/09/29/why-won-t-pac-validation-turn-off.aspx).</span>

<span style="FONT-FAMILY: &#39;Times New Roman&#39;,&#39;serif&#39;"></span>
