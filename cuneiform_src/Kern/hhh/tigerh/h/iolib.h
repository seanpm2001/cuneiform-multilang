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


//
// 08-14-93 02:46pm, Mike
//
// 1. Function TBFOPEN has been added as a temporary (?) solution.
//    This function is needed at the modules SPELABC.C & SPELSPEC.C.
//

#include "c_types.h"

#define TG_STREAM  1
#define TB_STREAM  2
#define ED_STREAM  3
#define TX_STREAM  4
#define DB_STREAM  5
#define IM_STREAM  6
#define VC_STREAM  7
#define WK_STREAM  8

#ifdef PC_TYPE

#include <stdlib.h>
/*#include <io.h>*/
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <bios.h>

#define BO_CREAT      O_CREAT|O_TRUNC|O_BINARY|O_RDWR,S_IREAD|S_IWRITE
#define BO_READ       O_RDONLY|O_BINARY
#define BO_READ_TEXT  O_RDONLY|O_TEXT
#define BO_RDWR       O_RDWR|O_BINARY
#define BO_CREAT_FL   O_CREAT|O_TRUNC|O_BINARY|O_RDWR
#define BO_READ_FL    O_RDONLY|O_BINARY
#define BO_RDWR_FL    O_RDWR|O_BINARY
#define BO_I_FL       S_IREAD|S_IWRITE
#define BO_WRITE      O_WRONLY|O_BINARY
#define DIRSEP        '/'

# else

#include <stdlib.h>
#include <fcntl.h>

#define BO_CREAT      O_CREAT|O_TRUNC|O_BINARY|O_RDWR,0
#define BO_READ       O_RDONLY|O_BINARY
#define BO_READ_TEXT  O_RDONLY|O_TEXT
#define BO_RDWR       O_RDWR|O_BINARY
#define BO_CREAT_FL   O_CREAT|O_TRUNC|O_BINARY|O_RDWR
#define BO_READ_FL    O_RDONLY|O_BINARY
#define BO_RDWR_FL    O_RDWR|O_BINARY
#define BO_I_FL       0
#define BO_WRITE      O_WRONLY|O_BINARY
#define S_IREAD       0
#define S_IWRITE      0
#define DIRSEP        ':'
#define O_TEXT        0

#endif

/* /------------------ I/O prototypes ------------------------------- */



#define TGFILELTH  my_filelth
#define TGOPEN     my_open
#define TGFOPEN    my_fopen
#define TGREAD     my_read
#define TGWRITE    my_write
#define TGCLOSE    my_close
#define TGSEEK     my_seek
#define TGSIZE     my_chsize
#define TBOPEN     my_table_op
#define TGREMOVE   my_file_rem

// 08-14-93 02:49pm, Mike
#define TBFOPEN     my_table_fop
#define TGGETS      handle_fgets

#ifdef __cplusplus
extern "C" {
#endif

LONG my_filelth (INT);                      /* / handle    */
INT  my_open    (INT, CHAR *, INT, INT);    /* / stream, name, ... */
FILE *my_fopen  (INT, CHAR *, CHAR *);      /* / stream, name, ... */
void my_file_rem (INT, CHAR *);             /* / stream, name      */
INT  my_close   (INT);                      /* / handle            */
LONG my_read    (INT, CHAR *, LONG);        /* / handle, addr, lth */
LONG my_write   (INT, CHAR *, LONG);        /* / handle, addr, lth */
LONG my_seek    (INT, LONG, INT);           /* / handle, addr, type*/
INT  my_chsize  (INT, LONG);                /* / handle, addr      */
BOOL save_dir   (INT, void * );             /* / stream, name      */
INT  is_open    (INT);                      /* / stream            */
void set_name_ext   (CHAR *);               /* / stream=TX; extention  */
INT  my_table_op (INT, INT, INT, INT);      /* / table #, ...          */

// 08-14-93 02:49pm, Mike
FILE *my_table_fop (INT, INT, CHAR *);      /* / table #, ...          */
CHAR *handle_fgets ( INT handle, CHAR * s, INT len );

#ifdef __cplusplus
}
#endif
