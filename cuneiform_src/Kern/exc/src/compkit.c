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

#include <string.h>
#include "globus.h"
#include "struct.h"

Bool16 boxes_account();

extern c_comp wcomp;
extern MN * main_number_ptr;
extern BOX * boxchain, *dl_last_in_chain;
extern Word8 lpool[];
extern Word16 lpool_lth;
extern Word8 work_raster[];
extern version * start_rec, * rec_ptr;
extern Word8 records_change;
extern Word16 comp_max_h, comp_max_w, comp_min_h, comp_min_w; // defined in extrcomp.c

static void boxes_to_line();

Bool16 component_account()
{
 if (boxes_account())
 	{
 	boxes_to_line();
 	wcomp.type = 0;
 	return TRUE;
 	}
 return FALSE;
}

Bool16 boxes_account()
{
 MN *mn;
 BOX *bp;
 Int16 left, right, n;
 memset (&wcomp,0,sizeof(wcomp));
 mn = main_number_ptr; bp = mn->mnfirstbox;
 left = bp->boxleft; right = bp->boxright; boxchain = bp;
 while ((bp = (dl_last_in_chain = bp)->boxnext) != NULL)
  {
   if (bp->boxleft < left) left = bp->boxleft;
   if (bp->boxright > right) right = bp->boxright;
  }

 wcomp.upper = mn->mnupper; wcomp.left = left;
 wcomp.h = mn->mnlower - mn->mnupper;
 wcomp.rw = ((wcomp.w = right - left) + 7)/8;
 wcomp.nl = mn->mnlines; wcomp.begs = mn->mnbegs; wcomp.ends = mn->mnends;

 if (mn->mnflag & mnpicture)
    return FALSE;

 if ((wcomp.w >= comp_max_w) || (wcomp.h >= comp_max_h))
                        return FALSE; // check size
 if ((wcomp.w <= comp_min_w) || (wcomp.h <= comp_min_h))
                        return FALSE; // check size

 if (mn->mnboxcnt > great_box_count)
    return FALSE;
 if (wcomp.nl < usual_box_count)
    return TRUE;

 for (bp = boxchain, n=0; bp; bp = bp->boxnext)
  {
   if ((bp->boxflag & BOXBEG) == 0) continue;
   if (bp->boxptr > sizeof(BOX) + sizeof(LNSTRT) + 2) {n++; continue;}
   if ((bp->boxflag & (BOXFREEBEG + BOXFREEEND - BOXBEG - BOXEND)) == 0) n++;
  }
 if (n >= usual_box_count)
    return FALSE;
 else
    return TRUE;
}


static void boxes_to_line()
{
 BOX *bp;
 lnhead *lnp;
 LNSTRT *lsp;
 interval * ip;
 BOXINT * bip;
 Int16 x, n;

 bp = boxchain; lnp = (lnhead *)lpool; goto enter_loop;
next_line:
 bp = bp->boxnext;
enter_loop:
 lsp = (LNSTRT *)(bp+1); lnp->row = lsp->y - wcomp.upper;
 lnp->flg = bp->boxflag; ip = (interval *) (lnp+1);
 ip->l = (Word8)lsp->l; x = lsp->x - wcomp.left; (ip++)->e = (Word8)x;
 bip = (BOXINT *)(lsp+1);
 n = (bp->boxptr - sizeof(BOX) - sizeof(LNSTRT))/sizeof(BOXINT);
cont_box:
 while (n--) { ip->l = (Word8)bip->l; x += (bip++)->d, (ip++)->e = (Word8)x; }
 if ((bp->boxflag & BOXEND) == 0)
  {
   bp = bp->boxnext; bip = (BOXINT *)(bp+1);
   n = (bp->boxptr - sizeof(BOX))/ sizeof(BOXINT); goto cont_box;
  }
  ip->e = 0; // Vald for compability with asm prototype
 (ip++)->l = 0;
 lnp->lth = (Word8*)ip - (Word8*)lnp;
 lnp->h = (lnp->lth - sizeof(*lnp) - sizeof(*ip))/sizeof(*ip);
 lnp->flg |= bp->boxflag; lnp = (lnhead *)ip;
 if (bp != dl_last_in_chain) goto next_line;
 lnp->lth = 0;
 lpool_lth = (Word8*)lnp - lpool + sizeof (lnp->lth);
}


static Word8 make_fill[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};
void WriteInterval ( Word8 *p, int end , int len)
{
Int16     sh;
Word16    w;
p = p + (end>>3);
sh = end & 7;
while (len > 8)   { w = 0xff00 >> sh;*p |= w & 0xff; *(--p) |= w>>8; len-=8; }
w = make_fill[len]; w = w << (8-sh); *p |= w & 0xff; *(p-1) |= w >> 8;
return;
}

