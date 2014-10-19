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

#include "stdafx.h"
#include "Conversion.h"

float KgToLbs(float kg)
{
	return kg / 0.45359237f;
}

float InchToHPA(float inch)
{

		float diff = (inch - 29.92f) / 0.02952998f;
		return 1013 + diff;

}

float HPAToInch(float hpa)
{
	float diff = (hpa - 1013) * 0.02952998f;

    return 29.92f + diff;
}

float MetersToKnot(float meters)
{
	return meters * 1.9438444924406f;
}

float KilogramsToGallons(float kg)
{
	float lbs = KgToLbs(kg);
	return lbs / 6.699219f;
}

float LbsToKg(float lbs)
{
	return lbs * 0.45359237f;
}

int GetBCD(int orig)
{
	int res = 0;

    int dig1 = (orig / 1000);
    if(dig1>0)
        orig = orig % (dig1 * 1000);
    int dig2 = (orig / 100);
    if(dig2>0)
        orig = orig % (dig2 * 100);
    int dig3 = (orig / 10);
    if(dig3>0)
        orig = orig % (dig3 * 10);
    int dig4 = orig;

    res = (res + dig1) << 4;
    res = (res + dig2) << 4;
    res = (res + dig3) << 4;
    res = (res + dig4);

	return res;

}

float WrapHeading(float hdg)
{
	while(hdg<0)
		hdg+=360;
	while(hdg>=360)
		hdg-=360;
	return hdg;
}