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

#include "Tuner.H"
/*
  BYTE    tl,     // ������� �����
          tr,     // ������� ������
          bl,     // ������  �����
          br;     // ������  ������
*/
#ifndef INTERSEPTOR
static BYTE def_corn[]=
{
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /* ! */
0000, 0000, 0000, 0000, /* " */
0000, 0000, 0000, 0000, /* # */
0000, 0000, 0000, 0000, /* $ */
0000, 0000, 0000, 0000, /* % */
0000, 0000, 0000, 0000, /* & */
0000, 0000, 0000, 0000, /* ' */
0000, 0000, 0000, 0000, /* ( */
0000, 0000, 0000, 0000, /* ) */
0000, 0000, 0000, 0000, /* * */
0000, 0000, 0000, 0000, /* + */
0000, 0000, 0000, 0000, /* , */
0000, 0000, 0000, 0000, /* - */
0000, 0000, 0000, 0000, /* . */
0000, 0000, 0000, 0000, /* / */
0000, 0000, 0000, 0000, /* 0 */
0000, 0000, 0000, 0000, /* 1 */
0000, A_C , 0000, 0000, /* 2 */
0000, A_C , 0000, A_C , /* 3 modify for UFA. old angles :0,0,0,0 */
0000, 0000, 0000, 0000, /* 4 */
0000 , N_C , 0000, A_C, /* 5 modify for UFA. old angles :0,0,0,A_C */
0000, 0000, 0000, 0000, /* 6 */
0000, N_C , 0000, 0000, /* 7 */
A_C , A_C , A_C , A_C , /* 8 */
A_C , 0000, 0000, A_C , /* 9 */
0000, 0000, 0000, 0000, /* : */
0000, 0000, 0000, 0000, /* ; */
0000, 0000, 0000, 0000, /* < */
0000, 0000, 0000, 0000, /* = */
0000, 0000, 0000, 0000, /* > */
0000, A_C , 0000, 0000, /* ? */
0000, 0000, 0000, 0000, /* @ */
0000, 0000, 0000, 0000, /* A */
0000, 0000, 0000, 0000, /* B */
0000, 0000, 0000, 0000, /* C */
0000, 0000, 0000, 0000, /* D */
0000, 0000, 0000, 0000, /* E */
0000, 0000, 0000, 0000, /* F */
0000, 0000, 0000, 0000, /* G */
0000, 0000, 0000, 0000, /* H */
0000, 0000, 0000, 0000, /* I */
0000, 0000, 0000, 0000, /* J */
0000, 0000, 0000, 0000, /* K */
0000, 0000, 0000, 0000, /* L */
0000, 0000, 0000, 0000, /* M */
0000, 0000, 0000, 0000, /* N */
0000, 0000, 0000, 0000, /* O */
0000, 0000, 0000, 0000, /* P */
0000, 0000, 0000, 0000, /* Q */
0000, 0000, 0000, 0000, /* R */
0000, 0000, 0000, 0000, /* S */
0000, 0000, 0000, 0000, /* T */
0000, 0000, 0000, 0000, /* U */
0000, 0000, 0000, 0000, /* V */
0000, 0000, 0000, 0000, /* W */
0000, 0000, 0000, 0000, /* X */
0000, 0000, 0000, 0000, /* Y */
0000, 0000, 0000, 0000, /* Z */
0000, 0000, 0000, 0000, /* [ */
0000, 0000, 0000, 0000, /* \ */
0000, 0000, 0000, 0000, /* ] */
0000, 0000, 0000, 0000, /* ^ */
0000, 0000, 0000, 0000, /* _ */
0000, 0000, 0000, 0000, /* ` */
0000, 0000, 0000, 0000, /* a */
0000, 0000, 0000, 0000, /* b */
0000, 0000, 0000, 0000, /* c */
0000, 0000, 0000, 0000, /* d */
0000, 0000, 0000, 0000, /* e */
0000, 0000, 0000, 0000, /* f */
0000, 0000, 0000, 0000, /* g */
0000, 0000, 0000, 0000, /* h */
0000, 0000, 0000, 0000, /* i */
0000, 0000, 0000, 0000, /* j */
0000, 0000, 0000, 0000, /* k */
0000, 0000, 0000, 0000, /* l */
0000, 0000, 0000, 0000, /* m */
0000, 0000, 0000, 0000, /* n */
0000, 0000, 0000, 0000, /* o */
0000, 0000, 0000, 0000, /* p */
0000, 0000, 0000, 0000, /* q */
0000, 0000, 0000, 0000, /* r */
0000, 0000, 0000, 0000, /* s */
0000, 0000, 0000, 0000, /* t */
0000, 0000, 0000, 0000, /* u */
0000, 0000, 0000, 0000, /* v */
0000, 0000, 0000, 0000, /* w */
0000, 0000, 0000, 0000, /* x */
0000, 0000, 0000, 0000, /* y */
0000, 0000, 0000, 0000, /* z */
0000, 0000, 0000, 0000, /* { */
0000, 0000, 0000, 0000, /* | */
0000, 0000, 0000, 0000, /* } */
0000, 0000, 0000, 0000, /* ~ */
0000, 0000, 0000, 0000, /*  */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , A_C , /* � */
N_C , A_C , N_C , A_C , /* � modify for UFA. old angles : 0,A_C,0,A_C */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , 0000, N_C , 0000, /* � modify for UFA. old angles : 0,0,0,0 */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , N_C , /* � */
A_C , A_C , A_C , A_C , /* � */
N_C , N_C , N_C , N_C , /* � */
0000, A_C , 0000, 0000, /* � */
A_C , 0000, A_C , A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, N_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
A_C , 0000, 0000, 0000, /* � */
0000, A_C , A_C , 0000, /* � */
0000, 0000, A_C , A_C , /* � */
0000, A_C , 0000, A_C , /* � */
N_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , A_C , A_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
N_C , N_C , N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , 0000, /* � */ /* �뫮 N_C */
A_C , A_C , A_C , A_C , /* � */
N_C , N_C , N_C , N_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, 0000, /* � */
A_C , 0000, A_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , N_C , N_C , /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , N_C , A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, A_C , A_C , /* � */
A_C , 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000  /*  */
};
#else
static BYTE def_corn[]= // INTERSEPTOR
{
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /* ! */
0000, 0000, 0000, 0000, /* " */
0000, 0000, 0000, 0000, /* # */
0000, 0000 , 0000, A_C,  /* $ INTERSEPTOR */
0000, 0000, 0000, 0000, /* % */
0000, 0000, 0000, 0000, /* & */
0000, 0000, 0000, 0000, /* ' */
0000, 0000, 0000, 0000, /* ( */
0000, 0000, 0000, 0000, /* ) */
0000, 0000, 0000, 0000, /* * */
0000, 0000, 0000, 0000, /* + */
0000, 0000, 0000, 0000, /* , */
0000, 0000, 0000, 0000, /* - */
0000, 0000, 0000, 0000, /* . */
0000, 0000, 0000, 0000, /* / */
0000, 0000, 0000, 0000, /* 0 */
0000, 0000, 0000, 0000, /* 1 */
0000, A_C , 0000, 0000, /* 2 */
0000, A_C , 0000, A_C , /* 3 modify for UFA. old angles :0,0,0,0 */
0000, 0000, 0000, 0000, /* 4 */
0000, 0000, 0000, A_C, /* 5 modify for UFA. old angles :0,0,0,A_C */
0000, 0000, 0000, 0000, /* 6 */
0000, N_C , 0000, 0000, /* 7 */
A_C , A_C , A_C , A_C , /* 8 */
A_C , 0000, 0000, A_C , /* 9 */
0000, 0000, 0000, 0000, /* : */
0000, 0000, 0000, 0000, /* ; */
0000, 0000, 0000, 0000, /* < */
0000, 0000, 0000, 0000, /* = */
0000, 0000, 0000, 0000, /* > */
0000, A_C , 0000, 0000, /* ? */
0000, 0000, 0000, 0000, /* @ */
0000, 0000, 0000, 0000, /* A */
0000, 0000, 0000, 0000, /* B */
0000, 0000, 0000, 0000, /* C */
0000, 0000, 0000, 0000, /* D */
0000, 0000, 0000, 0000, /* E */
0000, 0000, 0000, 0000, /* F */
0000, 0000, 0000, 0000, /* G */
0000, 0000, 0000, 0000, /* H */
0000, 0000, 0000, 0000, /* I */
0000, 0000, 0000, 0000, /* J */
0000, 0000, 0000, 0000, /* K */
0000, 0000, 0000, 0000, /* L */
0000, 0000, 0000, 0000, /* M */
0000, 0000, 0000, 0000, /* N */
0000, 0000, 0000, 0000, /* O */
0000, 0000, 0000, 0000, /* P */
0000, 0000, 0000, 0000, /* Q */
0000, 0000, 0000, 0000, /* R */
0000, 0000, 0000, 0000, /* S */
0000, 0000, 0000, 0000, /* T */
0000, 0000, 0000, 0000, /* U */
0000, 0000, 0000, 0000, /* V */
0000, 0000, 0000, 0000, /* W */
0000, 0000, 0000, 0000, /* X */
0000, 0000, 0000, 0000, /* Y */
0000, 0000, 0000, 0000, /* Z */
0000, 0000, 0000, 0000, /* [ */
0000, 0000, 0000, 0000, /* \ */
0000, 0000, 0000, 0000, /* ] */
0000, 0000, 0000, 0000, /* ^ */
0000, 0000, 0000, 0000, /* _ */
0000, 0000, 0000, 0000, /* ` */
0000, 0000, 0000, 0000, /* a */
0000, 0000, 0000, 0000, /* b */
0000, 0000, 0000, 0000, /* c */
0000, 0000, 0000, 0000, /* d */
0000, 0000, 0000, 0000, /* e */
0000, 0000, 0000, 0000, /* f */
0000, 0000, 0000, 0000, /* g */
0000, 0000, 0000, 0000, /* h */
0000, 0000, 0000, 0000, /* i */
0000, 0000, 0000, 0000, /* j */
0000, 0000, 0000, 0000, /* k */
0000, 0000, 0000, 0000, /* l */
0000, 0000, 0000, 0000, /* m */
0000, 0000, 0000, 0000, /* n */
0000, 0000, 0000, 0000, /* o */
0000, 0000, 0000, 0000, /* p */
0000, 0000, 0000, 0000, /* q */
0000, 0000, 0000, 0000, /* r */
0000, 0000, 0000, 0000, /* s */
0000, 0000, 0000, 0000, /* t */
0000, 0000, 0000, 0000, /* u */
0000, 0000, 0000, 0000, /* v */
0000, 0000, 0000, 0000, /* w */
0000, 0000, 0000, 0000, /* x */
0000, 0000, 0000, 0000, /* y */
0000, 0000, 0000, 0000, /* z */
0000, 0000, 0000, 0000, /* { */
0000, 0000, 0000, 0000, /* | */
0000, 0000, 0000, 0000, /* } */
0000, 0000, 0000, 0000, /* ~ */
0000, 0000, 0000, 0000, /*  */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , A_C , /* � */
N_C , A_C , N_C , A_C , /* � modify for UFA. old angles : 0,A_C,0,A_C */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , 0000, N_C , 0000, /* � modify for UFA. old angles : 0,0,0,0 */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , N_C , /* � */
A_C , A_C , A_C , A_C , /* � */
N_C , N_C , N_C , N_C , /* � */
0000, A_C , 0000, 0000, /* � */
A_C , 0000, A_C , A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, N_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
A_C , 0000, 0000, 0000, /* � */
0000, A_C , A_C , 0000, /* � */
0000, 0000, A_C , A_C , /* � */
0000, A_C , 0000, A_C , /* � */
N_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , A_C , A_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
N_C , N_C , N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , 0000, /* � */ /* �뫮 N_C */
A_C , A_C , A_C , A_C , /* � */
//N_C , N_C , N_C , N_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, 0000, /* � */
A_C , 0000, A_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , N_C , N_C , /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
A_C , 0000, 0000, 0000, /* � */
0000, 0000, A_C , A_C , /* � */
A_C , 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000  /*  */
};
#endif


