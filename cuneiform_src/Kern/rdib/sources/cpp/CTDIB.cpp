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

// CTDIB.cpp: implementation of the CTDIB class.
//
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include <stdlib.h>
#include "ctdib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDIB::CTDIB()
{
	hDIB = NULL;
	pDIB = NULL;
	DetachDIB();
	IsAvailable = FALSE;
	UnderConstruction = FALSE;
	wDirect = UnknownDirection;
	pExternalAlloc = NULL;
	pExternalFree = NULL;
	pExternalLock = NULL;
	pExternalUnlock = NULL;
	CreatedByMe = FALSE;

}

CTDIB::~CTDIB()
{
	DestroyDIB();
}

CTDIB::CTDIB(Handle hAtDIB)
{
	hDIB = NULL;
	pDIB = NULL;
	IsAvailable = FALSE;
	UnderConstruction = FALSE;
	wDirect = UnknownDirection;
	pExternalAlloc = NULL;
	pExternalFree = NULL;
	pExternalLock = NULL;
	pExternalUnlock = NULL;
	CreatedByMe = FALSE;
	DetachDIB();
	hDIB = hAtDIB;
}

Word32 CTDIB::GetActualColorNumber()
{
	Word32 RGBCount;
	PCTDIBBITMAPINFOHEADER pHeader;

	CTDIB_IFNODIB(0);
	pHeader = (PCTDIBBITMAPINFOHEADER)pDIBHeader;
	RGBCount = UsedColors( pHeader->biBitCount, pHeader->biClrUsed);
	return RGBCount;
}

Bool32 CTDIB::AttachDIB()
{
	PCTDIBBITMAPINFOHEADER pSimpleHead;
	if ( hDIB == NULL && pDIB == NULL )
	{
		return !DetachDIB();
	}


	if ( IsExternalsSets() && hDIB && !UnderConstruction )
		pDIB = pExternalLock(hDIB);
/*
#if defined (WIN32) & defined(CTDIB_USE_WIN32_API)
		pDIB = GlobalLock( hDIB );
#else
		pDIB = hDIB;
#endif
*/
	if ( pDIB )
	{	// bitmapinfoheader
		IsAvailable = TRUE;
		pSimpleHead = (PCTDIBBITMAPINFOHEADER) pDIB;
		wDirect = (pSimpleHead->biHeight > 0 ? BottomUp : TopDown );

		switch (pSimpleHead->biSize)
		{
		case CTDIB_VERSION_3_HEADER_SIZE:
			wVersion = WindowsVersion;
			break;
		case CTDIB_VERSION_4_HEADER_SIZE:
			wVersion = FourthVersion;
			break;
		case CTDIB_VERSION_5_HEADER_SIZE:
			wVersion = FifhtVersion;
			break;
		default:
			return !DetachDIB();
		}
		pDIBHeader =  pDIB;
		pRGBQuads  = (PCTDIBRGBQUAD)((PWord8)pDIB + pSimpleHead->biSize);
		pBitFild   = (PWord8)(pRGBQuads + GetActualColorNumber());
		IsAvailable = TRUE;
		wDirect = (pSimpleHead->biHeight > 0 ? BottomUp : TopDown );
		return TRUE;
	}
	return FALSE;
}

Bool32 CTDIB::DetachDIB()
{

	if ( hDIB )
	{
		hDIB = NULL;
	}

	pDIBHeader = NULL;
	pRGBQuads  = NULL;
	pBitFild   = NULL;
	pDIB       = NULL;
	IsAvailable = FALSE;
	CreatedByMe = FALSE;
	UnderConstruction = FALSE;
	wVersion = UnknownVersion;
	wDirect = UnknownDirection;
	return TRUE;
}

PVOID CTDIB::GetPtrToHeader()
{
	CTDIB_IFNODIB(NULL);
	return pDIBHeader;
}

