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
/*                          Skew of line                            */
/*                                                                  */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "func.h"

struct pairs {INT x,y; CHAR f;};
typedef struct pairs pairs;

extern INT nIncline;
extern INT page_nIncline;
extern INT line_number;
extern BYTE fax1x2;

static INT row,row_prev;
static INT incline_prev;
static INT relab;

static INT skew_pool_fill(CSTR_line,pairs *);
static INT skew_pool_refill(CSTR_line ln,pairs *pool);
static INT skew_pool_refill2(CSTR_line ln,pairs *pool);

static INT incl_init(INT,pairs *);
static WORD skew_stat(INT,CSTR_line,INT,pairs *);
static void total_skew();

#define DINCL  10
#define DINCLF 50
#define DPREVL 300
#define NRELABMIN 7
#define NRELABMAX 30

Int16 skew_corr(CSTR_line ln, INT pool_src)
 {
 INT skew_pool_n=0,inclini;
 WORD l,l1;
 pairs skew_pool[255];

 if (pool_src==0)
  skew_pool_n=skew_pool_fill(ln,skew_pool);
 else if (pool_src==1)
  skew_pool_n=skew_pool_refill(ln,skew_pool);
 else if (pool_src==2)  // for BAL !
  skew_pool_n=skew_pool_refill2(ln,skew_pool);

 if (skew_pool_n <=1)
	 return nIncline;

 nIncline=incl_init(skew_pool_n,skew_pool);

 total_skew();
 inclini=nIncline;
 l=skew_stat(nIncline,ln,skew_pool_n,skew_pool);
 if (l==0)
	 return nIncline;
 while ((l1=skew_stat((INT)(nIncline-1),ln,skew_pool_n,skew_pool))>l)
  { nIncline--; l=l1;}
 if (nIncline==inclini)
  while ((l1=skew_stat((INT)(nIncline+1),ln,skew_pool_n,skew_pool))>l)
   { nIncline++; l=l1;}
 total_skew();

 return nIncline;
}

static INT skew_pool_fill(CSTR_line ln,pairs *pool)
 {
 INT /*nl,*/i;
 pairs *p;
 CSTR_rast       rst=CSTR_GetFirstRaster(ln);
 CCOM_comp *com;
 CSTR_attr		attrlin;
 CSTR_rast_attr attr;

 CSTR_GetLineAttr(ln, &attrlin);
 rst=CSTR_GetNext(rst);
 if( !rst )
	 return 0;
 com=CSTR_GetComp(rst);
 row=com->upper;

 //nl=(c_comp **)((PCHAR)ln+ln->dust)-&(ln->c_comp[0]);
 //if (nl>255) nl=255;
 for (	 p=pool,i=0; i<255 && rst; rst=CSTR_GetNext(rst))
  {
  com=CSTR_GetComp(rst);
  CSTR_GetAttr(rst,&attr);
  if( !(attr.flg&CSTR_f_dust) )
      {
      p->x=com->left;
      p->y=(INT)(com->upper+com->h-row);//attrlin.row-1);
      i++;
      p++;
      }
  }
 return i;
 }

static INT skew_pool_refill(CSTR_line ln,pairs *pool)
 {
 INT nl;
 pairs *p;
 cell *cl;
 CSTR_attr		attrlin;

 CSTR_GetLineAttr(ln, &attrlin);
 cl=cell_f();
 p=pool;
 nl=0;
 while ((cl=cl->nextl)->nextl != NULL)
  {
  if (cl->difflg&64 && !(cl->difflg&4)) continue;   // skip totally forbidden
  p->x=cl->r_col;
  p->y=(INT)(cl->r_row+cl->h - attrlin.row);//-1);
  p++;
  nl++;
  if (nl > 254) break;
  }
 return nl;
 }

static INT skew_pool_refill2(CSTR_line ln,pairs *pool)
 {
 INT nl;
 pairs *p;
 CSTR_attr		attrlin;
 CSTR_rast       rst=CSTR_GetFirstRaster(ln);
 CSTR_rast_attr attr;

 CSTR_GetLineAttr(ln, &attrlin);

 p=pool;

// while ((cl=cl->nextl)->nextl != NULL)
 rst = CSTR_GetNextRaster(rst, CSTR_f_let | CSTR_f_bad);
 for(nl=0;nl<255; rst = CSTR_GetNextRaster(rst, CSTR_f_let | CSTR_f_bad) )
 {
  if( !rst )
	  break;
  CSTR_GetAttr(rst, &attr);

  if ( (attr.difflg&64) && !(attr.difflg&4))
	  continue;   // skip totally forbidden
  p->x=attr.r_col;
  p->y=(INT)(attr.r_row+attr.h - attrlin.row);//-1);
  p++;
  nl++;
 }
 return nl;
}

