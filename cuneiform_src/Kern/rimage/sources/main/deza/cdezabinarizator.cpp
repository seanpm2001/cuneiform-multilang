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

// CDezaBinarizator.cpp: implementation of the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#include "cdezabinarizator.h"
#include "cribinarizator.h"
#include "crimemory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDezaBinarizator::CDezaBinarizator()
{
	mpBinarizatorControl = NULL;
	bWhitePixel = 0;
	ptbtek = NULL;
	ptbosn = NULL;
	ptb = NULL;
	ptgrey = NULL;

}

CDezaBinarizator::~CDezaBinarizator()
{

}

Word32 CDezaBinarizator::OpenTrackBin(PCTBINTigerImageInfo Info, CRIBinarizator *pCaller, Word32 tip1)
{
	Int32 lenpor;
	Int32 i;

	mpBinarizatorControl = pCaller;

	urov[0]    = urov[1] = 0;
	x = y = lg = lgn = indbl = 0;
	ypor = shed = sheds = 0;
	spx = spy = spx1 = spx2 = lg0i = 0;
	sdvig = sy1 = nstr = nstrb = 0;
	indbeg = indend = 0;
	lg0 = 0x00;

	////////////////////////////////////////////
	//	AK add
	xx = 0;
	////////////////////////////////////////////
	for (i=0; i<SB; i++)
		chet_color[i]=0L;

	colall      = 0L;
	tip         = tip1;     /* ⨯ ᪠��஢���� 0-᫠��, 1-�।��, 2-��୮ */
	bWhitePixel = Info->bFotoMetrics;
	spx         = Info->wImageWidth;
	spy         = Info->wImageHeight;
	spx2        = (spx>>1)+1;
	spx1        = (spx+7)>>3;
	sdvig       = (8-(spx & 0x0007)) & 0x0007;
	lenpor      = LONGBUF/spx;
	nstr        = lenpor;
	lenpor      = lenpor*spx;

	if ((ptgrey=(PWord8)DB_MALLOC(lenpor))==NULL)
		return(0);

	lenpor      = LONGBUF / spx1;
	nstrb       = lenpor;
	lenpor      = lenpor * spx1;

	if ((ptbosn=(PWord8)DB_MALLOC(lenpor))==NULL)
		return(0);

	if (Read_por_first()==0)
		return(0);

	sy1 = indbl = 0;
	ptbtek = ptbosn;
	return(1);
}

PWord8 CDezaBinarizator::Black_file(PWord8 anPblack, Int32 stry)
{
	Word32 ir = 0;
	PWord8 regPtb = ptb;
	PWord8 ptbend;
	PWord8 pblack = anPblack;
	Int32 shby = 0, shift = 0;

	for ( regPtb = ptgrey, ptbend = regPtb+spx*stry; regPtb != ptbend; regPtb++ )
	{
		ir = *regPtb ? (ir<<1)+1 : ir<<1;

		if ( ++shby==spx )
		{
			shby = 0;
			ir <<= sdvig;
			*pblack++=(Word8)ir;
			shift = 0;
		}
		else
			if ( ++shift == 8 )
			{
				*pblack++=(Word8)ir;
				shift = 0;
			}
	}
	return(pblack);
	/*
	unsigned short int ir, l ,len;
	short int shby, shift;
	unsigned char *ptbend;
	shby=shift=0;
	for (ptb=ptgrey, ptbend=ptb+spx*stry; ptb!=ptbend; ptb++)
	  { ir<<=1; if (*ptb) ir++;
		if (++shby==spx) { shby=0; ir<<=sdvig; shift=7; }
		if (!(++shift&0x8FF8)) continue;
	*pblack++=(unsigned char)ir;
		shift=0;
	  }
	return(pblack);
	*/
}

