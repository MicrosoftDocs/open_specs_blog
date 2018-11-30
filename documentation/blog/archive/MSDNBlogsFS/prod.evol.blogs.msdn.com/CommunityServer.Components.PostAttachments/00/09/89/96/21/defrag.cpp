// defrag.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define LESSTHAN(x,y) ((x.HighPart <= y.HighPart) && (x.LowPart < y.LowPart))
int giLevel = 0;

HRESULT CopyStgMiniStream(LPSTORAGE lpSrcStg, LPSTORAGE lpDestStg)
{
	HRESULT hr = S_OK;
	LPENUMSTATSTG lpEnumStatStg = NULL;
	hr  = lpSrcStg->EnumElements(NULL, NULL, NULL, &lpEnumStatStg);

	// The assumption here is that the two input storages are identical except for 
	// the ministream.  Therefore, we only need to enumerate on one since the other structure 
	// will be consistent.  However, when we recurse, we drop down a level in the tree which means
	// we need to do that to both storages.

	ULONG cfch = 0;
	STATSTG tstat;
	while (lpEnumStatStg->Next(1, &tstat, &cfch)==S_OK)
	{
		LPSTREAM lpStmSrcTemp = NULL;
		LPSTREAM lpStmDestTemp = NULL;
		ULARGE_INTEGER nread, nwritten;
		ULARGE_INTEGER mini_limit = {4096,0};

		// if it's a stream, it's not 0 bytes and it's less than 4096 bytes, then we copy it.
		if ((tstat.type == STGTY_STREAM) && 
			((tstat.cbSize.LowPart > 0) || (tstat.cbSize.HighPart > 0)) && 
				(LESSTHAN(tstat.cbSize, mini_limit)))
		{
			hr = lpSrcStg->OpenStream(tstat.pwcsName, NULL, 
				STGM_READ|STGM_SHARE_EXCLUSIVE, NULL, &lpStmSrcTemp);
			hr = lpDestStg->CreateStream(tstat.pwcsName, 
				STGM_WRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &lpStmDestTemp);
			hr = lpStmSrcTemp->CopyTo(lpStmDestTemp, tstat.cbSize, &nread, &nwritten);
			lpStmSrcTemp->Release();
			lpStmDestTemp->Release();
		}
		
		if (tstat.type == STGTY_STORAGE)
		{
			LPSTORAGE lpNewSrcStg = NULL;
			LPSTORAGE lpNewDestStg = NULL;			
			hr = lpSrcStg->OpenStorage(tstat.pwcsName, NULL, 
				STGM_READ|STGM_SHARE_EXCLUSIVE , NULL, NULL, &lpNewSrcStg);
			hr = lpDestStg->OpenStorage(tstat.pwcsName, NULL, 
				STGM_READWRITE|STGM_SHARE_EXCLUSIVE , NULL, NULL, &lpNewDestStg);

			CopyStgMiniStream(lpNewSrcStg, lpNewDestStg);

			lpNewSrcStg->Release(); // Caller releases.
			lpNewDestStg->Release(); // Caller releases.
		}
	}
	lpEnumStatStg->Release();
	return hr;
}

HRESULT PruneStgMiniStream(LPSTORAGE lpSrcStg)
{
	HRESULT hr = S_OK;
	LPENUMSTATSTG lpEnumStatStg = NULL;
	hr  = lpSrcStg->EnumElements(NULL, NULL, NULL, &lpEnumStatStg);
	
	ULONG cfch = 0;
	STATSTG tstat;
	while (lpEnumStatStg->Next(1, &tstat, &cfch)==S_OK)
	{
		ULARGE_INTEGER mini_limit = {4096,0};

		// if it's a stream, it's not 0 bytes and it's less than 4096 bytes, then we delete it.
		if ((tstat.type == STGTY_STREAM) && 
			((tstat.cbSize.LowPart > 0) || (tstat.cbSize.HighPart > 0)) && 
			 (LESSTHAN(tstat.cbSize, mini_limit)))
			lpSrcStg->DestroyElement(tstat.pwcsName);

		if (tstat.type == STGTY_STORAGE)
		{
			LPSTORAGE lpNewStg = NULL;
			hr = lpSrcStg->OpenStorage(tstat.pwcsName, NULL, 
				STGM_READWRITE|STGM_SHARE_EXCLUSIVE , NULL, NULL, &lpNewStg);

			PruneStgMiniStream(lpNewStg);

			lpNewStg->Release(); // Caller releases.
		}
	}

	lpEnumStatStg->Release();
	return hr;
}

