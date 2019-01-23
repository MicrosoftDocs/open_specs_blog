<div id="page">

# Using Openssl to implement Crypto Operations in Netlogon Remote Protocol

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
10/23/2009 2:29:00
PM

-----

<div id="content">

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>** 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Background<span style="mso-spacerun: yes"> 
</span></span></span>**

The Netlogon Remote Protocol remote procedure call (RPC) interface is
used primarily by Microsoft Windows to maintain the relationship between
a machine and its domain.
<span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span><span style="COLOR: black">In
the protocol, a client delivers a logon request to the domain controller
over an established secure channel between a DC and its
clients.<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes">  </span>Before a secure channel
is established and any method can utilize it, the authentication process
must be completed, as described in 3.1 of
</span>[MS-NRPC](http://msdn.microsoft.com/en-us/library/cc237008\(PROT.10\).aspx)<span style="COLOR: black">.<span style="mso-spacerun: yes">  
</span>The authentication process consists of the following
steps:</span>

<span style="COLOR: black; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span>A session key is negotiated to establish the secure
channel (as specified in section 3.1.4.1) over non-protected RPC, which
is a RPC connection without any underlying security
support.<span style="mso-spacerun: yes">   </span>At the end of this
process, if successful, client and server will mutually verify each
other’s credentials and agree on a session key to be used to secure
further RPC communications.<span style="mso-spacerun: yes">   </span>The
two calculations involved with cryptographic operations
are:<span style="COLOR: black"></span>

<span style="COLOR: black; FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span>Session Key Computation
(3.1.4.3<span style="mso-spacerun: yes"> 
</span>MS-NRPC)<span style="mso-spacerun: yes">  
</span><span style="COLOR: black"></span>

<span style="COLOR: black">The client and the server use the shared
secret (password), client challenge and server challenge to compute the
session key. (For more information, see one of my </span>[previous
blog](http://blogs.msdn.com/openspecification/archive/2009/08/07/stronger-keys-for-netlogon-remote-protocol-in-windows-7.aspx)<span style="COLOR: black">
on this
topic).</span>

<span style="COLOR: black; FONT-FAMILY: &#39;Courier New&#39;; mso-fareast-font-family: &#39;Courier New&#39;"><span style="mso-list: Ignore">o<span style="FONT: 7pt &#39;Times New Roman&#39;">  
</span></span></span><span style="COLOR: black">Netlogon Credential
Computation</span> (3.1.4.4<span style="mso-spacerun: yes"> 
</span>MS-NRPC)<span style="mso-spacerun: yes">  
</span><span style="COLOR: black"></span>

<span style="COLOR: black">The client and the server compute a
credential using the challenge received and session key to compare it
with the credential received.<span style="mso-spacerun: yes"> 
</span></span>

<span style="COLOR: black"></span>

 

<span style="COLOR: black; FONT-FAMILY: Symbol; mso-bidi-font-family: Symbol; mso-fareast-font-family: Symbol"><span style="mso-list: Ignore">·<span style="FONT: 7pt &#39;Times New Roman&#39;">        
</span></span></span><span style="COLOR: black">Once the secure channel
has been established, almost all RPC calls will use Netlogon
Authenticators that are calculated using session key and stored nelogon
credential.<span style="mso-spacerun: yes">  </span>The
</span><span style="COLOR: black">algorithm in 3.1.4.4 is used.</span>

<span style="COLOR: black">Therefore, we can see that session key
computation and Netlogon credential computation are very important
operations in Netlogon remote protocol
implementation.<span style="mso-spacerun: yes">    </span>In the latest
Windows operating systems, <span style="mso-spacerun: yes">  </span>a
stronger encryption (AES) and a hashing algorithm (SHA) are
added.<span style="mso-spacerun: yes">   </span>As a valuable supplement
to the protocol documentation, it could be very helpful to implement the
algorithm using existing cryptographic libraries and provide sample data
for implementers to verify their
implementation.<span style="mso-spacerun: yes">  </span></span>

<span style="COLOR: black">In this blog, we will show how to implement
session key and Netlogon credential computation algorithms using
</span>[OpenSSL](http://www.openssl.org/)<span style="COLOR: black">,
which is a general purpose cryptography open source
library.<span style="mso-spacerun: yes">    </span>Our focus will be on
the implementation when AES and strong key support are negotiated
between the client and the server.<span style="mso-spacerun: yes"> 
</span></span>

<span style="COLOR: black">The code shown can be compiled using gcc
compiler with openssl library
installed.</span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>**

<span style="TEXT-DECORATION: none"></span>

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%"></span></span>** 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Session
Key
Computation</span></span>**<span style="COLOR: black"></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/crypto.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/aes.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/md4.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/md5.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/des.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/hmac.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: blue; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">\#include</span><span style="FONT-SIZE: 9pt; LINE-HEIGHT: 115%; FONT-FAMILY: &#39;Courier New&#39;; mso-no-proof: yes">
<span style="COLOR: #a31515">\<openssl/sha.h\></span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*The
function to compute theMD4<span style="mso-spacerun: yes">  </span>of a
unicode
string</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/</span>

<span style="FONT-SIZE: 9pt; COLOR: blue; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">int</span><span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">
ComputeM4ss(<span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
\*m4ss)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes">       
</span>retVal =
0;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      
</span>pswd\_l;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">char</span><span style="mso-spacerun: yes">      
</span>\*passwdPtr =
globals.password;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">wchar\_t</span><span style="mso-spacerun: yes">   
</span>pswd\_u\[2 \* MAX\_PASSWORD\_LEN +
2\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>mbstate\_t<span style="mso-spacerun: yes"> 
</span>ps;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>MD4\_CTX<span style="mso-spacerun: yes">   
</span>ctx;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: green">/\* Convert the ASCII password to
UNICODE
\*/</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>bzero(pswd\_u,
2\*MAX\_PASSWORD\_LEN+2);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">            
</span><span style="mso-spacerun: yes">    </span>pswd\_l = mbsrtowcs(
pswd\_u, (<span style="COLOR: blue">const</span>
<span style="COLOR: blue">char</span> \*\*)\&passwdPtr,
strlen(globals.password),
\&ps);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">       
</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">            </span><span style="COLOR: blue">if</span>
(pswd\_l \>
0)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">            </span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span>MD4\_Init(\&ctx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span>MD4\_Update(\&ctx,
pswd\_u, pswd\_l \*
<span style="COLOR: blue">sizeof</span>(<span style="COLOR: blue">wchar\_t</span>));</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span>MD4\_Final(m4ss,
\&ctx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span>retVal =
1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes">    </span>}</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">          
</span></span><span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">    </span><span style="COLOR: blue">return</span>
retVal;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">}<span style="COLOR: green"></span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*The
function to compute session key.<span style="mso-spacerun: yes"> 
</span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/</span>

<span style="FONT-SIZE: 9pt; COLOR: blue; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">int</span><span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">
NRPCComputeSessionKey(<span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
\*sessionKey)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes"> 
</span>retVal =
0;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes"> 
</span>zeros =
0;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes"> 
</span>sts;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">long</span>
<span style="COLOR: blue">long</span>
\*temp1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">long</span>
<span style="COLOR: blue">long</span>
\*temp2;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">            
</span><span style="mso-spacerun: yes">    </span><span style="COLOR: blue">long</span>
<span style="COLOR: blue">long</span>
sum;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
m4ss\[16\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
outBuf\[16\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k1\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k2\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
output\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
output2\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>MD5\_CTX<span style="mso-spacerun: yes">    
</span>md5Ctx;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">              </span>SHA256\_CTX<span style="mso-spacerun: yes"> 
</span>shaCtx;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">            
</span><span style="mso-spacerun: yes">    </span>DES\_cblock<span style="mso-spacerun: yes">      
</span>keyStruct;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">             </span>DES\_key\_schedule
keySch;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes">   </span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span>HMAC\_CTX<span style="mso-tab-count: 1">            
</span>hmacCtx;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">             </span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes">        
</span>hmacLen =
16;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">int</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      
</span>pswd\_l;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">char</span><span style="mso-spacerun: yes">      
</span>\*passwdPtr =
globals.password;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">wchar\_t</span><span style="mso-spacerun: yes">   
</span>pswd\_u\[2 \* MAX\_PASSWORD\_LEN +
2\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>mbstate\_t<span style="mso-spacerun: yes"> 
</span>ps;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>bzero(sessionKey,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">/\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>If AES support is negotiaited,
then<span style="mso-spacerun: yes">  </span>SHA will be used to
calculate the
session</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>key.</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*/</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">if</span> (globals.aes &
globals.strongkey)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">          
</span><span style="COLOR: green">/\*
</span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                             
</span>\* Convert the ASCII password to
UNICODE</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">            </span>\* The
password is actually OWF hash of the shared
secret</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                           
</span><span style="mso-spacerun: yes"> </span>\*/</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">           </span>bzero(pswd\_u,
2\*MAX\_PASSWORD\_LEN+2);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">      
</span><span style="mso-spacerun: yes">    </span>pswd\_l = mbsrtowcs(
pswd\_u, (<span style="COLOR: blue">const</span>
<span style="COLOR: blue">char</span> \*\*)\&passwdPtr,
strlen(globals.password),
\&ps);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">    
</span><span style="mso-spacerun: yes">   </span><span style="mso-spacerun: yes">   </span>SHA256\_Init(\&shaCtx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">     
</span><span style="mso-spacerun: yes">     </span>SHA256\_Update(\&shaCtx,
pswd\_u,
pswd\_l\*2);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">        </span>SHA256\_Update(\&shaCtx,
globals.clientChallenge,
MAX\_CHALLENGE\_LEN);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">          
</span>SHA256\_Update(\&shaCtx, globals.serverChallenge,
MAX\_CHALLENGE\_LEN);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">             
</span><span style="mso-spacerun: yes">              </span>SHA256\_Final(sessionKey,
\&shaCtx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">             
</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">        
</span><span style="mso-spacerun: yes">                   </span>retVal
=
1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">/\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>If strong key support is negotiaited,
then<span style="mso-spacerun: yes"> 
</span>MD5<span style="mso-spacerun: yes">  </span>will be used to
calculate the
session</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>key.</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*/</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">else</span>
<span style="COLOR: blue">if</span>
(globals.strongkey)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">             
</span>printf(<span style="COLOR: #a31515">"\\nSession Key: Strong key
computation\\n"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">             
</span><span style="COLOR: blue">if</span>
(ComputeM4ss(m4ss))</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">             
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">         
</span><span style="mso-tab-count: 1">     
</span><span style="mso-spacerun: yes">      </span>sts =
MD5\_Init(\&md5Ctx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      </span>sts =
MD5\_Update(\&md5Ctx, \&zeros,
4);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      </span>sts =
MD5\_Update(\&md5Ctx, globals.clientChallenge,
MAX\_CHALLENGE\_LEN);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      </span>sts =
MD5\_Update(\&md5Ctx, globals.serverChallenge,
MAX\_CHALLENGE\_LEN);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      </span>sts =
MD5\_Final(outBuf,
\&md5Ctx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">    
</span><span style="mso-tab-count: 1">          
</span><span style="mso-spacerun: yes">       </span><span style="mso-spacerun: yes">                </span><span style="mso-spacerun: yes"> </span>HMAC\_CTX\_init(\&hmacCtx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">        
</span><span style="mso-tab-count: 2">                      
</span><span style="mso-spacerun: yes">       </span>HMAC\_Init\_ex(\&hmacCtx,
m4ss, 16, EVP\_md5(),
NULL);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      
</span>HMAC\_Update(\&hmacCtx, outBuf,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      
</span>HMAC\_Final(\&hmacCtx, sessionKey,
\&hmacLen);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">      
</span>HMAC\_CTX\_cleanup(\&hmacCtx);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">       </span>retVal =
1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes"> </span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                
</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                
</span><span style="COLOR: green">/\*</span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>If strong key support is not negotiaited,
then<span style="mso-spacerun: yes"> 
</span>MD4<span style="mso-spacerun: yes">  </span>will be used to
calculate the
session</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*<span style="mso-spacerun: yes">  
</span>key.</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">\*/</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes"> </span><span style="COLOR: blue">else</span>
<span style="COLOR: blue">if</span> (\!globals.strongkey &
\!globals.aes)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">              
</span><span style="mso-spacerun: yes">  </span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">           </span><span style="mso-spacerun: yes"> </span><span style="COLOR: blue">if</span>
(ComputeM4ss(m4ss))</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span><span style="COLOR: green">//
SET sum to ClientChallenge +
ServerChallenge</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span>temp1 =
(<span style="COLOR: blue">long</span>
<span style="COLOR: blue">long</span>
\*)\&globals.clientChallenge;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span>temp2 =
(<span style="COLOR: blue">long</span>
<span style="COLOR: blue">long</span>
\*)\&globals.serverChallenge;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span>sum = \*temp1 +
\*temp2;<span style="mso-spacerun: yes">     
</span><span style="COLOR: green">// TODO: make sure overflow is
ignored.</span></span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">       
</span><span style="mso-tab-count: 1">       
</span><span style="mso-spacerun: yes">    </span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span><span style="COLOR: green">//
SET k1 to lower 7 bytes of the
M4SS</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span>bzero(k1,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span>bcopy(m4ss, k1,
7);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">        
</span><span style="mso-tab-count: 1">      
</span><span style="mso-spacerun: yes">    </span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span><span style="COLOR: green">//
SET k2 to upper 7 bytes of the
M4SS</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span><span style="mso-spacerun: yes"> </span>bzero(k2,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">     </span><span style="mso-spacerun: yes">  </span>bcopy(\&m4ss\[8\],
k2,
7);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">    </span>bcopy(k1, keyStruct,
<span style="COLOR: blue">sizeof</span>(keyStruct));</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">     
</span>DES\_key\_sched(\&keyStruct,
\&keySch);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">      
</span><span style="mso-spacerun: yes">                </span><span style="mso-spacerun: yes"> </span>DES\_ecb\_encrypt((DES\_cblock\*)\&sum,
(DES\_cblock\*)output, \&keySch,
DES\_ENCRYPT);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">  </span><span style="mso-tab-count: 1">         
</span><span style="mso-spacerun: yes">      </span><span style="mso-spacerun: yes"> </span>bcopy(k2,
keyStruct,
<span style="COLOR: blue">sizeof</span>(keyStruct));</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">   </span>DES\_key\_sched(\&keyStruct,
\&keySch);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes">     </span>DES\_ecb\_encrypt((DES\_cblock\*)output,
(DES\_cblock\*)output2, \&keySch,
DES\_ENCRYPT);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">       </span><span style="mso-spacerun: yes">                </span><span style="mso-spacerun: yes"> </span>bzero(sessionKey,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span><span style="mso-spacerun: yes"> </span>bcopy(output2,
sessionKey,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-tab-count: 1">           
</span><span style="mso-spacerun: yes">      </span><span style="mso-spacerun: yes"> </span>retVal
=
1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">          </span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">else</span>
<span style="COLOR: green">/\* Invalid to have just the AES flag set
\*/</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>printf(<span style="COLOR: #a31515">"ERROR - invalid to have just
the AES flag set without the Strong Key
flag.\\n"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">return</span>
retVal;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">}</span><span style="FONT-SIZE: 10pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-bidi-font-size: 9.0pt; mso-no-proof: yes"></span>

<span style="FONT-SIZE: 9pt; COLOR: #a31515; LINE-HEIGHT: 115%; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Examples
of Session Key
Calculations</span></span>**

<span style="FONT-SIZE: 9pt; COLOR: #a31515; LINE-HEIGHT: 115%; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">For
AES support
:</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">     
OWF Password:<span style="mso-spacerun: yes">    </span>13 c0 b0 4b 66
25 0d 08-b8 a3 90 4d cc 8b 34
e3</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">     
Client Challenge: 25 63 e3 5f 69 e1 5a
24</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">     
Server Challenge: 9C 66 5F 90 D9 83 DF 43
</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;">     
Session Key calculated: c9 c7 f7 2f c6 b9 13 e3-67 ae a9 1d 0a e3 a7
70</span>

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes"> 
</span></span>

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes"> </span>For
Strong Key
support:</span>

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"></span>

 

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes">      
</span>OWF Password:
</span><span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">31
a5 90 17 0a 35 1f d5-11 48 b2 a1 0a f2 c3
05</span><span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">
</span>

<span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes">     
</span><span style="mso-spacerun: yes"> </span>Client Challenge:
</span><span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">3a
03 90 a4 6d 0c 3d
4f</span>

<span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes">      
</span></span><span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Server
Challenge:
</span><span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">0c
4c 13 d1 60 41 c8
60</span>

<span style="mso-bidi-font-family: &#39;Courier New&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"><span style="mso-spacerun: yes">      
</span></span><span style="mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri">Session
Key calculated: ee fe 8f 40 00 7a 2e eb<span style="mso-spacerun: yes"> 
</span>68 43 d0 d3 0a 5b e2
e3</span>

<span style="mso-bidi-font-family: &#39;Times New Roman&#39;; mso-ascii-font-family: Calibri; mso-hansi-font-family: Calibri"></span>

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Netlogon
Credential
Computation</span></span>**

<span style="mso-spacerun: yes"></span> 

<span style="FONT-SIZE: 9pt; COLOR: green; LINE-HEIGHT: 115%; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>/\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*
Arguments</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*<span style="mso-spacerun: yes">   
</span>input: Is the client challenge when computing the credential for
the
client.</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*<span style="mso-spacerun: yes">          
</span>It is the server challenge when computing the credential for the
server.</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*<span style="mso-spacerun: yes"> 
</span>Return</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*<span style="mso-spacerun: yes">    
</span>1 =
success</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*<span style="mso-spacerun: yes">    
</span>0 =
error</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes"> </span>\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/</span>

<span style="FONT-SIZE: 9pt; COLOR: blue; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">int</span><span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">
NRPCComputeNetlogonCredentials(
<span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span> \*challenge,
<span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span> \*sessionKey,
<span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span> \*output
)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">         
</span><span style="mso-spacerun: yes">    </span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes">  
</span>retVal =
0;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">          </span><span style="COLOR: blue">int</span><span style="mso-spacerun: yes">  
</span>blockUsed =
0;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">         
</span><span style="mso-spacerun: yes">    </span><span style="mso-spacerun: yes"> </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k1\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k2\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k3\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
k4\[8\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
iv\[16\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">           </span><span style="COLOR: blue">unsigned</span>
<span style="COLOR: blue">char</span>
temp\[16\];</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes">  </span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span>AES\_KEY<span style="mso-spacerun: yes"> 
</span>aesKey;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">          
</span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">   </span>DES\_cblock<span style="mso-spacerun: yes">      
</span>keyStruct;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">            </span><span style="mso-spacerun: yes"> </span>DES\_key\_schedule
keySch;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">              
</span>/\*</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                
</span>\* When AES is negotiated, simply AES encrypt the input with the
session
key</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>\* Note: Currently using an IV of all zeros to cover the
statement:</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">       
</span><span style="mso-tab-count: 1">        </span>\* "AES128 is the
AES128 algorithm in CFB-8 mode without an initialization
vector"</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">               
</span><span style="mso-spacerun: yes"> </span>\*/</span>

<span style="FONT-SIZE: 9pt; COLOR: green; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">                
</span><span style="COLOR: blue">if</span> (globals.aes &
globals.strongkey)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="COLOR: blue">if</span>
(globals.debug)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">   
</span>printf(<span style="COLOR: #a31515">"AES
Computation\\n"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">   
</span>printf(<span style="COLOR: #a31515">"Challenge:\\n"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">    </span>HexDump(challenge,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">   
</span>printf(<span style="COLOR: #a31515">"Session
key:\\n"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="mso-spacerun: yes">    </span>HexDump(sessionKey,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bzero(iv,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">             </span>bzero(temp,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">             </span>bcopy(challenge,
temp,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">             </span><span style="mso-spacerun: yes"> </span>AES\_set\_encrypt\_key(sessionKey,
128,
\&aesKey);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">   
</span><span style="mso-spacerun: yes">            </span>AES\_cfb8\_encrypt(temp,
output, 16, \&aesKey, iv, \&blockUsed,
AES\_ENCRYPT);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="mso-spacerun: yes">  
</span><span style="mso-spacerun: yes">            </span><span style="mso-spacerun: yes"> </span>retVal
=
1;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span><span style="COLOR: blue">else</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="COLOR: green">/\* use DES
\*/</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bzero(k1,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bzero(k2,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bzero(k3,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bzero(k4,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bcopy(sessionKey, k1,
7);<span style="mso-tab-count: 2">                       
</span><span style="COLOR: green">// SET k1 to bytes(0, 6,
Sk)</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>InitLMKey(k1,
k3);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bcopy(\&sessionKey\[7\], k2,
7);<span style="mso-tab-count: 2">                               
</span><span style="COLOR: green">// SET k2 to bytes(7, 13,
Sk)</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>InitLMKey(k2,
k4);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="COLOR: blue">if</span>
(globals.debug)</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>{</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nNRPCComputeNetlogonCredentials()"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nsessionKey"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>HexDump(sessionKey,
16);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nk1"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>HexDump(k1,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nk2"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>HexDump(k2,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nk3"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>HexDump(k3,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>printf(<span style="COLOR: #a31515">"\\nk4"</span>);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 3">                                               
</span>HexDump(k4,
8);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="COLOR: green">// CALL DES\_ECB(Input, k3,
\&output1)</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bcopy(k3, keyStruct,
<span style="COLOR: blue">sizeof</span>(keyStruct));</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>DES\_key\_sched(\&keyStruct,
\&keySch);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>DES\_ecb\_encrypt((DES\_cblock\*)challenge, (DES\_cblock\*)temp,
\&keySch,
DES\_ENCRYPT);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span><span style="COLOR: green">// CALL DES\_ECB(temp, k4,
output)</span></span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>bcopy(k4, keyStruct,
<span style="COLOR: blue">sizeof</span>(keyStruct));</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>DES\_key\_sched(\&keyStruct,
\&keySch);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 2">                               
</span>DES\_ecb\_encrypt((DES\_cblock\*)temp, (DES\_cblock\*)output,
\&keySch,
DES\_ENCRYPT);</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-tab-count: 1">               
</span>}</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"><span style="mso-spacerun: yes">           
</span><span style="mso-spacerun: yes">    </span><span style="COLOR: blue">return</span>
retVal;</span>

<span style="FONT-SIZE: 9pt; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes">}</span>

 

**<span class="underline"><span style="FONT-SIZE: 12pt; COLOR: #0070c0; LINE-HEIGHT: 115%">Examples
of Session Key
Calculations</span></span>**

<span style="FONT-SIZE: 9pt; COLOR: #a31515; LINE-HEIGHT: 115%; mso-bidi-font-family: &#39;Courier New&#39;; mso-no-proof: yes"></span>

 

For AES support
:

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">     
Client Challenge: 25 63 e3 5f 69 e1 5a
24</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin">     
Session Key used: c9 c7 f7 2f c6 b9 13 e3-67 ae a9 1d 0a e3 a7
70</span>

<span style="FONT-SIZE: 11pt; FONT-FAMILY: &#39;Calibri&#39;,&#39;sans-serif&#39;; mso-ascii-theme-font: minor-latin; mso-hansi-theme-font: minor-latin"><span style="mso-spacerun: yes">     
</span>Computed Client Netlogon Credential: 58 6a df 53 ef 72 78
d9</span><span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes">   </span>

<span style="mso-spacerun: yes">      </span>

<span style="mso-spacerun: yes">  </span>For Strong Key support:

<span style="mso-spacerun: yes">      </span>Client Challenge:
<span style="mso-bidi-font-family: &#39;Courier New&#39;">3a 03 90 a4 6d
0c 3d 4f</span>

<span style="mso-spacerun: yes">     
</span><span style="mso-spacerun: yes"> </span>Session Key used: ee fe
8f 40 00 7a 2e eb<span style="mso-spacerun: yes">  </span>68 43 d0 d3 0a
5b e2 e3

<span style="mso-spacerun: yes">       </span>Computed Client Netlogon
Credential:<span style="mso-spacerun: yes">  </span>b6 38 95 82 44 fc ea
cd

 

<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes">  </span>With the code above, we
demonstrated the implementation of NRPC cryptographic operations using
the OpenSSL library.<span style="mso-spacerun: yes">   </span>Also
included are examples of input and output
data.<span style="mso-spacerun: yes"> 
</span><span style="mso-spacerun: yes"> </span>This will assist those in
the implementation of interoperability solutions against Windows
Netlogon RPC interface.

<span style="mso-spacerun: yes">    </span>Thanks to Nick Meier for
providing the source code for this blog.

</div>

</div>
