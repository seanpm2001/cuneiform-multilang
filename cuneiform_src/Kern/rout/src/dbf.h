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


// Dbf.h

/********************************************************************
	Description of dBase files (dBase IV)

	Version 1.0 (27.02.1992)
*******************************************************************/

#pragma once

// Fixed part of DBF-file header
typedef	struct	{
	unsigned char	tag;
	unsigned char	year;			//	Last modification date (Year - 1900)
	unsigned char	month;			//	Last modification date
	unsigned char	day;			//	Last modification date
    unsigned long	records;		//	Number of records
	unsigned short	header_size;	//	Header size ( with this structure )
	unsigned short	record_size;    //	Record size
    unsigned char	Reserved1 [2];
	unsigned char	transaction;    //	Transaction flag ( dBase IV only )
    unsigned char	Reserved2 [13];
	unsigned char	MDX_flag;		//	MDX flag ( dBase IV only )
    unsigned char	language;
    unsigned char	Reserved3 [2];
}	DBF_HEADER;

// Field descriptor
typedef	struct	{
	char			name[11];		//	Field name
	char			type;           //	Field type
	unsigned short	offset;			//	FoxBase+ only ?
	char			Reserved1 [2];
	unsigned char	length;			//	Field length ( with point & fract )
	unsigned char	decimal;        //	Fract size
	char			Reserved2 [13];
	unsigned char	MDX_tag;        //	MDX tag ( dBase IV only )
}	DBF_FIELD;

#define	DBF_TERM_BYTE	'\x0d'
#define	DBF_TERM_FILE	'\x1a'
#define	DBF_REC_USED	' '
#define	DBF_REC_DELETED	'*'

// DBT files header
typedef struct	{
	unsigned long	next_block;		//	Number of next free block

//	Next members of this structure used in dBase IV only

	char			Reserved1 [4];
	char			name[8];		//	File name
	char			Reserved2 [2];
	unsigned short	x102;			//	Always 0x102
	unsigned short	block_size;		//	Size of block in bytes
	char			Reserved3 [2];

}	DBT_HEADER;

#define	DBF_MEMO_BLKSIZE	512	//	for dBase III & dBase III plus

//	Line longer then 66 chars break out with pair (0x8d, 0x0a)
//	End of 'memo' is two Ctrl-Z chars

#define	DBF_SOFT_CR		'\x8d'	//	'�'
#define	DBF_TERM_MEMO	'\x1a'	//	Ctrl-Z
