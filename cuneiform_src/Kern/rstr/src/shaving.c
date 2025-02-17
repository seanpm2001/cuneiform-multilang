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
/*                                                                  */
/*			S K E L E T O N                             */
/*								    */
/********************************************************************/

# include <stdlib.h>
# ifndef SHV_DEBUG
# include "nt_types.h"
# include "struct.h"
# include "cuthdr.h"
# include "func.h"
# include "shaving.h"
#include "minmax.h"
# else
# include <stdio.h>
# include <conio.h>
# include <dos.h>

#define MIN(x,y)  ((x < y)? (x) : (y))

/*			Byte size data					*/

typedef signed char		CHAR;
typedef	unsigned char		BYTE;
typedef signed char near *	PCHAR;
typedef unsigned char near *	PBYTE;

/*			Word size data					*/

typedef signed short int		INT;
typedef unsigned short int		WORD;
typedef signed short int near *		PINT;
typedef unsigned short int near *	PWORD;

/*			Double word size data				*/

typedef signed long int			LONG;
typedef unsigned long int		DWORD;
typedef signed long int near *		PLONG;
typedef unsigned long int near *	PDWORD;

# define LEFT    0x4b
# define RIGHT   0x4d
# define UP      0x48
# define DOWN    0x50
# define F1      0x3b
# define F2      0x3c
# define F3      0x3d
# define F4      0x3e
# define F5      0x3f
# define F6      0x40
# define F7      0x41
# define F8      0x42
# define F9      0x43
# define F10     0x44

# define FILENAME_LENGTH 64

# define SCREEN_WIDTH    80
# define SCREEN_HEIGHT   25

# define SCR_LEFT        2
# define SCR_RIGHT       78
# define SCR_TOP         4
# define SCR_BOTTOM      23

# define STATUS_COLUMN   2
# define STATUS_ROW      2

# define MODE_COLUMN     2
# define MODE_ROW        24

# define TEXT_COLOR      CYAN
# define TEXT_BACKGROUND BLACK

# define FIELD_WIDTH     (SCR_RIGHT - SCR_LEFT + 1)
# define FIELD_HEIGHT    (SCR_BOTTOM - SCR_TOP + 1)

# define MAX_COLOR       LIGHTGRAY
# define MIN_COLOR       BLACK

# define WHITE_COLOR     MAX_COLOR
# define BLACK_COLOR     MIN_COLOR

INT cursor_beg, cursor_end;

static cursor_off                       (void);
static cursor_on                        (void);
static INT symbol_to_color              (INT);
static INT color_to_symbol              (INT);
static void init_field                  (void);
static void putch_cell                  (INT);
static void update_screen               (void);
static void goto_status                 (void);
static void clear_status                (void);
static void message                     (PCHAR);
static void smessage                    (PCHAR, PCHAR);
static void clear_buffer                (void);
static void load_field                  (void);
static void save_field                  (void);
static void update_mode_line            (void);
static void make_thinning               (void);
static void delete_skin                 (void);
static void make_unthinning             (void);
static void make_new                    (void);
static INT  account_neighbors           (INT x, INT);
static INT account_connect4_components  (INT, INT);
static print_object                     (void);
static INT Hall_algorithm               (void);
static INT compute_condition_N          (INT, INT);
static INT compute_condition_C          (INT, INT);
static INT Guo_and_Hall_algorithm       (void);
static INT skin                         (INT, INT);
static INT Karzanov_and_Komissarchik_algorithm (void);
static INT is_internal                  (INT, INT);
static INT check_shape                  (INT, INT, PCHAR, INT);
static INT check_picture                (INT, INT);
static INT Talalai_algorithm            (void);


static cursor_off ()
{
    union REGS inregs, outregs;

    inregs.h.ah = 3;
    int86 (0x10, &inregs, &outregs);
    cursor_beg = outregs.h.ch;
    cursor_end = outregs.h.cl;

    inregs.h.ah = 1;
    inregs.h.ch = 0x20;
    inregs.h.cl = 0;
    int86 (0x10, &inregs, &outregs);
}


