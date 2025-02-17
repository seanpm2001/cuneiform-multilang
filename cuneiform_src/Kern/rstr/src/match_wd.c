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
#include <stdio.h>
#include "nt_types.h"
#include "struct.h"
#include "ligas.h"
#include "func.h"
#include "cut_glue.h"
#include "match_wd.h"
#include "rstr.h"
#include "dpuma.h"
#include "wrgb.h"
#include "minmax.h"

extern BYTE * letters_pidx_table;
INT get_cuts (cell *C, struct cut_elm *list, INT nmax );
INT recogij(cell *C, cell **org_cells, INT N, BYTE cut_fl,
                   BYTE cut_fine,INT inc, INT *roi, BYTE *gvar,
                   SVERS *vers, INT *width );

extern BYTE mwInput[];
extern B_LINES my_bases;

#define LINE_WIDTH 8000            //line length in pixels
#define LINE_HEIGHT RASTER_HEIGHT
#define MAXINT16 0x7FFF
#define MAXWORD32 0xFFFFFFFF
#define RELY 220

#define middle(C)  ( (C)->r_col -1 + (((C)->w+1)>>1) )
#define lefter(C,x)  ( middle(C) < (x) )

#define debug_on ( db_status && snap_activity('a') )

//cut point - graph node in dynamic matching
typedef struct tagCutPoint
{
  WORD    x;  //position
  BYTE    dh;  //cut lehgth
  BYTE     h;  //cut begin from raster bottom
  BYTE   var;  //cut point type
  BYTE     n;  //number of components
  BYTE   flg;  //for any purpose
} CutPoint;

typedef struct tagGraphNode
{
  WORD   prev;  //previous node in this path
  Weight weight;   //accrued weight to current node
  DWORD  monitors; //rates of monitoring letters
// SVERS  vers;  //versions for segment from prev to current
} GraphNode;

typedef struct tagStrRaster   //���� ��ப�
{
  LONG w;       //�ਭ�
  LONG h;       //����
  LONG top;     //��ப� ������ ���孥�� 㣫�
  LONG left;    //�⮫���  -""-
  BYTE pict[LINE_WIDTH*LINE_HEIGHT/8];  //����
} StrRaster;

typedef struct tagCutAdd
{
  INT top;
  INT left;
  INT right;
  INT bottom;
  LONG nbig;
  SVERS vers; //��� ���ᨨ
} CutAdd;

static void set_param (MatchWordPar *param);
static BOOL make_cell_string(CSTR_rast wb, CSTR_rast we);
static void calc_bl();
static void make_alphabet(BYTE *word);
static INT compose_inc(cell *wb, cell *we);
static BOOL  make_str_raster(cell *wb, cell *we, StrRaster *raster);
static void comptorast(c_comp *cp1, StrRaster *raster);
static void inttorast( StrRaster *r, LONG h, LONG end, LONG lth);
static BOOL calc_cut_points(cell *wb, cell *we, INT rastlc, INT rastdr);
static void fict_sect(CutPoint *sec, CutAdd *sec_add, GraphNode *node, INT x, INT px);
static void close_ds(CutPoint *sec, CutAdd *sec_add, GraphNode *node, INT x, INT px);
static void cor_sect(cell *C, CutPoint *cut, INT left, INT down);
static INT get_points (cell *C, CutPoint *listn, INT nmax );
static void save_alpha_vers(cell *C, SVERS *svers);
static INT cut_by_alpha(INT n, version vers[]);
static Weight match(BYTE *word);
static BOOL test_set(LONG prev, LONG h0, BYTE nlet, LONG tol, BOOL rerecog, LONG *imax, LONG *pmax);
static LONG inc(CutPoint **cutp, LONG i, LONG ie, LONG set);
static LONG dec(CutPoint **cutp, LONG i, LONG ie, LONG set);
static LONG add_sect(LONG il, LONG ir, BYTE nlet, BOOL rerecog, BYTE *p);
static version *find_in_vers(SVERS *svers, BYTE let);
static BOOL equal(BYTE let1, BYTE let2);
static LONG select_cells(LONG il, LONG ir, BYTE cut_fl, cell **cells);
static void set_bad_vers(SVERS *c);
static Weight add_weight(Weight *wp, BYTE ro, BYTE nlet);
static void add_monitors(LONG il, LONG ir, BYTE nlet, BYTE pb);

static void mw_show_rast();
static void show_layer(BYTE let, LONG prev, LONG imax);


StrRaster str_raster;
#define MAX_CUT_POINT LINE_WIDTH/8
static CutPoint cut_list[MAX_CUT_POINT];
static GraphNode   layer1[MAX_CUT_POINT],layer2[MAX_CUT_POINT],*cur_layer=layer1,*prev_layer=layer2;
static LONG ncut;
static BYTE *templ,alpha[256]={0};
static INT com_inc;
struct dp_vers_struct vers_pool;   //��� ���ᨩ dp
#define vers_list (vers_pool.node)
static LONG min_cut_width=0;  //�� �ਭ� > min_cut_width ����� १���
static LONG wmin;
static MatchWordPar *param;

static struct
{
  BYTE d[4];
  BYTE m[3];
  BYTE u[4];
  BYTE z[3];
  BYTE a[3];
  BYTE e2[3];
  BYTE E2[3];
  BYTE ii[3];
} eq_list=
{
  {(BYTE)'�',r_cu_d,r_cu_g,0},    //   r_cu_d  0xf0     cursiv d tail up
                                  //   r_cu_g  0xf1     cursiv d tail down
  {(BYTE)'�',r_cu_m,0},           //   r_cu_m  0xf5     cursiv t
  {(BYTE)'�',(BYTE)'�',r_cu_u,0}, //   r_cu_u  0xf7     cursiv ee
  {(BYTE)'�',r_cu_z,0},           //   r_cu_z  0xf8     cursiv g
  {(BYTE)'�',r_cu_a,0},           //   r_cu_a  0xfd     a
  {(BYTE)'�',r_e_2dot,0},         //   r_e_2dot   0xC0  russian e ..  = 192
  {(BYTE)'�',r_EE_2dot,0},        //   r_EE_2dot  0xC8  russian E ..  = 200
  {(BYTE)'�',(BYTE)'�',0}
};

