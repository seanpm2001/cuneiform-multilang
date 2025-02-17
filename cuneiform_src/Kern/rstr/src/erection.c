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

/*

  ������������������������������������������������������������������
  ��                                                              ��
  ��                        ERECTION.C                            ��
  ��        finding and deleting incline of cell images           ��
  ��                                                              ��
  ������������������������������������������������������������������
*/

#define MAX_LEN_WORD  48
#define STEND_INC1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nt_types.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"	// 17.07.2001 E.P.

#include "compat_defs.h"

static BYTE solid_sticks[]     ="|1!Il\xbb"; // liga_i, liga_j // liga_i ����� 08.09.2000 E.P.
static BYTE incline_main[]     ="���⅍�������irbtfnBTEIPDFHLN"; // "���������������irbtfnBTEIPDFHLN"
static BYTE incline_chars[]    =
	"�正����뢯�������������������������\xf7\xf5ijbrtfnBTEIPDFHLNR";
//	"�����������������������������������������\xf7\xf5ijbrtfnBTEIPDFHLNR";
// cursive �,�
static BYTE aux_inc_chars[]    ="�ꥠ����ᬊ����ukdhqecmoKUOCM";
static BYTE disable_rerecog[] ="ᨬ��cb";
static BYTE incline_alias[]    ="�||";
static BYTE nonincline_alias[] ="6)>";
static BYTE incline_alias2[]   ="��";
static BYTE nonincline_alias2[]="��";
static INT  local_pass;
static INT  hist[256], num_extr, max_incline, line_incline;
static BYTE extr[4];

  extern BYTE db_status       ;    // snap presence byte
  extern BYTE langSer, langUkr;
  extern BYTE db_trace_flag   ;    // snap-detail presence byte
  extern BYTE db_pass         ;    // snap-pass indicator
  extern INT  nIncline        ;    // global page incline
  extern INT  pitchsize       ;    // string pitch
  extern BYTE erection_enable ;
  extern BYTE    decode_ASCII_to_[256][4];
  extern BYTE line_scale, p2_active;

  extern INT LeftDistance(BYTE *RASTER, INT NWIDTH);
  extern INT   line_number   ;


#define bytlen(bit) (((bit)+7)>>3)
INT    erection_inc=0;
static INT  erect_page_incline[2048/16],erect_page_incline_mode;
static INT  erect_page_num_represent, erect_page_words=0;
static BOOL erect_solid_stick(cell *c);
static INT  erection_incline_word_set(cell *b, cell *e);
static BOOL add_versions(cell *c, version *save_versions, INT save_nvers);
static BOOL no_bad_alias(cell *c);
static cell * convert_to_cells(cell *c);
static BOOL mode_incline( INT inc);
static LONG calc_dens(cell *c);
static INT  get_incline_of_word(cell *b, cell *e);
static INT  erection_incline_word(cell *b, cell *e, INT base_3,INT n_call);
static void shift_word( cell *c, cell *e, INT shift );
static cell * erect_end_word(cell *cs,BYTE *str,BYTE *word_len,INT limit);
static cell * erect_next_word(cell *cs);
static BOOL setup_incline_word(cell *b, cell *e, BOOL t);
static void clear_incline_word(cell *b, cell *e);
static INT  fict_shift_intervals(cell *c, INT tab_angle[] );
static INT  fict_shift_left_intervals(cell *c, INT tab_angle[] );
static INT  represent_inc(INT inc_list[],INT n, INT delta);
static INT  num_inc_in_list(INT inc_list[],INT n);
static void average_inc(INT inc_list[],INT n, INT *rint, INT *rnum);
static INT  select_inc(INT inc_list[],INT n, INT delta, INT *rinc, INT *rnum);
static INT diff_left_limit_cell(cell *c, INT tab_angle[], INT init_max);
static INT diff_left_limit_one_line(lnhead *line, INT tab_angle[], INT init_max);
static INT diff_left_limit_rast(BYTE *raster, INT dx, INT dy, INT tab_angle[]);

void erect_init_global_tab(void)
{
memset(erect_page_incline,0,sizeof(erect_page_incline));
erect_page_num_represent=0;
erect_page_words=0;
return;
}

void erect_set_global(void)
{
INT i,s,n;
for(s=n=i=0;i<(512+128)/16;i++)
    {
    if( erect_page_incline[i] )
        {
        s += erect_page_incline[i]*i;
        n += erect_page_incline[i];
        }
    }
erect_page_num_represent=n;
if( n )
    erect_page_incline_mode = (16*s)/n;
else
    erect_page_incline_mode = 0;
return;
}

INT erect_get_global(void)
{
return erect_page_incline_mode;
}

INT erect_get_local(void)
{
return erection_inc;
}

INT erect_get_represent(void)
{
return erect_page_num_represent;
}

INT erect_get_words(void)
{
return erect_page_words;
}



//
// this function shaves all components belong multicell *c
//
void pimple_shaving_cell( cell *c )
{
erect_cell_value (c, 0, 1 , TRUE);
return;
}

//
// make ratating table for incline c->stick_inc.
// Return FALSE if can't make table, TRUE is table is OK
//
BOOL erect_make_tab(cell *c, INT     tab_angle[])
{
INT     inc = -c->stick_inc       ;
INT     dy=c->h                   ;
INT     i                         ;

memset(tab_angle, 0, dy*sizeof(INT) );
if( c->stick_inc==NO_INCLINE ) return FALSE; // no rotating
if( ((long)(dy-1)*abs(inc))<2048 || inc>0 )
  inc=0;
if( inc==0 )                   return TRUE; // zero skew
for (i=0; i<dy; i++)
  tab_angle[i] = ((long)(dy-1-i)*inc)/2048;
return TRUE; // normal skew
}

//
// calculate virtual density after restore incline (by c->stick_inc )
//
INT erect_original_dens (cell *c)
{
INT     inc = -c->stick_inc, cw       ;
INT     dy=c->h                       ;
cell    cc=*c                         ;
INT     i                             ;
INT     tab_angle[256]                ;

if( !c->env     )              return c->w;
if( !(c->pos_inc&erect_rot) )  return c->w;
if( c->stick_inc==NO_INCLINE ) return c->w;
if( ((long)(dy-1)*abs(inc))<2048 || inc>0 )
  inc=0;
if( inc==0 )                   return c->w;
for (i=0; i<dy; i++)
  tab_angle[i] = ((long)(dy-1-i)*inc)/2048;

if( inc<0 )
  {  //  rotate without shaving
  LONG oldw = c->w;
  cw = fict_shift_intervals(&cc, tab_angle);
  if( c->dens!=255 )
    c->dens = (c->dens*oldw)/cw;
  }

return c->dens;
}

