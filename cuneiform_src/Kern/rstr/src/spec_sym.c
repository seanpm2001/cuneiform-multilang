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



/*  ������������������������������������������������������������������ */
/*  ��                                                              �� */
/*  ��  SPEC_SYM.C  recognize special symbols (bullet,*)  04.11.92. �� */
/*  ��                                                              �� */
/*  ������������������������������������������������������������������ */


#include <stdlib.h>
#include <string.h>

#include "nt_types.h"
#include "struct.h"
#include "func.h"
#include "status.h"
#include "msgerr.h"
#include "ligas.h"
#include "tuner.h"
#include "minmax.h"

/* incline of string NOT USED !!!                           */
/* BULLET hav one line ONLY, otherwise non study c_comp !!!   */

/* results compare abris-arrays with curves and vert lines  */
/* -1 - non compare, 0 - not similar, 1 - similar           */
static INT flag_abris_curve_left, flag_abris_curve_right,
	   flag_abris_line_left, flag_abris_line_right;

static INT comp_is_bullet(cell *c );
static void init_bullet_functions(void);
static BOOL comp_is_triangle(BYTE left[],BYTE right[],INT h,INT w);
static BOOL comp_is_rectangle(BYTE left[],BYTE right[],INT h);
static BOOL comp_is_ellipse(BYTE left[],BYTE right[],INT n,INT w);
static BOOL abris_line(BYTE fun[],INT n, INT denominator);
static INT place_fun_limit(BYTE fun[],INT n);
static INT place_fun(BYTE fun[],INT n);
static INT limit_place(BYTE left[],BYTE right[],INT h);
static void find_abris(cell *c,INT wid,BYTE left[],BYTE right[]);
static INT one_line_in_comp(cell *c);
static BOOL comp_glue(cell *c);
static BOOL comp_projections_overlay(cell *c1, cell *c2);
static INT context_next(cell *c);
static INT context_prev(cell *c);
static BOOL context_bullet(cell *c);
static INT conv_size_capital_let(BYTE let,INT wide);

static BOOL comp_is_star5(cell *c);
static BOOL pit(BYTE fun[],INT h);
static BOOL concave_line(BYTE fun[],INT h);
static BOOL ray_head_and_hands(BYTE left[],BYTE right[],INT h);
static BOOL ray_head_and_feets(cell *c);

#define NON_BULLET       0
#define BAD_RECTANGLE    1
#define RECTANGLE        2
#define ELLIPSE          3
#define TRIANGLE         4
#define LIMIT_HEIGHT     128
#define GOOD_CODE        254
#define BAD_CODE         126
#define CODE_BULLET      liga_bull

extern BYTE fax1x2;
#define INC_FAX( val )            \
	if( fax1x2 ) val += 2;
#define DEC_FAX( val )            \
	if( fax1x2 ) val -= 2;

/* is letter bullet ? */
INT chkbullet( BYTE let )
{
return( let==liga_bull);
}

/* bullet                                                            */
/*   ��� new_vers (file S_TOOLS.C)                                   */
/*   ��� comp_is_bullet                                              */
/*         ��� comp_glue                                             */
/*	   �	   ���  comp_projections_overlay                     */
/*	   ��� one_line_in_comp                                      */
/*	   ��� context_bullet                                        */
/*	   �	   ���  context_prev  �Ŀ                            */
/*	   �	   ���  context_netx  �Ĵ                            */
/*	   �			        ��� conv_size_capital_let    */
/*	   ��� find_abris                                            */
/*	   ��� filtr_bullet (file S_TOOLS.C)                         */
/*	   ��� limit_place                                           */
/*	   ��� get_b_lines  (A.Leman)                                */
/*	   ��� init_bullet_functions                                 */
/*	   ��� comp_is_rectangle �������Ŀ                           */
/*	   ��� comp_is_ellipse   ��Ŀ    �                           */
/*	   ��� comp_is_triangle  Ŀ �    �                           */
/*	        �   abris_curve �����    �                           */
/*	        ��� abris_line  ����������                           */

