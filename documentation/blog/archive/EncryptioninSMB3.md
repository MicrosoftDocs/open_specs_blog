<div id="page">

# Encryption in SMB3

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
6/8/2012 2:24:08 PM

-----

<div id="content">

SMB3 will debut in the upcoming version of Windows 8. This is a
significant update from the last version (SMB2.1) and a host of new
features are being introduced in this release. An important one among
them is Encryption. If server and client negotiate SMB3 and the server
is configured for encryption, all SMB packets are encrypted on the wire,
except for NEGOTIATE, SESSION\_SETUP and TREE\_CONNECT (when only share
level encryption is configured) requests and responses.  
This raises quite a few questions; the details of configuration, the
interaction with older versions clients of SMB, the granularity of
configuration etc. The ultimate and official source of answers on this
is the MS-SMB2 document. This document is currently in preview phase and
is available at MS-SMB2-Preview. I will try to answer some of the
questions posed above in a simplified manner in this blog.  
Encryption is offered at two levels; global (session) and share. Global
level encryption is for all the shares that are accessed under an
encrypted session. On the other hand, it is possible to enable
encryption at a share level and encryption will be enforced when the
encrypted share is accessed, when the session in not encrypted.  
To configure global level encryption, set the following parameter using
Power Shell cmdlets that are specifically written for this new version
of
SMB.

  
<span style="font-family: courier new,courier;">Set-SmbServerConfiguration
–EncryptData \<0|1\></span>

  
EncryptData makes sure that server advertises the encryption capability
in the negotiate response. Also, when a session is setup, the value of
Session.EncryptData is set to global EncryptData.  
There is another parameter that has an effect on encryption. It is
called RejectUnencryptedAccess. It is important to understand the
implication of this parameter before changing its default value (TRUE).
RejectUnencryptedAccess is modified by using the following Power Shell
cmdlet:

  
<span style="font-family: courier new,courier;">Set-SmbServerConfiguration
–RejectUnencryptedAccess \<0|1\></span>

  
The default value of EncryptData is FALSE. So, to enable encryption, it
must be set to 1 by using the above power shell cmdlet. Once set to
TRUE, if RejectUnencryptedAccess is not explicitly set to FALSE, the
server will reject session setup with ACCESS\_DENIED if the client does
not support encryption.  
SMB3 also supports share level encryption. In this mode, the SMB payload
is encrypted only if an encrypted share is accessed. A share can be
encrypted by using the following Power Shell
cmdlet:

  
<span style="font-family: courier new,courier; font-size: small;">Set-SmbShare
-Name \<share name\> -EncryptData 1</span>

The following table exhaustively explains the effect of EncryptData and
RejectUnencryptedAcces on a client that does not support
encryption.

<span style="text-decoration: underline;">EncryptData </span>

</div>

</div>

<span style="text-decoration: underline;">RejectUnencryptedAccess </span>

<span style="text-decoration: underline;">Client SMB version \<
3                                                     </span>

<span style="background-color: #c0c0c0;">0                     
</span>

<span style="background-color: #c0c0c0;">0                                              
</span>

<span style="background-color: #c0c0c0;">Client can setup a session and
access encrypted shares     </span>

0

1

Client can setup a session and access unencrypted shares

<span style="background-color: #c0c0c0;">1                    
</span>

<span style="background-color: #c0c0c0;">0                                              
</span>

<span style="background-color: #c0c0c0;">Client can setup a session and
access encrypted share       </span>

1

1

Client cannot setup a session

I would like to draw your attention to gray rows in the table above.
RejectUnencryptedAccess is set to TRUE by default. Before setting it to
zero, a careful thought must be given to the implications. When set to
0, access to encrypted server and shares are allowed for client that do
not support encryption. An SMB3 client will have access in all cases and
will use encryption in the cases where EncryptData is TRUE.  
As the table above describes, EncryptData controls encrypted access to a
server. What if we want to allow access to unencrypted shares but block
access to encrypted share for pre SMB3 clients? Share.EncryptData can be
utilized to achieve that. Obviously, EncryptData must be set to FALSE
since we allow access to server.  
The following table describes the effect of Share.EncryptData on the
allowed access to clients that do not support encryption (SMB version \<
3):

<span style="text-decoration: underline;">Share.EncryptData </span>

<span style="text-decoration: underline;">RejectUnencryptedAccess</span>

<span style="text-decoration: underline;">Client SMB version \<
3             </span>

0

0

Client can access this share

0

1

Client can access this
share

<span style="background-color: #c0c0c0;">1                                
</span>

<span style="background-color: #c0c0c0;">0                                             
</span>

<span style="background-color: #c0c0c0;">Client can access this
share</span>

1

1

Client cannot access this share

So for an encrypted share, if RejectUnencryptedAccess is not explicitly
set to FALSE, the server will reject tree connect command with
ACCESS\_DENIED for clients that do not support encryption.  
I would like to draw your attention to gray row again. Even if a share
is encrypted, if you set RejectUnencryptedAccess to FALSE, a client that
does not support encryption can access the share. It is important that
RejectUnencryptedAccess be left to its default value (TRUE) for thing to
be kosher.
