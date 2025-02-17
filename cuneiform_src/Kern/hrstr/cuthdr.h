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


#define prot_size 2

#define MAX_LINES 64	// maximum number of shortened lines in component
#define MAX_CELLS_IN_LIST 8
#define MAX_VERS_BOX 8
#define MERGE1 0x1
#define MERGE2 0x2
#define SPLIT  0x4
#define CONT   0x8
#define MAX_HOR 128
#define MAX_VERT 30

#define MAX_GLUE_COMP 8
#define LEFT_DISTANCE_MAX  10	// in % of width of letter
#define LEFT_DISTANCE_MIN  20
#define RIGHT_DISTANCE_MIN 10
#define RIGHT_DISTANCE_MAX 20

#define GFbadl   2      // may involve bad  at left side
#define GFbadr   1
#define GFsrc    4      // cell OR newcell to be used for makerast
#define GFndd    8      // don't disriminate by dust
///////// #define GFfinal  128    // decision will be treated as final
#define GFcut    64     // pattern produced by cut (not by Tiger)
#define GFtiger  32     // pattern estimated by Tiger

#define GRdust 4        // dust has been cut
#define GRflo  8        // {0OoQD} - not to be cut

#define GAtigr 1	// estimate by tiger
#define GABOXl 2        // estimate by BOX with NO LINEAR CRIT
#define GAbox  8        // try to take all-in-box to BOX
#define GAsur  4    // try to take all surrounding to BOX
#define GAown  16       // try to estimate as is
#define GABOXR  32       // BOX REQURED (not only allowed)
#define GABOXs  64       // BOX sticks REQURED (not only allowed)
#define GABOXt  128      // BOX estimate TIGR versions (not to apply full BOX)
#define GAallest GAbox+GAsur+GAown

struct andrew
 {
// char dum[prot_size];

 CHAR a_Totalth;
 CHAR a_bestc, a_best, a_bestx, a_bestxr, a_bestxh;
 CHAR a_Exbn, a_Exbx, a_Exfn, a_Exhn, a_Eahn, a_Eafn;
 CHAR a_Extrem, a_ExtreM, a_Extrex, a_ExtreX,
      a_Exbm, a_ExbM, a_ExfM, a_Exfm, a_ExhM, a_Exhm;
 CHAR a_Lbnd, a_lbnd, a_Rbnd, a_rbnd, a_lwpnt, a_rwpnt;
 CHAR a_nmax1, a_nmax2, a_bsnx;
 CHAR a_Startp, a_Endp, a_botflag;
 CHAR a_FlM, a_Nmb, a_lastmax, a_NBmax;
 struct extrem_elem *a_extrp;
 struct extrem_elem *a_extrc;
 struct extrem_elem *a_extrn;

 struct info_elm a_points[MAX_HOR];
 struct extrem_elem a_Exbody[MAX_HOR/2];
 struct extrem_elem a_Exfoot[MAX_HOR/2];
 struct extrem_elem a_Exhead[MAX_HOR/2];
 CHAR a_bodies[MAX_HOR];
 CHAR a_footes[MAX_HOR];
 CHAR a_heades[MAX_HOR];
 CHAR a_linums[MAX_HOR];
 CHAR a_linumsx[MAX_HOR];
 };

struct common
 {
 char dum[prot_size];
 struct own_cut *c_odrans;
 INT  c_pa8;
 INT  c_path_lth;
 CHAR c_S_flag;		// flag of "S"-case
 INT  c_s_width,c_s_width_b,c_s_height;
 INT  c_t_width,c_t_width_b,c_t_height;
 MN   *c_t_comp_ptr;   	// pointer to component header
 INT  c_t_left_shift;
 CHAR c_cut_flag;
 CHAR c_best_cut_point;
 lnhead *c_t_line_ptr;
 INT  c_max_line;
 BYTE *c_s_raster;
 struct less_vertex_elem c_less_vertices[MAX_LINES];
 struct short_line_header c_lines[MAX_LINES];
 BYTE c_path[MAX_LINES];
// char dum[prot_size];
 };

struct edik
 {
 INT  e_first_line;	// number of first (shortened) line in list
 INT  e_n_lines;
 INT  e_n_verts;
 struct short_line_header *e_fl_ptr, *e_sl_ptr;
 struct segment *e_segm_ptr;
 struct vertex_header *e_vert_ptr;
 CHAR e_meet_flag;
 CHAR e_wr_vertex_flag;
 struct vertex_header e_verts[MAX_LINES];
 CHAR e_adj_matrix[MAX_LINES][MAX_LINES];
 };

union Work
 {
 struct
  {
  BYTE w_rast[1024];
  BYTE a_sum[32];
  CHAR a_end[3][32], a_lth[3][32],
       a_dif0[32], a_dif1[32], a_dif2[32];
  CHAR a_roof[128];
  } ct;

 struct
  {
  struct common c;
  union
   {
   struct edik e;
   struct andrew a;
   } u;
  } cp;
 };

extern union Work string;
#define W string

// atlant's memory
#define cell_raster     W.ct.w_rast
//#define at_roof         W.ct.a_roof
//#define at_end          W.ct.a_end
//#define at_lth          W.ct.a_lth
//#define at_sum          W.ct.a_sum
//#define at_dif0         W.ct.a_dif0
//#define at_dif1         W.ct.a_dif1
//#define at_dif2         W.ct.a_dif2

