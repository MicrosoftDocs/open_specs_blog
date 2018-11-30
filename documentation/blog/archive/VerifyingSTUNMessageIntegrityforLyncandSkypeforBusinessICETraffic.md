<div id="page">

# Verifying STUN Message Integrity for Lync and Skype for Business ICE Traffic

[Tom Jebo](https://social.msdn.microsoft.com/profile/Tom%20Jebo)
2/23/2016 11:51:00
AM

-----

<div id="content">

## Verifying STUN Message Integrity for Lync and Skype for Business ICE Traffic

 

Recently there have been some inquiries about how to verify the
integrity of messages in STUN protocol conversations when used by Lync
and Skype for Business.  In this blog post, I will describe a common
scenario based on a recent customer inquiry and explain how it was
resolved.

### Background

Interactive Connectivity Establishment (ICE)<sup>1</sup> is a protocol
that allows endpoints to create a media based connection across multiple
networks where NAT is used.  ICE makes use of STUN<sup>2</sup> (Simple
Traversal of UDP through NAT) and TURN<sup>3</sup> (Traversal Using
Relay NAT), two solutions to finding the internal addresses of remote
endpoints when NAT is involved. Also useful in this discussion is the
fact that the Open  
Specifications include Microsoft’s extensions to ICE, namely the
\[MS-ICE\], \[MS-ICE2\] and \[MS-ICE2BWM\] documents. We will reference
\[MS-ICE2\]<sup>4</sup> in this discussion. ****

 

### Scenario

In this scenario, two Lync endpoints have established a connection via
the ICE protocol and are in the process of connectivity checks per STUN.
As part of the Binding Request message, an attribute is included called
the MESSAGE-INTEGRITY. This contains an HMAC-SHA1<sup>5</sup> hash of
the message called an HMAC.

In the network trace for this scenario, the STUN Binding Requests are
being sent, having already shared the temporary username and password in
previous Shared Secret Request.

Figure 1 MESSAGE-INTEGRITY
Attribute

[![](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/7827.message-integrityattribute.png)](media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/01/22/05/7827.message-integrityattribute.png)

The calculation of the MESSAGE-INTEGRITY attribute is described in
section 10.2.8 of the referenced STUN RFC. The HMAC-SHA1represents a
hash of the message up to but not including the MESSAGE-INTEGRITY
attribute itself.

To manually test this out, you can compute an HMAC using the
jsSHA<sup>6</sup> tool. Following the description in STUN 10.2.8, an
overview of the steps:

1.  Copy the message data from your network analysis tool as a hex
    stream from the beginning of the STUN Binding Request message to
    just before the MESSAGE-INTEGRITY attribute. Insert this into the
    “Input Text:” field of the HMAC Demo section in the jsSHA tool.
    Remember to pad the text with 0’s to make up the 64-byte multiple
    length. Set the Input Type to HEX
2.  Provide the Key from the previous Shared Secret Request (not
    included in this explanation)
3.  Set the Key Type to TEXT
4.  SHA Variant should be SHA-1 and Output Type should be HEX

The resulting Output Hash field will update automatically and should
match the value in the MESSAGE-INTEGRITY attribute.

In the above shown packet in figure 1, the text used to compute the HMAC
is:

0x0000 000100542112A442

0x0008 BEEBD6F17710B6A1

0x0010 B6A92CBD0006000C

0x0018 764F614D3A667641

0x0020 7300000000240004

0x0028 6EFFFEFF80290008

0x0030 000000000001E6E4

0x0038 8054000431000000

0x0040 8070000400000002 \<= stop just before the MESSAGE-INTEGRITY
attribute

0x0048 0000000000000000 \<= added 56 bytes of 0’s from here to the end

0x0050 0000000000000000

0x0058 0000000000000000

0x0060 0000000000000000

0x0068 0000000000000000

0x0070 0000000000000000

0x0078 0000000000000000

The total length of this is 0x80 hex or 128 bytes. The key from the
Shared Secret Request for this conversation was:

**ydYldnHIRgbOUr1MYUGy4t0g**

Given this data and the steps above, you can even verify the resulting
HMAC yourself by using the jsSHA tool. What you should see as a result
is: 

**b87d4d8d56fc76794667aacae3593e58e1dbfe97  
**

which matches the one in your trace.

However, in the customer scenario, for them it wasn’t matching.

### Problem and Resolution

The problem in this scenario was that the MESSAGE-INTEGRITY HMAC hash
value was not comparing correctly with the value computed using the
shared key and message data.

The customer in this scenario was following the newer STUN draft RFC
(http://tools.ietf.org/html/draft-ietf-behave-rfc3489bis-08\#section-14.4)
which adds some verbiage to the message integrity validation process,
specifically:

 

“Based on the rules above, the hash includes the length field from the
STUN message header.  This length indicates the length of the
entire message, including the MESSAGE-INTEGRITY attribute itself. 
Consequently, the MESSAGE-INTEGRITY attribute MUST be inserted into the
message (with dummy content) prior to the computation of the integrity
check.  Once the computation is performed, the value of the attribute
can be filled in.  This ensures the length has the correct value when
the hash is performed.  Similarly, when validating the
MESSAGE-INTEGRITY, the length field should be adjusted to point to the
end of the MESSAGE-INTEGRITY attribute prior to calculating the HMAC. 
Such adjustment is necessary when attributes, such as FINTERPRINT,
appear after MESSAGE-INTEGRITY.”

This is not in the old (obsoleted) STUN RFC
(<http://tools.ietf.org/html/draft-ietf-behave-rfc3489bis-02#section-10.2.8>).
The problem is that the Lync processing is using the older RFC for the
MESSAGE-INTEGRITY attribute and doesn’t include specifically the
modification of the length in the message header before calculating the
HMAC. The \[MS-ICE2\] open specification does reference the correct STUN
RFC (i.e. the old one) in 2.2.2 but references (indirectly via ICE/NAT
RFC<sup>1</sup> ) the newer one in 3.1.4.8.2.4 where it speaks directly
about calculating the MESSAGE-INTEGRITY attribute.  

So, the only change to make to get the correct result, was to stop
modifying the length field in the message header before calculating the
HMAC.

The main take away from this exercise is that Lync uses the original
STUN RFC for MESSAGE-INTEGRITY.  It’s important to take note of the
references provided in the Microsoft Open Specifications when parsing or
coding to a protocol that is used by a Microsoft product. In this case
\[MS-ICE2\] was misleading as it referred to the older STUN in 2.2.2 and
the newer one (indirectly) in 3.1.4.8.2.4.

### References

1.  ICE: <http://tools.ietf.org/html/draft-ietf-mmusic-ice-19>
2.  STUN: <http://tools.ietf.org/html/draft-ietf-behave-rfc3489bis-02>
3.  New STUN:
    <http://tools.ietf.org/html/draft-ietf-behave-rfc3489bis-08>
4.  TURN: <http://tools.ietf.org/html/draft-rosenberg-midcom-turn-08>
5.  \[MS-ICE2\]:
    <https://msdn.microsoft.com/en-us/library/dd907518(v=office.12).aspx>
6.  HMAC-SHA1: <http://tools.ietf.org/html/rfc2104>

 

 

</div>

</div>
