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

#define ABRIS_TOOLS_MODULE
#include "abris.h"
#include <string.h>
#define  MLTPLR 64

static BYTE pen_over_b2[8]={ 0,0,20,80,120,200,220,240 };
static INT  x_beg, y_beg, delta_x, delta_y;
static INT  orient;
static BYTE pen_for_convex[6] = { 0, 32, 48, 64, 128, 240 };


void abris_reset()
{
 sfl=0;
 abriflag=leftfl=rightfl=topfl=botfl=0;
}

INT make_histo ( BYTE *array, BYTE max_num)
{
INT i,j, mode_val;

 memset(histo,0,128);
 histo_max_value = 0;
 mode_val = -1;

 for (i=0; i< max_num; i++)
  {
   j = array[i];
   histo[j] ++;
   if ( j > histo_max_value )
     histo_max_value = (BYTE)j;
  }
 for (i=0, j=0; i <= histo_max_value; i++)
  {
   if ( histo[i] > j )
    { j = histo[i]; mode_val = i; }
  }
 return mode_val;
}
/****************************************/
void make_abris(s_glue *GL, cell *cl)
{
 lnhead *Lp1, *Lp2;
 interval *int1;
 c_comp *cp1;
 cell *a;
 BYTE *rpsum, *rpln, *rpleft, *rpright, *rpwid;
 BYTE intbeg, intend, intlth;
 INT  Lc1, lc1, nc,
      hw, hm, rowd, cold;

 Z=&string;
 get_b_lines(cl,&bases);
 gl_mincol=gl_minrow=32000;
 gl_maxcol=gl_maxrow=-32000;
 ncells=0;
 abriflag=1;
 memset(let_mon,0,8);
 while ((a=GL->celist[ncells++]) != NULL)
 {
  if (a->env == NULL)
    { abriflag=2; return; }
  if (gl_mincol > a->col) gl_mincol=a->col;
  if (gl_minrow > a->row) gl_minrow=a->row;
  if (gl_maxrow < (a->row+a->h)) gl_maxrow=a->row+a->h;
  if (gl_maxcol < (a->col+a->w)) gl_maxcol=a->col+a->w;
 }
 ncells--;

 {
  INT h = gl_maxrow - gl_minrow; INT w = gl_maxcol - gl_mincol;
   memset(l_abr,255,h);
   memset(r_abr,255,h);
   memset(top,255,w);
   memset(bot,255,w);
   memset(linh,0,h);
   memset(sumh,0,h);
   memset(wid,0,h);
 }

 fullh=gl_maxrow-gl_minrow;
 midh=fullh/2;
 fullw=gl_maxcol-gl_mincol;
 midw=fullw/2;
 nc=0;
 total_num_black_int = 0;
 total_lth_inter_white = 0;
 total_lth_black = 0;
 num_lines_of_comp = 0;
 while ((a=GL->celist[nc++]) != NULL)
 {
  rowd=a->row - gl_minrow;  // cell_to_box_relative row
  cold=a->col - gl_mincol;  // cell_to_box_relative col
  cp1 = a->env;
  Lc1=cp1->nl;        // number of lines in component
  Lp1=(lnhead *) ( (CHAR *)cp1 + cp1->lines + 2); // beginning of first line
  Lp2=Lp1;
  for (lc1=0; lc1 < Lc1; lc1++)
  {
      hm = Lp1->row + rowd;
      rpleft=&l_abr[hm];
      rpright=&r_abr[hm];
      rpsum=&sumh[hm];
      rpwid=&wid[hm];
      rpln=&linh[hm];
      int1=(interval *)(Lp1+1);		// ptr to first interval
      if (Lp1->h > 1) num_lines_of_comp++;
    // vertical abris
      for ( hw = 0; hw < Lp1->h;
            int1++, hm++, hw++ ,rpleft++, rpright++, rpsum++, rpln++, rpwid++)
      {
        intend=int1->e+cold;
        intlth=int1->l;
        intbeg=intend-intlth;
        {
            BYTE bhm = (BYTE)hm;
            BYTE bfl = fullh-hm;
            LONG i, e = intend;
            for (i=intbeg; i<e; i++)
            {
                if ((BYTE)bhm < (BYTE)top[i] ) top[i]=bhm;
                if ((BYTE)bfl < (BYTE)bot[i] ) bot[i]=bfl;
            }
        }
        intend=fullw-intend;
        *rpsum += intlth;
        *rpln  += 1;
        if (*rpright > intend) *rpright = intend;
        if (*rpleft  > intbeg) *rpleft  = intbeg;
        *rpwid=(fullw - *rpright - *rpleft);
      }
      if (Lp1->h > 1)  total_num_black_int += Lp1->h;

      Lp2=Lp1=(lnhead *) ((CHAR *)Lp2+Lp2->lth);   // next line
  }  // for all lines
 }  // while all cells

 for ( nc = 0; nc < fullh; nc++ )
 {
    total_lth_inter_white += wid[nc] - sumh[nc];
    total_lth_black += sumh[nc];
 }
 bold_flag = LIGHT_COMP;
 if ( abs( total_lth_black - total_lth_inter_white ) < midw )
    bold_flag = SEMI_BOLD_COMP;
 else
 {
    if ( total_lth_black > 3 * total_lth_inter_white )
        bold_flag = BOLD_COMP;
 }

}
/***************************************/
void make_wid()
{ INT i, ns; BYTE *pfun;
 if (sfl & 1) return;
 swt=0;
 for (i=0, pfun=wid; i<fullh; pfun++,i++)
   swt+=*pfun;
 sws=0;
 for (i=0, pfun=wid, ns=0; i<fullh; pfun++,i++)
   if (((*pfun) * fullh) < swt) { ns++; sws+=*pfun; }
 swt /= fullh;
 if (ns) sws /= ns; else sws=1;
}
/*************************************/
void make_left_max()
{ INT i; BYTE *pfun, sb, nb;
 if (leftfl & 1) return;
 leftfl |= 1;
 lmin1=lmin2=255;
 lmax=0;
 for (i=0, pfun=l_abr; i< fullh/3; i++,pfun++)
  {
   if (lmin1 > *pfun)  { lmin1 = *pfun; lxmin11=(BYTE)i; }
   if (lmin1 == *pfun)                  lxmin12=(BYTE)i;
  }
 for (i=fullh-i, pfun=&l_abr[i]; i<fullh; i++,pfun++)
  {
   if (lmin2 > *pfun)  { lmin2 = *pfun; lxmin21=(BYTE)i; }
   if (lmin2 == *pfun)                  lxmin22=(BYTE)i;
  }

 ljmp=0;  lnmid=255;
 for (i=lxmin12, pfun=&l_abr[i], sb=*pfun; i<lxmin21; i++, pfun++,sb=nb)
  {
    if (linh[i] < lnmid) lnmid=linh[i];
    nb=*pfun;
    if (lmax < nb) {  lmax=nb; lxmax1=(BYTE)i; }
    if (lmax == nb)   lxmax2=(BYTE)i;
    if ((nb > sb) && (ljmp==0))   { ljmp=1; ljmp1=(BYTE)i; }  // 1st right jump
    if ((nb < sb) && (ljmp > 0))  { ljmp=2; ljmp2=(BYTE)i; }  // last left jump
  }
}

