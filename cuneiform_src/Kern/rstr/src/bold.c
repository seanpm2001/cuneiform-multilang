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
/*								    */
/*			       Bolds program                        */
/*								    */
/********************************************************************/
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "cuthdr.h"
#include "func.h"

cell *SHAVE_CELL;
CHAR *SHAVE_RASTER;
CHAR *SHAVE_FLAGS;
INT SHAVE_HEIGHT;
INT SHAVE_WIDTH ;

BOOL boldshave(cell *C,INT method)
//
//  This procedure shaves skin from image in cell *C.
//
 {
 MN *mn;
 cell *D;
 INT bd; CHAR df;
 BYTE sv[sizeof(D->nvers)+sizeof(D->vers)];
 BYTE svf;
// cell c;

 Z=&string;
 // memcpy (&c,C,sizeof(c));
 memcpy (sv,&C->nvers,sizeof(C->nvers)+sizeof(C->vers));
 svf = (BYTE)C->flg;
 SHAVE_CELL=C;
 SHAVE_RASTER=save_raster(C);
 SHAVE_FLAGS=t_raster();
 memset(SHAVE_FLAGS,0,1024);
 shaving(method);
 mn=c_locomp(SHAVE_RASTER,(INT)((C->w+7)/8),C->h,C->r_row,C->r_col);
 if (!mn) return 0;
 D=C->prev;
 bd=C->bdiff; df = C->difflg & 0xf0;
 del_cell(C);
 C=create_cell(mn,D,(CHAR)bd, df);
 // memcpy (C,&c,sizeof(c));
 memcpy (&C->nvers,sv,sizeof(C->nvers)+sizeof(C->vers));
 C->flg = svf;
 adjust_links(C);
 return 1;
 }

static BYTE mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static void rotate_coords(INT *row,INT *col,INT direct,INT H)
{
INT t;

if( !direct )
  { // rotate coords
  t   = *row;
  *row = H-*col-1;
  *col = t;
  }
return;
}

static void clear_pixel(INT H,INT BW,INT row,INT col,INT direct)
//
//  This procedure clear one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

SHAVE_RASTER[ BW*row + (col>>3)] &= ~mask[ col&7 ];
return;
}

static void set_pixel(INT H,INT BW,INT row,INT col,INT direct)
//
//  This procedure set one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

SHAVE_RASTER[ BW*row + (col>>3)] |= mask[ col&7 ];
return;
}


static BYTE read_pixel(INT H,INT BW,INT row,INT col,INT direct)
//
//  This procedure read one pixel in raster
//
{
rotate_coords(&row,&col,direct,H);

if( row<0 || row>=SHAVE_HEIGHT )  return 0;
if( col<0 || col>=SHAVE_WIDTH  )  return 0;
return( (SHAVE_RASTER[ BW*row + (col>>3)] & mask[ col&7 ])!=0);
}

static INT enable_shl(INT ch,INT cw,INT row,INT col)
{
return( !read_pixel(ch,cw,(INT)(row-1),col,1) && !read_pixel(ch,cw,(INT)(row+1),col,1) );
}

static INT enable_shr(INT ch,INT cw,INT row,INT col)
{
return( !read_pixel(ch,cw,(INT)(row-1),col,1) && !read_pixel(ch,cw,(INT)(row+1),col,1) );
}

static INT enable_deleting_pimple(INT ch,INT cw,INT row,INT col,INT len,INT d)
{
INT i;
BYTE p;

for(p=0,i=0;i<len;i++)
      {
      p <<= 1;
      p += read_pixel(ch,cw,row,(INT)(col+i),d);
      }
return len==3 ? (p!=5) : ( p!=9 && p!=11 && p!=13 ) ;
}

static INT disable_deleting_pimple1(INT ch,INT cw,INT row,INT e,INT d)
{
if( row<0 || row>=SHAVE_HEIGHT )  return 0;
// 0xx0    0xx0  for len==1
// 00pe    0pe0
if( !read_pixel(ch,cw,row,(INT)(e-3),d) && !read_pixel(ch,cw,row,e,d) &&
      (read_pixel(ch,cw,row,(INT)(e-2),d)||read_pixel(ch,cw,row,(INT)(e-1),d))  )
  return 1;
if( !read_pixel(ch,cw,row,(INT)(e-2),d) && !read_pixel(ch,cw,row,(INT)(e+1),d) &&
      (read_pixel(ch,cw,row,(INT)(e-1),d)||read_pixel(ch,cw,row,e,d))   )
  return 1;

return 0;
}

