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
#include <stdlib.h>

#include "nt_types.h"
#include "struct.h"
#include "cstr.h"
//#include "cpage.h"
#include "func.h"
#include "linutil.h"
#include "fon.h"
#include "fondefs.h"
#include "ctbdefs.h"
#include "wrgb.h"
#include "dpuma.h"
#include "ligas.h"
#include "status.h"
#include "minmax.h"

#define INCL_FAC 2048

extern Handle hSnapSerifTrace;
extern INT    erection_inc;
extern BYTE   p2_active;

Bool32 p2_Line2Raster(c_comp *comp, RecRaster *rec);

static WORD tabserif[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        0,0x00C0,0,0,0x000C,0,0,0,0,0,0,0,0,0,0,0,0,                      // 48-64
//      0      1 2 3      4 5 6 7 8 9 : ; < = > ? @
        0x0033,0x2020,0,0x2020,0x2020,0x2030,0,0x3333,0xC0C0,0xC000,0x3030,0x3030,0x2133,0x2331,0,0x2030,0,0x1030,0,0x000C,0,0x3300,0x0300,0,0x000C,0,    //65-90
//      A      B      C      D      E      F G      H      I      J      K      L      M      N O      P Q      R S      T U      V      W X      Y Z
        0,0,0,0,0,0,                                            //91-96
//      [ \ ] ^ _ `
        0,0,0,0x0001,0,0x00C0,0,0x0033,0x00C0,0,0x0033,0x00C0,0x003F,0x0033,0,0x2030,0x0103,0x00C0,0,0,0x2200,0x3300,0x3300,0,0x3300,0,    // 97-122
//      a b c      d e      f g      h      i j      k      l      m      n o      p      q      r s t      u      v      w x      y z
        0,0,0,0,0,0x0033,0x2020,0x2020,0x2030,0x2100,0x2020,0x0C0C,0,0x3123,0x0023,0x3030,0x0103,0x2133,0x3333,0,0x2133,0x2030,0,0x000C,0x3300,0x0C0C,0x0333,0x3300,//123-150
//      { | } ~       �      �      �      �      �      �      � �      �      �      �      �      �      � �      �      � �      �      �      �      �      �
        0x3303,0x3F21,0x3F00,0x1020,0x3323,0x3020,0,0x3030,0x0103,0,0,0x2020,0x2030,0x2100,0,0x0C0C,0,0x3123,0x0023,0x3030,0x0103,0x2133,0x3333,0,0x2133,0,    //151-176
//           �      �      �      �      �      � �      �      � � �      �      �      � �      � �      �      �      �      �      �      � �      � �
        0x0033,0x0033,0x0033,0x003F,0x0030,0x0033,0,0,0x003F,0x00C0,0,0,0,                      // 177-189
//          fi     fl     ff    ffi     rt     ri % %    ffl      i j ! ?
//      �  �  �  �   �  � � �  �  � � � �
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � �
        0x2030,0,0x000C,0x3300,0x0C0C,0x3333,0x3300,0x3303,0x3F21,0x3F00,0x1020,0x3323,0x3020,0,0x3030,0x0103,                        // 224 - 239
//           � �      �      �      �      �      �      �      �      �      �      �      � �      �      �
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                       // 240 - 255
//      � � � � � � � � � � � � � � �

static WORD tabvserif[256]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        //  0-23
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 24-47
//                        ! " # $ % & ' ( ) * + , - . /
        0,0,0,0,0,0,0,0x0200,0,0,0,0,0,0,0,0,0,                      // 48-64
//      0 1 2 3 4 5 6      7 8 9 : ; < = > ? @
        0,0,0,0,0x0012,0x0002,0,0,0,0,0,0x0010,0,0,0,0,0,0,0,0x0202,0,0,0,0,0,0,    //65-90
//      A B C D      E      F G H I J K      L M N O P Q R S      T U V W X Y Z
        0,0,0,0,0,0,                                            //91-96
//      [ \ ] ^ _ `
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 97-122
//      a b c d e f g h i j k l m n o p q r s t u v w x y z
        0,0,0,0,0,0,0x0002,0,0x0002,0,0x0012,0,0,0,0,0,0,0,0,0,0,0,0,0x0202,0,0,0,0,//123-150
//      { | } ~  �      � �      � �      � � � � � � � � � � � � �      � � � � �
        0,0,0,0,0,0,0,0,0,0,0,0,0x0002,0,0,0,0,0,0,0,0,0,0,0,0,0,    //151-176
//      � � � � � � � � � � � �      � � � � � � � � � � � � � �
        0,  0, 0,  0, 0, 0,0,0,  0,0,0,0,0,                      // 177-189
//      fi fl ff ffi rt ri % % ffl i j ! ?
//      �  �  �  �   �  � � �  �  � � � �
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 190 - 223
//      � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � �
        0,0,0x0202,0,0,0,0,0,0,0,0,0,0,0,0,0,                        // 224 - 239
//      � �      � � � � � � � � � � � � � �
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                       // 240 - 255
//      � � � � � � � � � � � � � � �
static Word32 key=2;                  //��� snap'�

static cell *serif_word(cell *c);
static LONG new_serif(cell *c);
static LONG fon_test(cell *c);
static void find_serif(cell *c, WORD map, LONG *meas, LONG *np, LONG *nm);
static LONG downserif(c_comp *env, BYTE shape, INT H, STICK *st);
static LONG upserif(c_comp *env, BYTE shape, INT H, STICK *st);
static interval *interval_fit(INT i, lnhead *line, INT H, STICK *st);
static void ideal2rc(Point16 *p);
static void bound_cell(cell *c, Word32 color);

void serif_let()
{
  cell *c=cell_f()->next;
  while (c=serif_word(c));
//  LDPUMA_DeleteRects(NULL, key);
}

static cell *serif_word(cell *c)
{
  LONG np=0,nm=0,meas=0;
  LONG serif=0;
  BOOL reliable;
//  LONG tol;
  cell *beg=c,*end;
  B_LINES my_bases; //������ �����

  if (!c->next)
    return NULL;
  get_b_lines(NULL,&my_bases);
//  tol=my_bases.ps/16+1;

  do
  {
    if (snap_activity('k'))
    {
      snap_newcell(c);
      snap_monitor();
    }

    if (c->flg & (c_f_let | c_f_bad))
    {
      BYTE let;
      c->font &= ~(c_fp_gelv | c_fp_ser);
      if (!(c->font & c_fp_it) || is_upper(let=c->vers[0].let) ||
			strchr("���",let) &&
			!is_russian_baltic_conflict(let)&&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
		 )
      {
        LONG m=new_serif(c);
        if (m>0)  np++;
        else
        if (m<0)  nm++;
        if (m != 0)
        {
          if (m>0)  c->font |= c_fp_ser;
          else      c->font |= c_fp_gelv;
          if (!LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
            bound_cell(c,(m>0) ? wRGB(255,0,0) : wRGB(0,255,0));
        }
//        if (!(c->font&c_fp_it))
        meas += m;
      }
    }
    c=c->next;
  }
  while (!(c->flg & c_f_fict));
//  while (!(c->flg & (c_f_space | c_f_fict)));
  end=c;

  serif=meas;  reliable = MAX(np,nm) >= 2*MIN(np,nm);

  if (!reliable && p2_active)  //let's use database
  {
    c=beg;  meas=np=nm=0;
    do
    {
      if (snap_activity('k'))
      {
        snap_newcell(c);
        snap_monitor();
      }

      if (c->flg & (c_f_let | c_f_bad))
      {
        BYTE let;
        if (!(c->font & c_fp_it) || is_upper(let=c->vers[0].let) ||
				strchr("���",let) &&
					!is_russian_baltic_conflict(let)&&// 17.07.2001 E.P.
					!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
		   )
        {
          LONG m=fon_test(c);
          if (m==0)            //no match found, use old flags
          {
            if (c->font & c_fp_ser)  m++;
            if (c->font & c_fp_gelv) m--;
          }
          if (m>0)  { np++; m = 1; }
          else
          if (m<0)  { nm++; m =-1; }
          if (m != 0 && !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
            bound_cell(c,(m>0) ? wRGB(255,0,0) : wRGB(0,255,0));
  //        if (!(c->font&c_fp_it))
            meas += m;
        }
      }
      c=c->next;
    }
    while (!(c->flg & c_f_fict));
    serif=meas;  reliable = MAX(np,nm) >= 2*MIN(np,nm);
  }

  if (serif != 0)
  {
    BYTE font;
    if (reliable)  font = (serif>0) ? c_fp_ser : c_fp_gelv;
    else           font = c_fp_ser | c_fp_gelv;

    for (c=beg; c!=end; c=c->next)
//      if (c->flg & (c_f_let | c_f_bad) && !(c->font & c_fp_it))
      if (c->flg & (c_f_let | c_f_bad))
      {
        c->font &= ~(c_fp_gelv | c_fp_ser);
        c->font |= font;
      }

    if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
    {
      cell *last=end->prev;
      Rect16 box;
      Point16 pa={beg->col,my_bases.b1-1},pb={last->col+last->w,my_bases.b4+1};
      ideal2rc(&pa);   ideal2rc(&pb);
      box.left=pa.x;  box.top=pa.y;
      box.right=pb.x; box.bottom=pb.y;
      LDPUMA_DrawRect(NULL,&box,0/*nIncline*/,(serif>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,key);
    }
  }

  return c;
}

static LONG new_serif(cell *c)
{
  BYTE let=let_sans_acc[c->vers[0].let];
  WORD map=tabserif[let];
  LONG meas=0,np=0,nm=0;

  if (map==0)
    return 0;

  find_serif(c,map,&meas,&np,&nm);

  if (map=tabvserif[let])
  {
    BYTE *rast=save_raster(c);
    INT t_height=c->w,t_width_b = (c->h+7)/8;
    MN   *mn;

    c_rastror(rast,t_raster(),c->w,c->h);
    mn=c_locomp(t_raster(),t_width_b,t_height,c->r_row,c->r_col);
    if (mn)
    {
      cell *CC=create_cell(mn,c,0,0);
      if (snap_activity('k'))
      {
        snap_newcell(CC);
        snap_monitor();
      }
      find_serif(CC,map,&meas,&np,&nm);
      del_cell(CC);
    }
  }

  if ( (np != 0)^(nm != 0) )   //no ambiguity
    return meas;

  if (abs(meas)<2)  meas=0;
  if (meas==0 && p2_active)  meas=fon_test(c);
  return meas;
/*
  else
  {
    LONG nmin=(np,nm),nmax=MAX(np,nm);
    if (nmin>1 || nmax-nmin<2)
      return 0;

    meas= (meas+nfoot/2)/nfoot;
    return (abs(meas) > c->h/8) ? meas : 0;
  }
*/
}

static LONG fon_test(cell *c)
{
  FonTestInfo testInfo[MAXCHECKALT];
  RecRaster recRast;
  BYTE let=let_sans_acc[c->vers[0].let];
  WORD map=tabserif[let];
  LONG i,nitem=0,serific,nbit=8*sizeof(map);

  if (map==0)
    return 0;

  if ( !c->env )
    return 0;

  // Nick 20.9.99
  // ��� �������� �������� ?
  if( c->clu_attr )
      testInfo[0].flag = (BYTE)c->clu_attr;
  else
  {
    p2_Line2Raster(c->env,&recRast);
#ifdef _USE_FON_
    if (!FONTestChar(&recRast,let,testInfo,0))
#endif
      return 0;

    c->clu_attr =  testInfo[0].flag;
  }

  if (testInfo[0].flag & CTB_PRINT_SERIFIC)  serific=1;
  else
  if (testInfo[0].flag & CTB_PRINT_GELV)  serific=-1;
  else                                    serific= 0;

  if (serific != 0)
    for (i=0; i<nbit; i++,map=map>>1)
      nitem += map&1;

  return (serific>=0) ? nitem : -nitem;
}

static void find_serif(cell *c, WORD map, LONG *meas, LONG *np, LONG *nm)
{
  STICK *st,*sti;
  INT i,nstick;
  BYTE shape;
  INT w0=c->w,dw=0,w3,w23;  //w0 - width of untilted letter

  if (c->n_baton==255)
    nstick=sticks_in_letter(c,0,&st);
  else
  {
    nstick=c->n_baton;  st=c->save_baton;
  }

  if (nstick<=0)
    return;

  if (c->font & c_fp_it)
  {
    INT stick_inc=c->stick_inc;
    if (stick_inc==NO_INCLINE)
      if (erection_inc != 0)
        stick_inc=erection_inc;
      else
      {
        for (i=stick_inc=0,sti=st; i<nstick; i++,sti++)
          stick_inc += sti->incl;
        stick_inc /= nstick;
      }
    dw  = stick_inc*c->h/INCL_FAC;
    w0 -= dw;
  }
  w3=w0*2/5,w23=2*w0/3;

  shape=map&3;
  if (shape)  //right-down
  {
    sti=st+nstick-1;
    if (sti->y<=2 && w23 < sti->x && sti->x < c->w)
    {
      LONG m=downserif(c->env,shape,c->h,sti);
      if (m>0)  (*np)++;
      else
      if (m<0)  (*nm)++;
      *meas += m;
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //middle-down
  {
    for (i=0,sti=st; i<nstick; i++,sti++)
      if (sti->y<=2 && w3 <= sti->x && sti->x < w23)
      {
        LONG m=downserif(c->env,shape,c->h,sti);
        if (m>0)  (*np)++;
        else
        if (m<0)  (*nm)++;
        *meas += m;
        break;
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //left-down
  {
    sti=st;
    if (sti->y<=2 && sti->x < w3)
    {
      LONG m=downserif(c->env,shape,c->h,sti);
      if (m>0)  (*np)++;
      else
      if (m<0)  (*nm)++;
      *meas += m;
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape && nstick==1)  //stick like
  {
    LONG m=downserif(c->env,shape,c->h,st);
    if (m>0)  (*np)++;
    else
    if (m<0)  (*nm)++;
    *meas += m;
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //right-up
  {
    sti=st+nstick-1;
    if (sti->y+sti->l+2>=c->h)
    {
      INT x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
      if (w23 < x && x < c->w)
      {
        LONG m=upserif(c->env,shape,c->h,sti);
        if (m>0)  (*np)++;
        else
        if (m<0)  (*nm)++;
        *meas += m;
      }
    }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //middle-up
  {
    for (i=0,sti=st; i<nstick; i++,sti++)
      if (sti->y+sti->l+2>=c->h)
      {
        INT x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
        if (w3 <= x && x < w23)
        {
          LONG m=upserif(c->env,shape,c->h,sti);
          if (m>0)  (*np)++;
          else
          if (m<0)  (*nm)++;
          *meas += m;
          break;
        }
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape)  //left-up
  {
    for (i=0,sti=st; i<MIN(2,nstick); i++,sti++)   //in russian 'i' left stick maybe not first
      if (sti->y+sti->l+2>=c->h)
      {
        INT x=sti->x+sti->l*sti->incl/INCL_FAC-dw;
        if (x < w3)
        {
          LONG m=upserif(c->env,shape,c->h,sti);
          if (m>0)  (*np)++;
          else
          if (m<0)  (*nm)++;
          *meas += m;
          break;
        }
      }
  }

  map >>= 2;
  shape=map&3;
  if (shape && nstick==1)  //stick like
  {
    LONG m=upserif(c->env,shape,c->h,st);
    if (m>0)  (*np)++;
    else
    if (m<0)  (*nm)++;
    *meas += m;
  }
}

static LONG downserif(c_comp *env, BYTE shape, INT H, STICK *st)
{
  INT h,H8=H/8,H3=H/3;
  lnhead *line;
  LONG rv=0;

  for (line=(lnhead *)((PCHAR)(env)+env->lines+sizeof(INT));
			 line->lth>0;
       line=(lnhead *)((PCHAR)line+line->lth))
    if (line->flg&l_fend && (h=line->h) > H3 && line->row+h+2 >= H)
    {
      INT x1,x2,i,i0=h-H3-1,in,begl=0,begr=0;
      LONG lsum=0,rsum=0,imaxl=0,vmaxl=0,imaxr=0,vmaxr=0;
      interval *intv;

      i0=MAX(i0,0);
      i=h-H8-1;              //from line top
      if (!(intv=interval_fit(i,line,H,st)))
        continue;             //line not corresponds to stick

      //to stick base width
      for ( ;  i>=i0 && intv->l != st->w;  i--,intv--);
      if (i<i0)
        continue;             //strange width

      if (!interval_fit(i,line,H,st))
        continue;             //line not corresponds to stick

      x2=intv->e;  x1=x2-intv->l;
      in=h-i;
      for (i=0;  i<in;  i++,intv++)
//        if (intv->l<=w2 || intv->e<x1 || intv->e-intv->l>=x2)
//          break;              //a tump on the line end
//        else
        {
          INT dx=(i*st->incl+INCL_FAC/2)/INCL_FAC,s;
          if (shape&2)
          {
            s=x1-(intv->e-intv->l+dx);
            if (s>=vmaxl)  { vmaxl=s;  imaxl=i; }
            if (s>0)
            {
              lsum += s;
              if (begl==0)  begl=i;
            }
          }
          if (shape&1)
          {
            s=intv->e+dx-x2;
            if (s>vmaxr)  { vmaxr=s;  imaxr=i; }
            if (s>0)
            {
              rsum += s;
              if (begr==0)  begr=i;
            }
          }
        }

      if (shape&2)
        if (vmaxl<1)  rv -= 2;
        else
        if (vmaxl>0 && st->incl==0 || vmaxl>1)
//        if (vmaxl>0)
        {
          if (i-begl<=st->w)
          {
            rv++;
            if (vmaxl>=i-imaxl-1 || lsum>=(H+8)/16+1) rv++;
          }
        }

      if (shape&1)
        if (vmaxr<1)  rv -= 2;
        else
        if (vmaxr>0 && st->incl==0 || vmaxr>1)
//        if (vmaxr>0)
        {
          if (i-begr<=st->w)
          {
            rv++;
            if (vmaxr>=i-imaxr-1 || rsum>=(H+8)/16+1) rv++;
          }
        }

      if (rv==1 && lsum+rsum >= (H+8)/16+1)  rv=2;
      rv /= 2;

//      rv = lsum+rsum;
//      if (shape>2)  rv /= 2;
//      rv -= H/16+1;

      if (rv != 0)
        if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
        {
          Point16 pa={env->left+st->x,env->upper+env->h-st->y},pb={pa.x+st->incl*H/4/INCL_FAC,pa.y-H/4};
          LDPUMA_DrawLine(NULL,&pa,&pb,0/*nIncline*/,(rv>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,1);
        }
      return rv;
    }
  return 0;
}

static LONG upserif(c_comp *env, BYTE shape, INT H, STICK *st)
{
  INT h,H8=H/8,H3=H/3;
  lnhead *line;
  LONG rv=0;

  for (line=(lnhead *)((PCHAR)(env)+env->lines+sizeof(INT));
			 line->lth>0;
       line=(lnhead *)((PCHAR)line+line->lth))
    if (line->flg&l_fbeg && (h=line->h) > H3 && line->row <= 2)
    {
      INT x1,x2,i,i0=MIN(H3,h),begl=0,begr=0;
      LONG lsum=0,rsum=0,imaxl=0,vmaxl=0,imaxr=0,vmaxr=0;
      interval *intv;

      i=H8+1;                //from line top
      if (!(intv=interval_fit(i,line,H,st)))
        continue;             //line not corresponds to stick

      //to stick base width
      for ( ;  i<=i0 && intv->l != st->w;  i++,intv++);
      if (i>i0)
        continue;             //strange width

      if (!interval_fit(i,line,H,st))
        continue;             //line not corresponds to stick

      x2=intv->e;  x1=x2-intv->l;
//      i0=env->h-st->y-st->l;        //in 'p' stick not begins on top
      i0=MAX(line->row,env->h-st->y-st->l);  //finish interval in serif search
      i0=i-i0;                               //number of intervals in serif search
      for (i=0;  i<=i0 && shape != 0;  i++,intv--)
//        if (intv->l<=w2 || intv->e<x1 || intv->e-intv->l>=x2)
//          break;               //a tump on the line begin
//        else
        {
          INT dx=(i*st->incl+INCL_FAC/2)/INCL_FAC,s;
          if (shape&2)
          {
            s=x1-(intv->e-intv->l-dx);
/*
            if (s>w3)  //top of russian 'p'
            {
              vmaxl=lsum=0;   shape &= 0xFD;
            }
            else
*/
            {
              if (s>=vmaxl)  { vmaxl=s;  imaxl=i; }
              if (s>0)
              {
                lsum += s;
                if (begl==0)  begl=i;
              }
            }
          }
          if (shape&1)
          {
            s=intv->e-dx-x2;
/*
            if (s>w3)  //top of russian 'p'
            {
              vmaxr=rsum=0;  shape &= 0xFE;
            }
            else
*/
            {
              if (s>vmaxr)  { vmaxr=s;  imaxr=i; }
              if (s>0)
              {
                rsum += s;
                if (begr==0)  begr=i;
              }
            }
          }
        }

      if (shape&2)
        if (vmaxl<1)  rv -= 2;
        else
        if (vmaxl>0 && st->incl==0 || vmaxl>1)
//        if (vmaxl>0)
        {
          if (i-begl<=st->w)
          {
            rv++;
            if (vmaxl>=i-imaxl-1 || lsum>=(H+8)/16+1) rv++;
          }
        }

      if (shape&1)
        if (vmaxr<1)  rv -= 2;
        else
        if (vmaxr>0 && st->incl==0 || vmaxr>1)
//        if (vmaxr>0)
        {
          if (i-begr<=st->w)
          {
            rv++;
            if (vmaxr>=i-imaxr-1 || rsum>=(H+8)/16+1) rv++;
          }
        }

      if (rv==1 && lsum+rsum >= (H+8)/16+1)  rv=2;
      rv /= 2;

//      rv = lsum+rsum;
//      if (shape>2)  rv /= 2;
//      rv -= H/16+1;

      if (rv != 0)
        if( !LDPUMA_SkipEx(hSnapSerifTrace,FALSE,TRUE,1))
        {
          Point16 pa={env->left+st->x+st->incl*st->l/INCL_FAC,env->upper+env->h-st->y-st->l};
          Point16 pb={pa.x-st->incl*H/4/INCL_FAC,pa.y+H/4};
          LDPUMA_DrawLine(NULL,&pa,&pb,0/*nIncline*/,(rv>0) ? wRGB(255,0,0) : wRGB(0,255,0),1,1);
        }
      return rv;
    }
  return 0;
}

static interval *interval_fit(INT i, lnhead *line, INT H, STICK *st)
//return pointer to i-th interval of the line if it corresponds to stick st;
{
  INT h0=H-(line->row+i);    //from raster bottom
  INT x0=st->x+(h0-st->y)*st->incl/INCL_FAC;
  interval *intv=(interval *)((PCHAR)line+sizeof(lnhead))+i;
  if (x0 > intv->e || x0 < intv->e-intv->l)  intv=NULL;
  return intv;
}

static void ideal2rc(Point16 *p)
{
  INT y=p->y;
  p->y=y+(INT)((LONG)nIncline*p->x/2048);
  p->x=p->x-(INT)((LONG)nIncline*y/2048);
}

static void bound_cell(cell *c, Word32 color)
{
  Rect16 box;
  Point16 pa={c->col,c->row},pb={c->col+c->w-1,c->row+c->h-1};
  ideal2rc(&pa);   ideal2rc(&pb);
  box.left=pa.x;  box.top=pa.y;
  box.right=pb.x; box.bottom=pb.y;
  LDPUMA_DrawRect(NULL,&box,0/*nIncline*/,color,1,key);
}