#define EQU_d   0
#define EQU_t   EQU_d+sizeof(eq_list.d)
#define EQU_i   EQU_t+sizeof(eq_list.m)
#define EQU_g   EQU_i+sizeof(eq_list.u)
#define EQU_a   EQU_g+sizeof(eq_list.z)
#define EQU_e2  EQU_a+sizeof(eq_list.a)
#define EQU_E2  EQU_e2+sizeof(eq_list.e2)
#define EQU_ii  EQU_E2+sizeof(eq_list.E2)

CHAR eq_let[256]=
  {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 0
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 1
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 2
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 3
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 4
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 5
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 6
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 7
  -1, -1, -1, -1, -1,
                     EQU_E2,
                          -1, -1,
                                EQU_ii,
                                     EQU_ii,
                                          -1, -1, -1, -1, -1, -1,   // 8
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // 9
  EQU_a,
      -1, -1,
             EQU_g,
                 EQU_d,
                     EQU_e2,
                          -1, -1,
                                 EQU_i,
                                      EQU_i,
                                          -1, -1, -1, -1, -1, -1,   // A
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // B
  EQU_e2,
      -1, -1, -1, -1, -1, -1, -1,
                                 EQU_E2,
                                      -1, -1, -1, -1, -1, -1, -1,   // C
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // D
  -1, -1,
         EQU_t,
              -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,   // E
  EQU_d,
     EQU_d,
          -1, -1, -1,
                     EQU_t,
                          -1,
                             EQU_i,
                                  EQU_g,
                                      -1, -1, -1, -1,
                                                     EQU_a,
                                                          -1, -1    // F
  };
/*
//stubs

static void  CBS_Init() {}
static void  CBS_Calc() {}
static void  letToDust(BOOL exact) {}
static BOOL LINDefineLines(cell *f, cell *l)  { return FALSE; }
static BOOL SetBases ()   { return FALSE; }
static void StrRec(BYTE *alphabet, LONG mode) {}

LONG match_word(CSTR_rast wb, CSTR_rast we, BYTE *word, MatchWordPar *param)
//match word to line fragment from wb to we (not include)
//returns weight ( < 0, if error occur
{
  set_param(param);
  alphabet=make_alphabet(word);
  if (*alphabet==0)
    return -1;

  if (!make_cell_string(wb,we))
    return -1;

  if (!param->bases)
  {
    CBS_Init(); // init base lines
    CBS_Calc(); // calc base lines by big components
    letToDust(FALSE);
  }

  StrRec(alphabet,0);

  if (!param->bases)
  {
    BOOL linOK=FALSE;
    if (LINDefineLines(cell_f(),cell_l()))
      linOK=SetBases();
    if (linOK)
    {
      letToDust(TRUE);
      StrRec(alphabet,1);
    }
  }

  if (!make_str_raster(wb,we,str_raster))
    return -1;

  if (!calc_cut_points(wb,we))
    return -1;
  return match(word);
}

static BOOL make_cell_string(CSTR_rast wb, CSTR_rast we)
{
  return FALSE;
}

static void calc_bl()
{
  CBS.Init(); // init base lines
  CBS.Calc(); // calc base lines by big components
  letToDust(FALSE);
  linOK=FALSE;
  if (LINDefineLines(Cell_f(),Cell_l()))
    linOK=SetBases();
  if (linOK)
    letToDust(TRUE);
}
*/



static void set_param (MatchWordPar *par)
{
  B_LINES  *bl=par->bases;
  param=par;
  if (bl)  my_bases = *bl;
//  else     get_b_lines(NULL,&my_bases);
//  wmin=my_bases.ps/5;
  min_cut_width = param->cut_width;
}

BOOL match_word_prepare(CSTR_line ln, BYTE *alpha, MatchWordPar *param);

RSTR_FUNC(Bool32) RSTR_recog_one_word(CSTR_line ln, Word8 *word, Int8 *points,Word8 *res)
{
Weight      w;
MatchWordPar param={0};
int rc;

param.monitors=*((DWORD*)points);
param.p2_active=1;  //call while p2 pass
param.language=3;	// LANG_RUSSIAN

if((rc=setjmp(Control_Point())) != 0)
  return FALSE;

w=match_string(ln, word, &param);
if (w.meas<0)
  return FALSE;
memcpy(res,&w,sizeof(Weight));
res += sizeof(Weight);
memcpy(res,&param.monitors,4);
return TRUE;
}

Weight match_string(CSTR_line ln, BYTE *str, MatchWordPar *param)
{
  Weight m1={-1,-1};
  cell *wb,*we;
  B_LINES bas;

  set_param(param);

  make_alphabet(str);

  if (!match_word_prepare(ln,alpha,param))
    return m1;

  get_b_lines(NULL,&my_bases);
  wmin=my_bases.ps/5;

//���塞 �業�� � ���⠢�塞 �窨 ��� 'i'
  adjust_3x5(FALSE);

  wb=cell_f()->next;  we=cell_l();
  com_inc=compose_inc(wb,we);
  bas=my_bases;

  if (!make_str_raster(wb,we,&str_raster))
    return m1;

  if (!glue_overlap(wb->prev,we))
    return m1;

  wb=cell_f()->next;
  if (!calc_cut_points(wb,we,(INT)str_raster.left,(INT)(str_raster.top+str_raster.h-1)))
    return m1;

  if (debug_on)  mw_show_rast();

  return match(str);
}

