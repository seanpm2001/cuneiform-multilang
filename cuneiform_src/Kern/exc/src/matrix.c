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
/*                 Dot matrix tiff preparation                      */
/*                                                                  */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "globus.h"
//#include "nt_types.h"
//#include "status.h"
#include "msgerr.h"

//AK changes for MSVisual
//#include "spelwatc.h"

#define LBUF 0x6000

static Word8 *grim1 = NULL, *grim2;	// Pointer to internal buffer
static Int16 ls, nsb1;		// Length of string and strings per buffer
static Word16 rest;		// Length of rest in the buffer after previous read
static Word8 reserv[2048];
extern Int16 image_lth     ;  // bytes per line
static void matrix_process ();


void matrix_open ()
{
 grim1 = calloc (2,LBUF); 	// No memory
 if (grim1==NULL)
  error_exit(ERR_comp,13);
 grim2 = grim1 + LBUF;
 ls = image_lth +2 ;
 rest = 0;
}

void matrix_close()
{
 if (grim1)
    {
	 grim2 = NULL;
	 free (grim1);
	 grim1 = NULL;
    }
}

void matrix_reset ()
{
 matrix_close();
}

Int16 matrix_read(Word8 *buff, Word16 lth)
 {
 Int16 lmax, l;
 Word8 *pc, *pwr;

 // Read image cycle
 pwr = buff;
 l = rest % ls;
 pc = grim1 + rest;
 // read one line of raster
/*
#ifdef _DEBUG
  //AK! add for konsole apps when read 1 lines of raster
 if (lth == image_lth)
 {
    *pc++ = 0;
	memcpy(pc,buff,ls-2);
	pc += ls-2;
	*pc++ = 0;
	lmax = ls;
	nsb1 = 3;
	rest = ls * 2;
	memcpy(reserv, pc - rest, rest);
	matrix_process();
    pc = grim2 + ls + 1;
    memcpy (pwr,pc,ls-2);
	memcpy (grim1,reserv,rest);
 }
 else
 {
#endif  */
	 if (l)
       {
        memcpy (pc, buff, ls - l);
        buff += ls - l - 1;
		pc += ls - l;
		*(pc -1) = 0;
        lth -= ls - l - 1;
       }
     while (ls - 2 <= lth)
       {
        *pc++ = 0;
        memcpy (pc,buff,ls-2);
        buff += ls-2;
	    pc +=ls-2;
	    *pc++ = 0;
        lth -= ls-2;
       }

     if (lth)
       {
        *pc++ = 0;
	    memcpy (pc,buff,lth);
	    pc+=lth;
       }

     lmax = pc - grim1;
     nsb1 = lmax/ls;
     l = nsb1 - 2;
     rest = lmax - l*ls;
     memcpy (reserv, pc - rest, rest);
     matrix_process();

     pc = grim2 + ls + 1;
     while (l-- > 0)                             //AK added > 0  14.08.97
       {
        memcpy (pwr,pc,ls-2);
        pwr += ls-2;
        pc += ls;
       }
     memcpy (grim1,reserv,rest);
/*
#ifdef _DEBUG
   }
#endif
   */
 return (nsb1-2)*(ls-2);
}

static void matrix_process ()
{
// Process bitmap of nbs1 lines, source in grim1, destinator - grim2. ls - length of line
 Word8 *bw1,*bw2,*p;
 Word8 a,x1,x2,x3,x4,x5,y1,y2,y3,y4,y5,z1,z2,z3,z4,z5;
 Int16 j,k;

  memcpy(grim2,grim1,nsb1*ls);
  for (bw1=grim1+ls+1,bw2=grim2+ls+1,j=1; j<nsb1-1; j++, bw1+=2, bw2+=2)
   for (k=1; k<ls-1; k++,bw1++,bw2++)
    *bw2|=((a=*bw1)>>1)|(a<<1)|(*(bw1-1)<<7)|(*(bw1+1)>>7)|
          (a=*(p=bw1-ls))| /*(a>>1)|(a<<1)|(*(p-1)<<7)|(*(p+1)>>7)|*/
          (a=*(p=bw1+ls)) /*|(a>>1)|(a<<1)|(*(p-1)<<7)|(*(p+1)>>7)*/;
  memcpy(grim1,grim2,nsb1*ls);
  for (bw1=grim1+ls+1,bw2=grim2+ls+1,j=1; j<nsb1-1; j++,bw1+=2,bw2+=2)
   for (k=1; k<ls-1; k++,bw1++,bw2++)
    {
    y3=*bw1;        y1=(y3>>2)|((a=*(bw1-1))<<6); y2=(y3>>1)|(a<<7);
                    y4=(y3<<1)|((a=*(bw1+1))>>7); y5=(y3<<2)|(a>>6);
    x3=*(p=bw1-ls); x1=(x3>>2)|((a=*(p-1))<<6);   x2=(x3>>1)|(a<<7);
                    x4=(x3<<1)|((a=*(p+1))>>7);   x5=(x3<<2)|(a>>6);
    z3=*(p=bw1+ls); z1=(z3>>2)|((a=*(p-1))<<6);   z2=(z3>>1)|(a<<7);
                    z4=(z3<<1)|((a=*(p+1))>>7);   z5=(z3<<2)|(a>>6);
    *bw2|=(x3&(z2|z3|z4))|(z3&(x2|x4))|
          (y2&(x4|y4|z4|x5|y5|z5))|(y4&(x2|z2|x1|y1|z1))|
          (x2&(z4|y5))|(x4&(z2|y1))|(z2&y5)|(z4&y1);
    }
  memcpy(grim1,grim2,nsb1*ls);
  for (bw1=grim1+ls+1,bw2=grim2+ls+1,j=1; j<nsb1-1; j++,bw1+=2,bw2+=2)
   for (k=1; k<ls-1; k++,bw1++,bw2++)
    *bw2|=((((*bw1)>>1)|(*(bw1-1)<<7))&(((*bw1)<<1)|(*(bw1+1)>>7)))|
	  ((*(bw1-ls))&(*(bw1+ls)));
}
