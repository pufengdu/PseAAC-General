/****************************************************************************
 * error.h - Copyright 2013 Pufeng Du, Ph.D.                                *
 *                                                                          *
 * This file is part of PseAAC-Builder v2.0.                                *
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

#ifndef __ERROR_H__
#define __ERROR_H__
#include "common.h"
#include <stdarg.h>

#define EK_SUCCESS                             0
#define EK_WARN_INVALID_SYMBOL                 1
#define EK_WARN_INVALID_LAMBDA                 2
#define EK_ABORT_FILE_OPEN_ERROR               3
#define EK_ABORT_PARAMETER_ERROR               4
#define EK_ABORT_BAD_PCPROP                    5
#define EK_ABORT_BAD_PARAMETER_W               6
#define EK_WARN_AAC_INVSYM                     7
#define EK_WARN_DIP_INVSYM                     8
#define EK_WARN_ID_NOTFOUND                    9
#define EK_ABORT_LUAVM_ERROR                  10

typedef unsigned int emCode;

typedef struct emTableLine
{
    emCode e;
    const char *msg;
    bool isAbort;

} emTableLine;

int emProcess(emCode code, ...);

#endif //__ERROR_H__