Weight match_cell_word(cell *wb, cell *we, BYTE *word, MatchWordPar *param)
{
  Weight m1={-1,-1};
  set_param(param);
  com_inc=compose_inc(wb,we);
  get_b_lines(NULL,&my_bases);
  wmin=my_bases.ps/5;

  make_alphabet(word);

  if (!make_str_raster(wb,we,&str_raster))
    return m1;

  if (!glue_overlap(wb->prev,we))
    return m1;

  if (!calc_cut_points(wb,we,(INT)str_raster.left,(INT)(str_raster.top+str_raster.h-1)))
    return m1;

  if (debug_on)  mw_show_rast();

  return match(word);
}
/*
static BYTE *make_alphabet(BYTE *word)
{
  static BYTE alphabet[257];
  BYTE *a=alphabet,*w;

  *a=0;
  do
  {
    LONG let=256;
    for (w=word; *w; w++)
      if (*w>*a && *w<let)  let=*w;
    *(++a)=(BYTE)(let & 0xFF);
  }
  while (*a);
  return alphabet+1;
}
*/
static void make_alphabet(BYTE *word)
{
  BYTE *w,*l;

  memset(alpha,0,256);

  // 17.07.2001 E.P.
  if (is_baltic_language(param->language))
	  return;

  // 21.05.2002 E.P.
  if (is_turkish_language(param->language))
	  return;

  for (w=word; *w; w++)
  {
    LONG add=eq_let[*w];
    if (add<0)
      alpha[*w]=1;
    else
      for (l=&eq_list.d[add]; *l; l++)
		  alpha[*l]=1;
  }
}

static INT compose_inc(cell *wb, cell *we)
{
  cell *cells[LINE_WIDTH/8];
  INT n;

  for (n=0; wb != we && n<LINE_WIDTH/8; n++,wb=wb->next)  cells[n]=wb;
  return erection_compose_inc(n,cells);
}

static BOOL  make_str_raster(cell *wb, cell *we, StrRaster *str_raster)
{
  cell *c;
  INT left=MAXINT,top=MAXINT,right=0,bottom=0;

//raster size
  for (c=wb; c != we; c=c->next)
  {
    top=MIN(top,c->r_row);
    bottom=MAX(bottom,c->r_row+c->h);
    left=MIN(left,c->r_col);
    right=MAX(right,c->r_col+c->w);
  }
  str_raster->left=(LONG)left;
  str_raster->top =(LONG)top;
  str_raster->w=(LONG)(right-left);
  str_raster->h=(LONG)(bottom-top);

  if (str_raster->w > LINE_WIDTH || str_raster->h > LINE_HEIGHT)
    return FALSE;

  memset(&str_raster->pict,0,(str_raster->w+7)/8*str_raster->h);

//cells to raster
  for (c=wb; c != we; c=c->next)
    comptorast(c->env,str_raster);

  return TRUE;
}

static void comptorast(c_comp *cp1, StrRaster *str_raster)
{
 LONG Lc1=cp1->nl;   // number of lines in component
 LONG lc1,h1,y1;
 lnhead *lp1;
 interval *int1;

 lp1=(lnhead *) ( (CHAR *)cp1 + cp1->lines + 2); // beginning of first line
 for (lc1=0; lc1 < Lc1; lc1++)
 {
   h1=lp1->row+cp1->upper-str_raster->top;
   int1=(interval *)(lp1+1);       // ptr to current interval
   for (y1=0; y1 < (LONG)lp1->h; y1++, int1++, h1++)
     inttorast(str_raster,h1,(LONG)(int1->e+cp1->left-str_raster->left),int1->l);
   lp1=(lnhead *) ((CHAR *)lp1+lp1->lth);   // next line
 }
}

static void inttorast( StrRaster *r, LONG h, LONG end, LONG lth)
 {
 LONG j,je,me,ib,jb,mb,head=(r->w+7)/8*h;
 BYTE *str_raster=r->pict;

 ib=end-lth;
 mb = 0xff >> (ib & 7);
 jb = head + (ib>>3);
 me = 0xff << (8-(end & 7));
 je = head + (end >> 3);
 if (jb == je)
  {
  str_raster[jb] |= (mb & me);
  return;
  }
 str_raster[jb] |= mb;
 str_raster[je] |= me;
 for (j=jb+1; j < je; j++)
  str_raster[j] = 0xff;
 }

