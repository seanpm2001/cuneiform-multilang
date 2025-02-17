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

#include "lnslang.h"
	//#   define DEBUG_SBAMBUK

   #include <fcntl.h>

	#ifndef __SBAMBUK_H
	#	include "sbambuk.h"
	#endif
   #ifndef __BLCKWHTE_H
   #	include "blckwhte.h"
   #endif


   #include "hliner.h"

   #include "smooth.h"
   static int _smooth_height=0;

	static Word8 bytebit0[8] = {
		0x7F, 0xBF,	0xDF,	0xEF,
		0xF7,	0xFB,	0xFD,	0xFE
	};



   /* VERTICAL FILTRATION+SEGMENT EXTRACTION PART...*******************/
   //   static Word16 cur_row_ = 0;

   // static TBlackSeg far * v_segments_;
   //   static TBlackSeg far * cur_seg_;

   //   static BHandle new_segment_ = NULLBHandle;
   //   static TSegBambuk far * vsb_       = NULL;

	#ifndef NDEBUG_SBAMBUK
		static int started_ = 0;
	#endif

	static int width_dword_ = 0;
	static int width_byte_ = 0;

#ifdef __TGREADER_H
THVSegBambuk::THVSegBambuk( TigerReader  & reader )
 {
   errCode = ER_NONE;
   assert( reader.isOk() );


   /* set frame */
//   left   = reader.wAddX;
   left   = 0;
   right  = left + reader.wImageWidth -1;
//   bottom = reader.wAddY;
   bottom = 0;
   top    = bottom + reader.wImageHeight-1;

   xres   = reader.wResolutionX;
   yres   = reader.wResolutionY;

   width_dword_ =  (width() + 31)/32 ;
   width_byte_ =  (width() + 7)/8;

   TFltBuf filter( width_dword_ );
   if (filter.isOk())
   {
      if (vBambuk.createBambuk( MAX_V_SEGMENTS_COUNT, width() )){
         TBlackSeg zero_seg = { 0x3333, 0x2222, NULLBHandle, NULLBHandle };
         vBambuk.cleanMembers( &zero_seg, 0 );
         if(hBambuk.createBambuk( MAX_H_SEGMENTS_COUNT, height() )){
            hBambuk.cleanMembers( &zero_seg, 0 );

            makeIt( &reader, &filter );
            assert( errCode == ER_NONE );
         }
         else { errCode = BERROR( ER_NOMEMORY ); };
      }
      else {   errCode = BERROR( ER_NOMEMORY ); };
      filter.destroy();
   };
	/******
   if (errCode == ER_NONE)
		if (!hBambuk.shrinkBambuk( hBambuk.segCount ))
         {  errCode = BERROR( ER_NOMEMORY ); };

   if (errCode == ER_NONE)
		if (!vBambuk.shrinkBambuk( vBambuk.segCount ))
         {  errCode = BERROR( ER_NOMEMORY ); };
	******/

 }

