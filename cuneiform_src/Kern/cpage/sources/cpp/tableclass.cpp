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

// TableClass.cpp: implementation of the TableClass class.
//
//////////////////////////////////////////////////////////////////////
#include "mymem.h"
#include "tableclass.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TableCell::TableCell()
{
	m_nNumber = 0;

	m_PhCoord.x=0;
	m_PhCoord.y=0;

	m_nBlock = 0;
	m_nGeCount = 0;
}
TableCell::~TableCell()
{
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TableLine::TableLine()
{
	m_nCoord = 0;
	//m_nItems = 0;
}
//////////////////////////////////////////////////////////////////////
TableLine::~TableLine()
{
}
//////////////////////////////////////////////////////////////////////
Bool32 TableLine::Create(Int32 nCoord,Word32 nItems)
{
	Bool32 rc = FALSE;

	m_nCoord = nCoord;
	//m_nItems = nItems;
	rc = m_lpProperty.Create(nItems);

	return rc;
}
//////////////////////////////////////////////////////////////////////
void	TableLine::Delete()
{
	m_nCoord = 0;
	//m_nItems = 0;
	m_lpProperty.Delete();
}
//////////////////////////////////////////////////////////////////////
Bool32	TableLine::Attach(Handle hPage)
{
	return m_lpProperty.Attach(hPage);
}
//////////////////////////////////////////////////////////////////////
Bool32	TableLine::Store(Handle hPage)
{
	return m_lpProperty.Store(hPage);
}
//////////////////////////////////////////////////////////////////////
void    TableLine::Remove(Handle hPage)
{
	m_lpProperty.Remove(hPage);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TableClass::TableClass()
{
	//m_nVer = 0;
	//m_nHor = 0;
	m_nSkew2048 = 0;
	m_hBlock = NULL;
	m_hPage = NULL;
	m_nPhNumber = 0;

}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TableClass::~TableClass()
{
	Delete();
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Bool32 TableClass::Create(Int32 Skew2048,Word32 nVer,Int32 * lpVCor, Word32 nHor,Int32 * lpHCor)
{
	Bool32 rc = FALSE;
	m_nSkew2048 = Skew2048;

	if(nVer && nHor)
	{
		Word32 i = 0;
		rc = m_lpVerLines.Create(nVer);
		for(i=0;i<nVer && lpVCor;i++)
			rc &= m_lpVerLines[i].Create(lpVCor[i],nHor);

		rc &= m_lpHorLines.Create(nHor);
		for(i=0;i<nHor && lpHCor;i++)
			rc &= m_lpHorLines[i].Create(lpHCor[i],nVer);

		rc &= m_lpCell.Create(GetNumberRow()*GetNumberColumn());
	}
	m_hBlock = NULL;
	m_hPage = NULL;
return rc;
}
//////////////////////////////////////////////////////////////////////
void	TableClass::Delete()
{
	if(m_lpVerLines.GetSize() && m_lpHorLines.GetSize())
	{
		Word32 i = 0;
		for(i=0;i<m_lpVerLines.GetSize() ;i++)
			m_lpVerLines[i].Delete();
		m_lpVerLines.Delete();

		for(i=0;i<m_lpHorLines.GetSize() ;i++)
			m_lpHorLines[i].Delete();
		m_lpHorLines.Delete();

		m_lpCell.Delete();
	}
	//m_nVer = m_nHor = 0;
	m_nSkew2048 = 0;
	m_hBlock = NULL;
	m_hPage = NULL;
}
//////////////////////////////////////////////////////////////////////
TableClass * TableClass::Attach(Handle hPage,Handle hBlock)
{
	TableClass * rc = NULL;
	Handle Type = CPAGE_GetInternalType("TableClass");
	if( CPAGE_GetBlockDataPtr(hPage,hBlock,Type,(void**)&rc) &&
		rc->m_lpVerLines.GetSize() &&
		rc->m_lpHorLines.GetSize())
	{
		Word32 i = 0;
		rc->m_lpVerLines.Attach(hPage);
		for(i=0;i<rc->m_lpVerLines.GetSize() ;i++)
			rc->m_lpVerLines[i].Attach(hPage);

		rc->m_lpHorLines.Attach(hPage);
		for(i=0;i<rc->m_lpHorLines.GetSize() ;i++)
			rc->m_lpHorLines[i].Attach(hPage);
		rc->m_lpCell.Attach(hPage);
		rc->m_hBlock = hBlock;
		rc->m_hPage = hPage;
	}
	return rc;
}
//////////////////////////////////////////////////////////////////////
Handle	TableClass::Store(Handle hPage)
{
	Bool32 res = FALSE;
	Handle Type = CPAGE_GetInternalType("TableClass");

	m_hBlock = CPAGE_CreateBlock(hPage,Type,0,0,this,sizeof(*this));
	if( m_hBlock &&
		m_lpVerLines.GetSize() &&
		m_lpHorLines.GetSize())
	{
		Word32 i = 0;
		res = m_lpVerLines.Store(hPage);
		for(i=0;i<m_lpVerLines.GetSize() ;i++)
			res &= m_lpVerLines[i].Store(hPage);

		res &= m_lpHorLines.Store(hPage);
		for(i=0;i<m_lpHorLines.GetSize() ;i++)
			res &= m_lpHorLines[i].Store(hPage);

		res &= m_lpCell.Store(hPage);
		m_hPage = hPage;
	}
	if(res==FALSE)
		Remove();

	return  m_hBlock;
}
//////////////////////////////////////////////////////////////////////
void    TableClass::Remove()
{
	if(m_lpVerLines.GetSize() && m_lpHorLines.GetSize())
	{
		Word32 i = 0;
		for(i=0;i<m_lpVerLines.GetSize() ;i++)
			m_lpVerLines[i].Remove(m_hPage);
		m_lpVerLines.Remove(m_hPage);

		for(i=0;i<m_lpHorLines.GetSize() ;i++)
			m_lpHorLines[i].Remove(m_hPage);
		m_lpHorLines.Remove(m_hPage);

		m_lpCell.Remove(m_hPage);
	}
	if(m_hBlock)
		CPAGE_DeleteBlock(m_hPage,m_hBlock);
 /*
	m_nVer = m_nHor = 0;
	m_nSkew2048 = 0;
	m_hBlock = NULL;
	m_hPage = NULL;
*/
}
//////////////////////////////////////////////////////////////////////
TableClass * TableClass::GetNext()
{
	Handle Type = CPAGE_GetInternalType("TableClass");
	TableClass * rc = NULL;

	Handle hBlock = CPAGE_GetBlockNext(m_hPage,m_hBlock, Type);
	if(hBlock)
		rc = Attach(m_hPage,hBlock);

return rc;
}
//////////////////////////////////////////////////////////////////////
void  TableClass::Update()
{
	int i,j;
	// ���������� ���������� ������
	int nRow = GetNumberRow();
	int nCol = GetNumberColumn();
	Point32  p = {-1,-1};
	Int32    n = -1;

	m_nPhNumber = 0;

	for(i=0;i<nRow;i++)
		for(j=0;j<nCol;j++)
		{
				if(i && !m_lpHorLines[i].IsRealy(j))
				{// ��� ��������������� ����������� ������
					p = m_lpCell[(i-1)*nCol+j];
					n = m_lpCell[(i-1)*nCol+j];
				}
				else if(!j || m_lpVerLines[j].IsRealy(i))
				{// ���� ������������ ����������� �����
					n = ++m_nPhNumber;
					p.x = j;
					p.y = i;
				}
				else
				{// ���� ���. ����������� ������ � ��� ����. ������
					p = m_lpCell[i*nCol + j - 1];
					n = m_lpCell[i*nCol + j - 1];
				}

				m_lpCell[i*nCol + j] = n;
				m_lpCell[i*nCol + j] = p;
				m_lpCell[i*nCol + j].Fragment() = -1;
				m_lpCell[i*nCol + j].GeCount()++;
		}
}