static BOOL calc_cut_points(cell *wb, cell *we, INT rastlc, INT rastdr)
{
  CHAR x;
  INT i,j,ro,wide=MAXINT;
  INT  nc;        //������⢮ �祭�� cell'�
  INT dust_sect=0;//䫠�: �祭�� �� dust'��
  INT mincl=MAXINT16,maxcl=rastlc;  //���. � ���. �業�� ��� �ࠢ��� ��� dust-ᥪ樨
  CutPoint *seci=cut_list+1; //⥪�饥 �祭��
  CutAdd cut_add[MAX_CUT_POINT];
  CutAdd *seci_add=cut_add+1;
  GraphNode *nodei;
  cell *C,box;    //�ᯮ������� ⮫쪮 row, col, w, h - ࠧ���� ��஡��
  memset(&box,0,sizeof(cell));

  memset(&vers_pool.node,0,MAX_SEG_VERS*sizeof(seg_vers *));
  vers_list[0]=&vers_pool.pool[0];

  cur_layer=layer1;  prev_layer=layer2;  nodei=prev_layer+1;

  ncut=1;     //��饥 �᫮ �祭��

/* dust-������ �����������, ����:
 - ���������� dust, ������� �� ����������� ��������� "��������" ������������
 - dust �� ������������ ������� �� ����� ��� �������  */

//����������� �������

  fict_sect(cut_list,cut_add,prev_layer,0,0);  //0-� 䨪⨢��� �祭��
  for (C=wb; C != we; C=C->next)
  {
    if ( dust(C) )
    {
      if ( C->r_col>=maxcl )
      {
        if ( dust_sect )
        {                  //����뢠�� ����� dust-ᥪ��
          close_ds(seci,seci_add,nodei,(INT)(maxcl-rastlc),(INT)(ncut-1));
          ncut++; seci++; seci_add++; nodei++;
          if ( ncut==MAX_CUTS ) {dust_sect=0; break;}
        }
        else
          (seci-1)->x=maxcl-rastlc;
                           //���뢠�� ����� dust-ᥪ��
        dust_sect=1; mincl=(C->r_col+C->w);
      }
      else
        if ( !dust_sect    //dust ��४�뢠���� "����訬" ��������⮬
             && C->r_col+C->w>maxcl
             && C->row+C->h<my_bases.b3+(my_bases.ps>>1)
             && ( C->row>my_bases.bm || is_defis(C) )
           )               //����� ���� �窠 ��� ������
        {                  //���뢠�� dust-ᥪ��
          (seci-1)->x=((seci-1)->x+(C->r_col-rastlc))>>1;
          dust_sect=1; mincl=maxcl=C->r_col+C->w;
        }
      maxcl=MAX(maxcl,C->r_col+C->w);
      mincl=MIN(mincl,C->r_col+C->w);
    }
    else  //�� dust
    {
      if ( dust_sect )
      {
        dust_sect=0;
        if ( mincl<=C->r_col )          //����뢠��  dust-ᥪ��
        {
          if ( maxcl>C->r_col ) x=mincl-rastlc;
          else                  x=maxcl-rastlc;
          close_ds(seci,seci_add,nodei,x,(INT)(ncut-1));
//          close_ds(seci,MAX(mincl,C->r_col-1)-rastlc,ncut-1);
//          close_ds(seci,MIN(maxcl,C->r_col-1)-rastlc,ncut-1);
          ncut++;  seci++;  seci_add++;  nodei++;
          if ( ncut==MAX_CUTS )  break;
        }
        else
        if (mincl==maxcl && ((seci-1)->x + rastlc + mincl)>>1 < C->r_col)
        {                                 //����뢠��  dust-ᥪ��
          close_ds(seci,seci_add,nodei,(INT)(mincl-rastlc),(INT)(ncut-1));
          ncut++;  seci++;   seci_add++;  nodei++;
          if ( ncut==MAX_CUTS )  break;
        }
//        else                            //dust-ᥪ�� ��㫨�����
//          (seci-1)->x=((seci-1)->x+C->r_col-rastlc)>>1;
      }
      else  // !dust_sect
        if ( maxcl>C->r_col )           //���嫥��
        {
//          x=((seci-1)->x+C->r_col-rastlc)>>1;
//          ro=middle(C)-rastlc;
//          (seci-1)->x=MIN(x,ro);
          if ( lefter(C,(seci-1)->x+rastlc) )  //��४�뢠���� �।��騬
          {                        // "����訬" - ��室���� ��� � dust'��
            maxcl=MAX(maxcl,C->r_col+C->w); continue;
          }
          else
            (seci-1)->x=((seci-1)->x+C->r_col-rastlc)>>1;
        }
        else
          (seci-1)->x=maxcl-rastlc;
      maxcl=MAX(maxcl,C->r_col+C->w);
      nc=0;
      if ( bad(C) &&
           ( C->w > (INT)min_cut_width ||
             C->r_col < (seci-1)->x+rastlc ||
             C->r_col+C->w > C->nextl->r_col ) //��४�뢠���� � �ᥤﬨ
         )
      {                                        //०��
        nc=get_points(C,seci,(INT)(MAX_CUT_POINT-ncut-1));
        for ( j=(INT)ncut; j<ncut+nc; j++ )         //���४��㥬 �� ���������
          cor_sect(C,&cut_list[j],rastlc,rastdr);  //cell'�� � ����
        ncut+=nc;  seci+=nc;   seci_add+=nc;  nodei+=nc;
      }
      nc=ncut-nc-1;                     //�祭�� ᫥�� �� C
      fict_sect(seci,seci_add,nodei,(INT)(C->r_col+C->w-rastlc),nc);
      if (nc==0)
      {
        cut_list->n = C->cg_flag;          //�ਧ��� ࠧ१�
//        cut_list->gvarr = C->cg_flag_fine;     //⨯ ࠧ१�
      }
      seci_add->top   = C->row;
      seci_add->left  = C->col;
      seci_add->bottom= C->row+C->h;
      seci_add->right = C->col+C->w;
      ncut++;  seci++;   seci_add++;  nodei++;
      if ( ncut==MAX_CUT_POINT ) break;
    }
  }
  if ( dust_sect )                 //��᫥���� ᥪ�� -  dust-ᥪ��
  {
    close_ds(seci,seci_add,nodei,(INT)(maxcl-rastlc),(INT)(ncut-1));
    ncut++;
  }
  else
  {
    ro=maxcl-rastlc;  (seci-1)->x=ro;
  }
  cut_list->x=0;      //����� �ᯮ������ �� ���뢠��� ��� ���㫨஢����
                      //��ࢮ� dust-ᥪ樨

//��।��塞 cell'�  �� �祭��

  for (C=wb; C != we; C=C->next)
  {
    for ( i=1,seci=cut_list+1,seci_add=cut_add+1,nodei=prev_layer+1;
          i<ncut;
          i++,seci++,seci_add++,nodei++)
      if ( seci->dh == 0 && lefter(C,seci->x+rastlc) ) break;
    if ( i==ncut ) continue;        //ࠧ१�� �� 墠⨫�

    seci->n++;                  //�᫮ ��������� � ᥣ����
    if (let_or_bad(C))
      if (seci_add->nbig++)  // ����� 1 "����讣�" - ���㫨�㥬 ���ᨨ
      {
        seci_add->vers.flg=0;  set_bad_vers(&seci_add->vers);
      }
      else
        if (!just(C)) save_alpha_vers(C,&seci_add->vers);//just(C) �� �� �ᯮ��������

    seci_add->top   = MIN(seci_add->top   ,C->row);
    seci_add->left  = MIN(seci_add->left  ,C->col);
    seci_add->bottom= MAX(seci_add->bottom,C->row+C->h);
    seci_add->right = MAX(seci_add->right ,C->col+C->w);

    if (nodei->prev==0) cut_list->n |= C->cg_flag & c_cg_cutl;
  }

//����塞 ����

  for ( i=1; i<ncut; i++ )
  {
    seci=cut_list+i;   seci_add=cut_add+i;  nodei=prev_layer+i;
    if ( seci->dh == 0 )    //�࠭�� ��室���� cell'�
    {
      if (wide==MAXINT && seci->x>=wmin)  wide=seci->x;
      if (seci_add->vers.flg != 0)           //�ᯮ�������� - ��࠭塞
      {
        vers_list[0]->px=(INT)nodei->prev;
        vers_list[0]->ro = (seci->n > 1) ? -1 : 0;  //�⮡� �ᯮ������� � dust'���
        memcpy(&vers_list[0]->vers,&seci_add->vers,sizeof(SVERS));
        vers_list[0]->next=vers_list[i];
        vers_list[i]=vers_list[0];
        vers_list[0]++;
      };
    }
    nodei->weight.meas = (seci->x>wide || seci->x<wmin) ? -1 : 0;
  }
  return TRUE;
}

