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

#include "c_types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "leo.h"
#include "lang_def.h"
#include "ligas.h"

extern Word8 language;
extern Word8 langCyrilRoman;

static Bool32 CyrilLanguage(void)
{
    if( langCyrilRoman == 1)
		return TRUE;
    if( langCyrilRoman == 2)
        return FALSE;
	if (
		 language == LANG_RUSSIAN   || language == LANG_ENGLISH ||
         language == LANG_UKRAINIAN || language == LANG_SERBIAN ||
         language == LANG_BULGAR    ||
         language == LANG_RUSENG
		)
	{
         langCyrilRoman = 1;
		 return TRUE;
	}
	return FALSE;
}
/////////////////
static Bool32 RomanLanguage(void)
{
	if( langCyrilRoman == 1)
		return FALSE;
    if( langCyrilRoman == 2)
        return TRUE;

	if	(language == LANG_GERMAN ||
     language == LANG_FRENCH || language == LANG_SWEDISH ||
     language == LANG_ITALIAN|| language == LANG_SPANISH ||
     language == LANG_DANISH || language == LANG_PORTUGUESE ||
     language == LANG_DUTCH  ||
	 language == LANG_POLISH || language == LANG_CROATIAN ||
	 language == LANG_CZECH  || language == LANG_HUNGAR   ||
	 language == LANG_ROMAN	 || language == LANG_SLOVENIAN||
	 is_baltic_language(language)	|| // 09.07.2001 E.P.
	 is_turkish_language(language)
	 )
	{
		langCyrilRoman = 2;
	    return TRUE;
	}
	return FALSE;
}
//////////////
Bool32 LetDigSymbol(int i) // letter or digit
{
	if( i >= '0' && i <= '9' ||
		i >= 'A' && i <= 'Z' ||
		i >= 'a' && i <= 'z'
	  )
		return TRUE;
	if( i == 35)    // Nomer
		return TRUE;

    if( CyrilLanguage() &&
		(i >= 128 && i < 176  ||    // ascii A-�
		 i >= 224 && i < 240      //       �-�
	   )
      )
	  return TRUE;

	if( RomanLanguage() && i >= 0xC0 )
		return TRUE;

	return FALSE;
}
////////////
Bool32 BigSymbol(int i)
{
	if( i >= '0' && i <= '9' ||
		i >= 'A' && i <= 'Z' ||
		i == 'l' )
		return TRUE;

	if( CyrilLanguage() )
	{
      if( i >= 128 && i < 160  ||
		  i == 0xC6    // I ����������
        )
	   return TRUE;
      return FALSE;
	}

	if( RomanLanguage())
	{  if(i >= 0xC0 )
		return TRUE;
	   return FALSE;
	}

	return FALSE;
}
////////////
static const char bigLitLet[]="bdghklpqty";
static const char smallLet[] ="acemnorsuvwxz";
                   // Qj�����
static const char notStandLet[]="QJfj\x84\x89\x96\x99\xe4";
                   // W����
static const char wideLet[]="W\x86\x98\x99\x9e";

