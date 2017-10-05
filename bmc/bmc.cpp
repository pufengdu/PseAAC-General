/****************************************************************************
 * bmc.cpp - Copyright 2013 Pufeng Du, Ph.D.                                *
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

#include "bmc.h"
#include "listparser.h"

/**

    ARGS
    1   destination file -o
    2   GO-List/Fe-List -f
    3   PR-List -p
    4   ASSOC-List -m

**/

const char *emProgTitle = "PSEB-BMC";

int main(int argc, char *argv[])
{
/*
    FILE *dest = fopen(argv[1],"wb");
    bmWriteBlocks(argv[2],dest,bmParseGOList);
    bmWriteBlocks(argv[3],dest,bmParsePRList);
    bmWriteBlocks(argv[4],dest,bmParseASSOCList);
    fflush(dest);
    fclose(dest);
*/

/*A checking process for file existence and privilege can be applied.*/

	bmcPara *p = paraParseCmdLine(argc, argv);
	FILE *dest = fopen(p->destfile,"wb");
	bmWriteBlocks(p->gofile,dest,bmParseGOList);
	bmWriteBlocks(p->prfile,dest,bmParsePRList);
	bmWriteBlocks(p->assocfile,dest,bmParseASSOCList);
	fflush(dest);
	fclose(dest);

    return 0;
}

emTableLine emTable[] = {
    {EK_SUCCESS,"",false},
    {EK_ABORT_BMC,"Error while compiling %s.\n",true},

};

int emProcess(emCode code, ...)
{
#ifndef __PSEB_NOERR__
    const char *format_msg = emTable[code].msg;
    const char *level_msg = (emTable[code].isAbort) ? "ABORT: " : "WARNING: ";
    va_list plist;
    va_start(plist,code);

    time_t t=time(NULL);
    char *tm = ctime(&t);
    int l = strlen(tm);
    tm[l-1]='\0';

    fprintf(stderr,"[%s]",tm);
    fprintf(stderr," %s %s",emProgTitle, level_msg);

    vfprintf(stderr,format_msg,plist);
    if(emTable[code].isAbort)
    {
        exit(code);
    }
#endif
    return 0;
}

error_t paraOptionParser (int key, char *arg, struct argp_state *state)
{
    bmcPara *para = (bmcPara*)(state->input);
    switch (key)
    {
        case 'o':
		para->destfile = arg;
            break;
        case 'f':
		para->gofile = arg;
            break;
        case 'p':
		para->prfile = arg;
            break;
        case 'm':
		para->assocfile = arg;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

bmcPara *paraParseCmdLine(int argc, char* argv[])
{
    /*Parsing command-line with GNU Arg Parser*/
    /*Refer to http://crasseux.com/books/ctutorial/argp-example.html#argp%20example*/

    struct argp_option options[] =
    {
        {"output", 'o',"OUTPUT_FILE",0, "OUTPUT_FILE is the name of BEM before compression."},
        {"feature",'f',"FEATURE_FILE",0,"FEATURE_FILE is the list of all features."},
        {"protein",'p',"PROTEIN_FILE",0,"PROTEIN_FILE is the list of all proteins."},
        {"assoc",'m',"ASSOC_FILE",0,"ASSOC_FILE is the map between features and proteins."},
        {0}
    };

    /*
        GNU Arg Parser standard version declaration
    */

    argp_program_version = "Binary Extension Module Compiler for PseAAC-Builder";
    argp_program_bug_address = "Dr. Pufeng Du. <PufengDu@gmail.com>";
    char args_doc[] = "";
    char doc[] = "\nBMC - A program for building binary extension modules in pseaac-builder. We strongly recommended the users NOT to use this program independently. This program should be lauched by the online update program. The options -o -f -m -p are mandatory.\vReleased by \n---\nPufeng Du. Ph.D. \nSchool of Computer Science and Technology, Tianjin University\nTianjin 300072, China\n";

    struct argp argp = {options, paraOptionParser, args_doc, doc};
    bmcPara *mypara = (bmcPara*)calloc(sizeof(bmcPara),1);
    
	mypara->destfile=NULL;
	mypara->gofile=NULL;
	mypara->prfile=NULL;
	mypara->assocfile=NULL;

    argp_parse(&argp,argc,argv,0,0,mypara);
    return mypara;
}
