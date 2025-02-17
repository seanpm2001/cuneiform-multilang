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

// CRLEd.cpp: implementation of the CRLEd class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include "crled.h"
#include "crlmemory.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRLEd::CRLEd()
{
	mpEdBuffer = (PWord8)RLINGLock(mhEdBuffer = RLINGAlloc(CRL_ED_BUFFER_SIZE));
	mpEdOutBuffer = (PWord8)RLINGLock(mhEdOutBuffer = RLINGAlloc(CRL_ED_BUFFER_SIZE));
	memcpy(mHalfSpaces,"\x1e\x1f",3);
	memset(&mSdd, 0, sizeof(mSdd));
	memset(&mFdd, 0, sizeof(mFdd));
	memset(&mFd, 0, sizeof(mFd));
	mbReadyToAdd = FALSE;
}

CRLEd::~CRLEd()
{
	RLINGFree(mhEdBuffer);
	RLINGFree(mhEdOutBuffer);
}

void CRLEd::Init()
{
	mpEdFileEnd = mpEdBuffer;
	mpEdFileBound = &mpEdBuffer[CRL_ED_BUFFER_SIZE];
}

Bool32 CRLEd::MakeWord(CSTR_rast b, CSTR_rast e, PWord8 pLanguage)
{
	Word8           *l,p;
	CSTR_rast       c;
	UniVersions     uni;
	Int16           k,i,n,h,wb, nlig, nl;
	CSTR_rast_attr  a;
	Int16           top,bottom,left,right;

	Init();

	for(top=10000,bottom=0,left=10000,right=0,c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		if( a.col<left )
			left = a.col;
		if( a.col+a.w>right )
			right = a.col+a.w;
		if( a.row<top )
			top = a.row;
		if( a.row+a.h>bottom )
			bottom = a.row+a.h;
		if( c==b )
        {
			*pLanguage=a.language;
        }
    }
	h = bottom-top-1;
	wb= (right-left-1+7)/8;
	//    start sheet_disk_descr
    mSdd.code     =0x0a;
    mSdd.descr_lth=0x26;
    mSdd.resolution=300;
    Write((Word8*)&mSdd,sizeof(mSdd));
	// start fragm_disk_descr
    mFdd.code=0x0b;
    mFdd.language = *pLanguage;
    mFdd.height   = h;
    mFdd.w_width  = wb;
    mFdd.kegl     = 10;
    Write((Word8*)&mFdd,sizeof(mFdd));

	// start fragm_disk
    mFd.code=0x0b;
    Write((Word8*)&mFd,sizeof(mFd));

	for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		// start bitmap ref
		mBmr.code    =0;
		mBmr.pos     =a.cpos;
		mBmr.row     =a.r_row;
		mBmr.col     =a.r_col;
		mBmr.width   =a.w;
		mBmr.height  =a.h;
		CSTR_GetCollectionUni(c,&uni);
		n=(Int16)uni.lnAltCnt;
		if(n>7) n=7;
		if(n<1) n=1;

		if( n && memchr(mHalfSpaces,uni.Alt[0].Liga,2) )
        {
			Write((Word8*)&mBmr,sizeof(mBmr));
			p=uni.Alt[0].Prob;

			if( p&1 )
				p--;

			p++;
			mVr.code = uni.Alt[0].Liga;
			mVr.prob = p;
			Write((Word8*)&mVr,sizeof(mVr));
			continue;  // half spaces for spelling analisys
        }
		nlig = strlen((char*)uni.Alt[0].Code);

		if( uni.lnAltCnt>1 && nlig>1 )
			return FALSE;

		for(nl=0;nl<nlig;nl++)
		{
			Write((Word8*)&mBmr,sizeof(mBmr));

			for(k=i=0;k<n;)
			{
				p=uni.Alt[i].Prob;
				if( p&1 )   p--;
				if( !p  )   p=2;    // all odd propability
				for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
				{ // for ligas : strlen(uni.Alt[i].Code)>1
					if( k==n-1 )
						p = p+1; // last even propability
					mVr.code = *l;
					mVr.prob =  p;
					Write((Word8*)&mVr,sizeof(mVr));
				}
			}
		}
	}

	mbReadyToAdd = TRUE;
	return TRUE;
}

