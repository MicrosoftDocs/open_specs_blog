<div id="page">

# S4U\_DELEGATION\_INFO and Constrained Delegation

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
7/31/2009 2:50:00
PM

-----

<div id="content">

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Background<span style="mso-spacerun: yes"> 
</span></span></span>**

<span style="mso-spacerun: yes"> </span><span style="COLOR: black">The
constrained delegation extension, also called ***S4Uproxy***, is one of
the ***Service for User*** ***(S4U)*** extensions to Kerberos
protocol.<span style="mso-spacerun: yes">   </span>It allows a service
to obtain service tickets to a subset of other services on behalf of the
user.<span style="mso-spacerun: yes">  </span>The service can then
present the tickets to the other service as if the user has obtained the
service ticket and sends it to the back end service
directly.<span style="mso-spacerun: yes">   </span>The details of this
extension are documented in
\[</span>[MS-SFU](http://msdn.microsoft.com/en-us/library/cc246071\(PROT.13\).aspx)<span style="COLOR: black">\].<span style="mso-spacerun: yes"> 
</span></span>

<span style="COLOR: black">S4U\_DELEGATION\_INFO is an optional data
element in PAC, which is embedded in a Kerberos ticket and contains
authorization data.<span style="mso-spacerun: yes">     </span>It shows
the services that have been delegated through for this Kerberos
client.<span style="mso-spacerun: yes">     </span>For the definition of
the structure, see section 2.9 of
\[</span>[MS-PAC](http://msdn.microsoft.com/en-us/library/cc237917\(PROT.10\).aspx)\]<span style="COLOR: black">.<span style="mso-spacerun: yes">   
</span>This structure is mainly used for Kerberos auditing.</span>

<span style="COLOR: black">In this blog, we will examine the entire
process of how this structure is populated and propagated through
Kerberos TGS
exchanges.</span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">S4U\_DELEGATION\_INFO
in S4UProxy Extension</span></span>**

The following diagram shows the process for a service to obtain a
service ticket for another back end service on behalf of the
client.<span style="mso-spacerun: yes">  </span>This can normally happen
when the service is processing the request from the client and it is
found that the service needs to access another service on behalf of the
client in order to complete the
processing.

<span style="mso-spacerun: yes">                  </span><span style="mso-no-proof: yes"> ![](http://tcwz3q.bay.livefilestore.com/y1p-FzVylZ6ckeI-pM1FilcKLM4PVGJBOmHILvNvYO5U451uJy94NSvVdfqnL9DpKp8crZcjT-kK1R_enHlb3pFLav5Uj43oOKQ/Figure1.jpg)</span>

 

The entire process can be divided into three phases for better
illustration.<span style="mso-spacerun: yes">   </span>Please note that
Service A and Service B MUST belong to the same realm.

**Phase 1:**

<span style="mso-spacerun: yes"> </span>Before the service A can send
out TGS request with S4Uproxy extension, it needs to have the following
information on
hand.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>The client’s service ticket to service A

This ticket can be obtained using one of the following
ways

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>The client will send the service ticket to Service
A as a part of AP-REQ to Service A.<span style="mso-spacerun: yes"> 
</span>In this way, client has been authenticated to the Service
A.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>The Service A can request a service ticket to
itself on behalf of the user, using the other S4U extension, S4Uself.

<span style="mso-spacerun: yes">                    </span>The service
ticket must have the forwardable flag
set.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>TGT ticket of Service A

The service A has to use AS-REQ to request a TGT ticket for
itself.<span style="mso-spacerun: yes">   </span>Without TGT ticket,
Service A cannot request any service ticket.

<span style="mso-spacerun: yes">      </span>Within the PAC of the
service ticket, S4U\_DELEGATION \_INFO may or may not already
exist.<span style="mso-spacerun: yes">   </span>If the service ticket is
for the client itself,  then PAC doesn’t contain this
structure.<span style="mso-spacerun: yes">  </span>Otherwise the
structure contains the name of services that the delegation already goes
through.

**Phase 2:**

This is the phase when S4Uproxy extension comes to
action.<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes">  </span>TGS/KDC will process
TGS\_REQ request with S4Uproxy
enabled.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>Server A<span style="mso-spacerun: yes"> 
</span>sends TGS\_REQ with<span style="mso-spacerun: yes">  </span>the
following<span style="mso-spacerun: yes"> 
</span>settings:

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>cname-in-addl-tkt<span style="mso-spacerun: yes"> 
</span>(bit 15) is <span style="mso-spacerun: yes"> </span>enabled in
KdcOptions

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>The service ticket acquired in phase 1 is placed in
additonal-options
field.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Sname<span style="mso-spacerun: yes">  </span>=
name of Service
B

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Realm = realm of Service
B

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>TGS/KDC<span style="mso-spacerun: yes"> 
</span>checks AD attribute *Allowed-to-Delegate-To* of Service A’s
account object to see if Service B is in the list of services to which
Service A can act as a proxy.<span style="mso-spacerun: yes">  </span>If
not, the request is
rejected.

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>TGS creates
<span style="mso-spacerun: yes"> </span>a new service ticket to be
returned (for Service B) with the following
settings:

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>cName,crealm = cName and cRealm in service ticket
in additional-options field(also the same as
the<span style="mso-spacerun: yes">  </span>original service ticket for
Service
A)

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Copy the existing S4U\_DELEGATION\_INFO structure
in PAC in service ticket in additional-options field to the PAC of new
service
ticket.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Set S4U2ProxyTarget of S4U\_DELEGATION\_INFO in new
PAC to Service
B.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Add Service A to the S4U2TransitedService list of
S4U\_DELEGATION\_INFO, and increment TransListSize by
1.

<span style="FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>The new service ticket for Service B is returned to
Service A in KRB\_TGS\_REP message.

Please note only Service A possesses the session key to access the
service ticket, even the service has the cname and crealm of the user.

<span style="mso-spacerun: yes"> </span>**Phase 3:**

Since the Service A has the service ticket for Service B on behalf of
the client user, Service A can request the service from Service B as if
it is from the user directly in Kerberos AP
exchanges.

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">S4U\_DELEGATION\_INFO
with multiple proxies</span></span>**

<span style="mso-spacerun: yes"> </span>The S4UProxy extension could be
used through multiple proxy services.<span style="mso-spacerun: yes">  
</span>The following example shows that Service A request a ticket for
Service B on behalf of the user, then in turn, service B request a
ticket for Service C on behalf of user and so far so
on.

<span style="mso-no-proof: yes"></span>

![](http://tcwz3q.bay.livefilestore.com/y1p-8MnGTpsFiD79GHE-xmxLyihdGx1oIQKBOzrIHC4sFxed1YZnV7ZgvDim0y2QSSaxKS4d9FxmPEKEMSCdF3GfctDGq8L3T-k/Figure2.jpg) 

In the service ticket for Service D that Service C receives from
KDC/TGS,<span style="mso-spacerun: yes">   </span>S4U\_DELEGATION\_INFO
structure in PAC will have the data as follows.

\_S4U\_DELEGATION\_INFO

{<span style="mso-spacerun: yes">             </span>S4U2proxyTarget =
Service D

TransitListSize = 3

S4UTransitedServices = {**Service A, Service B, Service
C**}

}

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">S4U\_DELEGATION\_INFO
and Kerberos Auditing</span></span>**

The constrained delegation is widely used in services such as SQL
server, IIS and SharePoint service.<span style="mso-spacerun: yes">   
</span>Kerberos security auditing for transited services is very
important.<span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">   </span>This is made possible
by the S4U\_DELEGATION\_INFO structure in PAC within service
ticket.<span style="mso-spacerun: yes">    </span>The following is the
information for audit events using S4U\_DELEGATION\_INFO.

**Category :**<span style="mso-spacerun: yes">               
</span>Account Logon

**Event Type:**<span style="mso-spacerun: yes">            
</span>Success Audit (Windows Server2000, 2003 and 2008)

<span style="mso-spacerun: yes">                                 
</span>Failure Audit (Windows
<span style="mso-spacerun: yes"> </span>Server 2003 and 2008)

**Audit Event ID**:<span style="mso-spacerun: yes">      
</span>673<span style="mso-spacerun: yes">    
</span><span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">TGS\_TICKET\_REQUEST</span>

**<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">Event
Info:<span style="mso-spacerun: yes">                           
</span></span>**

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">User
Name</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"><span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>User
Domain</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Service
Name</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Service
ID</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Ticket
Options</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Ticket
Encryption
Type</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Client
Address</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Failure
Code: *<span class="Italic">Kerberos Error
Code</span>*</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Logon
GUID</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>***<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">Transited
Services</span>**<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                           
</span>\<--equivilent to<span style="mso-spacerun: yes"> 
</span>S4U\_DELEGATION\_INFO<span style="mso-spacerun: yes">    </span></span>***

***<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"></span></span>***

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Reference</span></span>**

 

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="mso-spacerun: yes"> </span>Constrained
Delegation<span style="mso-spacerun: yes">  
</span>

<span style="mso-spacerun: yes"> </span><http://technet.microsoft.com/en-us/library/cc739587(WS.10).aspx>

 

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="mso-spacerun: yes"> </span>Microsoft
Kerberos Tech Reference<span style="mso-spacerun: yes"> 
</span><http://technet2.microsoft.com/windowsserver/en/library/b748fb3f-dbf0-4b01-9b22-be14a8b4ae101033.mspx?mfr=true>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">3.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span><span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Segoe UI&#39;"><span style="mso-spacerun: yes"> </span>Document
“Troubleshooting Kerberos Errors”<span style="mso-spacerun: yes"> 
</span></span>[<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Segoe UI&#39;">http://www.microsoft.com/downloads/details.aspx?FamilyID=7DFEB015-6043-47DB-8238-DC7AF89C93F1</span>](http://www.microsoft.com/downloads/details.aspx?FamilyID=7DFEB015-6043-47DB-8238-DC7AF89C93F1)<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Segoe UI&#39;"></span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: &#39;Segoe UI&#39;"></span>

 

 

</div>

</div>
