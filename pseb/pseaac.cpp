/****************************************************************************
 * pseaac.cpp - Copyright 2013 Pufeng Du, Ph.D.                             *
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

#include "pseaac.h"

void pseaac(prseq **h, parameters *p)
{
    pseaacInit(h,p);
    pseaacExecute(h,p);
    pseaacWeightAdjust(h,p);
}

void pseaacInit(prseq **h, parameters *p)
{
    if (p->pse == PSE_GENERAL_BEMS)
        return;

    if (p->pse == PSE_GENERAL_USERS)
        return;

    prseq *t, *s;
    int pseLength = pseaacLength(p);
    HASH_ITER(hh,*h, t,s)
    {
        t->len_pseaac = pseLength;
        t->pseaac=(decimal*)calloc(sizeof(decimal),pseLength);
    }
}

void pseaacExecute(prseq **h, parameters *p)
{
    if (p->pse == PSE_GENERAL_BEMS)
    {
        pseaacGeneralBEMS(h,p);
    }
    else if (p->pse == PSE_GENERAL_USERS)
    {
        pseaacGeneralUsers(h,p);
    }
    else
    {
        int t_offset = 0;
        pseaac_aac(h,p,t_offset);
        t_offset += pseaac_aac_Length(p);
        pseaac_dip(h,p,t_offset);
        t_offset += pseaac_dip_Length(p);
        pseaac_pse(h,p,t_offset);
    }
}

void pseaacGeneralBEMS(prseq **h, parameters *p)
{
    prseq *s, *t;
    HASH_ITER(hh, *h,s,t)
    {
        bemsMakeFeatures(p->bem,s);
    }
}

void pseaacGeneralUsers(prseq **h, parameters *p)
{
    lua_State *L = luavmInit(p->user_settings);
    luavmLoadScript(L,p->luafile);

    prseq *s, *t;
    HASH_ITER(hh, *h, s, t)
    {
        luavmCallUserSeqProc(L, s);
    }
    luavmCleanUp(L);
}


void pseaacWeightAdjust_seg(prseq *t, int aac_start, int pse_start, int pse_length, decimal w)
{

    decimal *r = t->pseaac;
    decimal tmpSum = 0;
    for (int i = 0 ; i < NUMBER_OF_AA ; i++)
        tmpSum += r[aac_start + i];
    for (int i = 0 ; i < pse_length ; i++)
        tmpSum += r[pse_start + i]*w;
    for (int i = 0 ; i < NUMBER_OF_AA ; i++)
        r[aac_start + i] /= tmpSum;
    for (int i = 0 ; i < pse_length ; i++)
        r[pse_start + i] = r[pse_start + i]*w/tmpSum;
}

void pseaacWeightAdjust(prseq **h, parameters *p)
{
    if (p->pse == PSE_GENERAL_BEMS)
        return;
    if (p->pse == PSE_GENERAL_USERS)
        return;

    if(!p->with_w)
        return;
    if (p->usedip)
    {
        emProcess(EK_ABORT_BAD_PARAMETER_W,p->w,"-d");
    }

    if (p->useaac && p->usepse && !p->usedip)
    {
        int lenAAC = pseaac_aac_Length(p);
        //int lenPSE = pseaac_pse_Length(p);
        int pse_factor = (p->pse == PSE_CLASSIC) ? 1 : pcpropSelectionCount;
        prseq *t, *s;
        HASH_ITER(hh,*h,t,s)
        {
            for (int i = 0 ; i < t->cnt_segs ; i++)
            {
                int aac_start = NUMBER_OF_AA * i;
                int pse_length = (p->l)*pse_factor;
                int pse_start = lenAAC + pse_length * i;
                pseaacWeightAdjust_seg(t,aac_start, pse_start, pse_length, p->w);
            }
        }
    }
    else
    {
        emProcess(EK_ABORT_BAD_PARAMETER_W,p->w,"-a and -t are not used together");
    }
}

int pseaac_aac_Length(parameters *p)
{
    if (p->useaac)
        return NUMBER_OF_AA * (p->cntseg);
    else
        return 0;
}

int pseaac_dip_Length(parameters *p)
{
    if (p->usedip)
        return NUMBER_OF_AA * NUMBER_OF_AA * (p->cntseg);
    else
        return 0;
}

int pseaac_pse_Length(parameters *p)
{
    if (p->usepse)
    {
        int exp_factor = (p->pse == PSE_CLASSIC) ? 1 : pcpropSelectionCount;
        return (p->l)*exp_factor*(p->cntseg);
    }
    else
        return 0;
}

int pseaacLength(parameters *p)
{
    return pseaac_aac_Length(p)+pseaac_dip_Length(p)+pseaac_pse_Length(p);
}

int pseaac_aac_seg(prseq *t, int segid, int r_offset)
{
    int seg_start = t->segs[segid].start;
    int seg_length = t->segs[segid].length;
    decimal *r = t->pseaac + r_offset;
    char *seg_seq = t->seq + seg_start;
    int cnt_inval_sym =0;
    for (int i = 0 ; i < seg_length ; i++)
    {
        int r_idx = pcpropColDefMap[toupper(seg_seq[i])-'A'];
        if (r_idx >=0)
        {
            r[r_idx] ++;
        }
        else
        {
            cnt_inval_sym ++;
        }
    }
    for (int i = 0 ; i < NUMBER_OF_AA ; i++)
    {
        r[i]/=seg_length;
    }
    if(cnt_inval_sym)
        emProcess(EK_WARN_AAC_INVSYM,cnt_inval_sym,segid, t->id);
    return cnt_inval_sym;
}

void pseaac_aac(prseq **h, parameters *p, int offset)
{
    if (!p->useaac)
        return;
    else
    {
        prseq *t, *s;
        HASH_ITER(hh,*h,t,s)
        {
            for (int i = 0 ; i < t->cnt_segs ; i++)
            {
                pseaac_aac_seg(t,i,offset+NUMBER_OF_AA*i);
            }
        }
    }
}

int pseaac_dip_seg(prseq *t, int segid, int r_offset)
{
    int seg_start = t->segs[segid].start;
    int seg_length = t->segs[segid].length;
    decimal *r = t->pseaac + r_offset;
    char *seg_seq = t->seq + seg_start;
    int cnt_inval_sym =0;
    for (int i = 0 ; i < seg_length - 1; i++)
    {
        int r_idx_1 = pcpropColDefMap[toupper(seg_seq[i])-'A'];
        int r_idx_2 = pcpropColDefMap[toupper(seg_seq[i+1])-'A'];
        int r_idx = r_idx_1 * NUMBER_OF_AA + r_idx_2;
        if (r_idx_1 >=0 && r_idx_2 >=0)
        {
            r[r_idx] ++;
        }
        else
        {
            cnt_inval_sym ++;
        }
    }
    for (int i = 0 ; i < NUMBER_OF_AA*NUMBER_OF_AA ; i++)
    {
        r[i]/=(seg_length-1);
    }
    if(cnt_inval_sym)
        emProcess(EK_WARN_DIP_INVSYM,cnt_inval_sym,segid, t->id);
    return cnt_inval_sym;

}

void pseaac_dip(prseq **h, parameters *p, int offset)
{
    if (!p->usedip)
        return;
    else
    {
        prseq *t, *s;
        HASH_ITER(hh,*h,t,s)
        {
            for (int i = 0 ; i < t->cnt_segs ; i++)
            {
                pseaac_dip_seg(t,i,offset+NUMBER_OF_AA*NUMBER_OF_AA*i);
            }
        }
    }
}

int pseaac_pse_classic_seg(prseq *t, int segid, int r_offset, int max_delay)
{
    int seg_start = t->segs[segid].start;
    int seg_length = t->segs[segid].length;
    decimal *r = t->pseaac + r_offset;
    //char *seg_seq = t->seq + seg_start;
    decimal **seg_pcmat = (decimal**)calloc(sizeof(decimal*),pcpropSelectionCount);
    for (int i = 0 ; i < pcpropSelectionCount ; i++)
        seg_pcmat[i] = t->propmatrix[i]+seg_start;

    int cnt_inval_sym =0;
    for (int i = 1 ; i <= max_delay ; i++)
    {
        r[i-1] = pdulibDistCoef(seg_pcmat,seg_length,pcpropSelectionCount,i);
    }

    free(seg_pcmat);
    seg_pcmat = NULL;

    return cnt_inval_sym;
}

void pseaac_pse_classic(prseq **h, parameters *p, int offset)
{
    if (!p->usepse)
        return;
    else
    {
        prseq *t, *s;
        HASH_ITER(hh, *h, t, s)
        {
            pcpropConvertSingleSeqToProperties(t,p->pcmat);
            for (int i = 0 ; i < t->cnt_segs ; i++)
            {
                pseaac_pse_classic_seg(t,i,offset+(p->l)*i,p->l);
            }
            pcpropCleanUpSinggleSeqProps(t);
        }
    }
}

int pseaac_pse_amphiphilic_seg(prseq *t, int segid, int r_offset, int max_delay)
{
    int seg_start = t->segs[segid].start;
    int seg_length = t->segs[segid].length;
    decimal *r = t->pseaac + r_offset;
    decimal **seg_pcmat = (decimal**)calloc(sizeof(decimal*),pcpropSelectionCount);
    for (int i = 0 ; i < pcpropSelectionCount ; i++)
        seg_pcmat[i] = t->propmatrix[i]+seg_start;
    int cnt_inval_syms = 0;
    for (int i = 1 ; i <= max_delay ; i++)
    {
        for (int j = 0 ; j < pcpropSelectionCount ; j++)
        {
            r[(i-1)*pcpropSelectionCount + j] = pdulibAutoCoef(seg_pcmat[j],seg_length,i);
        }
    }
    free(seg_pcmat);
    seg_pcmat=NULL;
    return cnt_inval_syms;
}

void pseaac_pse_amphiphilic(prseq **h, parameters *p, int offset)
{
    if(!p->usepse)
        return;
    else
    {
        prseq *t, *s;
        HASH_ITER(hh, *h, t,s)
        {
            pcpropConvertSingleSeqToProperties(t,p->pcmat);
            for (int i = 0 ; i < t->cnt_segs ; i++)
            {
                pseaac_pse_amphiphilic_seg(t,i,offset+(p->l)*pcpropSelectionCount*i,p->l);
            }
            pcpropCleanUpSinggleSeqProps(t);
        }
    }
}

void pseaac_pse(prseq **h, parameters *p, int offset)
{
    if (p->pse == PSE_CLASSIC)
    {
        pseaac_pse_classic(h,p,offset);
    }
    else if (p->pse == PSE_AMPHIPHILIC)
    {
        pseaac_pse_amphiphilic(h,p,offset);
    }
}
