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


//#include "c_types.h"

#include <string.h>
#include "struct.h"
#include "v1comp.h"

//#include "func.h"
//#include "evsnap.h"
//#include "extrcomp.h"

//	Function description
//void snap_keep (Word8 code, Word8* arg1, Word16 arg2);

//		Common variables
// Oleg
extern  Word8   save_event_txt[],save_event_txts[];
extern Bool32  enable_save_stat;
// Oleg
extern c_comp wcomp;
extern version * start_rec, *rec_ptr;
extern Word8 recors_change;
extern Word8 lpool[];
extern Word8* events_tree;
extern void ev_lang_filter();
//	Internal function description
static Word16 net_comp_count();
static void ev_vector_cnt();
static Word16 seek_events(Word8* ev);
static Int16 first_var();
static void double_events(Word16 row, Word16 col);
static void varset (Int8 add);
static void upper_row(Int16 row);
static void upper_col(Int16 col);
static void lower_row(Int16 row);
static void lower_col(Int16 col);


//	Internal working fields
struct ev_vari	{
  Word8 ln;  // line number
  Int8 add; // additional byte:
		};
typedef struct ev_vari VAR;

Word8 evline[3+12], evline1[3+12];
VAR evvars[64], * evendvar;
Word16 evrow_b1, evrow_b2, evrow_b3, evrow_b4;
Word16 evcol_b1, evcol_b2, evcol_b3, evcol_b4, evcol_b5;
Word8* seek_responce;
extern Int16 evfulln;
static Word8 ev;
static VAR * vp;

//
//
//		Straight events counting and tables search
//
//

Word16 events_recog()
{
 Int16 nv;
 Word16 i,k;
 Word8* p;
 version *v;
 //snap_keep (snap_stright,NULL,0);
 if ((wcomp.nl > NET_COMPONENT) && (net_comp_count() > NET_COMPONENT))
  {/*snap_keep (snap_vers,NULL,0);*/ return 0;}
 ev_vector_cnt();
 if( evline[0]==0 || evline[0]> NET_COMPONENT )
    {
    return 0;
    }
 nv = seek_events(evline);
 if(0)
 { // 12-09-2000 ����������� ��������������� ������� �����
	 int i;
	 for(i=0;i<nv;i++)
	 {
		 if( seek_responce[i]==0 )
		 {
			memcpy(seek_responce+i,seek_responce+i+1,nv-i-1);
			nv--;
			i--;
		 }
	 }
	}
 if (nv == 0)
  {
   nv = first_var();
   if (nv < 0) { /*snap_keep (snap_vers,NULL,0);*/ return 0; }
  }
 p = seek_responce; i = nv; v = start_rec;
 k=0;
 while (i--)  { v->let = *p++;
 if( enable_save_stat )
    save_event_txts[k++] = v->let;
 (v++)->prob = 0xfe;
 }
 save_event_txts[k] = 0;
 rec_ptr = v; //snap_keep (snap_vers,(Word8*)start_rec,nv);
 ev_lang_filter();
 return nv;
}

//-------------	Counting the straight events vector ----------------------
//	Structure of events vector:
//	First 3 bytes: Numbers of lines, of free starts and free ends
//	One byte: topological code - will counted in seek_event
//	One byte per line: line start and ebd codes

#define next_var (a) { (a+1)->ln = a->ln; a++;}