Bool32 CRLEd::AddWord(CSTR_rast b, CSTR_rast e, PWord8 pLanguage)
{
	Word8           *l,p;
	CSTR_rast       c;
	UniVersions     uni;
	Int16           k,i,n, nlig, nl;
	CSTR_rast_attr  a;

	for(c=b;c && c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		if( c==b )
        {
			*pLanguage=a.language;
        }
    }

	for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		// start bitmap ref
		mBmr.code    =0;
		mBmr.pos     =a.cpos;
		mBmr.row     =a.r_row;
		mBmr.col     =a.r_col;
		mBmr.width   =a.w;
		mBmr.height  =a.h;
		CSTR_GetCollectionUni(c,&uni);
		n=(Int16)uni.lnAltCnt;
		if(n>7) n=7;
		if(n<1) n=1;

		if( n && memchr(mHalfSpaces,uni.Alt[0].Liga,2) )
        {
			Write((Word8*)&mBmr,sizeof(mBmr));
			p=uni.Alt[0].Prob;

			if( p&1 )
				p--;

			p++;
			mVr.code = uni.Alt[0].Liga;
			mVr.prob = p;
			Write((Word8*)&mVr,sizeof(mVr));
			continue;  // half spaces for spelling analisys
        }
		nlig = strlen((char*)uni.Alt[0].Code);

		if( uni.lnAltCnt>1 && nlig>1 )
			return FALSE;

		for(nl=0;nl<nlig;nl++)
		{
			Write((Word8*)&mBmr,sizeof(mBmr));

			for(k=i=0;k<n;)
			{
				p=uni.Alt[i].Prob;
				if( p&1 )   p--;
				if( !p  )   p=2;    // all odd propability

				for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
				{ // for ligas : strlen(uni.Alt[i].Code)>1
					if( k==n-1 )
						p = p+1; // last even propability

					mVr.code = *l;
					mVr.prob =  p;
					Write((Word8*)&mVr,sizeof(mVr));
				}
			}
		}
	}

	return TRUE;
}

void CRLEd::Write(PWord8 pP, Word16 wSize)
{
	if (mpEdFileBound -  mpEdFileEnd < wSize)
    {
		return;
    }

	memcpy (mpEdFileEnd, pP, wSize);
	mpEdFileEnd += wSize;
}

Bool32 CRLEd::ExcludeToVers(Int32 size, PChar8 pStr)
{
	Word8  *p,*pe;
	//struct  vers_ref     vf;
	//struct  bit_map_ref  bm;
	struct  VersRef     vf;
	struct  BitMapRef   bm;
	Int32   i;
	Bool32	new_bmp;

	//i=-1;
	i=miEdNVers=0;
	mEdVers[0].lnAltCnt=0;
	new_bmp=FALSE;
	p  = mpEdOutBuffer + sizeof(mSdd)+sizeof(mFdd)+sizeof(mFd);
	pe = &mpEdOutBuffer[size];

	for(;p<pe;)
    {
		switch( *p )
        {
        case    0:
            memcpy(&bm,p,sizeof(bm));
            p+=sizeof(bm);

			if( !new_bmp && !(!bm.row && !bm.col &&
				!bm.height && !bm.width) )
			{
                //i++;
                //memset(&ed_vers[i],0,sizeof(RecVersions));
                mEdRect[i].top    = bm.row;
                mEdRect[i].left   = bm.col;
                mEdRect[i].bottom = bm.height;
                mEdRect[i].right  = bm.width;
				new_bmp=TRUE;
			}
            break;
        default:
            memcpy(&vf,p,sizeof(vf));
            p+=sizeof(vf);
            //if( vf.code<30  )
			//  return FALSE;
            mEdVers[i].Alt[mEdVers[i].lnAltCnt].Code = vf.code;
            mEdVers[i].Alt[mEdVers[i].lnAltCnt].Prob =
                (vf.prob>1?vf.prob:254);
            if( !mEdVers[i].lnAltCnt )
                *pStr++ = vf.code;
            mEdVers[i].lnAltCnt++;
			new_bmp=FALSE;
            if( vf.prob&1 )
			{
                i++;
                mEdVers[i].lnAltCnt=0;
			}
            break;
        }


    }
	miEdNVers = i;
	for(i=0;i<miEdNVers;i++)
    {
		if( mEdVers[i].lnAltCnt && mEdVers[i].Alt[0].Code=='~' )
        {
			mEdVers[i].lnAltCnt =  0;
        }
    }
	*pStr=0;
	return TRUE;
}

Rect16 CRLEd::GetRectElement(Word32 i)
{
	Rect16 Out={0,0,0,0};

	if ( mbReadyToAdd )
		if ( /*i > -1 &&*/ i < CRL_ED_RECT_ARRAY_SIZE )
			return mEdRect[i];

	return Out;
}

RecVersions CRLEd::GetVersElement(Word32 i, PWord32 wNVers)
{
	RecVersions Out ={0,0,0};

	if ( mbReadyToAdd )
		if ( /*i > -1 &&*/ i < CRL_ED_VERS_ARRAY_SIZE )
		{
			*wNVers = miEdNVers;
			return mEdVers[i];
		}

	*wNVers = 0;
	return Out;
}