Bool THVSegBambuk::makeIt(   TigerReader * reader,
                             TFltBuf* filter )
	{
		assert( this );
		assert( reader );
		assert( reader->isOk() );
		assert( filter );
		assert( filter->isOk() );

      Word8*   new_line = NULL;
		int 		cur_row = 0;
//      int width_byte = reader->lineWidthByte(); assert(width_byte > 0);

		hBambuk.hInitProcess( left, right, 16 );

		if (height() < 17) {
			for (cur_row = bottom; cur_row <= top; cur_row++) {
            new_line = (Word8*)(reader->getLine());
            if (new_line == NULL)
            {
               errCode = ER_CANTREAD;
					return WRONG();
				};

            if (!hBambuk.hUpdate( new_line ))
            {
               errCode = ER_NOMEMORY;
					return WRONG();
				};
			};
			return TRUE;
		};

   //      vsb_ = &( vBambuk );
   //      v_segments_ = &((*vsb_)[0]);
		assert( hBambuk.memberTotalCount() == 0 );
		assert( vBambuk.memberTotalCount() == 0 );

		/* process first 16 lines - extract horisontal and fill buffer*/
		for (cur_row = bottom; cur_row < bottom + 16; cur_row++)
      {
         new_line = (Word8*)(reader->getLine());
         if (new_line == NULL)
         {
            errCode = ER_CANTREAD;
				return WRONG();
			};
         HLiner_AddImageLine(new_line);
         if (!hBambuk.hUpdate( new_line )) { errCode = ER_NOMEMORY;
				return WRONG();
			};
			filter->addLine( new_line );
		};

		/* now prepear filter to work */
		filter->preBuffer();

		// upper bound for h segments
		#ifndef NDEBUG_SBAMBUK
			started_ = 0;
		#endif
      new_line = (Word8*)(reader->getLine());
      if (new_line == NULL)
      {
         errCode = ER_CANTREAD;
			return WRONG();
		};
      HLiner_AddImageLine(new_line);

      if (!hBambuk.hUpdate( new_line )) {    errCode = ER_NOMEMORY;
			return WRONG();
		};

/*
      if (!vBambuk.vUpdateFirst(    (Int32*)new_line,
                                    (Int32*)(filter->cur16),
												16
										  )
			) {
         errCode = ER_NOMEMORY;
			return WRONG();
		};
*/
      Int32 white_line[1024]; memset(white_line, 0xff, sizeof(white_line));
      if (!vBambuk.vUpdate(    (Int32*)new_line,
                               (Int32*)(filter->cur16),
                               white_line,
								       16
								  )
			) {
         errCode = ER_NOMEMORY;
			return WRONG();
		};

		filter->updateByLine( new_line );

#ifdef SMOOTH98
      smooth_start(width_dword_);
      _smooth_height = smooth_get_height();
#endif
		/* main loop */
		for (cur_row = bottom + 17; cur_row <= top  ; cur_row++)
      {
         new_line = (Word8*)(reader->getLine());
         if (new_line == NULL)
         {
            errCode = ER_CANTREAD;
				return WRONG();
			};
         HLiner_AddImageLine(new_line);

         if (!hBambuk.hUpdate( new_line ))
         {
            errCode = ER_NOMEMORY;
				return WRONG();
			};
#ifdef SMOOTH98
         new_line = (Word8*)smooth_update((Int32*)new_line);
#endif
         if (!vBambuk.vUpdate( (Int32*)new_line,
                               (Int32*)filter->cur16,
                               (Int32*)filter->last16,
										 cur_row
									  )
            ){                               errCode = ER_NOMEMORY;
				return WRONG();
			}
			filter->updateByLine( new_line );
   	};

#ifdef SMOOTH98
      smooth_finish();
#endif
         if (!vBambuk.vUpdate( white_line,
                               (Int32*)filter->cur16,
                               (Int32*)filter->last16,
										 cur_row
									  )
            ){                               errCode = ER_NOMEMORY;
				return WRONG();
			}

/*
      if (!vBambuk.vUpdateLast(  (Int32*)(filter->cur16),
                                 (Int32*)(filter->last16),
											top+1
								 		)
         ){                               errCode = ER_NOMEMORY;
			return WRONG();       // close vertical segments touching down bound
		};
*/
		#ifndef NDEBUG_SBAMBUK
			assert( started_ == 0 );
		#endif
		return TRUE;
	}

#endif // __TGREADER_H

/**********************************************************************/
Bool TSegBambuk::vUpdate(  Int32* new_line,
                           Int32* cur16,
                           Int32* prev16,
									int cur_row
								)
{  assert( new_line != NULL );
	assert( cur16 != NULL );
	assert( prev16 != NULL );
	BHandle seg_handle;

   int j, col;

   int nbytes = width_byte_-1;

   Word8* cur_byte = (Word8*)cur16;
   Word8* prev_byte = (Word8*)prev16;
   Word8* new_byte = (Word8*)new_line;

	for ( int i = 0; i < nbytes; i++ )
   {
		/* 1. process started segments */
      Word8 was_born = (*prev_byte) & ~(*cur_byte);
      if (was_born)
      {
			col = i << 3; // i*8;
			do {
					segCount++;
					if (	(seg_handle = addMember( col+
											(j = FIRST_BIT_IN_BYTE( was_born ))
														)
							) == NULLBHandle
						)
                  return WRONG();
					THE[ seg_handle ].left = cur_row - 16;
					#ifndef NDEBUG_SBAMBUK
						started_++;
						THE[ seg_handle ].right = 0;
					#endif
			} while ( was_born &= bytebit0[j]);
      }

		/* 2. process ended segments */
      Word8 has_died = ~(*cur_byte) & (*new_byte);
   	if (has_died)
      {
			col = i << 3;   // i*8
			do {
					seg_handle = lastEntryMember( col+
										(j = FIRST_BIT_IN_BYTE( has_died ))
														);
					#ifndef NDEBUG_SBAMBUK
						assert( started_ > 0 );
						started_--;
						assert( seg_handle != NULLBHandle );
						assert(THE[ seg_handle ].right == 0);
					#endif
#ifdef SMOOTH98
					THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
					THE[ seg_handle ].right = cur_row-1;
#endif
			} while ( has_died &= bytebit0[j]);
		};

		new_byte++;
		cur_byte++;
		prev_byte++;
	}; // for
	return TRUE;
} // vUpdate()

