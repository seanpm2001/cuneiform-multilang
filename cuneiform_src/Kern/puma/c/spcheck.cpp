#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "spcheck.h"
#include "specprj.h"
#include "ligas.h"
#include "crling.h"


typedef struct _SPWord
{
	Bool32 is_latin;
	CSTR_rast begin, end;
	Word8 * text;
	int wlen;
} SPWord;


typedef SPWord * pSPWord;

typedef pSPWord SPWords[1024];


void copy_text(SPWord * word)
{
	word->text = (Word8 *) malloc((word->wlen + 1)*sizeof(Word8));
	CSTR_rast rast = word->begin;
	int i = 0;
	while ((i < word->wlen) && rast)
	{  
		if (rast->vers) 
		{
			word->text[i] = rast->vers->Alt[0].Code[0];
			i++;
		}
		rast=CSTR_GetNext(rast);
	}
	word->text[word->wlen] = 0;
}

Bool32 find_closest_local_match(SPWord * word, int pos, CSTR_rast start)
{
	CSTR_rast rast = start;
	int i = pos;
	while ((i < word->wlen) && rast)
	{
		if (rast->vers) 
		{
			if (rast->vers->lnAltCnt > 1)
			{
				for (int j = 1; j < rast->vers->lnAltCnt; j++)
				{
					word->text[i] = rast->vers->Alt[j].Code[0];
					Int32 poc;
					RLING_CheckWord((PInt8) word->text, &poc);
					if (poc)
					{
						memcpy((void *)&(rast->vers->Alt[0]), (void *)&(rast->vers->Alt[j]), sizeof(rast->vers->Alt[0]));
						return TRUE;
					}
					else
					{
						if(find_closest_local_match(word, i + 1, CSTR_GetNext(rast)))
							return TRUE;
					}
				}
				break;
			}
			i++;
		}
		rast=CSTR_GetNext(rast);
	}
	return FALSE;
}

int make_tokens(CSTR_line line, SPWord ** words)
{
	CSTR_rast       rast=CSTR_GetFirstRaster(line);
	CSTR_rast pre;
	bool wb = true;
	int wc = 0;
	for(rast = CSTR_GetNext(rast);rast;rast=CSTR_GetNext(rast))
	{
		if(wc > 1022) break;
		if (rast->vers) {
			if (strchr(",.():;!?\" «»%", rast->vers->Alt[0].Code[0]))
			{
				if(!wb) 
				{
					wb = true;
					words[wc-1]->end = pre;
					copy_text(words[wc-1]);
				}
			}  
			else
			{
				if (wb)
				{
					wb = false;
					words[wc] = new SPWord;
					words[wc]->begin = rast;
					if (rast->attr.language==LANG_RUSSIAN)
						words[wc]->is_latin = FALSE;  
					else
						words[wc]->is_latin = TRUE;
					words[wc]->wlen = 1;
					wc++;
				}
				else
				{
					words[wc-1]->wlen++;
				}
				pre = rast;
			}
		} 
	}
	if(!wb) 
	{
		words[wc-1]->end = pre;
		copy_text(words[wc-1]);
	}
	words[wc] = (SPWord *) 0;
	return wc;
}

void free_tokens(SPWord ** words)
{
	 for (int i = 0; words[i]; i++)
	 {
		if (words[i]->wlen > 0)
			free(words[i]->text);
		delete words[i];
	 }
}

Bool32 is_digit(SPWord * word) 
{
	for(int i = 0; i < word->wlen; i++)
		if ((word->text[i] < '0')||(word->text[i] > '9'))
			return FALSE;
		return TRUE;
}

Bool32 fine_check(SPWord * word)
{
	if ((word->wlen == 1) && strchr("dljmtsnDLJMTSN", word->text[0]))
			 return TRUE;
	return FALSE;
}

