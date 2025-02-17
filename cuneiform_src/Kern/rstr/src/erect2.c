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

//////////////////////////////////////////////////////////////////
// ERECT2 : ERECTION OF UNDEFINED CHARS WHITHOUT CLUSTERS       //
//////////////////////////////////////////////////////////////////
//         MAIN FUNCTION : cstr_erection_pass2                  //
//////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <string.h>

#include"cstr.h"
#include"fon.h"
#include"ctb.h"
#include"tuner.h"
#include"ligas.h"
#include"nt_types.h"	// 17.07.2001 E.P.
#include"status.h"		// 17.07.2001 E.P.

#include"compat_defs.h"

// SMAP.C
extern	Bool16	snap_monitor_ori(CSTR_line *snap_line, Int32 num_lines);
extern	Bool16	snap_activity(Word8 a);
extern	Word8	db_status;
extern	Bool16	snap_show_text(Word8 *txt);
// ERECTION.C
extern  Int16   erect_get_global(void);
extern  Int16   erect_get_represent(void);
extern  Int16   erect_get_words(void);
// RCM.C
extern Word8 decode_ASCII_to_[256][4];
extern Word8 line_tabcell;
// p2_cour.c
extern Bool32   p2_msk_inc(CSTR_rast    rst);
#define etap_name 'i'

#define MAX_LEN_WORD  48

static Word8 left_limit_word[] =" .";
static Word8 right_limit_word[]=" .,"
"�"
   "��\'\":/\x1f";
static CSTR_rast erect_next_word(CSTR_rast cs)
{
CSTR_rast       c=cs;
CSTR_rast_attr  attr;
RecVersions     vers;

CSTR_GetAttr(c,&attr);
if( attr.flg==CSTR_f_fict && attr.col==0x7fff )
    return (CSTR_rast)0; // last rast
do{
  c=CSTR_GetNext(c);
  CSTR_GetAttr(c,&attr);
  CSTR_GetCollection(c,&vers);
  }while( c && vers.lnAltCnt > 0 &&  // Nick 12.03.2001
	      memchr(left_limit_word,vers.Alt[0].Code,sizeof(left_limit_word)) &&
          !(attr.flg&CSTR_f_fict) );

return (attr.flg&CSTR_f_fict)?(CSTR_rast)0:c;
}

static CSTR_rast erect_end_word(CSTR_rast cs,Word8 *str,Word8 *word_len,Int16 right_limit, Bool32 need_space)
{
CSTR_rast       c=cs, n;
CSTR_rast_attr  attr, nattr;
Int16           i, dif;
RecVersions     vers;

dif=1;
CSTR_GetCollection(c,&vers);

*str++  = vers.lnAltCnt>0?vers.Alt[0].Code:'~';  // Nick 12.03.2001
i=1;
do{
  c=CSTR_GetNext(c);
  if( !c )
    break;
  CSTR_GetCollection(c,&vers);
  *str++  = vers.lnAltCnt>0?vers.Alt[0].Code:'~';
  CSTR_GetAttr(c,&attr);
  i++;
  }while( c &&
	      !memchr(right_limit_word,vers.lnAltCnt>0?vers.Alt[0].Code:'~',sizeof(right_limit_word)) &&
          attr.col+attr.w<=right_limit+dif &&
          !(attr.flg&CSTR_f_fict) && i<MAX_LEN_WORD-1);

if(       c &&
          !memchr(right_limit_word,vers.lnAltCnt>0?vers.Alt[0].Code:'~',sizeof(right_limit_word)) &&
          attr.col+attr.w-right_limit<attr.w/2+dif &&
          !(attr.flg&CSTR_f_fict) && i<MAX_LEN_WORD-1 )
    {
    c=CSTR_GetNext(c);
    CSTR_GetCollection(c,&vers);
    *str++  = vers.lnAltCnt>0?vers.Alt[0].Code:'~';
    CSTR_GetAttr(c,&attr);
    i++;
    }
*str='\0';
if( need_space && c && !(attr.flg&CSTR_f_fict) )
    {
    n = CSTR_GetNext(c);
    CSTR_GetAttr(n,&nattr);
    if( nattr.flg&(CSTR_f_let|CSTR_f_bad|CSTR_f_punct) )
        {
        Int16   dist=MIN(attr.h,nattr.h);
        dist = MIN( dist, attr.w);
        dist = MIN( dist,nattr.w);
        if( nattr.col - (attr.col+attr.w)<dist/3 )
            return (CSTR_rast)0;
        }
    }
c=c ? c : CSTR_GetLastRaster(CSTR_GetRasterLine(cs));
if( i )
    {
    i--;
    str--;
    }
*str='\0';
*word_len = (Word8)i;
return c;
}

