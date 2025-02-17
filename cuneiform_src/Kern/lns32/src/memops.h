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

#ifndef __MEMOPS_H
#define __MEMOPS_H
#ifdef __cplusplus

/*#ifndef NO_ASM*/
#if 0
   extern "C"
   {
//#endif
      // realized in 'memops.asm'
      void CopyOR(         void*             dst,
                           void*             src,
                           unsigned short    dword_cnt
                 );
      void CopyAND(         void*             dst,
                           void*             src,
                           unsigned short    dword_cnt
                 );

      #undef memcpy
      void* memcpy( void*          dst,
                    const void*    src,
                    unsigned int   byte_cnt
                  );

      void InvertTo( void*   dst,
                     void*   src,
                     unsigned short dwcnt
                   );

      void InvertSelf(  void*   dst,
                        unsigned short dwcnt
                     );
   #ifdef __cplusplus
   }
   #endif

#else // ============ NO_ASM ============

   #ifndef MEMOPS_INLINE
   #define MEMOPS_INLINE inline
      // you can use here 'static' instead
      // if you have some problems with 'inline'
   #endif

MEMOPS_INLINE
void __CopyOR(Word8* dst, const Word8* src, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ |= *src++;
	}
}

MEMOPS_INLINE
void CopyOR(void* dst, const void* src, const unsigned short dword_cnt) {
	__CopyOR((Word8*)dst, (Word8*)src, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void __CopyAND(Word8* dst, const Word8* src, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ &= *src++;
	}
}

MEMOPS_INLINE
void CopyAND(void* dst, const void* src, const unsigned short dword_cnt) {
	__CopyAND((Word8*)dst, (Word8*)src, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void __InvertSelf(Word8* dst, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ = ~*dst;
	}
}

MEMOPS_INLINE
void InvertSelf(void* dst, const unsigned short dword_cnt) {
	__InvertSelf((Word8*)dst, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void InvertTo(void* dst, const void* src, const unsigned short dword_cnt) {
	unsigned char *ldst = (unsigned char*)dst;
	unsigned char *lsrc = (unsigned char*)src;
	for(int i=0; i<4*(int)dword_cnt; i++) {
		*ldst++ = ~*lsrc++;
	}
}



#endif
#endif
#endif