static INT disable_deleting_pimple2(INT ch,INT cw,INT row, INT e, INT d)
{
if( row<0 || row>=SHAVE_HEIGHT )  return 0;
// 0xxx0   0xxx0  0xxx0   0xxx0 for len==2
// 0ppe0   00ppe  000ppe  ppe00
if( !read_pixel(ch,cw,row,(INT)(e-3),d) && !read_pixel(ch,cw,row,(INT)(e+1),d) &&
      (read_pixel(ch,cw,row,(INT)(e-2),d)||
       read_pixel(ch,cw,row,(INT)(e-1),d)||
       read_pixel(ch,cw,row,e,d))  )
  return 1;
if( !read_pixel(ch,cw,row,(INT)(e-4),d) && !read_pixel(ch,cw,row,e,d) &&
      (read_pixel(ch,cw,row,(INT)(e-3),d)||
       read_pixel(ch,cw,row,(INT)(e-2),d)||
       read_pixel(ch,cw,row,(INT)(e-1),d))   )
  return 1;

if( !read_pixel(ch,cw,row,(INT)(e-5),d) && !read_pixel(ch,cw,row,(INT)(e-1),d) &&
      (read_pixel(ch,cw,row,(INT)(e-4),d)||
       read_pixel(ch,cw,row,(INT)(e-3),d)||
       read_pixel(ch,cw,row,(INT)(e-2),d))   )
  return 1;
if( !read_pixel(ch,cw,row,(INT)(e-2),d) && !read_pixel(ch,cw,row,(INT)(e-2),d) &&
      (read_pixel(ch,cw,row,(INT)(e-1),d)||
       read_pixel(ch,cw,row,e,d)||
       read_pixel(ch,cw,row,(INT)(e-1),d))   )
  return 1;

return 0;
}

static void pimples_deleting_one_line(lnhead *line,INT cw,INT ch,INT direct)
//
//  This procedure deletes pimples from one line in work raster
//
{
INT      num_row, i, h;
interval *inter;

h=line->h;  i=0;  num_row = line->row;
inter=(interval *)((PCHAR)line+sizeof(lnhead));
for( ; h ; h--,inter++,i++,num_row++)
  if( (i==0 && (line->flg&l_fbeg))  || (h==1 && (line->flg&l_fend)) )
    {
    if( inter->l>2 )  continue;
    switch( inter->l )
      {
      case 1 :
      if( enable_deleting_pimple(ch,cw,(INT)(num_row+1),(INT)(inter->e-2),3,direct) &&
          enable_deleting_pimple(ch,cw,(INT)(num_row-1),(INT)(inter->e-2),3,direct) &&
          !disable_deleting_pimple1(ch,cw,(INT)(num_row+1),inter->e,direct)  &&
          !disable_deleting_pimple1(ch,cw,(INT)(num_row-1),inter->e,direct) )
          clear_pixel(ch,cw,num_row,(INT)(inter->e-1),direct);
        break;
      case 2 :
      if( enable_deleting_pimple(ch,cw,(INT)(num_row+1),(INT)(inter->e-3),4,direct) &&
          enable_deleting_pimple(ch,cw,(INT)(num_row-1),(INT)(inter->e-3),4,direct) &&
          !disable_deleting_pimple2(ch,cw,(INT)(num_row+1),inter->e,direct)  &&
          !disable_deleting_pimple2(ch,cw,(INT)(num_row-1),inter->e,direct) )
          {
          clear_pixel(ch,cw,num_row,(INT)(inter->e-1),direct);
          clear_pixel(ch,cw,num_row,(INT)(inter->e-2),direct);
          }
        break;
      }
    }

return ;
}


static void pimples_deleting(cell *c,INT direct)
//
//  This procedure deletes pimples from image in work raster
//
{
 lnhead   *line;
 INT      ll;
 INT      cw=direct?((c->w+7)/8):((c->h+7)/8),ch=c->w;

for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
        pimples_deleting_one_line(line,cw,ch,direct);

return;
}

