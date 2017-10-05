/****************************************************************************
 * luavm.cpp - Copyright 2013 Pufeng Du, Ph.D.                              *
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
 
#include "common.h"
#include "luavm.h"

lua_State *luavmInit(char *globalEnv)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L,luavmGetPCList);
    lua_setglobal(L,"psebGetPCList");
    lua_pushcfunction(L,luavmGetOptions);
    lua_setglobal(L,"psebGetOptions");
    luavmParseGlobalSettings(globalEnv, L);
    return L;
}

void luavmParseGlobalSettings(char *s, lua_State *L)
{
    char *t = strtok(s,";");
    luavmSetVars(t,L);
    while ((t = strtok(NULL,";")))
    {
        luavmSetVars(t,L);
    }
}

void luavmSetVars(char* s, lua_State *L)
{
    if (s == NULL)
        return;
    int l = strlen(s);
    bool valid_flag = false;
    for (int i = 0 ; i < l ; i++)
    {
        if (s[i] == '=')
        {
            valid_flag = true;
            s[i] = '\0';
            char *t_value = pdulibTrim(s+i+1);
            char *t_name = pdulibTrim(s);
            lua_pushstring(L,t_value);
            lua_setglobal(L,t_name);
            break;
        }
    }
    if (!valid_flag)
    {
        emProcess(EK_ABORT_LUAVM_ERROR,"Invalid user settings in the PSEB command line.");
    }
}

void luavmErrorHandle(lua_State **L, int eValue)
{
    if (eValue)
    {
        char *tMsg = strdup(lua_tostring(*L,-1));
        lua_close(*L);
        *L=0;
        emProcess(EK_ABORT_LUAVM_ERROR,tMsg);
    }
}

lua_State *luavmLoadScript(lua_State *L, char *fn)
{
    luavmErrorHandle(&L, luaL_loadfile(L,fn));
    luavmErrorHandle(&L, lua_pcall(L,0,0,0));
    return L;
}

void luavmCallUserSeqProc(lua_State *L, prseq *p)
{
    lua_getglobal(L,"pseb3_seq_proc");
    lua_pushstring(L,p->id);
    lua_pushstring(L,p->seq);
    luavmErrorHandle(&L, lua_pcall(L,2,2,0));
    int ttc = 0;
    if(lua_isnumber(L,-2))
    {
        ttc = lua_tonumber(L,-2);
    }

    p->len_pseaac = ttc;
    p->pseaac = (decimal*)calloc(sizeof(decimal),ttc);

    for (int i = 0 ; i < ttc ; i++)
    {
        if(lua_istable(L,-1))
        {
            lua_pushnumber(L,i+1);
            lua_gettable(L,-2);
            char *tn = (char*)lua_tostring(L,-1);
            p->pseaac[i] = (decimal)atof(tn);
            lua_pop(L,1);
        }
    }
}

int luavmGetPCList(lua_State *L)
{
    char *c = (char*)lua_tostring(L,1);
    pcprop *t = NULL;
    HASH_FIND_STR(pclibhandle,c,t);
    pdulibNormalize(t->pcvalue,NUMBER_OF_AA);
    if (t)
    {
        lua_newtable(L);
        lua_pushstring(L,"name");
        lua_pushstring(L,t->pcname);
        lua_settable(L,-3);
        lua_pushstring(L,"desc");
        lua_pushstring(L,t->pcdesc);
        lua_settable(L,-3);
        for (int i = 0 ; i < NUMBER_OF_AA ; i++)
        {
            char *tnm = strndup(&(pcpropColumnDefs[i]),1);
            lua_pushstring(L,tnm);
            lua_pushnumber(L,t->pcvalue[i]);
            lua_settable(L,-3);
            free (tnm);
            tnm = NULL;
        }
    }
    return 1;
}

int luavmGetOptions(lua_State *L)
{
    lua_newtable(L);
    lua_pushstring(L,"l");
    lua_pushnumber(L,parahandle->l);
    lua_settable(L,-3);
    lua_pushstring(L,"w");
    lua_pushnumber(L,parahandle->w);
    lua_settable(L,-3);
    return 1;
}

void luavmCleanUp(lua_State *L)
{
    lua_close(L);
    L=0;
}
