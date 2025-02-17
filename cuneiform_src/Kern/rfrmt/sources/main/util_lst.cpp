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

#ifdef _MSC_VER
#include<crtdbg.h>
#endif

#include "lst3_win.h"
#include "aldebug.h"

//#include <math.h>
#include "wind32.h"

#ifdef WIN32
	#include "snptools.h"
#endif


#ifdef alDebug
 extern short FlagGraphic1,Graphic1Color;
#endif

/*=========*/
KNOT *inc_lst(KNOT **beg,KNOT **beg_free)
/*=========*/
{ KNOT *ptr=*beg_free,*ptr1=*beg;
  if(ptr != NULL) /*� ������ ���� ������*/
  { if( (*beg_free=(*beg_free)->next) != NULL) (*beg_free)->back=NULL;
    ptr->next=ptr1; ptr->back=NULL; *beg=ptr;
    if(ptr1 != NULL) ptr1->back=ptr; /*Inc �� ������ ��-� � List*/
  }
  return ptr;
}
/*inc_after_lst - �������� � �������� ������ ����� �������� ptr
(����� ����� ��� ����������. �������)*/
KNOT *inc_after_lst(KNOT *ptr,KNOT **beg,KNOT **beg_free)
/*==========1-�������� �� ����� ������, 2-����� ������*/
{ KNOT *beg_free_old=*beg_free,*next;
  if(ptr == NULL) ERR(1,"inc_after_lst");
  if(*beg_free == NULL) return NULL; /*� ������ ������. ��� ������ */
  if(*beg == NULL) return inc_lst(beg,beg_free); /*��� ������ ����*/
  if( (*beg_free=(*beg_free)->next) != NULL) (*beg_free)->back=NULL;
  next=ptr->next; ptr->next=beg_free_old; beg_free_old->back=ptr;
  if(next != NULL) /*�������� �� ����� ����� ������*/
  { beg_free_old->next=next; next->back=beg_free_old;}
  else             /*�������� ����� ����� ������*/
  { beg_free_old->next=NULL; }
  return beg_free_old;
}
/*==========*/
void del_lst(KNOT *ptr,KNOT **beg,KNOT **beg_free)
/*=====??��� �����,���� ������� (1)������,(2)�����,(3)� ������ ����� 1��-�*/
{ KNOT *back=ptr->back,*next=ptr->next;
  if(back != NULL && next != NULL) /*��������.��-� ������*/
    { back->next=next; next->back=back; }/*�� ������ List � �� �����*/
  else if(next != NULL)                  /*�� �����  List, �� ������*/
    { *beg=next; next->back=NULL;  }
  else if(back != NULL)                  /*�� ������ List, �� �����*/
      back->next=NULL;
  else                                   /*� ����� � ������ List*/
      *beg=NULL;
  ptr->back=NULL;
  if(*beg_free != NULL) /*List Free �� ����,������������� ����� �� ������ �������*/
       { ptr->next=*beg_free; (*beg_free)->back=ptr; }
  else   ptr->next=NULL;
  *beg_free=ptr;
}
//�������� � ������������ � ����.��-��
KNOT* DelLstNext(KNOT *ptr,KNOT **beg,KNOT **beg_free)
{ KNOT *back=ptr->back,*next=ptr->next;
  if(back != NULL && next != NULL) /*��������.��-� ������*/
    { back->next=next; next->back=back; }/*�� ������ List � �� �����*/
  else if(next != NULL)                  /*�� �����  List, �� ������*/
    { *beg=next; next->back=NULL;  }
  else if(back != NULL)                  /*�� ������ List, �� �����*/
      back->next=NULL;
  else                                   /*� ����� � ������ List*/
      *beg=NULL;
  ptr->back=NULL;
  if(*beg_free != NULL) /*List Free �� ����,������������� ����� �� ������ �������*/
       { ptr->next=*beg_free; (*beg_free)->back=ptr; }
  else   ptr->next=NULL;
  *beg_free=ptr;
  return next;
}
/*==========*/
int init_lst(KNOT ***knot,int *k_bloc,int max_knot,KNOT **beg_free,int size_item)
/*==========*/
{ int i,j,size_bloc[MAX_BLOC],kb=-1,fl,k_item;
  KNOT *ptr,*ptr1,*ptr2,*kn[MAX_BLOC];
  //char *err="init_lst";
  /* ===������������� ���������� � �������� ������� ===*/
  if( (fl=alloc_seg(kn,&kb,max_knot,size_item,size_bloc)) ) return fl-10;
  if(*k_bloc == -1) /*������  ������ ������ ��� ������� ������*/
    {if( (*knot=(KNOT**)malloc((MAX_BLOC+2)*sizeof(KNOT*))) == NULL) return -3;}
  do0(i,0,kb) (*knot)[i+(*k_bloc+1)]=kn[i];
  /*===���������� ������������� ������===*/
  if(*k_bloc == -1) /*������  ������ ������ ��� ������� ������*/
    { *beg_free=kn[0]; kn[0][0].back=NULL; }
  else              /*���������� ����� ����� � ������ ������������� ������*/
  { if(*beg_free == NULL) /*������ ������. ������ ��������*/
      { *beg_free=kn[0]; kn[0][0].back=NULL; }
    else
      { ptr=*beg_free; while(ptr->next != NULL) ptr=ptr->next; /*���� �� ������*/
        kn[0][0].back=ptr; ptr->next=kn[0];
      }
  }
  *k_bloc+=kb+1; k_item=-1;
  do0(i,0,kb)
  { ptr=kn[i];
    do0(j,0,size_bloc[i])
    { ptr1=(KNOT*) ((char*)ptr + j*size_item); /*ptr[j]*/
      ptr2=(KNOT*) ((char*)ptr1 + size_item);  /*ptr[j+1]*/
      if( ++k_item == max_knot )
      { ptr1->next=NULL; goto endc; }
      else
      { if(j < size_bloc[i]) /*������ ������ ������ �����*/
          { ptr1->next=ptr2; ptr2->back=ptr1; }
        else                 /*������ ����� �������*/
          { if(i<kb) { ptr1->next=kn[i+1]; kn[i+1]->back=ptr1; }
            else return -4;
          }
      }
    }
  }
  endc: return 0;
}
/*==========*/
int alloc_seg(KNOT **kn,int *kb,int max_kn,uint size_item,int *size_bloc)
/*==========*/
{ int i,k_knot; uint size;
  *kb=-1;
  /*===������� ������� �������� ������ ��� max_knot �����*/
  k_knot=(int)(SIZE_SEG/size_item); //max_kn=max_knot;
  while(max_kn > -2)
  { uint k=MAX(MIN(k_knot,max_kn+2),1);
    if( (kn[++(*kb)]=(KNOT*)malloc(k*size_item)) != NULL) /*������ �����*/
       { max_kn-=k; size_bloc[*kb]=k-1; }
    else
    { --(*kb); size=determine_free_memory(k*size_item);
      if(size < (uint)MIN_KNOT*size_item) /*������ ���� �� �������*/
        { do0(i,0,*kb) free(kn[i]); return -3; }
      k_knot=size/size_item;
    }
    if((*kb) > MAX_BLOC-2) /*����� ����� ������ �������� ������*/
      { do0(i,0,*kb) free((char*)kn[i]); return -4; }
  }
  return 0;
}
/*================���-��� ����.���������� ��������.�����(max=65500)*/
uint determine_free_memory(uint size1)
/*===========================*/
{ char *ptr; uint size=size1;
  while( (ptr=(char*)malloc(size)) == NULL )
    if((size=((size>>1)+(size>>2))) < 100) {size=0;break;}
  if(size) free(ptr);
  return size;
}
/*===========������������ ������ ������*/
void free_lst(KNOT **knot,int k_bloc)
{ int i;
  if(k_bloc < 0 || knot == NULL) return;
  doi(i,k_bloc,0) free((char*)knot[i]); free((char*)knot);
}
#ifndef CLUST1