void make_right_max()
{
 INT i; BYTE *pfun, sb, nb;
 if (rightfl & 1) return;
 rightfl |= 1;
 rmin1=rmin2=255;
 rmax=0;
 for (i=0, pfun=r_abr; i< fullh/3; i++,pfun++)
  {
   if (rmin1 > *pfun)  { rmin1 = *pfun; rxmin11=(BYTE)i; }
   if (rmin1 == *pfun)                  rxmin12=(BYTE)i;
  }
 for (i=fullh-i, pfun=&r_abr[i]; i<fullh; i++,pfun++)
  {
   if (rmin2 > *pfun)  { rmin2 = *pfun; rxmin21=(BYTE)i; }
   if (rmin2 == *pfun)                  rxmin22=(BYTE)i;
  }

 rjmp=0;
 for (i=rxmin12, pfun=&r_abr[i], sb=*pfun; i<rxmin21; i++, pfun++,sb=nb)
  {
    nb=*pfun;
    if (rmax < nb) {  rmax=nb; rxmax1=(BYTE)i; }
    if (rmax == nb)   rxmax2=(BYTE)i;
    if ((nb > sb) && (rjmp==0))   { rjmp=1; rjmp1=(BYTE)i; }  // 1st  left  jump
    if ((nb < sb) && (rjmp > 0))  { rjmp=2; rjmp2=(BYTE)i; }  // last right jump
  }
}

