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

#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "func.h"

static WORD wb, hb;

static void rotate_8(PBYTE sr, PBYTE tr);

void c_rastror (PBYTE sr, PBYTE tr, WORD w, WORD h)
{
 PBYTE p;
 WORD l, ws, hs, loop1, loop2;
 wb = (w+7)/8; hb = (h+7)/8;
 p = sr + wb * h; l = wb * (hb*8 - h);
 if (l) memset (p,0,l);	// clear bottom of source raster
 ws = wb * 7; hs = hb * 8; loop1 = hb; tr += hb;
 goto enter_loop;
loop_1:
 p = tr; loop2 = wb;
loop_2:
 rotate_8(sr,p);
 sr++; p += hs; if (--loop2) goto loop_2;

 sr += ws;
enter_loop:
 tr--; if (loop1--) goto loop_1;
}

static BYTE subst1[256] = {
 0x00, 0x08, 0x80, 0x88, 0x04, 0x0C, 0x84, 0x8C, // 0x00
 0x40, 0x48, 0xC0, 0xC8, 0x44, 0x4C, 0xC4, 0xCC, // 0x08
 0x02, 0x0A, 0x82, 0x8A, 0x06, 0x0E, 0x86, 0x8E, // 0x10
 0x42, 0x4A, 0xC2, 0xCA, 0x46, 0x4E, 0xC6, 0xCE, // 0x18
 0x20, 0x28, 0xA0, 0xA8, 0x24, 0x2C, 0xA4, 0xAC, // 0x20
 0x60, 0x68, 0xE0, 0xE8, 0x64, 0x6C, 0xE4, 0xEC, // 0x28
 0x22, 0x2A, 0xA2, 0xAA, 0x26, 0x2E, 0xA6, 0xAE, // 0x30
 0x62, 0x6A, 0xE2, 0xEA, 0x66, 0x6E, 0xE6, 0xEE, // 0x38
 0x01, 0x09, 0x81, 0x89, 0x05, 0x0D, 0x85, 0x8D, // 0x40
 0x41, 0x49, 0xC1, 0xC9, 0x45, 0x4D, 0xC5, 0xCD, // 0x48
 0x03, 0x0B, 0x83, 0x8B, 0x07, 0x0F, 0x87, 0x8F, // 0x50
 0x43, 0x4B, 0xC3, 0xCB, 0x47, 0x4F, 0xC7, 0xCF, // 0x58
 0x21, 0x29, 0xA1, 0xA9, 0x25, 0x2D, 0xA5, 0xAD, // 0x60
 0x61, 0x69, 0xE1, 0xE9, 0x65, 0x6D, 0xE5, 0xED, // 0x68
 0x23, 0x2B, 0xA3, 0xAB, 0x27, 0x2F, 0xA7, 0xAF, // 0x70
 0x63, 0x6B, 0xE3, 0xEB, 0x67, 0x6F, 0xE7, 0xEF, // 0x78
 0x10, 0x18, 0x90, 0x98, 0x14, 0x1C, 0x94, 0x9C, // 0x80
 0x50, 0x58, 0xD0, 0xD8, 0x54, 0x5C, 0xD4, 0xDC, // 0x88
 0x12, 0x1A, 0x92, 0x9A, 0x16, 0x1E, 0x96, 0x9E, // 0x90
 0x52, 0x5A, 0xD2, 0xDA, 0x56, 0x5E, 0xD6, 0xDE, // 0x98
 0x30, 0x38, 0xB0, 0xB8, 0x34, 0x3C, 0xB4, 0xBC, // 0xA0
 0x70, 0x78, 0xF0, 0xF8, 0x74, 0x7C, 0xF4, 0xFC, // 0xA8
 0x32, 0x3A, 0xB2, 0xBA, 0x36, 0x3E, 0xB6, 0xBE, // 0xB0
 0x72, 0x7A, 0xF2, 0xFA, 0x76, 0x7E, 0xF6, 0xFE, // 0xB8
 0x11, 0x19, 0x91, 0x99, 0x15, 0x1D, 0x95, 0x9D, // 0xC0
 0x51, 0x59, 0xD1, 0xD9, 0x55, 0x5D, 0xD5, 0xDD, // 0xC8
 0x13, 0x1B, 0x93, 0x9B, 0x17, 0x1F, 0x97, 0x9F, // 0xD0
 0x53, 0x5B, 0xD3, 0xDB, 0x57, 0x5F, 0xD7, 0xDF, // 0xD8
 0x31, 0x39, 0xB1, 0xB9, 0x35, 0x3D, 0xB5, 0xBD, // 0xE0
 0x71, 0x79, 0xF1, 0xF9, 0x75, 0x7D, 0xF5, 0xFD, // 0xE8
 0x33, 0x3B, 0xB3, 0xBB, 0x37, 0x3F, 0xB7, 0xBF, // 0xF0
 0x73, 0x7B, 0xF3, 0xFB, 0x77, 0x7F, 0xF7, 0xFF  // 0xF8
};

