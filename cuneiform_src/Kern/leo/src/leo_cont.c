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

// container fot RecRasters
#include<stdio.h>
#include<string.h>
#include "ctb.h"
#include "std.h"
#include "leo.h"
#include "leo_func.h"
#define COLLECT_CTB1
static char         cont_name[256];
static CTB_handle   this_ctb={0};

extern int  LEO_error_code;
extern  Word8 field_number;
extern  Word8 leo_current_alpha_ndx;
void leo_close_cont(void)
{
if( this_ctb.bas )
    {
    CTB_close(&this_ctb);
    memset(&this_ctb,0,sizeof(this_ctb));
//    CTB_unlink(cont_name);
    cont_name[0]=0;
    }

return;
}

void leo_close_cont_temp(void)
{
if( this_ctb.bas )
    {
    CTB_close(&this_ctb);
#ifdef COLLECT_CTB
    CTB_copy(".\\tmp\\tc999999",".\\tmp\\ct999999");
#endif
    memset(&this_ctb,0,sizeof(this_ctb));
    }

return;
}

int leo_open_cont_temp(void)
{
if( !this_ctb.bas && cont_name[0])
    {
    return CTB_open(cont_name,&this_ctb,"w");
    }

return 0;
}

Bool32 leo_cont_new_page(Int32 id_page)
{
Bool32 op=TRUE;
if( this_ctb.bas )
    {
    CTB_close(&this_ctb);
    memset(&this_ctb,0,sizeof(this_ctb));
    cont_name[0]=0;
    }

if( id_page )
    {
    sprintf(cont_name,".\\tmp\\ct%06d",id_page);
    op=CTB_create(cont_name,NULL);
    if( op )
    op = CTB_open(cont_name,&this_ctb,"w");
    }
else
    {
    cont_name[0]=0;
    }
if( !op )
    {
    LEO_error_code=ER_LEO_OPEN;
    cont_name[0]=0;
    return FALSE;
    }

return TRUE;
}

// save to current page = CTB_file_name
Int16 leo_cont_store(RecRaster *r,Word8 let, Word8 nLns,Rect16 *rect,Word8 IsPrint,
		Word8	Prob, Word8 Valid, RecVersions *v,Word8 control)
{
Int32       num;
Word8       raster[4096],data[CTB_DATA_SIZE]={0};
Int32       wb, k;
Bool32      ret;
RecVersions ver;

if( !cont_name[0] || !this_ctb.bas )
    return 0;
data[0] = CTB_OEM_CHARSET;
data[1] = (Word8)r->lnPixWidth;
data[2] = (Word8)r->lnPixHeight;
data[3] = stdAnsiToAscii(let);
#if CTB_VERSION==7
data[32]         = nLns;
#else
data[4]          = nLns;
#endif
data[5] = IsPrint;
memcpy(&data[6],rect,sizeof(Rect16));
data[14] = Prob;
data[15] = Valid;
if( v )
    data[28] = v->Alt[0].Method;
if( v )
    {
    ver=*v;
    if( ver.lnAltCnt>4 )
        ver.lnAltCnt=4;
    data[16]=(Word8)ver.lnAltCnt;
    for(k=1;k<ver.lnAltCnt;k++)
        {
        data[17+k*2]= stdAnsiToAscii(ver.Alt[k].Code);
        data[18+k*2]= ver.Alt[k].Prob;
		data[28+k]  = ver.Alt[k].Method;
        }
    }
data[25] = field_number;
data[26] = leo_current_alpha_ndx;
data[27] = control;
wb = ((r->lnPixWidth+63)/64)*8;
memcpy( raster, r->Raster, wb*r->lnPixHeight);
CTB_align1_lines(raster, r->lnPixWidth, r->lnPixHeight);
ret = CTB_write( &this_ctb, -1, raster,  data);
if( !ret )
    return -1;
num = this_ctb.num;

return (Int16)num;  // id_rast = last sequentaly saved raster+1
}

Bool32 leo_cont_restore(RecRaster *r, Int32 id_page, Int16 id_rast, Word8 *nLns,Rect16 *rect,Word8 *IsPrint, Word8 *datao)
{
Word8       raster[4096],w,h,data[CTB_DATA_SIZE];
Int32       wb,ww,hh;
Bool32      ret;
if( !id_page )
    return FALSE;

ret = CTB_read( &this_ctb, id_rast-1, raster,  data);
if( !ret )
    return FALSE;

w = data[1];
h = data[2];
ww=w; hh=h;
wb = (w+7)/8;
memcpy( r->Raster, raster, wb*hh);
r->lnRasterBufSize = REC_MAX_RASTER_SIZE;
r->lnPixWidth  = ww;
r->lnPixHeight = hh;
#if CTB_VERSION==7
*nLns          = data[32];
#else
*nLns          = data[4];
#endif
*IsPrint       = data[5];
memcpy(rect,&data[6],sizeof(Rect16));
if( datao )
    memcpy(datao,data,CTB_DATA_SIZE);
CTB_align8_lines(r->Raster, r->lnPixWidth, r->lnPixHeight);

return ret;
}


Bool32 leo_cont_set_valid(Int32 id_page, Int16 id_rast, Word8 code, Word8 valid,Word8 control)
{
Word8       raster[4096],data[CTB_DATA_SIZE];
Bool32      ret;
if( !id_page )
    return FALSE;

ret = CTB_read( &this_ctb, id_rast-1, raster,  data);
if( !ret )
    return FALSE;
if( code && data[3]!=code )
	{
    data[3] = code;
	}
if( valid & LEO_VALID_LINGVO )
	{
	data[28]= REC_METHOD_CNT;
	}
data[15] |= valid;
data[27] |= control;
CTB_write_data(&this_ctb, id_rast-1, data);

return ret;
}

Bool32 leo_cont_del_final(Int32 id_page, Int16 id_rast)
{
Word8       raster[4096],data[CTB_DATA_SIZE];
Bool32      ret;
if( !id_page || !id_rast)
    return FALSE;

ret = CTB_read( &this_ctb, id_rast-1, raster,  data);
if( !ret )
    return FALSE;
data[15] ^= LEO_VALID_FINAL;
CTB_write_data(&this_ctb, id_rast-1, data);

return ret;
}

Bool32 leo_cont_store_collection(RecVersions *ver)
{
Int16       id_rast;
Int32       k;
Word8       data[CTB_DATA_SIZE]={0};

if( !cont_name[0] || !this_ctb.bas || ver->lnAltCnt<1 )
    return FALSE;

id_rast = ver->Alt[0].Info;
if( !id_rast )
   return FALSE;

if( !CTB_read_data( &this_ctb, id_rast-1, data) )
   return FALSE;

data[3]  = stdAnsiToAscii(ver->Alt[0].Code);
data[14] = ver->Alt[0].Prob;
data[28] = ver->Alt[0].Method;
if( ver->lnAltCnt>4 )
   ver->lnAltCnt=4;
data[16]=(Word8)ver->lnAltCnt;
for(k=1;k<ver->lnAltCnt;k++)
   {
   data[17+k*2]= stdAnsiToAscii(ver->Alt[k].Code);
   data[18+k*2]= ver->Alt[k].Prob;
   data[28+k]  = ver->Alt[k].Method;
   }

if( !CTB_write_data( &this_ctb, id_rast-1,  data) )
    return FALSE;

return TRUE;  // id_rast = last sequentaly saved raster+1
}