void make_top_max()
{ INT i; BYTE *pfun;
 if (topfl & 1) return;
 topfl |= 1;
 tmin1=tmin2=255;
 tmax=0;
 for (i=0, pfun=top; i< fullw/3; i++,pfun++)
  {
   if (tmin1 > *pfun)  { tmin1 = *pfun; txmin11=(BYTE)i; }
   if (tmin1 == *pfun)                  txmin12=(BYTE)i;
  }
 for (i=fullw-i; i<fullw; i++,pfun++)
  {
   if (tmin2 > *pfun)  { tmin2 = *pfun; txmin21=(BYTE)i; }
   if (tmin2 == *pfun)                  txmin22=(BYTE)i;
  }

 for (i=txmin12, pfun=&top[i]; i<txmin21; i++, pfun++)
  {
    if (tmax < *pfun) {  tmax=*pfun; txmax1=(BYTE)i; }
    if (tmax == *pfun)   txmax2=(BYTE)i;
  }
}


void make_bot_max()
{ INT i; BYTE *pfun;
 if (botfl & 1) return;
 botfl |= 1;
 bmin1=bmin2=255;
 bmax=0;
 for (i=0, pfun=bot; i< fullw/3; i++,pfun++)
  {
   if (bmin1 > *pfun)  { bmin1 = *pfun; bxmin11=(BYTE)i; }
   if (bmin1 == *pfun)                  bxmin12=(BYTE)i;
  }
 for (i=fullw-i; i<fullw; i++,pfun++)
  {
   if (bmin2 > *pfun)  { bmin2 = *pfun; bxmin21=(BYTE)i; }
   if (bmin2 == *pfun)                  bxmin22=(BYTE)i;
  }

 for (i=bxmin12, pfun=&bot[i]; i<bxmin21; i++, pfun++)
  {
    if (bmax < *pfun) {  bmax=*pfun; bxmax1=(BYTE)i; }
    if (bmax == *pfun)   bxmax2=(BYTE)i;
  }
}

void    make_inter_white_histo()
{
BYTE i;

    histo_max_value = 0;
    memset( histo, 0, 128 );
    for ( i = 0; i < fullh; i++ )
    {
        histo[i] = wid[i] - sumh[i];
        if ( histo[i] > histo_max_value )
            histo_max_value = histo[i];
    }
    return;
}

BYTE    find_2int_zone( BYTE from, BYTE to, PBYTE beg, PBYTE end )
{
    BYTE ret_code, i;
    INT wb, we;
    ret_code = NOT_FOUND_TWO_INT_ZONE;
    wb = we = -1;
    memset( histo, 0, 128 );

    for( i = from; i < to; i++ )
    {
        if ( ( (linh[i] == 2) && ( (linh[i+1] == 2) || (i > 2) ) ) ||
             ( ( linh[i] == 3 ) && ( linh[i-1] == 2 ) && ( linh[i+1] <= 2 ) ) )
        {
            we = i;
            histo[i] = wid[i] - sumh[i];
            if ( wb < 0 )
                wb = i;
        }
        else
        {
            if ( wb < 0 ) continue;
            we = i;
            break;
        }

    }
    if ( we - wb > 1 )
    {
        ret_code = FOUND_TWO_INT_ZONE;
        *beg = (BYTE)wb;
        *end = (BYTE)(we-1);
    }
    return ret_code;
}

 BYTE    find_1int_zone( BYTE from, BYTE to, PBYTE beg, PBYTE end )
{
    BYTE ret_code, i;
    INT wb, we;
    ret_code = NOT_FOUND_ONE_INT_ZONE;
    wb = we = -1;

    for ( i = (BYTE)midh; i >= from; i-- )
    {
        if ( (linh[i] == 1) ||
             ( (linh[i] == 2) && (linh[i-1] == 1) &&
               (linh[i+1] == 1) && (wid[i] - sumh[i] <= 1) ) )
            wb = i;
        else
        {
            if ( wb < 0 )   continue;
            break;
         }
    }
    for ( i = (BYTE)midh; i <= to; i++ )
    {
        if ( linh[i] == 1 )
            we = i;
        else
            break;
    }
    if ( ( wb < 0 ) || ( we < 0 ) )
        goto F1Z_Ret;
    if ( we - wb >= ( (fullh + 9)/10 ) )
    {
        ret_code = FOUND_ONE_INT_ZONE;
        *beg = (BYTE)wb;
        *end = (BYTE)we;
    }
F1Z_Ret:
    return ret_code;
}