/*	star                                                         */
/*	  ��� new_vers (file S_TOOLS.C)                              */
/*        ��� comp_is_star5                                          */
/*        	��� comp_glue                                        */
/*              �	��� comp_proections_overlay                  */
/*              ��� find_abris                                       */
/*              ��� ray_head_and_feets                               */
/*              ��� ray_head_and_hands                               */
/*              	��� pit                                      */
/*                           ��� find_minimum  (file S_TOOLS.C)      */
/*                           ��� concave_line                        */
/*					��� place_fun                */

/*  for any cell in string-list set bullet code if */
/*  similar RECTANGLE,ELLIPSE,TRIANGLE             */
void bullet(void)
{
cell *c=cell_f();

while( (c=c->next) != cell_l() )
	{
	switch( comp_is_bullet(c)  )
	   {
	   case BAD_RECTANGLE  :
/*		new_vers(c,CODE_BULLET,BAD_CODE); */
/*              NOT USED in next passes           */
		break;
	   case RECTANGLE  :
		new_vers(c,CODE_BULLET,GOOD_CODE);
		break;
	   case ELLIPSE  :
		new_vers(c,CODE_BULLET,GOOD_CODE);
		break;
	   case TRIANGLE :
		new_vers(c,CODE_BULLET,GOOD_CODE);
		break;
	   default :
		break;
	   }

	}
return;
}

static void init_bullet_functions(void)
{
flag_abris_line_left  = flag_abris_line_right  =
flag_abris_curve_left = flag_abris_curve_right = -1;
return;
}

/*  for any cell in string-list set    */
/* star code if similar 5-ray star     */
void star(void)
{
cell *c=cell_f();
while( (c=c->next) != cell_l() )
	{
	if( c->nvers==0 || !(c->flg&(c_f_let|c_f_bad)) ||
	    c->vers[0].let=='1' || c->vers[0].let=='t')
		{                    /*   �     or     dust       */
		if( comp_is_star5(c)==TRUE )
			new_vers(c,'*', GOOD_CODE );
		}
	//AK! c/g �������� ������� - c->next �� NULL
	if (c->next == NULL )
		AKCheckChain();                             //break;

	}
return;
}

