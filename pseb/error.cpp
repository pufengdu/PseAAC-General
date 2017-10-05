/****************************************************************************
 * error.cpp - Copyright 2013 Pufeng Du, Ph.D.                              *
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

#include "error.h"

emTableLine emTable[] = {
    {EK_SUCCESS,"",false},
    {EK_WARN_INVALID_SYMBOL,"PSEB WARNING: Invalid symbol %c detected in %s, 0 based coordinate is %d.\n",false},
    {EK_WARN_INVALID_LAMBDA,"PSEB WARNING: Invalid lambda value while processing %s.\n\t-Lambda = %d, Segment Length = %d, Segment ID = %d\n",false},
    {EK_ABORT_FILE_OPEN_ERROR,"PSEB ABORT: Failed to open file %s while %s.\n",true},
    {EK_ABORT_PARAMETER_ERROR,"PSEB ABORT: Fatal error while parsing arguments %s: \n\tINFO: %s\n", true},
    {EK_ABORT_BAD_PCPROP,"PSEB ABORT: Physicochemical propperties %s is invalid in selection file %s line %d.\n",true},
    {EK_ABORT_BAD_PARAMETER_W,"PSEB ABORT: Invalid use of w with value %.3f with option \"%s\"\n", true},
    {EK_WARN_AAC_INVSYM,"PSEB WARNING: %d invalid symbols encountered while computing amino acid compositions for segment %d of %s.\n",false},
    {EK_WARN_DIP_INVSYM,"PSEB WARNING: %d invalid symbol pairs encountered while computing di-peptide compositions for segment %d of %s.\n",false},
    {EK_WARN_ID_NOTFOUND,"PSEB WARNING: Protein name %s can not be identified in the loaded BEM file.\n",false},
    {EK_ABORT_LUAVM_ERROR, "PSEB ABORT: Lua script error detected.\n\tINFO: %s\n",true}

};

int emProcess(emCode code, ...)
{
#ifndef __PSEB_NOERR__
    const char *format_msg = emTable[code].msg;
    va_list plist;
    va_start(plist,code);

    time_t t=time(NULL);
    char *tm = ctime(&t);
    int l = strlen(tm);
    tm[l-1]='\0';

    fprintf(stderr,"[%s] ",tm);
    vfprintf(stderr,format_msg,plist);
#endif
    if(emTable[code].isAbort)
    {
        exit(code);
    }

    return 0;
}
