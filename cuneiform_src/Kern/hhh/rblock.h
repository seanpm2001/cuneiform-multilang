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

#ifndef __RBLOCK_H
#define __RBLOCK_H

#ifndef __GLOBUS_H
#include "globus.h"
#endif
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __RBLOCK__
  #define RBLOCK_FUNC  FUN_EXPO
#else
  #define RBLOCK_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#pragma pack (push,8)

#define RBLOCK_MAXNAME 260




RBLOCK_FUNC(Bool32) RBLOCK_Init(Word16 wHeightCode,Handle hStorage);
RBLOCK_FUNC(Bool32) RBLOCK_Done();
RBLOCK_FUNC(Word32) RBLOCK_GetReturnCode();
RBLOCK_FUNC(Int8 *) RBLOCK_GetReturnString(Word32 dwError);
RBLOCK_FUNC(Bool32) RBLOCK_GetExportData(Word32 dwType, void * pData);
RBLOCK_FUNC(Bool32) RBLOCK_SetImportData(Word32 dwType, void * pData);
/////////////////////////////////////////////////////////////


typedef enum
{
		RBLOCK_FNRBLOCK_ExtractTextBlocks = 1,
		RBLOCK_FNRBLOCK_ExtractTextStrings,
		RBLOCK_FNRBLOCK_GetAnglePage,
		RBLOCK_FNRBLOCK_ProgressStart,
		RBLOCK_FNRBLOCK_ProgressStep,
		RBLOCK_FNRBLOCK_ProgressFinish,
        RBLOCK_Bool32_SearchPicture,
		RBLOCK_Bool32_OneColumn

} RBLOCK_EXPORT_ENTRIES;


#define DEC_FUN(a,b,c) typedef a (*FN##b)c; RBLOCK_FUNC(a) b c

DEC_FUN(Bool32, RBLOCK_ExtractTextBlocks,(Handle hCCOM, Handle hCPAGE, Handle hCLINE));
DEC_FUN(Bool32, RBLOCK_ExtractTextStrings,(Handle hCCOM,Handle hCPAGE));
DEC_FUN(Bool32, RBLOCK_GetAnglePage,(Handle hCCOM,Int32 * lpNominator,Int32 * lpDenominator));
#undef DEC_FUN

typedef void   (*FNRBLOCK_ProgressStart)( void );
typedef	Bool32 (*FNRBLOCK_ProgressStep)(Word32 perc);
typedef	void   (*FNRBLOCK_ProgressFinish)( void );


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