typedef FRAME PRMTR;

#ifdef OLD_COOR
  typedef struct tagOLD_FRAME
  { unsigned long start_pos,end_pos; int left,up,right,down;//Standard FRAME
    int oldX,oldY;
  } OLD_FRAME;
#else
  typedef FRAME OLD_FRAME;
#endif

#define KBUF 512
#define KHIS 50

Rect16 RectCut;

#define TestExactReg(f) \
	   (DIST_V(f.left,f.right,RectCut.left,RectCut.right)  > 0 && \
		  DIST_V(f.up,  f.down, RectCut.top, RectCut.bottom) > 0)

//������ �� prt-����� ���� rb-����� � ���������. ����������� ��������� ����� �
//==========�������������� ���������� ������� �����, ���� ����� ����� �����
int read_frm(char *file_frm,FRAME ***frm_arr,int *k_arr_frm,FRAME ***frm,
             int *k_frm)
{
	FILE *rb;
	long file_len;
	int fl,k,i,j,size_bloc[MAX_BLOC];
	OLD_FRAME *ptr;
  FRAME **f;
	PRMTR *buf;
  long *his,kf,kf1;
  int SizeMin,kb=KBUF-1,kh=KHIS-1,dx,dy,kb1,kk,SizeItem;

	#ifdef OLD_COOR
    SizeItem=sizeof(FRAME)+sizeof(POINT);
  #else
    SizeItem=sizeof(FRAME);
  #endif
  //  SizeMin=par.AllowLine ? 0 : par.xmin_abs;
  SizeMin=0;
  if( (rb=fopen(file_frm, "rb")) == NULL)
  	return -1;
  file_len= filelength_m(rb);
  kf=file_len/sizeof(PRMTR);

	#ifdef EXACT_REG
		flOver=TRUE;
		if(kf > MAX_FRAME)
	  {
			flOver=FALSE;
			buf=(PRMTR*)malloc_m(KBUF*sizeof(PRMTR));
	    kf1=kf; kf=0;
	    while(kf1 > 0)
	    {
	    	if(kf1 > KBUF)
	    		{fread_m(buf,KBUF*sizeof(PRMTR),1,rb); kb1=KBUF;}
	      else
	      	 kb1=fread_m(buf,sizeof(PRMTR),KBUF,rb);
	      kf1-=kb1;
	      for(i=0; i < kb1; ++i)
	        if(TestExactReg(buf[i]))
						++kf;
	    }
    	fseek_m(rb,0L,SEEK_SET); free_m(buf);
		}
	#endif //EXACT_REG

	//==���� ����� �����, ������������ ����� SizeMin ��� ������ ������� �� �����
  if(kf > MAX_FRAME)
  {
		/* FIXME: to build in MS VC++
		#ifdef _MSC_VER
			if(SnpIsActive())
				SnpLog("***read_frm too many of frames %d, we'll be reduce",kf);
		#endif
		*/

		buf=(PRMTR*)malloc(KBUF*sizeof(PRMTR));
    his=(long*)malloc(KHIS*sizeof(long));
    kf1=kf; for(i=0; i <= kh; ++i) his[i]=0;
    while(kf1 > 0)
    {
    	if(kf1 > KBUF)
    		{fread(buf,KBUF*sizeof(PRMTR),1,rb); kb1=KBUF;}
      else
      	 kb1=fread(buf,sizeof(PRMTR),KBUF,rb);
      kf1-=kb1; --kb1;
      for(i=0; i <= kb1; ++i)
      {
        dx=buf[i].right-buf[i].left;dy=buf[i].down-buf[i].up;
        dx=MAX(dx,dy); ++his[MIN(dx,kh)];
      }
    }
    kf1=0;
    doi(i,kh,1)
    { kf1+=his[i];
      if(kf1 >= MAX_FRAME)
      	{SizeMin=MAX(SizeMin,i); kf=kf1-his[i]; break;}
    }
    fseek(rb,0L,SEEK_SET); free(buf); free(his);
  }
  *k_arr_frm=-1;
  if( (*frm_arr=(FRAME**)malloc((MAX_BLOC+2)*sizeof(FRAME*))) == NULL)
 		return -3;
  if((fl=alloc_seg((KNOT**)*frm_arr,k_arr_frm,(int)kf+3,SizeItem,size_bloc)))
  	return fl-20;

  if((*frm=(FRAME**) malloc(((int)kf+3)*sizeof(FRAME*))) == NULL)
      return -3;

  f=*frm;
  if((buf=(PRMTR*)malloc(KBUF*sizeof(PRMTR))) == NULL)
		return -3;
  kf1=file_len/sizeof(PRMTR); k=-1; ptr=(OLD_FRAME*)(*frm_arr)[kk=0]; j=-1;
  while(kf1 > 0)
  {
		if(kf1 > KBUF)
  		{fread(buf,KBUF*sizeof(PRMTR),1,rb); kb1=KBUF;}
    else
    	 kb1=fread(buf,sizeof(PRMTR),KBUF,rb);
    kf1-=kb1; --kb1;
    for(i=0; i <= kb1; ++i)
    {
    	dx=buf[i].right-buf[i].left;dy=buf[i].down-buf[i].up;
      if(dx <= 0 || dy <= 0)
      	return -10;
      if(MAX(dx,dy) > SizeMin
				#ifdef EXACT_REG
				 && (flOver || TestExactReg(buf[i]))
				#endif
				) //FiltrSmallest
      {
      	if(++j > size_bloc[kk])
        {
					if(++kk > *k_arr_frm)
						return -7;
				 	ptr=(OLD_FRAME*)(*frm_arr)[kk]; j=0;
				}
        //memmove_m(&ptr[j],&buf[i],sizeof(FRAME));
        memcpy(&ptr[j],&buf[i],sizeof(FRAME));
        #ifdef OLD_COOR
          //ptr[j].old.x=ptr[j].left; ptr[j].old.y=ptr[j].up;
          ptr[j].oldX=ptr[j].left; ptr[j].oldY=ptr[j].up;
          //*(int*)(&ptr[j]+sizeof(FRAME))=ptr[j].left;
          //*(int*)(&ptr[j]+sizeof(FRAME)+sizeof(int))=ptr[j].up;
          //memcpy(&ptr[j]+sizeof(FRAME),&ptr[j].left,2);
          //memcpy(&ptr[j]+sizeof(FRAME)+2,&ptr[j].up,2);
        #endif
        (*frm)[++k]=(FRAME*)&ptr[j];
      }
    }
  }
  for(i=kk+1; i <= *k_arr_frm; ++i)
		free((*frm_arr)[i]);
  *k_arr_frm=kk;
  free(buf); *k_frm=k;

	fclose(rb);
  return 0;
}
//������ �������� ����-� old �������� ������ ���� ����� f:
//==���� �� �������.������� �����, ���� Rotate
//void GetOldCoor(FRAME *f, POINT *old,float tg_ang)
void GetOldCoor(FRAME *f, POINT *old)
//==
{
  #ifndef OLD_COOR
    int xa,ya;
    float fi=(float)atan(tg_ang),si=(float)sin(fi),co=(float)cos(fi);
    xa=f->left; ya=f->up; old->x=xa*co+ya*si; old->y=-xa*si+ya*co;
  #else
    old->x=((OLD_FRAME*)f)->oldX;
		old->y=((OLD_FRAME*)f)->oldY;
  #endif
}
//==�������������� ������ ����-� ����� f � ����� fo
void RestoreOldCoorFrm(FRAME *f,FRAME *fo,float tg_ang)
{
	POINT old; int dx=f->right-f->left,dy=f->down-f->up;

	GetOldCoor(f,&old);
  fo->start_pos=f->start_pos;
	fo->end_pos=f->end_pos;
  fo->left=old.x;
	fo->up=old.y;
	fo->right=old.x+dx;
	fo->down=old.y+dy;
}
//==�������������� ������ ����-� ����� f � RECT fo
//void RestoreOldCoorRect(FRAME *f,RECT *fo,float tg_ang)
void RestoreOldCoorRect(FRAME *f,RECT *fo)
{
	//POINT old;
	//int dx,dy;
	int dx,dy,oldX,oldY;

	dx=f->right-f->left;
	dy=f->down -f->up;

  oldX=((OLD_FRAME*)f)->oldX;
	oldY=((OLD_FRAME*)f)->oldY;

	//GetOldCoor(f,&old);
	/*
  fo->left  =old.x;
	fo->top   =old.y;
	fo->right =old.x+dx;
	fo->bottom=old.y+dy;
	*/
  fo->left  =oldX;
	fo->top   =oldY;
	fo->right =(oldX+dx);
	fo->bottom=(oldY+dy);
}
////������ ����� ����� � ���������� �� ����������� ��������
//#define MAX_STAT 8000
//#define MAX_SIZE 30
////==========
//int read_frmW(char *file_frm,POINT_H *CentrW,int *SizeX_W,int *SizeY_W,
//    long *k_frm)
//{ FILE *rw; long kf,k; long ave_x,ave_y;
//  int kb=1000,kb1,i,x,y,dx,dy,sig,med,mod,ave;
//  STATIC POINT_H c; STATIC int *sx,*sy; STATIC FRAME *b;
//
//  if( (rw=fopen_m(file_frm,OF_READ)) == NULL)
//    {*SizeX_W=0;*SizeY_W=0;*k_frm=-1;return 1;}
//  kf= filelength_m(rw)/SIZE_FRAME_FILE;
//  //if(kf > (long)MAX_FRAME) kf=MAX_FRAME/2-1;
//  if((c=(POINT_H)halloc_m(kf,sizeof(POINT)))==NULL) return -3;
//  if(kf < MAX_STAT)
//  { sx=(int*)malloc_m((int)kf*sizeof(int));
//    sy=(int*)malloc_m((int)kf*sizeof(int));
//    if(sx==NULL||sy==NULL) return -3;
//  }
//  if((b=(FRAME*)malloc_m(kb*sizeof(FRAME)))==NULL) return -3;
//  --kf;
//
//  k=-1; ave_x=ave_y=0;
//  while(1)
//  { if((kb1=fread_m(b,sizeof(FRAME),kb,rw)) == 0 || k >= kf)break;
//    do0(i,0,kb1-1)
//    { if((dx=b[i].right-b[i].left) < MAX_SIZE &&
//         (dy=b[i].down-b[i].up) < MAX_SIZE) //����� ����������� ������� �����
//      { x=(b[i].left+b[i].right)>>1; y=(b[i].up+b[i].down)>>1;
//        c[++k].x=x; c[k].y=y;
//        if(kf < MAX_STAT) { sx[k]=dx; sy[k]=dy; }
//        else { ave_x+=(long)dx; ave_y+=(long)dy; }
//      }
//    }
//  }
//  fclose_m(rw);free_m(b);
//  if(kf < MAX_STAT)
//  { sig=0;med=0;mod=0;statis1(sx,(int)k,&ave,&sig,&med,&mod,10);*SizeX_W=ave;
//    sig=0;med=0;mod=0;statis1(sy,(int)k,&ave,&sig,&med,&mod,10);*SizeY_W=ave;
//    free_m(sx);free_m(sy);
//  }
//  else { *SizeX_W=(int)((float)(ave_x)/(k+1)+.5);
//         *SizeY_W=(int)((float)(ave_y)/(k+1)+.5);
//       }
//  *CentrW=c;*k_frm=k;
//  return 0;
//}
/*==������������ ������ �� FRAME*/
void PASC free_frm(FRAME **frm_arr,int k_arr_frm,FRAME **frm)
/*==*/
{
    free((char*)frm);
    free_lst((KNOT**)frm_arr,k_arr_frm);
}