static void fict_sect(CutPoint *sec, CutAdd *sec_add, GraphNode *node, INT x, INT px)
{
  memset(sec,0,sizeof(CutPoint));
  memset(sec_add,0,sizeof(CutAdd));
  memset(node,0,sizeof(GraphNode));
  sec->x=x; node->prev=px;
  sec_add->top=sec_add->left=MAXINT;  sec_add->right=sec_add->bottom=MININT;
}

static void close_ds(CutPoint *sec, CutAdd *sec_add, GraphNode *node, INT x, INT px)
{
  fict_sect(sec,sec_add,node,x,px);
  set_dust(&sec_add->vers);  set_bad_vers(&sec_add->vers);
//  cut->duflr=1;
}

static void cor_sect(cell *C, CutPoint *cut, INT left, INT down)
{
   cut->x+=C->r_col-left;
   cut->h+=down-(C->r_row+C->h-1);
}

static INT get_points (cell *C, CutPoint *listn, INT nmax )
{
  struct cut_elm list0[128],*li,*le;
  INT n;
  n=get_cuts(C,list0,(INT)MIN(nmax,127));
  for (li=list0,le=li+n; li<le; li++,listn++)
  {
    listn->x=li->x;  listn->dh=li->dh;  listn->h=li->h;  listn->var=li->var;  listn->n=0;
  }
  return n;
}

#undef CutAdd

static void save_alpha_vers(cell *C, SVERS *svers)
//save versions from alphabet only
{
  save_vers(C,svers);
  svers->nvers=cut_by_alpha(svers->nvers,svers->vers);
}

static INT cut_by_alpha(INT n, version vers[])
//remove not alphabet versions
{
  version *vo=vers,*vn=vers;
  INT i;

  if (n==0)
    return 0;
  for (i=0; i<n; i++,vo++)
    if (alpha[vo->let])
      *vn++=*vo;
  *vn=*vo;                 //terminating zero
  return vn-vers;
}


static Weight match(BYTE *word)
{
  LONG i,l,prev=0,curh=my_bases.ps;
  Weight weight;
  BYTE *bt;
  GraphNode *nodei;

  templ=word;

//first step - cutout possible dirt in the begin (width up to PS)
  for (i=0,nodei=prev_layer; i<ncut; i++,nodei++)
  {
    nodei->prev = 0;
    nodei->weight.meas = (cut_list[i].x < curh) ? 0 : -1;
    nodei->monitors=0;
  }

//main loop
  for (l=0; templ[l] && l<256; l++)
  {
    GraphNode *layer=prev_layer;
    LONG imax,pmax=-1;
    BOOL rerecog=FALSE;
    DWORD numbers=param->monitors;
    BYTE l1=(BYTE)(l+1);
    if (numbers)
      do
      {
        rerecog = (numbers & 0xFF)==l1;
        numbers >>= 8;
      }
      while(numbers && !rerecog);

    for (i=0,nodei=cur_layer; i<ncut; i++,nodei++)
    {
      nodei->prev = 0;
      nodei->weight.meas = -1;
      nodei->monitors=0;
    }

    if (!test_set(prev,curh,(BYTE)l,RELY,rerecog,&imax,&pmax))   //first from prev
    {
      LONG il=prev-1,ir=prev+1;
      LONG ile=MAX(0,il-1),ire=MIN(ncut-1,ir+1);
      LONG x=cut_list[prev].x;
      while (il>=ile || ir<ire)
      {
        LONG i0;
        if (il<ile)  i0=ir++;
        else
        if (ir>ire)  i0=il--;
        else
        if (x-cut_list[il].x < cut_list[ir].x-x)  i0=il--;
        else                                      i0=ir++;
        if (test_set(i0,curh,(BYTE)l,RELY,rerecog,&imax,&pmax))  break;
      }
    }
    if (pmax<0)
    {
      weight.meas=-1;
      return  weight;
    }
    prev=imax;
    prev_layer=cur_layer;  cur_layer=layer;
  }
  if (i>255)
    weight.meas=-1;
  else
  {
    if (param->monitors)
      param->monitors=(LONG)prev_layer[prev].monitors;
    weight=prev_layer[prev].weight;
    for (i=0,bt=(BYTE *)&weight.meas; i<3; i++,bt++)  *bt=255-(*bt);
  }
  return  weight;
}

