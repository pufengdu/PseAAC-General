/****************************************************************************
 * tools.cpp - Copyright 2013 Pufeng Du, Ph.D.                              *
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

#include "tools.h"

/*
    This is the normalization procedure
    This procedure applied in normalizing the physicochemical properties
    The physicochemical properties will first be selected and then normalized.
*/

void pdulibNormalize(decimal *s, int l)
{
    decimal tmpSum = 0, tmpSumSquare = 0;
    for (int i = 0 ; i < l ; i ++)
    {
        tmpSum += s[i];
        tmpSumSquare += s[i]*s[i];
    }
    decimal m = tmpSum / (decimal)l;
    decimal d = sqrt(((tmpSumSquare / (decimal)l) - m * m) * l / (l - 1) );
	
    for (int i = 0 ; i < l ; i ++)
    {
        if (d > 0)
        {
            s[i] = (s[i] - m)/d;
        }
    }
}

decimal pdulibAutoCoef(decimal *s, int l, int t)
{
    decimal tmpResult = 0;
    int eid = l - t;
    if (eid <= 0)
        return 0;
    for (int i = 0 ; i < eid ; i++)
    {
        tmpResult+=s[i]*s[i+t];
    }
    tmpResult /= (decimal)(l-t);
    return tmpResult;
}

decimal pdulibDistCoef(decimal **s, int l, int r, int t)
{
    decimal tmpResult = 0;
    int eid = l - t;
    if (eid <= 0)
        return 0;
    for (int i = 0 ; i < eid ; i++)
    {
        decimal tmpS = 0;
        for (int k = 0 ; k < r ; k++)
        {
            tmpS += (s[k][i]-s[k][i+t])*(s[k][i]-s[k][i+t]);
        }
        tmpResult += (tmpS/(decimal)r);
    }
    tmpResult /= (decimal)(l-t);
    return tmpResult;
}

//Trimming functions: these can be replaced by standard library if available.
char *pdulibRTrim(char *s)
{
    int l = strlen(s);
    for (int i = l -1 ; i >=0 ; i--)
    {
        if (isspace(s[i]))
            s[i]='\0';
        else
            break;
    }
    return s;

}

char *pdulibLTrim(char *s)
{
    int l = strlen(s);
    char *t = s;
    int c = 0;
    for (int i = 0 ; i < l ; i++)
    {
        if (isspace(t[i]))
        {
            t++;
            c++;
        }

        else
            break;
    }
    return (char*)memmove(s,t,l-c+1);
}

char *pdulibTrim(char *s)
{
    return pdulibLTrim(pdulibRTrim(s));
}

/*
    parseTextLines countinues reading the stream until it came across a '\n' or '\0'.
    Then it will return the pointer to this '\n' or '\0', as well as changing the '\n' to '\0';
    For safty reason, please besure that s is a null terminated string.

    !!THIS SHOULD BE REPLACED BY WARRPING strsep() ON BSD PLATFORM!!
*/
char *pdulibParseTextLines(char *s, bool *isend)
{
        *isend = false;
        int k = 0;
        while (s[k] != '\0' && s[k] != '\n')
            k++;
        if (s[k] == '\n')
            s[k] = '\0';
        else
            *isend = true;
        return s+k;
}

size_t pdulibFileSize(char *fn)
{
    struct stat t;
    stat(fn,&t);
    return t.st_size;
}

size_t pdulibLoadMemBlock(char *fn, char **memb)
{
    size_t fl = pdulibFileSize(fn);
    FILE *fh = fopen(fn, "rb");
    if (fh == NULL)
    {
        return 0;
    }
    *memb = (char*)calloc(fl,sizeof(char));
    if (*memb != NULL)
    {
        //fread(*memb,fl,1,fh);
        size_t v = fread(*memb,fl,1,fh);
	v = 0;
    }
    fclose(fh);
    return fl;
}