long time(void)
{
    return 0L;
}
#endif
//*****����-������*****

//Return: 0 - OK, NOT_ALLOC - ��� ����� ��� �����
//==�����������
int NewStack(int size,STACK *St)
//==
{ if((St->arr=(KNOTT**)malloc(size*sizeof(PTR)))==NULL) return NOT_ALLOC;
  St->size=size; St->pos=0;
  return 0;
}
//==����������
void DelStack(STACK *St)
//==
{ if(St->arr != NULL) free(St->arr); }
//Parameters:ptr - ��������� ������
//Return: 0 - OK, NOT_ALLOC - ������������ �����
//==������� � ����
int PushStack(KNOTT *ptr,STACK *St)
//==
{ if(St->pos >= St->size) return NOT_ALLOC;
  St->arr[St->pos++]=ptr;
  return 0;
}
//Return: ����������� ���������, ���� ���� ������ ��� NULL, ���� ���� ����
//==��������� �� �����
KNOTT *PopStack(STACK *St)
//==
{ return (St->pos > 0) ? St->arr[--St->pos] : NULL; }
//==�������� ���� ��� ������������ ������
void ClearStack(STACK *St)
//==
{St->pos=0;}
//==�������� ������������ �����: return=FALSE => ���� ��� �� ����������
int OverflowStack(STACK *St)
//==
{return St->pos > St->size;}

