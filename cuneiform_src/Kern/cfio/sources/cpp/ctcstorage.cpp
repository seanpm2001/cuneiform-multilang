/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

      * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
        ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
        ����� �� ��������.
      * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
        ������ ����������, ������������ ��� ���������������, ������ �����������
        ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
        ����������� ����� �� ��������.
      * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
        ���� ������������ � �������� �������� ��������� �/��� �����������
        ���������, ���������� �� ���� ��, ��� ���������������� �����������
        ����������.

��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
������������� ������, ��������� � �������������� ��� ���������� ����������
������������� ������������� ��������� (������� ������ ������, ��� ������,
������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
�� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
�������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//                    Cognitive Technologies Ltd.                               //
//                                                                              //
//                    Data Storage Container                                    //
//                                                                              //
//                                                                              //
//                                                                              //
//                                                                              //
//                    started at 25 may 1998                                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
//
//#define CFIO_USE_WIN32_API
#include "ctccontrol.h"
//#undef  CFIO_USE_WIN32_API
#ifndef  _MSC_VER
#include <unistd.h>
#endif
#include "compat_defs.h"
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageHeader::CTCStorageHeader():CTCGlobalHeader()
{
	pStorageFile = NULL;
	pcFolder[0] = 0;
	pcName[0] = 0;
	wContensCounter = 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
	static char ShFolder[_MAX_PATH];
	static char ShFile[_MAX_PATH + 4];
	static char ShExtension[_MAX_PATH];
	static char ShBuffer[_MAX_PATH + 4];
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageHeader::CTCStorageHeader(CTCGlobalFile * pNewStorage,
								   Word32 wNewFlag,
								   const char *pcNewStorageFolder )
								   :CTCGlobalHeader(pNewStorage,NULL,0,wNewFlag)//, Contents()
{
	extern CTCControl * Control_ctc;

	SetHandle(AcceptFile(pNewStorage));
	SetFlag(wNewFlag);
	SetHeaderSize( sizeof( class CTCFileHeader ) );

	if ( pcNewStorageFolder && pcNewStorageFolder[0] != 0x0)
	{
		CFIO_GETFOLDERSITEMS(pcNewStorageFolder, ShFolder, ShFile, ShExtension);
		// ����� ��������� ����������
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		// ���������� ����
		CFIO_MAKEPATH(pcFolder, ShFolder, ShFile, ShExtension);
	}
	else
	{
		Control_ctc->GetFolder(CFIO_TEMP_FOLDER, ShFolder);
		CFIO_MAKEFOLDER(ShFolder);
		//CreateDirectory(ShFolder, NULL);
		if ( GetTempFileName(ShFolder,"STG",0, ShFile) )
		{
			unlink(ShFile);
			CFIO_STRCPY(ShBuffer, ShFile);
			CFIO_GETFOLDERSITEMS(ShBuffer, ShFolder, ShFile, ShExtension);
			CFIO_MAKEPATH(pcFolder, ShFolder, ShFile, NULL);
			//CFIO_STRCPY(pcFolder, ShFile);
		}
		else
		{
#ifdef _DEBUG
            Word32 Err = GetLastError();
            // ��������� �������� �� ������  �������������������������������
			LPVOID lpMsgBuf;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				Err,                                       //GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL
			);

			// Display the string.
			MessageBox ( NULL,
				         ( PChar8 ) lpMsgBuf,
						 "CFIO: Storage can't create own unpack folder",
						 MB_OK|MB_ICONINFORMATION );

			// Free the buffer.
			LocalFree( lpMsgBuf );
			Control_ctc->GetFolder(CFIO_STORAGE_FOLDER, ShFile);
			//  ������� ��������� ����������������������������������������
#endif      // _DEBUG
			CFIO_STRCPY(pcFolder,ShFile);
			//MAKEPATH(pcFolder,NULL,ShFolder,ShFile,ShExtension);
		}
	}
	//MAKEFULLPATH(pcFolder,StorageFolder,_MAX_PATH);
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageHeader::~CTCStorageHeader()
{
	if ( GetStorage() )
	{
		delete GetStorage();
	}

}
//////////////////////////////////////////////////////////////////////////////////
//
/*
Bool32 CTCStorageHeader::AddItemToStorage(Handle hNewItem, Word32 wID, Word32 wNewSize)
{
	CTCStorageContents * pCurrent, * New;

	for ( pCurrent = &Contents; pCurrent->GetNext() != NULL; pCurrent = pCurrent->GetNext() )
	{}

	New      = new CTCStorageContents(hNewItem, wID, wNewSize, NULL);
	pCurrent->SetNext(New);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCStorageHeader::DeleteItemFromStorage(Handle Item)
{
	CTCStorageContents * pCurrent, * Erase;

	for ( pCurrent = ((Erase = &Contents)->GetNext()); pCurrent != NULL; pCurrent = pCurrent->GetNext() )
	{
		if ( pCurrent->GetHandle() == Item )
		{
			Erase->SetNext(pCurrent->GetNext());
			delete pCurrent;
			return TRUE;
		}

		Erase = pCurrent;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCStorageHeader::EnumItemContents(Handle Item)
{
	CTCStorageContents * pCurrent;

	if ( Item )
	{
		for( pCurrent = Contents.GetNext(); pCurrent != NULL; pCurrent = pCurrent->GetNext() )
		{
			if ( pCurrent->GetHandle() == Item )
				if ( pCurrent->GetNext() )
					return pCurrent->GetNext()->GetHandle();
				else
					return NULL;
		}
		return NULL;
	}

	return Contents.GetNext()->GetHandle();
}
*/
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageList::CTCStorageList()
{
	msFirstItem.SetNext(&msLastItem);
	msFirstItem.SetSize(0);
	msLastItem.SetSize(0);
	msFirstItem.SetHandle(FICTIV_Handle);
	msLastItem.SetHandle(FICTIV_Handle);

	pList = NULL;
	wItemCounter = 0;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageList::~CTCStorageList()
{

}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCStorageList::AddItem(CTCGlobalFile * pNewStorage, Word32 wNewFlag)
{
	CTCStorageHeader * Current, * NewBlock = NULL;
	Handle NewHandle = pNewStorage->GetFileHandle();

	if( !NewHandle )
		return NULL;

	for ( Current = pFirst(); Current->GetNext() != pLast(); Current = Current->GetNext() )
		if ( Current->GetHandle() == pNewStorage->GetFileHandle() )
		{
			return NULL;
		}

	NewBlock = new CTCStorageHeader(pNewStorage, wNewFlag, "");
	NewBlock->SetNext(Current->GetNext());
	Current->SetNext(NewBlock);
	IncreaseItemCounter();

	return NewHandle;
}
//////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTCStorageList::DeleteItem(Handle Storage, Word32 Flag)
{
	CTCStorageHeader * Current, * Last, * EraseBlock;
	Word32 IsOK    = 0;

	for ( Last = Current = pFirst(); Current != pLast(); Current = Current->GetNext() )
	{
		if ( Current->GetHandle() == Storage )
		{
			EraseBlock = Current;
			DecreaseItemCounter();
			Last->SetNext(Current->GetNext());
			delete EraseBlock;
			IsOK++;
			Current = Last;
		}
		else
		{
			Last = Current;
		}
	}
	return ( IsOK == 1 );
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCGlobalFile * CTCStorageList::GetItem(Handle Storage)
{
	CTCStorageHeader * pCurrent;
	CTCGlobalFile * pFounded = NULL;

	if ( pCurrent = GetItemHeader(Storage)  )
	{
		pFounded = pCurrent->GetStorageFile();
	}

	return pFounded;
}
//////////////////////////////////////////////////////////////////////////////////
//
CTCStorageHeader * CTCStorageList::GetItemHeader(Handle Storage)
{
	CTCStorageHeader * pCurrent;

	for ( pCurrent = pFirst(); pCurrent != pLast(); pCurrent = pCurrent->GetNext())
	{
		if ( pCurrent->GetHandle() == Storage  )
		{
			return pCurrent;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
Handle CTCStorageList::FindStorage(PChar8 lpStorageName)
{
	CTCGlobalFile * pStorage;
	CTCStorageHeader * pCurrent;

	for ( pCurrent = pFirst(); pCurrent != pLast(); pCurrent = pCurrent->GetNext())
	{
		 pStorage = pCurrent->GetStorage();

		 if ( pStorage )
			 if ( strcmp (pStorage->GetFileName(), lpStorageName ) == 0 )
				return pCurrent->GetHandle();
	}
	return NULL;

}
//////////////////////////////////////////////////////////////////////////////////
//end of file




