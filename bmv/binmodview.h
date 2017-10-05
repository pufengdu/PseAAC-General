/****************************************************************************
 * binmodeview.h - Copyright 2013 Pufeng Du, Ph.D.                          *
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
#ifndef __BINMODVIEW_H__
#define __BINMODVIEW_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <uthash.h>
#include <zlib.h>
#include "bmv.h"

#define PRID_LENGTH 16
#define FEID_LENGTH 16

struct bmSparseFeatureIndexItem
{
    char *name;
    int startIdx;
    int length;
    UT_hash_handle hh;
};

struct bmModel
{
    int numberOfCrossCells;
    int numberOfFeatures;
    int numberOfInstances;
    char **ftNames;
    struct bmSparseFeatureIndexItem *ftIndex;
    int *ftBlock;
};

typedef struct bmModel bmModel;
typedef struct bmSparseFeatureIndexItem bmIndexItem;

void bmvDisplayStatistics(bmModel *r);
void bmvDisplayFeature(bmModel *r, char *instance_name);
void bmvListFeatureNames(bmModel *r);
void bmvListProteinNames(bmModel *r);
void bmvDisplayMultipleFeature(bmModel *r, char *all_names);
bmModel *bmvParseGZipModule(char *fn);
bool checkGZipModule(char *fn);

#endif //__BINMODVIEW_H__