static void ev_vector_cnt()
{
 Word8* ep;
 lnhead * lp;
 interval * ip;

 evrow_b2 = (evrow_b4 = wcomp.h)/2;
 evrow_b1 = (evrow_b2+1)/2;
 evrow_b3 = evrow_b2 + (wcomp.h - evrow_b2)/2;
 evcol_b1 = ((evcol_b3 = wcomp.w) + 2)/3;
 evcol_b2 = evcol_b3 - evcol_b1;

 evfulln = wcomp.nl;
 evline[0] = (Word8)wcomp.nl;
 evline[1] = wcomp.begs; evline[2] = wcomp.ends;

 ep = evline+4;
 vp = evvars;
 vp->ln = 0;
 lp = (lnhead *)lpool;

 while (lp->lth)
  {
   ip = (interval *) (lp + 1);
   if (lp->h == 1)
    {
	// One point lines
     if (lp->flg & (l_fbeg + l_fend))
      {
       evline[0]--; evfulln--;
       if (lp->flg & l_fend) evline[2]--;
       if (lp->flg & l_fbeg) evline[1]--;
       lp = (lnhead *)((Word8*)lp + lp->lth); continue;
      }
     else
      {
by_beg:
       double_events((Word16)lp->row, (Word16)(ip->e - (ip->l + 1)/2));
       *ep++ = ev; lp = (lnhead *)((Word8*)lp + lp->lth); continue;
      }
    }
	// Two points line
   if (lp->h == 2)
    {
     if (lp->flg & l_fbeg) goto by_beg;
     else
      {
       double_events((Word16)(lp->row + 2),(Word16)((ip+1)->e - ((ip+1)->l + 1)/2));
       *ep++ = ev; lp =(lnhead *)((Word8*)lp + lp->lth); continue;
      }
    }
	// Long line
   vp->ln++;
   upper_row (lp->row);
   if (lp->flg & l_fbeg)
      upper_col ((Word16)(ip->e - (ip->l + 1)/2));
   else
      upper_col ((Word16)((ip+1)->e - ((ip+1)->l + 1)/2));
   lower_row ((Word16)(lp->row + lp->h));
   ip += lp->h;
   if (lp->flg & l_fend)
    lower_col ((Word16)((ip-1)->e - ((ip-1)->l + 1)/2));
   else
    lower_col ((Word16)((ip-2)->e - ((ip-2)->l + 1)/2));
   *ep++ = ev; lp = (lnhead *)(ip+1);
  }
 evendvar = vp;
}

static void double_events(Word16 row, Word16 col)
{
 VAR * wvp;
 vp->ln++;	wvp = vp;
 upper_row(row);	upper_col(col);
 ev += ev >> 4;
 while (wvp != vp) { wvp->add += wvp->add >> 4; wvp++;}
}

static void varset (Int8 add)
{
 vp->add = add; (vp+1)->ln = vp->ln; vp++;
}

static void upper_row(Int16 row)
{
 if (row >= evrow_b2)
  {
   if (row == evrow_b2) {ev = 2*64; varset(-64); return;}
   if (row >= evrow_b3) {ev = 3*64; if (row == evrow_b3) varset(-64); return;}
   ev = 2*64; if (row+1 == evrow_b3) varset(64);
   return;
  }
 else
  {
   if (row >= evrow_b1)
    {
     ev = 1*64;
     if (row == evrow_b1) {varset(-64); return;}
     if (row+1 == evrow_b2) varset(64);
     return;
    }
   ev = 0; if (row+1 == evrow_b1) varset(64);
   return;
  }
}

static void upper_col(Int16 col)
{
 if (col > evcol_b1)
  {
   if (col < evcol_b2) { ev+=16; if (col+1 == evcol_b2) varset(16); return; }
   ev+=2*16; if (col == evcol_b2)  varset(-16); return;
  }
 if (col == evcol_b1) { ev+=16; varset(-16); return; }
 if (col+1 == evcol_b1) varset(16); return;
}

static void lower_row(Int16 row)
{
 if (row >= evrow_b2)
  {
   if (row == evrow_b2) {ev+=2*4; varset(-4); return;}
   if (row >= evrow_b3) {ev+=3*4; if (row == evrow_b3) varset(-4); return;}
   ev+=2*4; if (row+1 == evrow_b3) varset(4);
   return;
  }
 else
  {
   if (row >= evrow_b1)
    {
     ev+=1*4;
     if (row == evrow_b1) {varset(-4); return;}
     if (row+1 == evrow_b2) varset(4);
     return;
    }
   if (row+1 == evrow_b1) varset(4); return;
  }
}