BYTE r_tail ( BYTE lev)
{
BYTE xtail=0, ltail, j, fl, tresh;
 ltail = fullh / 5;

 for ( j = fullw -1; j >= midw; j--)
  {
    if (
        ( top[j] > midh ) &&
        ( ( top[j-1] < midh ) ||
          ( ( top[j] > top[j-1]) && ( top[j-2] < midh ) )
        )
       )
      { xtail = j; break; }
  }
 if ( ( xtail ) && ( abs (top[xtail] - lev) <= ltail ) )
   return 1;
 tresh = fullw/10;
 if ( tresh > 0 )
    tresh--;
 fl = monotonous_decrease( l_abr, 1, (BYTE)(fullh - (fullh/4)), &j, tresh );
 if ( fl == NOT_FOUND_MON_DECR )
    return 0;
 if ( top[fullw -1] > 3 )
    return 0;
 fl = monotonous_increase( r_abr, top[fullw -1], (BYTE)(fullh - 1), &j, 0 );
 if ( fl != NOT_FOUND_MON_INCR )
    return 0;
 fl = monotonous_increase( r_abr, (BYTE)(j + 2), (BYTE)(fullh - 1), &ltail, 0 );
 if ( fl == FOUND_MON_INCR )
    return 1;
 return 0;
}


BYTE rb_corner()
{
#define PNL_ROUND_RBC  10      // Round abris at right bottom corner

 BYTE pen, shift;
 INT  fl, i1, i2, i3, i4, j1, j2, j3, j4;

 make_right_max();
 i1=fullh-1; i2=i1-1; i3=i2-1; i4=i3-1;
 j1=fullw-1;
 shift = ( fullw >> 2 );

 if ( ( (bot[fullw-1] - bot[fullw-shift])  > midh  ) ||
      ( (rmin1 < BOT_MIN_RGHT_S) && (rmin2 > TOP_MIN_RGHT_S) ) )  // italic
    j1 = fullw - shift;              // shift to left on 1/4 of width
 j2=j1-1; j3=j2-1; j4=j3-1;

   fl=0;


   if ( (bot[j1] > bot[j2]) &&
	(bot[j2] > bot[j3]) &&
	(bot[j3] > bot[j4]) )
      fl += 2;

  if ( (bot[j1]  > bot[j3]) && (bot[j2]  > bot[j4]) )
      fl +=1 ;

  if ( fl <= 1 ) goto Penalty;

  if ( (r_abr[i1] > r_abr[i2]) &&
    (r_abr[i2] > r_abr[i3]) &&
    (r_abr[i3] > r_abr[i4])     )
      fl += 2;
   if ( (r_abr[i1] > r_abr[i3]) &&
    (r_abr[i2] > r_abr[i4]) )
       fl += 1;

Penalty:
 pen = fl * PNL_ROUND_RBC;
 return pen;
}

BYTE a_rb_corner( INT cur_p )
{
INT pnl_rbc, newprob;

 pnl_rbc = rb_corner();
 if ( linh[fullh-2] == 2 )          // the bottom abris likes letter 'a'
   pnl_rbc = ( (pnl_rbc >> 2) & 0xfe);
 newprob = cur_p - pnl_rbc;

 if (newprob <= 0 )
  newprob = 2;
 return( (BYTE)newprob );
}