Bool TSegBambuk::vUpdateFirst(   Int32* new_line,
                                 Int32* cur16,
											int cur_row
								)
	{  assert( new_line != NULL );
		assert( cur16 != NULL );
		BHandle seg_handle;

      Int32 was_born_dword;
      Int32 has_died_dword;
		Word8 was_born, has_died;
		int j, col;

		for ( int i = 0; i < width_dword_ - 1; i++ ){
			/* process started segments */
			if ( ( was_born_dword = (~(*cur16)) ) != 0){      // 1(W) at prev; 0(B) at cur;
#ifdef BIG_ENDIAN
            was_born_dword = SWAPLONG(was_born_dword);
#endif
				if ( (was_born = (Word8)was_born_dword ) != 0){  // first byte
					col = i << 5;   // i*32
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ( (was_born = (Word8)(was_born_dword >> 8) ) != 0){  // second byte
					col = (i << 5) + 8;   // i*32 + 8
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ( (was_born = (Word8)(was_born_dword >> 16) ) != 0){  // third byte
					col = (i << 5) + 16;   // i*32 + 8
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ( (was_born = (Word8)(was_born_dword >> 24) ) != 0){  // forth byte
					col = (i << 5) + 24;   // i*32 + 24
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
			};

			/* process ended segments */
			if ( (has_died_dword = (~(*cur16) & (*new_line)) ) != 0 ){   // 0(B) at cur; 1(W) at new_line;
#ifdef BIG_ENDIAN
            has_died_dword= SWAPLONG(has_died_dword);
#endif
				if ((has_died = (Word8)has_died_dword ) != 0){   // first byte
					col = i << 5;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ((has_died = (Word8)(has_died_dword>>8) ) != 0){   // second byte
					col = (i << 5) + 8;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ( (has_died = (Word8)(has_died_dword>>16) ) != 0){   // third byte
					col = (i << 5) + 16;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ( (has_died = (Word8)(has_died_dword>>24) ) != 0){   // forth byte
					col = (i << 5) + 24;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
			};
			/**************************/
			new_line++;
			cur16++;
		}; // for
		return TRUE;
	} // vUpdateFirst()

Bool TSegBambuk::vUpdateLast(    Int32* cur16,
                                 Int32* last16,
											int cur_row
									 )
	{
		assert( cur16 != NULL );
		assert( last16 != NULL );
		BHandle seg_handle;

      Int32 was_born_dword;
      Int32 has_died_dword;
		Word8 was_born, has_died;
		int j, col;

		for ( int i = 0; i < width_dword_ - 1; i++ ){
			/* process started segments */
			if ( (was_born_dword = ((*last16) & ~(*cur16)) ) != 0 ){      // 1(W) at prev; 0(B) at cur;
#ifdef BIG_ENDIAN
            was_born_dword = SWAPLONG(was_born_dword);
#endif
				if ((was_born = (Word8)was_born_dword) != 0){  // first byte
					col = i << 5;   // i*32
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ((was_born = (Word8)(was_born_dword >> 8)) != 0){  // second byte
					col = (i << 5) + 8;   // i*32 + 8
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ( (was_born = (Word8)(was_born_dword >> 16) ) != 0){  // third byte
					col = (i << 5) + 16;   // i*32 + 8
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
				if ( (was_born = (Word8)(was_born_dword >> 24)) != 0){  // forth byte
					col = (i << 5) + 24;   // i*32 + 24
					do {
							segCount++;
							if (	(seg_handle = addMember( col+
													(j = FIRST_BIT_IN_BYTE( was_born ))
															  	)
									) == NULLBHandle
								)	return WRONG();
							THE[ seg_handle ].left = cur_row - 16;
							#ifndef NDEBUG_SBAMBUK
								started_++;
								THE[ seg_handle ].right = 0;
							#endif
					} while ( was_born &= bytebit0[j]);
				};
			};

			/* process ended segments */
			if ( ( has_died_dword = ~(*cur16) ) != 0){   // 0(B) at cur; 1(W) at new_line;
#ifdef BIG_ENDIAN
            has_died_dword= SWAPLONG(has_died_dword);
#endif
				if ( (has_died = (Word8)has_died_dword ) != 0){   // first byte
					col = i << 5;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ( ( has_died = (Word8)(has_died_dword>>8) ) != 0){   // second byte
					col = (i << 5) + 8;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ( ( has_died = (Word8)(has_died_dword>>16) ) != 0){   // third byte
					col = (i << 5) + 16;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
				if ( ( has_died = (Word8)(has_died_dword>>24) ) != 0){   // forth byte
					col = (i << 5) + 24;   // i*32
					do {
							seg_handle = lastEntryMember( col+
												(j = FIRST_BIT_IN_BYTE( has_died ))
															 	);
							#ifndef NDEBUG_SBAMBUK
								assert( started_ > 0 );
								started_--;
								assert( seg_handle != NULLBHandle );
								assert(THE[ seg_handle ].right == 0);
							#endif
#ifdef SMOOTH98
							THE[ seg_handle ].right = cur_row - _smooth_height - 1;
#else
							THE[ seg_handle ].right = cur_row-1;
#endif
					} while ( has_died &= bytebit0[j]);
				};
			};
			/**************************/
			last16++;
			cur16++;
		}; // for
		return TRUE;
	} // vUpdateLast()