static BOOL test_set(LONG prev, LONG h0, BYTE nlet, LONG tol, BOOL rerecog, LONG *imax, LONG *pmax)
{
  BYTE let=templ[nlet];
  CutPoint *cut,*cute=cut_list+ncut,*cutr,*cutl;
  LONG let2=let*2;
  LONG prmin=letters_pidx_table[let2],prmax=letters_pidx_table[let2+1],pr=(prmin+prmax)/2;
  LONG w0 = (pr<=64) ? pr*h0/64 : h0*64/(128-pr);
  LONG xb=cut_list[prev].x,x0=xb+w0;
  LONG set,cc;
  LONG  il,ir;
  BYTE p;
  BOOL rv=FALSE;

//  set_user_alphbet(alpha);

  for (cut=cut_list+prev; cut<cute;  cut++)   cut->flg=FALSE;

  for (set=1; set<=4; set++)
  {
  //���� ࠧ१ �� ����ﭨ� ����� w0 ����� ᬥ頥��� � ��� ��஭�
    LONG i=prev+1; cut=cut_list+i; il=ir=-128;
    while (cut<cute)
    {
      BYTE var=cut->var & 0x7F;
      if (cut->x > xb && in_set(var,set))
        if(cut->x <= x0)  { il=i;  cutl=cut; }
        else              { ir=i;  cutr=cut; break; }
      cut++; i++;
    }

    while (il>=0 || ir>=0)
    {
      if (il<0)    goto right;
      else
        if (ir<0)  goto left;
        else
          if (cutr->x-x0 < x0-cutl->x)  goto right;
left:
      if (cut_list[il].flg)
        il=dec(&cutl,il,prev,set);
      else
      {
        cut_list[il].flg=TRUE;
        cc=add_sect(prev,il,nlet,rerecog,&p);
        if (cc & 4)                     //㧪��
          il=-128;
        else
        {
          if (cc==0)
          {
            if (p>*pmax) { *pmax=p; *imax=il; }
            if (p>=tol)  { rv=TRUE;  goto ret;   }
          }
          il=dec(&cutl,il,prev,set);
        }
      }
      continue;
right:
      if (cut_list[ir].flg)
        ir=inc(&cutr,ir,ncut,set);
      else
      {
        cut_list[ir].flg=TRUE;
        cc=add_sect(prev,ir,nlet,rerecog,&p);
        if (cc & 8)                     //�ப��
          ir=-128;
        else
        {
          if (cc==0)
          {
            if (p>*pmax) { *pmax=p; *imax=ir; }
            if (p>=tol)  { rv=TRUE;   goto ret;   }
          }
          ir=inc(&cutr,ir,ncut,set);
        }
      }
    }
  }

ret:
  if (debug_on && det_trace)  show_layer(let,prev,*imax);
  return rv;
}

static LONG inc(CutPoint **cutp, LONG i, LONG ie, LONG set)
{
  if (i>=ie) return -128;
  (*cutp)++; i++;
  while (1)
  {
    CHAR var=(*cutp)->var & 0x7F;
    if (i==ie || in_set(var,set))  return i;
    (*cutp)++; i++;
  }
  return -128;
}

static LONG dec(CutPoint **cutp, LONG i, LONG ie, LONG set)
{
  if (i<=ie) return -128;
  (*cutp)--; i--;
  while (1)
  {
    CHAR var=(*cutp)->var & 0x7F;
    if (i==ie || in_set(var,set))  return i;
    (*cutp)--; i--;
  }
  return -128;
}