static cursor_on ()
{
    union REGS inregs, outregs;

    inregs.h.ah = 1;
    inregs.h.ch = cursor_beg;
    inregs.h.cl = cursor_end;
    int86 (0x10, &inregs, &outregs);
}


static INT symbol_to_color (INT c)
{
    switch (c)
    {
	case ' ':  return (WHITE_COLOR);
	case '*':  return (BLACK_COLOR);
	default:
	    return (c < '0' || c > '7' ? BLACK_COLOR: (MAX_COLOR - (c - '0')));
    }
}


static INT color_to_symbol (INT c)
{
    switch (c)
    {
	case WHITE_COLOR:  return (' ');
	case BLACK_COLOR:  return ('*');
	default:           return ((MAX_COLOR - c) + '0');
    }
}


CHAR field [FIELD_WIDTH][FIELD_HEIGHT];
INT  white_color;

static void init_field (void)
{
    INT x, y;

    for (x = 0; x < FIELD_WIDTH; x++)
	for (y = 0; y < FIELD_HEIGHT; y++)
	    field [x][y] = WHITE_COLOR;

    white_color = WHITE_COLOR;
}


static void putch_cell (INT color)
{
    textbackground (color);
    putch (' ');
}


static void update_screen (void)
{
    INT x, y;

    cursor_off ();

    for (y = 0; y < FIELD_HEIGHT; y++)
    {
	gotoxy (SCR_LEFT, y + SCR_TOP);

	for (x = 0; x < FIELD_WIDTH; x++)
	    putch_cell (field [x][y]);
    }
    cursor_on ();
}


static void goto_status (void)
{
    gotoxy (STATUS_COLUMN, STATUS_ROW);
}


static void clear_status (void)
{
    textcolor (TEXT_COLOR);
    textbackground (TEXT_BACKGROUND);
    goto_status ();
    cprintf ("%*s", SCREEN_WIDTH - STATUS_COLUMN + 1, "");
}


static void message (PCHAR s)
{
    clear_status ();
    goto_status ();
    cputs (s);
}


static void smessage (PCHAR format, PCHAR s)
{
    clear_status ();
    goto_status ();
    cprintf (format, s);
}


static void clear_buffer (void)
{
    while (kbhit ()) getch ();
}


static void load_field (void)
{
    CHAR buffer[FILENAME_LENGTH + 3], *filename;
    FILE *f;
    INT  x, y;
    INT  c;

    message ("Enter filename to load : ");

    buffer[0] = FILENAME_LENGTH;
    if (!*(filename = cgets (buffer))) return;

    f = fopen (filename, "r");

    if (!f)
    {
        smessage ("Can't open file \"%s\"", filename);
        return;
    }

    init_field ();

    for (y = 1; y < FIELD_HEIGHT - 1; y++)
    {
        for (x = 1; x < FIELD_WIDTH - 1; x++)
            switch (c = getc (f))
            {
            case EOF:
                goto END_READING;

            case '\n':
                goto END_OF_LINE;

            default:
                field [x][y] = symbol_to_color (c);
		    break;
            }

        while ((c = getc (f)) != '\n' && c != EOF)
		;

      END_OF_LINE:
	;
    }

  END_READING:
    fclose (f);
    smessage ("File \"%s\" loaded", filename);
    update_screen ();
}


static void save_field (void)
{
    CHAR buffer[FILENAME_LENGTH + 3], *filename;
    FILE *f;
    INT  x, y;
    INT  c;

    message ("Enter filename to save : ");

    buffer[0] = FILENAME_LENGTH;
    if (!*(filename = cgets (buffer))) return;

    f = fopen (filename, "w");

    if (!f)
    {
        smessage ("Can't create file \"%s\"", filename);
        return;
    }

    for (y = 1; y < FIELD_HEIGHT - 1; y++)
    {
        for (x = 1; x < FIELD_WIDTH - 1; x++)
            putc (color_to_symbol (field [x][y]), f);

        putc ('\n', f);
    }

    smessage ("File \"%s\" saved", filename);
    fclose (f);
}

# define METHODS  4

INT Hall_algorithm (void);
INT Guo_and_Hall_algorithm (void);
INT Karzanov_and_Komissarchik_algorithm (void);
INT Talalai_algorithm (void);

