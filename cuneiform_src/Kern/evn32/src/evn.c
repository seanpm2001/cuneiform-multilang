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

#include <math.h>
#include <string.h>
#include <fcntl.h>
/*#include <io.h>*/
#include <stdio.h>
#include <stdlib.h>
#include "evn.h"
#include "struct.h"

#include <sys/stat.h>

#include "compat_defs.h"

#ifdef WIN32
#define NAME     ".\\bin\\hnd1rus.dat"
#define NAME_RT  ".\\bin\\hnd2rus.dat"
#define NAMEP    ".\\bin\\rec1r&e.dat"
#define NAME_RTP ".\\bin\\rec2r&e.dat"
#else
#define NAME     "hnd1rus.dat"
#define NAME_RT  "hnd2rus.dat"
#define NAMEP    "rec1r&e.dat"
#define NAME_RTP "rec2r&e.dat"
#endif
//-------------- FROM DIF.DLL
extern Bool32 DIF_SetLanguage(Word8 lang);
static evn_error_code=ER_EVN_NO_ERROR;
static  char    load_tab1[256], load_tab2[256];
char alphabet[256];
Word8   language;
Bool32  enable_save_stat=FALSE;
Word8   save_event_txt[36],save_eventr_txt[36];
Word8   save_event_txts[80],save_eventr_txts[80];
Word8 *events_treeh=NULL, *events_tree_rth=NULL;  // event tables hnd
Word8 *events_treep=NULL, *events_tree_rtp=NULL;  // event tables prn
Word8 *events_tree=NULL,  *events_tree_rt=NULL;  // event tables
extern c_comp wcomp;         // working component structure
extern version* start_rec;   // ptr to answer
extern Word8 lpool[];
extern Word8 evline[], evline1[];

extern MN * c_locomp (Word8* raster, Int32 bw, Int32 h, Int16 upper, Int16 left);
extern void MN_to_line(MN *);
extern Int32 recog_letter(void);
extern Int32 recog_letter_lp(/*ExtComponent*/CCOM_comp *ec, Word8 *lp,Word16 lth);


static void *EvnAlloc(Word32 len) { return malloc(len); }
static void  EvnFree(void *ptr,Word32 len) { free(ptr); }
static Int32 GetFileLength(Int32 handle) { return filelength(handle);}

static void* (*my_alloc)(Word32 len)=EvnAlloc;
static void  (*my_free)(void *ptr,Word32 len)=EvnFree;
Int32 evn_close(void)
{
if( events_treeh )
    my_free( events_treeh, 0 );
if( events_tree_rth )
    my_free( events_tree_rth, 0 );
    events_treeh=NULL;
    events_tree_rth=NULL;
    return 1;
}

Int32 evn_close_prn(void)
{
if( events_treep )
    my_free( events_treep, 0 );
if( events_tree_rtp )
    my_free( events_tree_rtp, 0 );
    events_treep=NULL;
    events_tree_rtp=NULL;
    return 1;
}

Int32 evn_tab_init( void )
{
  Int32  h;
  Int32  size;

  evn_error_code = ER_EVN_NO_ERROR;

  h=open(NAME, O_BINARY| O_RDONLY);
  strcpy(load_tab1, NAME);
  if( h==-1 )
    {
    evn_error_code = ER_EVN_OPEN;
    return 0;
    }
  size=GetFileLength(h);
  events_treeh=(Word8 *)my_alloc( size );
  if( !events_treeh )
    {
    evn_error_code = ER_EVN_MEMORY;
    close( h );
    return 0;
    }
  if( read(h,events_treeh,size) < size )
   {
   evn_error_code = ER_EVN_READ;
   close( h );
   return 0;
   }
  ///////////////////////////////////////////////////////////

  close( h );

  h=open( NAME_RT, O_BINARY| O_RDONLY );
  strcpy(load_tab2, NAME_RT);
  if( h==-1 )
    {
    evn_error_code = ER_EVN_OPEN;
    return 0;
    }
  size=GetFileLength(h);
  events_tree_rth=(Int8 *)my_alloc( size );
  if( !events_tree_rth )
    {
    evn_error_code = ER_EVN_MEMORY;
    close( h );
    return 0;
    }
  if( read(h,events_tree_rth,size) < size )
   {
   evn_error_code = ER_EVN_READ;
   close( h );
   return 0;
   }
  close( h );
  return 1;
}


