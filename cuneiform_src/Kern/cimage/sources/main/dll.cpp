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

// ============================================================================
// Copyright (c) 1998 Cognitive Technology Ltd.
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"

#include "cticontrol.h"

#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static Word16            gwHeightRC = 0;
static Word16            gwLowRC =    0;
static HINSTANCE         ghInst =     NULL;
CTIControl *      Control_cti =    NULL;
static Int32             InitCount =  0;
/////////////////////////////////////////
#undef APIENTRY
#define APIENTRY

BOOL APIENTRY DllMain( HINSTANCE  hModule,
                        DWORD ul_reason_for_call,
                        LPVOID lpReserved )
{
    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
		ghInst = hModule;
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Init(Word16 wHeightCode,Handle hStorage)
{
	gwHeightRC = wHeightCode;

	if ( !Control_cti )
	{
		if ( !InitCFIOInterface(TRUE) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_OTHER_DLL_NOT_INITIALIZED);
			return FALSE;
		}

		Control_cti = new CTIControl;
	}

	if ( Control_cti )
	{
		InitCount++;
		return TRUE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Done()
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
	{
		if (--InitCount == 0)
		{
			delete Control_cti;
			Control_cti = NULL;
			InitCFIOInterface(FALSE);
		}
		return TRUE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Word32) CIMAGE_GetReturnCode()
{
	if ( !gwLowRC )
		return 0;

	return (Word32)(gwHeightRC<<16)|(gwLowRC - IDS_CIMAGE_ERR_NO);
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(PInt8) CIMAGE_GetReturnString(Word32 dwError)
{
	Word16 rc = (Word16)(dwError & 0xFFFF) + IDS_CIMAGE_ERR_NO;
	static Char8 szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;

	if( rc >= IDS_CIMAGE_ERR_NO )
		LoadString(ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return (PInt8)szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
#define CASE_FUNCTION(a)	case CIMAGE_FN_##a:	*(FNCIMAGE##a *)pData = CIMAGE_##a; break
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetExportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;


	switch(dwType)
	{
	CASE_FUNCTION(WriteCallbackImage);
	CASE_FUNCTION(GetCallbackImage);
	CASE_FUNCTION(WriteDIB);
	CASE_FUNCTION(ReadDIB);
	CASE_FUNCTION(GetData);
	CASE_FUNCTION(GetDIBData);
	CASE_FUNCTION(ReplaceData);
	CASE_FUNCTION(GetImageInfo);
	CASE_FUNCTION(DeleteImage);
	CASE_FUNCTION(FreeCopedDIB);
	CASE_FUNCTION(FreeBuffers);
	CASE_FUNCTION(Reset);
	CASE_FUNCTION(AddReadCloseRects);
	CASE_FUNCTION(RemoveReadCloseRects);
	CASE_FUNCTION(AddWriteCloseRects);
	CASE_FUNCTION(RemoveWriteCloseRects);

	default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_SetImportData(Word32 dwType, void * pData)
{
	BOOL rc = FALSE;
	gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
void SetReturnCode_cimage(Word16 rc)
{
	if ( rc == IDS_CIMAGE_ERR_NO || gwLowRC == IDS_CIMAGE_ERR_NO )
		gwLowRC = rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
Word16 GetReturnCode_cimage()
{
	return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