BYTE round_rb_corner()
{
#define PNL_NOT_ROUND_RBC  10      // Penalty for not round
                                   //  abris at right bottom corner

 BYTE pen;
 INT  st, bnd, prev, cur;

 pen = 0;
 bnd = fullh - (fullh >> 2) - 1;

 for ( st=4, cur=fullh-1, prev=fullh-2; ( cur>bnd && st>0); prev--, cur-- )
  {
    if ( r_abr[cur] - r_abr[prev] > 3 )
     { st = 4; continue; }
    if ( r_abr[cur] - r_abr[prev] < 0 )
     { st = 4; break; }
    if ( r_abr[cur] == r_abr[prev] ) continue;
    st --;
  }

  if ( fullh < 22 )                                 st -= 2;
  if ( ( fullh >= 22 ) && ( fullh < 36 ))           st --;

  if ( st > 0 )  pen = st * PNL_NOT_ROUND_RBC;
 return pen;
}

 BYTE test_bottom_corner( PBYTE abris, INT *size)
{

 BYTE type;
 INT  pen, convex_st, concave_st, bnd, prev, cur;

 pen = 0; type = 0;
 bnd = fullh - (fullh >> 2) - 1;


 for ( cur=fullh-1, prev=fullh-2, concave_st = 0, convex_st=0;
       ( cur>bnd && abs(concave_st)<5 && convex_st < 5);
      prev--, cur-- )
  {
    if ( abris[cur] - abris[prev] > 3 )   // jump case - reset counters
     { concave_st = 0; convex_st = 0; continue; }
    if ( abris[cur] - abris[prev] < 0 )   // the sign of concave abris
     { concave_st --; continue; }
    if ( abris[cur] == abris[prev] )      // flat abris
     continue;
    convex_st ++;                         // the sign of convex abris
  }

  if ( ( concave_st != 0 ) && ( convex_st != 0 ) )
   {
    type = BC_SERIF;
    if ( concave_st + convex_st == 0 )
     {
      if ( convex_st - concave_st == 2 )
        type = BC_STRAIGHT_ANGLE;
      pen = 4;
     }
    else
      pen = abs ( concave_st + convex_st );
    pen = pen * PNL_FOR_SERIF_BC;
    goto TBC_Ret;
   }
  if ( convex_st > 0 )
   {
    type = BC_CONVEX;
    pen = convex_st * PNL_FOR_CONVEX_BC;
   }
  if ( concave_st < 0 )
   {
     type = BC_CONCAVE;
     pen = (-concave_st) * PNL_FOR_CONCAVE_BC;
   }
TBC_Ret:
  *size = pen;
  return type;
}

INT two_int_over_b2 ( cell *curc, INT *loc_2_int)
// This procedure determines the apperance two black intervals near 2nd base
// This procedure is used by discriminaters of follow letters: 'b', 'h' and 'd'
{
INT twoint, retpen, h14, i;
BYTE  shift_from_top;

 retpen = 0;
 twoint = -1;
 shift_from_top = 1;                  // default shift from top of the box

 get_b_lines(curc,&bases);

 i = bases.b2 - bases.b1;

 if ( ( curc->h > TALL_LETTER_bdh ) && ( i > DISTANCED_BASE_LINES ) )
   shift_from_top = ( curc->h >> 3 ) + 2;    // shift = 1/8 of full height

 if ( i > 2 )
   h14 = (bases.b2 - curc->row);   // distance between second base line and
                                   // top of the box
 else
   h14 = ((curc->h - 2) >> 2) - 1;

// searching for the point where two black intervals take place in box
    for ( i = shift_from_top; i < curc->h; i++ )
     {
        if ( ( linh[i] > 1 ) && ( linh[i+1] > 1) )
        {
          // to avoid italic letters with small tail at top
          if ( ( linh[i] == 2 ) &&
               ( curc->h > MEDIUM_LETTER_bdh ) &&
               ( curc->h <= TALL_LETTER_bdh ) &&
               ( wid[i] - sumh[i] < ( curc->h/10 ) ) )
            continue;
          else
            { twoint = i; break; }
       }
     }

 *loc_2_int = twoint;   // return resulting value
 if (twoint >= 0)  // place with 2 intervals has been found
 {
   if ( (i=h14 - twoint) > 1 )  // 2 intervals found above b2
   {
     i= (i<<3)/(bases.b2 - bases.b1);
     if (i >= (sizeof(pen_over_b2)/sizeof(pen_over_b2[0])))
       i = (sizeof(pen_over_b2)/sizeof(pen_over_b2[0]))-1;
     retpen += pen_over_b2[i];
   }
 }
 return retpen;
}

 INT Let_bint_index( CHAR Let )
{
 switch (Let)
  {
   case 'a':   return 145;
   case 'h':   return 128;
   case 'b':   return 115;
   case 'd':   return 128;
   case 'o':   return 128;
   case 'R':   return 128;
   case 's':   return 110;
   default :   return wint;
  }
}