Bool32 all_words_local(SPWord ** words)
{
	 for (int i = 0; words[i]; i++)
	 {
		Int32 poc;
		RLING_CheckWord((PInt8) words[i]->text, &poc);
		if (!poc)
		{
			if(fine_check(words[i])|| is_digit(words[i]))
				continue;			
			if (find_closest_local_match(words[i], 0, words[i]->begin))
				continue;
			return FALSE;
		}
	 }
	 return TRUE;
}

void replace_line(CSTR_line dest, CSTR_line src)
{
	CSTR_rast beg = CSTR_GetFirstRaster(dest);
	CSTR_rast end = CSTR_GetLastRaster(dest);
	CSTR_rast r = CSTR_GetNext(beg), rn;
	while ((r != end) && r)
		r = CSTR_DelRaster(r);
	rn = beg;
	CSTR_rast beg2 = CSTR_GetFirstRaster(src);
	CSTR_rast end2 = CSTR_GetLastRaster(src);
	r = CSTR_GetNext(beg2);
	while((r!=end2)&&r)
	{
		rn = CSTR_InsertRaster(rn);
		CSTR_CopyRaster(rn, r);
		r = CSTR_GetNext(r);
	}
}




Bool32 load_dicts(int second_lang)
{
	char * lp = GetModulePath();
	if (!RLING_IsDictonaryAvailable(second_lang, (PInt8)lp))
	{
		 return FALSE;
	}
	if (!RLING_IsDictonaryAvailable(LANG_RUSSIAN, (PInt8)lp)) 
	{
		 return FALSE;
	}
	RLING_UnloadDictonary();
	RLING_UnloadSecDictonary();
	return RLING_LoadDictonary(second_lang, (PInt8)lp) && RLING_LoadSecDictonary(LANG_RUSSIAN, (PInt8)lp);
	return FALSE;
}

Bool32 replace_word(SPWord * dest, SPWord * src)
{
	Bool32 res = CSTR_ReplaceWord(&(dest->begin), &(dest->end), src->begin, src->end);
	if (dest->wlen > 0)
		free(dest->text);
	copy_text(dest);
	return res;
}

Bool32 replace_russian_if_unknown(SPWord * russian, SPWord * local)
{
	int poc;
	RLING_CheckSecWord((PInt8) russian->text, &poc) ;
	if((!poc)&&(!is_digit(russian))) 
	{
		RLING_CheckWord((PInt8) local->text, &poc);
		if(poc)
		{  
			return replace_word(russian, local);
		}
		else
		{
			//if ((russian->wlen == 1) && fine_check(local))
			//	return replace_word(russian, local);
			if (find_closest_local_match(local, 0, local->begin))
				return replace_word(russian, local);
		}
	}
	return TRUE;
}

Bool32 replace_local_if_unknown(SPWord * local, SPWord * russian)
{
	int poc;
	RLING_CheckWord((PInt8) local->text, &poc) ;
	if(!poc) 
	{
		RLING_CheckSecWord((PInt8) russian->text, &poc);
		if(poc)
		{  
			return replace_word(local, russian);
		}
	}
	return TRUE;
}


Bool32 match_lines(SPWord ** dest, SPWord ** src)
{
	int destnw, srcnw;
	for (destnw = 0; dest[destnw]; destnw++);
	for (srcnw = 0; src[srcnw]; srcnw++);
	int count = destnw < srcnw ? destnw : srcnw;
	int b;
	for (b = 0; b < count; b++)
	{
		if(abs(dest[b]->wlen - src[b]->wlen) <= 2)
		{
			if (!replace_russian_if_unknown(dest[b], src[b]))
				  return FALSE;
		}
		else break;
	}
	for (int i = 1; i < (count - b); i++)
	{
		if(abs(dest[destnw - i]->wlen - src[srcnw - i]->wlen) <= 2)
		{
			if(!replace_russian_if_unknown(dest[destnw - i], src[srcnw - i]))
				return FALSE;
		} 
		else break;
	}
	return TRUE;
}
 
