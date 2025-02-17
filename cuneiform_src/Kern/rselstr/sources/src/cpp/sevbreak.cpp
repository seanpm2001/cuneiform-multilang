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


/****************************************************************************
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              SEBREAK.C - vertical breaking of strings                    *
 *                                                                          *
 ***************************************************************************/

# include <stdlib.h>
# include "extract.h"
#include "minmax.h"

void StringCountRecog (STRING *);   // 940223  AL
BOOL StringBreakOnVertical (STRING *p,  int x,
                            STRING **pq, STRING **pr)
{
    int  i;
    int  iLettersLimit;
    int  iDustLimit;
    ROOT *pRootLastInQ;
    ROOT *pRootFirstInR;

    for (i = 0; i < p -> nLetters; i++)
        if (pRoots [p -> pLettersList [i]].xColumn > x)
            break;

    iLettersLimit = i;

    if (iLettersLimit == 0 || iLettersLimit == p -> nLetters)
    {
        return (FALSE);
    }

    for (i = 0; i < p -> nDust; i++)
        if (pRoots [p -> pDustList [i]].xColumn > x)
            break;

    iDustLimit = i;

    pRootLastInQ  = &pRoots [p -> pLettersList [iLettersLimit - 1]];
    pRootFirstInR = &pRoots [p -> pLettersList [iLettersLimit]];

    String = *p;
    String.nLetters    = iLettersLimit;
    String.nDust       = iDustLimit;
    String.uFlags      |= SF_NEED_UPDATE;
    String.xRight      = pRootLastInQ -> xColumn + pRootLastInQ -> nWidth;

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 4)
        LT_GraphicsCurrentStringOutput ("First part of breaked string");
# endif

    *pq = StringAddToList ();
    (*pq) -> uFlags |= (SF_NEED_UPDATE | SF_VERTCUT);  // 940223 AL

    StringCountRecog (*pq);   // 940223  AL

    String = *p;
    String.pLettersList += iLettersLimit;
    String.nLetters     -= iLettersLimit;
    String.pDustList    += iDustLimit;
    String.nDust        -= iDustLimit;
    String.uFlags       |= SF_NEED_UPDATE;
    String.xLeft        = pRootFirstInR -> xColumn;

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 4)
        LT_GraphicsCurrentStringOutput ("Second part of breaked string");
# endif

    *pr = StringAddToList ();
    (*pr) -> uFlags |= (SF_NEED_UPDATE | SF_VERTCUT);  // 940223 AL
    StringCountRecog (*pr);   // 940223   AL

    p -> uFlags |= SF_NEED_DELETE;
    return (TRUE);
}

static BOOL StringProcessVerticalBreaking (STRING *p);

static BOOL CheckIntervalsAndProcessString
(
    STRING *p,
    int nBigDistance,
    int xLetEnd,  int xLetBegin,
    int xDustEnd, int xDustBegin
)
{
    STRING *q, *r;
    int xCrossEnd, xCrossBegin;

    if (xDustBegin - xDustEnd < nBigDistance)
    {
        return (FALSE);
    }

    if (xLetBegin < xDustEnd || xDustBegin < xLetEnd)
    {
        return (FALSE);
    }

    xCrossEnd   = MAX (xLetEnd,   xDustEnd);
    xCrossBegin = MIN (xLetBegin, xDustBegin);

    if (xCrossBegin - xCrossEnd >= nBigDistance &&
        StringBreakOnVertical (p, (xCrossBegin+xCrossEnd) / 2, &q, &r))
    {
        StringProcessVerticalBreaking (q);
        StringProcessVerticalBreaking (r);
        return (TRUE);
    }

    return (FALSE);
}

static BOOL StringProcessVerticalBreaking (STRING *p)
{
    STRING *q, *r;
    int i, j;
     int nBigDistance = (p -> yMiddleBottom - p -> yMiddleTop + 1) * 6;
    int xLetEnd,   xLetBegin;
    ROOT *pLet1,  *pLet2;
    ROOT *pDust1, *pDust2;


    if (p -> nLetters > 30 &&
        p -> nTopDispersion     > p -> nMiddleHeight / 10 &&
        p -> nBottomDispersion  > p -> nMiddleHeight / 10)
    {

        String = *p;

# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 1)
            LT_GraphicsCurrentStringOutput ("May be shifted string");
# endif
    }

    //   Search for big spaces in string

    if (p -> nLetters == 0)
    {
        return (FALSE);
    }

    pLet2 = &pRoots [p -> pLettersList [0]];

    for (i = 1; i < p -> nLetters; i++)
    {
        pLet1 = pLet2;
        pLet2 = &pRoots [p -> pLettersList [i]];

        xLetEnd   = pLet1 -> xColumn + pLet1 -> nWidth - 1;
        xLetBegin = pLet2 -> xColumn;

        if (xLetBegin - xLetEnd < nBigDistance)
            continue;

        // Case: no dust

        if (p -> nDust == 0)
        {
            if (StringBreakOnVertical (p, (xLetBegin + xLetEnd) / 2, &q, &r))
            {
                StringProcessVerticalBreaking (q);
                StringProcessVerticalBreaking (r);
                return (TRUE);
            }

            continue;
        }

        // Case : no dust at the beginning of string

        pDust2 = &pRoots [p -> pDustList [0]];

        if (CheckIntervalsAndProcessString (p,
                nBigDistance,
                xLetEnd,
                xLetBegin,
                p -> xLeft,
                pDust2 -> xColumn))
        {
            return (TRUE);
        }

        // Case : no dust at the end of string

        pDust1 = &pRoots [p -> pDustList [p -> nDust - 1]];

        if (CheckIntervalsAndProcessString (p,
                nBigDistance,
                xLetEnd,
                xLetBegin,
                pDust1 -> xColumn + pDust1 -> nWidth - 1,
                p -> xRight))
        {
            return (TRUE);
        }

        // Case : regular

        pDust2 = &pRoots [p -> pDustList [0]];

        for (j = 1; j < p -> nDust; j++)
        {
            pDust1 = pDust2;
            pDust2 = &pRoots [p -> pDustList [j]];

            if (CheckIntervalsAndProcessString (p,
                    nBigDistance,
                    xLetEnd,
                    xLetBegin,
                    pDust1 -> xColumn + pDust1 -> nWidth - 1,
                    pDust2 -> xColumn))
            {
                return (TRUE);
            }
        }
    }

    return (FALSE);
}

void StringsBreakOnVertical (void)
{
    STRING *pString;

  AGAIN:
    for (pString = pStringsList; pString != NULL; pString = pString -> pNext)
    {
        if (pString -> uFlags
            & (SF_NEED_UPDATE | SF_NEED_DELETE | SF_DONT_VBREAK))
        {
            continue;
        }

        if (StringProcessVerticalBreaking (pString))
            goto AGAIN;
        else
            pString -> uFlags |= SF_DONT_VBREAK;
    }

    StringsListUpdate ();
}
