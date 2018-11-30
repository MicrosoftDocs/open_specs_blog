// testRAIOP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Strsafe.h"
#include "testRAIOP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


void MyHandleError(char *s)
{
	printf("An error occurred in running the program.\n");
	printf("%s\n",s);
	printf("Error number %x\n.",GetLastError());
	printf("Program terminating.\n");
	exit(1);
}

void printHex(BYTE *buf, DWORD dwSize, char*string)
{
	printf("\n");
	printf("%s (%i bytes):\n",string,dwSize);
	for (unsigned int n = 0; n < dwSize; n++)   
	{
		printf("%.02x ", buf[n]);
	}
	printf("\n");
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;
	HCRYPTHASH hHash;
	DWORD dwHashLen,dwHashLenSize;
	BYTE       *pbHash;
	BYTE       szLastHash[6];  //used in step 5

	wchar_t szConnectionString[128];
	wchar_t ValidPasswordCharacterString[30]; 

	StringCchCopyW(ValidPasswordCharacterString,29+1, L"BCDFGHJKLMNPQRSTVWXYZ23456789");

	BYTE AESKeyBlob[] = {
          0x08,0x02,0x00,0x00,0x0e,0x66,0x00,0x00, // BLOB header ,AES128
           0x10,0x00,0x00,0x00,                     // key length, in bytes
           0x00, 0x00,  0x00,  0x00,  0x00 , 0x00 , 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 0x00,  0x00 , 0x00  // AES key  value popluated late
           };

	DWORD timeSeconds= 1218745079;
	DWORD dwLength = 0;
	DWORD dwConnectionStringLength= 0;
	size_t sizeTemp;
	
	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			//
			//Initialization of the Crypto system
			//
			if ( !CryptAcquireContextW(&hCryptProv, 0, (LPCWSTR)L"Microsoft Enhanced RSA and AES Cryptographic Provider", PROV_RSA_AES, CRYPT_SILENT) )
			{
 				printf("CryptAcquireContextW failed\n");
				if ( GetLastError() == -2146893802 )
				{
					if ( !CryptAcquireContextW(&hCryptProv, 0, (LPCWSTR)L"Microsoft Enhanced RSA and AES Cryptographic Provider", 0x18u, 0xF0000048u) )
						MyHandleError("Error during CryptAcquireContext!");
					if( GetLastError() != 0)
						MyHandleError("Error during CryptAcquireContext!");
				}
			}

			/////////////////////////////////////////////////////////////////////////
			//Step 1: Assume for the connection string "SAMPLE", when encryption is done, 
			//        1218745079 seconds have elapsed since January 1, 1970 UTC. 
			/////////////////////////////////////////////////////////////////////////
			StringCchCopyW(szConnectionString, 6+1, L"SAMPLE");
			if (SUCCEEDED(StringCchLength(szConnectionString,128,(size_t*)&sizeTemp))) 
				dwConnectionStringLength = (DWORD)sizeTemp;

			printHex ( (BYTE*) &szConnectionString[0],dwConnectionStringLength * sizeof(wchar_t), "Connection String (Step 1):");
            
			//////////////////////////////////////////////////////////////////////////
			//Step 2:  Padding 20 bytes (10 wchar) to the connection string ("SAMPLE") 
			//////////////////////////////////////////////////////////////////////////
			wchar_t padding[10];
			wmemset(padding,0,10);

			//add 20 bytes of zero for initial padding
			wmemcpy(&szConnectionString[6],padding,10);
			dwLength = dwConnectionStringLength + 10;

			printHex ( (BYTE*) &szConnectionString[0],dwLength * sizeof(wchar_t), "Inuput for first hash:");


			////////////////////////////////////////////////////////////////////////////
			//Step 3,4 & 5: for the 100,000 hash iterations.  The hash value of the each
			//              iteration is copied to the last 20 bytes of the input buffer
			//              for the next iteration
			////////////////////////////////////////////////////////////////////////////
			for (int it = 0 ; it< 100000;it++)
			{
	    		if(!CryptCreateHash(hCryptProv, CALG_SHA, 0, 0, &hHash))
				  MyHandleError("Error during CryptCreateHash!");

	    		if(!CryptHashData(hHash, (BYTE *)szConnectionString, dwLength * sizeof(wchar_t), 0))
				  MyHandleError("Error during CryptHashData!");

				 if(!CryptGetHashParam(
					   hHash,
					   HP_HASHSIZE,
					   (BYTE *)&dwHashLen,
					   &dwHashLenSize,
					   0))
					MyHandleError("CryptGetHashParam failed to get size.");

				if(!(pbHash = (BYTE*)malloc(dwHashLen)))
					 MyHandleError("Allocation failed.");

				if(!CryptGetHashParam(
				   hHash,
				   HP_HASHVAL,
				   pbHash,
				   &dwHashLen,
				   0))
					MyHandleError("Error during reading hash value.");

				//This is the output of value for the Step 3
				if (it == 0)
				{
					printHex ( (BYTE*)pbHash,dwHashLen, "The first hash output (step 9):");
				}
				else if (it == 99999)  //last iteration
				{
					//This is the hash value of the last hash iteration
					printHex ( (BYTE*)pbHash,6, "The final hash  (step 5):");
					memcpy((BYTE*) &szLastHash[0],pbHash,6);
					break;
				}

				memcpy((BYTE*) &szConnectionString[dwConnectionStringLength],pbHash,dwHashLen);
				free(pbHash);
				CryptDestroyHash(hHash);
			}


			////////////////////////////////////////////////////////////////////////////
			//Step 6:  Use the first six bytes of hash values to compute the index to 
			//         ValidPasswordCharacterString array for each byte.
			////////////////////////////////////////////////////////////////////////////
			for (int j = 0;j <6;j++)
			  szConnectionString[j] = ValidPasswordCharacterString[szLastHash[j] * 29 / 256];
			szConnectionString[6] =0;
			printHex ( (BYTE*)&szConnectionString[0],6*sizeof(wchar_t), "The string from the valid password character  (step 6):");


			////////////////////////////////////////////////////////////////////////////
			//Step 7:  When the number of seconds since Jan 1, 1970 is divided by 3600, the 
			//         resulting figure is the number of hours that have elapsed since 
			//         Jan 1, 1970 UTC. This number is concatenated to the derived characters  
			////////////////////////////////////////////////////////////////////////////
			wchar_t szTemp[32];
			StringCchPrintf(szTemp, 32,L"%d", timeSeconds/3600);

			StringCchCatW(&szConnectionString[0],128,szTemp);

			if (SUCCEEDED(StringCchLengthW(&szConnectionString[0],128,&sizeTemp)))
				dwConnectionStringLength = (DWORD)sizeTemp;

			printHex (  (BYTE*)&szConnectionString[0], dwConnectionStringLength*sizeof(wchar_t) , "After adding the time  (step 7):");

			////////////////////////////////////////////////////////////////////////////
			//step 8,9,10,11 :   Do 100,000 iterations of hash, copying the output
			//                   of the previous iteration to the input of next iteration   
			////////////////////////////////////////////////////////////////////////////
			wmemset(padding,0,10);
			wmemcpy(&szConnectionString[dwConnectionStringLength],padding,10);
			dwLength = dwConnectionStringLength+ 10;

			//output the result for step 8
			printHex (  (BYTE*)&szConnectionString[0], dwLength *sizeof(wchar_t) , "The first hash input after padding 20 bytes of zeros  (step 8):");

			for (int it = 0 ; it< 100000;it++)
			{

				if(!CryptCreateHash(hCryptProv, CALG_SHA, 0, 0, &hHash))
					  MyHandleError("Error during CryptCreateHash!");

		    	if(!CryptHashData(hHash, (BYTE *)szConnectionString, dwLength * sizeof(wchar_t), 0))
					  MyHandleError("Error during CryptHashData!");

				 if(!CryptGetHashParam(
						   hHash,
						   HP_HASHSIZE,
						   (BYTE *)&dwHashLen,
						   &dwHashLenSize,
						   0))
					MyHandleError("CryptGetHashParam failed to get size.");

				if(!(pbHash = (BYTE*)malloc(dwHashLen)))
				{
					 MyHandleError("Allocation failed.");
  					return 1;
				}

				if(!CryptGetHashParam(
					   hHash,
					   HP_HASHVAL,
					   pbHash,
					   &dwHashLen,
					   0))
					MyHandleError("Error during reading hash value.");


				if (it == 0)  //output the result for step 9
					printHex ( (BYTE*)pbHash,dwHashLen, "The first hash output (step 9):");
				else if (it == 99999)
				{
					//output the hash result in step 11
					printHex ( (BYTE*)pbHash,16, "The first 16 bytes of the final hash  (step 11):");

					//convert it to the HexConvertedUnicodeString
					wchar_t temp[20];
					wchar_t *pInput = (wchar_t*)  szConnectionString;
					pInput[0] = 0;
					for (int j = 0;j <16;j++)
					{
						swprintf((wchar_t*)&temp[0],20,L"%X",pbHash[j]);
						wcscat((wchar_t *)&pInput[0],temp);
					}
					dwLength = 64;

					//output the hash result in step 11
					printHex ( (BYTE*)pInput,dwLength, "The hex converted string format of the hash result (step 12):");

					free(pbHash);
    				CryptDestroyHash(hHash);
					break;
				}

				//copy the hash value into the hash input buffer for the next iteration 
				memcpy((BYTE*) &szConnectionString[dwConnectionStringLength],pbHash,dwHashLen);

				//output the result for step 10
				if (it ==0)
					printHex (  (BYTE*)&szConnectionString[0], dwLength *sizeof(wchar_t) , "The second hash input after copying the first hash value (step 10):");

				free(pbHash);
				CryptDestroyHash(hHash);
			}

			////////////////////////////////////////////////////////////////////////////
			//Step 13: Using the SHA-1 hash algorithm, hash the key string from step 12. 
			////////////////////////////////////////////////////////////////////////////
    		if(!CryptCreateHash(hCryptProv, CALG_SHA, 0, 0, &hHash))
				  MyHandleError("Error during CryptCreateHash!");

    		if(!CryptHashData(hHash, (BYTE *)szConnectionString, dwLength , 0))
				  MyHandleError("Error during CryptHashData!");

			 if(!CryptGetHashParam(
				   hHash,
				   HP_HASHSIZE,
				   (BYTE *)&dwHashLen,
				   &dwHashLenSize,
				   0))
     			MyHandleError("CryptGetHashParam failed to get size.");

			if(!(pbHash = (BYTE*)malloc(dwHashLen)))
				 MyHandleError("Allocation failed.");

			if(!CryptGetHashParam(
			   hHash,
			   HP_HASHVAL,
			   pbHash,
			   &dwHashLen,
			   0))
				MyHandleError("Error during reading hash value.");

			memcpy ((BYTE*)  szConnectionString,(BYTE*)pbHash,dwHashLen);
			printHex ((BYTE*)szConnectionString,dwHashLen, "The hash value of the hex key string in step 12 (step 13):");

			free(pbHash);
			CryptDestroyHash(hHash);

			////////////////////////////////////////////////////////////////////////////
			//Step 14:  Using the resultant hash in step 13 and the following logic to 
			//          derive the AES 128 hash key
			////////////////////////////////////////////////////////////////////////////
			char buffer1[64];
			BYTE *pInput = (BYTE*)  &szConnectionString[0];

			memset((char*) &buffer1[0],0x36,64);

			for (int i=0;i<20;i++)
				buffer1[i] = buffer1[i] ^  pInput[i];

			if(!CryptCreateHash(hCryptProv, CALG_SHA, 0, 0, &hHash))
				MyHandleError("Error during CryptCreateHash!");

			if(!CryptHashData(hHash, (BYTE *)buffer1, 64, 0))
				MyHandleError("Error during CryptHashData!");

			if(!CryptGetHashParam(
				hHash,
				HP_HASHSIZE,
				(BYTE *)&dwHashLen,
				&dwHashLenSize,
			    0))
		        MyHandleError("CryptGetHashParam failed to get size.");
    
		   if(!(pbHash = (BYTE*)malloc(dwHashLen)))
              MyHandleError("Allocation failed.");

			if(!CryptGetHashParam(
			   hHash,
			   HP_HASHVAL,
			   pbHash,
			   &dwHashLen,
			 0))
				MyHandleError("Error during reading hash value.");

			memcpy((BYTE*) &AESKeyBlob[12],pbHash,16);

			printHex ( (BYTE*) &AESKeyBlob[12],16, "The AES 128 key drived from the resulatnt hash in step 13 (step 14):");
			free(pbHash);
			CryptDestroyHash(hHash);

			////////////////////////////////////////////////////////////////////////////
			//Step 15: encrypt original connection string "SAMPLE: with the cipher key 
			//         in step 14.
			////////////////////////////////////////////////////////////////////////////
			StringCchCopyW(szConnectionString, 6+1, L"SAMPLE");
			if (SUCCEEDED(StringCchLength(szConnectionString,128,(size_t*)&sizeTemp))) 
				dwConnectionStringLength = (DWORD)sizeTemp;
			dwConnectionStringLength = dwConnectionStringLength * sizeof(wchar_t); 


			//Create a key from the key blob
			if (!CryptImportKey(hCryptProv,
		               (BYTE*) &AESKeyBlob[0],
					   sizeof(AESKeyBlob),
					   0,
					   CRYPT_EXPORTABLE,
						&hKey))
				MyHandleError("Error create a AES key from the  key blob");

			//Encrpyt the connection string "SAMPLE" using the key 
         	if (!CryptEncrypt(
            hKey,
			NULL,
			TRUE,
             0,
   			 (BYTE*) &szConnectionString[0],
			 &dwConnectionStringLength,
			 128))
				MyHandleError("Error encrypting the connection string");

		    printHex (  (BYTE*) &szConnectionString[0],dwConnectionStringLength, "Encrypted data: (Step 15)");

			////////////////////////////////////////////////////////////////////////////
			//final clean up
			////////////////////////////////////////////////////////////////////////////
			if(hKey)
	 			if(!(CryptDestroyKey(hKey)))
					MyHandleError("Error during CryptDestroyKey");

		    if(hCryptProv)
				if(!(CryptReleaseContext(hCryptProv, 0)))
					MyHandleError("Error during CryptReleaseContext");
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