Int32 CDezaBinarizator::Our1(Int32 spr)
{
    PWord8 regPtb;
    Word8 regUrov;
    Int32 regSpx = spx;

    ypor=spr;
    bufmark[0]=bufmark[1]=0;

	do       //CLB:
	{
		x=bufmark[0];
		y=bufmark[1];
		regPtb = ptb=ptgrey+y*spx+x;    // use register instead of static
		regUrov = (Word8)urov[1];              // see above Stas!!!

		while ( *regPtb < regUrov )
		{
			if (++x < regSpx)
			{
				regPtb++;
				continue;
			}
			else
				if (++y>=ypor)
				{
					ptb = regPtb;       // synchronize
					return(0);
				}
				else
				{
					x=0;
					regPtb++;
				}
		}

		ptb = regPtb;               // synchronize
		lg=urov[0];
		bufmark[0]=x;
		bufmark[1]=y;
		*ptb=0x00;
		indbeg=indend=0;
		st[indend++]=x;
		st[indend++]=y;
		ptb++;
		Right_str();
		st[indend++]=shed;

		while (indbeg!=indend)
		{
			Elem_st();
			indbeg+=3;
		}
	}
	while (TRUE);  //goto CLB;
	/*
	ypor=spr; bufmark[0]=bufmark[1]=0;
	CLB: x=bufmark[0]; y=bufmark[1]; ptb=ptgrey+y*spx+x;
	while ( *ptb<urov[1] )
	{
		if (++x<spx) { ptb++; continue; }
		if (++y>=ypor) return(0);
		x=0; ptb++;
	}
	lg=urov[0]; bufmark[0]=x; bufmark[1]=y;
	*ptb=0x00; indbeg=indend=0;
	st[indend++]=x; st[indend++]=y; ptb++;
	Right_str(); st[indend++]=shed;
	while (indbeg!=indend) { Elem_st(); indbeg+=3; }
	goto CLB;
	*/
}

Int32 CDezaBinarizator::Xygo(Int32 x1, Int32 y1)
{
	if (y1 >= ypor || y1 < 0 || x1 >= spx || x1 < 0)
		return(0);

	x   = x1;
	y   = y1;
	ptb = ptgrey + y * spx + x;
	return(1);
}

Int32 CDezaBinarizator::Grey_black()
{
	Int32 spypic, sy2, spybg;

	spybg = spypic = ((sy1 + nstrb) <= spy) ? nstrb : (spy-sy1);
	sy2 = ((sy1 + nstr) <= spy) ? nstr : (spy - sy1);

	while(spypic!=0)
	{
		if (sy2 > spypic)
			sy2 = spypic;

		if ( DB_GREYREAD(ptgrey, sy1, sy2) != sy2)
			return(0);

		Raspred(sy2);
		sy1  += sy2;
		Get1();
		Our1(sy2);
		ptbtek = Black_file(ptbtek,sy2);
		spypic -= sy2;
	}

	ptbtek = ptbosn;
	indbl = 0;
	return(spybg);
}

Int32 CDezaBinarizator::Elem_st()
{
	Int32 i;

	if (Xygo(st[indbeg], st[indbeg+1]+1)==0)
		return(0);

	bufmark[2]=x;
	bufmark[3]=y;
	Left_str();

	if (sheds)
	{
		Stek();
		st[indend++]=x;
		st[indend++]=y;
		st[indend++]=sheds;
		x=bufmark[2];
		y=bufmark[3];
		ptb=ptgrey+y*spx+x;
	}

	i=0;

	while (i <= st[indbeg+2])
	{
		i++;

		if (*ptb>=lg)
		{
			Stek();
			st[indend++]=x;
			st[indend++]=y;
			*ptb++=0x00;
			Right_str();
			i+=shed;
			st[indend++]=shed;
		}
		else
		{
			if (++x<spx)
				ptb++;
			else
			{
				x--;
				break;
			}
		}
	}
	return(1);
}

Int32 CDezaBinarizator::Read_por_first()
{
	Int32 str, i, j;

	i = spy>>1;
	str = (nstr<i) ? nstr:i;
	str=(str<=spy/6) ? str:spy/6;
	colall=1L*spx*str;
	j=spy/2;

	if (tip==2)
		j=spy-str;

	for (i=0; i<j; i+=str)
    {
		if (DB_GREYREAD(ptgrey, i, str) != str)
			return(0);

		Ras1_pred(str);
	}

	if (DB_GREYREAD(ptgrey,spy - str,str) != str)
		return(0);

	Ras1_pred(str);

	for (lg0i=1; lg0i<SB; lg0i++)
		if (chet_color[lg0i]!=0L)
			break;

	lgn = lg0i+1;
	lg0 = (Word8)lg0i;
	chet_color[lg0i] += chet_color[0];
	chet_color[0] = 0L;
	colall -= chet_color[lg0i];
	return(1);
}

