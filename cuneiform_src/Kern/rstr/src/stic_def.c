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



#include "nt_types.h"

  #include <stdlib.h>
  #include <string.h>

  #include "func.h"
  #include "ligas.h"
  #include "stick.h"
  #include "status.h"
  #include "lang.h"
  #include "inc_tab_rstr.h"
#include "minmax.h"
extern INT nIncline  ;

extern BYTE fax1x2;	// MK NEW 14.01.1993
extern int  inc_num_EEM;	// in ST_TOOLS.C;
extern int  mk_dis_for_liga_exm;	// 06.01.1994
extern BYTE left_letter_EEM;	// 17.01.1994
/*......................................................................*/
#ifdef	MKPRINT_ENABLE				// MK OTLADKA Variables
extern WORD	mkm1, mkm2, mkm3, mkm4, mkm5;
extern WORD	mkma, mkmb, mkmc, mkmd, mkme;
extern WORD	d,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10;
extern	WORD	mkFlag_Dump;
#endif
/*----------------------------------------------------------------------*/
#ifdef	MKPRINT_ENABLE				// MK MACROS for PRINT:
/*......................................................................*/
#define	MKPRINT_BUF(n,buf)	if ((d&7)==n)  {  d0=buf[0];		\
	d1=buf[1]; d2=buf[2]; d3=buf[3]; d4=buf[4]; d5=buf[5];		\
	d6=buf[6]; d7=buf[7]; d8=buf[8]; d9=buf[9]; d10=buf[10];  }
/*......................................................................*/
#define	MKPRINT_STR(n,s,e)	if ((d&7)==n)  {  d0=s[0].e;		\
	d1=s[1].e; d2=s[2].e; d3=s[3].e; d4=s[4].e; d5=s[5].e;		\
	d6=s[6].e; d7=s[7].e; d8=s[8].e; d9=s[9].e; d10=s[10].e;  }
/*......................................................................*/
#endif
/*----------------------------------------------------------------------*/
extern	INT del_sticks_F_60 (cell *c);			// 05.03.1993
/*----------------------------------------------------------------------*/
							// ST_TOOLS.C:
				// 14.12.1993 move them here from FUNC.H:
void full_snap (cell *c,
		s_glue *GLU,	// 14.12.1993 add this parametr;
		STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
		INT tab_angle[],
		INT left_mode, INT right_mode, INT opt, INT wide,
		INT inc_v, INT t);
void short_snap (CHAR *s, INT t);
/*----------------------------------------------------------------------*/
						// STIC_MAK.C MODULES:

extern INT make_center_line(center_interval center[],INT nc,
		      BYTE left[], BYTE right[],
		      INT dy, INT dx,  INC_BASE *angles[],INT num_angles,
		      INT tab_angle[],
		      BOOL comp_wide,BOOL sig_T,BOOL sig_f,BOOL sig_r,
          INT *wid,INT hooks[],INT *inc_v, BYTE enable_correct);

INT abris_convexity(BYTE fun[],INT n,INT w);
extern	BOOL bad_overlay(INT over,INT width,INT dy,INT sig_wide,BOOL c_f);
extern	INT centers_len_to_hist(center_interval fun[],INT n,INT dy,INT dx,
			  BYTE hist[]);
extern	INT overlay_interval(center_interval c[],INT nc, INT col,INT typ,
			    INT tab_angle[]);

extern	INT max_center_hist(BYTE fun[],INT n,
		    center_interval center[],INT nc,INT tab[],INT typ);
/*----------------------------------------------------------------------*/
static INT abris_expansion (BYTE left[], BYTE right[],
			    INT dy, INT dx, INT tab_angle[]);
static INT discrim_left_brace (BYTE left[], BYTE right[],
			INT dy, INT dx, INT wid);
static INT discrim_right_brace (BYTE left[], BYTE right[],
			INT dy, INT dx, INT wid);
static BOOL dis_bold_r (BYTE fun[], INT n);
static INT first_tg (INC_BASE *tab_inc[], INT num_inc, INT tg2048 );

static void calc_discrim_braces(BYTE left[], BYTE right[], INT dy, INT dx,
				center_interval center[], INT nc, INT wide,
				INT tab_angle[], BYTE left1[], BYTE right1[],
				BOOL c_brace_l, BOOL c_brace_r,
				INT *d_l, INT *d_r);

static void filtrate_abris( BYTE left[],BYTE right[],INT dy,INT dx,INT wide,
		      INT hooks[],INT ul,INT ur,INT dl,INT dr,INT inc);
static INT calc_T_config(INT hist_int[],INT n,INT up_lim,INT lev);
static WORD calc_T_2_3 (INT hist_int[],INT n);	//MK 21.01.1993 (old Y_config)
static WORD abris_leap_new(BYTE left[], BYTE right[], INT n, INT wide); // MK
static BOOL T_roof(INT hist_int[],BYTE left[],BYTE right[],INT n,INT w);
static void add_stick_vers_a_posteriory (cell *c,
		STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);

			// here was DEBUG_GRAPH
static INT cell_to_centers(cell *c, INT nc,INT offstr,INT offcol,INT maxcol,
		    center_interval center[],BYTE left[], BYTE right[]);
static INT multicell_to_centers(cell *c,s_glue *GL, center_interval center[],
			    BYTE left[], BYTE right[], INT *dy, INT *dx);

static INT num_of_lines(center_interval center[],INT nc,INT dy,INT h[]);
static INT off_shift_string( BYTE string[],INT len );

static INT dest_to_comp( BYTE raster[],INT hei, INT wid);
static INT num_zero_intervals(BYTE fun[],INT n,INT lev);
static INT correct_braces(BYTE fun[],INT n,INT lev,INT typ);
static INT lnhead_to_centers(lnhead *lin, INT wid,
		    center_interval center[],BYTE left[], BYTE right[]);
static INT study_typ_of_center_line(cell *c,INT typ_snap,
	INT typ,BYTE right[],INT dy,INT prob);

#define MIN_LIMIT_HEIGHT 4
#define LIMIT_HEIGHT     256
#define LIMIT_WIDTH      64                /* N.B. LIMIT_WIDTH*4 < 256 !!! */
#define LIMIT_CENTER     LIMIT_HEIGHT*3
#define LIMIT_OF_ANGLES  8
/*----------------------------------------------------------------------*/
						// GLOBAL VARIABLES :
BYTE GL_hist [2*LIMIT_HEIGHT];			/* array for histogramm	*/

center_interval GL_cent [LIMIT_HEIGHT];		/* center of intervals	*/

center_interval GL_center [LIMIT_CENTER];	// center of c_comp-intervals

	BYTE GL_left1 [LIMIT_HEIGHT],	/* auxiliary left and		*/
	     GL_right1[LIMIT_HEIGHT];	/*    right abris-arrays	*/