Word8* make_raster()
{
 lnhead * lp;
 interval *ip;
 Word8 *p, *pp;
 Int16 x, l, sh;
 Word16 w;

 memset (work_raster,0,wcomp.rw*wcomp.h);
 lp = (lnhead *)lpool;
 while (lp->lth)
  {
   pp = work_raster + lp->row * wcomp.rw; ip = (interval *) (lp+1);
   while (x = ip->e, (l = (ip++)->l) != 0)
    {
     p = pp + (x>>3); sh = x & 7;
     while (l > 8) { w = 0xff00 >> sh; *p |= w & 0xff; *(--p) |= w>>8; l-=8; }
     w = make_fill[l]; w = w << (8-sh); *p |= w & 0xff; *(p-1) |= w >> 8;
     pp += wcomp.rw;
    }
   lp = (lnhead *)ip;
  }
 return work_raster;
}


Word8* make_extended_raster(c_comp *cp)
{
 lnhead * lp;
 large_interval *ip;
 Word8 *p, *pp;
 Int16 x, l, sh;
 Word16 wd,w;

 memset (work_raster,0,cp->rw * cp->h); wd = cp->rw;
 lp = (lnhead *)((Word8*)cp + cp->lines + sizeof(Word32));
 while (lp->lth)
  {
   pp = work_raster + lp->row * wd; ip = (large_interval *) (lp+1);
   while (x = ip->e, (l = (ip++)->l) != 0)
    {
     p = pp + (x>>3); sh = x & 7;
     while (l > 8) { w = 0xff00 >> sh; *p |= w & 0xff; *(--p) |= w>>8; l-=8; }
     w = make_fill[l]; w = w << (8-sh); *p |= w & 0xff; *(p-1) |= w >> 8;
     pp += wd;
    }
   lp = (lnhead *)((Word8*)ip - sizeof(Word16));
  }
 return work_raster;
}

lnhead *c_boxln(MN *mn)
{
 main_number_ptr = mn;
 boxes_account();
 boxes_to_line();
 return (lnhead *)lpool;
}

Int16 MN_to_line(MN * mn)
{
 if( !mn )
    return 0;
 main_number_ptr = mn;
 boxes_account();
 boxes_to_line();
 wcomp.type = 0;
 return 0;
}

void save_component(c_comp *,version *,version *,Word8*,Word16);
void save_wcomp()
{
 save_component(&wcomp,start_rec,rec_ptr,lpool,lpool_lth);
}

Word16 length_table[] = {0x100, 0x300, 0x700,0xf00,0x1f00,0x3f00,0x7f00,0xff00};

void save_dust_comp (dust_comp * dp, c_comp *cp, Word8 *lp)
{
 interval * ip;
 Word8 *p;
 dp->size = 1; dp->upper = cp->upper; dp->left = cp->left;
 dp->h = (Word8)cp->h; dp->w = (Word8)cp->w;
 memset ((p=dp->raster),0,sizeof(dp->raster));
 ip =(interval *)(lp + sizeof(lnhead));
 while (ip->l) { *p++ = (length_table [ip->l - 1]) >> (ip->e); ip++; }
}

Word8 byte_seg_size[256] = {
// 0   1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
0x00,0x18,0x17,0x28,0x16,0x00,0x27,0x38,0x15,0x00,0x00,0x00,0x26,0x00,0x37,0x48,        // 0
0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x36,0x00,0x47,0x58,        // 1
0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 2
0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x46,0x00,0x57,0x68,        // 3
0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 5
0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 6
0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x45,0x00,0x00,0x00,0x56,0x00,0x67,0x78,        // 7
0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 8
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // 9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // a
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // b
0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // c
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // d
0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,        // e
0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x00,0x66,0x00,0x77,0x88         // f
};

Int16 read_dust_comp (dust_comp *cp, Word8 *p)
{
 lnhead * lp;
 interval * ip;
 Int16 n, lth;
 Word8 *pp;
 Word8 b;
 lp = (lnhead *)p;
 lp->row = 0; lp->flg = l_fbeg+l_fend;
 n = lp->h = cp->h; lp->lth = lth = sizeof(lnhead) + (n+1) * sizeof(interval);
 ip = (interval *)(lp+1); pp = cp->raster;
 do {
   b = byte_seg_size[*pp++]; ip->l = b >> 4; (ip++)->e = b & 15;
 } while (--n);
 (ip++)->l = 0; ((lnhead *)ip)->lth = 0;
 return lth + sizeof(lp->lth);
}

Word16 sort_events_vers()
{
 version * v, *a;
 Word16 n;
 Word8 let, prob;

 if (records_change == 0) goto ret;
 for (n=1, v = start_rec + 1; n < wcomp.nvers; n++, v++)
  {
   a = v;
   while ((a != start_rec) && (a->prob > (a-1)->prob))
    {
     let = a->let; prob = a->prob; a->let = (a-1)->let;
     a->prob = (a-1)->prob; a--; a->let = let; a->prob = prob;
    }
  }
 while (v-- != start_rec) if (v->prob) break;
 rec_ptr = v+1; wcomp.nvers = rec_ptr-start_rec;
ret:
 return rec_ptr - start_rec;
}
