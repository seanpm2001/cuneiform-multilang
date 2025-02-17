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

#ifndef __DIFFRB_H__
#define __DIFFRB_H__
INT EndBlackInterval(BYTE *RASTER, INT NWIDTH);
INT LeftDistance(BYTE *RASTER, INT NWIDTH);
INT RightDistance(BYTE *RASTER, INT NWIDTH);
INT SumIntervalBits( BYTE *RASTER, INT n1, INT n2);
INT SumBits( BYTE *RASTER, INT NWIDTH);
INT VertSum( BYTE *RASTER, INT Wx, INT NHEIGHT, INT Column);
INT MinMaxLeft( BYTE *RAST, INT Wx, BYTE NW, BYTE NH,INT *min, INT *max);
INT MinMaxRIght( BYTE *RAST, INT Wx, BYTE NW, BYTE NH,INT *min, INT *max);
INT NumHorizInterval( BYTE *RASTER, INT NWIDTH);
INT NumVertInterval( BYTE *RASTER, INT Wx, INT NHEIGHT, INT Column);
INT FOOT_A( BYTE *RASTER, INT Wx, BYTE NWIDTH, BYTE NLENGTH);
INT FOOT3_2( BYTE *RASTER, INT Wx, BYTE NWIDTH, BYTE NLENGTH);
INT FOOT3( BYTE *RASTER, INT Wx, BYTE START, BYTE NWIDTH, BYTE NLENGTH, INT SHIFT);
INT Asymm_let( BYTE *RASTER, INT Wx, BYTE NWIDTH, BYTE NLENGTH, INT TW);
INT FOOT( BYTE *RAST, INT Wx, BYTE NW, BYTE NH,  BYTE HARD_FILTER);
INT CenterVertInterval( BYTE *R, INT Wx, INT NH, INT Col,INT *up, INT *down);
INT LinesWithNumIntervals(BYTE *rastr,INT Wx, INT NLENGHT, INT num);
INT LeftEdgeOfRightmostInt( BYTE *rst, INT Wdth);
#endif