static INT  GL_hooks[4];		/* array of "hooks"		*/
static INT  GL_hist_int[LIMIT_HEIGHT];	/* number of intervals in any row */

	BYTE GL_left0 [LIMIT_HEIGHT],	/* left and right abris-arrays	*/
	     GL_right0[LIMIT_HEIGHT];	// NO STATIC from 19.01.1993

static INT GL_tab_angle [LIMIT_HEIGHT];		/* optimal center inc line */
static INT num_lines;
/*----------------------------------------------------------------------*/
/*                                                                       */
/* in WATCOM __STK need time for init size for array - use global array  */
/*                                                                       */
/*----------------------------------------------------------------------*/
/* stick_center_study : discriminator for sticks                     */
/*                                                                   */
/* *c,*GL - cell and glue-cell-set( may be NULL -> use *c only ),    */
/*     typ_snap=1 - make all works                                   */
/*     typ_snap=0 - show results and debug information only          */
/*                                                                   */
/* GLOBAL : array left,right,left1,right1,base_angles,               */
/*                tab_angle,center                                   */
INT erector (cell *c , INT disable_rotate, INT enable_shave,INT typ_snap, INT enable_diskrim )
{
INT dis_slash=0, typ_inc;
INT typ,wide,opt,prob,inc_v;
INT skip_ul,skip_dl,skip_ur,skip_dr;
INT nc,num_angles,f_a;

INT dy=c->h,dx=c->w;
BOOL c_wide = check_let(c,'r')|check_let(c,'T')|
    check_let(c,'J')|check_let(c,'L');
BOOL c_slash = check_let(c,'/'), c_f=check_let(c,'f');
BOOL c_T = check_let(c,'T'),c_r=check_let(c,'r'),c_Y=check_let(c,'Y');
BOOL corr = ( c->nvers==1 && c->vers[0].let=='r' );

num_lines = 0;
c->stick_inc=0;
if( dy<=MIN_LIMIT_HEIGHT || dy>=LIMIT_HEIGHT || dx>=LIMIT_WIDTH )
	{
    short_snap("too big c_comp",typ_snap);
	return(2);   /* too big c_comp */
	}

if( (typ = multicell_to_centers (c, NULL, GL_center,
				GL_left0, GL_right0, &dy, &dx)) < 0 )
	{
    short_snap("too many intervals in glue-list-c_comp ",typ_snap);
	return(2);
	}
if( dy>=LIMIT_HEIGHT || dx>=LIMIT_WIDTH ) /* study new sizes dx,dy        */
	{
    short_snap(" too big c_comp ",typ_snap);
	return(2);   /* too big c_comp */
	}
else nc = typ;
prob = ADD_PROB(c) - abs(dy-nc)*20;

/* ---------------- upper dust not selected ------------------------ */

num_angles =   sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0]);
f_a=first_tg(stick_inc_rs, num_angles, nIncline );
num_angles=MIN(LIMIT_OF_ANGLES+(c_slash!=0),
           (INT)(sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0])-f_a-1));
if( f_a<11 ) f_a=11;
typ=make_center_line (GL_center, (INT)(nc-(GL_center[nc-1].len==1)),
		GL_left0, GL_right0, dy, dx, &stick_inc_rs[f_a], num_angles,
		GL_tab_angle, c_wide, c_T, c_f, c_r,
    &wide, GL_hooks, &inc_v, 0); // without correct

/* ------- center_line, array of hooks, typ inc and wide ready -------- */
if( c_slash)
	dis_slash = ( dx<(dy*nIncline)>>10 ) ? 128 : 0;
if( enable_diskrim || !c_slash  )
    typ = study_typ_of_center_line (c, typ_snap, typ, GL_right0, dy, prob);
else
    typ = -2;

if( typ!=-2 )
	return( typ );  /* not stick : ret code >0 */

/* --- stick is correct ( not similar '(',')' and exist central line) --- */

opt = MAX ( GL_tab_angle[0], GL_tab_angle[dy-1] );
abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);
/* --------- shift abris if inc; step of abris = 4 ---------- */

opt = (opt>>1)<<1;
typ_inc = (inc_num_EEM!=0);	// 09.12.1993, because OLD CONDITION

calc_skipped_lines (GL_hist_int, GL_left0, GL_right0, dy, dx, wide, opt,
		    (BOOL)(c_r|c_Y), &skip_ul, &skip_ur, &skip_dl, &skip_dr);
filtrate_abris (GL_left0, GL_right0, dy, dx, wide, GL_hooks,
		skip_ul, skip_ur, skip_dl, skip_dr, typ_inc);

/* redefine widtf of stick for inc 'r' : variable                   */
/*                                       width of central intervals */
if (GL_tab_angle[0]!=GL_tab_angle[dy-1]  &&
    (GL_hooks[0] || GL_hooks[2])  &&  corr)
	{   /* redefined wide for inc by two upper hooks */
	wide = centers_len_to_hist(GL_cent,dy,dy,dx,GL_hist);
	wide = (max_center_hist(&GL_hist[1], (INT)(wide-1),
				GL_cent, dy, GL_tab_angle, 0) >> 1) + 1;
	}


if( !disable_rotate )
  {
  erect_cell_table (c,  GL_tab_angle, enable_shave, TRUE);
  c->pos_inc   = erect_rot;
  }
else
  c->pos_inc   = erect_no;
c->stick_inc = inc_v;
return(0);   /* normal return */
}

