/****************************************************************************
 * bmv.cpp - Copyright 2013 Pufeng Du, Ph.D.                                *
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
#include "binmodview.h"
#include "bmv.h"

const char *emProgTitle = "PSEB-BMV";

int main(int argc, char *argv[])
{

    bmvPara *p = paraParseCmdLine(argc,argv);
    executeBMV(p);

    return 0;
}

void executeBMV(bmvPara *p)
{
    if (!p->module_flag)
        return;
    char *fn = p->bemfile;
    if (!checkGZipModule(fn))
    {
        emProcess(EK_ABORT_FILE_OPEN_ERROR, p->bemfile);
    }
    bmModel *m = bmvParseGZipModule(fn);
    if (p->list_flag)
    {
        if (p->list_flag == 'p')
        {
            bmvListProteinNames(m);
        }
        else if (p->list_flag == 'f')
        {
            bmvListFeatureNames(m);
        }
    }
    if (p->stat_flag)
    {
        bmvDisplayStatistics(m);
    }
    if (p->query_flag)
    {
        bmvDisplayMultipleFeature(m,p->query_name);
    }
}

emTableLine emTable[] = {
    {EK_SUCCESS,"",false},
    {EK_WARN_INVALID_LIST,"Invalid list requirement \"%c\".\n",false},
    {EK_WARN_INVALID_INSTANCE,"Invalid instance name %s.\n",false},
    {EK_ABORT_FILE_OPEN_ERROR,"Failed to open module file %s.\n",true},

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
    bmvPara *para = (bmvPara*)(state->input);
    switch (key)
    {
        case 'q':
            para->query_flag = true;
            para->query_name = arg;
            break;
        case 'm':
            para->bemfile = arg;
            para->module_flag = true;
            break;
        case 'l':
            {
                para->list_flag = true;
                if (tolower(arg[0]) == 'f')
                    para->list_flag = 'f';
                else if (tolower(arg[0]) == 'p')
                    para->list_flag = 'p';
                else
                {
                    emProcess(EK_WARN_INVALID_LIST,arg[0]);
                    para->list_flag = '\0';
                }

            }
            break;
        case 'i':
            para->stat_flag = true;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

bmvPara *paraParseCmdLine(int argc, char* argv[])
{
    /*Parsing command-line with GNU Arg Parser*/
    /*Refer to http://crasseux.com/books/ctutorial/argp-example.html#argp%20example*/

    struct argp_option options[] =
    {
        {"query", 'q',"INSTANCE_ID",0, "Display the features corresponding to the instance with INSTANCE_ID in the MODULE_FILE. INSTANCE_ID can be multiple ids that are seperated by comma"},
        {"module",'m',"MODULE_FILE",0,"Load the MODULE_FILE."},
        {"list",'l',"{f|p}",0,"List every possible feature/instance names."},
        {"info",'i',0,0,"Display the statistic summary of loaded MODULE_FILE."},
        {0}
    };

    /*
        GNU Arg Parser standard version declaration
    */

    argp_program_version = "Binary Extension Module Viewer for PseAAC-Builder";
    argp_program_bug_address = "Dr. Pufeng Du. <PufengDu@gmail.com>";
    char args_doc[] = "";
    char doc[] = "BMV - A program for viewing the content of binary extension modules in pseaac-builder. \vReleased by \n---\nPufeng Du. Ph.D. \nSchool of Computer Science and Technology, Tianjin University\nTianjin 300072, China\n";

    struct argp argp = {options, paraOptionParser, args_doc, doc};
    bmvPara *mypara = (bmvPara*)calloc(sizeof(bmvPara),1);
    mypara->bemfile = NULL;
    mypara->query_name = NULL;
    mypara->list_flag = '\0';
    mypara->stat_flag = false;
    mypara->query_flag = false;
    argp_parse(&argp,argc,argv,0,0,mypara);
    return mypara;
}