PVOID CTDIB::GetPtrToRGB()
{
	CTDIB_IFNODIB(NULL);
	return (PVOID)pRGBQuads;
}

Word32 CTDIB::GetDIBVersion()
{
	CTDIB_IFNODIB(0);
	return wVersion;
}

Bool32 CTDIB::SetDIBbyPtr(PVOID ptr)
{
	if ( (hDIB && pDIB) || !ptr )
		return FALSE;

	pDIB = ptr;
	return AttachDIB();
}

Bool32 CTDIB::SetDIBbyHandle(Handle hnd)
{
	if ( hDIB || pDIB || !hnd )
		return FALSE;

	hDIB = hnd;
	return AttachDIB();
}

Bool32 CTDIB::IsDIBAvailable()
{
	return IsAvailable;
}
//////////////////  !
Int32 CTDIB::GetImageHeight()
{
	CTDIB_IFNODIB(0);

	return ((PCTDIBBITMAPINFOHEADER)pDIBHeader)->biHeight;
}

Word32 CTDIB::GetLinesNumber()
{
	CTDIB_IFNODIB(0);

	return abs(GetImageHeight());
}
//////////////////  !

Int32 CTDIB::GetImageWidth()
{
	CTDIB_IFNODIB(0);
	return ((PCTDIBBITMAPINFOHEADER)pDIBHeader)->biWidth;
}

Word32 CTDIB::GetLineWidth()
{
	CTDIB_IFNODIB(0);

	return abs(GetImageWidth());
}

Word32 CTDIB::GetLineWidthInBytes()
{
	CTDIB_IFNODIB(0);

	return DIB_BITS_TO_BYTES(GetLineWidth() * GetPixelSize());
}

Word32 CTDIB::GetUsedLineWidthInBytes()
{
	CTDIB_IFNODIB(0);
	return BITS_TO_BYTES(GetLineWidth() * GetPixelSize());
}

Word32 CTDIB::GetImageSize()
{
	CTDIB_IFNODIB(0);

	return GetLinesNumber() * GetLineWidth();
}

