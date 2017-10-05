/****************************************************************************
 * tools.h - Copyright 2013 Pufeng Du, Ph.D.                                *
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

#ifndef __TOOLS_H__
#define __TOOLS_H__

/*This is a library of commonly used functions that are not provided by standard POSIX or GNU C library*/
/*This library should be independent to the rest parts of the projects*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

/*Precision setting for floating computation*/

//#define __USE_DOUBLE__
#ifdef __USE_DOUBLE__
  typedef double decimal;
#else
  typedef float decimal;
#endif

/*Precision setting end here*/

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int uint64;

void pdulibNormalize(decimal *s, int l);
decimal pdulibAutoCoef(decimal *s, int l, int t);
decimal pdulibDistCoef(decimal **s, int l, int r, int t);
char *pdulibRTrim(char *s);
char *pdulibLTrim(char *s);
char *pdulibTrim(char *s);

char *pdulibParseTextLines(char *s, bool *isend);
size_t pdulibFileSize(char *fn);
size_t pdulibLoadMemBlock(char *fn, char **memb);

#endif //__TOOLS_H__