static INT incl_init(INT n,pairs *pool)
 {
 INT i,incl,shift,m,d;
 LONG sx,sy,sxy,sx2,w,sigma,dd;
 pairs *p;

 for (p=pool,sx=sy=sxy=sx2=i=0; i<n; i++,p++)
  {
  sx+=p->x;
  sy+=p->y;
  sxy+=((LONG)p->x)*(p->y);
  sx2+=((LONG)p->x)*(p->x);
  }
 w=(n*((sx2+128)/256)-((sx+8)/16)*((sx+8)/16));
 if (w<=0)
  {relab=0; return nIncline;}
 dd=8*(n*sxy-sx*sy);
 incl=(INT)((dd+((dd>0)?w/2:-w/2))/w);
 shift=(INT)((((sx2+128)/256)*sy-((sxy+8)/16)*((sx+8)/16)+w/2)/w);
 for (p=pool,w=i=0; i<n; i++,p++)
  {
  dd=(LONG)incl*p->x;
  d=p->y-(INT)((dd+((dd>0)?1024:-1024))/2048)-shift;
  if (abs(d)>100)
   d=100;
  w+=d*d;
  }
 sigma=(w+n/2)/n;
 if (sigma<=((fax1x2)?2:1))
  {
  if (n>=NRELABMIN)
   relab=1;
  else
   relab=0;
  return incl;
  }
 for (p=pool,sx=sy=sxy=sx2=m=i=0; i<n; i++,p++)
  {
  dd=(LONG)incl*p->x;
  d=p->y-(INT)((dd+((dd>0)?1024:-1024))/2048)-shift;
  if (abs(d)<100 && d*d<=sigma)
   {
   sx+=p->x;
   sy+=p->y;
   sxy+=((LONG)p->x)*(p->y);
   sx2+=((LONG)p->x)*(p->x);
   p->f=1;
   m++;
   }
  else p->f=0;
  }
 w=m*((sx2+128)/256)-((sx+8)/16)*((sx+8)/16);
 if (w>0 && 3*m>2*n)
  {
  dd=8*(m*sxy-sx*sy);
  incl=(INT)((dd+((dd>0)?w/2:-w/2))/w);
  shift=(INT)((((sx2+128)/256)*sy-((sxy+8)/16)*((sx+8)/16)+w/2)/w);
  for (p=pool,w=i=0; i<n; i++,p++)
   if (p->f)
    {
    dd=(LONG)incl*p->x;
    d=p->y-(INT)((dd+((dd>0)?1024:-1024))/2048)-shift;
    if (abs(d)>100)
     d=100;
    w+=d*d;
    }
  sigma=(w+m/2)/m;
  if (m>=NRELABMIN && sigma<=((fax1x2)?2:1) ||
      m>=NRELABMAX && sigma<=((fax1x2)?3:2))
   relab=1;
  else
   relab=0;
  }
 else
  {incl=nIncline; relab=0;}
 return incl;
 }

#define HIMAX  1000

static WORD skew_stat(INT incl,CSTR_line ln,INT pool_n,pairs *pool)
 {
 INT i,im,d;
 WORD l;
 BYTE hist[HIMAX];
 CSTR_attr		attrlin;

 CSTR_GetLineAttr(ln, &attrlin);
 im=(INT)(2*attrlin.hei);
 if (im<0 || im>=HIMAX) return 0;
 memset(hist,0,HIMAX);// Piter change im) for HIMAX;
 for (i=0; i<pool_n; i++)
  {
  d=im/2+pool[i].y-(INT)((LONG)incl*pool[i].x/2048);
  if (d>0 && d<HIMAX) hist[d]++;
  }
 for (l=i=0; i<im; i++)
  l+=(WORD)hist[i]*hist[i];
 return l;
 }

static void total_skew()
 {
 INT totincl;

 if (relab) return;
 if (line_number>1 && abs(row-row_prev)<=DPREVL)
  totincl=incline_prev;
 else totincl=page_nIncline;
 if (abs(nIncline-totincl)>((fax1x2)?DINCLF:DINCL))
  nIncline=totincl;
 }

void skew_end()
 {

 row_prev=row;
 incline_prev=nIncline;
 }

void ideal_rc(cell *c)
 {
 c->row=c->r_row-(INT)((LONG)nIncline*c->r_col/2048);
 c->col=c->r_col+(INT)((LONG)nIncline*c->r_row/2048);
 }