// common (cutgraph & cutpoint)
#define Ed_lines	W.cp.c.c_lines
#define path		W.cp.c.c_path
#define odrans          W.cp.c.c_odrans
#define pa8             W.cp.c.c_pa8
#define path_lth	W.cp.c.c_path_lth
#define S_flag		W.cp.c.c_S_flag
#define	less_vertices	W.cp.c.c_less_vertices
#define t_width		W.cp.c.c_t_width
#define	s_width		W.cp.c.c_s_width
#define	s_width_b	W.cp.c.c_s_width_b
#define	s_height	W.cp.c.c_s_height
#define	t_width_b	W.cp.c.c_t_width_b
#define	t_height	W.cp.c.c_t_height
#define	cut_flag	W.cp.c.c_cut_flag
#define	best_cut_point	W.cp.c.c_best_cut_point
#define s_raster	W.cp.c.c_s_raster
#define	t_line_ptr	W.cp.c.c_t_line_ptr
#define	max_line	W.cp.c.c_max_line
#define	t_comp_ptr	W.cp.c.c_t_comp_ptr
#define	t_left_shift	W.cp.c.c_t_left_shift

// cutpoint
#define bodies		W.cp.u.a.a_bodies
#define footes		W.cp.u.a.a_footes
#define heades		W.cp.u.a.a_heades
#define linums		W.cp.u.a.a_linums
#define linumsx	        W.cp.u.a.a_linumsx
#define Totalth	        W.cp.u.a.a_Totalth
#define Exbody		W.cp.u.a.a_Exbody
#define Exfoot		W.cp.u.a.a_Exfoot
#define Exhead		W.cp.u.a.a_Exhead
#define bestc		W.cp.u.a.a_bestc
#define best		W.cp.u.a.a_best
#define bestx		W.cp.u.a.a_bestx
#define bestxr		W.cp.u.a.a_bestxr
#define bestxh		W.cp.u.a.a_bestxh
#define bestv		W.cp.u.a.a_bestv
#define bestvx		W.cp.u.a.a_bestvx
#define bestvc		W.cp.u.a.a_bestvc
#define Exbn		W.cp.u.a.a_Exbn
#define Exfn		W.cp.u.a.a_Exfn
#define Exhn		W.cp.u.a.a_Exhn
#define Extrem		W.cp.u.a.a_Extrem
#define ExtreM		W.cp.u.a.a_ExtreM
#define points 	        W.cp.u.a.a_points
#define ExbM		W.cp.u.a.a_ExbM
#define Exbm		W.cp.u.a.a_Exbm
#define ExfM		W.cp.u.a.a_ExfM
#define Exfm		W.cp.u.a.a_Exfm
#define ExhM		W.cp.u.a.a_ExhM
#define Exhm		W.cp.u.a.a_Exhm
#define Lbnd		W.cp.u.a.a_Lbnd
#define lbnd		W.cp.u.a.a_lbnd
#define Rbnd		W.cp.u.a.a_Rbnd
#define rbnd		W.cp.u.a.a_rbnd
#define extrp		W.cp.u.a.a_extrp
#define extrc		W.cp.u.a.a_extrc
#define extrn		W.cp.u.a.a_extrn
#define Startp		W.cp.u.a.a_Startp
#define Endp		W.cp.u.a.a_Endp
#define botflag         W.cp.u.a.a_botflag
#define nmax1           W.cp.u.a.a_nmax1
#define nmax2           W.cp.u.a.a_nmax2
#define bsnx            W.cp.u.a.a_bsnx
#define FlM             W.cp.u.a.a_FlM
#define Axhead          W.cp.u.a.a_Axhead
#define Axfoot          W.cp.u.a.a_Axfoot
#define Eahn            W.cp.u.a.a_Eahn
#define Eafn            W.cp.u.a.a_Eafn
#define lastmax         W.cp.u.a.a_lastmax
#define NBmax           W.cp.u.a.a_NBmax
#define Nmb             W.cp.u.a.a_Nmb
#define Exbx            W.cp.u.a.a_Exbx
#define Extrex          W.cp.u.a.a_Extrex
#define ExtreX          W.cp.u.a.a_ExtreX
#define lwpnt           W.cp.u.a.a_lwpnt
#define rwpnt           W.cp.u.a.a_rwpnt

// cutgraph
#define	verts		W.cp.u.e.e_verts
#define	adj_matrix	W.cp.u.e.e_adj_matrix
#define	first_line	W.cp.u.e.e_first_line
#define	n_lines		W.cp.u.e.e_n_lines
#define	n_verts		W.cp.u.e.e_n_verts
#define	fl_ptr		W.cp.u.e.e_fl_ptr
#define	sl_ptr		W.cp.u.e.e_sl_ptr
#define	segm_ptr	W.cp.u.e.e_segm_ptr
#define	vert_ptr	W.cp.u.e.e_vert_ptr
#define	meet_flag	W.cp.u.e.e_meet_flag
#define	wr_vertex_flag	W.cp.u.e.e_wr_vertex_flag

#define prot_dum        W.cp.c.dum

static union Work *Z;