INT erect_original_width (cell *c)
{
INT     inc = -c->stick_inc           ;
INT     dy=c->h                       ;
INT     i                             ;
INT     tab_angle[256]                ;

if( !c->env     )              return c->w;
if( !(c->pos_inc&erect_rot) )  return c->w;
if( c->stick_inc==NO_INCLINE ) return c->w;
if( ((long)(dy-1)*abs(inc))<2048 || inc>0 )
  inc=0;
if( inc==0 )                   return c->w;
for (i=0; i<dy; i++)
  tab_angle[i] = ((long)(dy-1-i)*inc)/2048;

if( inc<0 )
  {  //  rotate without shaving
  return fict_shift_intervals(c, tab_angle);
  }

return c->w;
}

//
// incline is VALUE, shave - enable shaving
// use for back rotating too
//
INT erect_raster_value (BYTE *raster, INT dx, INT dy, INT inc)
{
BYTE    sh_raster[1024] ;
INT     i               ;
INT     tab_angle[256]  ;

if( ((long)(dy-1)*abs(inc))<2048 )
  inc=0;
if( inc==0 )
  return dx;

for (i=0; i<dy; i++)
  tab_angle[i] = ((long)(dy-1-i)*inc)/2048;

dx = shift_raster (raster, dy, dx, tab_angle,
      (INT)(MAX (abs(tab_angle[0]), abs(tab_angle[dy-1]))), sh_raster, inc);
memcpy(raster,sh_raster,dy*((dx+7)>>3) );
return dx;
}

//
// incline is VALUE, shave - enable shaving
// use for back rotating too
//
cell * erect_cell_value (cell *c, INT inc, INT shave, BOOL cutting)
{
INT     dx=c->w, dy=c->h, le, ri, sinc=inc     ;
INT     lminx, lminsx, lminy, lminsy           ;
MN      *sh_mn                                 ;
cell    *sh_cell[MAX_CELLS_IN_LIST+1],res_cell ;
PBYTE   raster                                 ;
BYTE    sh_raster[1024*2]                      ;
INT     i, d_x, n                              ;
INT     tab_angle[256]                         ;
cell    *cret = c                              ;

if ( dy >= 256 )
	dy = 255;                                        //AK! add for crash steck protection
if( !c->env     )
	return c;

if( c->flg&c_f_dust)  shave = 0; // dust can't shave
ri = le = 0;
if( ((long)(dy-1)*abs(inc))<2048 )
  inc=0;
if( inc!=0 )
  for (i=0; ( i < dy ) && ( i < 256 ); i++)          //AK add c/g 06.03.98
    tab_angle[i] = ((long)(dy-1-i)*inc)/2048;
else
    memset(tab_angle,0,dy*2);

if( shave || inc )
  {
	// ���� ������� ��� ����� - ������
	// ����� �������� ����� � �.�.  Nick 07.04.2002
	if( (long)dy * ( (dx + (long)(MAX (abs(tab_angle[0]), abs(tab_angle[dy-1]))+7))>>3) >
		sizeof(sh_raster)
	  )
	  return c;

    raster = save_raster (c);

    le = diff_left_limit_rast(raster,dx,dy,tab_angle);

    if( inc<0 ) // inc<0 - clear rotating
      ri = diff_left_limit_cell(c, tab_angle, c->w);

    d_x = shift_raster (raster, dy, dx, tab_angle,
      (INT)(MAX (abs(tab_angle[0]), abs(tab_angle[dy-1]))), sh_raster, inc);

/*
	 if( line_number == 16 && c->col == 462)
	 {
		char qq[64];
		MessageBox(GetActiveWindow(),itoa(dx,qq,10),"old",MB_OK);
		MessageBox(GetActiveWindow(),itoa(dy,qq,10),"dy",MB_OK);
		MessageBox(GetActiveWindow(),itoa(inc,qq,10),"inc",MB_OK);
		MessageBox(GetActiveWindow(),itoa(d_x,qq,10),"new",MB_OK);
	 }
*/

  if( (sh_mn = c_locomp (sh_raster, (INT)bytlen(d_x), dy, 0, 0))==NULL )
    return c;

  for(i=0; sh_mn && i<MAX_CELLS_IN_LIST; i++, sh_mn = sh_mn->mnnext)
    {
    if( (sh_cell[i]=create_cell (sh_mn, c, c->bdiff, (CHAR)(c->difflg&0xf0)))==NULL )
      return c;
    if( sh_cell[i]->w>RASTER_MAX_WIDTH || sh_cell[i]->h>RASTER_MAX_HEIGHT ||
		!sh_cell[i]->env
	  )
      {
      for(;i>=0;i--)
        del_cell(sh_cell[i]);
      return c;
      }

    sh_cell[i]->stick_inc = NO_INCLINE; // rotate disabled
    }

  sh_cell[i] = NULL;
  n=i;

  for(lminx=lminy=c->w,i=0;i<n;i++)
    {
    if( lminx>sh_cell[i]->env->left )
      lminx=sh_cell[i]->env->left;
    if( lminy>sh_cell[i]->env->upper )
      lminy=sh_cell[i]->env->upper;
    }


  for(i=0;i<n;i++)
    pimpleshave( sh_cell[i], shave, inc );

  for(lminsx=lminsy=c->w,i=0;i<n;i++)
    {
    if( lminsx>sh_cell[i]->env->left )
      lminsx=sh_cell[i]->env->left;
    if( lminsy>sh_cell[i]->env->upper )
      lminsy=sh_cell[i]->env->upper;
    }

  memcpy( &res_cell, c , sizeof(cell));
  compose_cell(n, sh_cell, &res_cell);

  if( n<2 )
    c->cg_flag &= 0xFB         ; // clear compose flag for normal comp
  else
    c->cg_flag |= c_cg_comp    ; // set composed flag
  c->env        = res_cell.env ;
  c->env->upper = c->r_row     ;
  c->env->left  = c->r_col     ;
  if( n>1 || c->dens==255 )
    c->dens     = 255          ; // undef
  else
    {
    LONG dens   = calc_dens(c);
    c->dens     = (dens*32)/(res_cell.w*res_cell.h);
    }
  c->w          = res_cell.w   ;
  c->h          = res_cell.h   ;
  c->recsource  = 0            ; // artifact


  if( le<0 && c->col+le>=0 && c->r_col+le>=0 && c->env->left+le>=0 )
    {
    c->env->left += le;
    c->col       += le;
    c->r_col     += le;
    }
  if( ri>0 )
    {
    c->env->left += ri;
    c->col       += ri;
    c->r_col     += ri;
    }
  if( lminsx>lminx )
    {
    lminsx -= lminx;
    c->env->left += lminsx;
    c->col       += lminsx;
    c->r_col     += lminsx;
    }
  if( lminsy>lminy )
    {
    lminsy -= lminy;
    c->env->upper += lminsy;
    c->row        += lminsy;
    c->r_row      += lminsy;
    }
  if( (c->flg&(c_f_bad|c_f_let)) && c->w<MAX_DUST_WIDTH && c->h<MAX_DUST_HEIGHT )
    {
    cell *wc;
    c->flg   = c_f_dust;
    c->nvers = 0;
    c->vers[0].let=c->vers[0].prob=0;
    (wc=c->prevl)->nextl = c->nextl;
    c->nextl->prevl      = c->prevl;
    err_pnlet(c);
    }

  if( !(inc==0 && shave!=0) )    // not shaving without erection
    c->stick_inc  = inc;
  if( inc!=0 )
    c->pos_inc=erect_rot;

  if( n>1 && cutting)
    cret = convert_to_cells(c);

  }

// Nick 7.04.2002 c->cret - � ����� ���� ������ � convert_to_cells!
if( !inc &&  (cret->flg&c_f_dust) )
  {
  cret->stick_inc  = sinc;
  cret->pos_inc=erect_rot;
  }

return cret;
}