static Bool32 erect_no_incline(CSTR_rast beg, CSTR_rast end,
        Bool32 inc)
{
CSTR_rast       r;
CSTR_rast_attr  a;
for(r=beg;r && r!=end; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    if( inc )
        {
        if( !(a.font&CSTR_fp_it) )
            return TRUE;
        }
    else
        {
        if( (a.font&CSTR_fp_it) )
            return TRUE;
        }
    }
return FALSE;
}

static Bool32 erect_valid_font(CSTR_rast beg, CSTR_rast end,
    Bool32 inc, Bool32  small_inc)
{
#ifdef _USE_FON_
CSTR_rast       r;
CSTR_rast_attr  attr;
RecRaster       recRast;
UniVersions     u;
FonTestInfo     Attribut[MAXCHECKALT];
Int32           nval, nall, na, nalt;

for(nalt=nall=nval=0,r=beg;r && r!=end; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&attr);
    if( !(attr.flg & CSTR_f_let) )
        continue;
    CSTR_GetCollectionUni(r,&u);
    CSTR_GetImage(r,(Word8*)&recRast, CSTR_TYPE_IMAGE_RS);
    if( u.lnAltCnt &&
        (na=FONTestChar(&recRast,u.Alt[0].Liga, &Attribut[0],u.Alt[0].Info))>0 &&
        Attribut[0].prob>180  )
        {
		// Nick 20.9.99 - ���������, ����� �� ������� ������

		attr.clink    = Attribut[0].prob;
		attr.clu_attr = Attribut[0].flag;
		attr.nClust   = Attribut[0].nClust;
        CSTR_SetAttr(r,&attr);
		///////

        if( na>1 && Attribut[0].prob-Attribut[1].prob<10 &&
            (Attribut[0].flag & CTB_PRINT_ITALIC) &&
           !(Attribut[1].flag & CTB_PRINT_ITALIC) )
            continue;
        if( inc )
            { // set inc
            if( (Attribut[0].flag & CTB_PRINT_ITALIC) ||
                (!line_tabcell && p2_msk_inc(r)) )
            nval++;
            }
        if( !inc  )
            { // clear inc
            if( !(Attribut[0].flag & CTB_PRINT_ITALIC) )
                nval++;
            if( (!line_tabcell&&p2_msk_inc(r)) )
                nalt++;
            }
        }
    else
        {
        if( inc && !line_tabcell && p2_msk_inc(r))
            nval++;
        }
    nall++;
    }
//if( nall==1 ) //???
if( nall<4 && nall && nval==nall && nalt<1 )
    return TRUE;
if( !small_inc )
    {
    if( inc && nall>3 )
        {
        if( nval*25>nall*14 )
            return TRUE;
//        if( nall==4 && nval>1 )
//            return TRUE;
        }
    if( !inc && nall>3 && nval*3>=nall*2 && nalt<1 )
        return TRUE;
    }
#endif
return FALSE;
}

static Bool32 erect_rotate(CSTR_rast beg,CSTR_rast  end, Int16 inc)
{
CSTR_rast       r;
CSTR_rast_attr  a;
for(r=beg;r && r!=end; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    a.stick_inc = 0;
    a.save_stick_inc = inc;
    a.pos_inc = CSTR_erect_rest;
    a.font |= CSTR_fp_it;
    if( a.font & CSTR_fp_str )
        a.font -= CSTR_fp_str;
    CSTR_SetAttr(r,&a);
    }
return TRUE;
}

static Bool32 erect_clear(CSTR_rast beg,CSTR_rast  end, Int16 inc)
{
CSTR_rast       r;
CSTR_rast_attr  a;
for(r=beg;r && r!=end; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    a.stick_inc = 0;
    a.save_stick_inc = 0;
    a.pos_inc = CSTR_erect_no;
    if( a.font & CSTR_fp_it )
        a.font -= CSTR_fp_it;
    a.font |= CSTR_fp_str;
    CSTR_SetAttr(r,&a);
    }
return TRUE;
}

static Int16 erect_min_inc(CSTR_rast beg)
{
CSTR_rast       r;
CSTR_rast_attr  a;
Int16           mininc=NO_INCLINE;
for(r=beg;r ; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    if( a.font & CSTR_fp_it )
        {
        if(mininc>a.save_stick_inc )
            mininc=a.save_stick_inc;
        }
    }
return mininc;
}