INT stick_center_study (cell *c, s_glue *GLU, INT typ_snap )
{
INT dis_left_brace=0,dis_right_brace=0; /* discrim value for braces   */
INT	dis_slash=0, l_f_sym=0, r_f_sym=0, typ_inc, typ_T;
///WORD	T_2_3, T_skok;			// OLD  typ_Y  for  Y_config
WORD	T_2_3, T_skok_LR;		// OLD  typ_Y  for  Y_config
INT typ,wide,opt,prob,left_mode,right_mode,inc_v;
INT skip_ul,skip_dl,skip_ur,skip_dr;
INT nc,num_angles,f_a,wide_up;
STICK_CHARS left_chars,right_chars;
STICK_SIGNUMS signums;

			// here was DEBUG_GRAPH
B_LINES bl;
INT dy=c->h,dx=c->w;
BOOL c_brace_l = check_let(c,'('), c_brace_r = check_let(c,')');
BOOL c_wide = check_let(c,'r')|check_let(c,'T')|
		check_let(c,'J')|check_let(c,'L'),
     c_brace = c_brace_l | c_brace_r;
BOOL c_slash = check_let(c,'/'),c_t=check_let(c,'t'),c_f=check_let(c,'f');
BOOL c_T = check_let(c,'T'),c_r=check_let(c,'r'),c_Y=check_let(c,'Y');
BOOL corr = ( c->nvers==1 && c->vers[0].let=='r' );

	left_letter_EEM = ' ';	// 17.01.1994  for dis_i (similar 'h');
  if (c!=NULL&&(c->flg&(c_f_let|c_f_bad)))
    {   // see also ST_TOOLS;
    cell  *cell_LEFT = c->prevl;
    if (cell_LEFT!=NULL  &&  cell_LEFT->nvers!=0)
      left_letter_EEM = cell_LEFT->vers[0].let;
    }
num_lines = 0;
if( !(c->pos_inc&erect_rot) )
  c->stick_inc=0;
if( dy<=MIN_LIMIT_HEIGHT || dy>=LIMIT_HEIGHT || dx>=LIMIT_WIDTH )
	{
			// here was DEBUG_GRAPH
	short_snap("too big c_comp",typ_snap);
	return(2);   /* too big c_comp */
	}

			// here was DEBUG_GRAPH

if (c->nvers)  sort_vers(c);	// 03.01.1994

if( (typ = multicell_to_centers (c, GLU, GL_center,
				GL_left0, GL_right0, &dy, &dx)) < 0 )
	{
	short_snap("too many intervals in glue-list-c_comp ",typ_snap);
	return(2);
	}
if( dy>=LIMIT_HEIGHT || dx>=LIMIT_WIDTH ) /* study new sizes dx,dy        */
	{
	short_snap(" too big c_comp ",typ_snap);
	return(2);   /* too big c_comp */
	}
else nc = typ;
prob = ADD_PROB(c) - abs(dy-nc)*20;
if ( num_of_lines (GL_center, nc, dy, GL_hist_int) && !c_T )
		{
		short_snap("non stick : >1 heigh line in c_comp ",typ_snap);
		if( typ_snap )
//////			del_sticks(c,0);	// OLEG:decrease prob for stics
			del_sticks_F_60(c);	// 05.03.1993
		return(1);
		}

/* -------- array center,hist_int ready; num of line if correct --------- */

if ( !c_r && GL_hist_int[0]==2 )
{
INT i;
for(i=0; GL_hist_int[i]==2 && i<dy; i++);
if( i>dy>>2 )  /* too big number of 2-interval-rows */
  {      /* selected upper glue dust          */
			// here was DEBUG_GRAPH
	short_snap("non stick : 2 upper lines in c_comp ",typ_snap);
	if( typ_snap )
		del_sticks(c,'Y');   /* decrease prob for stics      */
	return(1);
	}
}

/* ---------------- upper dust not selected ------------------------ */

num_angles =   sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0]);
f_a=first_tg(stick_inc_rs, num_angles, nIncline );
num_angles=MIN(LIMIT_OF_ANGLES+(c_slash!=0),
           (INT)(sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0])-f_a-1));

typ=make_center_line (GL_center, (INT)(nc-(GL_center[nc-1].len==1)),
		GL_left0, GL_right0, dy, dx, &stick_inc_rs[f_a], num_angles,
		GL_tab_angle, c_wide, c_T, c_f, c_r,
    &wide, GL_hooks, &inc_v, 1); // with correct

/*********************************************************
mkma = (tab_angle[0] << 8)  |  (tab_angle[2] & 0xFF);
mkmb = (tab_angle[3] << 8)  |  (tab_angle[7] & 0xFF);
mkmc = (tab_angle[8] << 8)  |  (tab_angle[20]& 0xFF);
mkmd = (tab_angle[21]<< 8)  |  (tab_angle[22]& 0xFF);
//////mkme = (tab_angle[3] << 8)  |  (tab_angle[7] & 0xFF);
***********************************************************/

//////mkm1 = typ;

/* ------- center_line, array of hooks, typ inc and wide ready -------- */

			// here was DEBUG_GRAPH
if( (c_brace||dx<=4) && typ!=1 && typ !=2 ) /* no recognize braces */
	calc_discrim_braces(GL_left0, GL_right0, dy, dx, GL_center, nc,
			wide, GL_tab_angle,
			GL_left1, GL_right1, c_brace_l, c_brace_r,
			&dis_left_brace, &dis_right_brace);

if( c_slash)
	dis_slash = ( dx<(dy*nIncline)>>10 ) ? 128 : 0;

			// here was DEBUG_GRAPH
//////typ = study_typ_of_center_line(c,typ_snap,typ,right,dy,prob);
typ = study_typ_of_center_line (c, typ_snap, typ, GL_right0, dy, prob);

//////mkm2 = typ;

if( typ!=-2 )
	return( typ );  /* not stick : ret code >0 */

/* --- stick is correct ( not similar '(',')' and exist central line) --- */

opt = MAX ( GL_tab_angle[0], GL_tab_angle[dy-1] );
if( c_f )
	{   /* calculate symptoms 'f' befor shift */
	INT e_len = dx-1-((opt+wide*2)>>2);
	if( opt%4==0 ) e_len++;
	l_f_sym = calc_left_f_symptom (GL_left0, dy, (INT)(dx>>1), (BOOL)(c_f&c_t));
	r_f_sym = calc_right_f_symptom(GL_right0, dy, e_len);
	}

abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);
/* --------- shift abris if inc; step of abris = 4 ---------- */

opt = (opt>>1)<<1;

//////typ_inc = (GL_tab_angle[0]!=GL_tab_angle[dy-1]);	BEFORE 09.12.1993
typ_inc = (inc_num_EEM!=0);	// 09.12.1993, because OLD CONDITION
				// was WRONG; zB swea11/50 'r' ("ar"),
				// it was INC, but the same values was
				// on the both ends of stick !!!!!!!!!!!!!

calc_skipped_lines (GL_hist_int, GL_left0, GL_right0, dy, dx, wide, opt,
		    (BOOL)(c_r|c_Y), &skip_ul, &skip_ur, &skip_dl, &skip_dr);
filtrate_abris (GL_left0, GL_right0, dy, dx, wide, GL_hooks,
		skip_ul, skip_ur, skip_dl, skip_dr, typ_inc);

/* ---------  abris-arrays are ready for finding stick chars ---------- */

/* redefine widtf of stick for inc 'r' : variable                   */
/*                                       width of central intervals */
if (GL_tab_angle[0]!=GL_tab_angle[dy-1]  &&
    (GL_hooks[0] || GL_hooks[2])  &&  corr)
	{   /* redefined wide for inc by two upper hooks */
	wide = centers_len_to_hist(GL_cent,dy,dy,dx,GL_hist);
	wide = (max_center_hist(&GL_hist[1], (INT)(wide-1),
				GL_cent, dy, GL_tab_angle, 0) >> 1) + 1;
	}

