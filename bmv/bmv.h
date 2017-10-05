/****************************************************************************
 * bmv.h - Copyright 2013 Pufeng Du, Ph.D.                                  *
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
 
#ifndef __BMV_H__
#define __BMV_H__

#include <argp.h>
#include <stdarg.h>
#include <time.h>
#define EK_SUCCESS                             0
#define EK_WARN_INVALID_LIST                   1
#define EK_WARN_INVALID_INSTANCE               2
#define EK_ABORT_FILE_OPEN_ERROR               3

extern const char *emProgTitle;

typedef unsigned int emCode;

typedef struct emTableLine
{
    emCode e;
    const char *msg;
    bool isAbort;

} emTableLine;

int emProcess(emCode code, ...);

typedef struct bmvPara
{
    char *bemfile;
    char *query_name;
    char list_flag;
    bool stat_flag;
    bool query_flag;
    bool module_flag;
} bmvPara;

error_t paraOptionParser (int key, char *arg, struct argp_state *state);
bmvPara *paraParseCmdLine(int argc, char* argv[]);
void executeBMV(bmvPara *p);


#endif //__BMV_H__
