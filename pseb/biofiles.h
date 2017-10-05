/****************************************************************************
 * biofiles.h - Copyright 2013 Pufeng Du, Ph.D.                             *
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

#ifndef __BIOFILES_H__
#define __BIOFILES_H__

#include "common.h"


struct ofSettings
{
    int ofType;
    const char *FS;
    const char *IDFS;
    bool useIDX;
};

typedef struct ifTextFileLine
{
    int id;
    char *content;
    UT_hash_handle hh;
} ifTextFileLine;

typedef struct ifTextFile
{
    int cntLines;
    char **lines;
} ifTextFile;

typedef void (*ofWriteHanle)(prseq *, FILE *, const char *, const char *, bool);

struct ifTextFile *ifLoadTextFile(char *fn);
void ifUnloadTextFile(ifTextFile *t);

void ofWriteAll(prseq **h, parameters *p);
void ofWriteDecimal(prseq *t, FILE *out_file, const char *FS, const char *IDFS, bool useIDX);
void ofWriteBinary(prseq *t, FILE *out_file, const char *FS, const char *IDFS, bool useIDX);

void ofOpenFile(char *fn, FILE **fh);
void ofCloseFile(FILE **fh);

#endif //__BIOFILES_H__
