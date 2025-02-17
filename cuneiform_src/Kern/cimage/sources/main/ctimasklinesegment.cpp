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

// CTIMaskLineSegment.cpp: implementation of the CTIMaskLineSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "ctimasklinesegment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTIMaskLineSegment::CTIMaskLineSegment()
                   : mpNext(NULL),
				     mwStart(-1),
					 mwEnd(-1)
{
}

CTIMaskLineSegment::~CTIMaskLineSegment()
{

}

CTIMaskLineSegment::CTIMaskLineSegment(Int32 Start, Int32 End)
                   : mpNext(NULL),
				     mwStart(-1),
					 mwEnd(-1)
{
	if ( Start >= 0 && End >= 0 && Start <= End)
	{
		mwStart = Start;
		mwEnd   = End;
	}
}

CTIMaskLineSegment::CTIMaskLineSegment(PCTIMaskLineSegment pSegm)
                   : mpNext(pSegm->GetNext()),
				     mwStart(pSegm->GetStart()),
					 mwEnd(pSegm->GetEnd())
{
}

Word32 CTIMaskLineSegment::IsIntersectWith(PCTIMaskLineSegment pSegm)
{
	Word32 Intrsct = 0;
	Word32 S;
	Word32 E;
	Int32  iDS;
	Int32  iDE;

	if ( pSegm )
	{
		S = pSegm->GetStart();
		E = pSegm->GetEnd();
		iDS = GetPointDirect(S);
		iDE = GetPointDirect(E);

		if ( IsEqual(pSegm)	)
			Intrsct = CTIMLSEGMINTERSECTEQUAL;
		else
		{
			if ( iDS == CTIMLSEGMPOINTLEF &&
				 iDE == CTIMLSEGMPOINTRIGHT )
				 Intrsct = CTIMLSEGMINTERSECTOVER;
			else
			{
				if ( IsPointInSegment(S) )
				{
					if ( IsPointInSegment(E) )
						Intrsct = CTIMLSEGMINTERSECTIN;
					else
						Intrsct = CTIMLSEGMINTERSECTRIGHT;
				}
				else
				{
					if ( IsPointInSegment(E) )
						Intrsct = CTIMLSEGMINTERSECTLEFT;
					else
						if ( iDS == CTIMLSEGMPOINTLEF &&
							 iDE == CTIMLSEGMPOINTLEF    )
						Intrsct = CTIMLSEGMINTERSECTFULLLEFT;
						else
							Intrsct = CTIMLSEGMINTERSECTFULLRIGHT;
				}
			}
		}
	}
	return Intrsct;
}

Bool32 CTIMaskLineSegment::IntersectWith(PCTIMaskLineSegment pSegm)
{
	Bool32 bRet = FALSE;

	if (pSegm)
	{
		switch( IsIntersectWith(pSegm) )
		{
		case CTIMLSEGMINTERSECTLEFT :
			mwEnd  = pSegm->GetEnd();
			bRet =  TRUE;
			break;
		case CTIMLSEGMINTERSECTRIGHT :
			mwStart = pSegm->GetStart();
			bRet =  TRUE;
			break;
		case CTIMLSEGMINTERSECTIN :
			mwEnd   = pSegm->GetEnd();
			mwStart = pSegm->GetStart();
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

Bool32 CTIMaskLineSegment::AddWith(PCTIMaskLineSegment pSegm)
{
	Bool32 bRet = FALSE;

	if (pSegm)
	{
		switch( IsIntersectWith(pSegm) )
		{
		case CTIMLSEGMINTERSECTLEFT :
			mwStart = pSegm->GetStart();
			bRet = TRUE;
			break;
		case CTIMLSEGMINTERSECTRIGHT :
			mwEnd  = pSegm->GetEnd();
			bRet = TRUE;
			break;
		case CTIMLSEGMINTERSECTIN :
			bRet = TRUE;
			break;
		case CTIMLSEGMINTERSECTOVER:
			mwStart = pSegm->GetStart();
			mwEnd  = pSegm->GetEnd();
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

Bool32 CTIMaskLineSegment::CutLeftTo(PCTIMaskLineSegment pSegm)
{
	Bool32 bRet = FALSE;

	if (pSegm)
	{
		switch( IsIntersectWith(pSegm) )
		{
		case CTIMLSEGMINTERSECTRIGHT :
		case CTIMLSEGMINTERSECTIN :
			mwEnd  = pSegm->GetStart();
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

Bool32 CTIMaskLineSegment::CutRightTo(PCTIMaskLineSegment pSegm)
{
	Bool32 bRet = FALSE;

	if (pSegm)
	{
		switch( IsIntersectWith(pSegm) )
		{
		case CTIMLSEGMINTERSECTLEFT :
		case CTIMLSEGMINTERSECTIN :
			mwStart = pSegm->GetEnd();
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

Int32 CTIMaskLineSegment::GetPointDirect(Word32 X)
{
	Int32 iRet = CTIMLSEGMPOINTIN;

	if ( !IsPointInSegment(X) )
	{
		if ( (Int32)X < GetStart() )
			iRet = CTIMLSEGMPOINTLEF;
		else
			iRet = CTIMLSEGMPOINTRIGHT;
	}

	return iRet;
}
/////////////////////////////////////////////////////////////////////////
// end of file