static LONG add_sect(LONG il, LONG ir, BYTE nlet, BOOL rerecog, BYTE *p)
{
  BYTE let=templ[nlet];
  version *v;
  void *kit;       //㪠��⥫� �⥪� kit
  cell *cells[MAX_CUTS];
  cell **cp,*lc;
  CutPoint *cutl=cut_list+il,*cutr=cut_list+ir;
  LONG xl=cutl->x,xr=cutr->x;
  LONG dh;
  LONG n;
  BYTE cut_fl = (( cutl->dh != 0 ) ? c_cg_cutl : 0) +
                 (( cutr->dh != 0 ) ? c_cg_cutr : 0);
  BYTE left_let=0;
  SVERS vers;      //���ᨨ ᥣ���� (i1,i0)
  INT width;       //��� �ਭ�
  Int8  gvar;       //ᯮᮡ ᡮન
#define ROI_LEN 4
  INT ro,roi[ROI_LEN]; //ro � ��� ��⠢���騥
  Weight wp=prev_layer[il].weight,wc=cur_layer[ir].weight,wt;
  BOOL change=TRUE;
  seg_vers *cur_vers;

  if (xr-xl-1 < wmin)
    return 4;

  if (wp.meas<0)
    return 16;  //no path

//�饬 �।� ࠭�� �ᯮ�������

  if (!rerecog)
    if ( cur_vers=find_vers((INT)il,(INT)ir,vers_list) )
    {
      if (v=find_in_vers(&cur_vers->vers,let))
      {
        if (cur_vers->ro<0)      //���� �ᯮ����� � dust'���
        {
          cur_vers->ro=-cur_vers->ro;
          memcpy(&vers,&cur_vers->vers,sizeof(SVERS));
          set_bad_vers(&vers);
          vers.nvers=1;  vers.vers[0]=*v;
        }
        else                     //㦥 �ᯮ��������
        {
          *p=v->prob;
          goto result;
        }
      }
    }

  kit=give_kit_addr();
  dh=select_cells(il,ir,cut_fl,cells);
  if (xr-xl-1 > (dh<<1)+(dh>>2))
  {
    for (cp=cells; *cp; cp++)  del_cell(*cp);
    take_kit_addr(kit);
    return 8;
  }

  if (!(*cells))
    return 64;

  for (n=1,cp=cells+1,lc=cells[0]; *cp; cp++,n++)
    if ((*cp)->col < lc->col)  lc=*cp;
  do lc=lc->prev;  while (!fict(lc) && !let_or_bad(lc));

//����� ������������ ����� ����������� ������ '�'
  if (let=='|' && !fict(lc) &&
	  !is_baltic_language(param->language) &&	// 17.07.2001 E.P.
	  !is_turkish_language(param->language)		// 21.05.2002 E.P.
	 )
  {
    left_let=lc->vers[0].let;
    lc->vers[0].let=(BYTE)'�';
  }

  memset(&vers,0,sizeof(SVERS));
  ro=recogij(lc,cells,(INT)n,cut_fl,0,com_inc,roi,&gvar,&vers,&width);
  take_kit_addr(kit);
  if (left_let) lc->vers[0].let=left_let;
//  vers.nvers=cut_by_alpha(vers.nvers,vers.vers);
  v=find_in_vers(&vers,let);
  *p = (v) ? v->prob : 0;

result:
  ro = MAX_RO-(*p);
//  wt = (wp & ~0xFF) + ro;
//  wt = MAX(wt,wp);
  wt = add_weight(&wp,(BYTE)ro,nlet);
  if (wc.meas<0 || wc.meas>wt.meas)
  {
    wc=wt;  cur_layer[ir].prev=(WORD)il;  add_monitors(il,ir,(BYTE)(nlet+1),(*p));
  }
  else
    change=FALSE;

//  if ((wc & 0xFF) >= 254 && wc <= 0xFFFF)  wc <<= 4;
  cur_layer[ir].weight=wc;
  if (change)
    if (debug_on && det_trace)
    {
      BYTE msg[80];
      LONG wpm=wp.meas,wcm=wc.meas;

      sprintf(msg,"%c p=%d (%d) %d %d %d (%d) %d %d %d\n",let,*p,
              il,wpm>>16,(wpm>>8)&0xFF,wpm&0xFF,ir,wcm>>16,(wcm>>8)&0xFF,wcm&0xFF);
      glsnap('a',cell_f(),msg);
/*
      *show_dp(msg,prev_layer,ir)=0;
      *show_dp(msg+strlen(msg),cur_layer,ir)=0;
      show_and_wait(msg);
*/
    }

  return 0;
}

static version *find_in_vers(SVERS *svers, BYTE let)
{
  version *v=&svers->vers[0];
  LONG i;
  for (i=0; i<svers->nvers; i++,v++)
    if (equal(v->let,let))
      return v;
  return NULL;
}

static BOOL equal(BYTE let1, BYTE let2)
{
  if (let1==let2)
    return TRUE;
  else
  {
    LONG offset=eq_let[let1];
    if (offset<0)
      return FALSE;
    return strchr(eq_list.d+offset,let2) != 0;
  }
}



static LONG select_cells(LONG il, LONG ir, BYTE cut_fl, cell **cells)
{
  LONG i;
  cell *CI=cell_f();
  INT minrow=my_bases.b2;
  CutPoint *cuti;
  BYTE csv[32];       //��ࠬ���� �祭��
  cut_pos    cpos={0};
  struct cut_elm  cutl,cutr;
  LONG xl=cut_list[il].x,xr=cut_list[ir].x;
  LONG xla=str_raster.left+xl,xra=str_raster.left+xr;
  BYTE seg_rast[RASTER_WIDTH*RASTER_HEIGHT/8],*sr=seg_rast,*wr;
  LONG rwb,wwb=(str_raster.w+7)/8;
  LONG beg=(LONG)xl/8-1,end=(LONG)(xr+7)/8+1;
  LONG row,col;
  MN *mn;
  beg=MAX(0,beg);  end=MIN(end,wwb-1);
  rwb=end-beg+1;
  *cells=NULL;

//  if (rwb*str_raster.h > RASTER_WIDTH*RASTER_HEIGHT/8)
  if (rwb>16)
    return 0;

  for (row=0; row<str_raster.h; row++)
  {
    wr=str_raster.pict+beg+row*wwb;
    for (col=0; col<rwb; col++)  *sr++=*wr++;
  }

  cuti=cut_list+il;
  cutl.x=xl-beg*8;  cutl.dh=cuti->dh;  cutl.h=cuti->h;  cutl.var=cuti->var;
  mn=cut_rast(seg_rast,(INT)(rwb*8),(INT)str_raster.h,(INT)str_raster.top,(INT)(str_raster.left+beg*8),
              &cutl,0,1,csv,&cpos);
  cuti=cut_list+ir;
  cutr.x=xr-beg*8;  cutr.dh=cuti->dh;  cutr.h=cuti->h;  cutr.var=cuti->var;
  mn=cut_rast(seg_rast,(INT)(rwb*8),(INT)str_raster.h,(INT)str_raster.top,(INT)(str_raster.left+beg*8),
              &cutr,0,2,csv,&cpos);

  for ( i=0; i<MAX_CUTS-1 && mn; i++ )
  {
     CI=create_my_cell(mn,CI,0,0);
     CI->stick_inc=NO_INCLINE;
     if ( !lefter(CI,xla) && lefter(CI,xra) )
     {
       //����砥� ��१����
       if (cut_fl & c_cg_cutl && CI->r_col==xla+1)
         CI->cg_flag |= c_cg_cutl;
       if (cut_fl & c_cg_cutr && CI->r_col+CI->w==xra)
         CI->cg_flag |= c_cg_cutr;

       if (dust(CI) && cut(CI) && CI->w==1)  //������� 䮪���
         CI=del_cell(CI);
       else
       {
         *cells++=CI;  minrow=MIN(minrow,CI->row);
       }
     }
     else
       CI=del_cell(CI);
     mn=mn->mnnext;
  }
  *cells=NULL;
  return my_bases.b3-minrow;
}

