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


/*
   spellout.c - ed-file modification procedures of spelling checker
*/
  #include "tuner.h"
  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include <stdlib.h>
  #include <ctype.h>

  #include "speldefs.h"
  #include "spelfunc.h"
  #include "lang.h"

  //////////////////////
  //AK change : 27.01.99
  #include "tigeremulate.h"
  void	ErrorExit(int Code);
  // end AK

#ifdef SECOND_PASS
  RSTAT    rst[RST_BUFF_SIZE];
  INT      rst_last=0;
  BYTE     ast[MAX_ARTS];
#endif

extern dQ SPQ;                        //Q;                        /* ED-file access common structure */
#ifdef ART
extern struct artstr artbase[24];
   /* base of artificial changes      */
#endif
#ifdef  RUS_ENG_LANG
extern BYTE multy_language ;
#endif
/*=================================================================== */
/*                    Local  functions prototypes                     */
/*=================================================================== */
  static INT outpos_repl (SOBJ * obj,INT pos, BYTE cnew);
  static INT outpos_repl_bl (SOBJ * obj, SPART * part,INT pos, INT anew);
  static INT outpart (SOBJ * obj, SPART * part);
  static INT outpos_ins_bl (SOBJ * obj, SPART *part, INT pos, INT anew);
  static INT outpos_ins (SOBJ * obj,INT pos, BYTE cnew);
  static INT outpos_del (SOBJ * obj, INT pos);
  static INT outpos_context ( SPART * part, INT pos, BYTE * c);
  static INT  getpos_bel (SOBJ * obj, INT pos, LT  ** beg,
                          LT  ** end, INT * lth );
  static INT corrpos_lt (SOBJ * obj, INT pos, LONG lth);
  static INT shift_left(INT v_s,struct segm * cur_segm,
                          CHAR * cur_symb);
  static INT outpos_ins_shift (SOBJ * obj, INT pos, BYTE cnew);

#ifdef SECOND_PASS
  static void collect_repl_stat(SOBJ *,INT npos,INT nalt);
#endif


/***********************************************************************/
/*  ................
								       */
/***********************************************************************/
CHAR * find_byte_flag()
{
 struct segm * sgm;
 CHAR * b_f;
 sgm= SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].first_segm;
 b_f=sgm->string;
 while(*b_f!=SS_SHEET_DESCR)
  {
   b_f=next_symb(NO,YES,YES,sgm,b_f);
   if(b_f==NULL)
    {
#ifdef SYSPR_ERROR
     PRINTF("Dont make such a production,wrong file");
#endif
      ErrorExit(RLING_ERROR_WRONG_ED_FILE);
    }
  }
 SPQ.h_sheet_disk_descr_ptr=(struct sheet_disk_descr *)b_f;
 b_f=&SPQ.h_sheet_disk_descr_ptr->byte_flag;
 return(b_f);
}

/***********************************************************************/
/* This procedure outputs object to ED-file via
   output of its parts-constituents.
   The best part-chain is specified by obj->opt_part[]
								       */
/***********************************************************************/
INT outobj (SOBJ * obj, SPART * part)