//
// incline is TABLE, shave - enable shaving
// not use for back rotating
//
cell * erect_cell_table (cell *c, INT tab_angle[], INT shave, BOOL cutting)
{
INT     dx=c->w, dy=c->h, le, ri               ;
INT     lminx, lminsx, lminy, lminsy           ;
MN      *sh_mn                                 ;
cell    *sh_cell[MAX_CELLS_IN_LIST+1],res_cell ;
PBYTE   raster                                 ;
BYTE    sh_raster[1024]                        ;
INT     i, d_x, n, inc                         ;
cell    *cret = c                              ;

if( !c->env     )              return c;
le = ri = 0;
for (i=0; i<dy; i++)
  tab_angle[i] >>= 2;
inc = (tab_angle[0] != tab_angle[dy-1]);
if( shave || inc)
  {
  raster = save_raster (c);
  le = diff_left_limit_rast(raster,dx,dy,tab_angle);
  d_x = shift_raster (raster, dy, dx, tab_angle,
        (INT)(MAX (tab_angle[0], tab_angle[dy-1])), sh_raster, 1);

  if( (sh_mn = c_locomp (sh_raster, (INT)bytlen(d_x), dy, 0, 0))==NULL )
    return NULL;

  for(i=0; sh_mn && i<MAX_CELLS_IN_LIST; i++, sh_mn = sh_mn->mnnext)
    {
    if( (sh_cell[i]=create_cell (sh_mn, c, c->bdiff, (CHAR)(c->difflg&0xf0)))==NULL )
      return NULL;
    sh_cell[i]->stick_inc = NO_INCLINE; // rotate disabled
    }
  sh_cell[i] = NULL;
  n = i;

  for(lminx=lminy=c->w,i=0;i<n;i++)
    {
    if( lminx>sh_cell[i]->env->left )
      lminx=sh_cell[i]->env->left;
    if( lminy>sh_cell[i]->env->upper )
      lminy=sh_cell[i]->env->upper;
    }

  for(i=0;i<n;i++)
    pimpleshave( sh_cell[i], shave, inc );

  for(lminsx=lminsy=c->w,i=0;i<n;i++)
    {
    if( lminsx>sh_cell[i]->env->left )
      lminsx=sh_cell[i]->env->left;
    if( lminsy>sh_cell[i]->env->upper )
      lminsy=sh_cell[i]->env->upper;
    }

  memcpy( &res_cell, c , sizeof(cell));
  compose_cell(n, sh_cell, &res_cell);

  if( n<2 )
    c->cg_flag &= 0xFB         ; // clear compose flag for normal comp
  else
    c->cg_flag |= c_cg_comp    ; // set composed flag
  c->env        = res_cell.env ;
  c->env->upper = c->r_row     ;
  c->env->left  = c->r_col     ;
  if( n>1 || c->dens==255 )
    c->dens     = 255          ; // undef
  else
    {
    LONG dens   = calc_dens(c);
    c->dens     = (dens*32)/(res_cell.w*res_cell.h);
    }
  c->w          = res_cell.w   ;
  c->h          = res_cell.h   ;
  c->recsource  = 0            ; // artifact
  if( lminsx>lminx )
    {
    lminsx -= lminx;
    c->env->left += lminsx;
    c->col       += lminsx;
    c->r_col     += lminsx;
    }
  if( lminsy>lminy )
    {
    lminsy -= lminy;
    c->env->upper += lminsy;
    c->row        += lminsy;
    c->r_row      += lminsy;
    }
  if( c->flg!=c_f_dust && c->w<MAX_DUST_WIDTH && c->h<MAX_DUST_HEIGHT )
    {
    cell *wc;
    c->flg   = c_f_dust;
    c->nvers = 0;
    c->vers[0].let=c->vers[0].prob=0;
    (wc=c->prevl)->nextl = c->nextl;
    c->nextl->prevl      = c->prevl;
    err_pnlet(c);
    }
  if( le<0 && c->col+le>=0 && c->r_col+le>=0 && c->env->left+le>=0 )
    {
    c->env->left += le;
    c->col       += le;
    c->r_col     += le;
    }
  c->pos_inc=erect_rot;
  if( n>1 && cutting)
    cret = convert_to_cells(c);
  }

return cret;
}

BOOL no_incline_word(cell *b,cell *e)
{
cell *c;
for(c=b;c!=e;c=c->next)
  if( c->pos_inc&erect_rot )
    return FALSE;
return TRUE;
}

BYTE   erection_one_incline_word(cell *b, cell *e)
{
BOOL    inc   ;
B_LINES bl    ;

if( !setup_incline_word(b,e, FALSE) ) // without statistic making
  {
  if( db_status && snap_activity('i') )
    {
    snap_show_text("ERECT : No erection images");
    snap_monitor();
    }
  clear_incline_word(b,e);
  return 0;
  }
get_b_lines(b,&bl);

inc = erection_incline_word_set(b, e);
return (BYTE)inc;
}

