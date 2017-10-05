/****************************************************************************
 * fasta.cpp - Copyright 2013 Pufeng Du, Ph.D.                              *
 *                                                                          *
 * This file is part of PseAAC-Builder v3.0.                                *
 * Pseaac-Builder is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * PseAAC-Builder is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with PseAAC-Builder.  If not, see <http://www.gnu.org/licenses/>.  *
 ****************************************************************************/

#include "fasta.h"

void faParseIdAndLabel(char *idline, prseq *p)
{
    int l = strlen(idline);
    char *t = NULL;
    for (int i = 1 ; i < l ; i++)
    {
        if (idline[i] == '|')
        {
            idline[i] = '\0';
            t=idline +i +1;
            break;
        }
    }
    p->id=strdup(idline+1);
    p->label=atoi(t);
}

prseq *faParseProteinSeq(int startLineNumber, int countOfLines, int totalLength, char **ls)
{
    //Creat a new item
    prseq *p = (prseq*)calloc(sizeof(prseq),1);
    faParseIdAndLabel(ls[startLineNumber],p);
    p->len_id=strlen(p->id);
    p->seq=(char*)calloc(sizeof(char),totalLength+1);
    for (int j = 0 ; j < countOfLines ; j++)
    {
        strcat(p->seq,pdulibTrim(ls[startLineNumber+1+j]));
    }
    p->len_seq=strlen(p->seq);
    return p;
}

prseq *faParseFastaInTextFile(struct ifTextFile *t)
{
    prseq *seqhash = NULL;
    int cntTotalLines = t->cntLines;
    char **ls = t->lines;
    //Init some status vars
    int startlinenumber =0;
    int countoflines =0;
    int totallength=0;
    //Now, lets dance!
    for (int i = 0 ; i < cntTotalLines ; i++)
    {
        if (ls[i][0]=='>')
        {
            //This is a new item
            if (totallength !=0)
            {
                prseq *p = faParseProteinSeq(startlinenumber, countoflines, totallength, ls);
                HASH_ADD_KEYPTR(hh,seqhash,p->id,p->len_id,p);
                startlinenumber = i;
                countoflines = 0;
                totallength = 0;
            }
        }
        else
        {
            countoflines ++ ;
            totallength += strlen(ls[i]);
        }
    }
    //The final item can not be deteced by the next items starting signal, so finalize it here
    prseq *p = faParseProteinSeq(startlinenumber,countoflines,totallength,ls);
    HASH_ADD_KEYPTR(hh,seqhash,p->id,p->len_id,p);
    return seqhash;
}

segment* faCreatSegment(int cnt_segs, int l)
{
    segment* tmpResult = (segment*)calloc(sizeof(segment),cnt_segs);
    int seg_len = l/cnt_segs;
    for (int i = 0 ; i < cnt_segs ; i++)
    {
        tmpResult[i].start = i*seg_len;
        tmpResult[i].length = seg_len;
        if (tmpResult[i].start >= l)
        {
            tmpResult[i].start = 0;
            tmpResult[i].length = l;
        }
    }
    int last_len = l - (cnt_segs - 1)*seg_len;
    if (l>0)
    {
        tmpResult[cnt_segs -1].length = last_len;
    }
    else
    {
        tmpResult[cnt_segs-1].start = 0;
        tmpResult[cnt_segs-1].length = l;
    }
    return tmpResult;
}

void faAddSegment(prseq **t, int cnt_segs)
{
    prseq *s, *tmp;
    HASH_ITER(hh,*t,s,tmp)
    {
        s->cnt_segs = cnt_segs;
        s->segs = faCreatSegment(cnt_segs,s->len_seq);
    }
}

void faCleanSequences(prseq **h)
{
    prseq *s, *t;
    HASH_ITER(hh, *h, s, t)
    {
        free(s->pseaac);
        s->pseaac=NULL;
        if (s->propmatrix != NULL)
        {
            for (int i = 0 ; i < pcpropSelectionCount ; i++)
            {
                free(s->propmatrix[i]);
                s->propmatrix[i]=NULL;
            }
            free(s->propmatrix);
            s->propmatrix=NULL;
        }
        free(s->segs);
        s->segs=NULL;
        free(s->seq);
        s->seq=NULL;
        free(s->id);
        s->id=NULL;

        HASH_DEL(*h,s);
        free(s);
        s=NULL;

    }
}