static INT comp_is_bullet(cell *c)
{

BYTE left[LIMIT_HEIGHT],right[LIMIT_HEIGHT];
INT height=c->h, width=c->w;
BOOL i1=FALSE, i2=FALSE, i3=FALSE;
INT place_limit, place=(height-2)*width;
B_LINES bl;
INT center_bl,center_bul,top,bot;

if( height<6 || width<6     ) return(0);      /* no bullet : small c_comp */
if( comp_glue(c)==TRUE      ) return(0);      /* no bullet : c_comp glue  */
if( one_line_in_comp(c)!=1  ) return(0);      /* no bullet : >1 lines   */
if( !context_bullet(c)      ) return(0);      /* no bullet : near c_comp  */
if( c->nvers==1 && c->vers[0].let=='*' &&
    c->vers[0].prob==254    ) return(0);      /* it is my 5-ray star    */

memset(left,0xFF,height);
memset(right,0xFF,height);

find_abris(c, width, left, right);
if( left[height-1]+right[height-1]+1==width )
	height--;                             /* shave upper pimple     */
filtr_bullet( left, height);
filtr_bullet(right, height);
#ifdef INTERSEPTOR
filtr_shave( left, height);
filtr_shave(right, height);
{
INT i;
for(i=0;i<height;i++)
  {
  if( left[i]==0 )
    left[i]=1;
   if( right[i]==0 )
    right[i]=1;
  }
}
#endif
place_limit=limit_place(left,right,height);

top = c->row; bot = top + height;
get_b_lines(c,&bl);
center_bl = (bl.b3+bl.b2)/2;
center_bul = top+c->h/2;

if (bot<center_bl)
 return(0);

if( center_bul>center_bl &&
    (place_limit>height/2 || place_limit>=3 && bl.b3-bot<=2) )
	{
        /* 1. downer area                    */
	if(height<=bl.ps/2+1 || width<=bl.ps/2+1 )
			      return(0);      /* no bullet : small c_comp */
	}

if( width<8 && height<8 || MAX(width,height)<=8 && MIN(width,height)>=6 )
	{       /* too small  c_comp */
	if( abs(width-height)>2 )
		return(0);     /* no bullet : c_comp hav bad proportions   */
	/* 2.     no square                 */
	if( top<bl.b2 || bot>bl.b3 )
	/* 3. small c_comp overlay 2 base line */
	/* 4. small c_comp overlay 3 base line */
		return(0);     /* no bullet : c_comp overlay base line 2,3 */
	if( top>=bl.bm || bot<=bl.bm )
	/* 5. c_comp not overlay central base line */
		return(0);     /* no bullet : upper or downer small c_comp */
	}

else
	{ /* nornal sizes        */
#define NUMERV 9
#define DENOMV 16         /* vertical treshold NUMERV/DENOMV */
#define NUMERH 4
#define DENOMH 5          /* horizontal treshold NUMERH/DENOMH */

	INC_FAX(height);
	if(width*DENOMV < NUMERV*height || height*DENOMH < NUMERH*width ||
		place_limit*4>=place )
		return(0);      /* no bullet : too big places of limit or */
				/*   bad proportionals                    */
	DEC_FAX(height);

#undef NUMERV
#undef DENOMV
#undef NUMERH
#undef DENOMH
	}

init_bullet_functions();
i2=comp_is_ellipse(left,right,height,width);
if( i2==FALSE )
	i1=comp_is_rectangle(left,right,height);
if( i1==FALSE )
	i3=comp_is_triangle(left,right,height,width);

if( i3==TRUE )
		return(TRIANGLE);
if( i2==TRUE )
		return(ELLIPSE);
if( i1==TRUE )
	{
	INT corr= ( place_limit>10 ? 10 : 5) ;

	if( place_limit < place/corr )
		return(RECTANGLE);
	else
		return(BAD_RECTANGLE);
	}


return(NON_BULLET);
}

static BOOL context_bullet(cell *c)
{
INT r1  = context_prev(c), r2 = context_next(c) ;

return ( r1==2 || r1>=1 && r2>=1 );
/* return TRUE if not exist near next or prev cell */
}

/* return  2 - prev cell non exist  */
/*         1 - prev cell is far     */
/*         0 - prev cell is near    */
static INT context_prev(cell *c)
{
cell *cc=c->prev;
INT k=16,d=9,scc,ww;

if( cc==cell_f() )
	return(2);              /* not exist left c_comp   */

ww = ( cc->vers[0].let==bad_char )?MIN(cc->w,cc->h):cc->w;
if(   (c->col - cc->col - cc->w)*2 < conv_size_capital_let(cc->vers[0].let,ww) )
	return(0);  	        /* near components       */

while( cc!=cell_f() && cc->flg!=c_f_let )
	cc = cc->prev;          /* find prev letter-c_comp */

if( cc!=NULL && c->h < conv_size_capital_let(cc->vers[0].let,cc->h)/2 )
	{                       /* high c_comp             */
	if( cc->row + cc->h - c->row - c->h >= cc->h/4+1 )
		{k=4;d=1;}      /* upper zone            */
        scc = conv_size_capital_let(cc->vers[0].let,cc->w) *
	      conv_size_capital_let(cc->vers[0].let,cc->h);
	if( c->w * c->h * k < scc * d )
		return(0); 	/* small component       */
	}

return(1);                      /* good tandem           */
}

