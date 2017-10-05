/****************************************************************************
 * parameters.h - Copyright 2013 Pufeng Du, Ph.D.                           *
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

#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include "common.h"

typedef enum PSE_TYPE
{	PSE_NONE,   //Not using any pseudo-amino acid composition
	PSE_CLASSIC,    //Using classical type of pseaac
	PSE_AMPHIPHILIC,	//Using amphiphilic type of pseaac
	PSE_GENERAL_BEMS, //Using Binary Extension Modules
	PSE_GENERAL_USERS, //User Script Extension Modules
	PSE_GENERAL_COMPLEXITY, //Reserved
	PSE_GENERAL_AUTOMATA    //Reserved
} PSE_TYPE;	//Enum of pseudo-amino acid types

typedef enum OUTPUT_TYPE
{
	OPF_SVM,    //LibSVM format
	OPF_MAT,    //MATLAB format
	OPF_CSV //Excel CSV format
} OUTPUT_TYPE;  //Enum of output file format types

typedef struct parameters
{
	bool useaac;
	bool usedip;
	bool usepse;
	bool with_w;
	PSE_TYPE pse;
	int l;
	decimal w;
	int cntseg;
	OUTPUT_TYPE outfmt;
	char *inputfile;
	char *outputfile;
	char *pcselfile;
	char **pclist;
	int cntpc;
	int queryflag;
	decimal **pcmat;

	void (*WriteOutFile)(struct prseq *, FILE *, const char *, const char *, bool);
    struct bemsModel *bem;
    char *bemfile;

    char *luafile;
    char *user_settings;

} parameters;

error_t paraOptionParser (int key, char *arg, struct argp_state *state);
parameters *paraParseCmdLine(int argc, char* argv[], struct pcprop *proplib);

#endif // __PARAMETERS_H__
