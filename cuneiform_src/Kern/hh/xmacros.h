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

#ifndef __XMACROS_H
#define __XMACROS_H

#ifdef __cplusplus
     #define Private( Type, Var )                     \
		private:  Type  Var;                           \
														\
		public:                                        \
		  inline Type const & Get##Var ( void ) const   \
			{ return Var; }

	  #define Protected( Type, Var )                   \
		protected:                                      \
		  Type  Var;                                    \
		public:                                         \
		  inline Type const & Get##Var( void )  const 	\
			{ return Var; }


         template <class T> inline void swap( T& a, T& b )
            {
               T tmp; tmp = a; a = b; b = tmp;
            }

#endif

#ifdef NDEBUG
   #define REPORT_LINE()
#else
   #define REPORT_LINE() \
         stdConsole("*** File %s, line %d", __FILE__, __LINE__);
#endif
#ifdef NDEBUG
   #define COMMENT_LINE()
#else
   #define COMMENT_LINE( comment ) \
      stdConsole("*** File %s, line %d: %s", __FILE__, __LINE__, comment);
#endif

   #define RET_FALSE  { REPORT_LINE(); return FALSE; }
   #define RET_ZERO   { REPORT_LINE(); return 0;     }
   #define RET_VOID   { REPORT_LINE(); return ;      }
   #define RET_NULL   { REPORT_LINE(); return NULL;  }
   #define RET_FFFF   { REPORT_LINE(); return -1;    }


   #ifndef IFRETFALSE
   #define IFRETFALSE( ptr )  { if ( ptr == NULL )  RETFALSE; };
   #endif

   #ifndef IFRETNULL
   #define IFRETNULL( ptr )  { if ( ptr == NULL )  RETNULL; };
   #endif

   #ifndef IFRETVOID
   #define IFRETVOID( ptr )  { if ( ptr == NULL )  RETVOID; };
   #endif

   #ifndef RETFALSE
   #define RETFALSE  {  assert(0); return FALSE; };
   #endif

   #ifndef RETZERO
   #define RETZERO   {  assert(0); return 0; };
   #endif


   #ifndef RETNULL
   #define RETNULL   {  assert(0); return NULL; };
   #endif

   #ifndef RETVOID
   #define RETVOID   {  assert(0); return; };
   #endif

   #ifndef RETERR
   #define RETERR(err) {  assert(0); return err; };
   #endif

   #ifndef RETIFERR
   #define RETIFERR(expr) {  Err16 err = (expr); if (err!=ER_NONE) return err; }
   #endif

   #ifndef Set0
   #define Set0( t ) memset( &t, 0, sizeof(t) );
   #endif

   #ifndef DELPTR
   #define DELPTR( ptr ) { if (ptr!=NULL) { delete ptr; ptr=NULL; } };
   #endif

   #ifndef LENGTH_OF
   #define LENGTH_OF( arr ) (sizeof(arr)/sizeof(arr[0]))
   #endif

   #include "minmax.h"

   #ifndef RCEXP
   #define RCEXP(rc)  rc.left, rc.top, rc.right, rc.bottom
   #endif

   #ifndef PTEXP
   #define PTEXP(pt)  pt.x, pt.y
   #endif

#endif