Int32 evn_tab_init_prn(const char *file1, const char *file2 )
{
  Int32  h;
  Int32  size;

  evn_error_code = ER_EVN_NO_ERROR;

  h = open_data_file(file1, O_BINARY| O_RDONLY);
  strcpy(load_tab1, file1);
  if( h==-1 )
    {
    evn_error_code = ER_EVN_OPEN;
    return 0;
    }
  size=GetFileLength(h);
  events_treep=(Int8 *)my_alloc( size );
  if( !events_treep )
    {
    evn_error_code = ER_EVN_MEMORY;
    close( h );
    return 0;
    }
  if( read(h,events_treep,size) < size )
   {
   evn_error_code = ER_EVN_READ;
   close( h );
   return 0;
   }
  ///////////////////////////////////////////////////////////

  close( h );

  h = open_data_file(file2, O_BINARY| O_RDONLY);
  strcpy(load_tab2, file2);
  if( h==-1 )
    {
    evn_error_code = ER_EVN_OPEN;
    return 0;
    }
  size=GetFileLength(h);
  events_tree_rtp=(Int8 *)my_alloc( size );
  if( !events_tree_rtp )
    {
    evn_error_code = ER_EVN_MEMORY;
    close( h );
    return 0;
    }
  if( read(h,events_tree_rtp,size) < size )
   {
   evn_error_code = ER_EVN_READ;
   close( h );
   return 0;
   }
  close( h );
  return 1;
}



int evn_active=0, evn_active_prn=0;
Word8 ev_rt_num_ln,ev_num_ln;
EVN_FUNC(Bool32)  EVNInitHND( MemFunc* mem )
{
if(	mem )
{
	my_alloc = mem->alloc;
	my_free  = mem->free;
}
else
{
	my_alloc = EvnAlloc;
	my_free	 = EvnFree;
}
if( !evn_active )
    evn_active      =   evn_tab_init();
return ( evn_active );
}

EVN_FUNC(Bool32)  EVNInitPRN( MemFunc* mem )
{
if(	mem )
{
	my_alloc = mem->alloc;
	my_free  = mem->free;
}
else
{
	my_alloc = EvnAlloc;
	my_free	 = EvnFree;
}
if( !evn_active_prn )
    evn_active_prn  =   evn_tab_init_prn(NAMEP,NAME_RTP);
return ( evn_active_prn );
}

EVN_FUNC(Bool32)  EVNInit( MemFunc* mem )
{
return ( EVNInitPRN(mem) & EVNInitHND(mem) );
}


EVN_FUNC(Bool32) EVNInitLanguage(const char *tabevn1, const char *tabevn2, Word8 lang)
{

if( evn_active_prn && language!=lang &&
    (strcmp(load_tab1,tabevn1) ||
     strcmp(load_tab2,tabevn2)) )
    { // close for new language
    evn_active_prn=0;
    evn_close_prn();
    }
if( !evn_active_prn )
    { // open and set DIF
    evn_active_prn =   evn_tab_init_prn(tabevn1,tabevn2) ;
    DIF_SetLanguage(lang);
    }
language=lang; // store new lang code
return evn_active_prn;
}

EVN_FUNC(Bool32) EVNSetLanguage( Word8 lang)
{
language=lang;
DIF_SetLanguage(lang);
return TRUE;
}


EVN_FUNC(void)  EVNDone(void)
{
if( evn_active )
	{
	evn_active=0;
	evn_close();
	}
if( evn_active_prn )
	{
	evn_active_prn=0;
    evn_close_prn();
	}

return ;
}

EVN_FUNC(Int16) EVNGetErr(void)
{
return evn_error_code;
}

EVN_FUNC(Bool32)  EVNSetAlphabet(
                        char     char_tbl_put_to[] // char table[0-255]
                   )
{
memcpy(alphabet,char_tbl_put_to,256);
return TRUE;
}