struct {
      INT (*algorithm) (void);
      CHAR *method_name;
       }  method_desc [METHODS] =
{
 Hall_algorithm,
     "Fast Parallel Thinning Algorithm by Richard W.Hall",
 Guo_and_Hall_algorithm,
     "Two-Subiteration Algorithm by Zicheng Guo & Richard W. Hall",
 Karzanov_and_Komissarchik_algorithm,
     "Strictly Parallel Algoritm by A.Karzanov & E.Komissarchik",
 Talalai_algorithm,
     "Natural Thinning Algorithn by Alex Talalai",

};

INT method_number = 0;


static void update_mode_line (void)
{
    gotoxy (MODE_COLUMN, MODE_ROW);
    textcolor (TEXT_COLOR);
    textbackground (TEXT_BACKGROUND);
    cprintf ("%*s", SCREEN_WIDTH - MODE_COLUMN + 1, "");
    gotoxy (MODE_COLUMN, MODE_ROW);
    cputs (method_desc [method_number].method_name);
}


static void make_thinning (void)
{
    if (white_color == BLACK_COLOR - 1)
    {
	message ("Thinning stack overflow");
	return;
    }

    white_color--;
    (method_desc [method_number].algorithm) ();
    update_screen ();
}


static void delete_skin (void)
{
    INT x, y;

    for (x = 0; x < FIELD_WIDTH; x++)
	for (y = 0; y < FIELD_HEIGHT; y++)
	    if (field [x][y] != BLACK_COLOR)
            field [x][y] = WHITE_COLOR;

    white_color = WHITE_COLOR;
    update_screen ();
}


static void make_unthinning (void)
{
    INT x, y;

    if (white_color == WHITE_COLOR)
    {
        message ("Thinning stack empty");
        return;
    }

    for (x = 0; x < FIELD_WIDTH; x++)
	for (y = 0; y < FIELD_HEIGHT; y++)
	    if (field [x][y] == white_color)
            field [x][y] = BLACK_COLOR;

    white_color++;
    update_screen ();
}


static void make_new (void)
{
    init_field ();
    update_screen ();
}

/****************************************************/
void main (INT argc, CHAR **argv)
{
    INT x, y;
    INT c;

    textcolor (TEXT_COLOR);
    textbackground (TEXT_BACKGROUND);
    clrscr ();

    gotoxy (1, 1);
    cputs (
" *************** SKELETON ALGORITHMES SHOW **********************************"
    );

    gotoxy (1, SCREEN_HEIGHT);
    cputs (
" F1-Help F2-Save F3-Load F4-Mode  F5-Thin F6-UnThin F7-Shave  F9-New F10-Quit"
    );

    make_new ();
    update_mode_line ();

    x = 1;
    y = 1;

    for (;;)
    {
        gotoxy (x + SCR_LEFT, y + SCR_TOP);
        textcolor (MAX_COLOR - field [x][y]);

        switch (c = getch ())
        {
	    case 0:
		break;

	    case LEFT:
		if (x != 1) x--;
		break;

	    case RIGHT:
		if (x != FIELD_WIDTH - 2) x++;
		break;

	    case UP:
		if (y != 1) y--;
		break;

	    case DOWN:
		if (y != FIELD_HEIGHT - 2) y++;
		break;

	    case F1:
		break;

	    case F2:
		save_field ();
		break;

	    case F3:
		load_field ();
		break;

	    case F4:
		method_number = (method_number + 1) % METHODS;
		update_mode_line ();
		break;

	    case F5:
		make_thinning ();
		break;

	    case F6:
		make_unthinning ();
		break;

	    case F7:
		delete_skin ();
		break;

	    case F9:
		make_new ();
		break;

	    case F10:
		textcolor (TEXT_COLOR);
		textbackground (TEXT_BACKGROUND);
                clrscr ();
		exit (0);

	    default:
		putch_cell (field [x][y] = symbol_to_color (c));
		break;
        }
    }
}
# endif

