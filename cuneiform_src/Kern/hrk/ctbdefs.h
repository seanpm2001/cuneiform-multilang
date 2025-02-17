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

#ifndef __CTBDEFS_H
#define __CTBDEFS_H


#include "recdefs.h"

#define CTB_VERSION 7

#ifndef	__RECDEFS_H
typedef unsigned char       Word8;
typedef unsigned short int  Word16;
typedef short int           Int16;
typedef int                 Int32;
typedef int                 Bool32;
#endif

#include <stdio.h>
/***********************   header structure ****************************/
#define CTB_DATA_SIZE    256

#define CTB_PRINT_NONE      0x00
#define CTB_PRINT_FONT      0x01
#define CTB_PRINT_ITALIC    0x02
#define CTB_PRINT_BOLD      0x04
#define CTB_PRINT_SERIFIC   0x08
#define CTB_PRINT_GELV      0x10
#define CTB_PRINT_NARROW    0x20
#define CTB_PRINT_STRIGHT   0x40
#define CTB_PRINT_LIGHT     0x80
typedef struct  {
	Word8   charset;        // 0) font charset
	Word8   width;          // 1) 0-255
	Word8   height;         // 2) 0-127
	Word8   let;            // 3) letters or ligature
    Word8   typedface;      // 4) typeface. 0 for standart letter
    Word8   IsPrint;        // 5) print type of font (and others feats)
	Word8   reserved[CTB_DATA_SIZE-6];
	} CTB_data_standart;

typedef struct  {
	Word8   charset;        // 0) not free for version <=5
	Word8   width;          // 1) 0-255
	Word8   height;         // 2) 0-127
	Word8   let;            // 3) letters or ligature      Alt[0].Code
    Word8   nLns;           // 4) num of lines
    Word8   IsPrint;        // 5) print type of font (and others feats)
    Word8   rect16[8];      // 6-13) rectangle16 - box on the image
    Word8   Prob;           // 14) Alt[0].Prob
    Word8   Valid;          // 15) main validate code
    Word8   lnAltCnt;       // 16) number of alternates
    Word8   Alt0Code;       // 17) Alt[0].Code
    Word8   Alt0Prob;       // 18) Alt[0].Prob
    Word8   Alt1Code;       // 19) Alt[1].Code
    Word8   Alt1Prob;       // 20) Alt[1].Prob
    Word8   Alt2Code;       // 21) Alt[2].Code
    Word8   Alt2Prob;       // 22) Alt[2].Prob
    Word8   Alt3Code;       // 23) Alt[3].Code
    Word8   Alt3Prob;       // 24) Alt[3].Prob
    Word8   field_number;   // 25) number of recognized field
    Word8   alphabet;       // 26) number of alpha in local table
    Word8   CaseType;       // 27) control for case change
    Word8   Alt0Method;     // 28) Alt[0].Method
    Word8   Alt1Method;     // 29) Alt[1].Method
    Word8   Alt2Method;     // 30) Alt[2].Method
    Word8   Alt3Method;     // 31) Alt[3].Method
	} CTB_data_container5;


typedef struct  {
	Word8   charset;        // 0) not free for version <=5
	Word8   width;          // 1) 0-255
	Word8   height;         // 2) 0-127
	Word8   let;            // 3) letters or ligature      Alt[0].Code
    Word8   typedface;      // 4) typeface. 0 for standart letter
    Word8   IsPrint;        // 5) print type of font (and others feats)
    Word8   rect16[8];      // 6-13) rectangle16 - box on the image
    Word8   Prob;           // 14) Alt[0].Prob
    Word8   Valid;          // 15) main validate code
    Word8   lnAltCnt;       // 16) number of alternates
    Word8   Alt0Code;       // 17) Alt[0].Code
    Word8   Alt0Prob;       // 18) Alt[0].Prob
    Word8   Alt1Code;       // 19) Alt[1].Code
    Word8   Alt1Prob;       // 20) Alt[1].Prob
    Word8   Alt2Code;       // 21) Alt[2].Code
    Word8   Alt2Prob;       // 22) Alt[2].Prob
    Word8   Alt3Code;       // 23) Alt[3].Code
    Word8   Alt3Prob;       // 24) Alt[3].Prob
    Word8   field_number;   // 25) number of recognized field
    Word8   alphabet;       // 26) number of alpha in local table
    Word8   CaseType;       // 27) control for case change
    Word8   Alt0Method;     // 28) Alt[0].Method
    Word8   Alt1Method;     // 29) Alt[1].Method
    Word8   Alt2Method;     // 30) Alt[2].Method
    Word8   Alt3Method;     // 31) Alt[3].Method
    Word8   nLns;           // 32) num of lines
    Word8   res;            // 33) num of lines
	} CTB_data_container7;

