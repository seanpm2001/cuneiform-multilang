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

#ifndef __CLINE_H
#define __CLINE_H

#include "clinedefs.h"

#ifndef __GLOBUS_H
#include "globus.h"
#endif

#ifdef __CLINE__
  #define CLINE_FUNC  FUN_EXPO
#else
  #define CLINE_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
        extern "C" {
#endif

#pragma pack (push,8)

#define CLINE_MAXNAME 260

//#ifndef Handle
 //       #define Handle void *
//#endif

CLINE_FUNC(Bool32) CLINE_Init(Word16 wHeightCode,Handle parent);
CLINE_FUNC(Bool32) CLINE_Done();
CLINE_FUNC(Word32) CLINE_GetReturnCode();
CLINE_FUNC(Int8 *) CLINE_GetReturnString(Word32 dwError);
CLINE_FUNC(Bool32) CLINE_GetExportData(Word32 dwType, void * pData);
CLINE_FUNC(Bool32) CLINE_SetImportData(Word32 dwType, void * pData);
/////////////////////////////////////////////////////////////




typedef enum
{
	CLINE_FNCLINE_Reset = 1,
	CLINE_FNCLINE_CreateContainer,
	CLINE_FNCLINE_DeleteContainer,
	CLINE_FNCLINE_GetFirstContainer,
	CLINE_FNCLINE_GetNextContainer,
	CLINE_FNCLINE_GetMainContainer,
	CLINE_FNCLINE_CleanContainer,
	CLINE_FNCLINE_GetLineCount,
	CLINE_FNCLINE_GetFirstLine,
	CLINE_FNCLINE_GetNextLine,
	CLINE_FNCLINE_AddNewLine,
	CLINE_FNCLINE_DelLine,
	CLINE_FNCLINE_DelAllLines,
	CLINE_FNCLINE_GetLineData,
	CLINE_FNCLINE_SetLineData,
	CLINE_FNCLINE_GetEventCount,
	CLINE_FNCLINE_GetFirstEvent,
	CLINE_FNCLINE_GetNextEvent,
	CLINE_FNCLINE_AddNewEvent,
	CLINE_FNCLINE_DelEvent,
	CLINE_FNCLINE_DelAllEvents,
	CLINE_FNCLINE_GetEventData,
	CLINE_FNCLINE_SetEventData,
	CLINE_FNCLINE_GetCutPointCount,
	CLINE_FNCLINE_GetFirstCutPoint,
	CLINE_FNCLINE_GetNextCutPoint,
	CLINE_FNCLINE_AddNewCutPoint,
	CLINE_FNCLINE_DelCutPoint,
	CLINE_FNCLINE_DelAllCutPoints,
	CLINE_FNCLINE_GetCutPointData,
	CLINE_FNCLINE_SetCutPointData,
	CLINE_FNCLINE_GetCompCount,
	CLINE_FNCLINE_GetFirstComp,
	CLINE_FNCLINE_GetNextComp,
	CLINE_FNCLINE_AddNewComp,
	CLINE_FNCLINE_DelComp,
	CLINE_FNCLINE_DelAllComps,
	CLINE_FNCLINE_GetCompData,
	CLINE_FNCLINE_SetCompData,
	CLINE_FNCLINE_GetEventInvCount,
	CLINE_FNCLINE_GetFirstEventInv,
	CLINE_FNCLINE_GetNextEventInv,
	CLINE_FNCLINE_AddNewEventInv,
	CLINE_FNCLINE_DelEventInv,
	CLINE_FNCLINE_DelAllEventInvs,
	CLINE_FNCLINE_GetEventInvData,
	CLINE_FNCLINE_SetEventInvData,
	CLINE_FNCLINE_GetCutPointInvCount,
	CLINE_FNCLINE_GetFirstCutPointInv,
	CLINE_FNCLINE_GetNextCutPointInv,
	CLINE_FNCLINE_AddNewCutPointInv,
	CLINE_FNCLINE_DelCutPointInv,
	CLINE_FNCLINE_DelAllCutPointInvs,
	CLINE_FNCLINE_GetCutPointInvData,
	CLINE_FNCLINE_SetCutPointInvData,
	CLINE_FNCLINE_CopyData
} CLINE_EXPORT_ENTRIES;


/*  �������� �������  */
#define DEC_FUN(a,b,c) typedef a (*FN##b)c; CLINE_FUNC(a) b c
DEC_FUN(void, CLINE_Reset, (void));
DEC_FUN(CLINE_handle, CLINE_CreateContainer, (Bool32 Main));
DEC_FUN(void, CLINE_DeleteContainer, (CLINE_handle container));
DEC_FUN(CLINE_handle, CLINE_GetFirstContainer, (void));
DEC_FUN(CLINE_handle, CLINE_GetNextContainer, (CLINE_handle container));
DEC_FUN(CLINE_handle, CLINE_GetMainContainer, (void));
DEC_FUN(void, CLINE_CleanContainer, (CLINE_handle container));

DEC_FUN(int, CLINE_GetLineCount, (CLINE_handle container));
DEC_FUN(CLINE_handle, CLINE_GetFirstLine, (CLINE_handle container));
DEC_FUN(CLINE_handle, CLINE_GetNextLine, (CLINE_handle line_prev));
DEC_FUN(CLINE_handle, CLINE_AddNewLine, (CLINE_handle container));
DEC_FUN(void, CLINE_DelLine, (CLINE_handle container,CLINE_handle line));
DEC_FUN(void, CLINE_DelAllLines, (CLINE_handle container));
DEC_FUN(CPDLine, CLINE_GetLineData, (CLINE_handle line));
DEC_FUN(Bool32, CLINE_SetLineData, (CLINE_handle line,CPDLine cpdata));

DEC_FUN(int, CLINE_GetEventCount, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetFirstEvent, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetNextEvent, (CLINE_handle event));
DEC_FUN(CLINE_handle, CLINE_AddNewEvent, (CLINE_handle line));
DEC_FUN(void, CLINE_DelEvent, (CLINE_handle line,CLINE_handle event));
DEC_FUN(void, CLINE_DelAllEvents, (CLINE_handle line));
DEC_FUN(CPDEvent, CLINE_GetEventData, (CLINE_handle event));
DEC_FUN(Bool32, CLINE_SetEventData, (CLINE_handle event,CPDEvent cpdata));

DEC_FUN(int, CLINE_GetCutPointCount, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetFirstCutPoint, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetNextCutPoint, (CLINE_handle cupoint));
DEC_FUN(CLINE_handle, CLINE_AddNewCutPoint, (CLINE_handle line));
DEC_FUN(void, CLINE_DelCutPoint, (CLINE_handle line,CLINE_handle cupoint));
DEC_FUN(void, CLINE_DelAllCutPoints, (CLINE_handle line));
DEC_FUN(CPDCutPoint, CLINE_GetCutPointData, (CLINE_handle cupoint));
DEC_FUN(Bool32, CLINE_SetCutPointData, (CLINE_handle cupoint,CPDCutPoint cpdata));

DEC_FUN(int, CLINE_GetCompCount, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetFirstComp, (CLINE_handle line));
DEC_FUN(CLINE_handle, CLINE_GetNextComp, (CLINE_handle comp));
DEC_FUN(CLINE_handle, CLINE_AddNewComp, (CLINE_handle line));
DEC_FUN(void, CLINE_DelComp, (CLINE_handle line,CLINE_handle comp));
DEC_FUN(void, CLINE_DelAllComps, (CLINE_handle line));
DEC_FUN(CPDComponent, CLINE_GetCompData, (CLINE_handle comp));
DEC_FUN(Bool32, CLINE_SetCompData, (CLINE_handle comp,CPDComponent cpdata));

DEC_FUN(int, CLINE_GetEventInvCount, (CLINE_handle event));
DEC_FUN(CLINE_handle, CLINE_GetFirstEventInv, (CLINE_handle event));
DEC_FUN(CLINE_handle, CLINE_GetNextEventInv, (CLINE_handle inv));
DEC_FUN(CLINE_handle, CLINE_AddNewEventInv, (CLINE_handle event));
DEC_FUN(void, CLINE_DelEventInv, (CLINE_handle event,CLINE_handle inv));
DEC_FUN(void, CLINE_DelAllEventInvs, (CLINE_handle event));
DEC_FUN(CPDInterval, CLINE_GetEventInvData, (CLINE_handle inv));
DEC_FUN(Bool32, CLINE_SetEventInvData, (CLINE_handle inv,CPDInterval cpdata));

DEC_FUN(int, CLINE_GetCutPointInvCount, (CLINE_handle cupoint));
DEC_FUN(CLINE_handle, CLINE_GetFirstCutPointInv, (CLINE_handle cupoint));
DEC_FUN(CLINE_handle, CLINE_GetNextCutPointInv, (CLINE_handle inv));
DEC_FUN(CLINE_handle, CLINE_AddNewCutPointInv, (CLINE_handle cupoint));
DEC_FUN(void, CLINE_DelCutPointInv, (CLINE_handle cupoint,CLINE_handle inv));
DEC_FUN(void, CLINE_DelAllCutPointInvs, (CLINE_handle cupoint));
DEC_FUN(CPDInterval, CLINE_GetCutPointInvData, (CLINE_handle inv));
DEC_FUN(Bool32, CLINE_SetCutPointInvData, (CLINE_handle inv,CPDInterval cpdata));
DEC_FUN(Bool32, CLINE_MoveLine, (CLINE_handle container_to, CLINE_handle container_from, CLINE_handle line));
DEC_FUN(Bool32, CLINE_CopyData, (void* to,const void* from,int size));

#undef DEC_FUN




#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
