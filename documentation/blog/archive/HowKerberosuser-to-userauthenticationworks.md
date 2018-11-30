<div id="page">

# How Kerberos user-to-user authentication works?

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
5/24/2017 7:17:36 PM

-----

<div id="content">

<div>

The Kerberos user-to-user (U2U) authentication mechanism enables a
client to authenticate to a service that is not in possession of the
long-term secret key. U2U allows one principal to authenticate using a
ticket encrypted with the session key from a TGT issued to another
principal. This article discusses the messages involved in the
mechanism. It illustrates various scenarios and applicability settings.
It finally provides samples of packet exchanges.

</div>

<div>

</div>

<div>

Table of Contents

</div>

<div>

Preamble 2

Glossary 2

Refresher: Basic Kerberos authentication 2

</div>

<div style="padding-left: 30px">

The Authentication Service Exchange 2

The Ticket-Granting Service Exchange 3

The Client / Application Server Exchange 3

</div>

<div>

User-to-User Authentication Exchange 3

</div>

<div style="padding-left: 30px">

User-to-User Mechanism 3

User-to-User GSS-API messages 4

User-to-User tickets are not cached 5

</div>

<div>

How is U2U authentication initiated? 5

</div>

<div style="padding-left: 30px">

Deliberate client awareness 5

U2U via KDC policy 5

U2U via server policy 5

</div>

<div>

U2U illustrations 5

</div>

<div style="padding-left: 30px">

Deliberate Kerberos U2U from client: CredSSP in Remote Desktop Session 5

Kerberos U2U: Enabling SSO with NTLM for smart card sign-in 6

Kerberos U2U via KDC policy: Offering Remote Assistance 6

</div>

<div>

Conclusion 7

</div>

<div>

Appendix 1. Related Kerberos Error codes 7

Appendix 2. Steps of standard CredSSP’s Kerberos U2U from client 8

Appendix 3. Steps of Kerberos U2U via KDC policy in Offering Remote
Assistance 24

References 48

</div>

<div>

# <span>Preamble</span>

This article is not about Public Key Cryptography Based User-to-User
(PKU2U). PKU2U is used in homegroup environments to authenticate peers
by leveraging local certificates between peer computers.

This article is not about Service for User and Constrained Delegation,
both referred to as S4U \[MS-SFU\]. S4U enables an application service
to obtain a Kerberos service ticket on behalf of a user (S4U2Self,
S4U2Proxy).

We assume the reader is familiar with Kerberos authentication
\[RFC4120\] and GSS-API concepts \[RFC2743\]. We start the article with
a terse overview of basic Kerberos authentication before elaborating on
U2U.

# <span>Glossary</span>

API – Application Program Interface

AP – Application server

AS – Authentication service

GSS – Generic Security Service

KDC - Key distribution center

PAC – Privilege Account Certificate

SPN - Service Principal Name

SPNEGO - Simple and Protected GSSAPI Negotiation Mechanism

TGT - Ticket granting ticket

TGS - Ticket granting service

U2U – User-to-user

</div>

<div>

# <span>Refresher: Basic Kerberos authentication</span>

In its classic form, the Kerberos protocol supports authentication
between a client and an application server. It consists of three main
message exchanges.

## <span></span>The Authentication Service Exchange

The AS exchange consists of KRB\_AS\_REQ and KRB\_AS\_REP. In the
KRB\_AS\_REQ, the client requests a TGT from the KDC’s Authentication
Service (AS). In the KRB\_AS\_REP, the AS returns the TGT, which is the
user's initial ticket meant only for use by the TGS. The TGT includes a
session key that the client can use to encrypt communication with the
TGS.

The TGT is encrypted with a key only known by the KDC (the KDC key is
shared by all KDCs in the realm). Thus, the TGT is opaque to all
entities except the KDC itself. The TGT has a short validity time
(typically 10 hours).

The encrypted part of AS exchange uses the key that the client shares
with the KDC.

## <span></span>The Ticket-Granting Service Exchange

The TGS exchange involves KRB\_TGS\_REQ and KRB\_TGS\_REP. The client
requests the TGS to issue a service ticket for the target service. In
the KRB\_TGS\_REQ, the client presents the TGT, an authenticator, and
identifies the target service by its SPN.

The TGS validates the TGT and the authenticator. It then issues a
service ticket. It encrypts the ticket with the long-term key that is
shared by the KDC and the target service. The ticket includes the
client's identity and a copy of the session key. The TGS encrypts the
client's copy of the session key with the key that the KDC shares with
the client.

## <span></span>The Client / Application Server Exchange

To authenticate to the server, the client composes KRB\_AP\_REQ and
sends the service ticket along with a new authenticator to the target
server. The server decrypts the ticket with its long-term key and
validates the authenticator. In Windows, the service also creates an
access token based on the user’s PAC.

Optionally, the client might have requested mutual authentication with
the target server. If so, the target server retrieves the client’s
timestamp from the authenticator, encrypts it with the session key, and
returns it to the client as part of the KRB\_AP\_REP.

</div>

<div>

# <span>User-to-User Authentication Exchange</span>

U2U authentication provides a method to perform authentication when the
verifier does not have access to the service’s long-term key. The basic
form of Kerberos authentication is not suitable for peer-to-peer
authentication. The U2U variant of Kerberos enables authentication of
such peers. The idea is to obtain a TGS ticket encrypted with the
session key from another TGT issued to another user, instead of the
long-term server key that would have not be accessible in the acceptor’s
context.

The following sections summarize the user-to-user authentication
sequence.

</div>

## <span>User-to-User Mechanism</span>

<div>

The OID for user-to-user mechanism is:

1.2.840.113554.1.2.2.3

</div>

<div>

It is a child OID of GSSAPI Kerberos OID 1.2.840.113554.1.2.2.

</div>

<div>

The U2U mech-type extends the conventional Kerberos GSS-API protocol and
adds a round trip with the server to retrieve the TGT from the service.

</div>

## <span>User-to-User GSS-API messages</span>

<div>

In the GSS-API encapsulated message exchange for U2U Kerberos, the
request token has an innerContextToken with a 2-octet TOK\_ID field
containing 04 00 (KERB-TGT-REQUEST) followed by the message:

