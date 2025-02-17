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
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTTREES.C - working with geometrical and logical trees      *
 *                                                                          *
 ***************************************************************************/

# include <stdlib.h>
# include "layout.h"
# include "my_mem.h"
/************************************** Remove function
# define BLOCKS_LINKS_QUANTUM       8
# define BLOCKS_LINKS_QUANTUM_MASK  7
# define BLOCKS_LINKS_QUANTUM_SHIFT 3

static void BlocksAddLink (BLOCK *p, BLOCK *q)
{
    if ((p -> nChildren & BLOCKS_LINKS_QUANTUM_MASK) == 0)
    {
        p -> pChildren =
            realloc (p -> pChildren,
              (size_t) (
               (((p -> nChildren >> BLOCKS_LINKS_QUANTUM_SHIFT) + 1)
                 << BLOCKS_LINKS_QUANTUM_SHIFT) * sizeof (BLOCK *)));
    }

    if (p -> pChildren == NULL)
        ErrorNoEnoughMemory ();

    p -> pChildren [p -> nChildren++] = q;
    q -> nParents++;
}
**************************************************/

static BOOL IsPossibleParent (BLOCK *p, BLOCK *q)
{
    if (p -> Type == BLOCK_TEXT && q -> Type == BLOCK_TEXT &&
        p -> nEmbedding != q -> nEmbedding)
    {
        return (FALSE);
    }

    return (p -> Rect.yBottom <= q -> Rect.yTop &&
            ! (p -> Rect.xLeft  > q -> Rect.xRight ||
               p -> Rect.xRight < q -> Rect.xLeft));
}

/*************************************** Remove function
static int nCheckPathCounter;

static BOOL CheckPath (BLOCK *p, BLOCK *q)
{
    int i;

    nCheckPathCounter++;

    if (nCheckPathCounter > 10000)
        goto EXIT;

    if (p -> Rect.yBottom > q -> Rect.yTop)
        goto EXIT;

    for (i = 0; i < p -> nChildren; i++)
    {
        if (p -> pChildren [i] == q                  ||
            IsPossibleParent (p -> pChildren [i], q) ||
            CheckPath (p -> pChildren [i], q))
        {
            nCheckPathCounter--;
            return (TRUE);
        }
    }

  EXIT:
    nCheckPathCounter--;
    return FALSE;
}
*****************************************************/

/********************************************** Now empty
void TreeBuild (void)
{
    BLOCK *p, *q;

    for (p = pBottomBlocksList; p != NULL; p = p -> pUp)
    {
     if (p -> Type == BLOCK_VERT_SEPARATOR) continue;

        for (q = p -> pDown; q != NULL; q = q -> pDown)
        {
            if (q -> Type == BLOCK_VERT_SEPARATOR) continue;

            nCheckPathCounter = 0;

            if (IsPossibleParent (p, q) && ! CheckPath (p, q))
                BlocksAddLink (p, q);
        }
    }
}
**********************************************/


/**********************************************
void TreeEdit (void)
{
}
**********************************************/

static BLOCK * GetNextInTreeOrder( void )
{
 BLOCK *p, *q;
 for (p = pLeftBlocksList; p != NULL; p = p -> pRight)
 {
   if ( p->nParents == -1 ) continue;
   if (p -> Type == BLOCK_VERT_SEPARATOR) goto found;
   for (q = p->pParent ? p->pParent : p->pUp; q != NULL; q = q -> pUp)
   {
     if (q -> Type == BLOCK_VERT_SEPARATOR) continue;
     if (q -> nParents == -1 ) continue;
     if (IsPossibleParent( q, p ))  { p->pParent = q; goto next_p; }
   }
found:   p->nParents = -1;  return p;
next_p:;
 }
 return NULL;
}

void TreePass (void)
{
    //  BOOL bDone;
    int  nTextBlock;
    int  nSeparator;
    BLOCK *p;
    // int  iChild;
    ROOT *pRoot;

    nTextBlock = 0;
    nSeparator = 0;

    while ( (p = GetNextInTreeOrder()) != NULL )
    {
                switch (p -> Type)
                {
                    case BLOCK_TEXT:
                        ++nTextBlock;

                        for (pRoot = p -> pRoots;
                                 pRoot != NULL;
                                     pRoot = pRoot -> u1.pNext)
                        {
                            if (! pRoot -> bReached)
                            {
                                pRoot -> bReached = TRUE;
                                pRoot -> nBlock   = nTextBlock;
                            }
                        }

                        p -> nNumber = nTextBlock;
                        break;

                    case BLOCK_HORZ_SEPARATOR:
                    case BLOCK_VERT_SEPARATOR:
                    case BLOCK_RECT_SEPARATOR:
                        p -> nNumber = ++nSeparator;
                        break;

                    default:
                        BlocksRemoveDescriptor (p);
                        break;
                }
    }
/************************ Old function
    do
    {
        bDone = TRUE;

	for (p = pLeftBlocksList; p != NULL; p = p -> pRight)
        {
            if (p -> nParents == 0)
            {
                p -> nParents = -1;

                for (iChild = 0; iChild < p -> nChildren; iChild++)
                    p -> pChildren [iChild] -> nParents--;

                switch (p -> Type)
                {
                    case BLOCK_TEXT:
                        ++nTextBlock;

                        for (pRoot = p -> pRoots;
                                 pRoot != NULL;
                                     pRoot = pRoot -> u1.pNext)
                        {
                            if (! pRoot -> bReached)
                            {
                                pRoot -> bReached = TRUE;
                                pRoot -> nBlock   = nTextBlock;
                            }
                        }

                        p -> nNumber = nTextBlock;
                        break;

                    case BLOCK_HORZ_SEPARATOR:
                    case BLOCK_VERT_SEPARATOR:
                    case BLOCK_RECT_SEPARATOR:
                        p -> nNumber = ++nSeparator;
                        break;

                    default:
                        BlocksRemoveDescriptor (p);
                        break;
                }

                bDone = FALSE;
                break;
            }
        }
    }
    while (!bDone);
 **********************************************/
}
