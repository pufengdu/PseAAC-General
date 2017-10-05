/****************************************************************************
 * listparser.cpp - Copyright 2013 Pufeng Du, Ph.D.                         *
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
#include "listparser.h"

int bmParseGOList(char *memb, size_t l_memb, FILE *dest)
{
    int fd = fileno(dest);
    char *curField = strtok(memb,",");
    int cnt_go_ids = atoi(curField);
    char *dest_buf = (char*)calloc(cnt_go_ids,FEID_LENGTH);
    int count_go_fields = 0;
    while ((curField = strtok(NULL,",")))
    {
        strncpy(dest_buf+count_go_fields*FEID_LENGTH,curField,FEID_LENGTH-1);
        count_go_fields ++;
    }
    if (count_go_fields == cnt_go_ids)
    {
        write(fd,&cnt_go_ids,sizeof(int));
        write(fd,dest_buf,cnt_go_ids*FEID_LENGTH);
        free(dest_buf);
        dest_buf = NULL;
        return cnt_go_ids;
    }
    else
    {
        free(dest_buf);
        dest_buf = NULL;
        return 0;
    }
}

int bmParsePRList(char *memb, size_t l_memb, FILE *dest)
{
    int fd = fileno(dest);
    char *curField = strtok(memb, ",");
    int cnt_pr_ids = atoi(curField);
    char *dest_buf = (char*)calloc(cnt_pr_ids,PRID_LENGTH+sizeof(int)*2);
    int count_pr_fields = 0;
    while((curField = strtok(NULL,",")))
    {
        char *id_ptr = dest_buf+count_pr_fields*(PRID_LENGTH+sizeof(int)*2);
        int *start_coord = (int*)(void*)(id_ptr + PRID_LENGTH);
        int *record_length = (int*)(void*)(id_ptr + PRID_LENGTH + sizeof(int));
        strncpy(id_ptr,curField,PRID_LENGTH - 1);
        curField = strtok(NULL,",");
        *start_coord = atoi(curField);
        curField = strtok(NULL,",");
        *record_length = atoi(curField);
        count_pr_fields ++;
    }
    if (count_pr_fields == cnt_pr_ids)
    {
        write(fd,&cnt_pr_ids, sizeof(int));
        write(fd,dest_buf,cnt_pr_ids*(PRID_LENGTH+sizeof(int)*2));
        free(dest_buf);
        dest_buf = NULL;
        return cnt_pr_ids;
    }
    else
    {
        free(dest_buf);
        dest_buf = NULL;
        return 0;
    }
}

int bmParseASSOCList(char *memb, size_t l_memb, FILE *dest)
{
    int fd = fileno(dest);
    char *curField = strtok(memb, ",");
    int head_guard = atoi(curField);
    if (head_guard != -1)
        return 0;
    char *lastField = NULL;
    for (int i = l_memb -1 ; i > 0; i --)
    {
        if (memb[i]== '\n')
            memb[i] = '\0';
        if (memb[i] == ',')
        {
            lastField = memb + i + 1;
            memb[i] = '\0';
            break;
        }
    }
    int cnt_asf = atoi(lastField);
    int *dest_buf = (int*)calloc(cnt_asf+1,sizeof(int));
    int count_assoc_fields = 0;
    int curValue = 0;
    while((curField = strtok(NULL,",")))
    {
        curValue = atoi(curField);
        dest_buf[count_assoc_fields+1] = curValue;
        count_assoc_fields ++;
    }
    dest_buf[0]=count_assoc_fields;
    if (count_assoc_fields == cnt_asf)
    {
        write(fd,dest_buf,sizeof(int)*(count_assoc_fields+1));
        free(dest_buf);
        dest_buf = NULL;
        return count_assoc_fields;
    }
    else
    {
        free(dest_buf);
        dest_buf = NULL;
        return 0;
    }
}

void bmWriteBlocks(char *fn, FILE *dest, int (*parser)(char *,size_t, FILE *))
{
    char *fb = NULL;
    size_t fs = pdulibLoadMemBlock(fn,&fb);
    if (fs > 0)
    {
        int ek = parser (fb,fs,dest);
        if (ek == 0)
        {
            printf("Block corrupted\n");
        }
        free(fb);
        fb=NULL;
    }
    else
    {
        free(fb);
        fb = NULL;
    }
}



