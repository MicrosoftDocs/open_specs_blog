<div id="page">

# Implementing the Algorithm for Deriving Password and encrypting Connection String in MS-RAIOP

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
5/13/2011 9:04:00 PM

-----

<div id="content">

<span style="font-family: Calibri; font-size: large;">In Microsoft Open
Protocol documents, there are many algorithms that involve with crypto
operations in some particular ways.   Sometimes the protocol documents
also provide specific protocol examples that include the initial input,
the intermediate results and the final result for each step of the
algorithm.   The correctness of the result  is very crucial for the
implementers to validate their implementation step by step. 
 Occasionally there may be some steps missing or wrong information
included.  The best way to verify if the examples are right is to have a
sample implementation that is only based on the algorithm
documented.</span>

<span style="font-size: large;"><span style="font-family: Calibri;">One
of the examples is the operation of deriving a password and encrypting a
connection string used in Remote Assistance Initiation over PNRP
Protocol
(</span>[<span style="color: #0066ff;">MS-RAIOP</span>](http://msdn.microsoft.com/en-us/library/dd357155\(v=prot.10\).aspx)<span style="font-family: Calibri;">).  
An example is provided at the section 4.1 of the document.  It requires
many steps of buffer manipulation, hashing, cipher key derivation and
encryption.    The documentation of some steps was not very accurate or
clear initially.   Some readers had problems to implement the operation
correctly.  We have to develop a sample implementation to verify the
existing documentation and then fix the problems found. This blog will
provide the sample implementation for each step and point out the
document problems corrected.  As an attachment, the complete source file
will be attached for any implementer to use with the protocol
document.</span></span>

<span style="font-family: Calibri; font-size: large;">I will copy the
section 4.1 of MS-RAIOP below and annotate it with the sample
implementation.</span>

<span style="font-family: Calibri; font-size: large;">The first is the
initialization part for variable initialization and crypto system setup.
</span>

<span style="font-family: Calibri; font-size: large;">﻿</span>int
\_tmain(int argc, TCHAR\* argv\[\], TCHAR\* envp\[\])

{

         int nRetCode = 0;HCRYPTPROV hCryptProv;

        HCRYPTKEY hKey;

         HCRYPTHASH hHash;

         DWORD dwHashLen,dwHashLenSize;

         BYTE       \*pbHash;

         BYTE       szLastHash\[6\];  //used in step 5

          wchar\_t szConnectionString\[128\];

         wchar\_t ValidPasswordCharacterString\[30\];

         StringCchCopyW(ValidPasswordCharacterString,29+1,
L"BCDFGHJKLMNPQRSTVWXYZ23456789");

         BYTE AESKeyBlob\[\] = {
 0x08,0x02,0x00,0x00,0x0e,0x66,0x00,0x00, // BLOB header ,AES128

           0x10,0x00,0x00,0x00,                     // key length, in
bytes

           0x00, 0x00,  0x00,  0x00,  0x00 , 0x00 , 0x00,  0x00,  0x00, 
0x00,  0x00,  0x00,  0x00, 0x00,     

           0x00 , 0x00  // AES key  value popluated late };

          DWORD timeSeconds= 1218745079;

         DWORD dwLength = 0;

         DWORD dwConnectionStringLength= 0;

         size\_t sizeTemp;

         //

         //Initialization of the Crypto system

         //

        if ( \!CryptAcquireContextW(\&hCryptProv,   0, 
(LPCWSTR)L"Microsoft Enhanced RSA and AES Cryptographic
Provider",  PROV\_RSA\_AES, CRYPT\_SILENT) )

         {

              printf("CryptAcquireContextW failed\\n");

              if ( GetLastError() == -2146893802 )

              {

                  if (\!CryptAcquireContextW(\&hCryptProv, 0,

                                         (LPCWSTR)L"Microsoft Enhanced
RSA and AES Cryptographic Provider",

                                          0x18u,

                                          0xF0000048u) )

                    MyHandleError("Error during CryptAcquireContext\!");

               }

        
 }

<span style="font-size: large;">**<span style="font-family: Verdana;">4.1  
Deriving a Password and Encrypting a Connection String for Unsecured
Peer Name Initiation</span>**</span>

<span style="font-size: large;"><span style="font-family: Calibri;">This
example follows the steps that appear in sections
</span>[<span style="color: #0066ff;">3.1.5.1</span>](http://blogs.msdn.com/tiny_mce/plugins/paste/pasteword.htm#z194aa7f6d0ab422e975bf531805686f1)<span style="font-family: Calibri;">
and
</span>[<span style="color: #0066ff;">3.1.5.2</span>](http://blogs.msdn.com/tiny_mce/plugins/paste/pasteword.htm#zfedf1082dba94feab8f67594bc37c2e5)<span style="font-family: Calibri;">,
which show how to derive a password and encrypt a sample
string:</span></span>

<span style="font-size: large;"><span style="font-family: Verdana;"> 1.</span>   
<span style="font-family: Verdana;">Assume for the connection string
"SAMPLE", when encryption is done, 1218745079 seconds have elapsed since
January 1, 1970 UTC.</span></span>

         
/////////////////////////////////////////////////////////////////////////

           //Step 1: Assume for the connection string "SAMPLE", when
encryption is done,   1218745079 seconds have elapsed since January 1,
1970 UTC.

        
/////////////////////////////////////////////////////////////////////////

         StringCchCopyW(szConnectionString, 6+1, L"SAMPLE");

         if
(SUCCEEDED(StringCchLength(szConnectionString,128,(size\_t\*)\&sizeTemp)))

                 dwConnectionStringLength = (DWORD)sizeTemp;

         printHex ( (BYTE\*)
\&szConnectionString\[0\],dwConnectionStringLength \* sizeof(wchar\_t),
"Connection String (Step 1):");

         

<span style="font-size: large;">   2.   
<span style="font-family: Verdana;">The first hash input is the
following, expressed in hexadecimal
values.</span></span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">     
53 00 41 00 4d 00 50 00 4c 00 45 00 00 00 00 00 00 00 00
00</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">      00
00 00 00 00 00 00 00 00 00 00 00</span></span>

</div>

       
//////////////////////////////////////////////////////////////////////////

//Step 2:  Padding 20 bytes (10 wchar) to the connection string
("SAMPLE")

//////////////////////////////////////////////////////////////////////////

wchar\_t padding\[10\];

wmemset(padding,0,10);

 //add 20 bytes of zero for initial padding

wmemcpy(\&szConnectionString\[6\],padding,10);

dwLength = dwConnectionStringLength + 10;

printHex ( (BYTE\*) \&szConnectionString\[0\],dwLength \*
sizeof(wchar\_t), "Inuput for first hash:");

<span style="font-family: Verdana; font-size: large;">3.  After the
first hash iteration, the hash result is the following, expressed in
hexadecimal
values.</span>

<div style="padding-left: 30px;">

<span style="font-size: large;"><span style="font-family: Courier New;">  
00 df 01 a1 9d 25 89 60 e6 a4 4a a9 8b e9 c2 6f 00 29 22
39</span></span>

</div>

<span style="font-family: Verdana; font-size: large;">4.  When the hash
result is added back to the original input, a new hash input results,
which is expressed here in hexadecimal
values.</span>

<div style="padding-left: 30px;">

<span style="font-size: large;"><span style="font-family: Courier New;">  
53 00 41 00 4d 00 50 00 4c 00 45 00 00 df 01 a1 9d 25 89
60</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">  
e6 a4 4a a9 8b e9 c2 6f 00 29 22 39</span></span>

</div>

<span style="font-family: Verdana; font-size: large;">5.  After the
100,000 iterations, the first sequence of 6 bytes of the hash result is
the following, expressed in hexadecimal
values.</span>

<div style="padding-left: 30px;">

<span style="font-size: large;"><span style="font-family: Courier New;">  
1d f6 35 43 74 92</span></span>

      
////////////////////////////////////////////////////////////////////////////

      //Step 3,4 & 5: for the 100,000 hash iterations.  The hash value
of the each  iteration is copied to the last 20 bytes of the input
buffer for the next iteration

     
////////////////////////////////////////////////////////////////////////////

      for (int it = 0 ; it\< 100000;it++)

      {

           if(\!CryptCreateHash(hCryptProv, CALG\_SHA, 0, 0, \&hHash))

              MyHandleError("Error during CryptCreateHash\!");

            if(\!CryptHashData(hHash, (BYTE \*)szConnectionString,
dwLength \* sizeof(wchar\_t), 0))

                                    MyHandleError("Error during
CryptHashData\!");

           if(\!CryptGetHashParam(hHash,  HP\_HASHSIZE,(BYTE
\*)\&dwHashLen,\&dwHashLenSize,0))

                 MyHandleError("CryptGetHashParam failed to get size.");

            if(\!(pbHash = (BYTE\*)malloc(dwHashLen)))

                 MyHandleError("Allocation failed.");

            
if(\!CryptGetHashParam(hHash,HP\_HASHVAL,pbHash,\&dwHash,0))

                 MyHandleError("Error during reading hash value.");

             //This is the output of value for the Step 3

            if (it == 0)

              printHex ( (BYTE\*)pbHash,dwHashLen, "The first hash
output (step 9):");

            else if (it == 99999)  //last iteration

            {

                 //This is the hash value of the last hash iteration

                 printHex ( (BYTE\*)pbHash,6, "The final hash  (step
5):");

                 memcpy((BYTE\*) \&szLastHash\[0\],pbHash,6);

                 break;

            }

            memcpy((BYTE\*)
\&szConnectionString\[dwConnectionStringLength\],pbHash,dwHashLen);

            free(pbHash);

            CryptDestroyHash(hHash);

         }

﻿

<span style="font-family: Verdana; font-size: large;">6.  Each byte is
divided by 256 and then multiplied by 29 (the number of characters in
the string). The resulting fractions are discarded to obtain an index
for each byte. The resulting index for each byte is looked up in the
password character string "BCDFGHJKLMNPQRSTVWXYZ23456789" to obtain the
corresponding character for that
byte.</span>

<span style="font-size: large;"><span style="font-family: Courier New;">First
value: 0x1d is 29, (29/256) \* 29 =3.29 -\>
3</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">Indexes:
3 27 6 7 13
16</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">Resultant
characters: F 8 J K R
V</span></span><span style="font-family: Courier New; font-size: x-small;"> </span>

    
////////////////////////////////////////////////////////////////////////////

     //Step 6:  Use the first six bytes of hash values to compute the
index to ValidPasswordCharacterString array for each byte.

    
////////////////////////////////////////////////////////////////////////////

     for (int j = 0;j \<6;j++)

         szConnectionString\[j\] =
ValidPasswordCharacterString\[szLastHash\[j\] \* 29 / 256\];

     szConnectionString\[6\] =0;

<div>

     printHex ( (BYTE\*)\&szConnectionString\[0\],6\*sizeof(wchar\_t),
"The string from the valid password character  (step 6):");

</div>

<div>

 

</div>

<div>

<span style="font-family: Verdana; font-size: large;">7.  When the
number of seconds since Jan 1, 1970 is divided by 3600, the resulting
figure is the number of hours that have elapsed since Jan 1, 1970 UTC.
This number is concatenated to the derived characters to get the string
"F8JKRV338540".</span>

   
////////////////////////////////////////////////////////////////////////////

    //Step 7:  When the number of seconds since Jan 1, 1970 is divided
by 3600, the   resulting figure is the number of hours that have elapsed
since

    //         Jan 1, 1970 UTC. This number is concatenated to the
derived characters 

   
////////////////////////////////////////////////////////////////////////////

    wchar\_t szTemp\[32\];

    StringCchPrintf(szTemp, 32,L"%d", timeSeconds/3600);

    StringCchCatW(\&szConnectionString\[0\],128,szTemp);

     if
(SUCCEEDED(StringCchLengthW(\&szConnectionString\[0\],128,\&sizeTemp)))

         dwConnectionStringLength = (DWORD)sizeTemp;

     printHex (  (BYTE\*)\&szConnectionString\[0\],
dwConnectionStringLength\*sizeof(wchar\_t) , "After adding the time 
(step 7):");

<span style="font-family: Verdana;"> </span>

<span style="font-family: Verdana; font-size: large;">8.  The first hash
input is the previous string copied into a byte buffer that is expressed
in hexadecimal
values.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">   46
00 38 00 4a 00 4b 00 52 00 56 00 33 00 33 00 38 00 35
00</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">  
34 00 30 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">  
00 00 00 00</span></span>

</div>

<span style="font-family: Verdana; font-size: large;">9.  The first hash
result is the following value, expressed in
hexadecimal.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">  
5f 3d 53 db 86 9a f1 ac 24 63 21 f4 c1 78 90 6d 91 a7 1a
d5</span></span>

</div>

<span style="font-family: Verdana; font-size: large;">10.       The hash
result is then copied into the byte buffer and expressed in the
following hexadecimal
values.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">  
46 00 38 00 4a 00 4b 00 52 00 56 00 33 00 33 00 38 00 35
00</span></span>

</div>

</div>

</div>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">34
00 30 00 5f 3d 53 db 86 9a f1 ac 24 63 21 f4 c1 78 90
6d</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">91
a7 1a d5</span></span>

</div>

<span style="font-family: Verdana; font-size: large;">     11.       The
first 16 bytes of the final hash value, which occurs after 100,000
iterations, is expressed as the following hexadecimal
values.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">      30
e3 db fb 31 4b 40 9a 70 bc ce 74 4c ad e6 5f
</span></span>

<span style="font-size: large;"><span style="font-family: Courier New;">   </span></span><span style="font-family: Verdana; font-size: large;">12.      
The previous value is then converted into a Unicode string and results
in "30E3DBFB314B409A70BCCE744CADE65F".</span>

                 
////////////////////////////////////////////////////////////////////////////

                 //step 8,9,10,11 :   Do 100,000 iterations of hash,
copying the output  of the previous iteration to the input of next
iteration  

                
////////////////////////////////////////////////////////////////////////////

               wmemset(padding,0,10);

            
 wmemcpy(\&szConnectionString\[dwConnectionStringLength\],padding,10);

              dwLength = dwConnectionStringLength+ 10;

             //output the result for step 8

             printHex (  (BYTE\*)\&szConnectionString\[0\], dwLength
\*sizeof(wchar\_t) , "The first hash input after padding 20 bytes of
zeros  (step 8):");

            for (int it = 0 ; it\< 100000;it++)

           {

                 if(\!CryptCreateHash(hCryptProv, CALG\_SHA, 0, 0,
\&hHash))

                    MyHandleError("Error during CryptCreateHash\!");

                if(\!CryptHashData(hHash, (BYTE \*)szConnectionString,
dwLength \* sizeof(wchar\_t), 0))

                   MyHandleError("Error during CryptHashData\!");

                 if(\!CryptGetHashParam(hHash,HP\_HASHSIZE,(BYTE
\*)\&dwHashLen,\&dwHashLenSize,0))

                     MyHandleError("CryptGetHashParam failed to get
size.");

                if(\!(pbHash = (BYTE\*)malloc(dwHashLen)))

                     MyHandleError("Allocation failed.");

               
if(\!CryptGetHashParam(hHash,HP\_HASHVAL,pbHash,\&dwHashLen,0))

                     MyHandleError("Error during reading hash value.");

                 if (it == 0)  //output the result for step 9

                     printHex ( (BYTE\*)pbHash,dwHashLen, "The first
hash output (step 9):");

                 else if (it == 99999)

                 {

                        //output the hash result in step 11

                       printHex ( (BYTE\*)pbHash,16, "The first 16 bytes
of the final hash  (step 11):");

                       //convert it to the HexConvertedUnicodeString

                        wchar\_t temp\[20\];

                        wchar\_t \*pInput = (wchar\_t\*) 
szConnectionString;

                        pInput\[0\] = 0;

                       for (int j = 0;j \<16;j++)

                       {

                           
swprintf((wchar\_t\*)\&temp\[0\],20,L"%X",pbHash\[j\]);

                           wcscat((wchar\_t \*)\&pInput\[0\],temp);

                       }

                       dwLength = 64;

                       //output the hash result in step 11

                       printHex ( (BYTE\*)pInput,dwLength, "The hex
converted string format of the hash result (step 12):");

                      free(pbHash);

                      CryptDestroyHash(hHash);

                      break;

               }

              //copy the hash value into the hash input buffer for the
next iteration

             memcpy((BYTE\*)
\&szConnectionString\[dwConnectionStringLength\],pbHash,dwHashLen);

             //output the result for step 10

             if (it ==0)

                   printHex (  (BYTE\*)\&szConnectionString\[0\],
dwLength \*sizeof(wchar\_t) , "The second hash input after copying the
first hash value (step 10):");

               free(pbHash);

              CryptDestroyHash(hHash);

     }

<span style="font-family: Verdana; font-size: large;">13.       Using
the SHA-1 hash algorithm, hash the key string from step 12 to obtain the
following hexadecimal
values.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">    bb
50 02 ab ff f3 f8 23 6d 84 7d 50 ee a9 9a ba 2b 2c 1e 45</span></span>

        
////////////////////////////////////////////////////////////////////////////

         //Step 13: Using the SHA-1 hash algorithm, hash the key string
from step 12.

        
////////////////////////////////////////////////////////////////////////////

         if(\!CryptCreateHash(hCryptProv, CALG\_SHA, 0, 0, \&hHash))

             MyHandleError("Error during CryptCreateHash\!");

          if(\!CryptHashData(hHash, (BYTE \*)szConnectionString,
dwLength , 0))

             MyHandleError("Error during CryptHashData\!");

        if(\!CryptGetHashParam(hHash,HP\_HASHSIZE,(BYTE
\*)\&dwHashLen,\&dwHashLenSize,0))

              MyHandleError("CryptGetHashParam failed to get size.");

         if(\!(pbHash = (BYTE\*)malloc(dwHashLen)))

              MyHandleError("Allocation failed.");

         if(\!CryptGetHashParam(hHash,HP\_HASHVAL,pbHash,\&dwHashLen,0))

               MyHandleError("Error during reading hash value.");

          memcpy ((BYTE\*) 
szConnectionString,(BYTE\*)pbHash,dwHashLen);

         printHex ((BYTE\*)szConnectionString,dwHashLen, "The hash value
of the hex key string in step 12 (step 13):");

          free(pbHash);

        
CryptDestroyHash(hHash);

<span style="font-size: large;"><span style="font-family: Verdana;">14.      
Using the resultant hash in step 13 and the AES\_128 algorithm, as
specified in
</span>[<span style="font-family: Verdana; color: #0066ff;">\[FIPS197\]</span>](http://go.microsoft.com/fwlink/?LinkId=89870)<span style="font-family: Verdana;">,
derive the following cipher key for
encryption.</span></span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">49
95 da af 8f cb fd fc 1d 21 f5 72 52 46 52 eb</span></span>

    
////////////////////////////////////////////////////////////////////////////

      //Step 14:  Using the resultant hash in step 13 and the following
logic to derive the AES 128 hash key

     
////////////////////////////////////////////////////////////////////////////

      char buffer1\[64\];

       BYTE \*pInput = (BYTE\*)  \&szConnectionString\[0\];

      memset((char\*) \&buffer1\[0\],0x36,64);

      for (int i=0;i\<20;i++)

         buffer1\[i\] = buffer1\[i\] ^  pInput\[i\];

      if(\!CryptCreateHash(hCryptProv, CALG\_SHA, 0, 0, \&hHash))

         MyHandleError("Error during CryptCreateHash\!");

      if(\!CryptHashData(hHash, (BYTE \*)buffer1, 64, 0))

         MyHandleError("Error during CryptHashData\!");

      if(\!CryptGetHashParam(hHash,HP\_HASHSIZE,(BYTE
\*)\&dwHashLen,\&dwHashLenSize,0))

         MyHandleError("CryptGetHashParam failed to get size.");

      if(\!(pbHash = (BYTE\*)malloc(dwHashLen)))

         MyHandleError("Allocation failed.");

       if(\!CryptGetHashParam(hHash,HP\_HASHVAL,pbHash,\&dwHashLen,0))

           MyHandleError("Error during reading hash value.");

       memcpy((BYTE\*) \&AESKeyBlob\[12\],pbHash,16);

       printHex ( (BYTE\*) \&AESKeyBlob\[12\],16, "The AES 128 key
drived from the resulatnt hash in step 13 (step 14):");

       free(pbHash);

       CryptDestroyHash(hHash);

<span style="font-family: Verdana; font-size: large;">15.      
Encrypting the original connection string "SAMPLE" with the cipher key,
results in the following encrypted
value.</span>

<div>

<span style="font-size: large;"><span style="font-family: Courier New;">    7f
d6 54 48 2f e0 92 73 d7 69 85 b0 1d 4b 7a 4b</span></span>

    
////////////////////////////////////////////////////////////////////////////

      //Step 15: encrypt original connection string "SAMPLE: with the
cipher key in step 14

     
////////////////////////////////////////////////////////////////////////////

      StringCchCopyW(szConnectionString, 6+1, L"SAMPLE");

      if
(SUCCEEDED(StringCchLength(szConnectionString,128,(size\_t\*)\&sizeTemp)))

         dwConnectionStringLength = (DWORD)sizeTemp;

      dwConnectionStringLength = dwConnectionStringLength \*
sizeof(wchar\_t);

       //Create a key from the key blob

      if (\!CryptImportKey(hCryptProv,(BYTE\*)
\&AESKeyBlob\[0\],sizeof(AESKeyBlob),0,CRYPT\_EXPORTABLE, \&hKey))

             MyHandleError("Error create a AES key from the  key blob");

       //Encrpyt the connection string "SAMPLE" using the key

      if (\!CryptEncrypt(hKey,NULL,TRUE,0,(BYTE\*)
\&szConnectionString\[0\],\&dwConnectionStringLength,128))

             MyHandleError("Error encrypting the connection string");

      printHex (  (BYTE\*)
\&szConnectionString\[0\],dwConnectionStringLength, "Encrypted data:
(Step 15)");

 <span style="font-family: Calibri; font-size: large;">     The
following section will do the final clean up.</span>

       
////////////////////////////////////////////////////////////////////////////

       //final clean up

       
////////////////////////////////////////////////////////////////////////////

       if(hKey)

          if(\!(CryptDestroyKey(hKey)))

            MyHandleError("Error during CryptDestroyKey");

        if(hCryptProv)

          if(\!(CryptReleaseContext(hCryptProv, 0)))

             MyHandleError("Error during
CryptReleaseContext");

<span style="font-family: Calibri;"> </span>

<span style="font-family: Calibri;"> <span style="font-size: large;">While
developing the sample Implementation, we found that the following steps
have unclear information that may cause an unsuccessful
implementation.</span></span>

  - <span style="font-family: Calibri; font-size: large;"> In step 12,
    the string used should be a Unicode string instead of an ASCII
    string as it appears.   </span>
  - <span style="font-family: Calibri; font-size: large;">The detailed
    steps of deriving an AES key from a string are not specified unless
    you just use Windows API CryptDeriveKey() function.  This may cause
    problem for the implementers who don’t use the Windows
    platform.</span>

<span style="font-family: Calibri; font-size: large;">Based on the
verification using the sample implementation, we fixed the related
section in MS-RAIOP.</span>

<span style="font-family: Calibri; font-size: large;"> This proves that
the sample implementation based on the protocol example is a very
effective way to verify the protocol documentation and also provides a
good reference for the third party to implement the interoperable
solutions with the Windows platform. 
</span>

</div>

</div>

</div>

</div>

[testRAIOP.cpp](images/testRAIOP.cpp)

</div>

</div>