Bool32	cstr_erection_pass2(CSTR_line lout)
{
Bool32      ret=FALSE;
CSTR_rast   beg,end;
Word8       len, wrd[MAX_LEN_WORD+40];
Int16       ginc = erect_get_global();
Int16       gninc= erect_get_represent();
Int16       gall = erect_get_words();
Int16       min_inc=erect_min_inc(CSTR_GetFirstRaster(lout));

if( gninc>1 )
{
if( !ginc ) // local inc ???
    {
    if( db_status && snap_activity(etap_name) )
        {
        snap_show_text("Can't erection on pass2");
	    snap_monitor_ori(&lout,1);
	    }
    return FALSE;
    }
if( db_status && snap_activity(etap_name) )
    {
    snap_show_text("Before erection on pass2");
	snap_monitor_ori(&lout,1);
	}

end = CSTR_GetFirstRaster(lout);
while(1)
    {
    beg = erect_next_word(end);
    if( !beg )
        break;
    end = erect_end_word(beg,wrd,&len,32767,FALSE);
    if( erect_no_incline(beg, end, TRUE) &&
        erect_valid_font(beg, end, TRUE, FALSE) )
        {
        erect_rotate(beg, end, ginc);
        if( db_status && snap_activity(etap_name) )
            {
            snap_show_text(wrd);
		    snap_monitor_ori(&lout,1);
		    }
        }
    }


}
if( gninc<4 || // 3
    gninc<5 && gall>280 ||  // �� ����
    gninc<6 && gall>300 ||  // <4 && >400
    min_inc<300)
{
if( db_status && snap_activity(etap_name) )
    {
    snap_show_text("Before clear erection on pass2");
	snap_monitor_ori(&lout,1);
	}
end = CSTR_GetFirstRaster(lout);
while(1)
    {
    beg = erect_next_word(end);
    if( !beg )
        break;
    end = erect_end_word(beg,wrd,&len,32767,FALSE);
    if( erect_no_incline(beg, end, FALSE) &&
        erect_valid_font(beg, end, FALSE, FALSE) )//(min_inc==256)) )
        {
        erect_clear(beg, end, ginc);
        if( db_status && snap_activity(etap_name) )
            {
            snap_show_text(wrd);
		    snap_monitor_ori(&lout,1);
		    }
        }
    }
if( db_status && snap_activity(etap_name) )
    {
    snap_show_text("After clear erection on pass2");
	snap_monitor_ori(&lout,1);
	}
}
return ret;
}

Bool32 non_near_letters(RecVersions *v)
{
Word8   v0, v1, p0, p1;

// 21.05.2002 E.P.
if (is_baltic_language(language) ||
    is_turkish_language(language)
   ) return TRUE;

if( v->lnAltCnt<2 )
    return TRUE;

v0 = decode_ASCII_to_[v->Alt[0].Code][0];
v1 = decode_ASCII_to_[v->Alt[1].Code][0];
p0 = v->Alt[0].Prob;
p1 = v->Alt[0].Prob;

// ������ ��������. 21.05.2002 E.P.
//if( strchr("����",v0) && strchr("����",v1) && abs(p0-p1)<15 &&
if( strchr("����",v0) && strchr("����",v1) && abs(p0-p1)<15 )
    return FALSE;

if( strchr("����",v0) && strchr("����",v1) && abs(p0-p1)<15 )
    return FALSE;

return TRUE;
}

void p2_add_probs(CSTR_line ln)
{
CSTR_rast       r;
CSTR_rast_attr  a;
UniVersions     u;
Int32           p, p0, i;

for(r=CSTR_GetNext(CSTR_GetFirstRaster(ln));r ; r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( (a.RecogHistory & CSTR_hi_fon) &&
        u.lnAltCnt &&
        u.Alt[0].Method!=REC_METHOD_FON &&
        a.clink>200 )
        {
        p0 = u.Alt[0].Prob;
        u.Alt[0].Prob = (Word8)a.clink ;
        for(i=1;i<u.lnAltCnt;i++)
            {
            p=u.Alt[i].Prob;
            u.Alt[i].Prob = (Word8)(p*a.clink/p0);
            }
        CSTR_StoreCollectionUni(r,&u);
        }
        /*
    if( !(a.RecogHistory & CSTR_hi_fon) &&
        (a.clink>200 && a.flg&CSTR_f_confirmed) )
        {
        a.RecogHistory |= CSTR_hi_fon;
        CSTR_SetAttr(r,&a);
        p0 = u.Alt[0].Prob;
        u.Alt[0].Prob = (Word8)a.clink ;
        for(i=1;i<u.lnAltCnt;i++)
            {
            p=u.Alt[i].Prob;
            u.Alt[i].Prob = (Word8)(p*a.clink/p0);
            }
        CSTR_StoreCollectionUni(r,&u);
        }
        */
    }
return;
}