//////mkm3 = (opt<<8) | wide;
//////mkm4 = tab_angle[0];
//////mkm5 = tab_angle[dy-1];

set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		(INT)(opt - MIN (GL_tab_angle[0], GL_tab_angle[dy-1])),
		skip_ul, skip_dl, skip_ur, skip_dr,
		(INT)inc_num_EEM,
		&left_chars, &right_chars, &signums,
		&left_mode, &right_mode);
if( c_Y )
	correct_upper_peaks(&left_chars,&right_chars,dy);
/* --------- stick charcteristics ready ----------------- */

if( left_chars.m_meandr + right_chars.m_meandr>5 )
	wide =    right_mode - left_mode + 1;
	/* redefine width if many pimples */

if( GL_hist_int[0]==1 && c_f && right_chars.mount[0]>wide &&
    !(left_chars.mount[0]&&left_chars.mb_pos[0]==0&&
      right_chars.mount[0]&&right_chars.mb_pos[0]==0) &&
      (l_f_sym | r_f_sym) )		// MK PROBA 18.02.1993: HOTJ ODIN !!!
	correct_f_symptoms (GL_hist_int, dy, &l_f_sym, &r_f_sym);

/* ------- f_symptoms;wide are corrected ------- */

if( c_Y || c_T )
	{   /* calculate similar-flags for TY  */
	typ_T = calc_T_config (GL_hist_int, (INT)(dy>>1), (INT)MAX(skip_ul,skip_ur),
			       (INT)(wide<3?1:2));
	T_2_3 = calc_T_2_3 (GL_hist_int, (INT)(dy>>1));		// 21.01.1993
	T_skok_LR = abris_leap_new (GL_left0, GL_right0, (INT)(dy/2), wide);
								// 17.02.1993

	if( !typ_T && dy>15 &&
	    left_chars.down_serif==0 && right_chars.down_serif==0 )
		{  /* > 8-kegl; no down serif; typ_t==0 */
		INT s=MAX(skip_ul,skip_ur);
		typ_T = T_roof (&GL_hist_int[s], &GL_left0[s], &GL_right0[s],
				2, dx);
		}
//////	if( !typ_T && wide<<2>dy && wide>4 )		// OLD OLEG;
//////		typ_T = abris_leap(left,dy,wide<<2);
//////	if( !typ_T )					// NEW MK 14.01.1993
//////		typ_T = abris_leap_new (left, right, dy/2, wide);
//////		typ_T = abris_leap_new (left0, right0, dy/2, wide);
	}
else
//////	typ_T = T_2_3 = T_skok = 0;	// OLD  typ_T = typ_Y = 0 ;
	typ_T = T_2_3 = T_skok_LR = 0;	// OLD  typ_T = typ_Y = 0 ;
/* -------------------- TY-sign ready ------------------- */

			// here was DEBUG_GRAPH
wide_up=wide;
get_b_lines(c,&bl);
if( wide<3 && left_chars.down_serif && right_chars.down_serif )
	{    /* calculation new wide by thin stick */
	wide_up = centers_len_to_hist (GL_center, nc, (INT)(bl.b2-c->row+2),
					dx, GL_hist);
	wide_up = max_center_hist (GL_hist, wide_up,
				  GL_center, nc, GL_tab_angle, 0)>>1;
	if (wide_up>wide)  wide_up = wide;	// 16.12.1993
	}					// (I want less, not more!)

set_stick_aux_signums (c, GL_left0, GL_right0, dy, wide_up,
		&left_chars,&right_chars,&signums,
		dis_left_brace,dis_right_brace,dis_slash,l_f_sym,r_f_sym,
		typ_T, T_2_3, T_skok_LR,	// OLD  typ_T,typ_Y,
		&bl,num_lines,(INT)(inc_v-nIncline));
/* -------------  all chars and signums are ready -------------- */

mk_dis_for_liga_exm = 0;	// 06.01.1994	MK NEW PAREMETR;

	// NOTA BENE: VERSION ARE SORTED NOW (used for liga_exm):
add_thick_stick_versions(c,dx,wide,typ_inc,typ_snap,		// �, 1, !,
			&left_chars,&right_chars,&signums);	// liga_exm,
								// invers_exm;
//////add_thin_stick_versions(c,left,right,dy,dx,typ_inc,
add_thin_stick_versions (c, GL_left0, GL_right0, dy, dx,	// t, f, (, )
			typ_inc, &left_chars,&right_chars,&signums);
/* ------------- symbols 1,�,(,),f,t added --------------------- */

sort_vers(c);
full_snap (c,
	   GLU,		// 14.12.1993 add this parametr;
	   &left_chars, &right_chars, &signums, GL_tab_angle,
	   left_mode, right_mode,
	   (INT)MIN (GL_tab_angle[0], GL_tab_angle[dy-1]),
	   wide,
	   inc_v, typ_snap);	/* show results */

if (typ_snap)  {
  if( c->pos_inc&erect_rot )
    signums.incline = c->stick_inc;
	discrim_all_sticks (c, &left_chars, &right_chars, &signums);
	add_stick_vers_a_posteriory (c, &left_chars, &right_chars, &signums);
	}
/* ----------- all stick-versions disriminated ------------------ */
if( !(c->pos_inc&erect_rot) && !(c->pos_inc&erect_zero) )
  {
  c->stick_inc = inc_v-nIncline;
  if( c->stick_inc==0 )
    c->stick_inc=1;  /* versions in cell discrim ! */
  }
if( c->pos_inc&erect_zero )
  {
  c->stick_inc=1;  /* versions in cell discrim ! */
  }