#define GAP_MAX_SIZE 128
 INT gap_in_side(
       BYTE from, BYTE to, BYTE code_func, INT tresh, BYTE *flit)
{
INT retgap;
BYTE fmax, fmin, xmax, xmin, i;
BYTE *func;

   *flit = 0; retgap = 0;
   switch ( code_func )
    {
     case LEFT_S:
       func = l_abr;
       break;
     case RIGHT_S:
       func = r_abr;
       break;
     case TOP_S:
       func = top;
       break;
     case BOTTOM_S:
       func = bot;
       break;
     default: goto GIS_Ret;
    }
   // find extremum onto interval (from, to) of the func
   get_max_min ( func, from, to, &fmax, &fmin, &xmax, &xmin );
   if ( ncells > 1 ) goto  GIS_AfterSideTest;
    for ( i = from; i < to - 1; i++ )
     {
       if ( abs ( func[i] - func[i+1] ) > tresh )
         retgap += abs ( func[i] - func[i+1] ) - tresh;
       if ( abs ( func[i] - func[i+2] ) > tresh )
         retgap += abs ( func[i] - func[i+2] ) - tresh;
       if ( abs ( func[i+1] - func[i+2] ) > tresh )
         retgap += abs ( func[i+1] - func[i+2] ) - tresh;
     }

GIS_AfterSideTest:
   if ( ( retgap == 0 ) && ( (fmax - fmin) > tresh ) )
   {
     if (code_func == RIGHT_S)
     {
      if ( xmax > xmin )     // incline as straight slash
       {
        *flit = 1;          // italic's flag
        retgap = fmax - func[xmax + 2] - 1;
       }
      if ( xmax < xmin )
       retgap =  fmax - func[xmax - 2] - 1;
     }
     if (code_func == LEFT_S)
     {
      if ( xmax < xmin )     // incline as straight slash
       {
        *flit = 1;          // italic's flag
        retgap = fmax - func[xmax - 2] - 1;
       }
      if ( xmax > xmin )     // incline as back slash
       retgap =  fmax - func[xmax + 2] - 1;
     }
     if ( retgap < 0 )  retgap = 0;
    }
  if ( retgap > GAP_MAX_SIZE ) retgap = GAP_MAX_SIZE;
GIS_Ret:
  return retgap;
}

void get_max_min ( PBYTE func, BYTE from, BYTE to,
                     PBYTE fmax, PBYTE fmin, PBYTE x_fmax, PBYTE x_fmin )
{
BYTE i;

   for ( i=from, *fmax=0, *fmin=255; i < to; i++ )
    // find extremum onto interval (from, to) of the func:
    // maximum - most far from start; minimum - most near from start
    {
     if ( func[i] >= *fmax )    { *fmax = func[i]; *x_fmax = i; }
     if ( func[i] < *fmin )     { *fmin = func[i]; *x_fmin = i; }
    }
    return;
}

 BYTE constancy_vally_lth (PBYTE func, BYTE from, BYTE to, BYTE tresh)
{
BYTE i, cvlth, fl, beg, end;

   cvlth = 0; fl = 0; beg = 0; end = 0;
   for ( i = from + 1; i < to - 1; i++, fl = 0 )
    {
      if ( abs(func[i+1] - func[i-1]) > tresh )
       fl++;
      if ( abs(func[i+1] - func[i]) > tresh )
       fl++;
      if ( abs(func[i-1] - func[i]) > tresh )
       fl++;
      if ( fl > 0 )
       {
         if ( beg > 0 )
           {
            if ( i - beg > 3)  break;
            else               beg = 0;
           }
         continue;
       }
      else
       {
        if ( beg > 0 )  continue;
        beg = i - 1;
       }
    }
   if ( beg )
    {
     end = i + 1;
     cvlth = end - beg;
    }
   return cvlth;
}

