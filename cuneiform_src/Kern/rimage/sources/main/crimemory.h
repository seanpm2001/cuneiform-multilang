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

#ifndef __CTI_MEMORY_H__
#define __CTI_MEMORY_H__
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

/*#include <crtdbg.h>*/
//////////////////////////////////////////////////////////////////////////////////////////
//
#include "criimage.h"
//#ifndef Handle
//	#define Handle void *
//#endif
///////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef _DEBUG
	#ifndef IS_VALID
		#define IS_VALID(a) _ASSERT(_CrtIsValidPointer(a, 1, TRUE ))
	#endif
#else
	#ifndef IS_VALID
		#define IS_VALID(a)
	#endif
#endif
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	extern "C" {
#endif
///////////////////////////////////////////////////////////////////////////////////
void SetReturnCode_rimage(Word16 rc);
Word16 GetReturnCode_rimage();
/////////////////////////////////////////////////////////////////////////////////////////
//
void    RIMAGEComment(const char *Comment);
void *	RIMAGEAlloc(Word32 stAllocateBlock);
void *	RIMAGEDAlloc(Word32 stAllocateBlock, const char *Comment);
void	RIMAGEFree(void * mem);
void *  RIMAGELock(void * mem);
void    RIMAGEUnlock(void * mem);
/////////////////////////////////////////////////////////////////////////////////////////
//
Handle  RIMAGEOpenSave(char * lpName);
Handle  RIMAGEOpenRestore(char * lpName);
/////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int  RIMAGEWrite(Handle h,void * lpdata,unsigned int size);
unsigned int  RIMAGERead(Handle h,void * lpdata,unsigned int size);
/////////////////////////////////////////////////////////////////////////////////////////
//
void    RIMAGEClose(Handle h);
/////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef __cplusplus
            }
#endif
/////////////////////////////////////////////////////////////////////////////////////////
//
#endif
/////////////////////////////////////////////////////////////////////////////////////////
//end of file
