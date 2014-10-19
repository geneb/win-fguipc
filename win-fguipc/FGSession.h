/*
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <windows.h>
#include <stdio.h>
//#include "main.h"


#define FG_CALL_CONNECTION			"FSASMLIB:IPC"
#define FG_ACTION_READ				1
#define FG_ACTION_WRITE			2
#define FG_ACTION_READTRUE			3
#define FG_ACTION_WRITETRUE		4
#define FG_ACTION_READTOKEN		5
#define FG_ACTION_WRITETOKEN		6
#define FG_ACTION_SEARCH			7

#define FG_RETURN_FAILURE			0
#define FG_RETURN_SUCCESS			1


void module_init(HINSTANCE dllInstance);
void module_deinit();



typedef struct tagFG_ACTION_READ_HDR {
	DWORD action;
	DWORD offset;
	DWORD size;
	DWORD data;
} FG_ACTION_READ_HDR;

typedef struct tagFG_ACTION_WRITE_HDR {
	DWORD action;
	DWORD offset;
	DWORD size;
} FG_ACTION_WRITE_HDR;

typedef struct tagFG_ACTION_READTOKEN_HDR {
	DWORD action;
	DWORD token;
	DWORD data;
} FG_ACTION_READTOKEN_HDR;

typedef struct tagFG_ACTION_WRITETOKEN_HDR {
	DWORD action;
	DWORD token;
} FG_ACTION_WRITETOKEN_HDR;

typedef struct tagFG_ACTION_SEARCH_HDR {
	DWORD action;
	DWORD offset_from;
	DWORD offset_to;
	DWORD size;
} FG_ACTION_SEARCH_HDR;

enum OperationType {
	WaitingT,
	WritingT,
	ReadingT,
	SearchingT
};