/****************************************************************************
 * biofiles.cpp - Copyright 2013 Pufeng Du, Ph.D.                           *
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

#include "biofiles.h"

//I do really think there should be some standard library for doing the following task.
//However, I have not found one, so I wrote the following quick and dirty procedures.
//If some one knows which library I should use for this purpose, please send me an email PufengDu@gmail.com
//This is very standard procedure, if you want to deal with text files line by line.
//Load the text file into memory with utHash as intermediate structure
//Dependecies : utHash
struct ifTextFile *ifLoadTextFile(char *fn)
{
    //Set the input stream, either STDIN or the specified filename
    FILE *fp = NULL;
    if (fn==NULL)
        fp = stdin;
    else
    {
        if (!access(fn,R_OK))
        {
            fp = fopen(fn,"r");
        }
        else
        {
            emProcess(EK_ABORT_FILE_OPEN_ERROR,fn,"reading input file");
        }
    }

    //Load the file line by line into temporary structure of Hash, remember every line of space was dynamically allocated
    int curLineNumber = 0;
    ifTextFileLine *tmpHandleOfLines = NULL;
    while (!feof(fp))
    {
        char *tmpLine = NULL;
        size_t tmpLength = 0;
        size_t tmpXL = getline(&tmpLine,&tmpLength,fp);
        //if getline return -1, this means reading error or eof, either way, should we stop.
        if ((int)tmpXL == -1)
        {
            free(tmpLine);
            tmpLine = NULL;

            break;
        }
        //Well, we need to chomp the line, like we were in perl
        if (tmpLine[tmpXL-1] == '\n')
            tmpLine[tmpXL-1] = '\0';

        ifTextFileLine *tmpFileLine = (ifTextFileLine*)calloc(sizeof(ifTextFileLine),1);
        tmpFileLine->content=tmpLine;
        tmpFileLine->id=curLineNumber;
        HASH_ADD_INT(tmpHandleOfLines,id,tmpFileLine);
        curLineNumber ++ ;
    }

    if (fp!=stdin)
    {
        fclose(fp);
        fp=NULL;
    }
    //Reforming the structure to an array
    char **alllines = (char**)calloc(sizeof(char*),curLineNumber);
    for (int i = 0 ; i < curLineNumber ; i++)
    {
        ifTextFileLine *t = NULL;
        HASH_FIND_INT(tmpHandleOfLines,&i,t);
        alllines[i]=t->content;
    }
    ifTextFile *tmpText = (ifTextFile*)calloc(sizeof(ifTextFile),1);
    tmpText->cntLines = curLineNumber;
    tmpText->lines = alllines;

    //Cleanup all temporary structures
    ifTextFileLine *s, *t;
    HASH_ITER(hh,tmpHandleOfLines,s,t)
    {
        HASH_DEL(tmpHandleOfLines,s);
        free(s);
        s = NULL;
    }
    HASH_CLEAR(hh,tmpHandleOfLines);

    return tmpText;
}

void ifUnloadTextFile(ifTextFile *t)
{
    int tmpCnt = t->cntLines;
    for (int i = 0 ;i < tmpCnt ; i++)
    {
        free(t->lines[i]);
        t->lines[i]=NULL;
    }
    t->cntLines = 0;
    free(t->lines);
    t->lines=NULL;
    free(t);
    t=NULL;
}

void ofWriteAll(prseq **h, parameters *p)
{
    struct ofSettings ofFormats[] =
    {
        {OPF_SVM, " ",":",true},
        {OPF_MAT, "\t",";",false},
        {OPF_CSV,",",";",false}
    };

    FILE *ofHandle = NULL;
    ofOpenFile(p->outputfile, &ofHandle);
    int kType = (int)(p->outfmt);
    prseq *t, *s;
    HASH_ITER(hh, *h,t,s)
    {
        p->WriteOutFile(t, ofHandle, ofFormats[kType].FS, ofFormats[kType].IDFS, ofFormats[kType].useIDX);
    }

    ofCloseFile(&ofHandle);
}

void ofOpenFile(char *fn, FILE **fh)
{
    if (fn == NULL)
        *fh = stdout;
    else
    {
        char *tfn = strdup(fn);
        if (!access(dirname(tfn),W_OK))
        {

            *fh = fopen(fn,"w");
            free(tfn);
            tfn = NULL;
        }
        else
        {
            free(tfn);
            tfn=NULL;
            *fh = stdout;
            emProcess(EK_ABORT_FILE_OPEN_ERROR,fn,"writing results");
        }
    }
}

void ofCloseFile(FILE **fh)
{
    if (*fh != stdout)
    {
        fflush(*fh);
        fclose(*fh);
        *fh = NULL;
    }
}

void ofWriteDecimal(prseq *t, FILE *out_file, const char *FS, const char *IDFS, bool useIDX)
{
    decimal *r = t->pseaac;
    int l = t->len_pseaac;
    fprintf(out_file, "%d", t->label);
    fprintf(out_file, "%s", FS);
    for (int i = 0 ; i < l ; i++)
    {
        if (useIDX)
        {
            fprintf(out_file, "%d", i+1);
            fprintf(out_file, "%s", IDFS);
        }
        fprintf(out_file, "%.3f", r[i]*GAIN_OUTPUT);
        fprintf(out_file, "%s", FS);
    }
    fprintf (out_file, "#%s\n", t->id);
}

void ofWriteBinary(prseq *t, FILE *out_file, const char *FS, const char *IDFS, bool useIDX)
{
    int l = t->len_pseaac;
    int maxl = t->len_binfts;
    int *tmpRec = (int*)calloc(sizeof(int),maxl);
    for (int i = 0 ; i < l ; i++)
        tmpRec[(int)(t->pseaac[i])] = 1;

    fprintf(out_file, "%d", t->label);
    fprintf(out_file, "%s", FS);
    for (int i = 0 ; i < maxl ; i++)
    {

        if (useIDX)
        {
            if (tmpRec[i] == 0)
                continue;
            fprintf(out_file, "%d", i + 1);
            fprintf(out_file, "%s", IDFS);
        }
        fprintf(out_file, "%d", tmpRec[i]);
        fprintf(out_file, "%s", FS);
    }
    fprintf (out_file, "#%s\n", t->id);
    free (tmpRec);
    tmpRec = NULL;
}