//*****�������� � TREE2*********
//����������� �� ����� ����������� ��� ����-� �� ������ �� �����,
//���� ���������. � ����� ����� �������� ��� ������, ����� down

//���������: Curr - �����.����, St - ���������. ���� (������ = max ������� TREE)
//����� ������� ������ NextKnot ��������� ������������ ����� ���� ������ �-���
//==������� � ���������� ���� ������ (��������� ������ - ����, ����� ������)
KNOTT *NextKnot(KNOTT *Curr,STACK *St)
//==
{
 /*
	KNOTT *Next=Curr->next;
  if(Curr->down)        //���� �������� ����, ���� ����
  { if(Next) PushStack(Next,St); return Curr->down; }
  else                  //���� ���� ������ �����, ���� � ����, ����� �������� �� �����
    return (Next) ? Next : PopStack(St);
*/
	KNOTT *Next=Curr->next;
  if(Curr->down)        //���� �������� ����, ���� ����
   {
		if(Next) PushStack(Next,St);
#ifdef alDebug
    if(FlagGraphic1)  ++Graphic1Color;
#endif
		return Curr->down;
	 }
  else                  //���� ���� ������ �����, ���� � ����, ����� �������� �� �����
	 {
		if(!Next)  {Next=PopStack(St);
#ifdef alDebug
		if(FlagGraphic1)  --Graphic1Color;
#endif
		}
		return (Next);
   }




}
//---����� �������� ������ � ����������:---
//1.InitSubAlloc(&SubAlloc)
//2.Ptr=SubMalloc(WORD size,&SubAlloc)
//3.DeleteSubAlloc(&SubAlloc)
//---�����������: ---
//1.��� �������� ������ � �������������� ������� ��������� �� ������������
//  ������ ����������
//2.���������� ������������ ������, ������� ���������� SubMalloc, �� �����,
//  �������� ����� DeleteSubAlloc
//3.����� ������ ���������.�����.������-�� �.�. > 64Kb
//4.������������ �.�. ��������� ���������.�����.������-�.
//5.����� ������� ���  ������� ������ ��������� ��������,�.�. �� �����������
//WIndows-����������� � �� �������� ����� �� ������ ������������� ��������
//malloc/free � GlobalLock/GlobalAlloc/LocalLock/LocalAlloc/GlobalFree � �.�.
//�������������� ������ ���������.�����.������-�� Sub ������� Size ����
//Return: 0 - OK, NOT_ALLOC - ��� ������
//==
int InitSubAlloc(long Size,SUB_ALLOC *Sub)
//==
{ int NumPtr=(int)(Size/SIZE_SEGL),in; long k;
  if((long)NumPtr*SIZE_SEGL  < Size) ++NumPtr;
  Sub->Ptr=(char**)malloc(NumPtr*sizeof(PTR));           //ml
  Sub->SizePtr=(long*)malloc(NumPtr*sizeof(long));       //ml
  Sub->NumPtr=NumPtr; Sub->CurrPtr=0; Sub->CurrPos=0; Sub->Size=Size;
  in=-1;
  while(Size > 0)
  { k=MIN(SIZE_SEGL,Size);
    if((Sub->Ptr[++in]=(char*)malloc((uint)k))==NULL)return NOT_ALLOC;
    Sub->SizePtr[in]=k; Size-=k;
  }
  return 0;
}
//��������� ������ � SubMalloc-�����.������������ s, ��� �����-�� - �
//�������� ��������. ��������
//==
char *Submalloc(uint size,SUB_ALLOC *s)
//==
{ long pos; const char *err="Submalloc";
  #ifdef DEBUG_MEM
   if(size == 0 || size > SIZE_SEGL) ERR(2,err);
  #endif
  if(s->CurrPtr >= s->NumPtr || s->SizePtr[s->CurrPtr]-s->CurrPos < (long)size)
  { s->CurrPos=0;//������� � ����.��������
    while (++s->CurrPtr < s->NumPtr && s->SizePtr[s->CurrPtr] < (long)size);
    if(s->CurrPtr >= s->NumPtr)//��������� ��� ��������,������� �����
    { s->Ptr=(char**)realloc_m(s->Ptr,s->NumPtr*sizeof(PTR),
        (s->NumPtr+1)*sizeof(PTR));
      s->SizePtr=(long*)realloc_m(s->SizePtr,s->NumPtr*sizeof(long),
        (s->NumPtr+1)*sizeof(long));
      if((s->Ptr[s->NumPtr]=(char*)malloc((uint)SIZE_SEGL))==NULL)return NULL;
      s->SizePtr[s->NumPtr]=SIZE_SEGL; s->Size+=SIZE_SEGL;
      s->CurrPtr=s->NumPtr++;
    }
  }
  if((pos=s->CurrPos) < 0 || pos+(long)size > s->SizePtr[s->CurrPtr])
     ERR(1,err);
  s->CurrPos+=size;
  return &s->Ptr[s->CurrPtr][pos];
}
//==���������� ������ SubAllocator-a
void DeleteSubAlloc(SUB_ALLOC *s)
//==
{ int i;
  for(i=0; i < s->NumPtr; ++i) free(s->Ptr[i]);
  free(s->Ptr); free(s->SizePtr);
}
//==����������������� � ������ SubAllocator-a ��� ������� ������ ��� ����
void ClearSubAlloc(SUB_ALLOC *s)
  {s->CurrPtr=0;s->CurrPos=0;}

