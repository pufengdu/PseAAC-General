/****************************************************************************
 * common.h - Copyright 2013 Pufeng Du, Ph.D.                               *
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

#ifndef __COMMON_H__
#define __COMMON_H__

//This file contains the common settings for run time configuration

//Uncommnet the following to use GNU standard
// #define _GNU_SOURCE

//Uncomment the following to use PSEB DEBUG procedures: mtrace
// #define __PSEB_DEBUG__

//Uncomment the following to surpress the error message from pseb
// #define __PSEB_NOERR__

//Here are the common header files we need
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <argp.h>
#include <zlib.h>   //This is new in version 3, we need GZipped file IO facility
#include <sys/types.h>
#include <sys/stat.h>

//These are library headers may not be portable
#include <libgen.h>

//Here are the libraries we need
#include "uthash.h" //This depends on the uthash-dev package
#include <lua.hpp>

//How many amino acid types we have in the AAIndex database
#define NUMBER_OF_AA 20
#define NUMBER_OF_LETTERS 26
#define GAIN_OUTPUT 100
#define BITS_OF_A_BYTE 8
//Common used tools library header that should be included first
#include "tools.h"
//Here are all header files we have in this project
#include "parameters.h"
#include "fasta.h"
#include "pcprop.h"
#include "pseaac.h"
#include "biofiles.h"
#include "error.h"
#include "bems.h"
#include "pseb.h"
#include "luavm.h"

extern struct pcprop *pclibhandle;
extern struct parameters *parahandle;
#endif //__COMMON_H__