Int32 CDezaBinarizator::Get1()
{
	Int32 i, j, imax, lg1, pg1, lg2, pg2, pgn;
	Int32 irl, srz, chet[256], srz1, srz2;

	for (i=0; i<256; i++)
		chet[i]=chet_color[i];

	srz=colall/100L;
	irl=0L;

	for (pgn=255; irl<srz; pgn--)
		irl+=chet[pgn];

	for (i=lgn,srz=0L; i<=pgn; i++)
		srz+=chet[i];

	if (lgn==pgn)
		pgn++;

	srz=(srz+(pgn-lgn)/2)/(pgn-lgn);

	for (imax=lgn; chet[imax]<srz; imax++) ;

	for ( ; imax<=pgn; imax++)
		if (chet[imax]>chet[imax+1])
			break;

	for (i=imax+1,srz=0L; i<=pgn; i++)
		srz+=chet[i];

	if (pgn==imax)
		pgn++;

	if ((pgn-lgn)<128)  /* ������ ��� ᫠��� ⥪�⮢ */
	{
		srz1=chet[lg0i]*100L/colall;

		if (srz1<500L)
		{
			srz=colall/10L;

			for  (urov[0]=lgn, irl=0L; irl<srz; urov[0]++)
				irl+=chet[urov[0]];

			urov[1]=urov[0]+10;
			return(0);
		}
	}

	for (lg1=imax+1,srz=(srz+(pgn-imax)/2)/(pgn-imax);lg1<=pgn;lg1++)
		if (chet[lg1]<=srz && chet[lg1+1]<=srz)
			break;

	for (i=lg1+1,srz=0L;i<=pgn;i++)
		srz+=chet[i];

	if (pgn==lg1)
		pgn++;

	for (pg2=pgn,srz=(srz+(pgn-lg1)/2)/(pgn-lg1); pg2>lg1; pg2--)
		if (chet[pg2]>=srz && chet[pg2-1]>=srz && chet[pg2-2]>=srz)
			break;

	for (; pg2>lg1; pg2--)
		if (chet[pg2]<=srz && chet[pg2-1]<=srz && chet[pg2-2]<=srz)
			break;

	for (i=lg1+1,srz=0L; i<=pg2; i++)
		srz+=chet[i];

	if (pg2==lg1)
		pg2++;

	for (pg1=lg1,srz=(srz+(pg2-lg1)/2)/(pg2-lg1); pg1<pg2; pg1++)
		if (chet[pg1]<=srz && chet[pg1+1]<=srz && chet[pg1+2]<=srz)
			break;

	for (i=pg1+1,srz=0L; i<=pg2; i++)
		srz+=chet[i];

	if (pg2==pg1)
		pg2++;

	for (lg2=pg2,srz=(srz+(pg2-pg1)/2)/(pg2-pg1); lg2>pg1; lg2--)
		if (chet[lg2]<=srz && chet[lg2-1]<=srz && chet[lg2-2]<=srz)
			break;

	if (lg2==pg1)
	{
		xx=pg1;
		//goto Gt1;
	}
	else
	{
		for (i=pg1, irl=2L*chet[pg1]; i<=lg2; i++)
			if ((srz=labs(chet[i]-2L*chet[pg1]*(lg2-i)/(lg2-pg1)))<=irl)
			{
				irl=srz;
				xx=i;
			}
	}
//Gt1:
	srz=colall/25L;

	if ((pgn-lgn)<128 && xx>128)
		tip=0;

	for (j=xx, irl=0L; irl<srz; j--)
		irl+=chet[j];

	urov[0]=j;
	srz=colall/50L;

	for (j=xx, irl=0L; irl<srz; j++)
		irl+=chet[j];

	urov[1]=j;

	if (tip==0)
	{
		srz1=colall/20L;
		srz2=colall/10L;

		for (xx=lgn, irl=0L; irl<srz1; xx++)
			irl+=chet[xx];

		urov[0]=xx;

		for (j=xx, irl=0L; irl<srz2; j++)
			irl+=chet[j];

		urov[1]=j;
	}
	return(0);
}

void CDezaBinarizator::Right_str()
{
	shed=0;

	if (++x<spx)
		while (*ptb>=lg)
		{
			*ptb++=0x00;
			shed++;

			if (++x>=spx)
				break;
		}

	x--;
	ptb--;
}

void CDezaBinarizator::Left_str()
{
	sheds=0;
	ptb--;

	if (--x>=0)
		while (*ptb>=lg)
		{
			*ptb--=0x00;
			sheds++;

			if (--x<0)
				break;
		}

	x++;
	ptb++;
}

