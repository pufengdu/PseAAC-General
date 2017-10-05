/****************************************************************************
 * fasta.h - Copyright 2013 Pufeng Du, Ph.D.                                *
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

#ifndef __FASTA_H__
#define __FASTA_H__

#include "common.h"

typedef struct segment
{
    int start;
    int length;
} segment;

typedef struct prseq
{
    char *id;
    char *seq;
    int label;
    int len_seq;
    int len_id;
    segment *segs;
    int cnt_segs;
    decimal **propmatrix;
    decimal *pseaac;
    int len_pseaac;
    int len_binfts;
    UT_hash_handle hh;

} prseq;

void faParseIdAndLabel(char *idline, prseq *p);
prseq *faParseFastaInTextFile(struct ifTextFile *t);
prseq *faParseProteinSeq(int startLineNumber, int countOfLines, int totalLength, char **ls);
segment* faCreatSegment(int cnt_segs, int l);
void faAddSegment(prseq **t, int cnt_segs);
void faCleanSequences(prseq **h);



#endif //__FASTA_H__