#ifdef __STR__

//==������ ���� � ����������� ������==
/* // !!! Art - ��������
int GetCriptDir(char *path,int len1)
{ int len;
  //#ifdef BLANK
   #ifdef WIN_MOD
    GetPrivateProfileString("Files","BaseDir","",path,len1,"scriptum.ini");
   #endif
  len=strlen_m(path);
  if(len) {path[len]=92; path[++len]='x'; path[++len]=0;}
  return strlen_m(path);
}
*/ // !!! Art - ��������
//==��������� ��� ���������==
/* // !!! Art - ��������
int GetTypeDoc(void)
{ int len;
  #if defined (WIN_MOD) && !defined (__MRK__) && !defined (__DOT__)
    char name[10],ext[5];
    if((len=GetCriptDir(Fullpath,99))==0) return -2;
    _splitpath(Fullpath,drive,dir,name,ext);
    _makepath(FileParStr,drive,dir,"struct","ini");
    return GetPrivateProfileInt("USER","TypeDoc",PLAIN,FileParStr);
  #else
    return PLAIN;
  #endif
}
*/ // !!! Art - ��������
#endif /*__STR__ */

int memmove_m(void *out,void *in,long size) {
    memmove(out, in, (uint)size);
    return 0;
}

/*
//==
int ProjectFrm(FRAME **frm,int NumFrm,float tg_ang)
//==
{ FRAME *f; int i;
  float fi=(float)atan(tg_ang);
  int xa,ya,xc,yc,dx,dy;
  float si=(float)sin(fi),co=(float)cos(fi);
  for(i=0; i < NumFrm; ++i)
  { f=frm[i]; xa=(f->right + f->left)>>1; ya=(f->up + f->down)>>1;
    xc=(int)(xa*co+ya*si); yc=(int)(-xa*si+ya*co);
    dx=xc-xa; dy=yc-ya;
    f->left+=dx; f->right+=dx; f->down+=dy; f->up+=dy;
  }
  return 1;
}
*/
//#undef CT_SKEW