static const Word8 notCursivNames[]="\xa0\xa3\xa4\xa8\xe2";  // dos - "�����";
static const Word8 cursivNames[]   ="\xfd\xf8\xf0\xf7\xf5";
////////////////
Bool32 BigSmallSymbol(int i)
{
	// ���
	if( // 16.07.2001 E.P. �������� � a_bottom_accent_baltic 0xe0
		CyrilLanguage() &&
			(i==0xa1 || i==0xe0 || i==0xe3) ||
		strchr(bigLitLet,i)
      )
	  return TRUE;

	return FALSE;
}
////////////
Bool32 SmallSymbol(int i)
{
  if( CyrilLanguage()  )
  {
	if(i >= 160 && i < 176 )
	{
		// ���
	 if( i == 0xa1 || i == 0xa4 || i == 0xa9)
		 return FALSE;
	 return TRUE;
	}

	if(i >= 224 && i < 240 )
	{
		// �����
	 if( i == 0xe0 || i == 0xe3 || i == 0xe4 || i == 0xe6 || i == 0xe9)
		 return FALSE;
	 return TRUE;
	}
  }

  if( strchr(smallLet,i) )
		return TRUE;
  return FALSE;
}
/////////////
Bool32 is_czech_special(BYTE let)
{
return (
     let == AA_right_accent || let == a_right_accent	||
     let == CC_inv_roof		|| let == c_inv_roof		||
     let == DD_inv_roof		|| let == d_inv_roof		||
     let == EE_right_accent || let == e_right_accent	||
     let == EE_inv_roof		|| let == e_inv_roof		||
     let == II_right_accent || let == i_right_accent	||
     let == NN_inv_roof		|| let == n_inv_roof		||
     let == OO_right_accent || let == o_right_accent	||
     let == RR_inv_roof		|| let == r_inv_roof		||
     let == SS_inv_roof_latin	|| let == s_inv_roof_latin	||
     let == TT_inv_roof		|| let == t_inv_roof		||
     let == UU_right_accent || let == u_right_accent	||
     let == UU_circle_accent|| let == u_circle_accent	||
     let == YY_right_accent || let == y_right_accent	||
     let == ZZ_inv_roof_latin		|| let == z_inv_roof_latin
    );
}
///////////////
Bool32 is_roman_special(BYTE let)
{
return (
     let == AA_semicircle    || 	let == a_semicircle		||
     let == AA_roof_accent   || 	let == a_roof_accent	||
     let == II_roof_accent	 || 	let == i_roof_accent	||
     let == SS_bottom_accent_latin || 	let == s_bottom_accent_latin	||
     let == TT_bottom_accent || 	let == t_bottom_accent
    );
}
////////////////////
Bool32 is_hungar_special(BYTE let)
{
return (
     let == AA_right_accent || 		let == a_right_accent ||
     let == EE_right_accent || 		let == e_right_accent ||
     let == II_right_accent || 		let == i_right_accent ||
     let == OO_right_accent || 		let == o_right_accent ||
     let == OO_2dot_accent  || 		let == o_2dot_accent  ||
     let == OO_double_right || 		let == o_double_right
    );
}
/////////////////
Bool32 is_latvian_special(BYTE let)
{
return (
     let == AA_macron			||let == a_macron			||
     let == CC_inv_roof			||let == c_inv_roof			||
     let == EE_macron			||let == e_macron			||
     let == GG_bottom_accent	||let == LATVIAN_g			||
     let == II_macron			||let == i_macron			||
     let == KK_bottom_accent	||let == k_bottom_accent	||
     let == LL_bottom_accent	||let == l_bottom_accent	||
     let == NN_bottom_accent	||let == n_bottom_accent	||
     let == SS_inv_roof_baltic	||let == s_inv_roof_baltic	||
     let == UU_macron			||let == u_macron			||
     let == ZZ_inv_roof_baltic	||let == z_inv_roof_baltic	||
	 0
    );
}
////////////////
Bool32 is_lithuanian_special(BYTE let)
{
return (
     let == AA_bottom_accent	|| 	let == a_bottom_accent	||
     let == CC_inv_roof			|| 	let == c_inv_roof		||
     let == EE_bottom_accent	|| 	let == e_bottom_accent	||
     let == EE_dot_accent		|| 	let == e_dot_accent		||
     let == II_bottom_accent	|| 	let == i_bottom_accent	||
     let == SS_inv_roof_baltic	|| 	let == s_inv_roof_baltic||
     let == UU_bottom_accent	|| 	let == u_bottom_accent	||
     let == UU_macron			|| 	let == u_macron			||
     let == ZZ_inv_roof_baltic	|| 	let == z_inv_roof_baltic||
	 0
    );
}
//////////////
Bool32 is_estonian_special(BYTE let)
{
return (
     let == AA_2dot_accent		|| 	let == a_2dot_accent		||
     let == OO_tild_accent		|| 	let == o_tild_accent		||
     let == OO_2dot_accent		|| 	let == o_2dot_accent		||
     let == SS_inv_roof_baltic	|| 	let == s_inv_roof_baltic	||
     let == UU_2dot_accent		|| 	let == u_2dot_accent		||
     let == ZZ_inv_roof_baltic	|| 	let == z_inv_roof_baltic	||
	 0
    );
}
////////////////////////////
Bool32 is_turkish_special(BYTE let)
{
return (
     let == AA_roof_accent			|| 	let == a_roof_accent			||
     let == CC_bottom_accent		|| 	let == c_bottom_accent			||
     let == GG_semicircle			|| 	let == g_semicircle				||
     let == II_dot_accent			|| 	let == i_sans_accent			||
     let == II_roof_accent			|| 	let == i_roof_accent			||
     let == OO_2dot_accent			|| 	let == o_2dot_accent			||
     let == SS_bottom_accent_turkish|| 	let == s_bottom_accent_turkish	||
     let == UU_2dot_accent			|| 	let == u_2dot_accent			||
     let == UU_roof_accent			|| 	let == u_roof_accent			||
	 0
    );
}
////////////////////////////
Bool32 NotStandardSymbol(int i)
{
  if( strchr(notStandLet,i) )
	  return TRUE;

  if( RomanLanguage()  )
  {
	  if(i>= 0xC0 && i < 0xDE )
		  return TRUE;

	  if( is_roman_special((BYTE)i) )
		  return TRUE;

	  switch(language)
	  {
	   case LANG_HUNGAR:
		   if( is_hungar_special((BYTE)i) )
		      return TRUE;
		   break;
       case LANG_CZECH:
		   if( is_czech_special((BYTE)i) )
		      return TRUE;
		   break;
	   case LANG_LITHUANIAN:
		   if( is_lithuanian_special((BYTE)i) )
		      return TRUE;
		   break;
       case LANG_LATVIAN:
		   if( is_latvian_special((BYTE)i) )
		      return TRUE;
		   break;
       case LANG_ESTONIAN:
		   if( is_estonian_special((BYTE)i) )
		      return TRUE;
		   break;
       case LANG_TURKISH:
		   if( is_turkish_special((BYTE)i) )
		      return TRUE;
		   break;

       default: break;
	  }
  }

  return FALSE;
}
///////////////
Bool32 WideSymbol(int i)
{
	if( strchr(wideLet,i) )
	  return TRUE;
    return FALSE;
}
////////////
Word8 TestCursivName(Word8 let)
{
		 char *qq;

		 if(!CyrilLanguage())
			 return let;

		 if( (qq=(char *)strchr(cursivNames,let))!=NULL )
			 let=notCursivNames[(Word8*)qq-cursivNames];

		 return let;
}
////////////////////////
