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

#ifndef __DIF_H
   #define __DIF_H

   #ifndef __GLOBUS_H
      #include "globus.h"
   #endif
   #include "difdefs.h"

#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"

   #ifdef __DIF__
      #define DIF_FUNC  FUN_EXPO
   #else
      #define DIF_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif

   DIF_FUNC(Bool32)  DIFInit( RecRaster *r, Bool32 broken, Bool32 broken_II,
                    Bool32 cut_left, Bool32 cut_right);
      // Start Initialization; TRUE - OK
      // for mem==NULL need used internal memory functions from DLL
      // New call - after close library without DIFDone
   DIF_FUNC(void)  DIFSetFont( Int32 typ_of_font);

   DIF_FUNC(Bool32)  DIFSetAlphabet(
        char*     char_tbl_put_to ); // char table[0-255]
      // Set alphabet for recognition

   DIF_FUNC(Bool32)  DIFPenaltyChar(
      RecRaster*  raster,     // raster description
      RecVersions* DIFres );  // acuracy

  DIF_FUNC(Bool32)  DIFStick_expert(
      Int16 dx, Int16 dy,    // raster description
      Word8*      pool,
      RecVersions* DIFres );  // acuracy

  DIF_FUNC(Int32)  DIFGet_incline( RecRaster*  r, Word8 *pool);

   DIF_FUNC(void)  DIFDone(void);
      // Stop function. Call after DIFInit()

   DIF_FUNC(Int16) DIFGetErr(void);
      // Err16.h defined codes or one of the next

  DIF_FUNC(Int16) DIFLeftDistance(Word8 *RASTER, Int16 NWIDTH);
  DIF_FUNC(Int16) DIFRightDistance(Word8 *RASTER, Int16 NWIDTH);
  DIF_FUNC(void)  DIFClearRightZone(Word8 *RASTER, Int16 NWIDTH, Int16 WBYTE, Int16 NHEIGHT);
  DIF_FUNC(Int16) DIF_GetNoCutPoint(Word8 *RASTER, Int16 Wb, Word8 NWIDTH, Word8 NLENGTH);
  DIF_FUNC(Bool32) DIF_SetLanguage(Word8 lang);
  DIF_FUNC(Word16) DIF_typ_thin_stick(Word8 *lin,Int16 dy,Int16 dx);
   #ifdef __cplusplus
            }
   #endif

#endif