/* c->stick_inc - information for next passes ( COMPLEX WORD ) */
/*......................................................................*/
return(0);   /* normal return */
}
/*----------------------------------------------------------------------*/
static void add_stick_vers_a_posteriory (cell *c,	// 23.11.1993
		STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  {

//////INT	i, n=c->nvers, num, tmp, c_f=check_let(c,'f');
INT	wid=s->stick_width, dy=s->height, inc=s->inc;
INT	prob_0=c->vers[0].prob, prob_new;
INT	let_0=c->vers[0].let;

	if (check_let (c,'t')  &&		// liga_exm	('!')
	    l->conc[2]  &&  r->conc[2]  &&
//////	    (l->conc[2] + r->conc[2]) >= (wid>>1)  &&	SEE BELOW:
	    abs (l->ce_pos[2] - r->ce_pos[2]) < 2  &&
	    (l->ce_pos[2] - l->cb_pos[2] > 4 ||
	     r->ce_pos[2] - r->cb_pos[2] > 4)  &&
	    !l->num_long_flags  &&  !r->num_long_flags)  {
		prob_new = (l->conc[2] + r->conc[2]) >= ((wid+1)>>1) ?
			   MIN (prob_0+2, 254) : MAX (prob_0-2, 2);
//////		add_stick_vers (c, liga_exm, 254);	// ST_TOOLS.C
		add_stick_vers (c, (CHAR)liga_exm, (BYTE)prob_new);
//////		sort_vers (c);
		}
					// NOTA BENE: without TEST of LINEAR;

	if (let_0==')'  &&	// j2/14 "[1-900-884-7237]"
	    inc  &&
	    l->mount[0]<2 && l->mount[4]<2  &&
	    dy > 24)
		add_stick_vers (c, ']', (BYTE)MIN (prob_0+2, 254));

//////	sort_vers (c);
	return;
}
/*----------------------------------------------------------------------*/
static BOOL T_roof(INT hist_int[],BYTE left[],BYTE right[],INT n,INT w)
{
INT i;
w <<= 2;
for(i=0;i<n;i++)
	if( hist_int[i]>1 || w-(right[i]-left[i])>8 )
//////		return(FALSE); /* >2 steps or >1 intervals */
		return (0);	/* >2 steps or >1 intervals */

//////return(TRUE); /* upper layer similar roof        */
	return (2);	/* upper layer similar roof        */
}
/*----------------------------------------------------------------------*/
static INT calc_T_config(INT hist_int[],INT n,INT up_lim,INT lev)
{							// 21.01.1993
INT	i, s;
//////		It was  n2=n>>1;  (REALLY USED h/4 ???)
//////	for (s=0,i=1; i<n2; i++)  if (hist_int[i]>2)  s++;	// see T_3
//////	if (s>0)  return (200 + s);

for(i=0;i<n&&hist_int[i]!=1;i++);  /* upper trash     */
if( i>up_lim && i>1 )
	return(0);                 /* no roof         */
for(s=0;i<n&&hist_int[i]==1;i++,s++); /* T-roof       */
if(s<lev )
	return(0);                 /* too thin T-roof */
for(s=0;i<n&&hist_int[i]!=1;i++,s++);
if( s>lev )
	return(1);                 /* too long T-ears */

return(0);
}
/*----------------------------------------------------------------------*/
#ifdef	OLEG_abris_leap_OLD
/* abris 'Y' hav'nt long jumps in upper zone */
static BOOL abris_leap(BYTE fun[],INT n,INT width)
{
INT i;
n>>=1;
width>>=1;
for(i=1;i<n;i++)
	if( fun[i]+width<fun[i+1] )
		return(TRUE);
return(FALSE);
}
#endif
/*----------------------------------------------------------------------*/
/* abris 'Y' hav'nt long jumps in upper zone */		// 21.01.1993	MK
/////static BOOL abris_leap_new (BYTE left[], BYTE right[], INT n, INT wide)  {
static WORD abris_leap_new (BYTE left[], BYTE right[], INT n, INT wide)  {
							// 17.02.1993
////INT	i, porog = MAX (wide, 4) << 2, d, dL=0, dR=0;
INT	i, porog = MAX (wide-2, 4) << 2, d, dL=0, dR=0;
		// NOTA BENE: left,right - MASTAB.4 (*4),  wide - MAST.1 (sic!)

	for(i=1; i<n; i++)  {
//////	    if ( (dd = left[i+1] - left [i])  > porog)  return (dd>>2);
//////	    if ( (dd = right[i] - right[i+1]) > porog)  return (dd>>2);
	    if ( (d = left[i+1] - left [i])  > dL)  dL = d;
	    if ( (d = right[i] - right[i+1]) > dR)  dR = d;
	    }
	dL = (dL>porog) ? (dL>>2) : 0;
	dR = (dR>porog) ? (dR>>2) : 0;
	return ( (dL<<8) | dR );
}
/*----------------------------------------------------------------------*/
static WORD calc_T_2_3 (INT hist_int[], INT n)  {	// 21.01.1993
INT	i, k, n2=0, n3=0;
							// OLD Y_config
	/* number of rows with >2 intervals : Y hav upper pit, T - no */

		// OLD OLEG: test of N.intervals>1 (from 1)
//////for(i=1;i<n&&hist_int[i]!=1;i++);  /* upper trash */  //////return(i);

		// MK NEW SECOND: test of N.intervals==2 (from 1 to n=h/2)
		// add 18.01.1993: if 3 or more => reset Y to 1 !!!
	for(i=1; i<n; i++)  {
	    if ((k=hist_int[i]) > 1)  {
		if (k==2)  n2++;		// if 2 intervals
		     else  { n3++; n2 = 0; }   	// sic: RESET if 3 or more !!!
		}
	    }
	return ( (n2<<8) | n3 );
}
/*----------------------------------------------------------------------*/
			// here was DEBUG_GRAPH
static INT study_typ_of_center_line(cell *c,INT typ_snap,
	INT typ,BYTE right[],INT dy,INT prob)
{
INT	ret;
/* c_comp	*env; */		// 08.07.1993	for 'r'
INT	N_lines;
switch(typ)
	{
	case 4 :                      /* wide c_comp  */
			// here was DEBUG_GRAPH
		if ( dis_bold_r(right,dy) )  {
		    N_lines = 0;		// 08.07.1993 for 'r': (!!!)
/***		    if ((env=c->env) &&
			(!(c->cg_flag&c_cg_noenv)))  N_lines = env->nl; **/
		    if (tenv(c))
		       N_lines = c->env->nl;    // 18.02.1994
//////		    if (N_lines<2)  		// 08.07.1993
		    if (N_lines<=2)  {		// 09.03.1993
			short_snap ("non stick : wide c_comp", typ_snap);
                        if( typ_snap )
				del_sticks(c,0);
				  /* decrease prob for stics */
				  /* and bad bold-r          */
			}
		    else  {			// 08/09.07.1993:
//////			short_snap ("non stick : wide c_comp, Lines>1: r-good",
			short_snap ("non stick : wide c_comp, Lines>2: r-good",
					typ_snap);
                        if( typ_snap )
				del_sticks(c,'r');
			}
		    }
		else	{
			short_snap ("non stick : wide c_comp (r-good)",
					typ_snap);
			if( typ_snap )
				del_sticks(c,'r');
			}
		ret=1;
		break;

	case 3 :                                /* no stick */
			// here was DEBUG_GRAPH
		short_snap("non stick : non exist central line ",typ_snap);
                if( typ_snap )
			del_sticks(c,'Y');   /* decrease prob for stics */
		ret=1;
		break;

	case 1 :                                /* find '('                */
			// here was DEBUG_GRAPH
		short_snap("non stick : similar (", typ_snap);
		if( typ_snap )  {
//////			del_sticks_OLD (c, 0);	/* decrease prob for stics */
			del_sticks (c, '(');	// 21.10.1993
			if( !(c->cg_flag&c_cg_cut) )  {
				prob =  cut_by_pos(c,'(',prob,1,1) ;
				add_stick_vers(c,'(',(BYTE)prob);  /* add ')'   */
				}
			}
                sort_vers(c);
		ret = 0;
		break;

	case 2 :                                /* find ')'                */
			// here was DEBUG_GRAPH
		short_snap("non stick : similar )", typ_snap);
		if( typ_snap )  {
//////			del_sticks_OLD (c, 0);	/* decrease prob for stics */
			del_sticks (c, ')');	// 21.10.1993
			if( !(c->cg_flag&c_cg_cut) )  {
				prob =  cut_by_pos(c,')',prob,1,1) ;
				add_stick_vers(c,')',(BYTE)prob); /* add ')'    */
				}
			}
                sort_vers(c);
		ret = 0;
		break;

	default :
		ret = -2;
		break;
	}
return(ret);   /* -2 - all right, 0 - add braces, 1 - no sticks */
}              /* anothe ret-code used TC-version               */

static void filtrate_abris( BYTE left[],BYTE right[],INT dy,INT dx,INT wide,
		      INT hooks[],INT ul,INT ur,INT dl,INT dr,INT inc)
{
INT center_h;

if( language==LANG_RUSSIAN && dx*2>wide*7 && wide>4 )
    {
    filtr121(&left[1+ul],(INT)(dy-2-ul-dl));
    }

if( wide>6 || dx>wide*4 && wide>2 || dx>wide*5 && wide<3 )   /* !!!???!!! */
	{
	filtr121(&left[1+ul],(INT)(dy-2-ul-dl));
	filtr121(&right[1+ur],(INT)(dy-2-ur-dr));
	}

if( dx>wide*3 && wide>2 && hooks[2]>1 )   /* !!!???!!! */
	{        /* filtrate right upper hook 'f' */
	center_h = (dy-ur-dr )>>1;
	filtr_short(&right[ur],center_h,5);
	}

if( ( dx>wide*3 || inc&&dx>wide*2 ) &&    /* by may be false pimples */
    wide>2 && hooks[3]>1 )   /* !!!???!!! */
	{        /* filtrate right downer hook 'l','i' */
	center_h = (dy-2-ur-dr )>>1;
	filtr_short(&right[1+ur+center_h],(INT)(dy-2-ur-dr-center_h),5);
	}

}
/*----------------------------------------------------------------------*/
static void calc_discrim_braces(BYTE left[],BYTE right[],INT dy,INT dx,
				center_interval center[],INT nc,INT wide,
				INT tab_angle[],BYTE left1[],BYTE right1[],
				BOOL c_brace_l,BOOL c_brace_r,
				INT *dis_l,INT *dis_r)
{
INT	i, n_2, porog;	// MK 20.01.1993
wide = centers_len_to_hist(center,nc,dy,dx,GL_hist);
wide = max_center_hist(GL_hist,wide,center,nc,tab_angle,0)>>1;
/* calc average width     */

memcpy(left1, left , dy);
memcpy(right1,right, dy);
if( dx>=3 )
	{    /* narrow brace after filtrate lose all pimple */
	filtr121(left1,dy);
	filtr121(right1,dy);
	}
/*......................................................................*/
				// MK 20.01.1993 for 'C' => '(' (BOLD-36)
	if ( ( c_brace_l ) && ( *dis_l==0) && (dx>7) )  {
	    n_2 = dy - 2;
	    porog = dx / 4;	// NB: realy 3/4: right FROM ZERO ON RIGHT !
	    for (i=0; i<n_2; i++)
//////		if ((right1[i]+left1[i]) < porog)  { *dis_l = 44; return; }
		if ((right1[i]+left1[i]) < porog)  { *dis_l = 70; return; }
	    }
/*......................................................................*/
if( wide<7 )
	{    /* wide braces may have big bugles -->> not similar arc */
	if( c_brace_l || dx<=4 )
		*dis_l=discrim_left_brace(left1,right1,dy,dx,wide);
	if( c_brace_r || dx<=4 )
		*dis_r=discrim_right_brace(left1,right1,dy,dx,wide);
	}
return;
}
/*----------------------------------------------------------------------*/
/* study right abris for letter 'r'(bold)   */
static BOOL dis_bold_r(BYTE fun[],INT n)
{
INT i,m=n>>1,s1,s2,f,f_next,s;
			 /*   NEED :              */
for(s1=i=0;i<m;i++)      /*   ��������            */
        s1 += fun[i];    /*   ��������  s1 -> 0   */
for(s2=0;i<n;i++)        /*   ����                */
        s2 += fun[i];    /*   ����      s2 >> s1  */

for(s=i=0,f=fun[0];i<n-1;i++)
	{
	f_next=fun[i+1];
	if( f_next<f)       /* s - number of inc-leap */
		{
		s=0;
		break;
		}
	else if( f_next>f )
		s++;
	f=f_next;
	}

return((s1<<2)>s2 || s>=(n/3) );    /* see pict. + inc right abris */
}

/* fisrt_angle  :         tg2048/2048 is inc current line  */
static INT first_tg(INC_BASE *tab_inc[], INT num_inc, INT tg2048 )
{
INT i;
if( abs(tg2048)>32 )      /* nonzero incline      */
	{
	for(i=0; i<num_inc && tg2048>tab_inc[i]->tg;i++);
	if( i ) i--; /* for i>0 tab_inc[i-1]->tg <tg2048<= tab_inc[i]->tg */
		     /* for i=0 tg2048< tab_inc[0]->tg                    */
	}
else
	{
	for(i=0;i<num_inc;i++)
		if( tab_inc[i]->tg==0 )
			break;
	if( i==num_inc )
		i=0;  /* not exist table for 0 inc */
	}
return(i);
}
/*----------------------------------------------------------------------*/
/* sub incline if exist inc and chenge step of fun-scale : old=1, new=4	*/
/* in :	left[0:dy-1],right[dy-1] - abris arrays, dx - wide of c_comp	*/
/*	tab_angle[0:dy-1] - inc						*/
/* out : corrected left and right abris-arrays				*/
static INT abris_expansion (BYTE left[], BYTE right[],
			    INT dy, INT dx, INT tab_angle[])  {
							// 09.12.1993
INT	i, opt;
INT	k, max_negat_left=0, max_negat_right=0;	// 09.12.1993

opt = MAX (tab_angle[0], tab_angle[dy-1]);	// NB: NO VERY GOOD !!!
for (i=0; i<dy; i++) {	/* dilate (step=4) and shift (inc = tab_angle) */
	if ( left[i]!=0xFF )  {
		/******************************	BEFORE 09.12.1993
		left[i] <<= 2;
		left[i]  = opt + left[i] - tab_angle[i];
		****************************************/
		k = opt + (left [i] << 2) - tab_angle [i];	// 09.12.1993
//////		if (k<0)  k = 0;	// NEGAT CASE (It is possible !!!!!!)
		if (k<0)  if (max_negat_left>k)  max_negat_left = k;
		left [i] = (BYTE)k;
		}
        else
		left[i] = (BYTE)opt;  /* empty string */

	if ( right[i]!=0xFF )  {
		/******************************	BEFORE 09.12.1993
		right[i] = (dx - 1 - right[i]) << 2;
		right[i] = opt + right[i] - tab_angle[i];
		****************************************/
		k = opt + ((dx - 1 - right[i]) << 2) - tab_angle [i];
//////		if (k<0)  k = 0;	// NEGAT CASE (It is possible !!!!!!)
		if (k<0)  if (max_negat_right>k)  max_negat_right = k;
		right [i] = (BYTE)k;
		}
	else
		right[i] = (BYTE)opt; /* empty string  */
	}
/*......................................................................*/
if (max_negat_left)					// 09.12.1993
	for (i=0; i<dy; i++)  left [i] -= (BYTE) max_negat_left;

if (max_negat_right)
	for (i=0; i<dy; i++)  right [i] -= (BYTE) max_negat_right;

return(1);
}
/*----------------------------------------------------------------------*/
static INT discrim_left_brace(BYTE left[],BYTE right[],
			       INT dy,INT dx,INT wid)
{
INT i,ret;
for(i=0;i<dy;i++)
	right[i] = dx-1 - right[i]; /* vert axes simmetry */
if( wid<4 )
	ret = (abris_convexity(left,dy,0)       &&
	       abris_convexity(right,dy,0)      ?0:128);
else
	ret = (abris_convexity(&left[1],(INT)(dy-2),0)&&
	       abris_convexity(&right[1],(INT)(dy-2),0)?0:128);
for(i=0;i<dy;i++)
	right[i] = dx-1 - right[i]; /* restore array      */
return(ret);
}
/*----------------------------------------------------------------------*/
static INT discrim_right_brace(BYTE left[],BYTE right[],
				INT dy,INT dx,INT wid)
{
INT i,ret;
for(i=0;i<dy;i++)
	left[i]  = dx-1 - left[i];  /* vert axes simmetry */
if( wid<4 )
	ret = (abris_convexity(left,dy,0)&&
	       abris_convexity(right,dy,0)?0:128);
else
	ret = (abris_convexity(&left[1],(INT)(dy-2),0)&&
	       abris_convexity(&right[1],(INT)(dy-2),0)?0:128);
for(i=0;i<dy;i++)
	left[i]  = dx-1 - left[i];  /* restore array      */
return(ret);
}
/*----------------------------------------------------------------------*/
//////	make_center_line - see STIC_MAK.C ////////////////////////////////
/*----------------------------------------------------------------------*/
//////	static INT inc_periods[LIMIT_HEIGHT];	### NB: It was HERE !!!!!!
/*----------------------------------------------------------------------*/
			// here was DEBUG_GRAPH
/* make left and right abris and array of center of intervals  */
/* in  : GL - multy-cell                                       */
/* out : center[] - center of intervals                        */
/*       left[],right[] - left and right abris                 */
/*       *dy,*dx - sizes of box                                */
/* return : number of center or 0 (too many intervals)         */
static INT multicell_to_centers(cell *base_c,s_glue *GL,
			    center_interval center[],
			    BYTE left[], BYTE right[],INT *dy, INT *dx)
{
cell *c;
INT i,nc=0,n,mincol,minrow,maxcol,maxrow,ncells=0;

memset(left, 0xFF, *dy );	// first - for CELL only;
memset(right,0xFF, *dy );

if( GL==NULL )
	{
	c=base_c;

	*dx = c->w;
	*dy = c->h;
	if( !(n=cell_to_centers(c,nc,0,0,(INT)(*dx+c->col),center,left,right)) )
		return(0);   /* too many center */
	else 	return(n);   /* normal return   */

	}
/*......................................................................*/
						/* GL != NULL */
c=GL->celist[ncells++];       /* first cell */
mincol=c->col;
minrow=c->row;
maxrow=c->row+c->h;
maxcol=c->col+c->w;

while ((c=GL->celist[ncells++]) != NULL)
	{
	if (c->env == NULL)
		continue;       /* no env */

	if (mincol > c->col) mincol=c->col;
	if (minrow > c->row) minrow=c->row;
	if (maxrow < (c->row+c->h)) maxrow=c->row+c->h;
	if (maxcol < (c->col+c->w)) maxcol=c->col+c->w;
	}
ncells--;

*dx = maxcol-mincol;
*dy = maxrow-minrow;

memset(left, 0xFF, *dy );	// 14.12.1993 second - for MULTICELL !!!!!!
memset(right,0xFF, *dy );

for(i=0;i<ncells;i++)
	{
	if( (c=GL->celist[i])->env==NULL )
		continue;              /* no enviroment   */
	if( !(n=cell_to_centers(c,nc,(INT)(c->row-minrow),
				     (INT)(c->col-mincol),maxcol,
				     center,left,right)) )
			  return(0);   /* too many center */
	else nc=n;                     /* normal return   */
	}

return(nc);
}
/*----------------------------------------------------------------------*/
/* for cell (*c) correct left and right abris and array of centers */
static INT cell_to_centers(cell *c, INT nc,
		    INT off_str,INT off_col,INT max_col,
		    center_interval center[],BYTE left[], BYTE right[])
{
 INT ll,ind,n=nc,off_col_1=off_col-1, wid=max_col-c->col;
 lnhead   *line;
 interval *inter;
 BYTE l,r,h;
 center_interval *p_center=&center[nc];

for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
		(ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
	{
	h=(BYTE)line->h;
	n += h;
	if( n>=LIMIT_CENTER )
		return( 0 );
	for( ind=off_str+line->row,
	     inter=(interval *)((PCHAR)line+sizeof(lnhead));
	     h ;ind++,h--,inter++)     		/* one line     */
		{
		BYTE inter_e = inter->e, inter_l = inter->l;
		r=  wid    - inter_e;	        /* one interval */
		l = inter_e - inter_l+off_col;

		if( right[ind]>r )  right[ind] = r;
		if( left[ind]>l  )  left[ind]  = l;

		p_center->col   = l + inter_e + off_col_1 ;  /* center*2 */
		p_center->row   = ind;
		p_center->len   = inter_l;
		p_center++;
		}
	}

return(n);
}

/* transform list of intervals *lin to array of center_interval          */
/* and make abris-arrays left[],right[]. used in function typ_thin_stick */
/* During EVENTS-pass comps are unknown - we have (lnhead * ) only       */
static INT lnhead_to_centers(lnhead *lin, INT wid,
		    center_interval center[],BYTE left[], BYTE right[])
{
 INT ll,ind,n;
 lnhead   *line;
 interval *inter;
 BYTE l,r,h;
 center_interval *p_center=&center[0];

for (n=0,line=lin; (ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
	{
	h=(BYTE)line->h;
	n += h;
	if( n>=LIMIT_CENTER )
		return( 0 );
	for( ind=line->row,inter=(interval *)((PCHAR)line+sizeof(lnhead));
			h ;ind++,h--,inter++)     /* one line    */
		{
		BYTE inter_e = inter->e, inter_l = inter->l;
		r = wid     - inter_e;            /* one interval */
		l = inter_e - inter_l;

		if( right[ind]>r )  right[ind] = r;
		if( left[ind]>l  )  left[ind]  = l;

		p_center->col   = l + inter_e - 1 ;  /* center*2 */
		p_center->row   = ind;
		p_center->len   = inter_l;
		p_center++;
		}
	}

return(n);
}

/* typ_thin_stick : study thin stick (width<=4) for EVENTS-pass		*/
/*     *lin - list of intervals, dy - height, dx - width of c_comp	*/
/*     return : propabilites letters 'f','r','t'  ( 0b00ttrrff or 0 )	*/
/*              tt,rr,ff = 01(bad),10(good),11(verybad)			*/
			// here was DEBUG_GRAPH
WORD typ_thin_stick(lnhead *lin,INT dy,INT dx)
{
INT wide,opt,left_mode,right_mode,inc_v, nc, num_angles,f_a;
WORD ret=0;
STICK_CHARS left_chars,right_chars;
STICK_SIGNUMS signums;

//////if (dx==3 && dy==18)  {mkm1=mkm2=mkm3=mkm4=mkm5=0x7777;}

			// here was DEBUG_GRAPH
//////	memset(left, 0xFF, dy );
//////	memset(right,0xFF, dy );
	memset(GL_left0, 0xFF, dy );
	memset(GL_right0,0xFF, dy );

//////	if( (nc = lnhead_to_centers(lin,dx,center,left,right))==0 )
	if( (nc = lnhead_to_centers(lin,dx,GL_center,GL_left0,GL_right0))==0 )
		return(0);  /* too many intervals */

if ( num_of_lines (GL_center, nc, dy, GL_hist_int) )
	return(0); /* non stick:>1lines*/

num_angles =   sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0]);
f_a=first_tg(stick_inc_rs, num_angles, nIncline );
num_angles=MIN(LIMIT_OF_ANGLES,
           sizeof(stick_inc_rs)/sizeof(stick_inc_rs[0])-f_a-1);

if( make_center_line(GL_center, (INT)(nc-(GL_center[nc-1].len==1)),
		     GL_left0, GL_right0,dy, dx,
		     &stick_inc_rs[f_a], num_angles, GL_tab_angle,
         0, 0, 1, 0, &wide, GL_hooks, &inc_v, 1) ) // with correct
	return(0); /* abnormal set of ceneters : silmular to (,) or */
		   /* not exist center-line                         */

opt = ((MAX( GL_tab_angle[0], GL_tab_angle[dy-1] ))>>1)<<1;
abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);

set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		(INT)(opt - MIN ( GL_tab_angle[0], GL_tab_angle[dy-1] )),
		0, 0, 0, 0,  0,		// NB: LAST ZERO PAR - inc_num (?????)
		&left_chars, &right_chars, &signums,
		&left_mode, &right_mode);

ret = similar_wide_frt1(&left_chars,&right_chars);

//////if (dx==3 && dy==18)  {mkm1 = dx; mkm2 = dy; mkm3 = ret;}

if (dy>16 && dx<4)
  ret |= 0x0C;  // 09.07.1993 SUPPRESS 'r'

//////if (dx==3 && dy==18)  mkm4 = ret;
if (language == LANG_POLISH &&
    similar_l_stroked(&left_chars,&right_chars,&signums) )
  ret |= 0x300;

return(ret);
}
/*-----------------------------------------------------------------------*/
/* if abris(fun[0:n-1]) is arc curve(w-limit wide) return 1 else return 0  */
/*static INT abris_convexity(BYTE fun[],INT n,INT w)
{
BYTE i,ff,fo,imin,num,minim,eq;

minim = find_minimum( fun, n, &imin );

if( w>0 )
	{
	i=n>>2;
	if( (imin<i || imin>(n-i)) )
		return(0) ;
	}
else if( imin==0 || imin==n-1 )
	return(0);

for(eq=num=0,fo=fun[0],i=1;i<imin;i++)
	{
	ff=fun[i];
	if( ff>fo )
		num++;
        if( ff==fo )
		eq++;
	fo=ff;
	}


for(fo=fun[imin],i=imin+1;i<n;i++)
	{
	ff=fun[i];
	if( ff<fo )
		num++;
	if( ff==fo )
		eq++;
	fo=ff;
	}
if( w>0 )
	{
	w>>=1;
	if( !w ) w=2;
	if( (fun[0]-minim + fun[n-1]-minim)<=2*w )
		return(0);
	}

if( w>0 )
	return( num==0 && eq*4<n*3 );
else
	return (num<3) ;
}*/
/*----------------------------------------------------------------------*/
/* simple rule for defined number of sticks in c_comp   */
static INT num_of_lines(center_interval center[],INT nc,INT dy,INT hist[])
{
INT n,*r,*e,ret;
center_interval *p_center,*p_end=&center[nc];

memset(hist,0,dy*sizeof(hist[0]));
for(p_center=&center[0];p_center<p_end;p_center++)
	hist[ p_center->row ] ++;     /* make hist of numbers of */
				      /* intervals in any row    */
if( nc>dy  )
{     /* too many intervals */
for(n=0,r=&hist[1],e=&hist[dy-2];r<e;r++)
	if(*r>1)
		n++;
num_lines = n*20; /* OLEG:03.03.94 */
dy -= 2;
ret = (n<<2>(dy-2)*3);      /* !!!???!!! */
if( ret==0 )
	{    /* normal number of intervals */
	INT i,j;
	for(i=1;i<dy&&hist[i]==1;i++); /* one interval in row     */
	for(j=i;i<dy&&hist[i]>=2;i++); /* more 1 intervals        */
	if( (i-j)<<1>dy )
		ret=1;                  /* too many central 2-row */
	}
dy += 2;
}
else ret = 0; /* nc<=dy */
return(ret);  /* 0 - OK, 1 - bad c_comp (>1 line) */
}
/*----------------------------------------------------------------------*/