{
 INT beg, ei, pi, pos;

 beg = 0;                             /* previous valuable pos              */
 for (ei=1; ei<=obj->pos_part_nmb; ei++) /* scan best parts                 */
  {
   if (!(obj->opt_part[ei]))          /* no best part delimited here        */
    continue;                         /* no new end-beg continue            */
   pos = obj->opt_part[ei];           /* pos-delimiter                      */


   if (findpart (obj, part, beg, obj->opt_part[ei], &pi)) /* find part      */
      {
       if (obj->pos[pos].type_sp & (T_SP1|T_SP2)) /* spsp1 or spsp2-pos ?       */
        // 02-25-94 01:33pm, Mike
        //if(ei==obj->pos_part_nmb)
        //  outpos_repl (obj, pos, ' ');        /* sure parts' delimiter      */
        //else
        if(ei!=obj->pos_part_nmb)
	 {
          INT dummy;
	  for(dummy=ei+1;dummy<=obj->pos_part_nmb;dummy++)
	    if(obj->opt_part[dummy]) break;
	  if(
	  findpart(obj,part,obj->opt_part[ei],obj->opt_part[dummy],&dummy)
	   ||((obj->opt_part[ei]-beg-((beg)?1:0))>=MINlth_for_space))
	     outpos_repl (obj, pos, ' ');        /* sure parts' delimiter      */
	 }
	outpart (obj,&(part[pi]));  /* output ed-file changes within a part*/
      }
   else
     {
       if (obj->pos[pos].type_sp & (T_SP1|T_SP2)) /* spsp1 or spsp2-pos ?       */
         // 02-25-94 01:33pm, Mike
         //if(ei==obj->pos_part_nmb)
         //   outpos_repl (obj, pos, ' ');   /* sure parts' delimiter      */
         //else
         if(ei!=obj->pos_part_nmb)
	  {
           INT dummy;
           INT dm;
	   for(dummy=ei+1;dummy<=obj->pos_part_nmb;dummy++)
	     if(obj->opt_part[dummy]) break;
	   if(
	     findpart(obj,part,obj->opt_part[ei],obj->opt_part[dummy],&dm)
	     &&((obj->opt_part[dummy]-obj->opt_part[ei]-1)>=MINlth_for_space))
	       outpos_repl (obj, pos, ' ');        /* sure parts' delimiter      */
	  }
      setpart_blue(obj,beg,obj->opt_part[ei]);
     }
   beg = obj->opt_part[ei];          /* end --> beg of next part            */
  }

 return (Ok);                       /* output successfully finished */

}

/***********************************************************************/
/* This procedure outputs a part of obj to ED-file.
   All changes of ED-file within the part are being considered and done
								       */
/***********************************************************************/
 INT outpart (SOBJ * obj, SPART * part)