/*
 *      C O M M O N   A L G O R I T H M E S   N O T A T I O N :
 *
 *               North
 *
 *          P1   P2   P3
 *  West    P8   P    P4    East
 *          P7   P6   P5
 *
 *               South
 *
 */

# ifdef SHV_DEBUG
# define WIDTH  FIELD_WIDTH
# define HEIGHT FIELD_HEIGHT
# define DELETABLE_MASK         0x10
# define IS_PIXEL(x, y)        ((x) > 0 && (x) < WIDTH  && \
                                (y) > 0 && (y) < HEIGHT && \
                                (field[x][y] & ~DELETABLE_MASK) \
                                    <= white_color)

# define SET_PIXEL(x, y)       (field[x][y] = BLACK_COLOR)
# define CLEAR_PIXEL(x, y)     (field[x][y] = (field[x][y] & DELETABLE_MASK) |\
                                   white_color)

# define SET_DELETABLE(x, y)   (field [x][y] |= DELETABLE_MASK)
# define IS_DELETABLE(x, y)    (field [x][y] & DELETABLE_MASK)
# define CLEAR_DELETABLE(x, y) (field [x][y] &= ~DELETABLE_MASK)
# endif

# define PP      (IS_PIXEL (x, y))
# define PP1     (IS_PIXEL (x - 1, y - 1))
# define PP2     (IS_PIXEL (x, y - 1))
# define PP3     (IS_PIXEL (x + 1, y - 1))
# define PP4     (IS_PIXEL (x + 1, y))
# define PP5     (IS_PIXEL (x + 1, y + 1))
# define PP6     (IS_PIXEL (x, y + 1))
# define PP7     (IS_PIXEL (x - 1, y + 1))
# define PP8     (IS_PIXEL (x - 1, y))

# define DP      (IS_DELETABLE (x, y))
# define DP1     (IS_DELETABLE (x - 1, y - 1))
# define DP2     (IS_DELETABLE (x, y - 1))
# define DP3     (IS_DELETABLE (x + 1, y - 1))
# define DP4     (IS_DELETABLE (x + 1, y))
# define DP5     (IS_DELETABLE (x + 1, y + 1))
# define DP6     (IS_DELETABLE (x, y + 1))
# define DP7     (IS_DELETABLE (x - 1, y + 1))
# define DP8     (IS_DELETABLE (x - 1, y))

# define PATH_LEN   8
INT path_x [PATH_LEN] = { -1,  0,  1,  1,  1,  0, -1, -1 };
INT path_y [PATH_LEN] = { -1, -1, -1,  0,  1,  1,  1,  0 };

# include <stdio.h>


static INT  account_neighbors (INT x, INT y)
{
    INT i;
    INT counter;
    INT wmod8;
    INT Xwrk, Ywrk;

    counter = 0;
/*****************************************************
    for (i = 0; i < PATH_LEN; i++)
	counter += IS_PIXEL(x+path_x[i],y+path_y[i]);
*******************************************************/

    wmod8 = (SHAVE_CELL->w+7)>>3;
    for (i = 0; i < PATH_LEN; i++)
    {
        Xwrk=(x+path_x[i]);
		Ywrk=(y+path_y[i]);
		if (
			  Xwrk >= 0 &&
			  Xwrk < SHAVE_CELL->w &&
			  Ywrk >= 0 &&
			  Ywrk < SHAVE_CELL->h
		   )

	       counter +=
        	 (
			  ((*(SHAVE_RASTER + Ywrk * wmod8 + Xwrk /8)
			    & (0x80 >> (Xwrk % 8))) != 0 )

			 ||

			  ((*(SHAVE_FLAGS + Ywrk * wmod8 + Xwrk /8)
			    & (0x80 >> (Xwrk % 8))) != 0 )
			 );
	 }

    return (counter);
}

/*
 *   F A S T   P A R A L L E L   T H I N N I N G   A L G O R I T H M
 *            B Y   R I C H A R D   W .   H A L L
 *
 *  1. Determine the deletability of all pixels P = 1.
 *     P is deletable if all of the following hold:
 *
 *     a. 1 < B (x, y) < 7
 *     b. P's 8-heighborhood contains exactly 1
 *        4-connected component (connected set) of ones
 *
 *  2. Delete all deletable pixels except those which satisfy
 *     one of the following:
 *
 *     a. P2 = P6 = 1 and P4 is deletable
 *     b. P4 = P8 = 1 and P6 is deletable
 *     c. P4, P5, P6 are deletable
 */