HRESULT PrintStgTree(LPSTORAGE lpSrcStg)
{
	HRESULT hr = S_OK;
	LPENUMSTATSTG lpEnumStatStg = NULL;
	hr  = lpSrcStg->EnumElements(NULL, NULL, NULL, &lpEnumStatStg);

	ULONG cfch = 0;
	STATSTG tstat;
	while (lpEnumStatStg->Next(1, &tstat, &cfch)==S_OK)
	{
		int tlevel = giLevel;

		while (tlevel--)
			printf("\t");  // tab out

		printf("|__");

		printf("%S\n", tstat.pwcsName);	

		if (tstat.type == STGTY_STORAGE)
		{
			LPSTORAGE lpNewStg = NULL;
			hr = lpSrcStg->OpenStorage(tstat.pwcsName, NULL, STGM_READ|STGM_SHARE_EXCLUSIVE , NULL, NULL, &lpNewStg);
			giLevel++;  // going down into the tree, so increase depth level
			PrintStgTree(lpNewStg);
			giLevel--;  // coming back up the tree, so decrease depth level
			lpNewStg->Release(); // Caller releases.
		}

	}

	lpEnumStatStg->Release();
	return hr;
}

#define STREAM1SIZE     64*2
#define STREAM2SIZE     64
#define STREAM3SIZE     64*8