{
 INT pa;       /* part->pos[pa] part-pos-numbers                           */
 INT pa_x=0;   /* displacement of pa for words: part->word->altn[pa+pa_x]  */
 INT pi,       /* curr obj-pos[pi]                                         */
     ai,       /* curr obj->pos[pi]->alt[ai]                               */
     arti;     /* current art-change-id                                    */

 if (                                          /* if all alts are original */
      (!(part->word->type_art & (T_ALTS|T_REPL|T_GC|T_BRK))) /* and no arts      */
    &&(!(part->type_sp  & (T_SP1|T_SP2|T_BLANK))) /* and no spsp or blanks */
    )
  {
#ifdef EDPR_NOTCHANGE
#ifdef WORDPR
 prtwrd(obj,part->word,part->word->mark,0);
#endif
   PRINTF ("+ + +SPELLOUT: part not changed");
#endif
   goto Outpart_ok;                         /* the whole part not changed */
  }
#ifdef WORDPR
#ifdef EDPR_CORRWORD
 prtwrd(obj,part->word,part->word->mark,0);
#endif
#endif

 for (pa=0, pi= part->beg;  pi<part->end;  pi++)
  {
   if (obj->pos[pi].type_sp & T_HYPHEN)            /* hyphen ?             */
    goto Outpart_skip;				   /* Yes, skip it         */
   if (obj->pos[pi].type_sp & (T_SP1|T_SP2))       /* spsp1 or spsp2-pos ? */
    {
     if (pi==part->beg)                            /* 1-st part pos ?      */
/*   outpos_repl (obj, pi, ' '); */           /* sure parts' delimiter*/
       goto Outpart_skip;               /* already replaced by ' ' (outobj) */
     else                                          /* No - delete inter-sp */
      if(part ->word ->  type & (T_LOW|T_CAP))
       {
         if ( part ->word -> voc_kind >= 4)  goto Outpart_skip;
         if(
            ((pi) && ( obj->pos[pi-1].alt[0].type & T_DELIM))   ||
            ((pi+1 != part ->end)&& (obj->pos[pi+1].alt[0].type & T_DELIM))
           )  goto Outpart_skip;
         else
          goto Outpart_del;                            /* delete it            */
       }
      else
	goto Outpart_skip;
    }
				/* No hyphens & spsp, process regular symb:*/
   arti = part->art[pa+pa_x]; /* + pa_x --- Lepik ( crash in crsh39) 10/29/92 08:53pm
                                 id of artificial change                 */
   if (!(arti))                 /* ordinary (not artificial) change ?      */
    goto Outpart_chkact;        /* Yes, only one-to-one change may occure  */
#ifdef ART
				/* Artificial change takes place:          */
				/* --------------------------------------- */
#ifdef SECOND_PASS
   ast[arti]++;
#endif

   ai = part->word->altn[pa+pa_x]; /* alt ind for 1-st pos of art-change   */
   if (artbase[arti].objts1 & T_BLANK)/* repl by blank=> alt from spec pos */
    outpos_repl_bl (obj, part, pi, ai);
   else                         /* not blank => repl from artbase          */
    outpos_repl (obj, pi, artbase[arti].obj1);
   if (!(artbase[arti].sr2))    /* two-pos source ?                        */
    {                           /* No: 1=>1 or 1=>2 is possible            */
     if (artbase[arti].obj2)    /* two-pos obj ?                           */
      {                     	/* Yes                                     */
       pa_x++;                  /* shift in object-pos-s                   */
       ai = part->word->altn[pa+pa_x]; /*alt ind for 1-st pos of art-change*/
       {INT ret;
	if (artbase[arti].objts2 & T_BLANK)/* ins blank=> alt from spec pos */
	 ret=outpos_ins_bl (obj, part, pi, ai);
	else                     /* not blank => insert from artbase        */
	 ret=outpos_ins (obj, pi, artbase[arti].obj2);
	if(ret==NO) obj->pos[pi].lt->attr&=EOLIST;
       }
      }
     pa++;                      /* next nmb of part orig pos               */
     continue;
    }
   else                         /* two-positional source of change:        */
    {			        /* 2=>1 or 2=>2 is possible                */
     pa++;                      /* next nmb of part orig pos               */
     do                         /* skip this & all next spsp & hyphen pos-s*/
      {
       pi++;
#ifdef S_COMMENTS
       if (obj->pos[pi].type_sp & (T_SP2|T_HYPHEN)) /* hyphen/spsp2-pos ?  */
	continue;				    /* Yes, skip it        */
       if (obj->pos[pi].type_sp & T_SP1)            /* spsp1-position ?    */
	outpos_del (obj, pi);   /* delete all alts of the pos        */
#endif
       if (obj->pos[pi].type_sp & T_HYPHEN)         /* hyphen ?            */
	continue;				    /* Yes, skip it        */
       if (obj->pos[pi].type_sp & (T_SP1|T_SP2))    /* spsp1/spsp2 pos ?   */
	outpos_del (obj, pi); /* delete all alts of the pos          */
      }
      while (obj->pos[pi].type_sp & (T_SP1|T_SP2|T_HYPHEN));
				/* result: new pi                          */
     if (!(artbase[arti].obj2)) /* two-pos obj ?                           */
      {       			/* No - del 2-nd pos                       */
       pa_x--;                  /* back shift in object pos-s              */
       pa++;                    /* skip the pos deleted                    */
       goto Outpart_del;        /* del 2-nd pos                            */
      }
     else
      {                     	/* Yes                                     */
       ai = part->word->altn[pa+pa_x]; /* alt ind for 2-nd pos of art-change*/
       if (artbase[arti].objts2 & T_BLANK)
	outpos_repl_bl (obj, part, pi, ai); /*blank=>repl alt from spec pos*/
       else                          /* not blank => repl from artbase     */
	outpos_repl (obj, pi, artbase[arti].obj2);
      }
     goto Outpart_next;
    }
		  /* END OF #ifdef ART */
#endif

Outpart_chkact:
   ai = part->word->altn[pa+pa_x]; /* alt nmb                              */
   if ( (obj->pos[pi].type_sp & T_BLANK)
      &&(!(obj->pos[pi].type_sp & T_MYBLANK))
      )                         /* original blank =>                       */
    {
     outpos_repl_bl (obj, part, pi, ai); /* repl blank by the alt found    */
     goto Outpart_next;
    }
   if ( !(part->word->type_art & T_ALTS) ) /* all alts are original ?      */
    goto Outpart_next;          /* Yes - pos not changed => skip it        */
   if (obj->pos[pi].orig.code==obj->pos[pi].alt[ai].orig.code)
    goto Outpart_next;          /* pos not changed => skip it              */

#ifdef SECOND_PASS
    collect_repl_stat(obj,pi,ai);
#endif
   outpos_exch (obj,pi,ai);/* ordinary exchange of alts within ED-file*/
   goto Outpart_next;
Outpart_del:
   outpos_del (obj, pi);  /* delete all alts of the pos              */
   continue;
Outpart_next:			/* next pos in original part->posn[]       */
   pa++;
Outpart_skip:
   continue;
  }

Outpart_ok:
 return (Ok);

}

 /***********************************************************************/