Word32 CTDIB::GetImageSizeInBytes()
{
	CTDIB_IFNODIB(0);

	return GetLineWidthInBytes() * GetLinesNumber();
}
/////////////////////////////////////////////////////////////////////
//
Word32 CTDIB::GetPixelSize()
{
	CTDIB_IFNODIB(0);
	return (Word32)(((PCTDIBBITMAPINFOHEADER)pDIBHeader)->biBitCount);
}
///////////////////////////////////////////////////////////////////////
//
Handle CTDIB::CreateDIBBegin(Int32 Width, Int32 Height, Word32 BitCount, Word32 UseColors, CTDIBVersion dVersion)
{
	#define DELETE_PHEADER(a)                if ( a )  delete a;

	PCTDIBBITMAPINFOHEADER pV3Header = NULL;
	PCTDIBBITMAPV4HEADER   pV4Header = NULL;
	PCTDIBBITMAPV5HEADER   pV5Header = NULL;
	PVOID                  pHeader = NULL;
	Word32 HeaderSize;
	Word32 FuelSize = 0;

	if ( !CTDIB_READYTOCREAT )
		return NULL;

	if ( IsDIBAvailable() )
		return NULL;

	switch(dVersion)
	{
	case WindowsVersion:
		HeaderSize = CTDIB_VERSION_3_HEADER_SIZE;
		pV3Header = new CTDIBBITMAPINFOHEADER;
		pV3Header->biSize          = CTDIB_VERSION_3_HEADER_SIZE;
		pV3Header->biWidth         = Width;
		pV3Header->biHeight        = Height;
		pV3Header->biPlanes        = CTDIB_DEFAULT_PLANES;
		pV3Header->biBitCount      = (Word16)BitCount;
		pV3Header->biCompression   = CTDIB_DEFAULT_COMPRESSION;
		pV3Header->biSizeImage     = DIB_BITS_TO_BYTES(Width * BitCount) * abs ( Height );
		pV3Header->biXPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV3Header->biYPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV3Header->biClrUsed       = (Word16)UseColors;
		pV3Header->biClrImportant  = CTDIB_DEFAULT_COLORSIMPORTANT;
		pHeader = (PVOID)pV3Header;
		break;
	case FourthVersion:
		HeaderSize = CTDIB_VERSION_4_HEADER_SIZE;
		pV4Header  = new CTDIBBITMAPV4HEADER;
		pV4Header->bV4Size          = CTDIB_VERSION_4_HEADER_SIZE;
		pV4Header->bV4Height        = Height;
		pV4Header->bV4Width         = Width;
		pV4Header->bV4Planes        = CTDIB_DEFAULT_PLANES;
		pV4Header->bV4BitCount      = (Word16)BitCount;
		pV4Header->bV4V4Compression = CTDIB_DEFAULT_COMPRESSION;
		pV4Header->bV4SizeImage     = DIB_BITS_TO_BYTES(Width * BitCount) * abs ( Height );
		pV4Header->bV4XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV4Header->bV4YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV4Header->bV4ClrUsed       = (Word16)UseColors;
		pV4Header->bV4ClrImportant  = CTDIB_DEFAULT_COLORSIMPORTANT;
		pHeader = (PVOID)pV4Header;
		break;
	case FifhtVersion:
		HeaderSize = CTDIB_VERSION_5_HEADER_SIZE;
		pV5Header  = new CTDIBBITMAPV5HEADER;
		pV5Header->bV5Size          = CTDIB_VERSION_5_HEADER_SIZE;
		pV5Header->bV5Height        = Height;
		pV5Header->bV5Width         = Width;
		pV5Header->bV5Planes        = CTDIB_DEFAULT_PLANES;
		pV5Header->bV5BitCount      = (Word16)BitCount;
		pV5Header->bV5Compression   = CTDIB_DEFAULT_COMPRESSION;
		pV5Header->bV5SizeImage     = DIB_BITS_TO_BYTES(Width * BitCount) * abs ( Height );
		pV5Header->bV5XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV5Header->bV5YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
		pV5Header->bV5ClrUsed       = (Word16)UseColors;
		pV5Header->bV5ClrImportant  = CTDIB_DEFAULT_COLORSIMPORTANT;
		pHeader = (PVOID)pV4Header;
		break;
	default:
		return NULL;
	}

	FuelSize += HeaderSize;
	FuelSize += UsedColors(BitCount, UseColors) * sizeof(CTDIBRGBQUAD);
	FuelSize += DIB_BITS_TO_BYTES(Width * BitCount) * abs ( Height );

	if ( !(hDIB = pExternalAlloc(FuelSize)) )
		return FALSE;

	if (!( pDIB = pExternalLock(hDIB)) )
	{
		pExternalFree(hDIB);
		return FALSE;
	}

	CreatedByMe = TRUE;
	memset(pDIB, 0x00, FuelSize);
	memcpy(pDIB, pHeader,HeaderSize);
	pDIBHeader = pDIB;

	DELETE_PHEADER(	pV3Header );
	DELETE_PHEADER( pV4Header );
	DELETE_PHEADER( pV5Header );

	IsAvailable = TRUE;
	UnderConstruction = TRUE;

	AttachDIB();

	return hDIB;
}

Bool32 CTDIB::SetExternals(PCTDIBMemAlloc pfAlloc, PCTDIBMemFree pfFree, PCTDIBMemLock pfLock, PCTDIBMemUnlock pfUnlock)
{
	if ( !pfAlloc && !pfFree && !pfLock && !pfUnlock )
		return FALSE;

	if ( (void *)pfAlloc == (void *)pfFree   ||
		 (void *)pfAlloc == (void *)pfLock   ||
		 (void *)pfAlloc == (void *)pfUnlock ||
		 (void *)pfFree  == (void *)pfLock   ||
		 (void *)pfFree  == (void *)pfUnlock   )
	{
		return FALSE;
	}

	if ( CTDIB_READYTOCREAT )
		return FALSE;

	if ( IsExternalsSets() )
		return false;

	if ( IsDIBAvailable() )
		return FALSE;

	pExternalAlloc   = pfAlloc;
	pExternalFree    = pfFree;
	pExternalLock    = pfLock;
	pExternalUnlock  = pfUnlock;
	return TRUE;
}

