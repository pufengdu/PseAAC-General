/****************************************************************************
 * bems.h - Copyright 2013 Pufeng Du, Ph.D.                                 *
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
 
#ifndef __BEMS_H__
#define __BEMS_H__

#include "common.h"

#define PRID_LENGTH 16
#define FEID_LENGTH 16

struct bemsSparseFeatureIndexItem
{
    char *name;
    int startIdx;
    int length;
    UT_hash_handle hh;
};

struct bemsModel
{
    int numberOfCrossCells;
    int numberOfFeatures;
    int numberOfInstances;
    char **ftNames;
    struct bemsSparseFeatureIndexItem *ftIndex;
    int *ftBlock;
};

typedef struct bemsModel bemsModel;
typedef struct bemsSparseFeatureIndexItem bemsIndexItem;

bemsModel *bemsParseGZipModule(char *fn);
bool bemsCheckGZipModule(char *fn);
void bemsMakeFeatures(bemsModel *r, prseq *p);
#endif //__BEMS_H__