//
// main erection function :
// find and erected incline words
//
BYTE   erection_incline_words(INT pass)
{
cell *c, *e, *tmp, *sc              ;
BYTE buf[MAX_LEN_WORD+40]           ;
BYTE wrd[MAX_LEN_WORD+40], word_len ;
BOOL inc, incline, no_res           ;
B_LINES bl                          ;
#ifdef STEND_INC
static CHAR  oldstr[256]="c:\\";
CHAR         str[256]      ;
static INT   oldline=-1    ;
FILE         *fp           ;
static INT   first=1       ;
#endif
INT          av_inc=0, n_inc=0;

local_pass=pass;
if( !setup_incline_word(cell_f()->nextl,cell_l(), TRUE) )
  {
  if( db_status && snap_activity('i') )
    {
    snap_show_text("ERECT : No erection images");
    snap_monitor();
    }
  clear_incline_word(cell_f()->nextl,cell_l());
  return 0;
  }


if( db_status && snap_activity('i') )
 {
 snap_show_text("ERECT : Begin erection words");
 snap_monitor();
 }

c       = cell_f() ;
incline = FALSE    ;
while(1)
  {
  if( (c  = erect_next_word(c))==NULL )break;
  no_res=1;
  inc   =0;
  strcpy(buf,"ERECT : Word : ");
  get_b_lines(c,&bl);
  e = erect_end_word(c, wrd, &word_len, (INT)(bl.ps/2) );
  strcpy(&buf[14],wrd);
  erect_page_words++;
  sc = snap_stopcell(c,e)?snap_get_stopcell():NULL;
  tmp = c->prev;
  inc = erection_incline_word(c, e, bl.b3, 0);
  if( inc && !local_pass )
    {
    incline = 1;
    av_inc+=inc;
    n_inc++;
    erect_page_incline[inc/16]++;
    }

#ifdef STEND_INC
  if( inc && !(db_status && snap_activity('i')) )
    {
    set_page_name(str);
    if( strcmp(str, oldstr) )
      {
      strcpy(oldstr, str);
      fp = fopen("erection.txt",first?"wt":"at");
      if( fp==NULL )
        fp = fopen("erection.txt","wt");
      if( first)  first=0;
      fprintf(fp,"\nfile : %s", str);
      fclose(fp);
      }
    if( oldline!=line_number)
      {
      oldline=line_number;
      fp = fopen("erection.txt","at");
      fprintf(fp,"\nline %d : ", line_number+1);
      fclose(fp);
      }
    sprintf(wrd,"(%d)",inc);
    no_res=0;
    strcat(buf,wrd)        ;
    fp = fopen("erection.txt","at");
    fprintf(fp," %s ", &buf[14]);
    fclose(fp);
    }
#endif

  if( db_status && snap_activity('i') )
    {
    if( inc || sc )
      {
      if( inc )
        sprintf(wrd,"(%d)",inc);
      else
        sprintf(wrd,"(no erection)");
      if( no_res )
        strcat(buf,wrd)      ;
      snap_newcell(sc?sc:tmp->next)        ;
      snap_show_text(buf)    ;
      snap_monitor()         ;
      }
    }
  if( e==NULL )
    break;
  c = e;
  }
if( n_inc )
    erection_inc = av_inc / n_inc;
clear_incline_word(cell_f()->nextl,cell_l());
if( db_status && snap_activity('i') )
 {
 snap_show_text("ERECT : End erection words ");
 snap_monitor();
 }

return(BYTE) incline ;
}


INT get_baton_inc(cell *c)
 {
 INT n, inc=0;
 STICK *res,*r;
 Word8 let;

if( c->nvers<1 )
    return 0;

 let = decode_ASCII_to_[c->vers[0].let][0];
 n=sticks_in_letter(c,0,&res);
 r=res;

 if (n>=0)
    {
    switch( let )
        {
        case    (Word8)'�':
        case    (Word8)'�':
			if (is_russian_baltic_conflict(let))	// 17.07.2001 E.P.
				{inc=1;break;}

            if( n==2 && abs(r[1].incl-r[0].incl)<50 )
                inc=r[1].incl;
            break;
        case    (Word8)'�':
        case    (Word8)'�':
            if( n==2 && abs(r[1].incl-r[0].incl)<50 )
                inc=r[1].incl;
            else if( n==1  )
                inc=r[0].incl;
            break;
        case    (Word8)'�':
        case    (Word8)'�':
            if( n==1  )
                inc=r[0].incl;
            break;
        default:
            inc=1;
            break;
        }
    }
 else
      return 0;

 return inc;
 }

// erecting all letters with sticks and bad symbols
BOOL setup_incline_word(cell *b, cell *e, BOOL calc)
{
cell *c , *cs;
INT  inc, ninc, nall;

if( calc )
  {
  memset(hist,0,sizeof(hist));
  memset(extr,0,sizeof(extr));
  }
for( nall=ninc=0, c=b; c!=e; c = c->next,nall++)
    {
    if( !(c->pos_inc&erect_rot) )
      {
      c->pos_inc=erect_no;
      c->right=0;
      if( (c->flg&c_f_let) && // for inclinables images
          (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars) ) &&
			!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
		  ||
           erect_solid_stick(c))  ||
          (c->flg&c_f_bad) )  // or  bad symbols
        {
        if( erector(c, 1, 0, 1, 0)==2 )  // set incline in c->stick_inc
            {

            }
        inc=0;
        if( c->stick_inc!=NO_INCLINE )
          inc = c->stick_inc;
        if( inc>=0 )
          {
          if( calc  )
            {
            hist[ inc/16 ]++;
            }
          ninc++;
          cs = c;
          }
        }
      }
    }

if( calc )
  {
  num_extr = 0;
  if( hist[0]+1==ninc && hist[16]==1 )
    return FALSE;
  if( !(hist[0]*3>ninc*2) && ninc>9) //10 )
    { // binarize if few nonIncline images & many incline images
    for(inc=0;inc<128;inc++)
      if( hist[ inc ]>=ninc/4 && num_extr<sizeof( extr ) )
        extr[ num_extr++ ] = (BYTE)inc;
    }
  if( num_extr )
    max_incline = MAX((extr[ num_extr-1 ]+1)*16,512);
  else
    max_incline = 512;
  if( !num_extr )
    {
    for( c=b; c!=e; c = c->next)
        {
        if( c->stick_inc<300 ||
            c->stick_inc<400 && get_baton_inc(c)==0 )
            {
            if( calc  )
                hist[ c->stick_inc/16 ]--;
            ninc--;
            if( !local_pass )
                c->stick_inc=0;
            }
        }
    }

  if( num_extr )
    {
    for( c=b; c!=e; c = c->next)
    if( !(c->pos_inc&erect_rot) )
      {
      if( (c->flg&c_f_let) && // for inclinables images
        memchr(aux_inc_chars,c->vers[0].let,sizeof(aux_inc_chars)) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
		)
      {
      inc = 0;
      erector(c, 1, 0, 1, 1);  // set incline in c->stick_inc
      if( c->stick_inc!=NO_INCLINE )
        {
        inc = c->stick_inc;
        ninc++;
        }
      if( !mode_incline(inc) && inc<300 )
        {  // non characteristical incline
        c->pos_inc=erect_no;
        c->right=0;
        }
      }
    }
    }
  line_incline = ( ninc*2>nall );
  }