/* This procedure replaces blank-pos obj->pos[pold]
   by the new alt, specified by obj->pos[SPEC_POS].alt[anew].orig.code.
   The context is to be taken into account
								       */
/***********************************************************************/
INT outpos_repl_bl (SOBJ * obj, SPART * part,
                    INT pos, INT anew)

{
 LT  * lt1;
 BYTE c;

 c = obj->pos[SPEC_POS].alt[anew].orig.code;
 lt1 = obj->pos[pos].lt;

 outpos_context (part, pos, &c); /* correct symbol context, if needed */

 lt1->code = c;
 lt1->attr&=EOLIST;
#ifdef EDPR_CORR
 PRINTF (
  "\n     %d. !!! ED-file corrected: REPLACEMENT in pos %d:  BLANK -> %c",
	 obj->nmb, pos, c);
#endif
return(OK);
}

/***********************************************************************/
/* This procedure replaces 1-st alt of obj->pos[pos]
   by the new alt, specified by char 'cnew'
								       */
/***********************************************************************/
 INT outpos_repl (SOBJ * obj, INT pos, BYTE cnew)

{
 LT  * lt1;
#ifdef EDPR_CORR
 BYTE c;
#endif
 if ( obj->pos[pos].type_sp & (T_BLANK|T_SP1|T_SP2|T_HYPHEN) )
  lt1 = obj->pos[pos].lt;           /* no detailed alt-info : pos beg      */
 else
  lt1 = obj->pos[pos].alt[0].lt;    /* detailed alt info : pos beg         */
#ifdef EDPR_CORR
 c = lt1->code;
#endif
 lt1->code = cnew;
 lt1->attr&=EOLIST;
 if (cnew == ' ') lt1 -> attr = 255;

#ifdef EDPR_CORR
 PRINTF (
  "\n     %d. !!! ED-file corrected: REPLACEMENT in pos %d:  %c -> %c",
	 obj->nmb, pos,
	 c, cnew);
#endif
 return(OK);
}

/***********************************************************************/
/* This procedure exchanges 1-st alt of obj->pos[pos]
   with the new alt, specified by "anew" alt
								       */
/***********************************************************************/
INT outpos_exch (SOBJ * obj, INT pos, INT anew)

{
 LT  * lt1;
 LT  * lt2;
 BYTE c;

 lt1 = obj->pos[pos].alt[0].lt;
 c = lt1->code;
 lt2 = obj->pos[pos].alt[anew].lt;
 lt1->code = lt2->code;
 lt2->code = c;
 lt1 -> attr &= EOLIST;               /* Lepik 09/04/92 09:43pm */
#ifdef EDPR_CORR
 PRINTF (
  "\n     %d. !!! ED-file corrected: pos %d, EXCHANGE %c <=> %c",
	 obj->nmb, pos,
	 obj->pos[pos].alt[anew].lt->code,
	 obj->pos[pos].alt[0].lt->code);
#endif
return(OK);
}

/***********************************************************************/
/* This procedure inserts obj->pos[SPEC_POS].alt[anew].orig.code,
   generated from BLANK,
   as the new position (!) after obj->pos[pos].
   The context is to be taken into account
								       */
/***********************************************************************/
INT outpos_ins_bl (SOBJ * obj, SPART * part,
                   INT pos, INT anew)

