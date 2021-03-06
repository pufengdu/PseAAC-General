/****************************************************************************
 * listparser.h - Copyright 2013 Pufeng Du, Ph.D.                           *
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
 
#ifndef __LISTPARSER_H__
#define __LISTPARSER_H__

#define PRID_LENGTH 16
#define FEID_LENGTH 16

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../pseb/tools.h"


int bmParseGOList(char *memb, size_t l_memb, FILE *dest);
int bmParsePRList(char *memb, size_t l_memb, FILE *dest);
int bmParseASSOCList(char *memb, size_t l_memb, FILE *dest);

void bmWriteBlocks(char *fn, FILE *dest, int (*parser)(char *,size_t, FILE *));

#endif //__LISTPARSER_H__