Bool32  EVNRecog(
      RecRaster   *rRaster,
      RecVersions *res
                  )
{
Int32     nvers, i, ii, nvers1;
MN *mn=NULL;
ev_num_ln=0;
evn_error_code = ER_EVN_NO_ERROR;
res->lnAltCnt = 0;	// 08.09.2000 E.P.

mn = c_locomp(rRaster->Raster,REC_GW_WORD8(rRaster->lnPixWidth),rRaster->lnPixHeight,0,0);
if( mn != NULL && mn->mnnext==NULL )
    {
    MN_to_line(mn);
    }
else
	return FALSE;
ev_num_ln= MIN(mn->mnlines,15)<<4;
nvers = 0;
nvers = recog_letter();   // to recognize
for(nvers1=0,i=0;i<nvers;i++)
	if(	alphabet[   (Word8)(start_rec+i)->let ] )
		nvers1++;

if( !nvers )
  {
  res->lnAltCnt = 0;
  evn_error_code = ER_EVN_NO_RECOG;
  return FALSE;
  }

res->lnAltCnt = nvers;
for(ii=i=0;i<nvers&&ii<res->lnAltMax;i++)
	{
	if( alphabet[   (Word8)start_rec->let ] )
		{
		res->Alt[ii].Code     = (Word8)start_rec->let;
		res->Alt[ii].CodeExt  = 0;
        res->Alt[ii].Prob     = 126 + ((ev_num_ln>4*16)+(ev_rt_num_ln>4)+(nvers1==1))*43;
		res->Alt[ii].Method   = REC_METHOD_EVN;
		ii++;
		}
  start_rec++;
  }
res->lnAltCnt = ii;
return TRUE;
}

EVN_FUNC(Bool32)  EVNRecogChar(
      RecRaster   *rRaster,
      RecVersions *res
                  )
{
events_tree_rt = events_tree_rth;
events_tree    = events_treeh;
return EVNRecog(rRaster, res );
}

EVN_FUNC(Bool32)  EVNRecogCharPRN(
      RecRaster   *rRaster,
      RecVersions *res
                  )
{
events_tree_rt = events_tree_rtp;
events_tree    = events_treep;
return EVNRecog(rRaster, res );
}


EVN_FUNC(Int32)  EVNGetNumComp(   RecRaster   *rRaster )
{
Int32     i;
MN *mn=NULL;

evn_error_code = ER_EVN_NO_ERROR;
mn = c_locomp(rRaster->Raster,REC_GW_WORD8(rRaster->lnPixWidth),rRaster->lnPixHeight,0,0);
if( !mn )
	return 0;
i=1;
while( (mn=mn->mnnext)!=NULL )
  {
  i++;
  }
return i;

}

Word8 evn_multy_lpool[6000+2];
EVN_FUNC(Word8 *)  EVNMakeLine( RecRaster   *rRaster , Int32 parm)
{
MN *mn=NULL;
Word8  *lp, *lpin, *lpend=(Word8*)(evn_multy_lpool+6000);
Word16  len,lall;

mn = c_locomp(rRaster->Raster,REC_GW_WORD8(rRaster->lnPixWidth),rRaster->lnPixHeight,0,0);
if( parm==1 )
    {
    if( mn != NULL && mn->mnnext==NULL )
        {
        lp   = (Word8*)evn_multy_lpool;
        lpin = lpool;
        MN_to_line(mn);
        while(1){
            lpin[4] += (Word8)mn->mnupper;
            len = *((Word16*)lpin);
            if( len==0 )
                break;
            if( lp+len>=lpend )
                return NULL;
            memcpy(lp,lpin,len);
            lp   += len;
            lpin += len;
            }
        lp[1]=lp[0]=0;
        lp = lpool;
        }
    else
        lp = NULL;
    }
else if( parm==0 )
    {
    if( mn != NULL )
        {
        lp   = (Word8*)evn_multy_lpool;
        do  {
            lpin = lpool;
            MN_to_line(mn);
            while(1)    {
                lpin[4] += (Word8)mn->mnupper;
                len = *((Word16*)lpin);
                if( len==0 )
                    break;
                if( lp+len>=lpend )
                    return NULL;
                memcpy(lp,lpin,len);
                lp   += len;
                lpin += len;
                }
            }
            while( (mn=mn->mnnext)!=NULL );
        lp[1]=lp[0]=0;
        lp = (Word8*)evn_multy_lpool;
        }
    else
        lp = NULL;
    }
else if( parm==2 )
    {
    if( mn != NULL )
        {
        lp   = (Word8*)&evn_multy_lpool[2];
        lall=0;
        do  {
            lpin = lpool;
            MN_to_line(mn);
            while(1)    {
                lpin[4] += (Word8)mn->mnupper;
                len = *((Word16*)lpin);
                if( len==0 )
                    break;
                if( lp+len>=lpend )
                    return NULL;
                memcpy(lp,lpin,len);
                lp   += len;
                lpin += len;
                lall +=len;
                }
            }
            while( (mn=mn->mnnext)!=NULL );
        lp[1]=lp[0]=0;
        lp[3]=lp[2]=0;
        //lp[5]=lp[4]=0;
        lp = (Word8*)evn_multy_lpool;
        *((Word16*)lp)=lall+2 ;
        }
    else
        lp = NULL;
    }
return lp;
}

