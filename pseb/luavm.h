/****************************************************************************
 * luavm.h - Copyright 2013 Pufeng Du, Ph.D.                                *
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

/****************************************************************************
 *  This is the user defined sequence processor extension in PseAAC-Builder *
 *                                                                          *
 *  In pseb v3.0, we allow users to define their own version of pseudo-amino*
 *  acid composition. This function will be achieved by embedding a simple  *
 *  Lua script intepreter within the program. Please refer to               *
 *  http://www.lua.org/ for more details about the Lua language.            *
 *                                                                          *
 *  In this version, the main loop in C will feed the sequence one by one   *
 *  to the script, user script just need to provide ability to process only *
 *  one sequence.                                                           *
 *                                                                          *
 *  However, we are planning to give users more flexibility in the future   *
 *  versions, as this kind of processing is of low efficiency.              *
 ****************************************************************************/

#include "common.h"

#ifndef __LUAVM_H__
#define __LUAVM_H__

lua_State *luavmInit(char *globalEnv);
void luavmCallUserSeqProc(lua_State *L, prseq *p);
lua_State *luavmLoadScript(lua_State *L, char *fn);
void luavmErrorHandle(lua_State **L, int eValue);
void luavmCleanUp(lua_State *L);
void luavmSetVars(char* s, lua_State *L);
void luavmParseGlobalSettings(char *s, lua_State *L);

//Interop functions
int luavmGetPCList(lua_State *L); //Interop stub : PhysicoChemical properties
int luavmGetOptions(lua_State *L); //Interop stub : Commandline options

#endif //__LUAVM_H__