BYTE monotonous_decrease
        ( PBYTE func, BYTE from, BYTE to, PBYTE last_decr, BYTE tresh)
{
INT  delta;
BYTE ret_code, smooth_sum, i;

    ret_code = FOUND_MON_DECR;
    for ( smooth_sum = 0, i = from; i < to - 1; i++ )
    {
        delta = func[i+1] - func[i];
        if ( delta > tresh )                     // function increasing
        {
            ret_code = NOT_FOUND_MON_DECR;
            *last_decr = i;
            break;
        }
        if ( abs(delta) <= tresh )
            smooth_sum ++;
    }
    if  ( smooth_sum == to - from - 1 )
        ret_code = SMOOTH_FUNC;
    return ret_code;
}

 BYTE monotonous_increase
        ( PBYTE func, BYTE from, BYTE to, PBYTE last_incr, BYTE tresh)
{
INT  delta;
BYTE ret_code, smooth_sum, i;

    ret_code = FOUND_MON_INCR;
    for ( smooth_sum = 0, i = from; i < to - 1; i++ )
    {
        delta = func[i] - func[i+1];
        if ( delta > tresh )                     // function decreasing
        {
            ret_code = NOT_FOUND_MON_INCR;
            *last_incr = i;
            break;
        }
        if ( abs(delta) <= tresh )
            smooth_sum ++;
    }
    if  ( smooth_sum == to - from - 1 )
        ret_code = SMOOTH_FUNC;
    return ret_code;
}

 INT top_end_bottom_weights_delta()
{
BYTE i, wto, wfrom;
INT  delta, wi, bot_sum, top_sum;

    wi = 1;
    wto = midh - 1;

    if ( (fullh & wi) == 1 )      // fullh is odd
        wfrom = midh + 1;
    else                          // fullh is even
        wfrom = (BYTE)midh;

    for ( top_sum = 0, i = 0; i <= wto; i++ )
        top_sum += sumh[i];

    for ( bot_sum = 0, i = wfrom; i < fullh; i++ )
        bot_sum += sumh[i];

    delta = top_sum - bot_sum;
    return delta;

}

 BYTE new_prob( INT penalty )
{
BYTE ret_prob;
INT wprob;

 wprob = cprob - penalty;
 if ( wprob < 2 )
  ret_prob = 2;
 else
  ret_prob = (BYTE)wprob;
 return ret_prob;
}

void construct_line_by_two_points( BYTE x1, BYTE y1, BYTE x2, BYTE y2 )
{
    x_beg   = MLTPLR * x1;
    y_beg   = MLTPLR * y1;
    delta_x = MLTPLR * abs( x1 - x2 );
    delta_y = MLTPLR * abs( y1 - y2 );
    if ( x1 >= x2 )
        orient = -1;
    else
        orient = 1;
    return;
}

INT get_x_coord( BYTE y_coord )
{
	INT xc;
	//AK! add for no divide by zerro
		if ( delta_y )
					xc = x_beg + orient * ( (( (y_coord * MLTPLR) - y_beg ) * delta_x ) / delta_y );
			else
					xc = x_beg;
		return xc;
}

BYTE test_against_convex( PBYTE func, BYTE from, BYTE to )
{
INT  x, i, dx;
BYTE pen, cnt;
LONG    total;

    pen = 0;
    total = 0;
    construct_line_by_two_points( func[from], from, func[to], to );
    for ( cnt = 0, i = from; i <= to; i++ )
    {
        x = get_x_coord( (BYTE)i );
        if ( ( dx = abs(x - (MLTPLR * func[i])) ) > 0 )
        {
            total += dx;
            cnt ++;
        }
    }
    if ( cnt )
    {
        i = total / cnt;
        i = i / MLTPLR ;
        if ( i > 5 )
            i = 5;
        pen += pen_for_convex[i];
    }
    return pen;
}