else // calc==0 : study one word
  if( ninc==0 )
    return FALSE;
return TRUE;
}

// clear incline for letters without sticks
void clear_incline_word(cell *b, cell *e)
{
cell *c, *prev ;
PBYTE p ;

for( c=b; c!=e; c = c->next)
  {
  if( !(c->pos_inc&erect_rot) )
    {
    if( c->stick_inc!=0 )
      c->stick_inc=NO_INCLINE;
    else
      c->pos_inc=erect_zero; // 0 incline
    }
  if( !(c->next->flg&c_f_fict) &&
        !(c->prev->flg&c_f_fict) &&
        c->col<c->prev->col )
        {
        prev = c->prev;
        p = del_save_cell(c);
        rest_cell(p, prev);
        }
  }
return;
}

void average_inc(INT inc_list[],INT n, INT *rinc, INT *rnum)
{
INT i, inc,ni;
for(ni=inc=i=0;i<n;i++)
  if( inc_list[i] )
    {
    inc+=inc_list[i];
    ni++;
    }
*rinc = (ni>0 ? inc/ni : 0);
*rnum = ni;
return;
}

INT select_inc(INT inc_list[],INT n, INT delta, INT *rinc, INT *rnum)
{
INT i, inc, m;
if( *rnum<2 )
  return 0;
for(inc=*rinc,i=0;i<n;i++)
  {
  m=inc_list[i];
  if( m )
    {
    if( m>inc+delta || m<inc-delta )
      {
      (*rnum)--; // delete first far value
      *rinc = (inc*((*rnum)+1)-m)/(*rnum);
      inc_list[i]=0;
      return 1;
      }
    }
  }
return 0;
}

// calculate representante value in array
INT  represent_inc(INT inc_list[],INT n, INT delta)
{
INT inc,num;
do{
  average_inc(inc_list,n, &inc, &num);
  }
while( num>1 && select_inc(inc_list,n, delta, &inc, &num));
return num>1?inc:0;
}

INT  num_inc_in_list(INT inc_list[],INT n)
{
INT i, num;
for(num=i=0;i<n;i++)
  num += (inc_list[i]!=0);
return num;
}

BOOL mode_incline( INT inc)
{
INT i;
inc /= 16;
for(i=0;i<num_extr;i++)
  if( abs(extr[i]-inc)<2 ) // characteristical incline in string
    return TRUE;

return FALSE;
}

// check bad eventual aliases for inlineables letters
BOOL no_bad_alias(cell *c)
{
INT  i, n;
BYTE *p;

if( c->nvers<2 )
    return TRUE;
for(i=1;i<c->nvers;i++)
    {
    if( p=memchr(nonincline_alias,c->vers[i].let,sizeof(nonincline_alias)) )
        {
        n = p-nonincline_alias;
        if( n<sizeof(incline_alias) && incline_alias[n]==c->vers[0].let )
            return FALSE;
        }
    if( p=memchr(nonincline_alias2,c->vers[i].let,sizeof(nonincline_alias2)) )
        {
        n = p-nonincline_alias2;
        if( n<sizeof(incline_alias2) && incline_alias2[n]==c->vers[0].let )
            {
            STICK * r;
            INT     n;
            n=sticks_in_letter(c,0,&r);
            if( n==2 && abs(r[1].incl-r[0].incl)>50 )
                return FALSE;
            }
        }
    }

return TRUE;
}

// calculate average incline in word
INT get_incline_of_word(cell *b, cell *e)
{
cell *c                       ;
LONG  inc, inc1, n1           ;
INT   i, n, mn, all, zeromn   ;
INT   inc_list[MAX_LEN_WORD]  ;
INT   norm_list[MAX_LEN_WORD] ;
INT   zero, in, zerall        ;
#define MAX_INCLINE 55

memset(inc_list,0,sizeof(inc_list));
for(n1=inc1=inc=0,zerall=zero=zeromn=all=mn=i=n=0,c=b; c!=e; c=c->next)
  {
  if( (c->flg&(c_f_let|c_f_bad)) &&
      c->stick_inc!=NO_INCLINE &&
      c->stick_inc>1 &&
      (memchr(incline_main, c->vers[0].let, sizeof(incline_main)) &&
		!is_russian_baltic_conflict(c->vers[0].let)	&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let)// 21.05.2002 E.P.
	  ||
       c->stick_inc<max_incline)  &&
      c->nvers>0 &&
       ( (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars) ) &&
			!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	   ||
       erect_solid_stick(c) ) ||
        memchr(aux_inc_chars,c->vers[0].let,sizeof(aux_inc_chars) ) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
		)
	)
    {
    inc += c->stick_inc;
    norm_list[n] = c->stick_inc;
    n++;
    if( c->nvers>0 &&
        memchr(incline_main,c->vers[0].let,sizeof(incline_main)) &&
		!is_russian_baltic_conflict(c->vers[0].let)	&& // 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let)&&// 21.05.2002 E.P.
        no_bad_alias(c)
	  )
      mn++;
    if( (c->flg&c_f_bad)&&mode_incline(c->stick_inc) )
      mn++;
    }
  else if( (c->flg&c_f_bad) && mode_incline(c->stick_inc) )
    inc_list[i++] = c->stick_inc;

  if( c->stick_inc==0 )
    zerall++;
  if( (c->flg&c_f_let) &&
      c->stick_inc==0 )
    {
    if( c->nvers && memchr(incline_main,c->vers[0].let,sizeof(incline_main)) &&
			!is_russian_baltic_conflict(c->vers[0].let)&& // 17.07.2001 E.P.
			!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	  )
        zeromn++;
    else if(   c->nvers && '|'==c->vers[0].let && c->h>16 )
        zeromn++;
    }

  if( (c->flg&c_f_let) &&      c->stick_inc==0 &&      c->nvers>0 &&
    (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars) ) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	||
       erect_solid_stick(c)) )
    zero++;

  if((c->flg&c_f_let) && c->stick_inc!=NO_INCLINE && c->stick_inc>1 &&
     c->nvers>0 && (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars) ) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	 ||
       erect_solid_stick(c)) )
    {
    inc1 += c->stick_inc;
    n1++;
    }
  if( (c->flg&c_f_let) )  all++;
  }

