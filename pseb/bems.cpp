/****************************************************************************
 * bems.cpp - Copyright 2013 Pufeng Du, Ph.D.                               *
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
 
#include "bems.h"

void bemsMakeFeatures(bemsModel *r, prseq *p)
{
    bemsIndexItem *t = NULL;
    char *instance_name = p->id;
    HASH_FIND_STR(r->ftIndex,instance_name,t);
    if (t == NULL)
    {
        p->pseaac = NULL;
        p->len_pseaac = 0;
        p->len_binfts = r->numberOfFeatures;
        emProcess(EK_WARN_ID_NOTFOUND,p->id);
        return;
    }
    else
    {
        int tmpS = t->startIdx;
        int tmpC = t->length;
        int *tmpB = r->ftBlock + tmpS;
        p->len_pseaac = tmpC;
        p->len_binfts = r->numberOfFeatures;
        p->pseaac = (decimal*)calloc(p->len_pseaac,sizeof(decimal));
        for (int i = 0 ; i < tmpC ; i++)
        {
            p->pseaac[i] = (decimal)tmpB[i];
        }
    }
}

bool bemsCheckGZipModule(char *fn)
{
    if (!access(fn,R_OK))
        return true;
    else
        return false;
}

bemsModel *bemsParseGZipModule(char *fn)
{
    bemsModel *tmpResult = (bemsModel*)calloc(sizeof(bemsModel),1);
    gzFile f = gzopen(fn,"rb");
    int t;
    gzread(f,&t,sizeof(int));
    tmpResult->numberOfFeatures = t;
    tmpResult->ftNames=(char**)calloc(tmpResult->numberOfFeatures,sizeof(char*));
    int ftNameBlockLength = (tmpResult->numberOfFeatures)*(FEID_LENGTH);
    char *ftNameBlock = (char*)calloc(ftNameBlockLength,sizeof(char));
    gzread(f,ftNameBlock,(tmpResult->numberOfFeatures)*FEID_LENGTH);
    for (int i = 0 ; i < tmpResult->numberOfFeatures ; i++)
    {
        tmpResult->ftNames[i] = ftNameBlock + i*FEID_LENGTH;
    }
    gzread(f,&t,sizeof(int));
    tmpResult->numberOfInstances = t;
    int ftIndexBlockLength = t * (PRID_LENGTH + sizeof(int)*2);
    char *ftIndexBlock = (char*)calloc(ftIndexBlockLength,sizeof(char));
    gzread(f,ftIndexBlock,ftIndexBlockLength);
    bemsIndexItem *h = NULL;
    for (int i = 0 ; i < tmpResult->numberOfInstances ; i++)
    {
        bemsIndexItem *r = (bemsIndexItem*)calloc(1,sizeof(bemsIndexItem));
        r->name = ftIndexBlock + i * (PRID_LENGTH + sizeof(int) * 2);
        r->startIdx = *(int*)(r->name + PRID_LENGTH);
        r->length = *(int*)(r->name + PRID_LENGTH + sizeof(int));
        HASH_ADD_KEYPTR(hh,h,r->name,strlen(r->name),r);
    }
    tmpResult->ftIndex = h;
    gzread(f,&t,sizeof(int));
    tmpResult->ftBlock=(int*)calloc(sizeof(int),t);
    tmpResult->numberOfCrossCells = t;
    gzread(f,tmpResult->ftBlock,sizeof(int)*t);
    gzclose(f);
    return tmpResult;
}