HRESULT CreateFraggedFile()
{
    char* stream1bytes = (char*)malloc(STREAM1SIZE);
    char* stream2bytes = (char*)malloc(STREAM2SIZE);
    char* stream3bytes = (char*)malloc(STREAM3SIZE);
	ULONG cb = 0;

	memset(stream1bytes, '1', STREAM1SIZE);
	memset(stream2bytes, '2', STREAM2SIZE);
	memset(stream3bytes, '3', STREAM3SIZE);

	HRESULT hr = S_OK;

	LPSTORAGE lpStg = NULL;
	hr = StgCreateStorageEx(L"frag.x", STGM_CREATE|STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
		STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, (LPVOID*) &lpStg);

	LPSTREAM lpStm1, lpStm2, lpStm3 = NULL;
	hr = lpStg->CreateStream(L"stream1", STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &lpStm1);
	hr = lpStg->CreateStream(L"stream2", STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &lpStm2);
	hr = lpStg->CreateStream(L"stream3", STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &lpStm3);

	hr = lpStm1->Write(stream1bytes, STREAM1SIZE, &cb);
	hr = lpStm2->Write(stream2bytes, STREAM2SIZE, &cb);
	lpStm1->Release();
	hr = lpStg->DestroyElement(L"stream1");
	hr = lpStg->Commit(STGC_DEFAULT);
	hr = lpStm3->Write(stream3bytes, STREAM3SIZE, &cb);


	lpStm2->Release();
	lpStm3->Release();

	hr = lpStg->Commit(STGC_DEFAULT);
	lpStg->Release();

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	int srclen = 0;


	if ((argc < 2)||((srclen = _tcslen(argv[1])) > 107)) // give 107 chars total, the path plus 7 for ".defrag" that we'll add
	{
		CreateFraggedFile();
		return -16; //help();
	}

	LPSTORAGE lpSrcStg = NULL;

	HRESULT hr = StgOpenStorage(argv[1], NULL, STGM_DIRECT | STGM_READ | STGM_SHARE_DENY_WRITE, NULL, NULL, &lpSrcStg);  // pop open the docfile and suck out the root storage
	
	if (FAILED(hr))
		return hr; //help();

	PrintStgTree(lpSrcStg);

	STATSTG StatStg;
	lpSrcStg->Stat(&StatStg, 0);
	

	// save off the extension
	_TCHAR* lptszTail = _tcsrchr((_TCHAR*)argv[1], '.' ); // find the last dot
  	
	// allocate the new path buffer and clear it
	_TCHAR* lptszTarget = new _TCHAR[108];  // allocate enough to hold the path plus the ".defrag" insertion and a null.
	memset(lptszTarget, 0x00, 108);  // clear out the remainder of the string.

	// copy the path up the the extension, insert ".defrag", then copy the extension.
	int posdot = lptszTail-argv[1];
	_tcsncpy_s(lptszTarget, posdot+1, argv[1], posdot); // copy everything before the dot.
	_tcsncpy_s(lptszTarget+posdot, 8, _T(".defrag"), 7);   // insert the ".defrag"
	_tcscpy_s(lptszTarget+posdot+7, _tcslen(lptszTail)+1, lptszTail); // copy the rest

	// allocate the new path buffer and clear it
	_TCHAR* lptszTemp = new _TCHAR[106];  // allocate enough to hold the path plus the ".temp" insertion and a null.
	memset(lptszTemp, 0x00, 106);  // clear out the remainder of the string.

	// copy the path up the the extension, insert ".defrag", then copy the extension.
	posdot = lptszTail-argv[1];
	_tcsncpy_s(lptszTemp, posdot+1, argv[1], posdot); // copy everything before the dot.
	_tcsncpy_s(lptszTemp+posdot, 6, _T(".temp"), 5);   // insert the ".defrag"
	_tcscpy_s(lptszTemp+posdot+5, _tcslen(lptszTail)+1, lptszTail); // copy the rest
	//LPSTORAGE lpDestStg = NULL;
	//hr = StgCreateDocfile(lptszTarget, STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, NULL, &lpDestStg);

	LPSTORAGE lpTempStg = NULL;
	hr = StgCreateStorageEx(lptszTemp, 
		STGM_CREATE|STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
		STGFMT_STORAGE, 0, NULL, NULL, 
		IID_IStorage, (LPVOID*) &lpTempStg);

	LPSTORAGE lpDestStg = NULL;
	hr = StgCreateStorageEx(lptszTarget, 
		STGM_CREATE|STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
		STGFMT_STORAGE, 0, NULL, NULL, 
		IID_IStorage, (LPVOID*) &lpDestStg);

	if (FAILED(hr))
		return hr;

	hr = PrintStgTree(lpSrcStg);  // before 

	hr = lpDestStg->SetClass(StatStg.clsid);
	hr = lpTempStg->SetClass(StatStg.clsid);

	hr = lpSrcStg->CopyTo(NULL, NULL, NULL, lpTempStg);

	if (FAILED(hr))
		return hr;

	// this deletes all the streams that live in the ministream
	hr = PruneStgMiniStream(lpTempStg);  
	lpTempStg->Commit(STGC_CONSOLIDATE);

	// copy everything but the ministream data
	hr = lpTempStg->CopyTo(NULL, NULL, NULL, lpDestStg);  
    hr = lpDestStg->Commit(STGC_CONSOLIDATE);

	// now copy the ministreams to the destination storage
	hr = CopyStgMiniStream(lpSrcStg, lpDestStg);  

	if (FAILED(hr))
		return hr;

	hr = lpDestStg->Commit(STGC_CONSOLIDATE);

	if (FAILED(hr))
		return hr;

	hr = PrintStgTree(lpDestStg);  // after

	lpSrcStg->Release();
	lpDestStg->Release();
	lpTempStg->Release();

	CoUninitialize();
	return 0;
}