if( n==0 )
  {
  if( i )
    return ( represent_inc(inc_list,i, 64) );
  else if(n1>0 && n1*2>all && line_incline )
    {
    inc1 /= n1;
    if( inc1<300 ) inc1=0;
    return (BYTE)inc1;
    }
  else
    return 0;
  }
else
  {
  if( (in=represent_inc(norm_list,n, 64))!=0 &&
       num_inc_in_list(norm_list,n)>2 )
    inc = in;
  else
    inc /= n;
  }


// delete incline for small statistic
if( inc<100 )
  inc = 0; // too small incline

if( mn==0 && ( num_extr==0 && n*5<all*3 || all==1 ))
  inc = 0; // too few inclineable images

if( mn==1 && num_extr==0 && nIncline>=MAX_INCLINE && inc-nIncline<=256 )
  inc = 0; // too few inclineable images  & large line incline

if( inc<300 &&  // small incline
    (zeromn>1 || hist[0]>5) &&  // exist zero inc in word or line
    (num_extr==0||num_extr==2 && extr[0]==0&&extr[1]<=16) && // non inc line
    (nIncline>=MAX_INCLINE && inc-nIncline<=256 ||  // large line incline
     zero>2 && mn<2) )           // many zero inc in word  & 1 inc
  inc = 0;

if( inc<350 && mn==0 && all<4 && zeromn>0 )
    inc=0;
if( mn==1 && zeromn>1 )          // many zero inc in word  & 1 inc
  inc = 0;

if( mn==1 && zeromn>=1 && nIncline>=MAX_INCLINE )
  inc = 0;

if( zeromn && zerall>MAX(all/3,2) && num_extr==0 && inc>max_incline )
  inc = 0;

if( inc>700 && mn<2 && !num_extr ) // big incline & too few inclineables letsx
  inc = 0;

// set new inc during bad letter with good incline
if( inc==0 && all<2 && num_extr )
  {
  INT num, ii;
  inc1 = represent_inc(inc_list,i, 64);
  for(num=ii=0;ii<i;ii++)
    if(inc_list[ii]==inc1) num++;
  if( num==i )
    inc = inc1;
  }

return (INT)inc;
}

static void erect_rotate_bl(cell *tmp, INT base_3, LONG inc, INT dir)
{
INT     h1, h2;
LONG    d;
h1 = tmp->row-base_3;
h2 = base_3 - tmp->row-tmp->h;
if( h2>0 )      d =  h2;
else if( h1>0 ) d = -h1;
else            d =   0;
if( (tmp->flg&c_f_dust) && base_3>tmp->row+2*tmp->h )
  d = base_3-(tmp->row+tmp->h/2) ;
if( d )
  {
  d *= inc; d /= 2048;
  if( dir>0 )
    d = -d;
  if( d && tmp->col>=d && tmp->r_col>=d )
	 // Nick 14.08.2002 - ���������� ���� - tmp->env �.�. NULL !!!
	 // &&   tmp->env->left>=d )
    {
    tmp->col       -= (INT)d;
    tmp->r_col     -= (INT)d;
    if( tmp->env  && tmp->env->left>=d )
      tmp->env->left -= (INT)d;
    }
  }
}

INT erect_cell_width (cell *c,INT inc)
{
INT     dy=c->h                       ;
INT     i                             ;
INT     tab_angle[256]                ;

if( !c->env     )              return c->w;
if( ((long)(dy-1)*abs(inc))<2048 )
  inc=0;
if( inc==0 )                   return c->w;
for (i=0; i<dy; i++)
  tab_angle[i] = ((long)(dy-1-i)*inc)/2048;

  //  rotate without shaving
return fict_shift_left_intervals(c, tab_angle);
}

BOOL    test_incline_of_word(cell *b,cell *e,LONG inc)
{
//BOOL    ret=TRUE;
cell  * c;
INT     i,wn, up,let;
for(up=let=i=0, c=b; c!=e; c=c->next, i++)
    {
    if( c->flg&(c_f_let|c_f_bad) )
        {
        let++;
        if( c->stick_inc!=NO_INCLINE && (((long)c->stick_inc*c->h)/204l)>16 &&
          ( (c->flg==c_f_bad) && c->stick_inc<max_incline ||
          c->nvers>0 && (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars)) &&
			!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
		  ||
          erect_solid_stick(c))) )
            wn=erect_cell_width (c, (INT)(c->stick_inc));
        else
            wn=erect_cell_width(c, (INT)(inc));
        if( wn>c->w+1 )
            {
            up++;
            }
        }
    }
return !(up*2>let&&let>2);
}

//
// rotate word cells
//
INT erection_incline_word(cell *b, cell *e, INT base_3, INT n_call)
{
cell  *c, *tmp, *cnext                  ;
LONG  inc,    i                         ;
INT   shave=(erection_enable==2)        ;
version save_versions[VERS_IN_CELL]     ;
INT   save_nvers                        ;

if( (inc=get_incline_of_word(b,e))==0 )
  return 0;
if( 1&&!test_incline_of_word(b,e,inc) )
    return 0;

for(i=0, c=b; c!=e; c=c->next, i++)
  {
  tmp = c->prev; // left cell
  cnext=c->next; // right cell
  if( local_pass )
    {
    if( c->pos_inc&erect_rot )
        continue;
    if( c->stick_inc!=NO_INCLINE && (((long)c->stick_inc*c->h)/204l)>16 &&
      ( (c->flg==c_f_bad) && c->stick_inc<max_incline ||
      c->nvers>0 && (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars)) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	  ||
      erect_solid_stick(c))) )
        c->save_stick_inc = c->stick_inc;
    else
        c->save_stick_inc = (INT)inc;
    c->pos_inc = erect_rest;
    continue;
    }
  // rotate cell images
  if( c->stick_inc!=NO_INCLINE && (((long)c->stick_inc*c->h)/204l)>16 &&
      ( (c->flg==c_f_bad) && c->stick_inc<max_incline ||
      c->nvers>0 && (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars)) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	  ||
      erect_solid_stick(c))) )
    c=erect_cell_value (c, c->stick_inc, shave , TRUE);
  else
    c=erect_cell_value (c, (INT)inc, shave , TRUE);

  for(tmp = tmp->next;tmp!=cnext;tmp=tmp->next)
    {    // shift cell boxes
    erect_rotate_bl(tmp,base_3,inc,-1);//(INT)(line_scale?(base_3>>line_scale):base_3),inc,-1);
    v2_pidx_crit(tmp);
    if( (save_nvers = c->nvers)>0 )
        memcpy(save_versions,c->vers,VERS_IN_CELL*sizeof(version));
    // recognizing corrected images
    if( (tmp->flg&(c_f_bad|c_f_let)) && !local_pass)
      {
      if(n_call==0  )
        {
        if( tmp->nvers>0 &&
			memchr(disable_rerecog,tmp->vers[0].let,sizeof(disable_rerecog) ) &&
				!is_russian_baltic_conflict(tmp->vers[0].let)&&// 17.07.2001 E.P.
				!is_russian_turkish_conflict(tmp->vers[0].let)   // 21.05.2002 E.P.
			||
            tmp->nvers<1 )
          {
          short_recog_cell( tmp );
          levcut(tmp,1);
          add_versions(tmp, save_versions, save_nvers);
          } // end of spec letters for 1 pass
        }
      else
        {
        s_glue GL={0};

        GL.celist[0]   = tmp;
        GL.maxlist[0]  = tmp;
        GL.complist[0] = tmp->env;
        GL.maxlist[1]  = GL.celist[1]=NULL;
        GL.complist[1] = NULL;
        GL.ncell       = 1;
        GL.row         = tmp->row;
        GL.col         = tmp->col;
        GL.width       = tmp->w;
        GL.height      = tmp->h;

        if(crecell(tmp,&GL,3)>=0)
          dmBOX(tmp,&GL);
        } // end of spec letters for 2 pass
      } // end of if letter
    } // end of cykl tmp
  }

