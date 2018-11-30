<div id="page">

# To KVNO or not to KVNO, what is the version\!?

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
11/13/2009 11:36:00
AM

-----

<div id="content">

<span style="font-size: small;"><span style="font-family: Calibri;"></span></span> 

<span style="font-size: small;"><span style="font-family: Calibri;">Shakespeare
knew nothing about Kerberos V5…
Nothing\!<span style="mso-spacerun: yes">  </span></span></span>

<span style="font-family: Calibri; font-size: small;">But, I still like
him\! And that, despite the fact that he had the audacity to paraphrase
me in his play “Hamlet”. Of course no one believes me\! </span>

<span style="font-family: Calibri; font-size: small;">I must admit it
would be much easier to convince you about this historic truth if I had
been born about \~400 years BEFORE him.</span>

<span style="font-family: Calibri; font-size: small;">But, oh well…
</span>

<span style="font-family: Calibri; font-size: small;">What I CAN
probably convince you about is today’s topic… KVNO is not always as
decisive as it’s thought to be.</span>

<span style="font-family: Calibri; font-size: small;">Aha\! I bet you
did not see THAT one
coming\!</span>

<span style="font-size: small;"><span style="font-family: Calibri;">First
things first: **“What does KVNO stand for?”**</span></span>

<span style="font-family: Calibri; font-size: small;">KVNO stands for:
Key Version Number.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Ok,
good. Now: **“What key are we talking about?”**</span></span>

<span style="font-family: Calibri; font-size: small;">Of course we are
talking about the Client’s Secret Key, what did you expect?</span>

<span style="font-family: Calibri; font-size: small;">Each machine on
the network possesses a Long Term Key (Secret Key) that is used to
authenticate with the KDC in order to obtain tickets and to encrypt
those tickets when sending them within the
AP\_REQ.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Before
you ask: **“What’s an AP\_REQ?”**</span></span>

<span style="font-family: Calibri; font-size: small;">The AP\_REQ is the
initial message that the client machine sends to the Server on the
network in order to request Kerberos authentication and gain access to a
specific Service on that Server. That service could be SMB, LDP or any
service that the server has registered with the KDC and has a
SPN.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">Ok,
now we need to understand: **“What is the KVNO field
for?”**</span></span>

<span style="font-family: Calibri; font-size: small;">The KVNO is a
field on the AP\_REQ that indicates what version of the key has been
used to encrypt the service ticket.</span>

<span style="font-family: Calibri; font-size: small;">This is where the
KVNO is located when looking into the
AP\_REQ:</span>

