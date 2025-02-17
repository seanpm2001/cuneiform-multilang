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

#ifndef __CEDINT_H
#define __CEDINT_H

#include <stdio.h>

#ifndef __GLOBUS_H
#include "globus.h"
#endif
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __cplusplus
	extern "C" {
#endif

//
#pragma pack (push,8)

#define CED_MAXNAME 260

#ifndef Handle
	#define Handle void *
#endif

#define CFIOHightCode 0x0001

#include "ced.h"
#include "ced_struct.h"
#include "cfio.h"
//#include "edfile.h"

#ifdef _DEBUG
#define EDASSERT(f) \
	{if (!(f)) \
		__asm { int 3 };}
#else
#define EDASSERT(f)
#endif

#include "compat_defs.h"

extern FNRDProc RDProced;//pointer to function, which processes raw data
void SetReturnCode_ced(Word32 rc);//set error code
Word32 GetReturnCode_ced();

//function, exported from CFIO
extern	FNCFIOReadMemoryFromFile    MemFromFile;              //19
extern	FNCFIOLockMemory					Lock;
extern	FNCFIOUnlockMemory				Unlock;
extern	FNCFIOFreeMemory					Free;
//extern	FNCFIOAllocMemory					Alloc;
extern	FNCFIOOpenFreeFile					Open;
extern	FNCFIOCloseFreeFile					Close;
extern	FNCFIOWriteToFile					Write;

#define DEC_FUN(a,b,c) a My##b c;a MyRawData##b c;extern FN##b b

DEC_FUN(void, CED_BitmapRef,(const bit_map_ref* pt));
DEC_FUN(void, CED_TextRef,(const text_ref* pt));
DEC_FUN(void, CED_FontKegl,(const font_kegl *pt));
DEC_FUN(void, CED_Kegl,(const kegl* pt));
DEC_FUN(void, CED_Shift,(const shift* pt));
DEC_FUN(void, CED_RetrieveLevel,(const retrieve_level* pt));
DEC_FUN(void, CED_Underline,(const underline* pt));
DEC_FUN(void, CED_DensPrint,(const dens_print* pt));
DEC_FUN(void, CED_Tabul,(const tabul* pt));
DEC_FUN(void, CED_TablTabul,(const tabl_tabul* pt));
DEC_FUN(void, CED_SheetDiskDescr,(const sheet_disk_descr* pt));
DEC_FUN(void, CED_FragmDiskDescr,(const fragm_disk_descr* pt));
DEC_FUN(void, CED_FragmDisk,(const fragm_disk* pt));
DEC_FUN(void, CED_StepBack,(const step_back* pt));
DEC_FUN(void, CED_LineBeg,(const line_beg* pt));
DEC_FUN(void, CED_Position,(const position* pt));
DEC_FUN(void, CED_EdTagLanguage,(const EdTagLanguage* pt));
DEC_FUN(void, CED_TableConformSizes,(const table_conform_sizes* pt));
DEC_FUN(void, CED_GroupWords,(const group_words* pt));
DEC_FUN(void, CED_GroupSymbols,(const group_symbols* pt));
DEC_FUN(void, CED_Border,(const border* pt));
DEC_FUN(void, CED_TableHeader,(const table_header* pt));
DEC_FUN(void, CED_ListOfFragments,(const list_of_fragments* pt));
DEC_FUN(void, CED_Extention,(const edExtention* pt,const void* ptExt));
DEC_FUN(void, CED_ExtentionNew,(const edExtentionNew* pt,const void* ptExt));
DEC_FUN(void, CED_Aksant,(const aksant* pt));
DEC_FUN(void, CED_Letter,(const letter* pt,const Word32 alternatives));

#undef DEC_FUN


#define DEC_FUN(a,b,c) a b c

DEC_FUN(Word32, CED_ReadED,(char * file,Bool32 readFromFile, Word32 bufLen));
DEC_FUN(void,CED_SetRawDataProc,(FNRDProc proc));
DEC_FUN(CEDPage*,CED_FormattedLoad,(char * file,Bool32 readFromFile, Word32 bufLen));
DEC_FUN(Bool32,CED_FormattedWrite,(char * fileName, CEDPage *page));
DEC_FUN(void,CED_DeleteTree,(CEDPage * pg));
/*
#ifdef _DEBUG
DEC_FUN(void, CED_ShowTree,(char * name, Handle hEdPage));
#endif

DEC_FUN(Bool32, CED_IsEdFile, (char * file,Bool32 readFromFile, Word32 bufLen));

DEC_FUN(Handle, CED_CreatePage,(char * _imageName,EDSIZE _sizeOfImage,EDSIZE _dpi,int _turn,int _pageNumber,EDSIZE _sizeInTwips, EDRECT _pageBordersInTwips, char _unrecogChar));
DEC_FUN(Bool32, CED_CreateFont,(Handle hEdPage, BYTE fontNumber, BYTE fontPitchAndFamily, BYTE fontCharset,char* fontName));
DEC_FUN(Bool32, CED_CreatePicture,(Handle hEdPage, int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign, int type, void * data, int len));
DEC_FUN(Handle, CED_CreateSection,(Handle hEdPage,EDRECT border, char sectionBreak, int width, int height, char orientation, int headerY, int footerY));
DEC_FUN(Handle, CED_CreateColumn,( Handle hEdSection,int width, int space));
DEC_FUN(Handle, CED_CreateParagraph,(Handle hEdSection, Handle hObject, int align, EDRECT indent, int UserNum, int FlagBorder, EDSIZE interval, EDBOX layout, int  color, int  shading, int spaceBetweenLines, char spcBtwLnsMult, char  keep));
DEC_FUN(Handle, CED_CreateLine,(Handle hEdParagraph));
DEC_FUN(Handle, CED_CreateChar,(Handle hEdLine, EDBOX layout, int fontHeight, int fontNum, int fontStyle, letterEx* alternatives));
DEC_FUN(Handle, CED_CreateFrame,(Handle hEdSection, Handle hObject, edBox rect, char position, DWORD borderSpace, DWORD dxfrtextx, DWORD dxfrtexty));
DEC_FUN(Handle, CED_CreateCell,(Handle hEdSection,Handle hEdTable));
DEC_FUN(Handle, CED_CreateTable,(  Handle hEdSection, Handle hObject,EDSIZE sz, int * cx,int * cy, int * table,Bool32 * bHorShow,Bool32 * bVerShow));
DEC_FUN(void, CED_DeletePage,(Handle hEdPage));
DEC_FUN(Handle, CED_ReadFormattedEd,(char * lpEdFile,Bool32 readFromFile, Word32 bufLen));
DEC_FUN(Bool32, CED_WriteFormattedEd,(char * lpEdFileName, Handle hEdPage));
DEC_FUN(char*, CED_GetPageImageName,(Handle hEdPage));
DEC_FUN(EDSIZE, CED_GetPageImageSize,(Handle hEdPage));
DEC_FUN(EDSIZE,CED_GetPageDpi,(Handle hEdPage));
DEC_FUN(Word32, CED_GetPageTurn,(Handle hEdPage));
DEC_FUN(EDSIZE,CED_GetPageSize,(Handle hEdPage));
DEC_FUN(char,CED_GetPageUnrecogChar,(Handle hEdPage));
DEC_FUN(Word32,CED_GetNumberOfParagraphs,(Handle hEdPage));
DEC_FUN(Handle, CED_GetParagraph,(Handle hEdPage,int _num));
DEC_FUN(Bool32, CED_GetFont,(Handle hEdPage,int number, BYTE* fontNumber, BYTE* fontPitchAndFamily, BYTE* fontCharset,char* fontName));
DEC_FUN(int, CED_GetNumOfFonts,(Handle hEdPage));
DEC_FUN(Word32,CED_GetCountSection,(Handle hEdPage));
DEC_FUN(Handle, CED_GetSection,(Handle hEdPage, Word32 number));
DEC_FUN(EDRECT, CED_GetSectionBorder,(Handle hEdSection));
DEC_FUN(Word32, CED_GetCountColumn,(Handle hEdSection));
DEC_FUN(Handle, CED_GetColumn,(Handle hEdSection,int number));
DEC_FUN(Word32, CED_GetColumnWidth,(Handle hEdColumn));
DEC_FUN(Word32, CED_GetColumnSpacing,(Handle hEdColumn));
//DEC_FUN(Word32, CED_GetCountFrame,(Handle hEdSection));
//DEC_FUN(Handle, CED_GetFrame,(Handle hEdSection,int number));
DEC_FUN(edBox, CED_GetFrameRect,(Handle hEdFrame));
DEC_FUN(Handle, CED_GetFirstObject,(Handle hObject));
DEC_FUN(Handle, CED_GetNextObject,(Handle hObject));
DEC_FUN(Bool32, CED_IsTable,(Handle hObject));
DEC_FUN(Bool32, CED_IsPicture,(Handle hObject));
DEC_FUN(Bool32, CED_IsFrame,(Handle hObject));
DEC_FUN(Bool32, CED_IsParagraph,(Handle hObject));
DEC_FUN(Bool32, CED_IsFictive,(Handle hEdParagraph));
DEC_FUN(Word32, CED_GetCountCell,(Handle hEdTable));
DEC_FUN(Handle, CED_GetCell,(Handle hEdTable, int number));
DEC_FUN(Int32*,CED_GetLinesX,(Handle hEdTable));
DEC_FUN(Int32 *,CED_GetLinesY,(Handle hEdTable));
DEC_FUN(Word32*,CED_GetTableOfCells,(Handle hEdTable));
DEC_FUN(EDSIZE, CED_GetSize,(Handle hEdTable));
DEC_FUN(EDRECT, CED_GetIndent,(Handle hEdParagraph));
DEC_FUN(Word32,CED_GetAlignment,(Handle hEdParagraph));
DEC_FUN(EDBOX, CED_GetLayout,(Handle hEdParagraph));
DEC_FUN(Word32, CED_GetUserNumber,(Handle hEdParagraph));
DEC_FUN(EDSIZE, CED_GetInterval,(Handle hEdParagraph));
DEC_FUN(Word32, CED_GetCountLine,(Handle hEdParagraph));
DEC_FUN(Handle, CED_GetLine,(Handle hEdParagraph,int number));
DEC_FUN(Word32, CED_GetCountChar,(Handle hEdLine));
DEC_FUN(Handle, CED_GetChar,(Handle hEdLine,int number));
DEC_FUN(struct letterEx*, CED_GetAlternatives,(Handle hEdChar));
DEC_FUN(Word32, CED_GetFontHeight,(Handle hEdChar));
DEC_FUN(Word32, CED_GetFontStyle,(Handle hEdChar));
DEC_FUN(Word32, CED_GetFontLang,(Handle hEdChar));
DEC_FUN(EDBOX, CED_GetCharLayout,(Handle hEdChar));
*/
#undef DEC_FUN

CEDPage * Formattedload_96(char * file,Bool32 readFromFile, Word32 bufLen);

#pragma pack(1)
struct pageDescr
{
	DWORD paperw;
	DWORD paperh;
	DWORD margl;
	DWORD margr;
	DWORD margt;
	DWORD margb;
	BYTE  resizeToFit;
	BYTE  recogLang;
};

#pragma pack(1)
struct fontDiscr
{
	WORD size;
    BYTE fontNumber;
    BYTE fontPitchAndFamily;
    BYTE fontCharset;
};

#pragma pack(1)
struct pictDescr
{
	DWORD size;
	WORD pictNumber;
	EDSIZE pictSize;
	EDSIZE pictGoal;
	BYTE pictAlign;
	BYTE type;
	DWORD len;
};

#pragma pack(1)
struct originalImageDesc
{
	WORD resolutionX;
	WORD resolutionY;
	WORD inclune;
    WORD pageNum;
    DWORD width;
    DWORD height;
	BYTE unrecogSymbol;
};

#pragma pack(1)
struct sectParams1
{
    DWORD topMargin;
    DWORD bottomMargin;
    DWORD leftMargin;
    DWORD rightMargin;
	BYTE columns;
	BYTE numSnakeCols;
    DWORD colInterval;
};
#pragma pack(1)
struct sectParams2
{
	BYTE sectionBreak;
    DWORD width;
    DWORD height;
	BYTE orientation;
	DWORD headerY;
	DWORD footerY;
	BYTE lineBetCol;
};

#pragma pack(1)
struct frameParam
{
	BYTE position;
    DWORD posx;
    DWORD posy;
    DWORD absw;
    DWORD absh;
	DWORD borderSpace;
	DWORD dxfrtextx;
	DWORD dxfrtexty;
	BYTE flag;
};

#pragma pack(1)
struct rowParam
{
	DWORD left;
	DWORD rowHeight;
	BYTE leftBrdrType;
	DWORD leftBrdrWidth;
	BYTE rightBrdrType;
	DWORD rightBrdrWidth;
	BYTE topBrdrType;
	DWORD topBrdrWidth;
	BYTE bottomBrdrType;
	DWORD bottomBrdrWidth;
	DWORD gaph;
	BYTE position;
	BYTE header;
};

#pragma pack(1)
struct cellParam
{
	DWORD cellX;
	BYTE merging;
	BYTE vertTextAlign;
	BYTE leftBrdrType;
	DWORD leftBrdrWidth;
	BYTE rightBrdrType;
	DWORD rightBrdrWidth;
	BYTE topBrdrType;
	DWORD topBrdrWidth;
	BYTE bottomBrdrType;
	DWORD bottomBrdrWidth;
	EDBOX layout;
	WORD shading;
	WORD color;
	BYTE flag;
};

#pragma pack(1)
struct paraParams
{
	DWORD firstIndent;
	DWORD leftIndent;
    DWORD rightIndent;
    BYTE  alignment;
	WORD  userNum;
	WORD  color;
	WORD  shading;
    DWORD spaceBefore;
    DWORD spaceAfter;
    DWORD spaceBetweenLines;
	BYTE  spcBtwLnsMult;
	BYTE  keep;
	BYTE leftBrdrType;
	DWORD leftBrdrWidth;
	BYTE rightBrdrType;
	DWORD rightBrdrWidth;
	BYTE topBrdrType;
	DWORD topBrdrWidth;
	BYTE bottomBrdrType;
	DWORD bottomBrdrWidth;
	BYTE brdrBtw;
};

#pragma pack(1)
struct charParams
{
	DWORD fontNumber;
	DWORD foregroundColor;
	DWORD backgroundColor;
};
#pragma pack()

#define MAX_WIDTH 300
#define MAX_RTF_GROUPS     50

struct StrRtfColor {                       // color table in an rtf file
      COLORREF color;                      // color
      };

struct StrRtfOut {                        // Rtf output file processing block
      int  output;                        // rtr output type: RTF_FILE,RTF_BUF,RTF_CB
      HANDLE hFile;                        // stream if file is used
      HGLOBAL hBuf;                       // output buffer handle if using buffer output
      BYTE *buf;                     // output buffer pointer if using buffer
      long BufLen;                        // allcated length of buffer
      long BufIndex;                      // index of next character in the buffer
      char text[MAX_WIDTH+1];             // temporary buffer to write text
      int  TextLen;                       // length of the text in the 'text' buffer
      BOOL SpacePending;                  // TRUE if space needs to be written out after the last control
      BOOL WritingControl;                // TRUE when writing a control word
      struct StrRtfColor *color;      // rtf color table pointer
      int  TotalColors;                   // total colors in the color table
      int  GroupLevel;
      UINT flags;                         // ROFLAG_ flags
	  int RtfInHdrFtr;
	  BOOL RtfInTable;
	  BOOL RtfInFrame;
	  BOOL wrtFrmSz;
	  CEDPage * page;
	  BOOL reset;
	  CEDParagraph* PrevPfmt;
	  CEDParagraph* frm;
	  CEDChar PrevChar;
	  char * oldFile;					//data from file we write
	  int oldFileLen;					//their length
      int * table;						//table of correspondence between fonts of old and new rtf
	  int maxFntNum;
      BOOL GroupBegin;                    // TRUE if the group begins
      BOOL GroupEnd;                      // TRUE if the group ends
      BOOL IsControlWord;                 // TRUE if control word
      long IntParam;                      // rtf control word parameter in integer numeric format
	  char CurWord[MAX_WIDTH+1];          // current word
      int  WordLen;                       // length of the current word
      int  TextIndex;                     // index if current BYTE in the 'text' buffer
      char CurChar;                       // last character read
      BOOL eof;                           // end of file
      char param[MAX_WIDTH/3];            // rtf control word parameter
      BYTE stack[MAX_WIDTH+1];            // characters returned from previous operations
      int  StackLen;                      // number of characters in the stack

	};

struct StrRtfFont {                       // font table in an rtf file
//      BOOL InUse;                         // TRUE when the slot is in use
      int  FontId;                        // font id
      char family[32];                    // font family
      char name[32];                      // font name
	  int  CharSet;		// Piter A
      };


#define RTF_FILE_INCOMPLETE 1
#define RTF_SYNTAX_ERROR    2
#define RTF_END_FILE        3

#define PARAM_NONE          0
#define PARAM_INT           1
#define PARAM_DBL           2

#define DEFAULT_CFMT       0x0
#define DEFAULT_PFMT		0x0

#define ROFLAG_IGNORE_TABLE   0x1      // ignore table information
#define ROFLAG_IGNORE_SLASH   0x2      // do not treat backslash as a special character
#define ROFLAG_IN_SUPSCR      0x4      // in superscript group
#define ROFLAG_IN_SUBSCR      0x8      // in subscript group

BOOL BeginRtfGroup(/*PTERWND w,*/struct StrRtfOut *rtf);
BOOL WriteRtfControl(/*PTERWND w,*/struct StrRtfOut *rtf, const char* control,int type, double val);
BOOL WriteRtfFont(struct StrRtfOut *rtf, BOOL head);
BOOL WriteRtfMargin(/*PTERWND w,*/struct StrRtfOut *rtf);
BOOL WriteRtfSection(/*PTERWND w,*/struct StrRtfOut *rtf, CEDSection* sect);
BOOL WriteRtfCharFmt(/*PTERWND w,*/struct StrRtfOut *rtf,CEDChar* curChar);
BOOL EndRtfGroup(/*PTERWND w,*/struct StrRtfOut *rtf);
BOOL WriteRtfParaFmt(/*PTERWND w,*/struct StrRtfOut *rtf,CEDParagraph* NewPfmt,CEDParagraph* PrevPfmt/*, int NewCell, int PrevCell, int NewFID, int PrevFID*/);
BOOL FlushRtfLine(/*PTERWND w,*/struct StrRtfOut *rtf);
BOOL PutRtfChar(/*PTERWND w,*/struct StrRtfOut *rtf,BYTE CurChar);
BOOL WriteRtfText(/*PTERWND w,*/struct StrRtfOut *rtf, const char* text, int TextLen);
BOOL WriteRtfRow(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* NewCell,CEDParagraph * prevRow);
BOOL WriteRtfCell(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* NewCell);

BOOL WriteRtfPara(struct StrRtfOut *rtf,CEDParagraph* p, BOOL brk);
BOOL WriteFrmPos(/*PTERWND w,*/struct StrRtfOut *rtf, CEDParagraph* frm, BOOL writeWidth);
BOOL WriteRtfDIB(/*PTERWND w,*/struct StrRtfOut *rtf,int pict);
BOOL PutRtfHexChar(/*PTERWND w,*/struct StrRtfOut *rtf,BYTE CurChar);
BOOL WriteRtfMetafile(/*PTERWND w,*/struct StrRtfOut *rtf,int pict);
BOOL WriteRtfMergedHeader(struct StrRtfOut *rtf, const char * name);
BOOL WriteRtfParaBorder(struct StrRtfOut *rtf, CEDParagraph * para);
BOOL WriteRtfColor(struct StrRtfOut *rtf,BOOL head = TRUE);


extern char logName[_MAX_PATH];
extern FILE *logStream;



// error code


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