{
 /*LT  * lt1;*/
 BYTE c;

 c = obj->pos[SPEC_POS].alt[anew].orig.code; /* what to insert          */
 outpos_context (part, pos, &c);     /* correct context, if needed */

 if(outpos_ins_shift (obj, pos, c)==YES)
 {
   #ifdef EDPR_CORR
     PRINTF (
       "\n     %d. !!! ED-file corrected: INSERT %c AFTER pos %d",
	 obj->nmb, c, pos);
   #endif
   return(OK);
 }
 else
 {
   #ifdef EDPR_CORR
     PRINTF (
       "\n     %d. !There is no room to INSERT %c AFTER pos %d",
	 obj->nmb, c, pos);
   #endif
 return(NO);
 }
}

/***********************************************************************/
/* This procedure inserts 1-st char "cnew" as a new position (!) after "pos"
								       */
/***********************************************************************/
 INT outpos_ins (SOBJ * obj,INT pos, BYTE cnew)

{
 if(outpos_ins_shift (obj, pos, cnew)==YES)
   {
#ifdef EDPR_CORR
 PRINTF (
  "\n     %d. !!! ED-file corrected: INSERT %c AFTER pos %d",
	 obj->nmb, cnew, pos);
#endif
return(OK);
  }
  else
 {
#ifdef EDPR_CORR
 PRINTF (
  "\n     %d. ! There is no room to INSERT %c AFTER pos %d",
	 obj->nmb, cnew, pos);
#endif
 return(NO);
 }
}

/***********************************************************************/
/* This procedure corrects context of symbol to output, if needed.
   The "in-symbol" (*c) is supposed to be capital,
   the "out-symbol" (also *c) will be low-case after the procedure,
   if the convertion required according to the word's context
								       */
/***********************************************************************/
INT outpos_context ( SPART * part,
                   INT pos, BYTE * c)

{
 if (part->word->type & T_LOW)          /*???&& (!(obj->type & T_DIG)) )*/
  {
   if (pos!=0)
    *c = _2low(*c);
   else
    if ( (!(part->word->type & T_NAME))
       &&(!(part->word->pos[0]->type & T_STMT))
       )
     *c = _2low(*c);
  }
return (OK);
}

/***********************************************************************/
/* This procedure inserts 1-st char "cnew" as a new position (!) after "pos"
								       */
/***********************************************************************/
INT outpos_ins_shift (SOBJ * obj, INT pos, BYTE cnew)

{
 struct segm  * savesegm;
 CHAR         * savesymb;
 LT        ca;        /* code & attr to insert                  */
 LT  * pca;       /* pointer to above                       */
 struct segm  * segm;        /* segm of where to insert                */
 LT  * symb;      /* end of pos, i.e. where to insert       */
 LT  * lt;        /* beg of pos,after which to insert       */
 INT lth=0;
 /*struct segm  *tmp;*/
 LONG shift=0;
 INT endposp, endpos;
 INT pi;                         /* curr pos                               */

 savesegm=SPQ.ns_segm;
 savesymb=SPQ.ns_symb;
 ca.code = cnew;
 ca.attr = EOLIST;
 pca = (LT  *)(&ca);
 endposp = obj->pos_part_nmb;       /* last pos index in partitioning-list */
 endpos  = obj->pos_part[endposp];  /* last pos of obj                     */

				    /* find where to insert:               */
 getpos_bel (obj, pos, &lt, &symb, &lth);  /* get beg(lt), end(symb) & lth */
					   /* actually, we don't need lth  */
 segm = obj->pos[pos].tif_ref.segm;        /* segm arg of the pos          */
 SPQ.ins_status=YES;
 if(!test_spare_space(segm,sizeof(LT)))
    return(NO);
  else
   if (insert_symb(segm,(CHAR *)symb,pca)==YES)
				/* either insert in old segm or not: */
  {                                 /* old segm is full, set newsegm       */
				    /* everywhere after the pos inserted:  */
   shift= (LONG)(SPQ.ns_symb-(BYTE  *)(lt));
   for(pi=pos+1; pi<endpos; pi++)
    {
     if(obj->pos[pi].tif_ref.segm!=segm) /* other segm then in ins-pos ?   */
      {                             /* Yes - end of the segm corrected     */
       SPQ.ns_segm=savesegm;
       SPQ.ns_symb=savesymb;
       return(OK);
      }
     else
      {
       obj->pos[pi].tif_ref.segm=SPQ.ns_segm; /* set new segm                */
       corrpos_lt (obj, pi, shift);   /* correct lt of pos & alts          */
      }
    }

   getpos_bel (obj, (INT)(pi - 1), &lt, &symb, &lth);/* get beg(lt),end(symb) & lth */
   SPQ.ns_symb = (CHAR  *)(symb); /* set SPQ.ns_symb via last pos     */
   /* ????SPQ.ns_symb = wrdimg[i-1].source+( (pi-1==pos) ? shift:0); */
  }
 else                                    /* segm not changed, just         */
  {                                      /* shift-to-right is needed :     */
   for (pi=pos+1;pi<endpos;pi++)         /* ACT !!! i=num+1 instead of num */
    {
     if (obj->pos[pi].tif_ref.segm!=segm) /* other segm then in ins-pos ?  */
      break;
     else
      corrpos_lt (obj, pi, (LONG)(sizeof(LT))); /* correct lt   */
    }
   SPQ.ns_segm = savesegm;
   SPQ.ns_symb = savesymb + ((segm==savesegm)? sizeof(LT):0);
   /* ??? SPQ.ns_symb=savesymb+ ((tmp==savesegm)? sizeof(LT):0); */
  }
return(OK);
}

