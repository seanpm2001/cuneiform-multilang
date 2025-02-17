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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nt_types.h"
//#include "struct.h"
#include "status.h"
#include "cstr.h"
//#include "func.h"
#include "linear.h"
#include "ligas.h"
//#include "excdefs.h"
#include "leodefs.h"
#include "evn.h"

//////
extern Int16  prop_l_delta, prop_r_delta;
extern Word8 *letters_pidx_table;
//////////////
// proport.c    - no more !!
WORD el_pidx_crit(WORD t_let,INT pidx)
{
  WORD let;

  let = (BYTE)t_let * 2;
  if( (pidx + prop_l_delta < letters_pidx_table[let]) ||
    (pidx - prop_r_delta > letters_pidx_table[let+1]) ) return FALSE;
  else return TRUE;
}
/////////////////
// prop.c
INT prop_index(INT h, INT w)
{
 if (fax1x2)	h+=2;

 if( h <= 0 )
	 h =1;
 if( w<=0 )
	 w = 1;

 if (w <= h)
	 return ((WORD)w << 6) / h;
 else
	 return 128 - ((WORD)h << 6) / w;
}
/////////////////
static void v2_pidx_crit (CSTR_rast c)
{
 INT pidx;
// version *v, *wv;
 UniVersions vers,wvers={0};
 WORD let;
 int i,j;
 CSTR_rast_attr attr;

 CSTR_GetAttr(c,&attr);
 CSTR_GetCollectionUni(c,&vers);
 // Paul 07-12-99
 if ( vers.lnAltCnt <= 0)
   return;
 //

 attr.reasno=0;
 pidx = prop_index(attr.h, attr.w);
// wv = v = c->vers;
 wvers.lnAltMax = REC_MAX_VERS;

// while (v->let)
 for(i=j=0;i<vers.lnAltCnt;i++)
  {
   if( is_cen_language( language) && !attr.accent_leader )
    //let = (wv->let=v->let) * 2;
    let = ((WORD)vers.Alt[i].Liga) * 2;
   else
    //let = (let_sans_acc[wv->let=v->let]) * 2;
    let = (let_sans_acc[vers.Alt[i].Liga]) * 2;


//   wv->prob = v->prob;
   if (pidx + prop_l_delta < letters_pidx_table[let])
   {
	   attr.reasno |= CSTR_rn_left;
	   continue;
   }

   if (pidx - prop_r_delta > letters_pidx_table[let+1])
   {
	   attr.reasno |= CSTR_rn_right;
       continue;
   }

//   v++;
//   wv++;
   wvers.Alt[j] = vers.Alt[i];
   j++;
  }

// wv->let = 0;
 CSTR_SetAttr(c,&attr);

 wvers.lnAltCnt = j;

// if ((c->nvers -= v-wv) == 0)
 if( wvers.lnAltCnt <= 0 )
	 set_bad_cell(c);
 else
	 CSTR_StoreCollectionUni(c,&wvers);
}

////////////////////
/*
static void comp_from_kit(cell *c)
{
	c_comp * w = c->env;
	//AK! add c/g close bag c->evn == NULL
	if ( c->env )
	{
		memcpy (&wcomp,w,sizeof(c_comp));
		w = (c_comp *)((PBYTE)w + w->lines);
		lpool_lth=*(PINT)w;
		w=(c_comp *)((PINT)w+1);
		memcpy (lpool,w,lpool_lth);
		rec_ptr=start_rec;
	}
}
*/
////////////////////////////////////////////////////////////////////////////////////////////
/*
static Bool32 Ccomp2ExtComponenet(CCOM_comp *cmp, ExtComponent *ec,
								  CSTR_rast_attr *cur, int line_scale)
{
   // test cmp - ������ �� �����
  if ( (cmp->type&CCOM_CH_GREAT) || cmp->scale>2-1*0)
            return FALSE;
  if ( cur->h>PROPMAX*cur->w)
            return FALSE;
  if (!cmp->scale && (cur->w > RASTER_MAX_WIDTH ||  cur->h > RASTER_MAX_HEIGHT )     )
            return FALSE;
  if ( cmp->scale && ((cur->w>>line_scale) > RASTER_MAX_WIDTH ||
                          (cur->h>>line_scale) > RASTER_MAX_HEIGHT )     )
            return FALSE;


  ec->size     = sizeof(ExtComponent); // +cmp->size_linerep +2;

  ec->upper    = cmp->upper>>line_scale;
  ec->left     = cmp->left>>line_scale;
  ec->h        = cmp->h>>line_scale;
  ec->w        = cmp->w>>line_scale;
  ec->rw       = ((ec->w+7)/8);
  ec->type     = cmp->type;
  ec->lines    = sizeof(ExtComponent);
  ec->nl       = cmp->nl;
  ec->begs     = cmp->begs;
  ec->ends     = cmp->ends;
  ec->large    = cmp->large;
  ec->scale    = cmp->scale;

  return TRUE;
}
*/
////////////////////
/////////////////
// Tools.c
INT short_recog_cell (CSTR_rast c, int line_scale)
{
 INT n,i;
 BYTE res[20];

 CSTR_rast_attr attr;
 CCOM_comp *comp;
// ExtComponent ec;
 Int16 *pint16;
 Word8 *lpool;
 UniVersions vers={0};

  CSTR_GetAttr(c,&attr);
  comp = CSTR_GetComp(c);

 // ������� ���� ������ Tools.c -
 // ���� c->env == 0 � lpool ������ ���������� ���!

 // if( (c->cg_flag & CSTR_cg_comp) &&  !c->env )
 //   return 0;
  if ( (attr.cg_flag & CSTR_cg_comp) ||   !comp )
	 return 0;

// comp_from_kit(c);

// CCOM_comp -> to ExtComponent
/*  memset(&ec,0,sizeof(ExtComponent));
  if( !Ccomp2ExtComponenet(comp,&ec,&attr,line_scale) )
     return 0;
*/
// n = (INT)EVNRecog_lp(c->env,lpool,lpool_lth,&res[0]);

  pint16  = (Int16 *)comp->linerep;
 // *pint16 == comp->size_linerep ?????
  lpool = comp->linerep+2;
//  n = (INT)EVNRecog_lp(&ec,lpool,*pint16,&res[0]);
  n = (INT)EVNRecog_lp(comp,lpool,*pint16,&res[0]);

  vers.lnAltMax = REC_MAX_VERS;
  if( n )
  {
     for(i=0;i<n;i++)
	 {
	    //c->vers[i].let=res[i];
	    //c->vers[i].prob=254;
        vers.Alt[i].Liga = res[i];
		vers.Alt[i].Prob = 254;
		vers.Alt[i].Method = REC_METHOD_EVN;
	 }
     attr.flg             = CSTR_f_let;
     attr.recsource      |= CSTR_rs_ev;
     attr.RecogHistory   |= CSTR_rs_ev;
  }
  else
  {
    attr.flg = CSTR_f_bad;
  }

  vers.lnAltCnt = n;
  CSTR_StoreCollectionUni(c,&vers);
  CSTR_SetAttr(c,&attr);

  v2_pidx_crit(c);
  sort_vers(c);

  CSTR_GetCollectionUni(c,&vers);

 return (Int16) vers.lnAltCnt;
}
////////////////////////////