static Weight add_weight(Weight *w0, BYTE ro, BYTE nlet)
{
  Weight wp=*w0;
  BYTE *wi=(BYTE *)&wp.meas,*li=(BYTE *)&wp.nlet;

  if (ro > *wi)
  {
    LONG i;
    BYTE *w1=wi+1,*l1=li+1;
    *wi=ro;  *li=nlet;
    for (i=0; i<2; i++,wi++,w1++,li++,l1++)
      if (*wi > *w1)
      {
        BYTE t=*w1;  *w1=*wi;  *wi=t;
             t=*l1;  *l1=*li;  *li=t;
      }
      else
        break;
  }
  return wp;
}

static void add_monitors(LONG il, LONG ir, BYTE nlet, BYTE pb)
{
  DWORD numbers=param->monitors;
  if (numbers)
  {
    DWORD monitors=prev_layer[il].monitors,p=pb;
    do
    {
      if ((numbers & 0xFF)==nlet)
      {
        monitors = monitors+p;  break;
      }
      numbers >>= 8;  p <<= 8;
    }
    while(numbers);
    cur_layer[ir].monitors=monitors;
  }

}

static void set_bad_vers(SVERS *c)
 {
 c->nvers=0;
 c->source = 0;
 c->vers[0].let=bad_char;
 c->vers[0].prob=0;
 c->vers[1].let=0;
 if (c->flg & (c_f_let+c_f_bad)) c->flg=c_f_bad;
 }

static void mw_show_rast()
{
  raster r;
  struct cut_elm cut_el[MAX_CUTS];
  LONG i;
  cell *c=cell_f()->next;

  if (str_raster.w>128 || str_raster.h>64)
  {
    Word32 key=1;
    for (i=1; i<ncut-1; i++)
    {
      Point16 vh,vl;
      vh.y=(Int16)str_raster.top;  vl.y=(Int16)(str_raster.top+str_raster.h);
      vh.x=vl.x=(Int16)(str_raster.left+cut_list[i].x);
      LDPUMA_DrawLine(NULL,&vh,&vl,0,wRGB(255,0,0),1,key);
    }
    glsnap('a',c,"raster too big to show");
    LDPUMA_DeleteLines(NULL, key);
    return;
  }

  r.top =(INT)str_raster.top;   r.w=(INT)str_raster.w;
  r.left=(INT)str_raster.left;  r.h=(INT)str_raster.h;
  memcpy(&r.pict,&str_raster.pict,str_raster.h*(str_raster.w+7)/8);

  for (i=0; i<ncut; i++)
  {
    struct cut_elm *ce=cut_el+i;
    CutPoint *cp=cut_list+i;
    ce->x  =(CHAR)cp->x;
    ce->dh =(CHAR)cp->dh;
    ce->h  =(CHAR)cp->h;
    ce->var=(CHAR)cp->var;
  }
  cut_el[ncut].x=127;

  cg_show_rast(c,&r,"",cut_el);
}

static void show_layer(BYTE let, LONG prev, LONG imax)
{
  char msg[600],*s=msg;
  GraphNode *layer=prev_layer;
  LONG i2=imax+3,i1,i,j,shift;
  i2=MIN(i2,ncut-1);
  i1=i2-15;  i1=MAX(i1,0);

  s += sprintf(msg,"%c %d %d\n",let,prev,imax);
  for (i=i1; i<=i2; i++)
    s += sprintf(s,"%4d",i);
  s += sprintf(s,"\n");
  for (j=0; j<2; j++)
  {
    for (shift=0; shift<=16; shift += 8)
    {
      for (i=i1; i<=i2; i++)
      {
        LONG w=layer[i].weight.meas;
        if (w>0) w = (w>>shift) & 0xFF;
        s += sprintf(s,"%4d",w);
      }
      s += sprintf(s,"\n");
    }
    layer=cur_layer;
  }
  show_and_wait(msg);

}

#ifdef MATCH_WORD

void myCharToOem(Word8 ansi[], Word8 ascii[]);

void test_match_cell_word(B_LINES *my_bases, INT cut_width)
{
  BYTE *e,*b,msg[36];
  INT col1=-10000,col2=-10000;
  Weight match;
  LONG m,n;
  cell *wb=cell_f()->next,*we;
  MatchWordPar  param;
  CHAR wascii[80];
  DWORD mon;

  strcpy(mwInput,"");
  glsnap('a',wb,"match_word: input col for begin and last cell and string");
  if (strlen(mwInput)==0)
    return;

  b=mwInput;
  e=strchr(mwInput,' ');
  if (!e)
    return;
  *e=0;
  col1=atoi(b);

  b=e+1;
  e=strchr(b,' ');
  if (!e)
    return;
  *e=0;
  col2=atoi(b);

  b=e+1;
  e=strchr(b,' ');
  if (!e)
    return;
  *e=0;
  mon=atoi(b);

  b=e+1;
  b += strspn(b," ");
  if (*b==0)
    return;

  e=strchr(b,' ');
  if (e)  *e=0;

  for ( ; wb->next; wb=wb->next)
    if (col1==wb->col)  break;

  we=wb;
  for ( ; we && we->next; we=we->next)
    if (col2==we->col)  break;

  if (!we->next)
    return;

  param.bases=my_bases;
  param.cut_width=cut_width;
  param.monitors=mon;

  myCharToOem(b,wascii);
  match=match_cell_word(wb,we->next,wascii,&param);
  m=match.meas; n=match.nlet;
  sprintf(msg,"end match %s %d: %d %d: %d %d: %d\n",wascii,
          n>>16,m>>16,(n>>8)&0xFF,(m>>8)&0xFF,n&0xFF,m&0xFF);
  glsnap('a',cell_f()->next,msg);
}

#endif
