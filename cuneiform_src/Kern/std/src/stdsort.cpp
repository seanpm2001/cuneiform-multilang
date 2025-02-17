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

#include "internal.h"
#pragma hdrstop

#include <string.h>
#include <std.h>

static void reverse(char *p1,char *p2, Int32 w);
static void piramida(char *pAr,Int32 firstel,Int32 N, Int32 w,
					Int32 (*compare)(const void *elm1,const void *elm2) );

STD_FUNC(void) stdQsort(void *pMas,Int32 Num, Int32 width,
			  Int32 (*compare)(const void *elm1,const void *elm2) )
{
 Int32 i,CurNum=Num;

 if( Num<2 )
    return;
 for(i=Num/2;i>0;i--)
    piramida((char *)pMas,i,Num,width,compare);

 do
 {
  char *p1=(char *)pMas,*p2=(char *)pMas+(CurNum-1)*width;
  reverse(p1,p2,width);
  CurNum--;
  piramida((char *)pMas,1,CurNum,width,compare);
 }
 while(CurNum>1);
}

static void piramida(char *pAr,Int32 firstel,Int32 N, Int32 w,
					Int32 (*compare)(const void *elm1,const void *elm2) )
{
 Int32 i,i_next;
 char *p1,*p2;

 i=firstel;
 while(1)
 {
  if( i+i > N ) return;
  if( i+i+1 > N )
  {
   p1=pAr+(i+i-1)*w; p2=pAr+(i-1)*w;
   if( compare((void *)(p1), (void *)(p2) )>0 )
    reverse(p2,p1,w);
   return;
  }

  p1=pAr+(i+i-1)*w; p2=pAr+(i+i)*w;
  if( compare( (void *)(p1), (void *)(p2))>0 ) i_next=i+i;
  else i_next=i+i+1;

  p1=pAr+(i-1)*w; p2=pAr+(i_next-1)*w;
  if( compare( (void *)(p1), (void *)(p2))>0 ) return;

  reverse(p1,p2,w);
  i=i_next;
 } // while

}

static void reverse(char *p1,char *p2, Int32 w)
{
  Int32 *t1=(Int32 *)p1,*t2=(Int32 *)p2,tmp;
  Int32 len,ost,i;
  char *c1,*c2,c ;

  ost=w%4; len=(w-ost)/4;
  c1=p1+w-ost; c2=p2+w-ost;
  for(i=0;i<len;i++,t1++,t2++)
  {
	tmp=*t1;
	*t1=*t2;
	*t2=tmp;
  }
  for(i=0;i<ost;i++,c1++,c2++)
  {
    c=*c1;
	*c1=*c2;
	*c2=c;
  }


}
