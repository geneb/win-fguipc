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
#include <map>
#include <string>
#include <cstdlib>


using namespace std;

extern BYTE* XConnectMemBlock;
extern bool Throttle1Disconnect;
extern bool Throttle2Disconnect;
extern bool PitchDisconnect;
extern bool RollDisconnect;

//extern XPLMDataRef test;




typedef std::map<string, void*> StringMap;

extern StringMap VarMap;

template <typename T>
void XCCopyMemory(unsigned char* target, T const* source)
{
  memcpy(static_cast<void*>(target), static_cast<void const*>(source), sizeof(T));
}