Word32 CTDIB::UsedColors(Word32 wBitCount, Word32 wClrUsed)
{
	Word32 RGBCount;

	if ( ( RGBCount = wClrUsed ) == 0)
	{
		switch( wBitCount )
		{
		case 0:
			RGBCount = 0;
			break;
			RGBCount = 0;
		case 1:
			RGBCount = 2;
			break;
		case 4:
			RGBCount = 16;
			break;
		case 8:
			RGBCount = 256;
			break;
		case 16:
		case 24:
		case 32:
			RGBCount = 0;
			break;
		default :
			RGBCount = 0;
		}
	}

	return RGBCount;
}

Bool32 CTDIB::SetResolutionDPI(Word32 X_Dpi, Word32 Y_Dpi)
{
	CTDIB_UNDECONST(FALSE);

	return SetResolutionDPM((Word32)CTDIB_DPI_TO_DPM(X_Dpi), (Word32)CTDIB_DPI_TO_DPM(Y_Dpi));
}

Bool32 CTDIB::SetResolutionDPM(Word32 X_Dpm, Word32 Y_Dpm)
{
	PCTDIBBITMAPINFOHEADER pH;

	CTDIB_UNDECONST(FALSE);

	pH = (PCTDIBBITMAPINFOHEADER)(pDIBHeader);
	pH->biXPelsPerMeter = X_Dpm;

	if ( Y_Dpm == 0 )
		pH->biYPelsPerMeter = pH->biXPelsPerMeter;
	else
		pH->biYPelsPerMeter = Y_Dpm;

	return TRUE;
}

Bool32 CTDIB::GetResolutionDPI(PWord32 pX_Dpi, PWord32 pY_Dpi)
{
	Word32 x_dpi;
	Word32 y_dpi;

	CTDIB_IFNODIB(FALSE);

	if ( pX_Dpi == NULL || !GetResolutionDPM(&x_dpi, &y_dpi) )
		return FALSE;


	*pX_Dpi = (Word32)CTDIB_DPM_TO_DPI(x_dpi);

	if ( pY_Dpi != NULL )
		*pY_Dpi = (Word32)CTDIB_DPM_TO_DPI(y_dpi);

	return TRUE;

}

Bool32 CTDIB::GetResolutionDPM(PWord32 pX_Dpm, PWord32 pY_Dpm)
{
	PCTDIBBITMAPINFOHEADER pH;

	CTDIB_IFNODIB(FALSE);

	if (pX_Dpm == NULL)
		return FALSE;

	pH = (PCTDIBBITMAPINFOHEADER)(pDIBHeader);
	*pX_Dpm = pH->biXPelsPerMeter;

	if ( pY_Dpm != NULL )
		*pY_Dpm = pH->biYPelsPerMeter;

	return TRUE;
}

Bool32 CTDIB::CreateDIBEnd()
{
	return !(UnderConstruction = FALSE);
}

PVOID CTDIB::GetPtrToLine(Word32 wLine)
{
	PWord8  pLine = NULL;

	CTDIB_IFNODIB(NULL);

	if ( wLine >= GetLinesNumber() )
		return NULL;

	pLine = pBitFild;

	if ( wDirect == BottomUp )
	{
		pLine += ((GetLinesNumber() - (wLine + 1)) * GetLineWidthInBytes());
		return (PVOID)pLine;
	}

	if ( wDirect == TopDown )
	{
		pLine += (wLine * GetLineWidthInBytes());
		return (PVOID)pLine;
	}

	return NULL;
}