static BYTE subst2[256] = {
 0x00, 0x01, 0x02, 0x03, 0x10, 0x11, 0x12, 0x13, // 0x00
 0x20, 0x21, 0x22, 0x23, 0x30, 0x31, 0x32, 0x33, // 0x08
 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17, // 0x10
 0x24, 0x25, 0x26, 0x27, 0x34, 0x35, 0x36, 0x37, // 0x18
 0x08, 0x09, 0x0A, 0x0B, 0x18, 0x19, 0x1A, 0x1B, // 0x20
 0x28, 0x29, 0x2A, 0x2B, 0x38, 0x39, 0x3A, 0x3B, // 0x28
 0x0C, 0x0D, 0x0E, 0x0F, 0x1C, 0x1D, 0x1E, 0x1F, // 0x30
 0x2C, 0x2D, 0x2E, 0x2F, 0x3C, 0x3D, 0x3E, 0x3F, // 0x38
 0x40, 0x41, 0x42, 0x43, 0x50, 0x51, 0x52, 0x53, // 0x40
 0x60, 0x61, 0x62, 0x63, 0x70, 0x71, 0x72, 0x73, // 0x48
 0x44, 0x45, 0x46, 0x47, 0x54, 0x55, 0x56, 0x57, // 0x50
 0x64, 0x65, 0x66, 0x67, 0x74, 0x75, 0x76, 0x77, // 0x58
 0x48, 0x49, 0x4A, 0x4B, 0x58, 0x59, 0x5A, 0x5B, // 0x60
 0x68, 0x69, 0x6A, 0x6B, 0x78, 0x79, 0x7A, 0x7B, // 0x68
 0x4C, 0x4D, 0x4E, 0x4F, 0x5C, 0x5D, 0x5E, 0x5F, // 0x70
 0x6C, 0x6D, 0x6E, 0x6F, 0x7C, 0x7D, 0x7E, 0x7F, // 0x78
 0x80, 0x81, 0x82, 0x83, 0x90, 0x91, 0x92, 0x93, // 0x80
 0xA0, 0xA1, 0xA2, 0xA3, 0xB0, 0xB1, 0xB2, 0xB3, // 0x88
 0x84, 0x85, 0x86, 0x87, 0x94, 0x95, 0x96, 0x97, // 0x90
 0xA4, 0xA5, 0xA6, 0xA7, 0xB4, 0xB5, 0xB6, 0xB7, // 0x98
 0x88, 0x89, 0x8A, 0x8B, 0x98, 0x99, 0x9A, 0x9B, // 0xA0
 0xA8, 0xA9, 0xAA, 0xAB, 0xB8, 0xB9, 0xBA, 0xBB, // 0xA8
 0x8C, 0x8D, 0x8E, 0x8F, 0x9C, 0x9D, 0x9E, 0x9F, // 0xB0
 0xAC, 0xAD, 0xAE, 0xAF, 0xBC, 0xBD, 0xBE, 0xBF, // 0xB8
 0xC0, 0xC1, 0xC2, 0xC3, 0xD0, 0xD1, 0xD2, 0xD3, // 0xC0
 0xE0, 0xE1, 0xE2, 0xE3, 0xF0, 0xF1, 0xF2, 0xF3, // 0xC8
 0xC4, 0xC5, 0xC6, 0xC7, 0xD4, 0xD5, 0xD6, 0xD7, // 0xD0
 0xE4, 0xE5, 0xE6, 0xE7, 0xF4, 0xF5, 0xF6, 0xF7, // 0xD8
 0xC8, 0xC9, 0xCA, 0xCB, 0xD8, 0xD9, 0xDA, 0xDB, // 0xE0
 0xE8, 0xE9, 0xEA, 0xEB, 0xF8, 0xF9, 0xFA, 0xFB, // 0xE8
 0xCC, 0xCD, 0xCE, 0xCF, 0xDC, 0xDD, 0xDE, 0xDF, // 0xF0
 0xEC, 0xED, 0xEE, 0xEF, 0xFC, 0xFD, 0xFE, 0xFF  // 0xF8
 };