/* return  2 - next cell non exist    */
/*         1 - next cell is far       */
/*         0 - next cell is near      */
static INT context_next(cell *c)
{
cell *cc=c->next;
INT  k=16,d=9,scc,ww;

if( cc==cell_l() )
	return(2);              /* not exist right c_comp  */

ww = ( cc->vers[0].let==bad_char )?MIN(cc->w,cc->h):cc->w;
if( (cc->col - c->col - c->w)*2 < conv_size_capital_let(cc->vers[0].let,ww) )
	return(0);  	        /* near components       */

while( cc!=cell_l() && cc->flg!=c_f_let )
	cc = cc->next;          /* find next letter-c_comp */

if( cc!=cell_l() && c->h < conv_size_capital_let(cc->vers[0].let,cc->h)/2 )
	{                       /* high c_comp             */
	if( cc->row + cc->h - c->row - c->h >= cc->h/4+1 )
		{k=4; d=1;}     /* upper zone            */
	scc = conv_size_capital_let(cc->vers[0].let,cc->w) *
	      conv_size_capital_let(cc->vers[0].let,cc->h);
	if( c->w * c->h * k < scc * d )
		return(0); 	/* small  component      */
	}

return(1);                      /* good tandem           */
}

/* decrease size for capital letter */
static INT conv_size_capital_let(BYTE let,INT wide)
{
return( let<91 ? (wide*2)/3 : wide);
}

static BOOL comp_glue(cell *c)
{
cell *cc=c->next;
if( cc!=cell_l() )
	{
	if( comp_projections_overlay(c, cc) )
		return( TRUE );
	}
cc=c->prev;
if( cc!=cell_f() )
	{
	if( comp_projections_overlay(c, cc) )
		return( TRUE );
	}

return(FALSE);
}

static BOOL comp_projections_overlay(cell *c1, cell *c2)
{
INT b1 = c1->col, b2 = c2->col;
INT e1 = b1 + c1->w, e2 = b2 + c2->w;
if( b1<=b2 && b2<=e1 )return(TRUE);
if( b2<=b1 && b1<=e2 )return(TRUE);
return( FALSE );
}

static BOOL comp_is_ellipse(BYTE left[],BYTE right[],INT h,INT w)
{
if( flag_abris_curve_left<0 )
	flag_abris_curve_left = abris_curve(left,h,w);
if( flag_abris_curve_right<0 )
	flag_abris_curve_right = abris_curve(right,h,w);
return flag_abris_curve_left && flag_abris_curve_right ;
}

static BOOL comp_is_rectangle(BYTE left[],BYTE right[],INT h)
{
if( flag_abris_line_left<0 )
	flag_abris_line_left = abris_line(left,h,4);
if( flag_abris_line_right<0 )
	flag_abris_line_right = abris_line(right,h,4);
if( flag_abris_line_left && !flag_abris_line_right )
    {
    flag_abris_line_right= abris_line(right,h,3);
    }
return flag_abris_line_left && flag_abris_line_right ;
}

static BOOL comp_is_triangle(BYTE left[],BYTE right[],INT h,INT w)
{
if( flag_abris_curve_left<0 )
	flag_abris_curve_left = abris_curve(left,h,w);
if( flag_abris_curve_right<0 )
	flag_abris_curve_right = abris_curve(right,h,w);
if( flag_abris_line_left<0 )
	flag_abris_line_left = abris_line(left,h,4);
if( flag_abris_line_right<0 )
	flag_abris_line_right = abris_line(right,h,4);
return flag_abris_curve_left && flag_abris_line_right ||
       flag_abris_line_left && flag_abris_curve_right ;
}

/* limit_place : sum of limit points in box (size h*w) */
/*               left(right) - array left(right) abris */
static INT limit_place(BYTE left[],BYTE right[],INT h)
{
return ( place_fun_limit(left,h) + place_fun_limit(right,h) ) ;
}

