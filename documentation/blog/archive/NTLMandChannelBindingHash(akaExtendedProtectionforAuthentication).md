<div id="page">

# NTLM and Channel Binding Hash (aka Extended Protection for Authentication)

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
3/26/2013 4:25:00 PM

-----

<div id="content">

Extended Protection for Authnetication (EPA) was introduced in Windows
7/WS2008R2 to thwart reflection attacks. This blog describes the changes
in the implementation of NTLM Authentication that are needed to
successfully authenticate to servers that have EPA enabled. Windows 7/WS
2008R2 and Windows 8/ WS2012 have EPA enabled out of the box.

 You can read the details about EPA here
<http://technet.microsoft.com/en-us/security/advisory/973811>

The concept in EPA is that authentication packets should be bound to the
secure channel on which they are transmitted. This concept is not new
and is known as channel binding ([RFC
5056](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/RFC%205056 "http://www.faqs.org/rfcs/rfc5056.html")).
[RFC
5929](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/RFC%205929 "http://www.faqs.org/rfcs/rfc5929.html")
describes channel bindings for TLS that Winodws uses to bind the secure
channel to authentication. Please note that EPA also uses Service
Pricipal Name (SPN) but it is not used for TLS and we will not discuss
it here.

Let's take an example of [HTTPS](http://tools.ietf.org/html/rfc2818), a
protocol that uses [TLS](http://tools.ietf.org/html/rfc5246).  Once a
secure channel is established and cipher change has happened,
[HTTP](http://tools.ietf.org/html/rfc2616) traffic starts flowing. In
this example, we are only considering services that require
authentication. NTLM or Kerberos will be used if you are using Windows
authentication. You are most likely to use NTLM since the whole point of
using HTTP and TLS is to allow clients to connect over internet (In
Windows 8, Kerberos can be used on the internet but we will concentrate
on NTLM here). In case of Windows client, these are the steps that are
taken to incorporate channel binding in the authentication process after
secure channel has been established:

1\. The hashing algorithm for the signature in the certificate is
identified, if present.

2\. SSPI calculates a hash (almost always
[SHA256](http://csrc.nist.gov/publications/fips/fips180-3/fips180-3_final.pdf)
hash, see below for details/exceptions) of the certificate, appends
other data relevent to the type of channel bindings and returns it to
the application.

3\. The application, at the receipt of channel bindings, calls
Initialize security context (ISC) and passes channel bindings as a
parameter to the method call.

4\. SSPI calculates the MD5 hash of channel bindings and uses it in  the
calculation of [NTLM
version 2](http://msdn.microsoft.com/en-us/library/cc236621.aspx) response.

5\. When server recives authenticate message, it queries SSPI for
channel bindings. SSPI does exactly the same thing as on the client side
and returns the data to the service. The service includes it in the call
to method Accept Security Context (ASC)

6\. In the process of verifying authenticate message, SSPI also takes
into account the channel bindings. It calculates the MD5 hash of the
channel bindings that were provided by the application (service) and
compares it to the one sent by the client. If they match and rest of the
authentication requirements are met, authentication is successful.

I'll now elaborate on each of the step listed above with a concrete
example of RPC-over-HTTP traffic. The TLS network traffic is encrypted
and I used Network Monitor expert Network Monitor Decryption Expert
([NmDecrypt](http://blogs.msdn.com/controlpanel/blogs/posteditor.aspx/NmDecrypt "http://nmdecrypt.codeplex.com/"))
to decrypt it. The decrypted network trace is attached to this blog.

If you open the network trace in Network Monitor, you'll see that in
frame 16 server sends a certificate to client, as below (copied and
pasted from the
trace):

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c13e0 
30 82 02 09 30 82 01 76-a0 03 02 01 02
</span><span style="font-family: courier new,courier;">02 10 cb 
0...0..v........</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c13f0 
69 79 cd 51 75 c5 b7 4b-67 30 83 6c 78
</span><span style="font-family: courier new,courier;">44 27 30 
iy.Qu..Kg0.lxD'0</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1400 
09 06 05 2b 0e 03 02 1d-05 00 30 16 31
</span><span style="font-family: courier new,courier;">14 30 12 
...+......0.1.0.</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1410 
06 03 55 04 03 13 0b 44-43 2d 57 53 32
</span><span style="font-family: courier new,courier;">30 30 38 
..U....DC-WS2008</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1420 
52 32 30 1e 17 0d 31 32-31 31 31 37 30
</span><span style="font-family: courier new,courier;">30 35 39 
R20...1211170059</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1430 
32 31 5a 17 0d 33 39 31-32 33 31 32 33
</span><span style="font-family: courier new,courier;">35 39 35 
21Z..39123123595</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1440 
39 5a 30 16 31 14 30 12-06 03 55 04 03
</span><span style="font-family: courier new,courier;">13 0b 44 
9Z0.1.0...U....D</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1450 
43 2d 57 53 32 30 30 38-52 32 30 81 9f
</span><span style="font-family: courier new,courier;">30 0d 06 
C-WS2008R20..0..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1460 
09 2a 86 48 86 f7 0d 01-01 01 05 00 03
</span><span style="font-family: courier new,courier;">81 8d 00 
.\*.H............</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1470 
30 81 89 02 81 81 00 9b-00 f8 1a 2d 37
</span><span style="font-family: courier new,courier;">c6 8d a1 
0..........-7...</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1480 
39 91 46 f3 6a 1b f9 60-6c b3 6c a0 ac
</span><span style="font-family: courier new,courier;">ed 85 e0 
9.F.j..\`l.l.....</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1490 
3f dc 92 86 36 bd 64 bf-36 51 db 57 3a
</span><span style="font-family: courier new,courier;">8a 82 6b 
?...6.d.6Q.W:..k</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14a0 
d8 94 17 7b d3 91 11 98-ef 19 06 52 30
</span><span style="font-family: courier new,courier;">03 73 67 
...{.......R0.sg</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14b0 
c8 ed 8e fa 0b 3d 4c c9-10 63 9f cf b4
</span><span style="font-family: courier new,courier;">cf 39 d8 
.....=L..c....9.</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14c0 
fe 99 eb 5b 11 f2 fc fa-86 24 d9 ff d9
</span><span style="font-family: courier new,courier;">19 f5 69 
...\[.....$.....i</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14d0 
b4 df 5a 5a c4 94 b4 b0-07 25 97 13 ad
</span><span style="font-family: courier new,courier;">7e 38 14 
..ZZ.....%...\~8.</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14e0 
fb d6 33 65 6f e6 f7 48-4b 2d b3 51 2e
</span><span style="font-family: courier new,courier;">6d c7 ea 
..3eo..HK-.Q.m..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c14f0 
11 76 9a 2b f0 00 4d 02-03 01 00 01 a3
</span><span style="font-family: courier new,courier;">60 30 5e 
.v.+..M......\`0^</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1500 
30 13 06 03 55 1d 25 04-0c 30 0a 06 08
</span><span style="font-family: courier new,courier;">2b 06 01 
0...U.%..0...+..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1510 
05 05 07 03 01 30 47 06-03 55 1d 01 04
</span><span style="font-family: courier new,courier;">40 30 3e 
<.....0G..U...@0>\></span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1520 
80 10 eb 65 26 03 95 4b-d6 c0 54 75 78
</span><span style="font-family: courier new,courier;">7c b6 2a 
...e&..K..Tux|.\*</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1530 
a1 bb a1 18 30 16 31 14-30 12 06 03 55
</span><span style="font-family: courier new,courier;">04 03 13 
....0.1.0...U...</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1540 
0b 44 43 2d 57 53 32 30-30 38 52 32 82
</span><span style="font-family: courier new,courier;">10 cb 69 
.DC-WS2008R2...i</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1550 
79 cd 51 75 c5 b7 4b 67-30 83 6c 78 44
</span><span style="font-family: courier new,courier;">27 30 09 
y.Qu..Kg0.lxD'0.</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1560 
06 05 2b 0e 03 02 1d 05-00 03 81 81 00
</span><span style="font-family: courier new,courier;">7b fa fe 
..+..........{..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1570 
ee 74 05 ac bb 79 e9 da-ca 00 44 96 94
</span><span style="font-family: courier new,courier;">71 92 b1 
.t...y....D..q..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1580 
db c9 9b 71 29 c0 e4 28-5e 6a 50 99 cd
</span><span style="font-family: courier new,courier;">a8 17 e4 
...q)..(^jP.....</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c1590 
56 b9 ef 7f 02 7d 96 a3-48 14 72 75 2f
</span><span style="font-family: courier new,courier;">b0 b5 87 
V....}..H.ru/...</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c15a0 
ee 55 e9 6a 6d 28 3c c1-fd 00 e4 76 e3
</span><span style="font-family: courier new,courier;">80 88 78 
.U.jm(\<....v...x</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c15b0 
26 0d 6c 8c b8 64 61 63-b7 13 3a ab c7
</span><span style="font-family: courier new,courier;">dd 1d 0a 
&.l..dac..:.....</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c15c0 
d7 15 45 a1 d6 d9 34 c7-21 48 fb 43 87
</span><span style="font-family: courier new,courier;">38 da 1f 
..E...4.\!H.C.8..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c15d0 
50 47 b1 a5 5c 47 ed 04-44 97 d3 ac 74
</span><span style="font-family: courier new,courier;">2d eb 09 
PG..\\G..D...t-..</span></span>

<span style="font-size: x-small;"><span style="font-family: courier new,courier;">00000000\`095c15e0 
77 59 bf a3 54 5b de 42-d5 23 5a 71
</span><span style="font-family: courier new,courier;">9f          
wY..T\[.B.\#Zq..</span></span>

 

After secure channel is established and cipher change has taken place,
HTTP traffic starts flowing.

In this example, HTTP is being used as a transport for RPC and RPC
server requires authentication. For authentication, the client
application first calculates the channel binding by using the following
process(in Windows this is done by SSPI but that is not important in
this discussion ). This process is based on RFC 5929.

1\. The channel binding type for this example is "tls-server-end-point"
since a certificate is used in handshake (RFC5929).

2\. The client calculates a hash of the certificate. The hashing
algorithm is SHA-256, unless all of the following conditions are met, in
which case the signature algorithm in the certificate will be used.

  - A certificate signature algorithm exist
  - The algorithm is only implemented in
    [CNG](http://msdn.microsoft.com/en-us/library/windows/desktop/aa376210\(v=vs.85\).aspx)
    ([ALG\_ID](http://msdn.microsoft.com/en-us/library/windows/desktop/aa375549\(v=vs.85\).aspx)
    is CALG\_OID\_INFO\_CNG\_ONLY)
  - The algorithm has a corresponding CNG algorithm identifier string
    ([pwszCNGAlgid](http://msdn.microsoft.com/en-us/library/windows/desktop/aa381435\(v=vs.85\).aspx))
  - The algorithm is not [SHA1](http://tools.ietf.org/html/rfc3174)
  - The algorithm is not [MD5](http://tools.ietf.org/html/rfc1321)

3\. The SHA-256 hash of the above certificate is: ea 05 fe fe cc 6b 0b
d5 71 db bc 5b aa 3e d4 53 86 d0 44 68 35 f7 b7 4c 85 62 1b 99 83 47 5f
95

4\. The Channel binding unique prefix (RFC5929) "tls-server-end-point"
is prefixed to the hash above (with a colon), resulting in  

74 6c 73 2d 73 65 72 76 65 72 2d 65 6e 64 2d 70 6f 69 6e 74 3a ea 05 fe
fe cc 6b 0b d5 71 db bc 5b aa 3e d4 53 86 d0 44 68 35 f7 b7 4c 85 62 1b
99 83 47 5f 95

5\. The above value is inserted as the value of application\_data field
of gss\_channel\_bindings\_struct structure, as pointed out by
[MS-NLMP](http://msdn.microsoft.com/en-us/library/cc236621.aspx) section
2.2.2.1

6\. Windows always sets the other fields of
gss\_channel\_bindings\_struct as zeros
(<span style="line-height: 115%; font-family: &#39;Calibri&#39;,&#39;sans-serif&#39;; font-size: 11pt; mso-fareast-font-family: Calibri; mso-ansi-language: EN-US; mso-fareast-language: EN-US; mso-bidi-language: AR-SA; mso-ascii-theme-font: minor-latin; mso-fareast-theme-font: minor-latin; mso-hansi-theme-font: minor-latin; mso-bidi-font-family: Arial; mso-bidi-theme-font: minor-bidi;">[SEC\_CHANNEL\_BINDINGS  
structure](http://msdn.microsoft.com/en-us/library/windows/desktop/dd919963\(v=vs.85\).aspx)</span>).
The resulting gss\_channel\_bindings\_struct is as follows (little
endian format):

<span style="font-family: courier new,courier;">00 00 00 00
//initiator\_addtype</span>  
<span style="font-family: courier new,courier;">00 00 00 00
//initiator\_address length</span>  
<span style="font-family: courier new,courier;">00 00 00
00 //acceptor\_addrtype</span>  
<span style="font-family: courier new,courier;">00 00 00
00 //acceptor\_address length</span>  
<span style="font-family: courier new,courier;">35 00 00 00
//application\_data length (53 bytes)</span>  
<span style="font-family: courier new,courier;">74 6c 73 2d
//application data, as calculated above  
</span><span style="font-family: courier new,courier;">73 65 72 76  
65 72 2d 65  
6e 64 2d 70  </span>  
<span style="font-family: courier new,courier;">6f 69 6e 74  
3a ea 05 fe  
fe cc 6b 0b  
d5 71 db bc  </span>  
<span style="font-family: courier new,courier;">5b aa 3e d4  
53 86 d0 44  
68 35 f7 b7  
4c 85 62 1b  </span>  
<span style="font-family: courier new,courier;">99 83 47 5f  
95</span>

 

After calculating channel binding, the client application starts
authentication and include channel binding as part of authentication. In
case of NTLM, the gss\_channel\_bindings\_struct  is called
ClientChannelBindingUnhashed (MS-NLMP section 3.1.1.2). As explained in
MS-NLMP section 3.1.5.1.2, the client adds an AV\_PAIR structure and set
the **AvId** field to MsvAvChannelBindings and the **Value** field to
MD5(ClientChannelBindingsUnhashed). The MD5 hash of the above
gss\_channel\_bindings\_struct  turns out to be:

65 86 E9 9D 81 C2 FC 98 4E 47 17 2F D4 DD 03 10

This value is part of the AUTHENTICATE\_MESSAGE in frame 27 in the
network trace attached (in the network trace it is shown in Base64
encoding as 45 41 42 6C 68 75 6D 64 67 63 4C 38 6D 45 35 48 46 79 2F 55
33 51 4D 51 with AvLen) .

When server receives the AUTHENTICATE\_MESSAGE, in addition to the
regular authentication processing, it also verifies the channel binding
hash by calculating it the same way the client did. If the channel
binding hash does not match, the authentication will not be successful.
The subsequent behavior is server dependent. In this example (IIS), the
server will stop communication on unsuccessful authentication.

Please note that two step hashing is being employed here. First the
application creates a hash of the certificate which becomes a part of
gss\_channel\_bindings\_struct structure. This structure is MD5 hashed
again to be included in AUTHENTICATE\_MESSAGE.

There are configurations on both Windows client and server side to
disable the EPA. For the server side, please consult the server specific
documentation. As for the server in this example, IIS, please consult 
<http://www.iis.net/configreference/system.webserver/security/authentication/windowsauthentication/extendedprotection>.

On the client side, there is a registry setting that is described in
KB976918 (<http://support.microsoft.com/kb/976918>) that can be used to
configure
EPA.

 

 

[ChannelBindingTraceDecrypted.cap](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/10/40/55/58/ChannelBindingTraceDecrypted.cap)

</div>

</div>