static void rotate_8 (PBYTE sr, PBYTE tr)
{
 BYTE w1, w2, w3, w4;
 BYTE b1, b2, b3, b4, b5, b6, b7, b8;
 b1=*sr; sr+=wb; b2=*sr; sr+=wb; b3=*sr; sr+=wb; b4=*sr; sr+=wb;
 b5=*sr; sr+=wb; b6=*sr; sr+=wb; b7=*sr; sr+=wb; b8=*sr;

 w1 = (b1 & 0xf0) + (b2 >> 4);
 w2 = (b3 & 0xf0) + (b4 >> 4);
 w3 = (b1 << 4) + (b2 & 0x0f);
 w4 = (b3 << 4) + (b4 & 0x0f);

 w1 = subst2[w1]; w2 = subst2[w2];
 b1 = subst1[(w1 & 0xf0) + (w2 >> 4)];
 b2 = subst1[((w1 & 0x0f) << 4) + (w2 & 0x0f)];    // square 00

 w3 = subst2[w3]; w4 = subst2[w4];
 b3 = subst1[(w3 & 0xf0) + (w4 >> 4)]; 	// square 01
 b4 = subst1[((w3 &0x0f) << 4) + (w4 & 0x0f)];

 w1 = (b5 & 0xf0) + (b6 >> 4);
 w2 = (b7 & 0xf0) + (b8 >> 4);
 w3 = (b5 << 4) + (b6 & 0x0f);
 w4 = (b7 << 4) + (b8 & 0x0f);

 w1 = subst2[w1]; w2 = subst2[w2];
 b5 = subst1[(w1 & 0xf0) + (w2 >> 4)];	// square 10
 b6 = subst1[((w1 & 0x0f) << 4) + (w2 & 0x0f)];

 w3 = subst2[w3]; w4 = subst2[w4];
 b7 = subst1[(w3 & 0xf0 )+ (w4 >> 4)];	// square 11
 b8 = subst1[((w3 & 0x0f) << 4) + (w4 & 0x0f)];

// 11->10; 00->01; 01-> 11; 10->00
 *tr = (b5 & 0xf0) + (b1 >> 4); tr += hb;
 *tr = (b1 & 0x0f) + (b5 << 4); tr += hb;
 *tr = (b6 & 0xf0) + (b2 >> 4); tr += hb;
 *tr = (b2 & 0x0f) + (b6 << 4); tr += hb;
 *tr = (b7 & 0xf0) + (b3 >> 4); tr += hb;
 *tr = (b3 & 0x0f) + (b7 << 4); tr += hb;
 *tr = (b8 & 0xf0) + (b4 >> 4); tr += hb;
 *tr = (b4 & 0x0f) + (b8 << 4);

}
