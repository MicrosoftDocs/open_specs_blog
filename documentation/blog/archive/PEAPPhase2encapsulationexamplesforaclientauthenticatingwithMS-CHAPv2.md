<div id="page">

# PEAP Phase 2 encapsulation examples for a client authenticating with MS-CHAPv2

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
6/5/2009 3:49:00 PM

-----

<div id="content">

Protected Extensible Authentication Protocol, or Protected EAP
([PEAP](http://msdn.microsoft.com/en-us/library/cc238354\(PROT.10\).aspx))
uses [EAP](http://www.ietf.org/rfc/rfc3748.txt) as a transport. The
Transport Layer Security (TLS) tunnel established in Phase 1 is utilized
to protect messages exchanged (e.g. authentication credentials) in Phase
2 against eavesdropping or man-in-the-middle attacks. Because of the TLS
tunnel, analyzing network traces is not trivial when troubleshooting
PEAP. Proper analysis of your scenario requires knowledge of the inner
packet format and encoding within the outer packet. This blog provides
examples of Phase 2 encapsulation for a client authenticating over PEAP
with
[MSCHAPv2](http://msdn.microsoft.com/en-us/library/cc224612\(PROT.13\).aspx)
where the server requests statement of heath check on the client.     

## Background

Protected EAP adds security services to EAP methods that support client
access to a network. The EAP framework works over wired or wireless
networks. PEAP is typically deployed in the context of network access
servers
([NAS](http://msdn.microsoft.com/en-us/library/cc895519\(VS.85\).aspx)),
wireless access points or virtual private network gateways.

The client - EAP peer - transmits PEAP messages to the NAS over
lower-layer protocols such as the Point-to-Point Protocol
([PPP](http://go.microsoft.com/fwlink/?LinkId=90283)), or
[\[IEEE802.1X\]](http://go.microsoft.com/fwlink/?LinkId=89910). NAS
elements carry PEAP exchange with the Network Policy Server (NPS) over
protocols such as the Remote Authentication Dial-In User Service
([RADIUS](http://www.ietf.org/rfc/rfc2865.txt)).

## Purpose of PEAP Phases

PEAP operates in two phases. In Phase 1, the EAP peer establishes a TLS
session and authenticates with the EAP server.

In Phase 2, an inner method is negotiated over the TLS session of Phase
1, and allows the EAP server to authenticate the EAP peer. Examples of
inner methods are EAP-MSCHAPv2, and EAP-TLS.

Phase 2 message exchange is encrypted and sent over the TLS tunnel of
Phase 1, thus safeguarding passwords and other dictionary attackable
user credentials from eavesdropping.

## SoH transmission

One security feature of PEAP is the transmission of Statement of Health
([SoH](http://msdn.microsoft.com/en-us/library/cc246924\(PROT.10\).aspx))
messages. SoH validation occurs in PEAP phase 2, either immediately
after Phase 1 completion if no identity request is required, or after
identity exchange. The presence of client SoH entities and EAP server
components determine whether or not such messages are transmitted.
<span id="_Toc227619787"></span>

## Cryptobinding

Another security feature of PEAP is the cryptobinding process which
enables to protect the PEAP negotiation against man-in-the-middle
attacks. The cryptobinding request and response achieve a two-handshake
between the peer and the EAP server by using key materials from Phase 1
and Phase 2. More information about the computation of the cryptobinding
packet can be found in MS-PEAP, HMAC
[\[RFC2104\]](http://go.microsoft.com/fwlink/?LinkId=90314) and SHA1
[\[RFC3174\]](http://go.microsoft.com/fwlink/?LinkId=90408).

## A look at Phase 1 TLS tunnel

PEAP Phase 1 establishes a TLS session with the option for the server to
request a certificate. PEAP supports TLS v1.0 with cipher suites
TLS\_RSA\_WITH\_RC4\_128\_MD5 and TLS\_RSA\_WITH\_RC4\_128\_SHA. Details
on the semantics associated with phase 1 of PEAP are specified in
\[MS-PEAP\].

## A look at Phase 2 exchange

  - Except result indications, all subsequent Phase 2 EAP messages are
    exchanged inside the tunnel established in Phase1.
  - EAP success or the EAP failure packets are result indications,
    therefore are handled by the PEAP implementation rather than the
    inner EAP method, thus never exchanged inside Phase 1 tunnel.
  - Inner EAP packet can be compressed, except for EAP TLV Extensions
    Method, Capabilities Negotiation Method, and SoH EAP Extensions
    Method. Inner EAP packet compression means Code, Identifier and
    Length fields are removed before encapsulation.
  - Decompression of the abovementioned inner packet is performed by
    decrypting the "inner EAP message". The Code and Identifier fields
    of the inner EAP packet are set to the values stored in the Code and
    Identifier fields of the outer EAP packet, and the Length field is
    set to the length of the decrypted inner EAP message plus 4 (Code +
    Identifier).
  - PEAP implementation supports a single EAP authentication method per
    session. The EAP TLV Extensions Method can always be used. The SoH
    EAP Extensions Method is optional.

## Phase 2 encapsulation breakdown

PEAP authentication uses EAP as a transport.  We first introduce the
general layout of EAP packets. We then show the specific encapsulation
details related to PEAP extension methods.

### General EAP Packet layout

The general layout of the EAP Request and Response packet format is as
follows (ref. RFC3748 Section 4.1).

  
0

</div>

</div>

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

1  
0

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

2  
0

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

3  
0

  
1

Code

Identifier

Length

Type

Type\_Data (variable)

...

Code

    1 for Request

    2 for Response

Examples of common types are:

    1       Identity

    2       Notification

    3       Nak (Response only)

    4       MD5-Challenge

    254   Expanded Types

Note that EAP Packet type for PEAP is Type 25 as specified in MS-PEAP.

The layout for the EAP Success and Failure packet format is shown below
(ref. RFC3748 Section 4.2).

  
0

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

1  
0

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

2  
0

  
1

  
2

  
3

  
4

  
5

  
6

  
7

  
8

  
9

3  
0

  
1

Code

Identifier

Length

     Code

      3 for Success

      4 for Failure                    

### PEAP Phase 2 packets: Headers and Methods details

  - PEAP Header for Request: 0 1 XX Y Y YY 1 9 0 0

          EAP Code = 01, Identifier = XX, Length = YY YY, EAP Type = 25,
Flags + Ver = 00

  - PEAP Header for Response: 02 XX Y Y YY 1 9 0 0

          EAP Code = 02, Identifier = XX, Length = YY YY, EAP Type = 25,
Flags + Ver = 00

  - SoH EAP Extensions Method Header: F E 0 0 0 1 3 7 0 0 0 0 0 0 2 1

          Type = 254 (Expanded Types), Vendor ID = 0x00 01 37, TLV
extensions method = 33(MS-Authentication TLV)

  - Vendor Specific TLV Header: 0 0 0 7 X X X X 0 0 0 0 0 1 3 7

          TLV Type = 00 07, Length = XX XX, Vendor\_ID = 0x00 00 01 37 

  - Cryptobinding TLV = 00 0C 00 38 + Data (56 bytes)

          TLV\_Type = 12 = 0x0C, Length = 56 = 0x38

  - EAP Success: 0 3 X X 0 0 0 4

### Scenario where Client is non-NAP capable

PEAP Phase 2 sequence 

Server to Client

Client to Sever

Identity

Request/Response

PEAP Header Request + Data

PEAP Header Response + Data

MS-SoH

Request/Response

 

PEAP Header + SoH EAP Extensions Method Header + Vendor Specific TLV
Header + SoH Request TLV

PEAP Header Response  + Data: NAK to the MS-SoH Request from Client to
Server (Client also proposed inner EAP method:

Type =0x03 (NAK), Type-Data = 0x1a (MS-CHAPv2)

Inner Method

Request/Response

PEAP Header Request + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x01 (Request)

PEAP Header Response + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x02 (Response)

Inner Method

Success

PEAP Header Request + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x03 (Success)

PEAP Header Response + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x03 (Success)

Result and Cryptobinding TLV

PEAP Header Request + EAP TLV Extension Method Header + Result TLV +
Cryptobinding TLV

PEAP Header Response + EAP TLV Extension Method Header + Result TLV +
Cryptobinding TLV

EAP Success

EAP Success

 

### Scenario where client is NAP capable

PEAP Phase 2 sequence 

Server to Client

Client to Sever

Identity

Request/Response

PEAP Header Request + Data

PEAP Header Response + Data

 

MS-SoH

Request/Response

 

PEAP Header + SoH EAP Extensions Method Header + Vendor Specific TLV
Header + SoH Request TLV

PEAP Header Response + SoH EAP Extensions Method Header + Vendor
Specific TLV Header + SoH TLV

Inner Method

Request/Response

PEAP Header Request + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x01 (Request)

PEAP Header Response + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x02 (Response)

Inner Method

Success

PEAP Header Request + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x03 (Success)

PEAP Header Response + Type-Data with Type-Code = 0x1a (MS-CHAPv2),
OpCode = 0x03 (Success)

Result, SoH Response and Cryptobinding TLV

PEAP Header  Request + EAP TLV Extension Method Header + Result TLV +
SoH Response TLV + Cryptobinding TLV

PEAP Header Response + EAP TLV Extension Method Header + Result TLV +
Cryptobinding TLV

EAP Success

EAP Success

 

 

I hope this helps in troubleshooting or debugging your implementation.

## References:

\[MS-PEAP\]: [Protected Extensible Authentication Protocol (PEAP)
Specification](http://msdn.microsoft.com/en-us/library/cc238354\(PROT.10\).aspx)

\[MS-SOH\]: [Statement of Health for Network Access Protection (NAP)
Protocol
Specification](http://msdn.microsoft.com/en-us/library/cc246924\(PROT.10\).aspx)

\[MS-CHAP\]: [Extensible Authentication Protocol Method for Microsoft
Challenge Handshake Authentication Protocol (CHAP)
Specification](http://msdn.microsoft.com/en-us/library/cc224612\(PROT.13\).aspx)

\[RFC3748\] [Extensible Authentication Protocol
(EAP)](http://www.ietf.org/rfc/rfc3748.txt)

\[RFC2716\] [PPP EAP TLS Authentication
Protocol](http://www.ietf.org/rfc/rfc2716.txt)

\[RFC1661\] [The Point-to-Point Protocol
(PPP)](http://www.ietf.org/rfc/rfc1661.txt)

\[RFC2865\] [Remote Authentication Dial-In User Service
(RADIUS)](http://www.ietf.org/rfc/rfc2865.txt)

[NAP Client
Architecture](http://msdn.microsoft.com/en-us/library/aa369702.aspx)

[NAP Server-side
Architecture](http://msdn.microsoft.com/en-us/library/cc895519\(VS.85\).aspx)