return (INT)inc;
}

INT erection_incline_word_set(cell *b, cell *e)
{
cell  *c                  ;
LONG  inc                 ;

if( (inc=get_incline_of_word(b,e))==0 )
  {
  for( c=b; c!=e; c=c->next)
    {
    c->pos_inc=erect_no;
    c->stick_inc = NO_INCLINE;
    c->save_stick_inc = NO_INCLINE;
    }
  return 0;
  }

for( c=b; c!=e; c=c->next)
  {
  // rotate cell images
  if( c->stick_inc!=NO_INCLINE && (((long)c->stick_inc*c->h)/204l)>16 &&
      ( (c->flg==c_f_bad) && c->stick_inc<max_incline ||
      c->nvers>0 && (memchr(incline_chars,c->vers[0].let,sizeof(incline_chars)) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	  ||
       erect_solid_stick(c))) )
    c->save_stick_inc=c->stick_inc;
  else
    c->save_stick_inc=(BYTE)inc;
  c->pos_inc=erect_no;
  c->stick_inc = NO_INCLINE;
  }

return (INT)inc;
}

// finding begin of word
cell * erect_next_word(cell *cs)
{
cell *c=cs ;

if( c==cell_l() ) return NULL;
if( c==cell_f() ) c=c->next;

return (c->flg&c_f_fict)?NULL:c;
}

// finding end of word
cell * erect_end_word(cell *cs,BYTE *str,BYTE *word_len, INT limit)
{
cell *c=cs             ;
INT   i=0, old=c->col+c->w  ;

while(1)
  {
  *str++  = (c->nvers>0&&c->flg==c_f_let)?c->vers[0].let:(c->flg!=c_f_dust?'':'');
  c = c->next;
  i++;
  if((c->flg&c_f_fict) || c->col - old>=limit ||  i>=MAX_LEN_WORD-1 )break;
  if( c->col+c->w>old) old =c->col+c->w;
  }
*str='\0';
*word_len = (BYTE)i;
return c;
}

//
// fict_shift_intervals : calculate width of rotate cell
//
INT fict_shift_intervals(cell *c, INT tab_angle[] )
{
INT      ind, ll, h, min_shift, left_shift, w ;
interval *inter;
lnhead   *line;

// calculating left offset of image
min_shift = MIN (tab_angle[0], tab_angle[c->h-1]);
for(line=(lnhead *)((PCHAR)c->env+c->env->lines+sizeof(INT)),left_shift=c->w;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead)); h; h--,inter++,ind++)
    if( (w=inter->e - inter->l - (tab_angle[ind]-min_shift))< left_shift )
      left_shift = w;  // max left limit
min_shift -= left_shift;

// rotating during shift table
for(line=(lnhead *)((PCHAR)c->env+c->env->lines+sizeof(INT)),w=0;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead));
        h; h--,inter++,ind++)
    if( (inter->e-tab_angle[ind]-min_shift) > w )
      w = inter->e-tab_angle[ind]-min_shift;  // max right limit - new width

return w;  // result width
}

INT fict_shift_left_intervals(cell *c, INT tab_angle[] )
{
INT      ind, ll, h, min_shift, left_shift, w ;
interval *inter;
lnhead   *line;

// calculating left offset of image
min_shift = MIN (tab_angle[0], tab_angle[c->h-1]);
for(line=(lnhead *)((PCHAR)c->env+c->env->lines+sizeof(INT)),left_shift=c->w;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead)); h; h--,inter++,ind++)
    if( (w=inter->e - inter->l - (tab_angle[ind]-min_shift))< left_shift )
      left_shift = w;  // max left limit
//min_shift -= left_shift;

// rotating during shift table
for(line=(lnhead *)((PCHAR)c->env+c->env->lines+sizeof(INT)),w=0;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead));
        h; h--,inter++,ind++)
    if( (inter->e-tab_angle[ind]) > w )
      w = inter->e-tab_angle[ind];  // max right limit - new width

return w-left_shift;  // result width
}

void erection_delete(void)
{
cell *c=cell_f()->next, *e=cell_l();
erection_delete_seq(c,e);
return;
}

void erection_delete_seq(cell *b,cell *e)
{
cell *c;
B_LINES bl;

get_b_lines(b,&bl);
for(c=b;c!=e;c=c->next)
  if( erection_enable && (c->pos_inc&erect_rot) && c->stick_inc!=NO_INCLINE )
    {
    c->save_stick_inc =  c->stick_inc;
    erect_cell_value(c, (INT)(-c->stick_inc), 0, FALSE);
    erect_rotate_bl(c,(INT)((p2_active&&line_scale)?(bl.b3>>line_scale):bl.b3),(LONG)(-c->stick_inc),+1);
    c->stick_inc = NO_INCLINE;
    c->pos_inc   = erect_rest;
    c->left      = c->col;
    c->right     = c->col+c->w;
    }
  else
    {
    c->save_stick_inc =  NO_INCLINE;
    c->pos_inc = erect_no;
    }
return;
}

void erection_restore(void)
{
cell *c=cell_f()->next, *e=cell_l();
erection_restore_seq(c,e);
return;
}

