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

/****************************************************************************
 *                                                                          *
 *                                                                          *
 *                   T I G E R   O C R   L I B R A R Y                      *
 *                                                                          *
 *                                                                          *
 *           File       :  _TGCALL.H                                        *
 *           Contains   :  Internal definitions for TGCALL.DLL and          *
 *                         TGCALL.LIB connection                            *
 *           Written by :  Yuri Panchul                                     *
 *           Compiler   :  Microsoft C v6.0                                 *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/

# ifndef _TGCALL_H_INCLUDED
# define _TGCALL_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////
#include "dlltypes.h"

# define TCB_IMAGE_OPEN               1
# define TCB_IMAGE_READ               2
# define TCB_IMAGE_CLOSE              3
# define TCB_REPORT_ERROR             4
# define TCB_PROGRESS_START           5
# define TCB_PROGRESS_FINISH          6
# define TCB_PROGRESS_SET_PERCENTAGE  7
# define TCB_PROGRESS_SET_STAGE       8
# define TCB_IMAGE_OPEN_XY            9
# define TCB_PROCESS_PICTURES	      10
# define TCB_TIGER_GET_WORD           11
//////////////////////////////////////////////////////////////////////////////
#if defined( FILE_TIGERAPI)
	#define TIGERLOC(typ) TIGERAPI(typ)
#else
	#define TIGERLOC(typ) TIGERFUN(typ)
#endif
//////////////////////////////////////////////////////////////////////////////
#ifndef __NT__
TIGERLOC(BOOL16)          __Tiger_Load          (DWORD dwTigerCallBack,
												 LPSTR lpTigerPathName);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          __BCR_Load            (DWORD dwTigerCallBack,
												 LPSTR lpTigerPathName);
#else
//////////////////////////////////////////////////////////////////////////////
#pragma warning ( disable : 4229 )
//////////////////////////////////////////////////////////////////////////////
typedef DWORD (TIGERFUN(*)fpTiger_Callback) (WORD wFuncNumber, LPDWORD lpParams);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          __Tiger_Load          (fpTiger_Callback fpCallback,
												 LPSTR lpPath);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          __BCR_Load            (fpTiger_Callback fpCallback,
												 LPSTR lpTigerPathName);
#endif
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          __Tiger_Unload        (void);
//////////////////////////////////////////////////////////////////////////////
#ifndef __NT__
TIGERLOC(DWORD)           Tiger_CallBack        (WORD wFuncNumber,
												 LPDWORD lpParams);
//////////////////////////////////////////////////////////////////////////////
#else
CBDWORD                   Tiger_CallBack        (WORD wFuncNumber,
												 LPDWORD lpParams);
#endif
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(DWORD)           __Tiger_CallBack      (WORD wFuncNumber);
//////////////////////////////////////////////////////////////////////////////
# define GMEM_FIX              (GMEM_FIXED | GMEM_NOCOMPACT | GMEM_NODISCARD)
# define COMMON_BUFFER_SIZE    (32 * 1024L)
# define CALLBACK_PARAMS_SIZE  (64 * sizeof (DWORD))
//////////////////////////////////////////////////////////////////////////////
#define FUNC_SHEET_UPDATE            1
#define FUNC_SHEET_DELETE            2
#define FUNC_SET_TEMP_DIR            3
#define FUNC_SET_TABLE               4
#define FUNC_SET_USER_DICT           5
#define FUNC_SET_WINDOWS_DIR         6
#define FUNC_HANDSHAKING_VERS        7
#define FUNC_HANDSHAKING_PASS        8
#define FUNC_SET_MMX                 9
#define FUNC_SET_EDEXT	            10
#define FUNC_SET_MASHINE_NAME       11
#define FUNC_PROTECT_INFO           12
#define FUNC_SET_RTF_COORDINATES    13
#define FUNC_SET_IMAGENAME          14
#define FUNC_SET_PAGENUMBER         15
#define FUNC_DELETE_TEMP_FILES      16
// duplicate in file ..\USER\TGCALL.H
//////////////////////////////////////////////////////////////////////////////
#undef TIGERLOC
# endif