void mix_lines(CSTR_line ruseng, CSTR_line local, CSTR_line rus)
{
	if(!load_dicts(gnSecondLanguage))
		 printf("WARNING: Dicitionaries not loaded\n");
	SPWords rewords, lwords, rwords;
	int recount, lcount, rcount;
	recount = make_tokens(ruseng, rewords);
	
	lcount = make_tokens(local, lwords);
	rcount = make_tokens(rus, rwords);
	if (all_words_local(lwords))
	{
		replace_line(ruseng, local);
		CSTR_attr       lattr;
		CSTR_GetLineAttr(ruseng,&lattr);
		lattr.language = gnSecondLanguage;
		CSTR_SetLineAttr(ruseng,&lattr);
		free_tokens(rewords);
		free_tokens(lwords);
		free_tokens(rwords);
		return;
	}
	if ((recount == lcount) && lcount)
	{
		for(int i = 0; i < recount; i++)
		{
			if(rewords[i]->is_latin)
			{
				if (!replace_word(rewords[i], lwords[i]))
				{
					free_tokens(rewords);
					recount = make_tokens(ruseng, rewords);
					break;
				}
			}
		}
		for(int i = 0; i < recount; i++)
		{
			if(!rewords[i]->is_latin)
			{
				
				if(!replace_russian_if_unknown(rewords[i], lwords[i])) 
				{
					free_tokens(rewords);
					recount = make_tokens(ruseng, rewords);
					break;
				}
			} 
		}

	} 
	else
		if(!match_lines(rewords, lwords))
		{
			free_tokens(rewords);
			recount = make_tokens(ruseng, rewords);
		}
	if ((recount == rcount) && rcount)
	for(int i = 0; i < recount; i++)
		if(rewords[i]->is_latin)
		{
			if(!replace_local_if_unknown(rewords[i], rwords[i]))
			{
				free_tokens(rewords);
				recount = make_tokens(ruseng, rewords);
				break;
			}
		} 
	free_tokens(rewords);
	free_tokens(rwords);
	free_tokens(lwords);
}

/*
int count_words(CSTR_line lin) 
{
    CSTR_rast       rast=CSTR_GetFirstRaster(lin);
	bool wb = true;
	int wc = 0;
    for(rast = CSTR_GetNext(rast);rast;rast=CSTR_GetNext(rast))
	{
		if (rast->vers) {
			if (rast->vers->Alt[0].Code[0] == ' ')
				wb = true;
			else
				if (wb)
				{
					wb = false;
					wc++;
				}
		} else return 0;
	}
	return wc;
}

CSTR_rast goto_next_start(CSTR_rast rast) 
{
    bool bp = false;
	while (rast)
	{
		if (rast->vers->Alt[0].Code[0] == ' ')
			bp = true;
		else
			if (bp)
				return rast;
		rast=CSTR_GetNext(rast);
	}
	return (CSTR_rast)0;
}

void mix_lines(CSTR_line ruseng, CSTR_line local)
{
	int countre = count_words(ruseng);
	if ((countre != count_words(local))||(!countre))
		return;
    CSTR_rast    rastre=CSTR_GetFirstRaster(ruseng);
	CSTR_rast    rastrl=CSTR_GetFirstRaster(local);
	rastre = CSTR_GetNext(rastre);
	rastrl = CSTR_GetNext(rastrl);
	while (rastre && rastrl)
	{
		if (rastre->vers->Alt[0].Code[0] == ' ') 
		{
			rastre = goto_next_start(rastre);
			continue;
		}
		if (rastrl->vers->Alt[0].Code[0] == ' ')
		{
			rastrl = goto_next_start(rastrl);
			continue;
		}
		if (rastre->attr.language==LANG_RUSSIAN)
		{
			rastre = goto_next_start(rastre);
			rastrl = goto_next_start(rastrl);
			continue;
		}
		memcpy((void *)&(rastre->vers->Alt[0]), (void *)&(rastrl->vers->Alt[0]), sizeof(rastrl->vers->Alt[0]));
		rastre = CSTR_GetNext(rastre);
		rastrl = CSTR_GetNext(rastrl);
	}
}


*/