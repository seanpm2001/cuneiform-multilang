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

/*    Image access protocol
 *
 */

#ifndef __IMXS_H
#define __IMXS_H

#include "cttypes.h"

#pragma pack(4)

typedef struct tagImxs_ImageInfo
{
    Word16  wImageHeight;           /* Height of the image in lines       */
    Word16  wImageWidth;            /* Width of the image in pixels       */
    Word16  wImageByteWidth;        /* Size of the image line in bytes    */
    Word16  wImageDisplacement;     /* Displacement of the first pixel of */
                                    /*   image line in pixels, usually 0  */
    Word16  wResolutionX;           /* Resolution by X-axe */
    Word16  wResolutionY;           /* Resolution by Y-axe */
    Word8   bFotoMetrics;             /* White pixel */
    Word8   bUnused;                  /* not used; for alignment purpose only */

    Word16  wAddX;
    Word16  wAddY;
} Imxs_ImageInfo;

#pragma pack()

	#ifdef __cplusplus
	extern "C"  {
	#endif
		typedef Int16  (*TImageGetBlock )(Word8* lpBuff, Int16 fstLine, Int16 nLines);
      typedef Bool16 (*TImageOpen     )(Imxs_ImageInfo* lpImageInfo);
		typedef Int16  (*TImageRead     )(Word8* lpImage, Word16 wMaxSize);
		typedef Bool16 (*TImageClose    )(void);
	#ifdef __cplusplus
	}
	#endif

typedef  struct tagImxs
   {
      TImageOpen           f_op;
      TImageRead           f_re;  // may be NULL
      TImageGetBlock       f_gb;  // may be NULL
      TImageClose          f_cl;
   }  Imxs;

#endif
