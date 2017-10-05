/****************************************************************************
 * binmodview.cpp - Copyright 2013 Pufeng Du, Ph.D.                         *
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
#include "binmodview.h"

/****************************************************************************
 *  The description of pseaac-builder's binary extension module format      *
 *  Binary extension module (BEM) of pseaac-builder is a pre-computed       *
 *  feature set that can be used to generate various general form pseaac.   *
 *  This file is actually a GZip compressed binary file. Because the        *
 *  pseaac-builder has built-in gzip file IO facility, the users should not *
 *  bother to uncompress the file before applying it. However, the users are*
 *  allowed to uncompress it to explorer the content of this file.          *
 *  In the uncompressed BEM file, the file was seperated into 3 different   *
 *  blocks: feature name block, instances indices block and feature block.  *
 *  The feature name block stores feature names sequentially with an        *
 *  interger at the beginning (4bytes for 32bit system) as the total number *
 *  of feature names. The instances indices block stores index of every     *
 *  instance with the [name, start_index, leangth] format continuesly with  *
 *  an integer at the beginning as the total number of instances. The       *
 *  feature block is a block of integers, every integer is a location in the*
 *  feature name block. for every instance, the (start, length) gives a     *
 *  segment in the feature block, which can demonstrate which features      *
 *  should be contained by that instance.                                   *
 ****************************************************************************/

/**

    bmvParseModelFromBytes: parse uncompressed binary stream to create a bmModel object.
    Deprecated since GZipped modules are used.
    Deprecated on Jan. 29th, 2013.

**/

/*

bmModel *bmvParseModelFromBytes(char *s, int l)
{
    bmModel *tmpResult = (bmModel*)calloc(1,sizeof(bmModel));

    int tmpNumberOfFt = *(int*)s;
    tmpResult->numberOfFeatures = tmpNumberOfFt;
    tmpResult->ftNames = (char**)calloc(tmpResult->numberOfFeatures,sizeof(char*));
    for (int i = 0 ; i < tmpNumberOfFt ; i++)
    {
        tmpResult->ftNames[i] = (char*)(s + sizeof(int) + i * FEID_LENGTH);
    }

    int tmpLengthOfFtNames = tmpNumberOfFt*FEID_LENGTH + sizeof(int);
    int tmpNumberOfInstance = *(int*)(s+tmpLengthOfFtNames);

    char *s1 = s+ tmpLengthOfFtNames ;

    tmpResult->numberOfInstances = tmpNumberOfInstance;
    bmIndexItem *h = NULL;
    for (int i = 0 ; i < tmpNumberOfInstance ; i++)
    {
        bmIndexItem *tNode = (bmIndexItem*)calloc(1,sizeof(bmIndexItem));
        tNode->name = s1 + i* (PRID_LENGTH + 2 * sizeof(int)) + sizeof(int);
        tNode->startIdx = *(int*)(tNode->name + PRID_LENGTH);
        tNode->length = *(int*)(tNode->name + PRID_LENGTH + sizeof(int));
        HASH_ADD_KEYPTR(hh,h,tNode->name,strlen(tNode->name),tNode);
    }

    tmpResult->ftIndex = h;

    int tmpLengthOfInstanceIndex = tmpNumberOfInstance * (PRID_LENGTH + 2*sizeof(int))+ sizeof(int);

    char *base_block = s1 + tmpLengthOfInstanceIndex ;
    tmpResult->ftBlock=(int*)base_block;

    return tmpResult;

}

*/

void bmvDisplayMultipleFeature(bmModel *r, char *all_names)
{
    char *tname = strtok(all_names,",");
    if (tname != NULL)
    {
        bmvDisplayFeature(r,tname);
    }
    else
    {
        bmvDisplayFeature(r,all_names);
        return;
    }
    while((tname = strtok(NULL,",")))
    {
        bmvDisplayFeature(r,tname);
    }
}

void bmvDisplayFeature(bmModel *r, char *instance_name)
{
    bmIndexItem *t = NULL;

    HASH_FIND_STR(r->ftIndex,instance_name,t);

    if (t == NULL)
    {
        emProcess(EK_WARN_INVALID_INSTANCE,instance_name);
        return;

    }
    int tmpS = t->startIdx;
    int tmpC = t->length;
    int *tmpB = r->ftBlock + tmpS;
    //printf("%d\t%d\n",tmpS,tmpC);
    printf ("%s\n", instance_name);
    for (int i = 0 ; i < tmpC ; i++)
    {
        printf("\t%s\n",r->ftNames[tmpB[i]]);
    }
}

bool checkGZipModule(char *fn)
{
    if (!access(fn,R_OK))
        return true;
    else
        return false;
}

bmModel *bmvParseGZipModule(char *fn)
{
    bmModel *tmpResult = (bmModel*)calloc(sizeof(bmModel),1);
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
    bmIndexItem *h = NULL;
    for (int i = 0 ; i < tmpResult->numberOfInstances ; i++)
    {
        bmIndexItem *r = (bmIndexItem*)calloc(1,sizeof(bmIndexItem));
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

void bmvListFeatureNames(bmModel *r)
{
    for (int i = 0 ; i < r->numberOfFeatures ; i++)
    {
        printf("DIM %08d = %s\n",i,r->ftNames[i]);
    }
}

void bmvListProteinNames(bmModel *r)
{
    bmIndexItem *s, *t;
    HASH_ITER(hh,r->ftIndex,s,t)
    {
        char *tname = s->name;
        printf("%s\n",tname);
    }
}

void bmvDisplayStatistics(bmModel *r)
{
    printf("Total features: %d\n",r->numberOfFeatures);
    printf("Total instances: %d\n",r->numberOfInstances);
    printf("Total ones in feature matrix: %d\n",r->numberOfCrossCells);
}
