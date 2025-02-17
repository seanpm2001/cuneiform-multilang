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

#include "cstr.h"
#include "cor_incl.h"
Bool32  incl_test_fragment(Int32 version, Int32 fragm)
{
Int32       old_bottom;
CSTR_line   line;
CSTR_attr   attr;
//return TRUE;
line = CSTR_FirstLineFragm( fragm, version );
CSTR_GetLineAttr(line,&attr);
old_bottom = attr.row+attr.hei;
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        CSTR_GetLineAttr(line,&attr);
        if( attr.row<old_bottom )
            return FALSE;
        old_bottom = attr.row+attr.hei;
        }
    }while( line );
return TRUE;
}

Int32   incl_calculate(Int32 version, Int32 fragm)
{
Int32   hist_incl[4096]={0},incline,maxi=-1,mini=4097, av, sav, i;
CSTR_line   line;
CSTR_attr   attr;

line = CSTR_FirstLineFragm( fragm, version );
CSTR_GetLineAttr(line,&attr);
incline = attr.incline;
if( 2048+incline>=0 && 2048+incline<4096 )
    {
    hist_incl[2048+incline]++;
    if( 2048+incline>maxi ) maxi =2048+incline ;
    if( 2048+incline<mini ) mini =2048+incline ;
    }
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        CSTR_GetLineAttr(line,&attr);
        incline = attr.incline;
        if( 2048+incline>=0 && 2048+incline<4096 )
            {
            hist_incl[2048+incline]++;
            if( 2048+incline>maxi ) maxi =2048+incline ;
            if( 2048+incline<mini ) mini =2048+incline ;
            }
        }
    }while( line );
for(sav=av=0,i=mini;i<=maxi;i++)
    {
    if( hist_incl[i] )
        {
        av  += hist_incl[i]*i;
        sav += hist_incl[i];
        }
    }
if( !sav )
    return -2048;
av/=sav;
return av-2048;
}

void incl_recalc_incline(CSTR_line   line, Int32 nIncline)
{
CSTR_attr       la;
CSTR_rast_attr  a;
CSTR_rast       r=CSTR_GetFirstRaster(line);
Int16           mincol, minrow, maxcol, maxrow;

CSTR_GetLineAttr(line,&la);
mincol=32000;
minrow=32000;
maxcol=-16000;
maxrow=-16000;
for(r=CSTR_GetNext(r);r;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    if( a.flg & (CSTR_f_let|CSTR_f_bad|CSTR_f_dust|CSTR_f_punct) )
        {
        a.row=a.r_row-(Int16)(nIncline*a.r_col/2048);
        a.col=a.r_col+(Int16)(nIncline*a.r_row/2048);
        if( mincol>a.col ) mincol = a.col;
        if( minrow>a.row ) minrow = a.row;
        if( maxcol<a.col+a.w ) maxcol = a.col+a.w;
        if( maxrow<a.row+a.h ) maxrow = a.row+a.h;
        }
    }
la.col=mincol;
la.row=minrow;
la.wid=maxcol-mincol;
la.hei=maxrow-minrow;
la.incline= nIncline;
CSTR_SetLineAttr(line,&la);
return;
}

void    incl_ordering(Int32 version, Int32 fragm, Int32 incline)
{
CSTR_line   line;

if( incline==-2048 )
    return;

line = CSTR_FirstLineFragm( fragm, version );
incl_recalc_incline(line,incline);
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        incl_recalc_incline(line,incline);
        }
    }while( line );
return;
}
