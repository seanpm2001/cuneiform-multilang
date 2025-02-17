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


/********************************************************************/
/*                                                                  */
/*                proportional indexes for narrow letters           */
/*                                                                  */
/********************************************************************/

INT    prop_narrow[256]=
{
/*   0   */0,
/*   1  */0,
/*   2  */0,
/*   3  */0,
/*   4  */0,
/*   5  */0,
/*   6  */0,
/*   7  */0,
/*   8  */0,
/*   9   */0,
/*  10   */0,
/*  11  */0,
/*  12   */0,
/*  13   */0,
/*  14  */0,
/*  15  */0,
/*  16  */0,
/*  17  */0,
/*  18  */0,
/*  19  */0,
/*  20  */0,
/*  21  */0,
/*  22  */0,
/*  23  */0,
/*  24  */0,
/*  25  */0,
/*  26  */0,
/*  27  */0,
/*  28  */0,
/*  29  */0,
/*  30  */0,
/*  31  */0,
/*  32   */0,
/*  33 ! */0,
/*  34 " */0,
/*  35 # */0,
/*  36 $ */0,
/*  37 % */0,
/*  38 & */0,
/*  39 ' */0,
/*  40 ( */0,
/*  41 ) */0,
/*  42 * */0,
/*  43 + */0,
/*  44 , */0,
/*  45 - */0,
/*  46 . */0,
/*  47 / */0,
/*  48 0 */38,
/*  49 1 */0,
/*  50 2 */34,
/*  51 3 */34,
/*  52 4 */34,
/*  53 5 */34,
/*  54 6 */34,
/*  55 7 */34,
/*  56 8 */34,
/*  57 9 */34,
/*  58 : */0,
/*  59 ; */0,
/*  60 < */0,
/*  61 = */0,
/*  62 > */0,
/*  63 ? */0,
/*  64 @ */0,
/*  65 A */40,
/*  66 B */40,
/*  67 C */40,
/*  68 D */40,
/*  69 E */40,
/*  70 F */40,
/*  71 G */40,
/*  72 H */43,
/*  73 I */0,
/*  74 J */0,
/*  75 K */40,
/*  76 L */40,
/*  77 M */0,
/*  78 N */40,
/*  79 O */0,
/*  80 P */40,
/*  81 Q */40,
/*  82 R */40,
/*  83 S */41,
/*  84 T */40,
/*  85 U */0,
/*  86 V */40,
/*  87 W */0,
/*  88 X */40,
/*  89 Y */40,
/*  90 Z */36,
/*  91 [ */0,
/*  92 \ */0,
/*  93 ] */0,
/*  94 ^ */0,
/*  95 _ */0,
/*  96 ` */0,
/*  97 a */40,
/*  98 b */40,
/*  99 c */40,
/* 100 d */40,
/* 101 e */40,
/* 102 f */0,
/* 103 g */36,
/* 104 h */36,
/* 105 i */0,
/* 106 j */0,
/* 107 k */30,
/* 108 l */0,
/* 109 m */0,
/* 110 n */40,
/* 111 o */40,
/* 112 p */36,
/* 113 q */0,
/* 114 r */0,
/* 115 s */40,
/* 116 t */0,
/* 117 u */40,
/* 118 v */40,
/* 119 w */0,
/* 120 x */40,
/* 121 y */40,
/* 122 z */40,
/* 123 { */0,
/* 124 | */0,
/* 125 } */0,
/* 126 ~ */0,
/* 127  */0,
/* 128 � */44,
/* 129 � */40,
/* 130 � */40,
/* 131 � */40,
/* 132 � */34,
/* 133 � */40,
/* 134 � */0,
/* 135 � */32,
/* 136 � */40,
/* 137 � */0,
/* 138 � */36,
/* 139 � */40,
/* 140 � */0,
/* 141 � */40,
/* 142 � */40,
/* 143 � */40,
/* 144 � */40,
/* 145 � */40,
/* 146 � */40,
/* 147 � */36,
/* 148 � */40,
/* 149 � */40,
/* 150 � */40,
/* 151 � */40,
/* 152 � */0,
/* 153 � */0,
/* 154 � */40,
/* 155 � */0,
/* 156 � */40,
/* 157 � */40,
/* 158 � */0,
/* 159 � */40,
/* 160 � */40,
/* 161 � */36,
/* 162 � */40,
/* 163 � */32,
/* 164 � */40,
/* 165 � */40,
/* 166 � */0,
/* 167 � */35,
/* 168 � */45,
/* 169 � */0,
/* 170 � */40,
/* 171 � */40,
/* 172 � */0,
/* 173 � */40,
/* 174 � */40,
/* 175 � */40,
/* 176 � */0,
/* 177 � */0,
/* 178 � */0,
/* 179 � */0,
/* 180 � */0,
/* 181 � */0,
/* 182 � */0,
/* 183 � */0,
/* 184 � */0,
/* 185 � */0,
/* 186 � */0,
/* 187 � */0,
/* 188 � */0,
/* 189 � */0,
/* 190 � */0,
/* 191 � */0,
/* 192 � */0,
/* 193 � */0,
/* 194 � */0,
/* 195 � */0,
/* 196 � */0,
/* 197 � */0,
/* 198 � */0,
/* 199 � */0,
/* 200 � */0,
/* 201 � */0,
/* 202 � */0,
/* 203 � */0,
/* 204 � */0,
/* 205 � */0,
/* 206 � */0,
/* 207 � */0,
/* 208 � */0,
/* 209 � */0,
/* 210 � */0,
/* 211 � */0,
/* 212 � */0,
/* 213 � */0,
/* 214 � */0,
/* 215 � */0,
/* 216 � */0,
/* 217 � */0,
/* 218 � */0,
/* 219 � */0,
/* 220 � */0,
/* 221 � */0,
/* 222 � */0,
/* 223 � */0,
/* 224 � */35,
/* 225 � */36,
/* 226 � */40,
/* 227 � */38,
/* 228 � */40,
/* 229 � */40,
/* 230 � */40,
/* 231 � */40,
/* 232 � */0,
/* 233 � */0,
/* 234 � */40,
/* 235 � */0,
/* 236 � */40,
/* 237 � */36,
/* 238 � */0,
/* 239 � */40,
/* 240 � */0,
/* 241 � */0,
/* 242 � */0,
/* 243 � */0,
/* 244 � */0,
/* 245 � */0,
/* 246 � */0,
/* 247 � */0,
/* 248 � */0,
/* 249 � */0,
/* 250 � */0,
/* 251 � */0,
/* 252 � */0,
/* 253 � */48,
/* 254 � */0,
/* 255 � */0
};