static INT place_fun_limit(BYTE fun[],INT n )
{
INT i,s,vert_line=1,nn=(n-2)>>1; /* nn-1/2 heigh of c_comp             */
for(s=0,i=1;i<n-1;i++)           /* first and last lines skipped     */
	{
	s+= fun[i];
	if( fun[i]>1 ) vert_line = 0;  /*  not vert line             */
	}
if( vert_line )                        /* fun[] is vert line         */
	{
	if( s>nn )                     /* place pits > place pimples */
		s = n - 2 - s;         /* pits , not pimples         */
	}
return( s );
}


static INT place_fun(BYTE fun[],INT n)
{
INT i,s;
for(s=i=0;i<n;i++)
	s+= fun[i];
return( s );
}

BOOL abris_curve(BYTE fun[],INT n,INT w)
{
BYTE i=n>>2,minim,ff,fo,imin;

minim = (BYTE)find_minimum( fun, n, &imin );

if( imin<i || imin>(n-i) )
	return(FALSE) ;     /* max of fun belong [n/4, 3n/4] */

for(fo=fun[0],i=1;i<imin;i++)
	{
	ff=fun[i];
	if( ff>fo )
		return(FALSE);          /* fun must decrease */
	fo=ff;
	}
for(fo=fun[imin],i=imin+1;i<n;i++)
	{
	ff=fun[i];
	if( ff<fo )
		return(FALSE);          /* fun must increase */
	fo=ff;
	}
w=(w>>2)-((fax1x2)?2:1);
if( (fun[0]-minim)<w || (fun[n-1]-minim)<w )
	return(FALSE);                  /* flat curve        */
return(TRUE);                           /* similar arc       */
}

static BOOL abris_line(BYTE fun[],INT n,INT denominator)
{
#define NUM_OF_LEAP (n/denominator)
INT i,hist=0;
n -= 2;
for( i=0; i<n; i++)
	if( fun[i+1]>1 )
		hist++;       /* number of long jumps in abris  */
return( hist<= NUM_OF_LEAP ); /* TRUE if too few jumps in abris */
#undef NUM_OF_LEAP
}