typedef struct  {
	Word8   charset;        // 0)    not free for version <=5
	Word8   width;          // 1)    0-255
	Word8   height;         // 2)    0-127
	Word8   let;            // 3)    letters or ligature      Alt[0].Code
    Word8   typedface;      // 4)    typeface. 0 for standart letter
    Word8   IsPrint;        // 5)    print type of font (and others feats)
    Int16   b0;             // 6,7
    Int16   b1;             // 8,9   firts base lines
    Int16   b2;             // 10,11 second base lines
    Int16   b3;             // 12,13 third base lines
    Int16   b4;             // 14,15 fourth base lines
    Int16   bm;             // 16,17 middle line
    Int16   ps;             // 18,19 point size
    Int16   n1;             // 20,21
    Int16   n2;             // 22,23
    Int16   n3;             // 24,25
    Int16   n4;             // 26,27
    } CTB_data_container7_puma; // saved from PUMA-snap (key Alt_F8)

typedef struct  {
	Word8	attr_size;      // Number of attributes
	Word8	label[CTB_DATA_SIZE-1];
	} CTB_global_data;

typedef struct 	{
    Word8 sign[2];          // can be "CT"
    Int16 version;          // version number
    Int16 size_x, size_y;   // frame sizes
    Word8 dot_per_byte;
    Int32 volume;           // number of images
    Word8 signums;          // special bits
    Word8 need_compress;    // need_comp-bit 1, sort -2
    Word8 attr_size;        // num of attributes
    Word8 data[CTB_DATA_SIZE]; // global attributes
    } H_CTB_file;



// CTB-file types : WIDTH_HEIGHT_COLORS
#define CTB_UNKNOWN     0
#define CTB_128_96_2    1
#define CTB_128_64_2    2
#define CTB_128_64_4    3
#define CTB_128_96_16   4
#define CTB_128_64_16   5
#define CTB_256_96_2    6
#define CTB_256_96_16   7
#define CTB_384_64_2    8
#define CTB_384_32_2    9
#define CTB_128_96_4    10
#define CTB_256_128_4   11
#define CTB_256_96_4    12
#define CTB_256_128_2   13
#define CTB_256_128_16  14
#define CTB_128_128_16  15
#define CTB_256_128_256 16

#define CTB_NON_SIGNUMS 0x00
#define CTB_GRAY_SCALE  0x01
#define CTB_PLANE       0x02
#define CTB_FONT        0x04
#define CTB_GRAY_SCALE2 0x08
#define CTB_INVERS      0x10
#define CTB_MEANDR      0x20
#define CTB_COLOR       0x80

#define CTB_GRAY        (CTB_GRAY_SCALE|CTB_PLANE)

#define CTB_COMP_NONE	0
#define CTB_COMP_PCX	1

#define BASE_MAX_W 255
#define BASE_MAX_H 127

//****************** FILES ***********************

//******** stream Input/Output *******************
#define FCREAT(S,T)     fopen(S,"wb")
#define RET_FREAD(a,b)  (b)
#define RET_FWRITE(a,b) (b)
#define BAD_FOPEN       NULL
#define R_B             "rb"
#define	W_B             "wb"
#define R_B_PLUS        "rb+"

typedef FILE*          FFILE;

#define UNLINK(S)       chmod(S,S_IWRITE);unlink(S)
#define RENAME(a,b)     rename(a,b)
#define STAT(a,b)       stat(a,b)

//****************** MEMORY **********************

#ifndef MEMSET
#define MEMCPY(a,b,c)   memcpy(a,b,c)
#define MEMSET(a,b,c)   memset(a,b,c)
#endif

//***************** STRINGS ********************
#define SPRINTF         sprintf
#define STRCHR          strchr
#define STRRCHR         strrchr
#define STRCPY(a,b)          strcpy((a), (b))
#define STRCAT(a,b)          strcat((a), (b))
#define STRLEN          strlen

typedef struct 	{
	FFILE  bas,ndx;       // file desriptions
    Int32  num;           // number of images
    Int32  len;           // length of binary frame
    Int16  type;          // type : sizes & resolution
    Int16  width,height,colors,version,dpb;
    Word8  signums;       // global attributes
    Word8  need_compress; // need call compress()
    Word8  attr;          // 'r' or 'w'
    Word8  is_sort;       // base is sorted
    Word8  attr_size;     // number of attributes
    } CTB_handle;
#define CTB_ANSI_CHARSET            0
#define CTB_DEFAULT_CHARSET         1
#define CTB_SYMBOL_CHARSET          2
#define CTB_SHIFTJIS_CHARSET        128
#define CTB_HANGEUL_CHARSET         129
#define CTB_HANGUL_CHARSET          129
#define CTB_GB2312_CHARSET          134
#define CTB_CHINESEBIG5_CHARSET     136
#define CTB_OEM_CHARSET             255
#define CTB_JOHAB_CHARSET           130
#define CTB_HEBREW_CHARSET          177
#define CTB_ARABIC_CHARSET          178
#define CTB_GREEK_CHARSET           161
#define CTB_TURKISH_CHARSET         162
#define CTB_VIETNAMESE_CHARSET      163
#define CTB_THAI_CHARSET            222
#define CTB_EASTEUROPE_CHARSET      238
#define CTB_RUSSIAN_CHARSET         204

#define CTB_MAC_CHARSET             77
#define CTB_BALTIC_CHARSET          186

#define CTB_UZBEK_CHARSET           254
#define CTB_KAZAH_CHARSET           253

#endif