PVOID CTDIB::GetPtrToPixel(Word32 wPixelX, Word32 wPixelY)
{
	PWord8  pLine = NULL;

	CTDIB_IFNODIB(NULL);

	if ( wPixelX >= GetLineWidth() )
		return NULL;

	pLine = (PWord8)GetPtrToLine(wPixelY);

	if ( !pLine )
		return NULL;

	pLine += ((wPixelX * GetPixelSize()) / 8);

	return pLine;
}


Bool32 CTDIB::ResetDIB()
{
	CTDIB_IFNODIB(FALSE);

	return DetachDIB();
}

Bool32 CTDIB::GetDIBHandle(PHandle phDIB)
{
	CTDIB_IFNODIB(FALSE);

	*phDIB = hDIB;
	return TRUE;
}

Bool32 CTDIB::GetDIBPtr(PPVOID ppDIB)
{
	CTDIB_IFNODIB(NULL);

	*ppDIB = pDIB;
	return TRUE;
}

Bool32 CTDIB::SetRGBQuad(Word32 wQuad, CTDIBRGBQUAD Quad)
{
	PCTDIBRGBQUAD pCurrentQuad;

	if ( pRGBQuads == NULL )
		return FALSE;
	//CTDIB_UNDECONST(FALSE);

	if ( wQuad > GetActualColorNumber() )
	{
		return FALSE;
	}

    pCurrentQuad = (PCTDIBRGBQUAD)GetPtrToRGB();
	pCurrentQuad += wQuad;

	*pCurrentQuad = Quad;

	return TRUE;
}

Bool32 CTDIB::DestroyDIB()
{
	Handle hMem;
	Bool32 FreeMem = FALSE;
	Bool32 bRet = TRUE;

	CTDIB_IFNODIB(FALSE);

	hMem = hDIB;
	FreeMem = CreatedByMe && IsExternalsSets();

	if ( CreatedByMe != IsExternalsSets() )
		bRet = FALSE;

	if ( FreeMem )
	{
		pExternalUnlock(hMem);
		pExternalFree(hMem);
	}

	DetachDIB();
	return bRet;
}

Bool32 CTDIB::SetDIBHandle(Handle hSetDIB)
{
	if ( IsDIBAvailable() || hDIB != NULL)
		return FALSE;

	hDIB = hSetDIB;
	return FALSE;
}


Word32 CTDIB::GetDIBSize()
{
	Word32 FuelSize = 0;

	CTDIB_IFNODIB(0);

	FuelSize += GetHeaderSize();
	FuelSize += GetRGBPalleteSize();
	FuelSize += GetImageSizeInBytes();

	return FuelSize;
}

Word32 CTDIB::GetHeaderSize()
{
	CTDIB_IFNODIB(0);

	return *((PWord32)(GetPtrToHeader()));
}

Word32 CTDIB::GetRGBPalleteSize()
{
	CTDIB_IFNODIB(0);

	return (GetActualColorNumber() * sizeof(CTDIBRGBQUAD));
}

Word32 CTDIB::GetPixelShiftInByte(Word32 dwX)
{
	CTDIB_IFNODIB(0);

	switch (GetPixelSize())
	{
	case 1 :
		return (dwX % 8);
		break;
	case 4 :
		if ( dwX % 2 == 0 )
			return 0;
		else
			return 4;
		break;
	default :
		return 0;
	}
}

Bool32 CTDIB::IsExternalsSets()
{
	return ( pExternalAlloc && pExternalFree && pExternalLock && pExternalUnlock ) != NULL ;
}

Bool32 CTDIB::GetRGBQuad(Word32 wQuad, PCTDIBRGBQUAD pQuad)
{
	PCTDIBRGBQUAD pCurrentQuad;

	if ( pRGBQuads == NULL )
		return FALSE;

	if ( wQuad > GetActualColorNumber() )
	{
		return FALSE;
	}

    pCurrentQuad = (PCTDIBRGBQUAD)GetPtrToRGB();
	pCurrentQuad += wQuad;

	*pQuad = *pCurrentQuad;

	return TRUE;

}

