<div id="page">

# RemoteFX RLGR3 decoding

[Obaid
Farooqi](https://social.msdn.microsoft.com/profile/Obaid%20Farooqi)
4/29/2011 2:47:34
PM

-----

<div id="content">

<span style="font-size: small;"><span style="font-family: Calibri;">Microsoft
introduced a new remote desktop (RDP) technology called RemoteFX in
Windows 7 SP1 and Windows Server 2008 R2 SP1. Remote FX shifts most of
the graphics processing to server from client. As such, thin clients can
display 3D images and stunning visual details that were not possible
before. Since most of the graphics processing takes place on server
side, there are some requirement on the server hardware. For details,
please bing "RemoteFX". For brevity, I'll use RFX for RemoteFX in the
rest of this document.</span></span>

<span style="font-family: Calibri; font-size: small;">To achieve the
graphics performance offered by RFX, a new codec is introduced for RDP.
The official specification for this codec is named “Remote Desktop
Protocol: RemoteFX Codec Extension” and will be mentioned as
\[</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">MS-RDPRFX</span>](http://msdn.microsoft.com/en-us/library/ff635423.aspx)<span style="font-size: small;"><span style="font-family: Calibri;">\]
in the rest of this blog.</span></span>

<span style="font-family: Calibri; font-size: small;">RFX codec uses
modified version of Adaptive Run-Length Golomb-Rice (ARLGR) encoding to
perform lossless compression on graphic data that is sent to a client.
Although RLGR encoding compresses data losslessly, RFX codec performs a
lossy compression on graphic data. The lossy part of compression is
performed at quantization stage. The details of encoding and decoding
are documented in
\[</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">MS-RDPRFX</span>](http://msdn.microsoft.com/en-us/library/ff635423.aspx)<span style="font-size: small;"><span style="font-family: Calibri;">\].
This blogs focuses on RLGR decoding and intends to provide test data for
developers implementing RLGR decoding on the client
side.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">TS\_RFX\_TILESSET
message contains number of tiles. Each tile contains RLGR encoded data
for Y, Cb and Cr channels. The first step is to RLGR decode the three
channels which results in three vectors of 4096 (64x64) elements each.
We will decode only Y channel in this blog. Decoding for Cb and Cr is
exactly the same. We will start from the data for Y channel as received
in a tile of TS\_RFX\_TILESET message and will perform the RLGR3
decoding. After the decoding as described in this document, data is
ready for “Sub-band Reconstruction” as described in section 3.1.8.2 of
\[MS-RDPRFX\].</span></span>

<span style="font-family: Calibri; font-size: small;">I have assumed
that the reader of this blog has read and somewhat understood section 2
and 3 of \[ARLGR\]. Please keep a copy handy while reading this blog.
This blog’s mission is to explain the algorithm presented in section
3.1.8.1.7.3.1 of MS-RDPRFX and I’ll use the terminology presented there
without explaining it. So, it will definitely help to keep a browser
window open pointed to
</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">\[MS-RDPRFX\]</span>](http://msdn.microsoft.com/en-us/library/ff635423.aspx)<span style="font-size: small;"><span style="font-family: Calibri;">.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Here
is the data for Y channel as received in one of the tiles of
TS\_RFX\_TILESET message from an actual Windows 7 SP1 virtual machine
running under hypervisor on Windows 2008 R2 SP1 server:</span></span>

<span style="font-family: courier new,courier;">0x06, 0x20, 0xda, 0x17,
0x42, 0xe8, 0xfa, 0x00, 0x1f, 0xfc, 0x80, 0x64, 0x06, 0x40, 0xc8,
0x32,</span>

<span style="font-family: courier new,courier;">0x0c, 0x86, 0x46, 0x4c,
0x99, 0x67, 0xc5, 0xf8, 0xba, 0x5d, 0x2e, 0x96, 0x4b, 0x45, 0x00,
0x00,</span>

<span style="font-family: courier new,courier;">0x01, 0xba, 0x44, 0x03,
0x30, 0xe8, 0x80, 0xcc, 0xe8, 0x83, 0x97, 0x80, 0x39, 0x88, 0x02,
0x01,</span>

<span style="font-family: courier new,courier;">0x01, 0x04, 0x10, 0x8f,
0x2f, 0x29, 0x1f, 0xff, 0xff, 0xff, 0x9f, 0x73, 0xee, 0x7d, 0xcf,
0xb9,</span>

<span style="font-family: courier new,courier;">0xf7, 0x3e, 0x17, 0x80,
0x00, 0x02, 0x00, 0x03, 0x67, 0x8c, 0x7a, 0xa7, 0x0a, 0x91, 0xc0,
0x70,</span>

<span style="font-family: courier new,courier;">0x1c, 0x1c, 0x38, 0xe2,
0xff, 0xfc, 0x0f, 0xec, 0xdf, 0x33, 0x7c, 0x4d, 0x89, 0x00, 0x00,
0x2f,</span>

<span style="font-family: courier new,courier;">0x07, 0x86, 0x10, 0x96,
0x90, 0x8b, 0xcf, 0xf2, 0x8f, 0xa1, 0x64, 0xb8, 0xc7, 0x81, 0x00,
0x8c,</span>

<span style="font-family: courier new,courier;">0x30, 0x03, 0x10,
0x00</span>

 

<span style="font-size: small;"><span style="font-family: Calibri;">The
size of this block is 116 bytes. The data is always a multiple of 4
bytes or in other words contains complete 32 bit blocks. This is due to
the fact that at the time of encoding, data is padded with zeros to make
output a multiple of 32 bits. This has some implication at the time of
decoding as we will see
later.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">After
RLGR3 decoding, the output vector has 4096 elements. Most of the
elements are zero, so I am reproducing here the elements that are not
zero. Please note that index starts from 0. These elements are called
coefficients in {MS-RDPRFX\] and I'll use that for these elements for
the remainder of this blog.</span></span>

 

\[0024\]=  3

</div>

</div>

 \[0056\]=  3

 \[0088\]=  3

 \[0120\]=  3

 \[0152\]=  3

 \[1152\]= -3

\[1153\]= -3

 \[1154\]= -3

 \[1155\]= -3

 \[1156\]= -3

 \[1157\]= -3

 \[1158\]= -3

\[1159\]= -3

 \[1160\]= -3

 \[1161\]= -3

 \[1162\]= -3

 \[1163\]= -3

 \[1164\]= -3

\[1165\]= -3

 \[1166\]= -3

 \[1167\]= -3

 \[1168\]= -3

 \[1169\]= -3

 \[1170\]= -3

\[1171\]= -3

 \[1172\]= -3

 \[1173\]= -3

 \[1174\]= -3

 \[1175\]= -3

 \[1176\]= -4

\[3083\]= -1

 \[3084\]= -6

 \[3099\]= -1

 \[3100\]= -6

 \[3115\]= -1

 \[3116\]= -5

\[3132\]=  1

 \[3344\]=  1

 \[3345\]=  1

 \[3346\]=  1

 \[3347\]=  1

 \[3348\]=  1

\[3349\]=  1

 \[3350\]=  1

 \[3351\]=  1

 \[3352\]=  1

 \[3353\]=  1

 \[3354\]=  1

\[3355\]=  1

 \[3356\]=  1

 \[3360\]= -8

 \[3361\]= -8

 \[3362\]= -8

 \[3363\]= -8

\[3364\]= -8

 \[3365\]= -8

 \[3366\]= -8

 \[3367\]= -8

 \[3368\]= -8

 \[3369\]= -8

\[3370\]= -8

 \[3371\]= -8

 \[3372\]= -8

 \[3373\]=  1

 \[3628\]=  1

 \[3846\]=-18

\[3854\]=-11

 \[3862\]=  2

 \[3904\]=  4

 \[3905\]=  4

 \[3906\]=  4

 \[3907\]=  4

\[3908\]=  4

 \[3909\]=  4

 \[3910\]=  3

 \[3911\]= -1

 \[3912\]=-14

 \[3913\]=-14

\[3914\]=-14

 \[3915\]=-14

 \[3916\]=-14

 \[3917\]=-14

 \[3918\]=-12

 \[3919\]=  2

\[3974\]= -1

 \[3982\]=  4

 \[4032\]=  6

 \[4037\]= -1

 \[4038\]= -8

 \[4039\]=-59

\[4040\]= 45

 \[4046\]= -5

 \[4047\]=-36

 \[4048\]= -2

 \[4054\]=  1

 \[4055\]=  7

\[4056\]= -1

 

 

 

 

 

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">The algorithm to
encode/decode is a modified form of
</span>[<span style="font-family: Calibri; font-size: small;">\[ARLGR\]</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237&tag=1)<span style="font-family: Calibri; font-size: small;">.
There is one very important difference that is captured in the pseudo
code for encoding and decoding algorithm but is not explicitly mentioned
in
\[</span>[<span style="font-family: Calibri; color: #0000ff; font-size: small;">MS-RDPRFX</span>](http://msdn.microsoft.com/en-us/library/ff635423.aspx)<span style="font-size: small;"><span style="font-family: Calibri;">\].
</span></span>

<span style="font-family: Calibri; font-size: small;">In run mode, the
input to GR(u,kr) is signed numbers i.e. no folding and interleaving
mapping is performed. On the other hand, the input to “no run” mode is
folding and interleaving mapped. Folding and interleaving mapping is
defined in
</span>[<span style="font-family: Calibri; font-size: small;">\[ARLGR\]</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237&tag=1)<span style="font-size: small;"><span style="font-family: Calibri;">.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">In
the following, I’ll present the steps taken to decode the data I
presented earlier in this blog. At the encoding stage, the output of
encoding starts from the MSB (most significant bit) of the byte. So, at
the time of decoding, we start from the MSB of the first byte in the
data. I’ll decode in chunks of 4 bytes at a time and refill from the
input data as needed.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
first four bytes are 0x06, 0x20, 0xda, 0x17 which in binary would be
00000110001000001101101000010111.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
initial values for decoding parameters are:</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=1
kp=8 kr=1 krp=8</span></span>

<span style="font-family: Calibri; font-size: small;">Since k=1, we are
in run mode, we remove first bit from the input which is a zero, the
output is two zeros, based on the fact that 2<sup>k</sup> is 2 so we
produce two coefficients each equal to zero (see Table 2 in
</span>[<span style="font-family: Calibri; font-size: small;">\[ARLGR\]</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237&tag=1)<span style="font-size: small;"><span style="font-family: Calibri;">).</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">\[0000\]
= 0, \[0001\]=0</span></span>

<span style="font-family: Calibri; font-size: small;">Next we increment
kp=kp+4=12, then we right shift kp,
</span>[<span style="font-family: Calibri; font-size: small;">LSGR</span>](http://msdn.microsoft.com/en-us/library/gg750720\(PROT.10\).aspx)<span style="font-size: small;"><span style="font-family: Calibri;">
times (3) to get new value of k, which is 1. We continue in this fashion
till we hit the first non-zero bit in the input. Here is the output
before we hit the first non-zero
bit.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now =
0000110001000001101101000010111</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Output
= two coefficients each equal to zero (\[0002\]=0,
\[0003\]=0)</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=2
kp=16</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Remove
one
bit</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now =
000110001000001101101000010111</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Output
= 4 coefficients each equal to zero (\[0004\]=0, \[0005\]=0, \[0006\]=0,
\[0007\]=0)</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=2
kp=20</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Remove
one
bit</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now =
00110001000001101101000010111</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Output
= 4 coefficients each equal to zero (\[0008\]=0, \[0009\]=0, \[0010\]=0,
\[0011\]=0)</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=3
kp=24</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Remove
one
bit</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now =
0110001000001101101000010111</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Output
= 8 coefficients each equal to zero (\[0012\]=0, \[0013\]=0, \[0014\]=0,
\[0015\]=0, </span></span><span style="font-size: small;"><span style="font-family: Calibri;"> \[0016\]=0,
\[0017\]=0, \[0018\]=0, \[0019\]=0)</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=3
kp=28</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Remove
one
bit</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now = 110001000001101101000010111</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri;"><span style="font-size: small;">Now
we have a non-zero bit and as we are in run mode, this is of the form 1
+ bin(</span>*m*, *k*) + *GR*(*u*, *kR*)
<span style="font-size: small;">(see
</span></span>[<span style="font-family: Calibri; font-size: small;">\[ARLGR\]</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237&tag=1)<span style="font-family: Calibri;"><span style="font-size: small;">,
Table 2) which means that the first 1 is to mark partial run, the next
three bits (since k=3) tell us how many zeros to output and they are 100
so we output four coefficients of zero each (\[0020\]=0, \[0021\]=0,
\[0022\]=0, \[0023\]=0). So far the number of coefficients produced is
4+8+4+4+2+2=24. The twenty fifth coefficient is encoded with
*GR*</span>(*u*, *kR*). <span style="font-size: small;"> As I explained
earlier, this GR is different from what is described in \[ARLGR\]. The
first bit is the sign bit. If it is zero, the coefficient is a positive
integer, otherwise negative. Since in this case first bit is 0, the
coefficient is positive. The bits that follow represent the GR code of
the (magnitude-1) of the coefficient. So the number of 1’s before we hit
a zero is equal to u/2<sup>kr</sup>. Since there is only one 1, u is
equal to 1X2 = 2 because kr=1. The next zero in the input is the
delimiter 0 to mark the end of 1’s. The next kr bits represent
remainder. So the remainder is 0. That means that (magnitude-1) is 2+0=2
and magnitude is +3. This is the 25<sup>th</sup> coefficient that we
produced (\[0024=3). The number of bit from input
used=4+4=8.</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;">Since we decoded a
GR(u,kr), we need to decrement kp and adjust krp based on number of 1’s
we decoded (*p*). Since *p* = 1 (see
</span>[<span style="font-family: Calibri; font-size: small;">\[ARLGR\]</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237&tag=1)<span style="font-size: small;"><span style="font-family: Calibri;">,
Table 3), no change in kr and krp. Kp is reduced by DN\_GR (6) and
becomes 22 and k=kp\>\>LSGR =22\>\>3=2. After removing 8 bits from
input:</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now = 0001101101000010111</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">We
will continue in “run mode” in similar fashion. When we produce a series
of non-zero coefficients without having zero coefficients in “run mode”,
it results in decrement of kp and eventually results in k becoming zero.
This happens when coefficients from index 1152 to 1162 are produced. All
these coefficients are non-zero (-3) and resulted in k going all the way
to zero. This changes the mode of operation from “run mode” to “no run”
or GR mode. When we enter GR mode we have the input
as</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now = 1111100010111111000</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">k=0
kp=4 kr=1 krp=8</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">So
looking at the input, we have five 1’s. So *p*=5. Since kr=1,
u=p\*2<sup>kr</sup> =5\*2=10.  The remainder is zero (since next bit is
zero and kr=1). We remove 7 bits from the
input.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now= 010111111000</span></span>

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
decoding we are performing here is RLGR3. In RLGR3, GR mode operates a
little different. For details please see section 3.1.8.1.7.2 of
\[MS-RDPRFX\].  So, 10 is sum of two coefficients. The first coefficient
is represented in the least number of bits needed to represent  the sum
(10).</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Now
log<sub>2</sub> 10 \> 3 so the least number of bits needed to represent
next coefficient is 4. The next 4 bits are 0101. That is 5 in decimal.
The second coefficient is 10-5=5. These two numbers are folding and
interleaving mapped. So the actual coefficients are -2y-1=5 =\> -2y=6
=\> y=-3. So, both are equal to -3 (\[1163\]=-3, \[1164\]=-3). We remove
4 bits from the input.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Input
now = 11111000</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
adaption parameters are adjusted differently in GR mode. Kr and krp are
adjusted for the sum. K and kp are adjusted based on the two
coefficients produced. Since *p*=5, krp = krp+5= 8+5=13.
Kr=krp\>\>LSGR=13\>\>3=1. Kp (and subsequently k) is adjusted as per the
pseudo code in section 3.1.8.1.7.3.2 of \[MS-RDPRFX\]. When we entered
Gr mode, k=0 and kp=4. The coefficients produced are non-zero so we
decrement kp by 2\*DQ\_GR. 2\*DQ\_GR is 6 and kp=4-6=-2. But the minimum
kp can go zero, so kp=0 and therefore k=0.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">So
far we have covered all the scenarios of RLGR3 decoding. The only thing
left is to discuss a little bit how the end of the input is processed.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Everything
is dealt as explained earlier but algorithm stops when all 4096
coefficients are generated. As explained earlier, the data is padded to
make number of bits a multiple of 32. So it is not necessary to exhaust
all the bits in the input.
</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">-------------------------------</span></span>

*<span style="font-size: small;"><span style="font-family: Calibri;">References</span></span>*

<span style="font-size: small;"><span style="font-family: Calibri;">\[MS-RDPRFX\]:
Remote Desktop Protocol: RemoteFX Codec
Extension</span></span>

[<span style="font-family: Calibri; font-size: small;">http://msdn.microsoft.com/en-us/library/ff635423(PROT.10).aspx</span>](http://msdn.microsoft.com/en-us/library/ff635423\(PROT.10\).aspx)

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-size: small;"><span style="font-family: Calibri;">\[ARLGR\]
Malvar, H.S., "Adaptive Run-Length / Golomb-Rice Encoding of Quantized
Generalized Gaussian Sources with Unknown Statistics", Proceedings of
the Data Compression Conference, 2006 (DCC 2006) pp. 23 - 32, March
2006,
</span></span>

[<span style="font-family: Calibri; font-size: small;">http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237</span>](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=01607237)

<span style="font-family: Calibri; font-size: small;"> </span>

<span style="font-family: Calibri; font-size: small;"> </span>