static INT one_line_in_comp(cell *c)
{
 BYTE fill[LIMIT_HEIGHT];
 lnhead *line;
 interval *inter;
 INT ind,i,ll,h,max_h=c->h,num_int;

 if (!tsimple(c))
    return 0;
 if( c->env->nl==1 )
	return(1);

 memset(fill, 0, max_h-- );

 num_int = 0;                        /* number of rows with 1 interval    */
 for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
	(ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
#ifdef INTERSEPTOR
  if( line->h>1 )
#endif
 for( h=line->h,i=0, inter=(interval *)((PCHAR)line+sizeof(lnhead));
	i<h;i++,inter++)             /* one line                          */
	{
	ind = line->row + i;         /* skipped first and last lines      */
	if( ind>0 && ind<max_h )
		{
		if( inter->l > 1 )   /* long inteval                      */
			{
		if( fill[ind] )      /* non one interval in line          */
			return(0);   /* two long intervals                */
		else                 /* empty line                        */
				{
				fill[ind] = 1;
				num_int++;
				}
			}
		}
	}

return( num_int==max_h-1 );          /* TRUE if all rows good, else FALSE */
}

static void find_abris(cell *c,INT wid,BYTE left[],BYTE right[])
{
 lnhead *line;
 INT i,ll,ind;
 interval *inter;
 BYTE l,r,h;

 for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
			(ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
 if( (h=(BYTE)line->h)>1 )                         /* long line        */
 for( i=0, inter=(interval *)((PCHAR)line+sizeof(lnhead));i<h;i++,inter++)
	{
	r= wid - inter->e;                   /* current interval */
	l = inter->e - inter->l;
	ind = line->row + i;
	if( right[ind]>r ) right[ind] = r;
	if( left[ind]>l  ) left[ind]  = l;
	}
return ;
}

static BOOL comp_is_star5(cell *c)
{
B_LINES bl;
BYTE left[LIMIT_HEIGHT],right[LIMIT_HEIGHT];
INT height=c->h, width=c->w ;

if( height<6 || width<6 ) return(0);      /* no star : small c_comp      */
#define NUMER 4
#define DENOM 5                           /* treshold NUMER/DENOM      */
#define NUM   2
#define DEN   3                           /* treshold NUM/DEN          */

if(width*DEN < NUM*height || (height+((fax1x2)?2:0))*DENOM < NUMER*width )
			  return(0);      /* no star : bad proportions */
#undef NUMER
#undef DENOM
#undef NUM
#undef DEN

get_b_lines(c,&bl);
if (c->row>=bl.bm) return(0);             /* no star : low position */
if( comp_glue(c)==TRUE  ) return(0);      /* no star : c_comp glue       */

memset(left,0xFF,height);
memset(right,0xFF,height);

find_abris(c, width, left, right);

if( !ray_head_and_feets( c ) )
	return(FALSE);
if( !ray_head_and_hands(  left, right, height) )
	return(FALSE);

return( TRUE );
}


static BOOL ray_head_and_hands(BYTE left[],BYTE right[],INT h)
{
return pit(left,h) & pit(right,h) ;
}

static BOOL ray_head_and_feets(cell *c )
{
BYTE work[LIMIT_HEIGHT];    /* number of intervals in row              */
INT height=c->h;
INT h_c=height/5;           /* height of ray                           */
INT skip_h =  height - h_c;
lnhead *line;
interval *inter;
INT ll,h,i,l;

memset(work, 0, height);

for (line=(lnhead *)((PCHAR)(c->env)+c->env->lines+sizeof(INT));
			(ll=line->lth)>0; line=(lnhead *)((PCHAR)line+ll))
 if( (h=line->h)>0 )        /*                  long line              */
 for( i=0, inter=(interval *)((PCHAR)line+sizeof(lnhead));
			i<h;i++,inter++)
		work[line->row+i] ++;

for( ll=0, i = height-( (work[height-1]!=2) ? ((fax1x2)?3:2) : 1 ) ;
 i>=0 && work[i]==2 ; i--, ll++);
			    /* ll - number of 2-interval downer lines  */
			    /* downer area : 2 feets                   */

if( ll>=3*(height>>2) ) return(FALSE);

for( l=0;i>=0;i--)
	l += (work[i]==1);  /* l - number of 1-interval upper lines     */

skip_h = height - ll;       /* skip_h - height of upper area            */
if( work[height-1]!=2 ) skip_h--;

for( i = 1 ; i<height && work[i]==1 ; i++);

h_c = ( h_c>3 ? h_c/3 : 1); /* h_c - number of rows with 2 intervals    */
if( h_c==1 && height<11 && (work[height-1]==2 || work[height-2]==2) )
	h_c=0; /* for small stars : exist more one row with 2 intervals */

return( (i>=height/4) && (l>skip_h/2) && (ll>h_c-((fax1x2)?1:0)) );
}

static BOOL pit(BYTE fun[],INT h)
{
BYTE imin, minim;
INT wide;

minim = (BYTE)find_minimum( fun, h,  &imin );
while( imin>0 && fun[imin]==minim ) imin--;
imin++;                                       /* find position horiz ray  */

wide = fun[0] - minim;                        /* find size of horiz ray   */
if( wide<h/3 )
	return(FALSE);                        /* too short ray            */
if( imin<h/4 || imin>(h*3)/4 )
	return( FALSE );                      /* too heigh or too low ray */

return( concave_line( fun, (INT)(imin+1) ) );        /* concave upper area       */
}

/* fun[0] = upper point of head; fun[n-1] = limits point of hand-ray */
static BOOL concave_line(BYTE fun[],INT n)
{
INT st = (fun[0]+fun[n-1])*n/2;        /* place of trapeze         */
INT sf = place_fun(fun,n);             /* integral of function     */
return( (n<5 || fax1x2)? (sf >= st) : (sf > st) ); /* fun is up convexity      */
}