static void lower_col(Int16 col)
{
 if (col > evcol_b1)
  {
   if (col < evcol_b2) { ev++; if (col+1 == evcol_b2) varset(1); return; }
   ev+=2; if (col == evcol_b2)  varset(-1); return;
  }
 if (col == evcol_b1) { ev++; varset(-1); return; }
 if (col+1 == evcol_b1) varset(1); return;
}

static Word16 net_comp_count()
{
 lnhead * p = (lnhead *)lpool;
 Word16 s = 0;
 while (p->lth)
  {
   if ((p->h != 1) ||
       ((p->flg & (l_fbeg+l_fend)) == 0)) s++;
   p =(lnhead *)((Word8*)p + p->lth);
  }
 return s;
}

//------------------- Seek in events table -----------------------------
//	Structure of the seek table:
//	10 headers for each number of lines by 256 entries per header.
//	Each entry contains displacement in overall table of start of
//	chain for vectors with same number of lines and hash.
//	Structure of chain element:
//	Byte 1 - number of versions (0 - end of chain)
//	Part of vector includes topological code and lines codes
//	Versions



Word8 taba[] = {0,1,3,6,10,15,21,28,36,45,55};
Word8 tabb[] = {0,255,0,3,9,19,34,55,83,119,164,219};

static Word16 rot(Word16 n) { return ((n<<3) + (n>>13)); }

static Word16 seek_events (Word8* ep)
{
 Word16 hash,i,nl;
 Word32 di;
 Word8* p;

 //if (db_status) snap_keep(snap_vector,ep,*ep+4);
 if(enable_save_stat)
 {
int i,n;
n=save_event_txt[0]=ep[0];
save_event_txt[1]=ep[1];
save_event_txt[2]=ep[2];
for(i=0;i<n;i++)
    save_event_txt[3+i]=ep[4+i];
 }
 nl = *ep;
 if( !nl )	return 0; // OLEG
 hash = *(ep+3) = (tabb[nl] + (nl-*(ep+1)< sizeof(taba)/sizeof(taba[0]) ? taba[nl-*(ep+1)]:0) + *(ep+2)) & 0xff;// Piter add control taba[]
 i = nl;
 p=ep+4;
 while (i--)
 { hash = rot(hash) ^ (Word16)(*(p++));
 }
 hash = (hash & 0xff) ^ (hash >> 8);
 p = ((nl-1) * 256 + hash) * sizeof(Word32) + events_tree;
 di = *((Word32 *)p); if (di==0) return 0; p = events_tree+di;
 /* Oleg : 17-08-1994 : changed WORD */
 nl++;
 while (*p)
  {
   if (memcmp (ep+3,p+1,nl) == 0)
    {
    seek_responce = p+nl+1;
    return *p;
    }
   p += *p+1+nl;
  }
 return 0;
}

//-------------------- Variations process --------------------------

static Int16 first_var()
{
 VAR *p=evvars;
 Word8 *ev1, *ev2;
 Word16 nl,n;
 if (p == evendvar) return -1;
 memcpy (evline1,evline,4); ev1 = evline+4; ev2 = evline1+4; nl =1;
var_loop:
 n = p->ln - nl;
 if (n) {nl = p->ln; memcpy (ev2,ev1,n); ev1+=n; ev2+=n;}
 *(ev2++) = *(ev1++) + p->add;
 if( evline[0]<nl )	 return -1; // OLEG
 n=evline[0]-nl; if (n) memcpy (ev2,ev1,n);
 //snap_keep (snap_variation,NULL,0);
 n = seek_events(evline1); if (n > 0) return n;
 if (++p == evendvar) return 0;
 ev1 = evline+3+nl; ev2=evline1+3+nl; goto var_loop;
}