void erection_restore_seq(cell *b,cell *e)
{
cell *c;
B_LINES bl;

 get_b_lines(b,&bl);
 for(c=b;c!=e;c=c->next)
 {
  if( erection_enable   &&
      c->save_stick_inc && c->save_stick_inc!=NO_INCLINE )
    {
		c = erect_cell_value(c, c->save_stick_inc, 0, FALSE);

		erect_rotate_bl(c,(INT)((p2_active&&line_scale)?(bl.b3>>line_scale):bl.b3),(LONG)(c->save_stick_inc),-1);
    }
 }
return;
}

INT diff_left_limit_rast(BYTE *raster, INT dx, INT dy, INT tab_angle[])
{
INT i, d, c, rr, lmax;
BYTE  *r ;

d  = bytlen (dx);
for (r=raster,lmax=i=0; i<dy; i++,r+=d)
  {
  c = tab_angle[i];
  rr = LeftDistance(r,d);
  if( rr-c<lmax )   lmax = rr-c;
  if( rr==0 ) break;
	}

return (lmax);
}

INT diff_left_limit_one_line(lnhead *line, INT tab_angle[], INT init_max)
//
//  This procedure find max left shift on the left abris during rotating
//  stuies one line
{
INT      num_row, i, h, lmax;
interval *inter;
INT      b, l;

h=line->h;
lmax=init_max; i=0;  num_row = line->row;
inter=(interval *)((PCHAR)line+sizeof(lnhead));

for( ; h ; h--,inter++,i++,num_row++)
  {
  b = inter->e  - inter->l;
  l = tab_angle[num_row];
  if( b-l<lmax )    lmax = b - l;
  if( init_max==0 && b==0 ) break;
  }

return lmax;
}

INT diff_left_limit_cell(cell *c, INT tab_angle[], INT init_max)
//
//  This procedure find max left shift on the left abris during rotating
//  studies all lines
{
 lnhead   *line;
 INT      ll, lmax, l;

for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT)),lmax=init_max;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
        {
        l = diff_left_limit_one_line(line, tab_angle, init_max);
        if( l<lmax )          lmax = l;
        }

return lmax;
}
#define NCOMPMAX 8
INT erection_compose_inc(INT n,cell **clist)
{
INT i;
LONG inc; INT ninc;

for ( ninc=0,inc=i=0; i<n && i<NCOMPMAX; i++)
  if( clist[i]->pos_inc&erect_rot )
    {
    if( clist[i]->stick_inc!=0 && clist[i]->stick_inc!=NO_INCLINE )
      {
      inc += clist[i]->stick_inc;
      ninc++;
      }
    }
  else if( clist[i]->pos_inc&erect_zero )
    {  // zero incline
    ninc++;
    }

inc = ninc ? inc/ninc:NO_INCLINE;
return (INT)inc;
}

LONG calc_dens(cell *c)
{
LONG dens=0l;
INT      ll, h ;
interval *inter;
lnhead   *line;

// calculating left offset of image
for(line=(lnhead *)((PCHAR)c->env+c->env->lines+sizeof(INT));
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,inter=(interval *)((PCHAR)line+sizeof(lnhead)); h; h--,inter++)
      dens += inter->l;
return dens;
}

cell *convert_to_cells(cell *start)
{
PBYTE    raster = save_raster (start);
MN      *sh_mn;
cell    *sh_cell, *next=start->next;
INT      i;

if( (sh_mn = c_locomp (raster, (INT)(bytlen(start->w)), start->h, 0, 0))==NULL )
    return NULL;

for(i=0; sh_mn && i<MAX_CELLS_IN_LIST; i++, sh_mn = sh_mn->mnnext)
    {
    if( (sh_cell=create_cell_work(sh_mn, start,
                              start->bdiff, (CHAR)(start->difflg&0xf0)))==NULL )
      return NULL;

    levcut(sh_cell,1);

    }

del_cell(start);

return next->prev;
}


//
// add_versions : adding versions from array save_versions[0:save_nvers-1]
//                to cell c
BOOL add_versions(cell *c, version *save_vers, INT save_nvers)
{
BOOL     res=FALSE;
version *v, *r;
INT      i, n;

if( save_nvers<1 )  return FALSE;
n=c->nvers;
for(r=&c->vers[n>0?n:0],v=save_vers,i=0;i<save_nvers&&n<VERS_IN_CELL;i++,v++)
  if( !check_let(c,v->let) )
    {
    *r++ = *v;
    n++;
    res = TRUE;
    }
if( res )
  {
  c->nvers = n;
  c->flg   = c_f_let;
  }

return res;
}


void  calcErection( cell *c )
{
   INT  s;

   if( memchr(incline_chars,c->vers[0].let,sizeof(incline_chars)) &&
		!is_russian_baltic_conflict(c->vers[0].let)&&	// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c->vers[0].let) // 21.05.2002 E.P.
	   ||
       erect_solid_stick(c) ){
      s = c->stick_inc;
      erector(c,1,1,1,1);
      c->save_stick_inc = c->stick_inc;
      c->stick_inc = s;
   }
   return;
}


//
// erect_shift_intervals
//
void erect_shift_intervals(void *addr, INT h, INT tab_angle[] )
{
INT      ind, ll, min_shift, w ;
interval *inter;
lnhead   *line;

// calculating left offset of image
for(line=(lnhead *)addr,min_shift=RASTER_MAX_WIDTH;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead)); h; h--,inter++,ind++)
    if( (w=inter->e - inter->l - tab_angle[ind]) < min_shift )
      min_shift = w;  // min dest to image from left bound

// rotating during shift table
for(line=(lnhead *)addr,w=0;
    (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
  for(h=line->h,ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead));
        h; h--,inter++,ind++)
      inter->e -= tab_angle[ind]+min_shift;  // max right limit - new width

return ;  // result width
}

BOOL       erect_solid_stick(cell *c)
{
INT i;
return FALSE;
for(i=0;i<c->nvers;i++)
    {
    if( !memchr(solid_sticks,c->vers[i].let,sizeof(solid_sticks)) &&
		c->vers[i].let!= liga_i	&& // 08.09.2000 E.P.
		!(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(c->vers[i].let==i_sans_accent||c->vers[i].let==II_dot_accent)
		 )
		)
        return FALSE;
    }
return (c->nvers>0);
}

INT erection_cond_language(BYTE language)
{
return !(language==LANG_RUSSIAN && !langSer && !langUkr);
}

INT erection_language(BYTE language)
{
return TRUE;
}

INT erection_cyr_language(BYTE language)
{
return (language==LANG_RUSSIAN);
}