/***********************************************************************/
/* This procedure delete all alts of obj->pos[pos]
								       */
/***********************************************************************/
 INT outpos_del (SOBJ * obj, INT pos)

{
 struct segm  * segm;          /* curr pos segm   */
 LT  * symb;        /* end of curr pos */
 LT  * lt;          /* beg of curr pos */
 INT    lth=0;
 INT endposp, endpos;
 INT pi;                           /* curr pos        */

 getpos_bel (obj, pos, &lt, &symb, &lth);  /* get beg(lt), end(symb) & lth */
 segm = obj->pos[pos].tif_ref.segm;        /* segm arg of the pos          */
 shift_left(lth,segm,(CHAR *)symb);         /* shift-to-left: lth, segm. symb      */

/* correct all obj->pos[P].lt & obj->pos[P].alt[K].lt with the same segm:  */
/* shift-to-left needed in all of them                                     */
 endposp = obj->pos_part_nmb;       /* last pos index in partitioning-list */
 endpos  = obj->pos_part[endposp];  /* last pos of obj                     */

 for (pi=pos+1; pi<endpos; pi++)
  {
   if (obj->pos[pi].tif_ref.segm == segm)   /* if the same segm => shift : */
    corrpos_lt (obj, pi, -((LONG)(lth)));   /* correct lt of pos & alts    */
   else
    break;
  }

#ifdef EDPR_CORR
 PRINTF ("\n     %d. !!! ED-file corrected: DELETE pos %d", obj->nmb, pos);
#endif
  return(OK);
}

/**************************************************************************/
/* This procedure corrects pos & alt references to ED-file.
    lth - the value which is to be add to the pointer
    obj->pos[pos].lt or
    obj->pos[pos].alt[..].lt
									  */
/* ********************************************************************** */
INT corrpos_lt (SOBJ * obj, INT pos, LONG lth)

{
 INT ai;
 obj->pos[pos].lt=(LT *)
   ((CHAR  *)(obj->pos[pos].lt)+lth);
			  /* correct pos                 */
  if ( !(obj->pos[pos].type_sp & (T_BLANK|T_SP1|T_SP2|T_HYPHEN)) )
				    /* if it is alt-detailed pos           */
  for (ai=0; ai <= obj->pos[pos].alt_nmb; ai++)    /* correct all alts:   */
  obj->pos[pos].alt[ai].lt=(LT *)
    ((CHAR  *)(obj->pos[pos].alt[ai].lt)+lth);
      /* correct alt         */
  return(OK);
}

/**************************************************************************/
/* This procedure has been extracted from the ED-internal-tools (EDWARD)
   It is used to delete a list of alternatives.
   It actually shifts ed text to the left.
    v_s - value of shift,
    cur_symb - address of rightmost ed_symb to shift
									  */