static BYTE def_corn_cs[]=
{
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /*   */
0000, 0000, 0000, 0000, /* ! */
0000, 0000, 0000, 0000, /* " */
0000, 0000, 0000, 0000, /* # */
0000, 0000, 0000, 0000, /* $ */
0000, 0000, 0000, 0000, /* % */
0000, 0000, 0000, 0000, /* & */
0000, 0000, 0000, 0000, /* ' */
0000, 0000, 0000, 0000, /* ( */
0000, 0000, 0000, 0000, /* ) */
0000, 0000, 0000, 0000, /* * */
0000, 0000, 0000, 0000, /* + */
0000, 0000, 0000, 0000, /* , */
0000, 0000, 0000, 0000, /* - */
0000, 0000, 0000, 0000, /* . */
0000, 0000, 0000, 0000, /* / */
0000, 0000, 0000, 0000, /* 0 */
0000, 0000, 0000, 0000, /* 1 */
0000, A_C , 0000, 0000, /* 2 */
0000, 0000, 0000, 0000, /* 3 */
0000, 0000, 0000, 0000, /* 4 */
0000, 0000, 0000, A_C , /* 5 */
0000, 0000, 0000, 0000, /* 6 */
0000, N_C , 0000, 0000, /* 7 */
A_C , A_C , A_C , A_C , /* 8 */
A_C , 0000, 0000, A_C , /* 9 */
0000, 0000, 0000, 0000, /* : */
0000, 0000, 0000, 0000, /* ; */
0000, 0000, 0000, 0000, /* < */
0000, 0000, 0000, 0000, /* = */
0000, 0000, 0000, 0000, /* > */
0000, 0000, 0000, 0000, /* ? */
0000, 0000, 0000, 0000, /* @ */
0000, 0000, 0000, 0000, /* A */
0000, 0000, 0000, 0000, /* B */
0000, 0000, 0000, 0000, /* C */
0000, 0000, 0000, 0000, /* D */
0000, 0000, 0000, 0000, /* E */
0000, 0000, 0000, 0000, /* F */
0000, 0000, 0000, 0000, /* G */
0000, 0000, 0000, 0000, /* H */
0000, 0000, 0000, 0000, /* I */
0000, 0000, 0000, 0000, /* J */
0000, 0000, 0000, 0000, /* K */
0000, 0000, 0000, 0000, /* L */
0000, 0000, 0000, 0000, /* M */
0000, 0000, 0000, 0000, /* N */
0000, 0000, 0000, 0000, /* O */
0000, 0000, 0000, 0000, /* P */
0000, 0000, 0000, 0000, /* Q */
0000, 0000, 0000, 0000, /* R */
0000, 0000, 0000, 0000, /* S */
0000, 0000, 0000, 0000, /* T */
0000, 0000, 0000, 0000, /* U */
0000, 0000, 0000, 0000, /* V */
0000, 0000, 0000, 0000, /* W */
0000, 0000, 0000, 0000, /* X */
0000, 0000, 0000, 0000, /* Y */
0000, 0000, 0000, 0000, /* Z */
0000, 0000, 0000, 0000, /* [ */
0000, 0000, 0000, 0000, /* \ */
0000, 0000, 0000, 0000, /* ] */
0000, 0000, 0000, 0000, /* ^ */
0000, 0000, 0000, 0000, /* _ */
0000, 0000, 0000, 0000, /* ` */
0000, 0000, 0000, 0000, /* a */
0000, 0000, 0000, 0000, /* b */
0000, 0000, 0000, 0000, /* c */
0000, 0000, 0000, 0000, /* d */
0000, 0000, 0000, 0000, /* e */
0000, 0000, 0000, 0000, /* f */
0000, 0000, 0000, 0000, /* g */
0000, 0000, 0000, 0000, /* h */
0000, 0000, 0000, 0000, /* i */
0000, 0000, 0000, 0000, /* j */
0000, 0000, 0000, 0000, /* k */
0000, 0000, 0000, 0000, /* l */
0000, 0000, 0000, 0000, /* m */
0000, 0000, 0000, 0000, /* n */
0000, 0000, 0000, 0000, /* o */
0000, 0000, 0000, 0000, /* p */
0000, 0000, 0000, 0000, /* q */
0000, 0000, 0000, 0000, /* r */
0000, 0000, 0000, 0000, /* s */
0000, 0000, 0000, 0000, /* t */
0000, 0000, 0000, 0000, /* u */
0000, 0000, 0000, 0000, /* v */
0000, 0000, 0000, 0000, /* w */
0000, 0000, 0000, 0000, /* x */
0000, 0000, 0000, 0000, /* y */
0000, 0000, 0000, 0000, /* z */
0000, 0000, 0000, 0000, /* { */
0000, 0000, 0000, 0000, /* | */
0000, 0000, 0000, 0000, /* } */
0000, 0000, 0000, 0000, /* ~ */
0000, 0000, 0000, 0000, /*  */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , N_C , N_C , /* � */
A_C , A_C , A_C , A_C , /* � */
N_C , N_C , N_C , N_C , /* � */
0000, A_C , 0000, 0000, /* � */
A_C , 0000, A_C , A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
N_C , N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
A_C , 0000, 0000, 0000, /* � */
0000, A_C , A_C , 0000, /* � */
0000, 0000, A_C , A_C , /* � */
0000, A_C , A_C , 0000, /* � */
A_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , A_C , A_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
0000, N_C , N_C , 0000, /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , N_C , 0000, /* � */
A_C , A_C , A_C , A_C , /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, A_C , N_C , 0000, /* � */
A_C , 0000, A_C , A_C , /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, N_C , 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, A_C , /* � */
0000, A_C , 0000, A_C , /* � */
0000, A_C , N_C , A_C , /* � */
A_C , 0000, 0000, 0000, /* � */
0000, A_C , 0000, A_C , /* � */
A_C , 0000, 0000, A_C , /* � top_ri=A_C ERECT */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, N_C , 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
A_C , 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000, /* � */
0000, 0000, 0000, 0000  /*  */
};
