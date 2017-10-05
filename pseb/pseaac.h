/****************************************************************************
 * pseaac.h - Copyright 2013 Pufeng Du, Ph.D.                               *
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

#ifndef __PSEAAC_H__
#define __PSEAAC_H__

#include "common.h"

void pseaac(prseq **h, parameters *p);
void pseaacInit(prseq **h, parameters *p);
void pseaacExecute(prseq **h, parameters *p);
void pseaacWeightAdjust(prseq **h, parameters *p);
int pseaacLength(parameters *p);
int pseaac_aac_Length(parameters *p);
int pseaac_dip_Length(parameters *p);
int pseaac_pse_Length(parameters *p);
int pseaac_aac_seg(prseq *t, int segid, int r_offset);
int pseaac_dip_seg(prseq *t, int segid, int r_offset);
int pseaac_pse_classic_seg(prseq *t, int segid, int r_offset, int max_delay);
int pseaac_pse_amphiphilic_seg(prseq *t, int segid, int r_offset, int max_delay);
void pseaac_aac(prseq **h, parameters *p, int offset);
void pseaac_dip(prseq **h, parameters *p, int offset);
void pseaac_pse(prseq **h, parameters *p, int offset);
void pseaac_pse_classic(prseq **h, parameters *p, int offset);
void pseaac_pse_amphiphilic(prseq **h, parameters *p, int offset);
void pseaacGeneralBEMS(prseq **h, parameters *p);
void pseaacGeneralUsers(prseq **h, parameters *p);
#endif // __PSEAAC_H__
