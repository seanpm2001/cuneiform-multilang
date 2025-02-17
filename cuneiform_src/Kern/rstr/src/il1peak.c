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


/********************************************************************/
/*								    */
/*			       Peaks program                        */
/*								    */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "cuthdr.h"
#include "func.h"
#include "il1peak.h"
#include "minmax.h"

struct local_extremum
 {
 INT x;        // x_coordinate of local extremum
 INT beg;      // begin of local extremum area
 INT end;      // end of local exremum area
 INT jump;     // difference between peak and valley
 };

extern INT n_peak[NFUNC];
extern PCHAR peak_vect[NFUNC];

void extremum (PCHAR func, INT n_func, INT from, INT to, INT threshold,
	       CHAR f2, CHAR fbound)
 {
 INT x;
 INT min,max;
 INT n_min,n_max;
 INT locmin2[MAX_HOR_IL1],locmin1[MAX_HOR_IL1];
 INT locmax1[MAX_HOR_IL1];
 struct local_extremum locmax2[MAX_HOR_IL1];
 INT i;
 INT minway,maxway;
 INT gmin,gmax;
 LONG f,t;
 INT r,l;
 INT j,k;
 INT width;
 INT bnd;

 Z=&string;
 width=to+1-from;
 min=gmin=10000;
 max=gmax=0;
 n_max=n_min=0;
 minway=WAY_UP;
 maxway=WAY_DOWN;
 for (x=from; x <= to; x++)
  {
  if ((func[x] < min) ||
      ((func[x] == min) && (minway == WAY_DOWN)))
   {
   minway=WAY_DOWN;
   locmin1[n_min]=x;
   min=func[x];
   if (gmin > min)
    gmin=min;
   }
  else
   {
   if ((func[x] > min) && (minway == WAY_DOWN))
    {
    n_min++;
    minway=WAY_UP;
    }
   min=func[x];
   }
  if ((func[x] > max) ||
      ((func[x] == max) && (maxway == WAY_UP)))
   {
   maxway=WAY_UP;
   locmax1[n_max]=x;
   max=func[x];
   if (gmax < max)
    gmax=max;
   }
  else
   {
   if ((func[x] < max) && (maxway == WAY_UP))
    {
    n_max++;
    maxway=WAY_DOWN;
    }
   max=func[x];
   }
  }
 if (minway == WAY_DOWN)
  n_min++;
 if (maxway == WAY_UP)
  n_max++;
				       // filtration of maxima
 k=0;
 if (n_min == 0)
  {
  if (n_max == 0)
   {
   n_peak[n_func]=0;	               // plateau
   return;
   }
  if (n_max > 1)
   {
   n_peak[n_func]=0;	               // abend
   return;
   }
  locmin2[0]=-1;
  locmin2[1]=-1;
  goto nomin;
  }
 if (locmin1[0] > locmax1[0])
  {
  locmin2[0]=-1;
  k=1;
  }
 memcpy(&locmin2[k],locmin1,n_min*sizeof(locmin1[0]));
 if (locmin1[n_min-1] < locmax1[n_max-1])
  locmin2[n_min+k]=-1;
nomin:
 n_peak[n_func]=0;
 if (f2)
  bnd=2;
 else
  bnd=1;
 for (i=0; i < n_max; i++)
  {
  f=func[locmax1[i]]-threshold;
  t=width;
  t=(gmax-threshold)*MIN_JUMP;
  l=r=(INT)f;
  if (locmin2[i] != -1)
   l-=MAX(0,func[locmin2[i]]-threshold);
  else
   {
   if (!fbound)
    l-=MAX(0,func[from]-threshold);
   }
  if (locmin2[i+1] != -1)
   r-=MAX(0,func[locmin2[i+1]]-threshold);
  else
   {
   if (!fbound)
    r-=MAX(0,func[to]-threshold);
   }
  t=MAX(t,width*MIN_WIDTH_JUMP);
  if ((l <= bnd) && (r <= bnd) || (gmax-threshold <= bnd))
   continue;                           // 2 pixels height - uncertain
  if ((l >= t) || (r >= t))
   {
   locmax2[n_peak[n_func]].x=locmax1[i];
   locmax2[n_peak[n_func]].jump=(INT)f;
   n_peak[n_func]++;
   }
  }
 for (i=0; i < n_peak[n_func]; i++)
  {
  f=func[locmax2[i].x]-threshold;
  t=f*(1-MAX_DIFF);
  for (j=locmax2[i].x; j >= 0; j--)
   {
   if (func[j]-threshold > f)
    break;
   if (func[j]-threshold >= t)
    locmax2[i].beg=j;
   else
    break;
   }
  for (j=locmax2[i].x; j <= to; j++)
   {
   if ((func[j]-threshold) > f)
    break;
   if ((func[j]-threshold) >= t)
    locmax2[i].end=j;
   else
    break;
   }
  }
				       //  defining of peak vector
 for (i=0; i < n_peak[n_func]; i++)
  {
  peak_vect[n_func][i]=0;
  t=width;
  if ((locmax2[i].jump >= (gmax-threshold)*SHORT_PEAK_LEVEL) &&
      (gmax-threshold >= 3))
   peak_vect[n_func][i] |= LONG_PEAK;
  if (locmax2[i].jump < (gmax-threshold)*LONG_PEAK_LEVEL)
   peak_vect[n_func][i] |= SHORT_PEAK;
  if ((locmax2[i].end-locmax2[i].beg+1) < t*NONSHARP_PEAK_LEVEL)
   peak_vect[n_func][i] |= SHARP_PEAK;
  if ((locmax2[i].end-locmax2[i].beg+1) >= t*SHARP_PEAK_LEVEL)
   peak_vect[n_func][i] |= NONSHARP_PEAK;
  if (locmax2[i].end-from < t*LEFT_PEAK_LEVEL)
   {
   peak_vect[n_func][i] |= LEFT_PEAK;
   continue;
   }
  if (locmax2[i].beg-from > t*RIGHT_PEAK_LEVEL)
   {
   peak_vect[n_func][i] |= RIGHT_PEAK;
   continue;
   }
  if (locmax2[i].end-from < t*VERY_RIGHT_PEAK_LEVEL)
   {
   peak_vect[n_func][i] |= (LEFT_PEAK | MIDDLE_PEAK);
   continue;
   }
  if (locmax2[i].beg-from > t*VERY_LEFT_PEAK_LEVEL)
   {
   peak_vect[n_func][i] |= (RIGHT_PEAK | MIDDLE_PEAK);
   continue;
   }
  peak_vect[n_func][i] |= MIDDLE_PEAK;
  }
 return;
 }

