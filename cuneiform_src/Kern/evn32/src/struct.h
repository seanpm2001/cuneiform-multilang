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


#ifndef H_struct_h
#define H_struct_h

//#include "Tuner.H"
#include "recdefs.h"
#include "evndefs.h"
#include "minmax.h"


#ifdef  HUGE_IMAGE
	#define	WORLD_MAX_HEIGHT	10000
	#define WORLD_MAX_WIDTH		10000    //7000
	#define WORLD_MAX_RESOLUTION	800
#else
	#define	WORLD_MAX_HEIGHT	4096  // ordinary image
	#define	WORLD_MAX_WIDTH		4096
	#define WORLD_MAX_RESOLUTION	400
#endif

/*   Moved to evndefs.h - Paul 23.09.97
#define RASTER_MAX_HEIGHT	63
#define RASTER_MAX_WIDTH	128
*/
#define SMALL_SIZE 		8
#define err_pnlet(c)  ((c))->nextl=((c))->prevl=(cell*)(0xffff0000);
// AL 900318

/*   Moved to evndefs.h - Paul 23.09.97
//-------------------- conectivity component ---------------------

struct mn_struc
 {
 void *mnfirstbox; 		// address of the first box
 Int16 mncounter;     // number of living lines in the component
#define mnfree	mnfirstbox	// reference to next free main number
 Int16 mnupper;       // upper bound of component
 Int16 mnlower;       // lower bound of component
 Int16 mnboxcnt;      // number of boxes in component
#define usual_box_count 20 	// heuristic of number of lines in a letter
#define great_box_count	200	// heuristic for number of boxes in a picture
 Word8 mnlines;                  // number of lines in the component
 Word8 mnbegs;             // number of free line begins
 Word8 mnends;                   // number of free line ends
 Word8 mnflag;                   // flag byte for main number
#define mnpicture 1		// component is a picture
 struct mn_struc *mnnext;	// address of next dead component
 };
typedef struct mn_struc MN;
*/


//------------------- The box has a header ----------------------

struct box_struct
 {
 struct box_struct *boxnext; 	// chain address (zero if no next box)
 MN *		    boxmain; 	// component main number pointer
 Word16       boxptr;   // ptr to the empty place in the box
 Int16        boxleft;  // left boundary for line envelope
 Int16        boxright;   // right boundary for line envelope
 Int16        boxey;  // row of line end+1 ( if line ends within
				//    box)
 Int16                boxel;  // length of the last segment (if line ends
				//    within box)
 Int16              boxex;  // coordinate of last segment end (if line
				//    ends within box)
 Word8         boxflag;  // byte for box attributes flags
 Word8       boxwf;  // working flag (for picture compress)
 Word16       boxresw;  // reserved word (for *4 arround)
 };
typedef struct box_struct BOX;

// Values of boxflag:

#define BOXMARK 	1	// flag for temporary box marking
#define BOXPICTURE 	2	// the picture box - only header in the box is true
#define BOXFREEBEG	0x30	// line start type mask
#define BOXFREEEND 	0xc0	// line end type mask
//	The 'boxfreebeg' and 'boxfreeend' bites has following meaning:
//	'11' - free begin/end of line
//	'01' - not free begin/end
//	'00' - no information about begin/end in this box
#define BOXBEG 		0x10	// line start not free
#define BOXEND		0x40	// line end not free

// BOX size

#define BOXHLTH	(sizeof(BOX))	// the box header length
#define BOXSIZE (BOXHLTH+32*4)	// the length of box
#define BOXBOUNDARY (BOXSIZE-6)	// the boundary control

struct ldescr_struct
 {
 Int16 y;   // y coord. of the first interval
 Int16 l;   // first interval length
 Int16 x;   // x coord. of the end of the first interval
 };
typedef struct ldescr_struct LNSTRT;

struct box_interval
 {
 Int16 l;   // length of interval
 Int16 d;   // displacement of the end of the current interval relative
 };      	//      to the previous
typedef struct box_interval BOXINT;

//-------------------- component in kit --------------------------

struct comp_struc
 {
 Word16 size;       // size of component in paragraphs >=3
 Int16 upper;   // upper boundary of component
 Int16 left;    // left boundary of component
 Int16 h;     // height of component
 Int16 w;           // width of component
 Word8 rw;               // raster width in bytes
 Word8 type;       // recognition type
#define ch_perfect    1	    // perfect type defined
#define ch_letter     2	    // letter type
#define ch_dust       4	    // dust type
#define ch_punct      8	    // punctuation
#define ch_stick     16	    // vertical bar component
#define ch_great     32	    // great component
#define ch_merge     64	    // merged components
#define ch_notltr   128	    // not letter or punctuation
 Word8 cs;         // recognition case (see bellow)
 Word8 pidx;         // proportional index (ref.)
 Int16 nvers;   // number of alternative versions
 Int16 records;   // recognition records offset
 Int16 lines;   // ptr to line representation
 Int16 nl;    // number of lines
 Word8 begs;       // number of free begins
 Word8 ends;         // number of free ends
 Word8 reasno;           // proportional criteria messages
 Word8 large;    // large type
#define ch_underlined	1	// component was underlined
#define ch_taken	2	// taken to line at dust_ini
 Word8 scale;      // scale of the component
// union {
// Word8 stairs[4];                // bit_map for stairs_vector components
 struct comp_struc * next;
//       };
 };
typedef struct comp_struc c_comp;

//----------------------- cell ----------------------------------

#define VERS_IN_CELL 16 // Vald. 06-04-94

struct vers_struct  	// versions with probabilities
 {
 Word8 let;    // character
 Word8 prob;   // probability
 };
typedef struct vers_struct version;

//-------------------- line representation ----------------------

//	At the beginning of line representation - word of total length -
//	not use it, simply skip
//	At end of each line zero byte as mark of line end
//	After last line zero word

//	line header
struct ln_head
 {
 Int16 lth; // length of one line representation
 Int16 h;   // height of line
 Int16 row; // relative row of line start
 Word16 flg;  // flags of free beg and free end
#define l_fbeg		0x20
#define l_fend		0x80
#define l_cbeg          0x02
#define l_cend          0x08
 };
typedef struct ln_head lnhead;

//	one interval
struct int_s
 {
 Word8 l;//Int16 l;  // length of interval
 Word8 e;//Int16 e;  // end of interval coordinates
 };
typedef struct int_s interval;

struct large_int_s
 {
 Word16 l;  // length of interval
 Word16 e;  // end of interval coordinates
 };
typedef struct large_int_s large_interval;




#endif