static INT account_connect4_components (INT x, INT y)
{
    INT i;
    INT color_changes;
    INT prev_color;
    INT color;

    color_changes = 0;
    prev_color    = IS_PIXEL (x + path_x [PATH_LEN - 1],
                              y + path_y [PATH_LEN - 1]);

    for (i = 0; i < PATH_LEN; i++)
    {
        color = IS_PIXEL (x + path_x[i], y + path_y[i]);

        if (color != prev_color)
        {
            color_changes++;
            prev_color = color;
        }
    }

    return (color_changes / 2);
}
/*

static print_object (void)
{
    INT x, y;

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
	    putchar (PP ? account_neighbors (x, y) + '0': '.');
        printf (": %2.2x%2.2x",   *R(0, y), *R(8, y));
        printf (": %2.2x%2.2x\n", *F(0, y), *F(8, y));
    }
    getch ();
}
*/


static INT Hall_algorithm (void)
{
    INT x, y;
    INT deleted_cells = 0;

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (PP)
	    {
	    int neighbors = account_neighbors (x, y);

            if (1 < neighbors && neighbors < 7  &&
		account_connect4_components (x, y) == 1)
		    SET_DELETABLE (x, y);
	    }

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (DP &&
		!(
		  PP2 && PP6 && DP4
			||
		  PP8 && PP4 && DP6
			||
		  DP6 && DP5 && DP4
		))
		{
		    CLEAR_PIXEL (x, y);
		    deleted_cells++;
		}

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    CLEAR_DELETABLE (x, y);

    return (deleted_cells);
}

/*
 *
 *         T W O - S U B I T E R A T I O N   A L G O R I T H M
 *    B Y   Z I C H E N G   G U O   &   R I C H A R  D   W .   H A L L
 *
 *  P is deleted, iff all of the following conditions
 *  are satisfied.
 *
 *     a. C (x, y) = 1
 *     b. 2 <= N (x, y) <= 3
 *     c. Apply one of the following:
 *        1. (P2 | P3 | ~P5) | P4 = 0 in odd iteration
 *        2. (P6 | P7 | ~P1) & P8 = 0 in even iteration
 *
 *  C (x, y) in condition can be evaluated efficiently by computing:
 *
 *     C (x, y) = ~P2 & (P3 | P4) + ~P4 & (P5 | P6)
 *              + ~P6 & (P7 | P8) + ~P8 & (P1 | P2)
 *
 *     N (x, y) = MIN (N1 (x, y), N2 (x, y))
 *
 *         N1 (x, y) = (P1 | P2) + (P3 | P4) + (P5 | P6) + (P7 | P8)
 *         N2 (x, y) = (P2 | P3) + (P4 | P5) + (P6 | P7) + (P8 | P1)
 *
 */


static INT compute_condition_N (INT x, INT y)
{
    INT n1 = (PP1 | PP2) + (PP3 | PP4) + (PP5 | PP6) + (PP7 | PP8);
    INT n2 = (PP2 | PP3) + (PP4 | PP5) + (PP6 | PP7) + (PP8 | PP1);
    return (MIN (n1, n2));
}


static INT compute_condition_C (INT x, INT y)
{
    return ((~PP2 & (PP3 | PP4)) + (~PP4 & (PP5 | PP6))
	      + (~PP6 & (PP7 | PP8)) + (~PP8 & (PP1 | PP2)));
}