void * CTDIB::GetPtrToBitFild()
{
	CTDIB_IFNODIB(NULL);

	return pBitFild;
}

CTDIB::CTDIBVersion CTDIB::GetVersion()
{
	CTDIB_IFNODIB(UnknownVersion);
	return wVersion;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTDIB::SetFuelLineFromDIB(CTDIB * pSrcDIB, Word32 nSrcLine, Word32 nDscLine, Word32 wSrcX)
{
	PWord8 pSrcStart = (PWord8)pSrcDIB->GetPtrToPixel(wSrcX, nSrcLine);
	PWord8 pDscStart = (PWord8)this->GetPtrToLine(nDscLine);
	PWord8 pBuffer;
	Word32 wShift;

	if ( pSrcDIB == NULL )
		return FALSE;

	if ( pSrcStart == NULL || pDscStart == NULL )
		return FALSE;

	if ( pSrcDIB->GetLineWidth() < this->GetLineWidth() + wSrcX )
		return FALSE;

	if ( pSrcDIB->GetPixelSize() != this->GetPixelSize() )
		return FALSE;

	Word32 t = pSrcDIB->GetUsedLineWidthInBytes() > this->GetUsedLineWidthInBytes() + ((wSrcX * GetPixelSize()) / 8) ? 1 : 0;
	switch ( this->GetPixelSize() )
	{
	case 1:
	case 4:
//		pBuffer = new Word8[pSrcDIB->GetUsedLineWidthInBytes() + 1];
		pBuffer = new Word8[pSrcDIB->GetUsedLineWidthInBytes()];

		if ( pBuffer == NULL )
			return FALSE;

		memset(pBuffer, 0, pSrcDIB->GetUsedLineWidthInBytes());
//		memcpy ( pBuffer, pSrcStart, this->GetUsedLineWidthInBytes() + 1 );
		memcpy ( pBuffer, pSrcStart, this->GetUsedLineWidthInBytes() + t);

		wShift = pSrcDIB->GetPixelShiftInByte(wSrcX);

		if ( wShift )
		{
//			Word32 wByte  = this->GetUsedLineWidthInBytes() + 1;
			Word32 wByte  = this->GetUsedLineWidthInBytes() + t;
			PWord8 pwByte = pBuffer;
			Word8  wShiftic = 0;

//			while ( wByte-- )
			while ( wByte - t > 0)
			{
/*	Commemted by Andrey 11.04.2002
				wShiftic = *(pwByte + 1);
				*pwByte = *pwByte << wShift;
				wShiftic = wShiftic >> (8 - wShift);
				*pwByte |= wShiftic;
				pwByte++;
*/
				wByte--;
				if (wByte) wShiftic = *(pwByte + 1);

				*pwByte = *pwByte << wShift;
				if (wByte)
				{
					wShiftic = wShiftic >> (8 - wShift);
					*pwByte |= wShiftic;
					pwByte++;
				}
			}
		}
		memcpy( pDscStart, pBuffer, this->GetUsedLineWidthInBytes() );

		delete[] pBuffer;
		break;
	case 8:
	case 16:
	case 24:
	case 32:
		memcpy( pDscStart, pSrcStart, this->GetUsedLineWidthInBytes() );
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

Bool32 CTDIB::CopyPalleteFromDIB(CTDIB *pSrcDIB)
{
	CTDIBRGBQUAD Quad;
	Word32       wQuad;
	Word32       nColors = this->GetActualColorNumber();

	CTDIB_IFNODIB(FALSE);

	if ( pSrcDIB->GetActualColorNumber() != nColors )
	{
		return FALSE;
	}

	for ( wQuad = 0; wQuad < nColors; wQuad++)
	{
		if ( !(pSrcDIB->GetRGBQuad(wQuad, &Quad)) || !(this->SetRGBQuad(wQuad,Quad)) )
			return FALSE;
	}

	return TRUE;
}

Bool32 CTDIB::CopyDPIFromDIB(CTDIB *pSrcDIB)
{
	Word32 wX, wY;

	CTDIB_UNDECONST(FALSE);

	if ( !pSrcDIB )
		return FALSE;

	if ( pSrcDIB->GetResolutionDPM( &wX, &wY ) &&
		 this->SetResolutionDPM( wX, wY )         )
		 return TRUE;

	return FALSE;
}

Word32 CTDIB::GetWhitePixel()
{
	CTDIBRGBQUAD fQ;
	CTDIBRGBQUAD sQ;
	Word32 i;
	Word32 Color = 0;

	CTDIB_IFNODIB(0);


	switch (GetPixelSize())
	{
	case 1 :
		if ( GetRGBQuad(0, &fQ) && GetRGBQuad(1, &sQ) )
		{
			if ( IsFirstQUADEqualSecond(&fQ, &sQ ) )
				return 1;

			return FirstQUADLighterThenSecond( &sQ, &fQ );
		}
		return 0;
		break;
	case 4 :
	case 8 :
		if ( GetRGBQuad( 0, &fQ ) )
		{
			for ( i = 1; i < GetActualColorNumber(); i++ )
			{
				if ( GetRGBQuad(i, &sQ) )
					if ( FirstQUADLighterThenSecond( &sQ, &fQ ) )
					{
						Color = i;
						GetRGBQuad(i, &fQ);
					}

			}
		}
		return Color;
		break;
	case 16 :
		return 0x00007fff;
		break;
	case 24 :
		return 0x00ffffff;
		break;
	case 32 :
		return 0xffffffff;
		break;
	default :
		return 0;
	}

}

Word32 CTDIB::GetBlackPixel()
{
	CTDIBRGBQUAD fQ;
	CTDIBRGBQUAD sQ;
	Word32 i;
	Word32 Color = 0;

	CTDIB_IFNODIB(0);


	switch (GetPixelSize())
	{
	case 1 :
		if ( GetRGBQuad(0, &fQ) && GetRGBQuad(1, &sQ) )
			return FirstQUADLighterThenSecond( &fQ, &sQ );
		return 0;
		break;
	case 4 :
	case 8 :
		if ( GetRGBQuad( 0, &fQ ) )
		{
			for ( i = 1; i < GetActualColorNumber(); i++ )
			{
				if ( GetRGBQuad(i, &sQ) )
					if ( FirstQUADLighterThenSecond( &fQ, &sQ ) )
					{
						Color = i;
						GetRGBQuad(i, &fQ);
					}

			}
		}
		return Color;
		break;
	case 16 :
	case 24 :
	case 32 :
		return 0;
		break;
	default :
		return 0;
	}

}

Bool32 CTDIB::FirstQUADLighterThenSecond(PCTDIBRGBQUAD fQuad, PCTDIBRGBQUAD sQuad)
{
	return ( (fQuad->rgbBlue  * fQuad->rgbBlue  +
		      fQuad->rgbGreen * fQuad->rgbGreen +
			  fQuad->rgbRed   * fQuad->rgbRed)  >
			 (sQuad->rgbBlue  * sQuad->rgbBlue  +
			  sQuad->rgbGreen * sQuad->rgbGreen +
			  sQuad->rgbRed   * sQuad->rgbRed)  ?
			 TRUE : FALSE );
}

Bool32 CTDIB::IsFirstQUADEqualSecond(PCTDIBRGBQUAD fQuad, PCTDIBRGBQUAD sQuad)
{
	return ( fQuad->rgbBlue  == sQuad->rgbBlue &&
		     fQuad->rgbGreen == sQuad->rgbGreen &&
			 fQuad->rgbRed   == sQuad->rgbRed );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