void CDezaBinarizator::Stek()
{
	Int32 r, ir;

	if (indend==BUFS)
	{
		if (indbeg==BUFS)
			indend=0;
		else
		{
			for (r = indbeg, ir = 0; r < (Int32)indend; r++, ir++)
				st[ir]=st[r];

			indend-=indbeg;
		}

		indbeg=0;
	}
}

void CDezaBinarizator::Raspred(Int32 str)
{
	PWord8 regPtb;
	Word8 regLg0 = lg0;
	Word32 i, lineCount;
	Int32 l;

	l = chet_color[lg0i];

	for ( lineCount = 0, regPtb=ptgrey; lineCount < (Word32)str ; lineCount++ )
	{
		for( i = 0 ; i <= (Word32)spx2 ; i++, regPtb++ )
		{
			if ( *regPtb < regLg0 )
				*regPtb=regLg0;

			chet_color[*regPtb]++;
		}

		for( ; i < (Word32)spx ; i++, regPtb++ )
		{
			if ( *regPtb < regLg0 )
				*regPtb=regLg0;
		}
	}

	colall = colall + 1L * spx2 * str - (chet_color[lg0i] - l);
	ptb    = regPtb;
	/*
	unsigned short int len, i;
	long l;
	l=chet_color[lg0i]; len=spx*str;
	for (i=0, ptb=ptgrey; i<len; i++, ptb++)
	{ if (*ptb<lg0) *ptb=lg0;
	if ( (i%spx) <=spx2) chet_color[*ptb]++;
	}
	colall=colall+1L*spx2*str-(chet_color[lg0i]-l);
	*/
}

void CDezaBinarizator::Ras1_pred(Int32 str)
{
	PWord8 regPtb;
    Int32 i;
	Int32 regSpx2 = spx2;
	Int32 lineCount;

    for ( lineCount = str, regPtb=ptgrey; lineCount > 0 ; lineCount-- )
	{
        for( i=0 ; i <= regSpx2 ; i++, regPtb++ )
		{
            chet_color[*regPtb]++;
        }

		i += (spx - spx2 - 1);
		regPtb += (spx - spx2 - 1);
    }

	colall += 1L * spx2 * str;
	/*
	unsigned short int len, i;
	len=spx*str;

	for (i=0, ptb=ptgrey; i<len; i++,ptb++)
		if ( (i%spx) <=spx2) chet_color[*ptb]++;

	colall+=1L*spx2*str;
	*/
}

Int32 CDezaBinarizator::GetBinarized(PWord8 ptbl, Word32 lenbl)
{
	Int32 spybr, ret, nstrb1;
	PWord8 ptbend;

	spybr  = lenbl/spx1;
	nstrb1 = spybr+indbl-nstrb;
	spybr  = (nstrb1<=(spy-sy1)) ? spybr:(spy-sy1+nstrb-indbl);
	ret    = 0;

	do      //GB1:
	{
		if (ptbtek==ptbosn)
			if ((nstrb=Grey_black())==0)
				return(ret);

		nstrb1 = nstrb-indbl;

		if (spybr<=nstrb1)
		{
			if(bWhitePixel)// Piter see flag invert
				for (ptbend=ptbtek+spx1*spybr; ptbtek!=ptbend;)
					*ptbl++=~*ptbtek++;
			else
				for (ptbend=ptbtek+spx1*spybr; ptbtek!=ptbend;)
					*ptbl++=*ptbtek++;

			indbl += spybr;

			if (indbl==nstrb/*1*/)
				ptbtek=ptbosn;// Piter new

			// indbl+=spybr; if (indbl==nstrb1) ptbtek=ptbosn;// Piter Old string
			return(ret+spybr);
		}

		if(bWhitePixel)// Piter see flag invert
			for (ptbend=ptbtek+spx1*nstrb1; ptbtek!=ptbend;)
				*ptbl++=~*ptbtek++;
		else
			for (ptbend=ptbtek+spx1*nstrb1; ptbtek!=ptbend;)
				*ptbl++=*ptbtek++;

		ptbtek = ptbosn;
		spybr  -= nstrb1;
		ret    += nstrb1;
	} while( TRUE );         //goto GB1;
}

Int32 CDezaBinarizator::CloseTrackBin()
{
	DB_FREE(ptgrey);
	DB_FREE(ptbosn);
	ptgrey = NULL;
	ptbosn = NULL;
	return(1);
}
