/****************************************************************************
 * pseb.cpp - Copyright 2013 Pufeng Du, Ph.D.                               *
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

#include "pseb.h"

#ifdef __PSEB_DEBUG__
#include <mcheck.h>
#endif
struct pcprop *pclibhandle;
struct parameters *parahandle;
int main(int argc, char *argv[])
{

#ifdef __PSEB_DEBUG__
    mtrace();
#endif

    parameters *x = paraParseCmdLine(argc,argv,0);
    parahandle = x;
    psebMainProcess(x);
    return 0;
}

void psebMainProcess(parameters *p)
{
    pcprop *h = pcpropLoadMemFile();
    pclibhandle = h;
    pcpropMakeColDefMap();
    pcpropMakeDefaultSelection();

    if (p->queryflag)
    {
        pcpropPrintMemFile(h);
        return;
    }

    decimal **propmat = NULL;
    if (p->pse == PSE_CLASSIC || p->pse == PSE_AMPHIPHILIC)
    {

        pcpropSelectByFile(h,p,&propmat);
        p->pcmat=propmat;
    }
    else if (p->pse == PSE_GENERAL_BEMS)
    {
        if (!bemsCheckGZipModule(p->bemfile))
        {
            emProcess(EK_ABORT_FILE_OPEN_ERROR,p->bemfile,"loading BEM file.");
        }
        else
        {
            p->bem = bemsParseGZipModule(p->bemfile);
        }
    }
    ifTextFile *t = ifLoadTextFile(p->inputfile);
    prseq *ps =faParseFastaInTextFile(t);
    ifUnloadTextFile(t);

    faAddSegment(&ps,p->cntseg);
    pseaac(&ps,p);
    ofWriteAll(&ps,p);

    //CleanUps
    free(p);
    p=NULL;
    faCleanSequences(&ps);
    pcpropCleanUp(&h);
    free(propmat);
    propmat=NULL;
    free(pcpropSelectedNames);
    pcpropSelectedNames=NULL;

}

