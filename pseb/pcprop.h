/****************************************************************************
 * pcprop.h - Copyright 2013 Pufeng Du, Ph.D.                               *
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

#ifndef __PCPROP_H__
#define __PCPROP_H__

#include "common.h"

typedef struct pcprop
{
    char *pcname;
    char *pcdesc;
    decimal *pcvalue;
    UT_hash_handle hh;
} pcprop;

extern char pcpropMemFile[];
extern char pcpropColumnDefs[];
extern char **pcpropSelectedNames;
extern struct ifTextFile* pcpropClassicSelection;
extern struct ifTextFile* pcpropAmphiphilicSelection;

extern int pcpropSelectionCount;
extern int pcpropColDefMap[];

pcprop *pcpropLoadMemFile();
void pcpropUnloadMemFile(pcprop *pchh);
void pcpropPrintMemFile(pcprop *pchh);
bool pcpropIsNameExist(pcprop *pchh, char *t);
void pcpropSelectByFile(pcprop *pchh, struct parameters *p, decimal ***refout_selectMatrix);

//Deprecated!
//void pcpropConvertSequenceToProperties(struct prseq **h, decimal **selectedPropsMatrix);

struct ifTextFile *pcpropLoadSelectionFile(parameters *p);
void pcpropUnloadSelectionFile(ifTextFile *t, parameters *p);
void pcpropMakeColDefMap();
void pcpropMakeDefaultSelection();
void pcpropCleanUp(pcprop **h);
void pcpropConvertSingleSeqToProperties(struct prseq *t, decimal **selectedPropsMatrix);
void pcpropCleanUpSinggleSeqProps(struct prseq *t);
#endif //_PCPROP_H__
