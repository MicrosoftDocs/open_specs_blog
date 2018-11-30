<div id="page">

# Encryption Negotiation in RDP connection

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
12/7/2011 8:56:00 PM

-----

<div id="content">

**<span style="font-family: Calibri;" data-face="Calibri">Encryption
Negotiation in RDP connection 
</span>**

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">The
RDP connections between clients and servers are normally encrypted for
better security.  When we observe the network traffic to troubleshoot
the RDP related problems, we may notice that the traffic between a
server and client could be encrypted one way (such as client to server)
and unencrypted the other way (such as server to client).  We may also
find that it doesn’t seem possible to turn off the encryption for the
PDUs sent from the client to server.   These questions can be answered
by taking a look at the encryption negotiation process in **MCS**
(MultiplePoint Communication Service) connect request and response. 
  </span></span>

**<span style="font-family: Calibri;" data-face="Calibri">Encryption
Settings in RDP
protocol</span>**

<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">There
are two encryption settings in this process: Encryption Level and
Encryption Method.  The Encryption Level is configured on the server and
sent to client and has four levels as below (5.3.2.1 of
</span></span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">MS-RDPBCGR</span>](http://msdn.microsoft.com/en-us/library/cc240445\(v=prot.10\).aspx)<span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">)
: 
    </span></span>

  -  <span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">**Low**:
    All data sent from the client to the server is protected by
    encryption based on the maximum key strength supported by the
    client*.  This is the only level that the traffic sent by the server
    to client is not
    encrypted.*</span></span>
  - <span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">**Client
    Compatible**: All data sent between the client and the server is
    protected by encryption based on the maximum key strength supported
    by the client.
    </span></span>
  - <span style="font-family: Calibri;" data-face="Calibri"><span style="font-size: small;" size="3">**High**:
    All data sent between the client and server is protected by
    encryption based on the server's maximum key strength.
    </span></span>
  - <span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">**FIPS**:
    All data sent between the client and server is protected using
    Federal Information Processing Standard 140-1 validated encryption
    methods.  </span></span>

The Encryption Method setting is sent from client to server to indicate
what client supports and what server finally selects for the encryption
of the connection. The possible encryption types are (2.2.1.3.3 of
[<span style="color: #0000ff;" color="#0000ff">MS-RDPBCGR</span>](http://msdn.microsoft.com/en-us/library/cc240445\(v=prot.10\).aspx)):

  - 40BIT\_ENCRYPTION\_FLAG 0x00000001        
  - 128BIT\_ENCRYPTION\_FLAG 0x00000002
  - 56BIT\_ENCRYPTION\_FLAG 0x00000008
  - FIPS\_ENCRYPTION\_FLAG 0x00000010

These two settings are closely related when the server sends them to the
client in MCS response.  The encryption method selected by the server
will be based on

  - the Encryption methods selected by the client in MCS Connect Initial
    PDU
  - the Encryption methods supported by the server
  - the Encryption Level configured on the server. This can be
    controlled using group policy setting as documented in
    [<span style="color: #0000ff;" color="#0000ff">http://technet.microsoft.com/en-us/library/cc770833.aspx</span>](http://technet.microsoft.com/en-us/library/cc770833.aspx)

  

**<span style="font-family: Calibri;" data-face="Calibri">Encryption
Negotiation
Packets</span>**

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">        
1.</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> First
clients advertise their cryptographic support in the Client Security
Data sent to the server in MCSConnect Initial as part of the Basic
Exchange Phase of the RDP connection sequence.  The sample packet is
shown as below.       
                                                 </span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">              [![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8132.RDP-1.JPG)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/8132.RDP-1.JPG)
</span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">                
In this case, the client claims that it supports 40 bit and 128 bit RC4
encryption.  The server will use this information to decide what
encryption to use, combined with the server
configuration.</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">         
2.</span>     
<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> The
server then responds with Encryption Level and Encryption Method to be
used.  </span></span>

                            
[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5078.RDP-2.JPG)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/5078.RDP-2.JPG) 

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> 
 In this case, the Encryption Level configured on the server is Low ,
then the server selects the strongest encryption supported by the client
,which is 128 bit RC4 , to protect the traffic sent from client to the
server, but the communication from server to client is unencrypted. 
Therefore the EncryptionMethod set by the server is 128 bit.
</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> 
It seems that theoretically it is possible to get the unencrypted
traffic from client to server if the Encryption Level is set to zero.  
But in order to protect the confidentiality if client-to-server user
data, an RDP server **MUST** set the Encryption Level to be greater than
zero as specified in 5.3.2 of MS-RDPBCGR.  In any non-zero Encryption
Level, client to server traffic will always be encrypted.    
</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3">As
a side note, in order to view the unencrypted traffic ,  we also need to
make sure the compression is turned off.  Please see
</span>[<span style="color: #0000ff; font-family: Calibri; font-size: small;" data-face="Calibri" size="3" color="#0000ff">http://support.microsoft.com/kb/885187</span>](http://support.microsoft.com/kb/885187)<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri">
for
details.</span></span>

<span style="font-size: small;" size="3"><span style="font-family: Calibri;" data-face="Calibri"> Answering
to the questions at the beginning, we can see that when the Encryption
Level on the server is configured to Low, the RDP traffic from client to
server will be encrypted, but the traffic from server to client will be
unencrypted.   The RDP client to server traffic will always be encrypted
since the Encryption Level on a Windows RDP server is always greater
than zero.
</span></span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

<span style="font-family: Calibri; font-size: small;" data-face="Calibri" size="3"> </span>

</div>

</div>
