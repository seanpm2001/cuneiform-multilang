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

/*
  ������������������������������������������������������������������
  ��                                                              ��
  ��     Copyright (C) 1990 Cognitive Technology Corporation.     ��
  ��	 All rights reserved. This program is proprietary and     ��
  ��     a trade secret of Cognitive Technology Corporation.      ��
  ��                                                              ��
  ������������������������������������������������������������������
*/

/****************************************************************
*  This is Extention code of ED format.
*
*   Piter 01.08.96
*****************************************************************/
#ifndef __EDEXT__
#define __EDEXT__

#include "ctp.h"

#define SS_EXTENTION	0x1C

struct edExtention
{
	unsigned char     	code;   /* always SS_EXTENTION <0x1C> */
	unsigned short int      Ecode;  /* New extention code */
	unsigned short int      length; /* Length in bytes */
};
/* This structure is header all structs included in extention ED */
typedef struct edExtention EDEXT;

/* DESCRIPTIONS EXTENTION CODE */

/* 0x0000 - 0x00FF special code */
#define ITS_EDEXT_SPECIAL(a)   (a>=0x0000 && a<0x0100)
/* 0x0100 - 0x01FF table description */
#define ITS_EDEXT_TABLE(a)     (a>=0x0100 && a<0x0200)
/* 0x0200 - 0x02FF picture description */
#define ITS_EDEXT_PICTURE(a)   (a>=0x0200 && a<0x0300)
/* 0x0300 - 0xEFFF Your code please...*/

/* 0xF000 - 0xFFFF temporary code for debugging ( ! Not used in releas version !)*/

#define EDEXT_VERSION		0x0000

#define EDEXT_TABLE_START 	0x0100 // edTable struct
#define EDEXT_TABLE_VER         0x0101 // array horiz. coord of vert lines (x0,x1,...)
#define EDEXT_TABLE_HOR         0x0102 // array vert. coord of horiz. lines (y0,y1,...)
#define EDEXT_TABLE_TAB         0x0103 // array ID of items
#define EDEXT_TABLE_VERLINE     0x0104 // array not delete vert lines (x00,y00,x01,y01,...)

#define EDEXT_CTP	 	0x0200 // filename of CTP file
#define EDEXT_PICTURE           0x0201 // struct of <edPicture>

typedef struct sTable
{
 EDEXT 	  head;
 INT      Sh;   	// count horizontal lines
 INT      Sv;           // count vertical lines
 INT      angle;        // angle of table 1/1024 rad
 INT      VerCount;     // count not delete of vert lines
 INT      ShiftX;       // shift tables in image
 INT      ShiftY;       //
} edTable;

#define EDEXT_SIZE_FILENAME 256

#endif
