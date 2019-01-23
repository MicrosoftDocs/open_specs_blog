<div id="page">

# Stronger Keys for Netlogon Remote Protocol in Windows 7

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
8/7/2009 4:17:00
PM

-----

<div id="content">

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Background<span style="mso-spacerun: yes"> 
</span></span></span>**

<span style="mso-spacerun: yes"> </span><span style="COLOR: black">Netlogon
Remote Protocol (MS-NRPC) provides a secure communication between domain
members and domain controllers.<span style="mso-spacerun: yes">  
</span>In the protocol, a client delivers a logon request to the domain
controller over an established secure channel between a DC and
clients.<span style="mso-spacerun: yes">  </span>The secure channel is
achieved by encrypting the communication traffic with a session key
computed using a secret key (machine password) shared by the client
</span><span style="COLOR: #002060">and</span><span style="COLOR: black">
the domain controller.<span style="mso-spacerun: yes">   </span>The
algorithm used for encrypting and signing the communication determines
the strength of security.<span style="mso-spacerun: yes">   </span>In
Windows 7/Windows Servers 2008 R2, there are new changes to enhance the
security strength.<span style="mso-spacerun: yes">  </span></span>

<span style="COLOR: black">In this blog, we will examine these changes
from the protocol layer as well as windows implementation.
</span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">AES
encryption and SHA2
hashing</span></span>**<span style="COLOR: black"></span>

The support for AES encryption and SHA2 hashing is added into Windows
7/Windows 2008 R2.<span style="mso-spacerun: yes">    </span>The
negotiation for the new capability between client and server is done
through the NegotiateFlags parameter in NeteServerAuthenticate2 or
NetrServerAuthenticate3.<span style="mso-spacerun: yes">    </span>There
are two flags related to the session key and Netlogon credential
computation.

 

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">NETLOGON\_SUPPORTS\_AES</span>
(<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">0x01000000)</span>

It is only available in Windows7/Windows 2008
R2.<span style="mso-spacerun: yes">  </span>It is added for indicating
the support of AES Encryption and SHA2 hashing by client and server. By
default, the Windows 7 or Windows server 2008 clients start negotiation
with this bit turned on.<span style="mso-spacerun: yes">   </span>If the
server returns STATUS\_ACCESS\_DENIED, then the client retries
negotiation again with weaker
key.

 

<span style="FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">NETLOGON\_SUPPORTS\_STRONG\_KEY
(0x00004000) </span><span style="mso-spacerun: yes"> </span>

It has existed since Windows 2000.<span style="mso-spacerun: yes"> 
</span>If this bit is enabled, a 128 bit session key, a so called strong
key, is computed using the MD5.<span style="mso-spacerun: yes">  
</span>By default, all Windows initialize this flag to
FALSE.<span style="mso-spacerun: yes">  </span>

 

<span style="mso-spacerun: yes"> </span>The logic to determine the key
strength based on the negotiate flag is as follows.

If NETLOGON\_SUPPORT\_AES is turned on,<span style="mso-spacerun: yes"> 
</span>Then

<span style="mso-spacerun: yes">    
</span><span style="mso-spacerun: yes"> </span>256 bit HMAC-SHA256
session key is computed.

<span style="mso-spacerun: yes">      </span>NetLogon credentials are
computed using AES encryption

If NETLOGON\_SUPPORT\_AES is turned off,
Then<span style="mso-spacerun: yes">  </span>

<span style="mso-spacerun: yes">      </span>NetLogon credentials are
computed using DES encryption

<span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes"> </span>If
NETLOGON\_SUPPORT\_STRONG\_KEY is on

<span style="mso-spacerun: yes">         </span>128 bit MD5 session key
is computed

<span style="mso-spacerun: yes">    </span>If
NETLOGON\_SUPPORT\_STRONG\_KEY is off

<span style="mso-spacerun: yes">           </span>64 bit session key is
computed using<span style="mso-spacerun: yes">  </span>DES encryption
algorithm with ECB mode.

By adding support for AES encryption and SHA hashing, Netlogon
significantly increase the security
strength.

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Setting
RejectMD5Servers</span></span>**

 

This Netlogon client setting is only available in Windows 7/Windows 2008
R2.<span style="mso-spacerun: yes">    </span>If this flag is turned on,
the client will have to reject any server that doesn’t have
<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">NETLOGON\_SUPPORTS\_AES
enabled in the returned NegotiatedFlag parameter from the server, and
the client will not retry the secure channel establishment with
<span style="mso-spacerun: yes"> </span>MD5.
<span style="mso-spacerun: yes">   </span></span>

<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;"></span>

 

<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">This
setting is not negotiated between the client and server, instead , it is
read from the following registry
key:</span>

<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;"></span>

 

*<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">SYSTEM\\\\CurrentControlSet\\\\Services\\\\Netlogon\\RejectMd5Servers</span>*

*Default<span style="mso-spacerun: yes"> 
</span>Value:<span style="mso-spacerun: yes"> 
</span>FALSE*

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>**

<span style="TEXT-DECORATION: none"> </span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Setting
RejectMD5Clients</span></span>**

 

This Netlogon server setting is only available in Windows 7/Windows 2008
R2.<span style="mso-spacerun: yes">   </span>This flag indicates whether
the server MUST reject incoming clients that are using MD5
encryption.<span style="mso-spacerun: yes">   
</span><span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">This
setting is not negotiated between client and server, instead , it is
read from the following registry
key:</span>

<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;"></span>

 

*<span style="mso-no-proof: yes; mso-bidi-font-family: &#39;Courier New&#39;">SYSTEM\\\\CurrentControlSet\\\\Services\\\\Netlogon\\RejectMd5Clients</span>*

*Default<span style="mso-spacerun: yes"> 
</span>Value:<span style="mso-spacerun: yes"> 
</span>FALSE*

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>**

<span style="TEXT-DECORATION: none"> </span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Reference</span></span>**

<span style="FONT-SIZE: 11pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-bidi-font-family: &#39;Times New Roman&#39;; mso-fareast-font-family: Calibri; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-theme-font: minor-bidi; mso-fareast-theme-font: minor-latin; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA">\[MS-RPC\]<span style="mso-spacerun: yes">   
</span><http://msdn.microsoft.com/en-us/library/cc237008(PROT.10).aspx></span>

</div>

</div>