KERB-TGT-REQUEST ::= SEQUENCE {

pvno\[0\]                         INTEGER,

msg-type\[1\]                     INTEGER,

server-name\[2\]                  PrincipalName OPTIONAL,

realm\[3\]                        Realm OPTIONAL

All fields are defined in the Kerberos RFC4120 and informational RFC
draft for U2U \[U2UDraft\].

msg-type is KRB\_TGT\_REQ (16).

</div>

<div>

The response message is a KERB\_TGT\_REPLY token which has an
innerContextToken with a 2-octet TOK\_ID field containing 04 01
(KERB-TGT-REPLY) followed by the message as follows:

KERB-TGT-REPLY ::= SEQUENCE {

pvno\[0\]                         INTEGER,

msg-type\[1\]                     INTEGER,

ticket\[2\]                       Ticket

}

msg-type is KRB\_TGT\_REP (17). The ticket contains the TGT for the
service.

</div>

<div>

Upon receipt of the service TGT, the client initiator can now request
the KDC to issue a service ticket by using a KERB-TGS-REQ with the
KDC-Options ENC-TKT-IN-SKEY and the service TGT in the
additional-tickets of the KDC-REQ-BODY.

</div>

<div>

Because ENC-TKT-IN-SKEY option is present in the KRB\_KDC\_REQ, the
Sname may be absent. However, the Sname for U2U ticket is typically set
to the principal name (i.e. user name) whose TGT the server supplied to
the client. If Sname is absent, the KDC will use the name from the
client in the additional ticket.

</div>

<div>

The TGS honors the request and issues the ticket (KERB-TGS-REP). It
encrypts the ticket with the session key of the TGT in the additional
ticket.

</div>

<div>

The client finally continues with the AP exchange. In the KERB-AP-REQ,
the client sets USE-SESSION-KEY flag in the AP-Options to indicate to
the server that U2U authentication is in use, and that the service
ticket is encrypted in the session key from the server's TGT rather than
in the server's long-term key.

</div>

<div>

## <span></span>User-to-User tickets are not cached

</div>

<div>

U2U service tickets are not cacheable. A U2U ticket is only usable to
one instance of a client application. On the other hand, to cache a
ticket, it must be valid to all instances of the service that share that
SPN. Multiple authentications that share a cacheable service ticket will
likewise share a session key.

</div>

<div>

How is U2U authentication initiated?

</div>

<div>

The Kerberos protocol does not specify the client’s trigger to U2U
authentication. It is in the application protocol sequence that the
client obtains the server’s TGT. Once this pre-requisite is fulfilled,
the client can initiate user-to-user authentication as previously
described.

U2U is often used in cases where SPNEGO starts Kerberos and then fails
either with KDC\_ERR\_MUST\_USE\_USER2USER from the KDC or with
KRB\_AP\_ERR\_USER\_TO\_USER\_REQUIRED from the server.

</div>

<div>

## <span></span>Deliberate client awareness

</div>

<div>

When the client is aware that the application server requires
user-to-user authentication, it will explicitly request U2U mechanism in
the first call to GSS\_Init\_Sec\_Context.

</div>

## U2U via KDC policy

<div>

The KDC may implement a policy on user accounts to reject conventional
service ticket requests. When a KERB\_TGS\_REQ that does not include a
second ticket with an ENC\_TKT\_IN\_SKEY KDC Option, the KDC will
respond with KDC\_ERR\_MUST\_USE\_USER2USER (0x1B).

</div>

## U2U via server policy

<div>

When a client sends a conventional KRB\_AP\_REQ and the service requires
U2U authentication, the server will respond with
KRB\_AP\_ERR\_USER\_TO\_USER\_REQUIRED (0x45). The server may provide
its TGT in the data of the error
message.

</div>

# U2U illustrations

## Deliberate Kerberos U2U from client: CredSSP in Remote Desktop Session

<div>

For Kerberos under CredSSP, the client chooses to do U2U on its own
without a prior error about U2U being required. The client initiates U2U
because CredSSP calls InitializeSecurityContext with the
ISC\_REQ\_USE\_SESSION\_KEY flag. That flag requires a new session key,
which Kerberos honors by doing U2U.

When one observes network packets of Windows-based CredSPP, the Kerberos
exchanges start with a conventional TGS ticket request, which is
unexpected. This conventional TGS exchange could be explained by the
fact that the code that forces to transition to U2U is emulating the
reception of the user-to-user required error, and thus after the point
where we get the normal ticket.  That initial TGS preceding the first
CredSSP TSRequest is a useless ticket retrieval. Indeed, that ticket is
not presented in the TSRequest. Instead the client deliberately decides
to send a U2U inside an SPNEGO token of the TSRequest.

CredSSP resorts to U2U mechanism because U2U service tickets are not
cacheable. When CredSSP uses U2U, it gets a unique session key to
encrypt credentials and this remains private to the process instance.
Had that connection used Kerberos without U2U, then the session key used
to protect the credentials would have been available to any process
running as the user that shares the user’s ticket cache.

Note that the SPNEGO token of the first CSSP TSRequest has
UserToUserMechanism in the MechToken = InitialContextToken{ThisMech:
UserToUserMechanism (1.2.840.113554.1.2.2.3) . . .}

Even though the MechTypes list contains:

MechTypes: {MsKerberosToken (1.2.840.48018.1.2.2); KerberosToken
(1.2.840.113554.1.2.2); Negoex (1.3.6.1.4.1.311.2.2.30); NLMP
(1.3.6.1.4.1.311.2.2.10)}

Here, the MechType is not really being negotiated.

In the appendix, we provide illustrative packets of the exchange. The
outer TLS session of the CredSSP packets has been decrypted.

</div>

## Kerberos U2U: Enabling SSO with NTLM for smart card sign-in

<div>

Kerberos U2U helps to provide single-sign-on (SSO) with NTLM for a smart
card sign-in. When the user logs in a client with a smartcard, Kerberos
U2U enables SSO to network services that require NTLM. One such scenario
is a remote desktop connection to a non-domain-joined server where the
client needs to delegate its credentials to the server which is not
capable of Kerberos authentication.

The client uses Kerberos U2U to retrieve the NTOWF hash of the user’s
password. It starts with a PKINIT exchange of KERB\_AS\_REQ /
KERB\_AS\_REP with AS-PK-REQ / AS-PK-REP as specified in \[MS-PKCA\]. By
using the PIN to unlock the smartcard, the client reads the principal
name and certificate, then signs the AS-REQ which the KDC will validate
before issuing the TGT.

The client then retrieves a U2U TGS with the user name as Sname. In the
U2U TGS’s KDC-REQ-BODY, the client uses the ENC-TKT-IN-SKEY option and
supplies its TGT in the additional tickets. This allows to decrypt the
resulting PAC (with the session key) and get the NTOWF in
NTLM\_SUPPLEMENTAL\_CREDENTIAL.

The encrypted NTOWF is part of the PAC and thus gets propagated to
service tickets that the user acquires, but only the system to which the
user logs on interactively can decrypt it. This allows a user that logs
on with a smartcard to acquire credentials necessary to access resources
that require NTLM.

Note that the encrypted NTOWF in the PAC is not due to U2U. The client
uses Kerberos U2U because a U2U ticket is un-cacheable. The session key
remains privy to that process instance. When another authentication is
needed, there is a separate ticket request.

</div>

## Kerberos U2U via KDC policy: Offering Remote Assistance

<div>

This example illustrates U2U authentication with the trigger via KDC
policy. It uses the scenario of an Expert providing an unsolicited
Remote Assistance (RA) to a Novice. The Expert initiates the RA offer
with “msra /offerRA \<Novice-FQDN\>” command.

To establish the RA session to the host identified by the Novice-FQDN,
the Expert needs to obtain a Kerberos ticket to the user account of the
Novice.

First, the Expert’s computer tries to obtain a Kerberos ticket for the
user account on the Novice's computer. The user account does not have
any service principal name (SPN) that is registered. Thus, the KDC
service on the DC returns the error KDC\_S\_PRINCIPAL\_UNKNOWN. The
extended error information conveys the client should fall back to U2U
authentication (the KDC reply indicates KDC\_ERR\_MUST\_USE\_USER2USER
in its extended error information).

The Expert computer sends the U2U token to the Novice’s computer and
obtain the user’s TGT. In this scenario, this U2U token exchange happens
as part of the MSRPC Bind because the RA session goes over DCOM (MSRPC).

The Expert computer uses an SPN with "RestrictedKrbHost" service class
because it has the server name but not the identity of the service. This
SPN is meant to authenticate with the server and not the service.

Afterwards, it proceeds U2U authentication exchange, as previously
described.

In the appendix, we show an example of parsed packets of the exchange.

</div>

# Conclusion

<div>

In this article, we discussed how Kerberos user-to-user authentication
works. It is often an unspoken extension to Kerberos. We described the
message exchange and provided examples of applications. The appendixes
provide samples of U2U authentication exchanges.

</div>

# Appendix 1. Related Kerberos Error codes

<div>

Error code, Error macro, Description

</div>

<div>

0x6, KDC\_ERR\_C\_PRINCIPAL\_UNKNOWN, Client not found in Kerberos
database

0x7, KDC\_ERR\_S\_PRINCIPAL\_UNKNOWN, Server not found in Kerberos
database

0x1B, KDC\_ERR\_MUST\_USE\_USER2USER, Server principal valid for
user2user only

0x45, KRB\_AP\_ERR\_USER\_TO\_USER\_REQUIRED, User-to-user authorization
is required

A network packet capture generally shows the error codes in KERB-ERROR
responses. To further diagnose, the Kerberos Event log can be useful,
notably when there is extended error information.

For diagnostic purpose, you can set the following registry to enable
Kerberos events. This should not be left enabled on live production,
otherwise it will fill up unnecessary disk
space.

https://support.microsoft.com/en-us/help/262177/how-to-enable-kerberos-event-logging

HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\Kerberos\\Parameters

Registry Value: LogLevel

Value Type: REG\_DWORD

Value Data: 0x1

</div>

# Appendix 2. Steps of standard CredSSP’s Kerberos U2U from client

<div>

This client’s TGT will be used later during U2U TGS.

</div>

<div>

KerberosV5 KerberosV5:AS Request Cname: Administrator Realm: contoso4
Sname: krbtgt/contoso4

KerberosV5 KerberosV5:KRB\_ERROR  - KDC\_ERR\_PREAUTH\_REQUIRED (25)

KerberosV5 KerberosV5:AS Request Cname: Administrator Realm: contoso4
Sname: krbtgt/contoso4

KerberosV5 KerberosV5:AS Response Ticket\[Realm: CONTOSO4.COM, Sname:
krbtgt/CONTOSO4.COM\]

</div>

<div>

</div>

<div>

The following TGS ticket (to Sname: TERMSRV/sut01tp4.contoso4.com) will
not be used, although it is retrieved.

</div>

<div>

</div>

<div>

KerberosV5 KerberosV5:TGS Request Realm: CONTOSO4.COM Sname:
TERMSRV/sut01tp4.contoso4.com

KerberosV5 KerberosV5:TGS Response Cname: Administrator

</div>

<div>

The client initiates U2U.

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag1:

\- NegoTokens:

\+ SequenceOfHeader:

\+ SequenceHeader:

\+ Tag0:

\+ AsnOctetStringHeader:

\- NegoToken:

\- InitialContextToken:

\+ ApplicationHeader:

\+ ThisMech: SpnegoToken (1.3.6.1.5.5.2)

\- InnerContextToken: 0x1

\- SpnegoToken: 0x1

\+ ChoiceTag:

\- NegTokenInit:

\+ SequenceHeader:

\+ Tag0:

\- MechTypes: Prefer MsKerberosToken (1.2.840.48018.1.2.2)

\+ SequenceHeader:

\+ MechType: MsKerberosToken (1.2.840.48018.1.2.2)

\+ MechType: KerberosToken (1.2.840.113554.1.2.2)

\+ MechType: Negoex (1.3.6.1.4.1.311.2.2.30)

\+ MechType: NLMP (1.3.6.1.4.1.311.2.2.10)

\+ Tag2:

\+ OctetStringHeader:

\- MechToken: unused (0)

\- MsKerberosToken: unused (0)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: unused (0)

\- KerberosToken: unused (0)

TokId: 0x400

\- UsertoUserSessionReq: KRB\_TGT\_REQ (0x10)

\+ SequenceHeader:

\+ Tag0:

\+ pvno: 5

\+ Tag1:

\+ MessageType: 16

\+ Tag2:

\+ ServerName: TERMSRV/sut01tp4.contoso4.com

</div>

<div>

the client receives the TGT form the server.

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag1:

\- NegoTokens:

\+ SequenceOfHeader:

\+ SequenceHeader:

\+ Tag0:

\+ AsnOctetStringHeader:

\- NegoToken:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag1:

\+ SupportedMech: MsKerberosToken (1.2.840.48018.1.2.2)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: unused (0)

\- MsKerberosToken: unused (0)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: unused (0)

\- KerberosToken: unused (0)

TokId: 0x401

\- UsertoUserSessionRep: KRB\_TGT\_REP (0x11)

\+ SequenceHeader:

\+ Tag0:

\+ pvno: 5

\+ Tag1:

\+ MessageType: 17

\+ Tag2:

\- Ticket: Realm: CONTOSO4.COM, Sname: krbtgt/CONTOSO4.COM

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO4.COM

\+ Tag2: 0x1

\+ Sname: krbtgt/CONTOSO4.COM

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag1:

\+ KvNo: 2

\+ Tag2:

\+ Cipher:
h:Ì°a---------------12Bz"?Uâ\*sä‘:ñ‹?ç;†·8µFnDê1@È\\wÐV8–ÄÔ÷uÊdÈvqÙÜ0
---------------/¤\<Ï¼»¢þkÎgÈmíXeðoôfòWòùÝ?$5.---------------´äŒ^´ÛÝïcJõ@ûdY’.®?\`Ä×53+iw˜

¿ÞFv´\&q'\`™Íü…01W

PÎø.ºOç3|+g\`"CRaØ\]\#\\ÊñÞ(xªþ›•â¸ôå±¡MŠå.05HÓö“¶ZÆQ†·&-------------------------------------------------------------¾}7^a•çúÅ£Ù.´sÕL†ÖãR1---------------œ{íºÄ/?)\\"
Lò\[áqœU

</div>

<div>

</div>

<div>

The client requests service ticket for U2U authentication.

</div>

<div>

KerberosV5 KerberosV5:TGS Request Realm: CONTOSO4.COM Sname:
TERMSRV/sut01tp4.contoso4.com

</div>

<div>

</div>

<div>

\- Kerberos: TGS Request Realm: CONTOSO4.COM Sname:
TERMSRV/sut01tp4.contoso4.com

\+ Length: Length = 2674

\- TgsReq: Kerberos TGS Request

\+ ApplicationTag:

\- KdcReq: KRB\_TGS\_REQ (12)

\+ SequenceHeader:

\+ Tag1:

\+ Pvno: 5

\+ Tag2:

\+ MsgType: KRB\_TGS\_REQ (12)

\+ Tag3:

\- PaData:

\+ SequenceOfHeader:

\- PaData: PA-TGS-REQ (1)

\+ SequenceHeader:

\+ Tag1:

\+ PaDataType: PA-TGS-REQ (1)

\+ Tag2:

\+ OctetStringHeader:

\- KrbApReq: KRB\_AP\_REQ (14)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REQ (14)

\+ Tag2: 0x1

\+ ApOptions:

\+ Tag3:

\- Ticket: Realm: CONTOSO4.COM, Sname: krbtgt/CONTOSO4.COM

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO4.COM

\+ Tag2: 0x1

\+ Sname: krbtgt/CONTOSO4.COM

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag1:

\+ KvNo: 2

\+ Tag2:

\+ Cipher: ¯++ä¨OQòåc
¦¹ÿÑãIªã\<»Xðë.\`¾bÌÎØI?N¦ÛIXzCXaº´Æ}±?­Õ6

›D£ði­ñËÕ?ÉeÕ?J0W¬šÚDÚù ¸aŠNxDÉ»’ÓQŠh øË˜öÕ¡¼wÊ?$æŒªya}|^líà@—ÎëÚ÷.\\ê¯z™Ÿðîö¢N‡Ù†¥¦2l Q?Ñ=úG²R¨Mj/lÕ¾­éŒL\<?—·…¸Ñ=?ãšF;ùÿ\\\\¸Ìr“ "

ôägùÖ°k…ÐÈÜG.Qƒ¼»IÐG¼9˜‚yDÿx«åÿ\[+H=Ç\\ýmù›QÐ

\+ Tag4:

\+ Authenticator:

\+ PaData: PA-PAC-OPTIONS (167)

\+ Tag4:

\- ReqBody:

\+ SequenceHeader:

\+ Tag0:

\- KdcOptions: 0x40810008

\+ KerberosFlagsHeader:

\+ Padding:

\- KrbFlags: 0x40810008

Reserved:              (0...............................)

Forwardable:           (.1..............................) Ticket to be
issued is to have its FORWARDABLE flag set

Forwarded:             (..0.............................) This is not a
request for forwarding

Proxiable:             (...0............................) Ticket to be
issued is not to have its PROXIABLE flag set

Proxy:                 (....0...........................) This is not a
request for a proxy

AllowPostDate:         (.....0..........................) Ticket to be
issued is not to have its MAY\_POSTDATE flag set

PostDated:             (......0.........................) This is not a
request for a postdated ticket

Unused7:               (.......0........................)

Renewable:             (........1.......................) Ticket to be
issued is to have its RENEWABLE flag set

Unused9:               (.........0......................)

Unused10:              (..........0.....................)

OptHardwareAuth:       (...........0....................)

Unused12:              (............0...................)

Unused13:              (.............0..................)

CnameInAddlTkt:        (..............0.................) This is not a
request for S4U2proxy functionality

Canonicalize:          (...............1................)

Unused16:              (................0000000000......)

DisableTransitedCheck: (..........................0.....) Checking of
the transited field is enabled

RenewableOk:           (...........................0....) Renewable
ticket is not acceptable

EncTktInSkey:          (............................1...) Indicates that
the ticket for the end server is to be encrypted in the session key from
the additional TGT provided

Unused29:              (.............................0..)

Renew:                 (..............................0.) Present
request is not for a renewal

Validate:              (...............................0) Request is not
to validate a postdated ticket

\+ Tag2: 0x1

\+ Realm: CONTOSO4.COM

\+ Tag3:

\+ Sname: TERMSRV/sut01tp4.contoso4.com

\+ Tag5: 0x1

\+ Till: 09/13/2037 02:48:05 UTC

\+ Tag7:

\+ Nonce: 794422766 (0x2F59EDEE)

\+ Tag8:

\+ Etype:

\+ TagA:

\+ EncAuthorizationData:

\+ TagB:

\- AdditionalTickets:

\+ SequenceOfHeader:

\- Ticket: Realm: CONTOSO4.COM, Sname: krbtgt/CONTOSO4.COM

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO4.COM

\+ Tag2: 0x1

\+ Sname: krbtgt/CONTOSO4.COM

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag1:

\+ KvNo: 2

\+ Tag2:

\+ Cipher:
h:Ì°a---------------12Bz"?Uâ\*sä‘:ñ‹?ç;†·8µFnDê1@È\\wÐV8–ÄÔ÷uÊdÈvqÙÜ0
---------------/¤\<Ï¼»¢þkÎgÈmíXeðoôfòWòùÝ?$5.---------------´äŒ^´ÛÝïcJõ@ûdY’.®?\`Ä×53+iw˜

¿ÞFv´\&q'\`™Íü…01W

PÎø.ºOç3|+g\`"CRaØ\]\#\\ÊñÞ(xªþ›•â¸ôå±¡MŠå.05HÓö“¶ZÆQ†·&-------------------------------------------------------------¾}7^a•çúÅ£Ù.´sÕL†ÖãR1---------------œ{íºÄ/?)\\"
Lò\[áqœU

</div>

<div>

</div>

<div>

KerberosV5 KerberosV5:TGS Response Cname: Administrator

</div>

<div>

\- Kerberos: TGS Response Cname: Administrator

\+ Length: Length = 1638

\- TgsRep: Kerberos TGS Response

\+ ApplicationTag:

\- KdcRep: KRB\_TGS\_REP (13)

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_TGS\_REP (13)

\+ Tag3:

\+ Crealm: CONTOSO4.COM

\+ Tag4:

\+ Cname: Administrator

\+ Tag5:

\- Ticket: Realm: CONTOSO4.COM, Sname: TERMSRV/sut01tp4.contoso4.com

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO4.COM

\+ Tag2: 0x1

\+ Sname: TERMSRV/sut01tp4.contoso4.com

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag2:

\+ Cipher:
­\[¼ˆÀfÙ÷\&v.'æÿÒ\~”´Ñ95D³3›Ø‹éæD@OÆæÆþ@œ?¹oÙÁ}&\&MáÛ\`? ‘oh¼õ?Îýƒíˆû---------------÷68”öR®˜ÏïV‹?ö5¾\_ú\>´I4˜

?ØDÏ‡Éš&”ù«yaøýª·mv?ïa%ß À

\+ Tag6:

\+ EncPart:

</div>

<div>

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag1:

\- NegoTokens:

\+ SequenceOfHeader:

\+ SequenceHeader:

\+ Tag0:

\+ AsnOctetStringHeader:

\- NegoToken:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: unused (0)

\- KerberosToken: unused (0)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: unused (0)

\- KerberosToken: unused (0)

TokId: Krb5ApReq (0x100)

\- ApReq: KRB\_AP\_REQ (14)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REQ (14)

\+ Tag2: 0x1

\- ApOptions:

\+ KerberosFlagsHeader:

\+ Padding:

\- KrbFlags: 0x60000000

Reserved:       (0...............................)

UseSessionKey:  (.1..............................)

MutualRequired: (..1.............................)

Unused:         (...00000000000000000000000000000)

\+ Tag3:

\- Ticket: Realm: CONTOSO4.COM, Sname: TERMSRV/sut01tp4.contoso4.com

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO4.COM

\+ Tag2: 0x1

\+ Sname: TERMSRV/sut01tp4.contoso4.com

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag2:

\+ Cipher:
­\[¼ˆÀfÙ÷\&v.'æÿÒ\~”´Ñ95D³3›Ø‹éæD@OÆæÆþ@œ?¹oÙÁ}&\&MáÛ\`? ‘oh¼õ?Îýƒíˆû---------------÷68”öR®˜ÏïV‹?ö5¾\_ú\>´I4˜

?ØDÏ‡Éš&”ù«yaøýª·mv?ïa%ß À

\+ Tag4:

\+ Authenticator:

</div>

<div>

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag1:

\- NegoTokens:

\+ SequenceOfHeader:

\+ SequenceHeader:

\+ Tag0:

\+ AsnOctetStringHeader:

\- NegoToken:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: KRB\_AP\_REP (15)

\- KerberosToken: KRB\_AP\_REP (15)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: KRB\_AP\_REP (15)

\- KerberosToken: KRB\_AP\_REP (15)

TokId: Krb5ApRep (0x200)

\+ ApRep: KRB\_AP\_REP (15)

\+ Tag3:

\+ OctetStringHeader:

\+ MechListMic: KRB\_AP\_REP (15)

</div>

<div>

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag1:

\- NegoTokens:

\+ SequenceOfHeader:

\+ SequenceHeader:

\+ Tag0:

\+ AsnOctetStringHeader:

\- NegoToken:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-completed (0)

\+ Tag3:

\+ OctetStringHeader:

\- MechListMic: unused (0)

\- KerberosToken: unused (0)

TokId: GSS\_GetMIC (0x404)

\- Krb5GssV2GetMic:

\+ Flags: 4 (0x4)

Filler: Binary Large Object (5 Bytes)

SndSeq: 792999904 (0x2F4437E0)

SgnCksum: Binary Large Object (218 Bytes)

</div>

<div>

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag3:

\+ PubKeyAuth: Encrypted

</div>

<div>

CSSP CredSSP:TSRequest, Version: 3

</div>

<div>

  TLSSSLData: Transport Layer Security (TLS) Payload Data

\+ TLS: TLS Rec Layer-1 SSL Application Data

\- Cssp: CredSSP

\- TsRequest:

\+ SequenceHeader:

\+ Tag0:

\+ Version: 3

\+ Tag2:

\+ AuthInfo:
Encrypted

</div>

# Appendix 3. Steps of Kerberos U2U via KDC policy in Offering Remote Assistance

<div>

KerberosV5:AS Request Cname: myuser01 Realm: CONTOSO.COM Sname:
krbtgt/CONTOSO.COM

KerberosV5:KRB\_ERROR  - KDC\_ERR\_PREAUTH\_REQUIRED (25)

KerberosV5:AS Request Cname: myuser01 Realm: CONTOSO.COM Sname:
krbtgt/CONTOSO.COM

KerberosV5:AS Response Ticket\[Realm: CONTOSO.COM, Sname:
krbtgt/CONTOSO.COM\]

KerberosV5:TGS Request Realm: CONTOSO.COM Sname:
RestrictedKrbHost/sut2016rs1.contoso.com

KerberosV5:TGS Response Cname: myuser01

. . .

KerberosV5:TGS Request Realm: CONTOSO.COM Sname: krbtgt/CONTOSO

KerberosV5:TGS Response Cname: myuser01

</div>

<div>

KerberosV5:TGS Request Realm: CONTOSO.COM Sname: Administrator

</div>

<div>

 - Kerberos: TGS Request Realm: CONTOSO.COM Sname: Administrator

\+ Length: Length = 1542

\- TgsReq: Kerberos TGS Request

\+ ApplicationTag:

\- KdcReq: KRB\_TGS\_REQ (12)

\+ SequenceHeader:

\+ Tag1:

\+ Pvno: 5

\+ Tag2:

\+ MsgType: KRB\_TGS\_REQ (12)

\+ Tag3:

\- PaData:

\+ SequenceOfHeader:

\- PaData: PA-TGS-REQ (1)

\+ SequenceHeader:

\+ Tag1:

\+ PaDataType: PA-TGS-REQ (1)

\+ Tag2:

\+ OctetStringHeader:

\- KrbApReq: KRB\_AP\_REQ (14)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REQ (14)

\+ Tag2: 0x1

\+ ApOptions:

\+ Tag3:

\+ Ticket: Realm: CONTOSO.COM, Sname: krbtgt/CONTOSO.COM

\+ Tag4:

\+ Authenticator:

\+ Tag4:

\- ReqBody:

\+ SequenceHeader:

\+ Tag0:

\+ KdcOptions: 0x40810000

\+ Tag2: 0x1

\+ Realm: CONTOSO.COM

\+ Tag3:

\+ Sname: Administrator

\+ Tag5: 0x1

\+ Till: 09/13/2037 02:48:05 UTC

\+ Tag7:

\+ Nonce: 1167598226 (0x45982292)

\+ Tag8:

\+ Etype:

\+ TagA:

\+ EncAuthorizationData:

</div>

<div>

</div>

<div>

KerberosV5:KRB\_ERROR  - KDC\_ERR\_S\_PRINCIPAL\_UNKNOWN (7)

</div>

<div>

\- Kerberos: KRB\_ERROR  - KDC\_ERR\_S\_PRINCIPAL\_UNKNOWN (7)

\+ Length: Length = 105

\- KrbError: KRB\_ERROR (30)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_ERROR (30)

\+ Tag4:

\+ Stime: 04/08/2017 06:06:33 UTC

\+ Tag5:

\+ SuSec: 429948

\+ Tag6:

\+ ErrorCode: KDC\_ERR\_S\_PRINCIPAL\_UNKNOWN (7)

\+ Tag9:

\+ Realm: CONTOSO.COM

\+ TagA:

\+ Sname: Administrator

\+ TagC:

\- EData:

\- EData:
0

0  ---------------?¡------------------------------------------------------------

\+ AsnOctetStringHeader:

OctetStream: 04 0D 30 0B 30 09 A0 03 02 01 80 A1 02 04 00

</div>

<div>

</div>

<div>

In the Kerberos error message, the extended error data has

Error Code: 0x1b KDC\_ERR\_MUST\_USE\_USER2USER

</div>

<div>

</div>

<div>

The Kerberos event log entry looks like the following:

\- \<Event
xmlns="http://schemas.microsoft.com/win/2004/08/events/event"\>

\- \<System\>

\<Provider Name="Microsoft-Windows-Security-Kerberos"
Guid="{98E6CFCB-EE0A-41E0-A57B-622D4E1B30B1}" EventSourceName="Kerberos"
/\>

\<EventID Qualifiers="32768"\>3\</EventID\>

\<Version\>0\</Version\>

\<Level\>2\</Level\>

\<Task\>0\</Task\>

\<Opcode\>0\</Opcode\>

\<Keywords\>0x80000000000000\</Keywords\>

\<TimeCreated SystemTime="2017-04-08T06:06:33.640231200Z" /\>

\<EventRecordID\>8201\</EventRecordID\>

\<Correlation /\>

\<Execution ProcessID="0" ThreadID="0" /\>

\<Channel\>System\</Channel\>

\<Computer\>client2016rs1.contoso.com\</Computer\>

\<Security /\>

\</System\>

\- \<EventData\>

\<Data Name="LogonSession" /\>

\<Data Name="ClientTime" /\>

\<Data Name="ServerTime"\>6:6:33.0000 4/8/2017 Z\</Data\>

\<Data Name="ErrorCode"\>0x1b\</Data\>

\<Data Name="ErrorMessage"\>Unknown Error\</Data\>

\<Data Name="ExtendedError" /\>

\<Data Name="ClientRealm" /\>

\<Data Name="ClientName" /\>

\<Data Name="ServerRealm"\>CONTOSO.COM\</Data\>

\<Data Name="ServerName"\>Administrator\</Data\>

\<Data Name="TargetName"\>Administrator@CONTOSO.COM\</Data\>

\<Data Name="ErrorText" /\>

\<Data Name="File"\>9\</Data\>

\<Data Name="Line"\>11b8\</Data\>

\<Binary\>300B3009A003020180A1020400\</Binary\>

\</EventData\>

\</Event\>

</div>

<div>

</div>

<div>

the client sends UsertoUserSessionReq: KRB\_TGT\_REQ (0x10) to the
server.

</div>

<div>

\- MSRPC: c/o Bind: unknown UUID{00000001-0000-0000-C000-000000000046} 
Call=0x2  Assoc Grp=0x0  Xmit=0x16D0  Recv=0x16D0

\- Bind: {00000001-0000-0000-C000-000000000046} unknown

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0B - Bind

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 304 (0x130)

AuthLength: 136 (0x88)

CallId: 2 (0x2)

MaxXmitFrag: 5840 (0x16D0)

MaxRecvFrag: 5840 (0x16D0)

AssocGroupId: 0 (0x0)

\+ PContextElem:

\- AuthVerifier: 0x1

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- InitialContextToken:

\+ ApplicationHeader:

\+ ThisMech: SpnegoToken (1.3.6.1.5.5.2)

\- InnerContextToken: 0x1

\- SpnegoToken: 0x1

\+ ChoiceTag:

\- NegTokenInit:

\+ SequenceHeader:

\+ Tag0:

\- MechTypes: Prefer MsKerberosToken (1.2.840.48018.1.2.2)

\+ SequenceHeader:

\+ MechType: MsKerberosToken (1.2.840.48018.1.2.2)

\+ MechType: KerberosToken (1.2.840.113554.1.2.2)

\+ MechType: Negoex (1.3.6.1.4.1.311.2.2.30)

\+ MechType: NLMP (1.3.6.1.4.1.311.2.2.10)

\+ Tag2:

\+ OctetStringHeader:

\- MechToken: unused (0)

\- MsKerberosToken: unused (0)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: unused (0)

\- KerberosToken: unused (0)

TokId: 0x400

\- UsertoUserSessionReq: KRB\_TGT\_REQ (0x10)

\+ SequenceHeader:

\+ Tag0:

\+ pvno: 5

\+ Tag1:

\+ MessageType: 16

\+ Tag2:

\+ ServerName: Administrator

\+ Tag3:

\+ Realm: CONTOSO

</div>

<div>

</div>

<div>

The server requests TGT for the user from the KDC.

</div>

<div>

</div>

<div>

KerberosV5:AS Request Cname: Administrator Realm: CONTOSO.COM Sname:
krbtgt/CONTOSO.COM

KerberosV5:KRB\_ERROR  - KDC\_ERR\_PREAUTH\_REQUIRED (25)

</div>

<div>

KerberosV5:AS Request Cname: Administrator Realm: CONTOSO.COM Sname:
krbtgt/CONTOSO.COM

</div>

<div>

\- Kerberos: AS Request Cname: Administrator Realm: CONTOSO.COM Sname:
krbtgt/CONTOSO.COM

\+ Length: Length = 312

\- AsReq: Kerberos AS Request

\+ ApplicationTag:

\- KdcReq: KRB\_AS\_REQ (10)

\+ SequenceHeader:

\+ Tag1:

\+ Pvno: 5

\+ Tag2:

\+ MsgType: KRB\_AS\_REQ (10)

\+ Tag3:

\+ PaData:

\+ Tag4:

\- ReqBody:

\+ SequenceHeader:

\+ Tag0:

\+ KdcOptions: 0x40810010

\+ Tag1:

\+ Cname: Administrator

\+ Tag2: 0x1

\+ Realm: CONTOSO.COM

\+ Tag3:

\+ Sname: krbtgt/CONTOSO.COM

\+ Tag5: 0x1

\+ Till: 09/13/2037 02:48:05 UTC

\+ Tag6:

\+ Rtime: 09/13/2037 02:48:05 UTC

\+ Tag7:

\+ Nonce: 1118221639 (0x42A6B547)

\+ Tag8:

\+ Etype:

\+ Tag9:

\- Addresses:

\+ SequenceOfHeader:

\- Address: SUT2016RS1

\+ SequenceHeader:

\+ Tag0:

\+ AddrType: Netbios address (20)

\+ Tag1:

\+ Address: SUT2016RS1

</div>

<div>

</div>

<div>

KerberosV5:AS Response Ticket\[Realm: CONTOSO.COM, Sname:
krbtgt/CONTOSO.COM\]

</div>

<div>

 - Kerberos: AS Response Ticket\[Realm: CONTOSO.COM, Sname:
krbtgt/CONTOSO.COM\]

\+ Length: Length = 1583

\- AsRep: Kerberos AS Response

\+ ApplicationTag:

\- KdcRep: KRB\_AS\_REP (11)

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AS\_REP (11)

\+ Tag2:

\+ Padata:

\+ Tag3:

\+ Crealm: CONTOSO.COM

\+ Tag4:

\+ Cname: Administrator

\+ Tag5:

\+ Ticket: Realm: CONTOSO.COM, Sname: krbtgt/CONTOSO.COM

\+ Tag6:

\+ EncPart:

</div>

<div>

</div>

<div>

the client receives UsertoUserSessionRep: KRB\_TGT\_REP (0x11) from the
server

</div>

<div>

\- MSRPC: c/o Bind Ack: unknown
UUID{00000001-0000-0000-C000-000000000046} Call=0x2  Assoc
Grp=0x55A4AC30  Xmit=0x16D0  Recv=0x16D0

\- BindAck:

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0C - Bind Ack

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 1303 (0x517)

AuthLength: 1187 (0x4A3)

CallId: 2 (0x2)

MaxXmitFrag: 5840 (0x16D0)

MaxRecvFrag: 5840 (0x16D0)

AssocGroupId: 1436855344 (0x55A4AC30)

\+ SecAddr: 54390

Pad2: 0x1

\+ PResultList:

\- AuthVerifier:

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag1:

\+ SupportedMech: MsKerberosToken (1.2.840.48018.1.2.2)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: unused (0)

\- MsKerberosToken: unused (0)

\- KerberosInitToken:

\+ ApplicationHeader:

\+ ThisMech: UserToUserMechanism (1.2.840.113554.1.2.2.3)

\- InnerContextToken: unused (0)

\- KerberosToken: unused (0)

TokId: 0x401

\- UsertoUserSessionRep: KRB\_TGT\_REP (0x11)

\+ SequenceHeader:

\+ Tag0:

\+ pvno: 5

\+ Tag1:

\+ MessageType: 17

\+ Tag2:

\- Ticket: Realm: CONTOSO.COM, Sname: krbtgt/CONTOSO.COM

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO.COM

\+ Tag2: 0x1

\+ Sname: krbtgt/CONTOSO.COM

\+ Tag3: 0x1

\+ EncPart:

</div>

<div>

</div>

<div>

The client request TGS from the KDC for the service's user.

</div>

<div>

</div>

<div>

KerberosV5:TGS Request Realm: CONTOSO.COM Sname: Administrator

</div>

<div>

\- Kerberos: TGS Request Realm: CONTOSO.COM Sname: Administrator

\+ Length: Length = 2692

\- TgsReq: Kerberos TGS Request

\+ ApplicationTag:

\- KdcReq: KRB\_TGS\_REQ (12)

\+ SequenceHeader:

\+ Tag1:

\+ Pvno: 5

\+ Tag2:

\+ MsgType: KRB\_TGS\_REQ (12)

\+ Tag3:

\- PaData:

\+ SequenceOfHeader:

\- PaData: PA-TGS-REQ (1)

\+ SequenceHeader:

\+ Tag1:

\+ PaDataType: PA-TGS-REQ (1)

\+ Tag2:

\+ OctetStringHeader:

\- KrbApReq: KRB\_AP\_REQ (14)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REQ (14)

\+ Tag2: 0x1

\+ ApOptions:

\+ Tag3:

\+ Ticket: Realm: CONTOSO.COM, Sname: krbtgt/CONTOSO.COM

\+ Tag4:

\+ Authenticator:

\+ PaData: PA-PAC-OPTIONS (167)

\+ Tag4:

\- ReqBody:

\+ SequenceHeader:

\+ Tag0:

\- KdcOptions: 0x40810008

\+ KerberosFlagsHeader:

\+ Padding:

\- KrbFlags: 0x40810008

Reserved:              (0...............................)

Forwardable:           (.1..............................) Ticket to be
issued is to have its FORWARDABLE flag set

Forwarded:             (..0.............................) This is not a
request for forwarding

Proxiable:             (...0............................) Ticket to be
issued is not to have its PROXIABLE flag set

Proxy:                 (....0...........................) This is not a
request for a proxy

AllowPostDate:         (.....0..........................) Ticket to be
issued is not to have its MAY\_POSTDATE flag set

PostDated:             (......0.........................) This is not a
request for a postdated ticket

Unused7:               (.......0........................)

Renewable:             (........1.......................) Ticket to be
issued is to have its RENEWABLE flag set

Unused9:               (.........0......................)

Unused10:              (..........0.....................)

OptHardwareAuth:       (...........0....................)

Unused12:              (............0...................)

Unused13:              (.............0..................)

CnameInAddlTkt:        (..............0.................) This is not a
request for S4U2proxy functionality

Canonicalize:          (...............1................)

Unused16:              (................0000000000......)

DisableTransitedCheck: (..........................0.....) Checking of
the transited field is enabled

RenewableOk:           (...........................0....) Renewable
ticket is not acceptable

EncTktInSkey:          (............................1...) Indicates that
the ticket for the end server is to be encrypted in the session key from
the additional TGT provided

Unused29:              (.............................0..)

Renew:                 (..............................0.) Present
request is not for a renewal

Validate:              (...............................0) Request is not
to validate a postdated ticket

\+ Tag2: 0x1

\+ Realm: CONTOSO.COM

\+ Tag3:

\+ Sname: Administrator

\+ Tag5: 0x1

\+ Till: 09/13/2037 02:48:05 UTC

\+ Tag7:

\+ Nonce: 1181218830 (0x4667F80E)

\+ Tag8:

\+ Etype:

\+ TagA:

\+ EncAuthorizationData:

\+ TagB:

\- AdditionalTickets:

\+ SequenceOfHeader:

\- Ticket: Realm: CONTOSO.COM, Sname: krbtgt/CONTOSO.COM

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO.COM

\+ Tag2: 0x1

\+ Sname: krbtgt/CONTOSO.COM

\+ Tag3: 0x1

\+ EncPart:

</div>

<div>

</div>

<div>

KerberosV5:TGS Response Cname: myuser01

</div>

<div>

\- Kerberos: TGS Response Cname: myuser01

\+ Length: Length = 1528

\- TgsRep: Kerberos TGS Response

\+ ApplicationTag:

\- KdcRep: KRB\_TGS\_REP (13)

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_TGS\_REP (13)

\+ Tag3:

\+ Crealm: CONTOSO.COM

\+ Tag4:

\+ Cname: myuser01

\+ Tag5:

\- Ticket: Realm: CONTOSO.COM, Sname: Administrator

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO.COM

\+ Tag2: 0x1

\+ Sname: Administrator

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag2:

\+ Cipher: ©lELY³’Ç·ÝÁò·¸@»^\_”@}?WSÀ|hÖ÷FÃrÑ«g†á†ü%º?÷
¢$ÌÿSÖŸ?×íE?ïB£\_†?¤tólëÄU?

\+ Tag6:

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag2:

\+ Cipher: À{ÙcBØP}½§WcæT%3×„·îç”mù×v-=Y¹A%1Âšãˆr\_·âh?

</div>

<div>

</div>

<div>

the client does the AP exchange of Kerberos token with the server, and
indicates in the APOptions USE-SESSION-KEY flag that the ticket is
encrypted with the session key from the server's TGT.

</div>

<div>

</div>

<div>

\- MSRPC: c/o Alter Cont: unknown 
UUID{00000001-0000-0000-C000-000000000046}  Call=0x2

\- AlterContext:

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0E - Alter Context

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 1659 (0x67B)

AuthLength: 1579 (0x62B)

CallId: 2 (0x2)

MaxXmitFrag: ignored

MaxRecvFrag: ignored

AssocGroupId: ignored

\+ PContextElem:

\- AuthVerifier: 0x1

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: unused (0)

\- KerberosToken: unused (0)

\- Kerberos: AP Request Ticket\[Realm: CONTOSO.COM, Sname:
Administrator\]

\- ApReq: KRB\_AP\_REQ (14)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REQ (14)

\+ Tag2: 0x1

\- ApOptions:

\+ KerberosFlagsHeader:

\+ Padding:

\- KrbFlags: 0x60000000

Reserved:       (0...............................)

UseSessionKey:  (.1..............................)

MutualRequired: (..1.............................)

Unused:         (...00000000000000000000000000000)

\+ Tag3:

\- Ticket: Realm: CONTOSO.COM, Sname: Administrator

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ TktVno: 5

\+ Tag1:

\+ Realm: CONTOSO.COM

\+ Tag2: 0x1

\+ Sname: Administrator

\+ Tag3: 0x1

\- EncPart:

\+ SequenceHeader:

\+ Tag0:

\+ EType: aes256-cts-hmac-sha1-96 (18)

\+ Tag2:

\+ Cipher: ©lELY³’Ç·ÝÁò·¸@»^\_”@}?WSÀ|hÖ÷FÃrÑ«g†á†ü%º?÷
¢$ÌÿSÖŸ?×íE?ïB£\_†?¤tólëÄU?

\+ Tag4:

\+ Authenticator:

</div>

<div>

</div>

<div>

Because MUTUAL-REQUIRED flag was set in AP-Options, the server includes
KRB-AP-REP in its response.

</div>

<div>

\- MSRPC: c/o Alter Cont Resp: unknown
UUID{00000001-0000-0000-C000-000000000046} Call=0x2  Assoc
Grp=0x55A4AC30  Xmit=0x16D0  Recv=0x16D0

\- AlterContextResponse:

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0F - Alter Context Resp

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 216 (0xD8)

AuthLength: 152 (0x98)

CallId: 2 (0x2)

MaxXmitFrag: ignored

MaxRecvFrag: ignored

AssocGroupId: ignored

\+ SecAddr:

\+ Pad2: 0x1

\+ PResultList:

\- AuthVerifier:

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: KRB\_AP\_REP (15)

\- KerberosToken: KRB\_AP\_REP (15)

\- Kerberos: AP Response

\- ApRep: KRB\_AP\_REP (15)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REP (15)

\+ Tag2: 0x1

\- AuthorizationData:

\+ SequenceHeader:

\+ Tag0:

\+ EType: rc4-hmac (23)

\+ Tag2:

\+ Cipher:
Uàs5˜3(,S

¥Ç×Ñ•Kìd/aò---------------Ã/}¦Œ«—ƒm–„Fzº4@¬q1\[«u,OCÿ¦ÇŠ}µ˜?D¸j¹šÚÜ|\!»\[ç¤|/öon‡ˆ=i+ï†(šPpãúN….¼wÈ†‚™vºÉWõ

</div>

<div>

</div>

<div>

\- MSRPC: c/o Alter Cont: unknown 
UUID{00000001-0000-0000-C000-000000000046}  Call=0x2

\- AlterContext:

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0E - Alter Context

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 216 (0xD8)

AuthLength: 136 (0x88)

CallId: 2 (0x2)

MaxXmitFrag: ignored

MaxRecvFrag: ignored

AssocGroupId: ignored

\+ PContextElem:

\- AuthVerifier: 0x1

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-incomplete (1)

\+ Tag2:

\+ OctetStringHeader:

\- ResponseToken: KRB\_AP\_REP (15)

\- KerberosToken: KRB\_AP\_REP (15)

\- Kerberos: AP Response

\- ApRep: KRB\_AP\_REP (15)

\+ ApplicationTag:

\+ SequenceHeader:

\+ Tag0:

\+ PvNo: 5

\+ Tag1:

\+ MsgType: KRB\_AP\_REP (15)

\+ Tag2: 0x1

\- AuthorizationData:

\+ SequenceHeader:

\+ Tag0:

\+ EType: rc4-hmac (23)

\+ Tag2:

\+ Cipher: ówûdf\_\_ÇßMk†¦;N”ß³“ºãu:ïýVwhû™F0ÔuDš\>©×¤‡Ÿ7@Pü¤º8MÚ­Ü 4H

\+ Tag3:

\+ OctetStringHeader:

\- MechListMic: KRB\_AP\_REP (15)

\- KerberosToken: KRB\_AP\_REP (15)

TokId: GSS\_GetMIC (0x404)

\- Krb5GssV2GetMic:

\+ Flags: 4 (0x4)

Filler: Binary Large Object (5 Bytes)

SndSeq: 1181218827 (0x4667F80B)

SgnCksum: Binary Large Object (12 Bytes)

</div>

<div>

</div>

<div>

\- MSRPC: c/o Alter Cont Resp: unknown
UUID{00000001-0000-0000-C000-000000000046} Call=0x2  Assoc
Grp=0x55A4AC30  Xmit=0x16D0  Recv=0x16D0

\- AlterContextResponse:

RpcVers: 5 (0x5)

RpcVersMinor: 0 (0x0)

PType: 0x0F - Alter Context Resp

\+ PfcFlags: 7 (0x7)

\+ PackedDrep: 0x10

FragLength: 105 (0x69)

AuthLength: 41 (0x29)

CallId: 2 (0x2)

MaxXmitFrag: ignored

MaxRecvFrag: ignored

AssocGroupId: ignored

\+ SecAddr:

\+ Pad2: 0x1

\+ PResultList:

\- AuthVerifier:

AuthType: RPC\_C\_AUTHN\_GSS\_NEGOTIATE - The Simple and Protected
GSS-API Negotiation (SPNEGO) mechanism selects either NT LAN Manager
(NTLM) or Kerberos authentication.

AuthLevel: dce\_c\_authn\_level\_pkt\_integrity - This level offers the
dce\_c\_authn\_level\_pkt services plus per-PDU modification and
deletion detection.

AuthPadLength: 0 (0x0)

AuthReserved: 0 (0x0)

AuthContextId: 0 (0x0)

\- AuthValue:

\- GssApi:

\- NegotiationToken:

\+ ChoiceTag:

\- NegTokenResp:

\+ SequenceHeader:

\+ Tag0:

\+ NegState: accept-completed (0)

\+ Tag3:

\+ OctetStringHeader:

\- MechListMic: unused (0)

\- KerberosToken: unused (0)

TokId: GSS\_GetMIC (0x404)

\- Krb5GssV2GetMic:

\+ Flags: 5 (0x5)

Filler: Binary Large Object (5 Bytes)

SndSeq: 1096155632 (0x415601F0)

SgnCksum: Binary Large Object (12 Bytes)

</div>

# References

<div>

\[RFC4120\] The Kerberos Network Authentication Service (V5)

https://tools.ietf.org/rfc/rfc4120.txt

\[U2UDraft\] User to User Kerberos Authentication using GSS-API

https://tools.ietf.org/html/draft-swift-win2k-krb-user2user-03

\[RFC2743\] "Generic Security Service Application Program Interface
Version 2, Update 1"

https://tools.ietf.org/rfc/rfc2743.txt

\[MS-CSSP\]: Credential Security Support Provider (CredSSP) Protocol

https://msdn.microsoft.com/en-us/library/cc226764.aspx

\[MS-SPNG\]: Simple and Protected GSS-API Negotiation Mechanism (SPNEGO)
Extension

https://msdn.microsoft.com/en-us/library/cc247021.aspx

\[MS-RA\]: Remote Assistance Protocol

https://msdn.microsoft.com/en-us/library/cc240013.aspx

\[MS-PKCA\]: Public Key Cryptography for Initial Authentication (PKINIT)
in Kerberos Protocol

https://msdn.microsoft.com/en-us/library/cc238455.aspx

\[PKU2U\] Introducing PKU2U in Windows

https://technet.microsoft.com/en-us/library/dd560634(v=ws.10).aspx

\[MS-SFU\]: Kerberos Protocol Extensions: Service for User and
Constrained Delegation Protocol

https://msdn.microsoft.com/en-us/library/cc246071.aspx

</div>

<div>

</div>

</div>

</div>
