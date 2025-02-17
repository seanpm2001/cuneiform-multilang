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

#ifndef __RSTRDEFS_H
#define __RSTRDEFS_H

#pragma pack(1)
/*
#ifndef __EXCSTRUC_H
   #include "excdefs.h"
#endif
*/

//#ifndef __FRHSTRUC_H
//   #include "frhdefs.h"
//#endif

//#include "cttypext.h"

#define RSTR_VERSION_CODE   1
typedef struct tagRSTR_Options
{
char     szAlphaString[256];
//FrhFieldSetup *setup;    // old fields
//                          char     szAlphaString[256]
Word8   language;
Word8 * Alphabet;       // 256 indicators
Word8   StyleRecog;     // printing, handprinting
Word8   EnableCut;      // only glue if 0

Word8   StyleCursive;   // no cursive if 0
Word8   StyleFont;      // normal,TW, DOT MATR
Word8   StyleContext;   // future
Word8	StyleSpelling;

Word8	EnableLearn;    //
Int32   pageSkew2048;
}RSTR_Options;
/*
typedef struct tagSaveString  // str in H\struct.h OLD TIGER
{
 Int16 row;             // upper bound of line box (in real coord)
 Int16 col;             // left  bound of line box (in real coord)
 Int16 lower;           // lower bound of line box (in real coord)
 Int16 right;           // right bound of line box (in real coord)
 Word16 dust;           // c_comp offset to first dust ptr (in bytes)
 Word16 end;            // offset to end end of dust ptrs (in bytes)
 Word16 lth;            // total size of struc 'end == lth'  (in bytes)
 Word16 first;          // unused now
 Word16 last;           // unused now
 Word16 scale;          // scale of the string (unused now)
 Int16 fragment;        // fragment of the string (unused now)
 Word8 language;        // language of the string (unused now)
 Int32 nSkew;		// string's skew (recalculated, not page!)
 Int32 strFlag;
 Word8 reserve[21];
#define Matrix_For_RCM   0x00000001
#define Matrix_For_LEO   0x00000002
 void *vAddStringData;  //Almi added
 void *vAddStatisticData;  //Almi added
 ExtComponent* c_comp[1];       // array of ptrs to components
}  SaveString;
*/

#pragma pack()

#endif
