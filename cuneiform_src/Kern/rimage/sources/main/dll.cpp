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
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

#include "resource.h"
#include "cridefines.h"
#include "crimemory.h"
#include "cricontrol.h"
#include "criimage.h"
//#include "dpuma.h"
#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static Word16            gwHeightRC = 0;
static Word16            gwLowRC = 0;
static Handle            ghStorage = NULL;
static Handle            ghInst =  NULL;
CRIControl *      Control_cri = NULL;
static Int32             InitCount =  0;

Bool32 InitCFIOInterface(Bool32 Status);
/////////////////////////////////////////
BOOL APIENTRY DllMain( HINSTANCE hModule,
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
RIMAGE_FUNC(Bool32) RIMAGE_Init(Word16 wHeightCode,Handle hStorage)
{

	if ( !Control_cri )
	{
		if ( InitCFIOInterface(TRUE) )
		{
			Control_cri = new CRIControl;
			gwHeightRC = wHeightCode;
		}
		else
			return FALSE;
	}

	if ( Control_cri )
	{
		InitCount++;
		return TRUE;
	}

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32)RIMAGE_Done()
{

	if ( Control_cri )
	{
		if (--InitCount == 0)
		{
			delete Control_cri;
			Control_cri = NULL;
			InitCFIOInterface(FALSE);
		}
		return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32)RIMAGE_Reset()
{

	if ( Control_cri )
	{
		if (InitCount == 1)
		{
			delete Control_cri;
			Control_cri = new CRIControl;
			return TRUE;
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Word32) RIMAGE_GetReturnCode()
{
	if ( !gwLowRC )
		return 0;

	return (Word32)(gwHeightRC<<16)|(gwLowRC - IDS_RIMAGE_ERR_NO);
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Int8 *) RIMAGE_GetReturnString(Word32 dwError)
{
	Word16 rc = (Word16)(dwError & 0xFFFF) + IDS_RIMAGE_ERR_NO;
	static Int8 szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;

	if( rc >= IDS_RIMAGE_ERR_NO )
		LoadString((HINSTANCE)ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
#define CASE_FUNCTION(a)	case RIMAGE_FN_##a:	*(FNRIMAGE##a *)pData = RIMAGE_##a; break
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_GetExportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;


	switch(dwType)
	{
	CASE_FUNCTION(SetMargins);
	CASE_FUNCTION(Binarise);
	CASE_FUNCTION(Rotate);
	CASE_FUNCTION(Turn);
	CASE_FUNCTION(Inverse);

	default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_SetImportData(Word32 dwType, void * pData)
{
	BOOL rc = FALSE;
	gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;

	switch(dwType)
	{
	case RIMAGE_FN_SetProgressStart:
		Control_cri->SetProgressCallBacks((PRIMAGECBPRogressStart)pData, NULL, NULL);
		rc = TRUE;
		break;
	case RIMAGE_FN_SetProgressStep:
		Control_cri->SetProgressCallBacks(NULL, (PRIMAGECBPRogressStep)pData, NULL);
		rc = TRUE;
		break;
	case RIMAGE_FN_SetProgressFinish:
		Control_cri->SetProgressCallBacks(NULL, NULL, (PRIMAGECBPRogressFinish)pData);
		rc = TRUE;
		break;
	default:
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
void SetReturnCode_rimage(Word16 rc)
{
	if ( rc == IDS_RIMAGE_ERR_NO || gwLowRC == IDS_RIMAGE_ERR_NO )
		gwLowRC = rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
Word16 GetReturnCode_rimage()
{
	return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