#ifdef CT_SKEW
	#include "skew1024.h"
#endif

//==
int ProjectFrm1024(FRAME **frm,int NumFrm,Int32 Skew1024)
//==
{ FRAME *f; int i;
  int xa,ya
		#ifndef CT_SKEW
		,xc,yc
    #endif
		,dx,dy;
  if(!Skew1024) return 0;
  for(i=0; i < NumFrm; ++i)
  {
		f=frm[i]; xa=(f->right + f->left)>>1; ya=(f->up + f->down)>>1;

		#ifndef CT_SKEW
	    xc=xa + (int)(((Int32)ya*Skew1024)/1024);
	    yc=ya - (int)(((Int32)xa*Skew1024)/1024);
	    dx=xc-xa; dy=yc-ya;
		#else
			Point16 pt;
			pt.x=xa; pt.y=ya;
			Deskew(pt,-Skew1024);
	    dx=pt.x-xa; dy=pt.y-ya;
		#endif

    f->left+=dx; f->right+=dx; f->down+=dy; f->up+=dy;
  }
  return 1;
}
//Return: 0 - OK (� ������ ������������� ������ dsym=INDEF)
//������ ���������. ��������� - ��� ������ ����� �������� ���� ����������
//������ ����� ��� ������ � ������� AS 0,1,2
//-------
//|1|0|2|
//-------
//���������� ���������.(��������.) ��������� dsym � ������ AveX,AveY
//������ 2 �������� ������ AveX,AveY:
//�) �� ��� �� �������, ��� � dsym ����� ����
//�) �� �������, �� ���. ������ dsym, ����� ���� �� ���� �����, ������� �����
//   �������� ~= mod dsym (�� ��� �������� ������� ��� ���� ��� ������� dsym)
//����:
//  frm[num] - ����� (if(Limit != NULL) => ������������������� ������)
//  bnd - ��������
//  dxAS,dyAS -  ������� ������ AS
//  Limit - ������ ����������� �������� (���� == NULL, ��� �����������)
//  MinVol - ���.����� ������� ��� ������ (�� ������� �� ������ ����� �����)
//  MinPerc - ���. % ����� � ��������. ���� ����������� �� ������ ����� !!� �������!!
//  limDX,limDY - ���������� �������� ��������� ���� ��� ������� dsym
//  AveX != NULL => ���� ��������� AveX,AveY
int EstIntrvlHor(FRAME **frm,int num,BOUND *bnd,int dxAS,int dyAS,
    RECT *Limit,int MinVol,float MinPerc,int limDX,int limDY,
    int *dsym,int *AveX,int *AveY)
{ AS As; SEG_ARR List; KNOT3 ***beg_as,*p,*pp,*ptr; RECT Rect = {0};
  int i,in,ret,iy,ix,up,down,left,right,ii,fl,dist,ky,kx,n,
      dx,dy,ddx,ddy,inX[3],iix,l,r,u,d,dxMax,NumMod,nn;
  int MaxNum=MAX(300,MinVol);
  int *arr,*arrY;
  const char *err="EstIntrvlHor";
  *dsym=*AveX=*AveY=INDEF; ret=0;
  if(Limit) //--����-� ������� ���������� �����--
  { int MinX=Limit->left,MaxX=Limit->right,MinY=Limit->top,MaxY=Limit->bottom;
    for(i=0,in=num-1,num=0; i <= in; ++i)
    { FRAME *f=frm[i];
      int dx=f->right-f->left,dy=f->down-f->up;
      if(dx > MinX && dx < MaxX && dy > MinY && dy < MaxY) num++;
      else {frm[i]=frm[in]; frm[in]=f; --i; --in;}
    }
  }
  if(num < MinVol) return 0;
  //--��������� AS
  List.NumSeg=-1; if((fl=init_lst(&List.ArrSeg,&List.NumSeg,num,
              (KNOT**)&List.free,sizeof(KNOT3))) != 0) return -100-fl;
  if(fl=GenAS(frm,num-1,dxAS,dyAS,bnd,(KNOT3*)List.free,-1,&As,Rect))return fl-10;
  n=0; ky=As.ky-1,kx=As.kx-1; beg_as=As.beg_as;
  if((arr=(int*)malloc(MaxNum*sizeof(int)))==NULL)
		return NOT_ALLOC;
  do0(iy,1,ky) do0(ix,1,kx)
  { inX[0]=ix; inX[1]=ix-1; inX[2]=ix+1;
    for(pp=beg_as[iy][ix]; pp; pp=pp->next)
    { FRAME *ff=pp->f;
      left=ff->left; right=ff->right; down=ff->down; up=ff->up;
      ddx=right-left; ddy=down-up; dxMax=-32000;
      do0(ii,0,2)
      { iix=inX[ii];
        for(p=beg_as[iy][iix]; p; p=p->next)
        { FRAME *f=p->f;
          l=f->left; r=f->right; d=f->down; u=f->up; dx=r-l; dy=down-up;
          if((dist=DIST_V(left,right,p->f->left,p->f->right)) < 0 &&
              dist > dxMax && DIST_V(up,down,u,d) > 0 &&
              pp->beg != p && /*����� �� ���� ���������� ���*/
              abs(dx-ddx) < limDX && abs(dy-ddy) < limDY) {dxMax=dist;ptr=p;}
        }
        if(!ii && dxMax != -32000) break;//���� � ����� ������ �����,� ��.�� ����
      }
      if(dxMax != -32000)//����� ������
      { pp->beg=ptr; ptr->beg=pp;
        //���� ����������� ��������������� ����, �� ����� pp->beg ���� ptr �� ������
        if(n >= MaxNum)
          {if((arr=(int*)realloc_m(arr,n*sizeof(int),(MaxNum*=2)*sizeof(int)))==NULL)return NOT_ALLOC;}
        arr[n++]=-dxMax;
      }
    }
  }
  if(n < MinVol) goto RET;
  //���� � ��������. ����������� ����
  if(statis2(arr,n-1,NULL,dsym,2,&NumMod)) {ret=-21;goto RET;}
  if(NumMod < (int)(MinPerc*n)) {*dsym=INDEF;goto RET;}
  //*dsym=-*dsym;
  //--������ AveX,AveY--
  if((arrY=(int*)malloc(MaxNum*sizeof(int)))==NULL)return NOT_ALLOC;
  nn=0;
  do0(iy,1,ky) do0(ix,1,kx)
  { for(pp=beg_as[iy][ix]; pp; pp=pp->next)
    { if(pp->beg)
      { FRAME *f,*ff;
        f=pp->f;      arr[nn]=f->right-f->left;   arrY[nn]=f->down-f->up;
        if(pp->beg == NULL) ERR(124,err);
        ff=pp->beg->f;
        pp->beg=NULL;
        if(abs(*dsym+DIST_V(f->left,f->right,ff->left,ff->right)) < 3) nn++;
        if(nn >= MaxNum)
        { arr=(int*)realloc_m(arr,nn*sizeof(int),(MaxNum=2*MaxNum)*(sizeof(int)));
          arrY=(int*)realloc_m(arrY,nn*sizeof(int),MaxNum*(sizeof(int)));
          if(arr==NULL||arrY==NULL) return NOT_ALLOC;
        }
      }
    }
  }
  if(nn > 2)
  { if(statis2(arr, nn-1,NULL,AveX,1,&NumMod)) {ret=-21;goto RET;}
    if(statis2(arrY,nn-1,NULL,AveY,1,&NumMod)) {ret=-21;goto RET;}
  }
  free(arrY);
RET:
  free(arr);
  free_lst(List.ArrSeg,List.NumSeg);
  do0(i,0,As.ky) free(As.beg_as[i]); free(As.beg_as);
  return ret;
}
//== ��������� ���������� ������� � ����� PRS-����
Bool16 GetScanRes_LenPrs(char *ImageName)
//==
{ split_path(ImageName, dir, fname, ext);
	ScanResolution=300;
	{ TITLE_PRS TitlePRS; float sx,sy;
		make_path(Fullpath, dir, fname,"pr1");
		if( (fip=fopen(Fullpath, "rb")) == NULL)
			return FALSE;
		fread(&TitlePRS,sizeof(TITLE_PRS),1,fip);
		sx=(float)TitlePRS.ScanResX; sy=(float)TitlePRS.ScanResY;
		if(TitlePRS.ScanResX!=0 && TitlePRS.ScanResY!=0 && sx/sy < 3. && sx/sy > .3
				&& sx > 9. && sy > 9.) ScanResolution=TitlePRS.ScanResX*10;
		WidthPRS= (TitlePRS.Name[3] == 'T' || TitlePRS.Name[3] == 't') ? 2 : 3;
		fclose(fip);
	}
	MulScanRes=(float)((ScanResolution+.05)/300.);
	if(WidthPRS != 2) return FALSE;
	return TRUE;
}