<span style="mso-no-proof: yes"><span style="font-family: Calibri; font-size: small;">![](images/4807.pictureblogkvno.jpg)</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">And, this is the
description of the KVNO found
in</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">
RFC4120</span>](http://www.ietf.org/rfc/rfc4120.txt)<span style="font-family: Calibri; font-size: small;">:</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">  
</span>Key Version Number
(kvno)</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">     
</span>A tag associated with encrypted data identifies which key was
used</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">     
</span>for encryption when a long-lived key associated with a
principal</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">     
</span>changes over time.<span style="mso-spacerun: yes">  </span>It is
used during the transition to a new
key</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">     
</span>so that the party decrypting a message can tell whether the
data</span>

<span style="FONT-FAMILY: &#39;Courier New&#39;; FONT-SIZE: 10pt; mso-fareast-font-family: &#39;Times New Roman&#39;"><span style="mso-spacerun: yes">     
</span>was encrypted with the old or the new key.</span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">Now, if there’s a
KVNO field, it is safe to assume that there could be different versions
of the key.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">That
leads to the next question: **“When and how is the long term key
changed?”**</span></span>

<span style="font-family: Calibri; font-size: small;">The key is
generated by an algorithm that derives it from the account
password.</span>

<span style="font-family: Calibri; font-size: small;">With account I’m
referring to the Active Directory object that represents the client
computer. That account has a password just like any user account but,
the one big difference is that the password for this account is not
known or set by any user. Instead it’s by default automatically reset
every 30 days by the computer itself.</span>

<span style="font-family: Calibri; font-size: small;">Now, as we stated
before, the key is derived from the password so… yes, you are right, the
key has to change every time that the password changes. As you might
have already figured out, the KVNO is incremented by 1 when this
happens.</span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
big question now is: **“What would be a scenario where the key used to
encrypt the ticket is different from the one that the server has
stored?”**</span></span>

<span style="font-family: Calibri; font-size: small;">Well, we can be
facing this scenario in the following
situation:</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">1)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">A
Client obtains a valid ticket from the KDC (this ticket is kept and used
until expired) and used the
KVNO=5</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">2)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">The
client then renews its password (if default, 30 days have passed since
last
reset)</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">3)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">KVNO
will increase to 6 and change is picked up by the target
server</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">4)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">Ticket
is still valid since it has not expired nor the machine has been
rebooted or the cache
purged</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">5)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">Machine
tries to access service on target server with ticket encrypted with
KVNO=5</span>

<span style="font-family: Calibri; font-size: small;">This scenario is
fairly usual and it’s covered by the
</span>[<span style="color: #0000ff;"><span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes"> </span>RFC4120</span></span></span>](http://www.ietf.org/rfc/rfc4120.txt)<span style="font-family: Calibri; font-size: small;">
as
    well:</span>

    The ticket is decrypted using the version of the server's key specified by the ticket.  If the decryption routines detect a modification of the ticket (each encryption system MUST provide safeguards to detect modified ciphertext), the KRB_AP_ERR_BAD_INTEGRITY error is returned (chances are good that different keys were used to encrypt and decrypt).

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">I
bet you are thinking: **Ooohhhh\!\!\! So that’s when KVNO saves the day
right\!?**</span></span>

<span style="font-family: Calibri; font-size: small;">Well, yes and
no.</span>

<span style="font-family: Calibri; font-size: small;">Yes, it can be
used for that. No, Windows does not pay attention to
KVNO.</span>

<span style="font-family: Calibri;"><span style="font-size: small;"><span style="mso-spacerun: yes"> </span>It
simply ignores it. Like if KVNO wasn’t even present
(</span><span style="LINE-HEIGHT: 115%; FONT-SIZE: 8pt">Editor’s Note:
Any similarities with the life of the author are pure
coincidence</span><span style="font-size: small;">)</span></span>

<span style="font-family: Calibri; font-size: small;">Windows server
will follow these
steps:</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">1)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">It
will try to decrypt the service ticket in the AP\_REQ with the current
key</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">2)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">If
it succeeds, it then sends the AP\_REP to the client and the process
moves
forward</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">3)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">But
if it fails, it will then make its best effort and try to decrypt the
ticket with the previous version of the key
(KVNO-1)</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">4)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">If
it succeeds, AP\_REP and process moves
forward</span>

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore"><span style="font-family: Calibri; font-size: small;">5)</span><span style="FONT: 7pt &#39;Times New Roman&#39;">     
</span></span></span><span style="font-family: Calibri; font-size: small;">If
it fails, it will fail the AP\_REQ and send an AP\_REP with
<span style="mso-spacerun: yes"> </span>KRB\_AP\_ERR\_MODIFIED</span>

<span style="font-size: small;"><span style="font-family: Calibri;">I
can read your mind; I know you are eager to say: **I got it\! This is
very interesting information\! I should visit this blog more
often\!**</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Well,
you will be even more surprised if I say to you: **But wait\! There’s
more\! If you read the next few lines in the next 30 seconds, you will
also receive a wonderful CAVEAT\!**</span></span>

<span style="font-family: Calibri; font-size: small;">And you reached
this line so… here’s the caveat.</span>

<span style="font-family: Calibri; font-size: small;">Not always,
Windows has the chance to store the previous key (KVNO-1) and make its
best effort to decrypt a ticket that was encrypted with an older
key.</span>

<span style="font-family: Calibri; font-size: small;">That will depend
on a simple requirement:</span>

<span style="font-family: Calibri; font-size: small;">In order for the
server to store the previous version of a key, the password change for
the computer account must have been done on that particular
server.</span>

<span style="font-family: Calibri; font-size: small;">What I mean with
this is that the server that received the request and that processed the
password change, saves the old password and can use it as the KVNO-1
key. The rest of the servers, do not have a KVNO-1 available and will
fail the request with <span style="mso-spacerun: yes"> </span>AP\_REP
with <span style="mso-spacerun: yes"> </span>KRB\_AP\_ERR\_MODIFIED
after trying with the current key.</span>

<span style="font-family: Calibri; font-size: small;">When the client
receives a <span style="mso-spacerun: yes"> </span>AP\_REP with
<span style="mso-spacerun: yes"> </span>KRB\_AP\_ERR\_MODIFIED, purges
its tickets cache and requests a new ticket to the KDC. This time, the
ticket will be encrypted with the current version of the key and the
exchange would
succeed.</span>

**<span style="font-size: small;"><span style="font-family: Calibri;">One
last juicy perk for having resisted your desire to close the browser and
run away from
me\!</span></span>**

<span style="font-size: small;"><span style="font-family: Calibri;"><span style="mso-spacerun: yes"> </span>A
script to change the computer password on demand and thus be able to
test all this details\!</span></span>

<span style="font-family: Calibri; font-size: small;">Or did you think
that it was going to be as simple as right clicking somewhere?</span>

<span style="font-family: Calibri; font-size: small;">Run this on the
domain controller where you want to change the account
password:</span>

*<span style="COLOR: red"><span style="font-size: small;"><span style="font-family: Calibri;">Dim
objComputer</span></span></span>*

*<span style="COLOR: red"><span style="font-size: small;"><span style="font-family: Calibri;">Set
objComputer =
GetObject("LDAP://CN=computername,CN=computers,DC=yourdomain,DC=yoursuffix")</span></span></span>*

*<span style="COLOR: red"><span style="font-size: small;"><span style="font-family: Calibri;">objComputer.SetPassword
"P@ssw0rd1"</span></span></span>*

*<span style="COLOR: red"><span style="font-size: small;"><span style="font-family: Calibri;">Wscript.Quit</span></span></span>*

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">Hope you found
this information useful\!</span>

<span style="font-family: Calibri; font-size: small;">Hasta
luego\!</span>

<span style="font-family: Calibri; font-size: small;"> </span>

</div>

</div>