static void jumps_deleting_one_line(lnhead *line,INT cw,INT ch)
//
//  This procedure deletes pimples from one line in work raster
//
{
INT      num_row, i, h;
interval *inter;
INT      pb,pe, b,e, nb,ne;

h=line->h;
if( h<3 ) return;
i=0;  num_row = line->row;
inter=(interval *)((PCHAR)line+sizeof(lnhead));

inter++; num_row++;
i++, h-=2; // skip first, last intervals on the line
for( ; h ; h--,inter++,i++,num_row++)
  {
  pe = inter[-1].e; pb = pe - inter[-1].l;
  e  = inter->e;     b = e  - inter->l;
  if( e<pb-1 )

    {  // clear defect of component
    b += pb-1-e;
    inter->e = b + inter->l;
    }
  if( b>pe )
    {  // clear defect of component
    b = pe;
    inter->e = b + inter->l;
    }
  ne = inter[1].e;  nb = ne - inter[1].l;

  if( pe==ne && e==ne+1 && b>pb && b>nb ) // right pimples, left hole
  if( enable_shl(ch,cw,num_row,e) )
    {
    clear_pixel(ch,cw,num_row,(INT)(e-1),1);
    set_pixel  (ch,cw,num_row,(INT)(b-1),1);
    }
  if( pb==nb && b==nb-1 && e<pe && e<ne ) // right pimples, left hole
  if( enable_shr(ch,cw,num_row,(INT)(b-1)) )
    {
    clear_pixel(ch,cw,num_row,b,1);
    set_pixel  (ch,cw,num_row,e,1);
    }
  if( pe==ne && pb==nb )
    {                      // equ intervals
    if( b==nb && e==ne-1 ) // right hole
      set_pixel  (ch,cw,num_row,e,1);
    if( e==ne && b==nb+1 ) // left hole
      set_pixel  (ch,cw,num_row,(INT)(b-1),1);
    }
  if( nb==pb && b==nb+1 && ( e-b==ne-nb || e-b==pe-pb ) )
    set_pixel  (ch,cw,num_row,(INT)(b-1),1);  // right hole
  if( ne==pe && e==ne-1 && ( e-b==ne-nb || e-b==pe-pb ) )
    set_pixel  (ch,cw,num_row,e,1);// left hole
  }

return ;
}

static void jumps_deleting(cell *c)
//
//  This procedure deletes pimples from image in work raster
//
{
 lnhead   *line;
 INT      ll;
 INT      cw=((c->w+7)/8),ch=c->w;

for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
        jumps_deleting_one_line(line,cw,ch);

return;
}

BOOL pimpleshave(cell *C, INT shave, INT incline)
//
//  This procedure deletes pimples from image in cell *C.
//
 {
 MN   *mn;
 cell *CC,*WW;
 INT   bd;
 CHAR df;
 BYTE  svf;

 Z=&string;
 svf = (BYTE)C->flg;
 // BEFORE SHAVE

 SHAVE_RASTER=save_raster(C);
 SHAVE_HEIGHT=C->h;
 SHAVE_WIDTH =C->w;
 if( incline )
  jumps_deleting(C);
bd=C->bdiff; df = C->difflg & 0xf0;
if( shave )
 {
 pimples_deleting(C,1); // modify SHAVE_RASTER
 mn=c_locomp(SHAVE_RASTER,(INT)((C->w+7)/8),C->h,C->r_row,C->r_col);
 if (!mn) return 0;

 WW=create_cell(mn,C,(CHAR)bd, df);
 // FIRST SHAVING

 SHAVE_RASTER=save_raster(WW);
 SHAVE_HEIGHT=WW->h;
 SHAVE_WIDTH =WW->w;
 c_rastror(SHAVE_RASTER,t_raster(),WW->w,WW->h);
			      // turn s_raster for 90 degrees clockwise

 if (WW->h%8 == 0)
  t_left_shift=0;
 else
  t_left_shift=8-WW->h%8;
 t_height=WW->w;
 t_width_b = (WW->h+7)/8;
 mn=c_locomp(t_raster(),t_width_b,t_height,0,(INT)(-t_left_shift));
			      // extraction components from t_raster
 if (!mn) {del_cell(WW);return 0;}
 CC=create_cell(mn,WW,(CHAR)bd, df);
 pimples_deleting(CC,0); // modify SHAVE_RASTER
 del_cell(CC);
 mn=c_locomp(SHAVE_RASTER,(INT)((WW->w+7)/8),WW->h,WW->r_row,WW->r_col);
 del_cell(WW);
 if (!mn) return 0;
 WW=create_cell(mn,C,(CHAR)bd, df);
 if( C->flg!=c_f_dust && WW->w<MAX_DUST_WIDTH && WW->h<MAX_DUST_HEIGHT )
  return 0;
 // SECOND SHAVING
 }
 else
  { // without SHAVING : correct rotate defectes only
  mn=c_locomp(SHAVE_RASTER,(INT)((C->w+7)/8),C->h,C->r_row,C->r_col);
  if (!mn)
    return 0;
  WW=create_cell(mn,C,(CHAR)bd, df);
  if( C->flg!=c_f_dust && WW->w<MAX_DUST_WIDTH && WW->h<MAX_DUST_HEIGHT )
    return 0;
  }
 C->h       = WW->h    ;
 C->w       = WW->w    ;
 C->row     = WW->row  ;
 C->r_row   = WW->r_row;
 C->r_col   = WW->r_col;
 C->col     = WW->col  ;
 C->env     = WW->env  ;
 del_cell(WW);
 // AFTER SHAVE

 return 1;
 }