/* ********************************************************************** */
INT shift_left(INT v_s,struct segm * cur_segm, CHAR * cur_symb)
 {
  CHAR  *c;
  /*INT l;*/
  CHAR  *from;
  CHAR  *to;
  CHAR  *end;

  c=cur_symb;
  if (c == NULL)
    return(OK);
/*
  end=(CHAR  *)cur_segm;
  end+=sizeof(struct segm)+cur_segm->busy_lth-1;
*/
  end=&cur_segm ->string[cur_segm->busy_lth];
  end+=0;
  from=(CHAR  *)cur_symb;
  to=from-v_s;
  while (from < end)
    *to++=*from++;
  cur_segm->busy_lth -=v_s;
/*****************************************************
   if(cur_segm == SPQ.ns_segm)    // THIS IS WRONG
       SPQ.ns_symb-= v_s;
 *****************************************************/
  if(cur_segm == SPQ.ns_segm)
  {
   if (cur_segm->busy_lth+v_s == SPQ.ns_symb-(BYTE *)cur_segm->string+2)
   {
    BYTE  *symb;
                       /* Set SPQ.ns_symb onto the last line position */

    skip_letter_in_line (cur_segm,-1) ;

                      /* Set SPQ.ns_symb onto the last alt of the last pos */
    do
       symb=SPQ.ns_symb;
        while((SPQ.ns_symb=next_symb(YES,NO,YES,SPQ.ns_segm,SPQ.ns_symb))!= NULL);
    SPQ.ns_symb=symb;
   }
    else
      SPQ.ns_symb -= v_s;
  }
   return(OK);
 }

/***********************************************************************/
/* This procedure gets beg & end pointers to the pos in ED-file,
   as well as total length of all alternatives of the pos
								       */
/***********************************************************************/
INT  getpos_bel (SOBJ * obj, INT pos,
                LT  ** beg,
                LT  ** end,
                INT * lth
	       )
{
 *lth = 0;

 if ( obj->pos[pos].type_sp & (T_BLANK|T_SP1|T_SP2|T_HYPHEN) )
  *beg = obj->pos[pos].lt;           /* no detailed alt-info : pos beg      */
 else
  *beg = obj->pos[pos].alt[0].lt;    /* detailed alt info : pos beg         */
 *end = *beg;
 while (!((*end)->attr&EOLIST))      /* find end of alt-list                */
  {(*lth)+=sizeof(LT);               /* and calc lengh of alt-s to be del   */
   (*end)++;
  }
 (*lth)+=sizeof(LT);                 /* lth = total length of all alts      */
 (*end)++;                           /* end_of_pos                          */
  return(OK);
}
/***********************************************************************/
CHAR suppress_voc = 0;
 INT setobj_blue(SOBJ *obj)
 {
  int i;
  if (suppress_voc) return (OK);	/* no paintings */
  for(i=0;i<obj->pos_part[obj->pos_part_nmb];i++)
    {
      ((LT *)(obj->pos[i].tif_ref.symb))->attr&=EOLIST;
    }
  return(OK);
  }
/*********************************************************************/
 INT setpart_blue(SOBJ *obj,INT beg, INT end)
  { register i = 0;
#ifdef  RUS_ENG_LANG
if (multy_language&&language==LANG_RUSSIAN)
    for(i=beg;i<end;i++)
      if ((obj ->pos[i].type & (T_CAP|T_LOW)))
                break;
    if (i ==end)
       return (OK);
#endif


    for(i=beg;i<end;i++)
      ((LT *)(obj->pos[i].tif_ref.symb))->attr&=EOLIST;
  return(OK);
  }
/*****************************************************************/
#ifdef SECOND_PASS

 void collect_repl_stat(SOBJ *obj,INT npos,INT nalt)
{
 if( rst_last < RST_BUFF_SIZE)
  { register INT i;
    register BYTE was,be,ex=No;
    was=obj->pos[npos].alt[0].orig.code;
     be=obj->pos[npos].alt[nalt].orig.code;
    for( i=0; i<rst_last;i++)
      if((rst[i].was==was)&&(rst[i].be==be))
	 { ex=Ok;
	   break;
	 }
    if(ex==Ok)
      rst[i].no++;
     else
      { rst[i].was=was;
	rst[i].be=be;
	rst[i].no=1;
	rst_last++;
      }
   }
 }

#endif