EVN_FUNC(Int32)  EVNGetRepresent(
      RecRaster   *rRaster,
      Word8 *evn, Word8 *evn_rot, Int32 font              )
{
Word8   *tmp, four[6], c;
int     i,nvers1,nvers;
MN      *mn=NULL;
mn = c_locomp(rRaster->Raster,REC_GW_WORD8(rRaster->lnPixWidth),rRaster->lnPixHeight,0,0);
if( mn != NULL && mn->mnnext==NULL )
    {
    MN_to_line(mn);
    }
else
	return -1;
switch( font )
    {
    case    0:
    events_tree_rt = events_tree_rth;
    events_tree    = events_treeh;
    break;

    default  :
    case    1:
    events_tree_rt = events_tree_rtp;
    events_tree    = events_treep;
    break;
    }
enable_save_stat=TRUE;
nvers=recog_letter();
enable_save_stat=FALSE;
tmp = evn;
for(i=0;i<3;i++)
    tmp += sprintf(tmp,"%d ",save_event_txt[i]);
for(i=0;i<save_event_txt[0];i++)
    {
    c = save_event_txt[3+i];
    four[0] = '0'+((c&0xC0)>>6);
    four[1] = '0'+((c&0x30)>>4);
    four[2] = '0'+((c&0x0C)>>2);
    four[3] = '0'+((c&0x03)>>0);
    four[4] = 0;
    tmp += sprintf(tmp,"%s ",four);
    }
for(i=0;save_event_txts[i];i++)
    if(	alphabet[ save_event_txts[i] ] )
        tmp += sprintf(tmp,"%c",save_event_txts[i]);
tmp = evn_rot;
for(i=0;i<3;i++)
    tmp += sprintf(tmp,"%d ",save_eventr_txt[i]);
for(i=0;i<save_eventr_txt[0];i++)
    {
    c = save_eventr_txt[3+i];
    four[0] = '0'+((c>>4)/5);
    four[1] = '0'+((c>>4)%5);
    four[2] = '0'+((c&0xF)/5);
    four[3] = '0'+((c&0xF)%5);
    four[4] = 0;
    tmp += sprintf(tmp,"%s ",four);
    }
for(i=0;save_eventr_txts[i];i++)
    if(	alphabet[ save_eventr_txts[i] ] )
        tmp += sprintf(tmp,"%c",save_eventr_txts[i]);
for(nvers1=0,i=0;i<nvers;i++)
	if(	alphabet[   (Word8)(start_rec+i)->let ] )
		nvers1++;

return nvers1;
}

EVN_FUNC(Int32)  EVNRecog_lp(  /*ExtComponent*/CCOM_comp *ec, Word8   *lp, Word16 lth,
                               Word8   *res    )
{
Int32     nvers, i, ii;

evn_error_code = ER_EVN_NO_ERROR;
if( !evn_active_prn )
    return 0;
events_tree_rt = events_tree_rtp;
events_tree    = events_treep;

nvers = recog_letter_lp(ec,lp, lth);   // to recognize
 ec->type   =wcomp.type   ;
 ec->cs     =wcomp.cs     ;
 ec->pidx   =wcomp.pidx   ;
 ec->reasno =wcomp.reasno ;
res[0]=0;
if( !nvers )
  return 0;

for(ii=i=0;i<nvers;i++,	start_rec++)
	{
	if( alphabet[   (Word8)start_rec->let ] )
		res[ii++]    =  (Word8)start_rec->let ;
    }
res[ii]=0;

return ii;
}

EVN_FUNC(MN *) EVN_CLocomp (Word8* raster, Int32 bw, Int32 h, Int16 upper, Int16 left)
{
MN *m;
left = MAX(0,left);
m=c_locomp (raster,bw,h,upper,left);
return m;
}
extern Word8* segment_pool;
EVN_FUNC(Word8*) EVN_GetSegmentPool(void)
{
return segment_pool;
}