static INT Guo_and_Hall_algorithm (void)
{
    INT x, y;
    INT iter;
    INT deleted_cells = 0;

    for (iter = 1; iter <= 2; iter++)
    {
        for (x = 0; x < WIDTH; x++)
            for (y = 0; y < HEIGHT; y++)
            {
		INT n = compute_condition_N (x, y);

                if (PP &&
                    2 <= n && n <= 3 &&
                    ((iter == 1) ?
                    ((PP2 | PP3 | !PP5) | PP4) == 0 :
                    ((PP6 | PP7 | !PP1) & PP8) == 0
                    ) &&
                    compute_condition_C (x, y) == 1
                   )
                SET_DELETABLE (x, y);

            }
    }

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (IS_DELETABLE (x, y))
	    {
		CLEAR_PIXEL (x, y);
                CLEAR_DELETABLE (x, y);
		deleted_cells++;
	    }

    return (deleted_cells);
}

/*
 *
 *   S T R I C T L Y   P A R A L L E L   A L G O R I T H M
 *   B Y   A . K A R Z A N O V   &   E . K O M I S S A R C H I K
 */


static INT skin (INT x, INT y)
{
    x=y;
    y=x+1;
    return (0);
}


INT Karzanov_and_Komissarchik_algorithm (void)
{
    INT x, y;
    INT deleted_cells = 0;

    Z = &string;
    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (skin (x, y))
		SET_DELETABLE (x, y);

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (IS_DELETABLE (x, y))
	    {
		CLEAR_PIXEL (x, y);
		deleted_cells++;
	    }

    return (deleted_cells);
}

/*
 *    N A T U R A L   T H I N N I N G   A L G O R I T H M
 *    B Y   A L E X   T A L A L A I
 */

typedef struct
      {
      INT   max_neighbors;
      INT   min_neighbors;
      PCHAR string;
      } SHAPE;

SHAPE picture_shape [] =
{
    7, 7, "*?.?.?* ",
    6, 6, "*?.?.*  ",
    6, 6, " *.?.?* ",
    5, 5, " *.?.*  ",
    5, 5, "  *?.?* ",
    4, 4, "  *?.*  ",
    4, 4, "   *.?* ",
    3, 2, "   *.*  ",
    3, 3, "    *?* ",
    0, 0
};


static INT is_internal (INT x, INT y)
{
    return (PP & PP2 & PP4 & PP6 & PP8);
}


static INT check_shape (INT x, INT y, PCHAR sh_string, INT offset)
{
    INT i, j;

    for (i = 0; i < PATH_LEN; i++)
    {
	j = (i + offset) % PATH_LEN;

	switch (sh_string [j])
	{
	    case ' ':
		if (IS_PIXEL (x + path_x [i], y + path_y [i]))
			return (0);
		break;

	    case '*':
		if (!IS_PIXEL (x + path_x [i], y + path_y [i]))
			return (0);
		break;

	    case '.':
		break;

	    case '?':
		if (!is_internal ((INT)(x + path_x [i]), (INT)(y + path_y [i])))
			return (0);
		break;
	}
    }

    return (1);
}


static INT check_picture (INT x, INT y)
{
    SHAPE *shape;
    INT   neighbors = account_neighbors (x, y);
    INT   angle_offset;

    if (!PP || neighbors == 8) return (0);

    for (shape = picture_shape; shape -> string; shape++)
    {
	if (neighbors > shape -> max_neighbors) return (0);
	if (neighbors < shape -> min_neighbors) continue;

	for (angle_offset = 0; angle_offset < 8; angle_offset += 2)
		if (check_shape (x, y, shape -> string, angle_offset))
			return (1);
    }

    return (0);
}


static INT Talalai_algorithm (void)
{
    INT x, y;
    INT deleted_cells = 0;

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (check_picture (x, y))
		SET_DELETABLE (x, y);

    for (x = 0; x < WIDTH; x++)
	for (y = 0; y < HEIGHT; y++)
	    if (IS_DELETABLE (x, y))
	    {
		CLEAR_PIXEL (x, y);
                CLEAR_DELETABLE (x, y);
		deleted_cells++;
	    }

    return (deleted_cells);
}

# ifndef SHV_DEBUG

INT shaving (INT method)
{
    switch (method)
    {
	case 1: return (Guo_and_Hall_algorithm ());
	case 2: return (Talalai_algorithm ());
	case 3: return (Hall_algorithm ());
/*	case 4: return (Karzanov_and_Komissarchik_algorithm ()); */
	default: return -1;
    }
}
# endif
