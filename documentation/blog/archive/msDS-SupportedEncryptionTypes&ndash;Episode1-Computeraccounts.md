<div id="page">

# msDS-SupportedEncryptionTypes – Episode 1 - Computer accounts

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
9/12/2009 3:22:00 AM

-----

<div id="content">

**<span class="underline">Introduction</span>**

In order to be concise with this article, I need to assume that the
reader is familiar with Kerberos and Active Directory.

If not, then I can quickly think of two
scenarios:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>Your favorite search engine
([Bing](http://www.bing.com/) in my case) took you here as a
misunderstanding.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>You came because you stumbled upon the name
“msDS-SupportedEncryptionTypes” somewhere and you really really want
to understand what it is related to, even if you need to learn about
Kerberos and Active Directory as a pre-requisite.

Let’s assume (for the sake of the posting) that the option you fall into
is \#2 and that you are eager to know where to find the docs that are
inherent to this article.

<span class="underline">Here are the links:</span>

The Kerberos Network Authentication Service (V5):
<http://www.ietf.org/rfc/rfc4120.txt>

\[MS-KILE\]: Kerberos Protocol Extensions:
<http://msdn.microsoft.com/en-us/library/cc233855.aspx>

\[MS-ADA2\]: Active Directory Schema Attributes M:
<http://msdn.microsoft.com/en-us/library/cc220154.aspx>

\[MS-ADA3\]: Active Directory Schema Attributes N-Z:
<http://msdn.microsoft.com/en-us/library/cc220699.aspx>

\[MS-ADTS\]: Active Directory Technical Specification:
<http://msdn.microsoft.com/en-us/library/cc223122.aspx>

**<span class="underline"></span>**

<span style="TEXT-DECORATION: none"> </span>

**<span class="underline">Juicy information</span>**

In order for the KDC to be able to generate tickets that the target
server can read, there has to be some mean of communicating what type of
encryptions the involved actors can understand.

For quite a while, that was not an issue because the older versions of
Windows that had Kerberos5 implementations (Windows 2000 all flavors,
Windows XP and Windows 2003 all flavors) only supported [DES
(RFC3961)](http://www.ietf.org/rfc/rfc3961.txt) and [RC4
(RFC4757)](http://www.ietf.org/rfc/rfc4757.txt) as the methods of
encryption.

However, Windows Vista, Windows Server 2008, Windows 7 and Windows
Server 2008 R2 incorporated the newer and more secure algorithm [AES
(RFC3962)](http://www.ietf.org/rfc/rfc3962.txt) (128 AND 256). With this
new addition, and with so many machines running previous versions of
Windows, it was imperative to have a way to inform which algorithms each
particular account could handle and to make sure that when newer
algorithms should become available they would not necessarily represent
many changes.

msDS-SupportedEncryptionTypes came up as the solution. This AD attribute
(defined in
[MS-ADA2](http://msdn.microsoft.com/en-us/library/cc220154.aspx),
section 2.324) is present in the Computer, User and Trust objects for
Schema version 44 (Windows 2008) and later. Its sole purpose is to hold
the values of the encryption types that the account owner supports.

Well, I guess that you could have deducted that from the name of the
attribute so; I better go a little deeper.

Its size is 4 bytes, its type is Unsigned Integer and its format is a
Bit Mask. The values that it can accept, are defined in the following
table (and originally in
[MS-KILE](http://msdn.microsoft.com/en-us/library/cc233855.aspx) section
3.1.5.4):

<span style="mso-bidi-font-size: 9.0pt">  
0</span>

</div>

</div>

<span style="mso-bidi-font-size: 9.0pt">  
1</span>

<span style="mso-bidi-font-size: 9.0pt">  
2</span>

<span style="mso-bidi-font-size: 9.0pt">  
3</span>

<span style="mso-bidi-font-size: 9.0pt">  
4</span>

<span style="mso-bidi-font-size: 9.0pt">  
5</span>

<span style="mso-bidi-font-size: 9.0pt">  
6</span>

<span style="mso-bidi-font-size: 9.0pt">  
7</span>

<span style="mso-bidi-font-size: 9.0pt">  
8</span>

<span style="mso-bidi-font-size: 9.0pt">  
9</span>

<span style="mso-bidi-font-size: 9.0pt">1  
0</span>

<span style="mso-bidi-font-size: 9.0pt">  
1</span>

<span style="mso-bidi-font-size: 9.0pt">  
2</span>

<span style="mso-bidi-font-size: 9.0pt">  
3</span>

<span style="mso-bidi-font-size: 9.0pt">  
4</span>

<span style="mso-bidi-font-size: 9.0pt">  
5</span>

<span style="mso-bidi-font-size: 9.0pt">  
6</span>

<span style="mso-bidi-font-size: 9.0pt">  
7</span>

<span style="mso-bidi-font-size: 9.0pt">  
8</span>

<span style="mso-bidi-font-size: 9.0pt">  
9</span>

<span style="mso-bidi-font-size: 9.0pt">2  
0</span>

<span style="mso-bidi-font-size: 9.0pt">  
1</span>

<span style="mso-bidi-font-size: 9.0pt">  
2</span>

<span style="mso-bidi-font-size: 9.0pt">  
3</span>

<span style="mso-bidi-font-size: 9.0pt">  
4</span>

<span style="mso-bidi-font-size: 9.0pt">  
5</span>

<span style="mso-bidi-font-size: 9.0pt">  
6</span>

<span style="mso-bidi-font-size: 9.0pt">  
7</span>

<span style="mso-bidi-font-size: 9.0pt">  
8</span>

<span style="mso-bidi-font-size: 9.0pt">  
9</span>

<span style="mso-bidi-font-size: 9.0pt">3  
0</span>

<span style="mso-bidi-font-size: 9.0pt">  
1</span>

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

0

E

D

C

B

A

 

Where the bits are defined as:

**<span style="LETTER-SPACING: -0.1pt">Value </span>**

**<span style="LETTER-SPACING: -0.1pt">Description</span>**

A

DES-CBC-CRC

B

DES-CBC-MD5

C

RC4-HMAC

D

AES128-CTS-HMAC-SHA1-96

E

AES256-CTS-HMAC-SHA1-96

 

This is an example of how it looks like in AD (formatted per
[RFC2989](http://www.ietf.org/rfc/rfc2849.txt)):

CN=server2008,CN=Computers,DC=testdomain,DC=com

…

msDS-SupportedEncryptionTypes: 0x1F = ( DES\_CBC\_CRC | DES\_CBC\_MD5 |
RC4\_HMAC\_MD5 | AES128\_CTS\_HMAC\_SHA1\_96 |
AES256\_CTS\_HMAC\_SHA1\_96 );

 

Although this attribute is present in all the computer objects of the
domain regardless of the version of the OS the physical machines have
installed, not all of them are aware of its existence hence, older
versions (2003 and earlier) do not populate it at any time.

Legacy systems leave it Blank, NULL, Zeroed or Empty at all times.
However, more recent versions of the OS do specify a value that could be
any combination of the first 5 bits being 31 (0x1F) the default. The
initial set up of the value happens at domain join time with subsequent
updates via LDAP should the algorithms that the machine supports change
for any reason.

When the KDC checks the attribute to decide what encryption algorithm to
use in order to encrypt the ticket, it could find basically two
scenarios:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>The attribute is
populated

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>The attribute is empty

If the attribute is populated, then the deal is done since the KDC can
determine the best common algorithm to encrypt the ticket with the value
present.

However, if the attribute is empty then the KDC will have to work harder
being the next step to check another attribute. This attribute is
defined in
[MS-ADA3](http://msdn.microsoft.com/en-us/library/cc220699.aspx)
(section
[<span style="COLOR: windowtext; TEXT-DECORATION: none; text-underline: none">2.341</span>](http://blogs.msdn.com/tiny_mce/jscripts/tiny_mce/blank.htm#z01cf3969bc7f42949223f15668557f5f))
and described in
[MS-ADTS](http://msdn.microsoft.com/en-us/library/cc223122.aspx)
(section 2.2.15) and it’s called userAccountControl. This attribute is
also a 4 byte Bit Mask that defines many aspects of the account but the
only one the KDC is interested in is the DK
(<span class="Bold"><span style="FONT-WEIGHT: normal; mso-bidi-font-weight: bold">ADS\_UF\_USE\_DES\_KEY\_ONLY</span></span>
) bit.

This bit defines what legacy encryption method will be
used:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>If the bit is set, then only DES will be
used

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2)<span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span>If the bit is NOT set, then DES and RC4 can be used

This check is especially relevant in domains that have Win7 and Windows
Server 2008 R2 machines joined because those two newer OSs disable their
bit by default so older DES is not an option for them.

 

**<span class="underline">Conclusion</span>**

Windows 2008, Vista, Windows 7 and Windows 2008 R2 have expanded the
options available when securing resources and communications on the
network.

Having the msDS-SupportedEncryptionTypes attribute is a good starting
point to further incorporating newer and more secure encryption
algorithms in the future.
