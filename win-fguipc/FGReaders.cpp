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
#include "FGReaders.h"
#include "FGRead.h"
#include "Conversion.h"
#include "Common.h"
#include <stdio.h>
#include <stdint.h>


extern SOCKET fgSock;

int GetString(char* resultBuf, const char* dataRef, int maxlen);

//XPLMDataRef GetDataRef(const char* dataRef)
//{
//	XPLMDataRef intvar;
//
//	StringMap::iterator it = VarMap.find(dataRef);
//
//	if(it==VarMap.end())
//	{
//		intvar = XPLMFindDataRef(dataRef);
//		VarMap[dataRef] = intvar;
//	}
//	else
//	{
//		intvar = it->second;
//	}
//
//	//intvar = XPLMFindDataRef(dataRef);
//	return intvar;
//
//}


float GetFloat(const char* dataRef)
{
	char buf[128];
    if (GetString((char*)&buf, dataRef, 128) == 0) {
        return atof(buf);
    }
}

int GetInt(const char* dataRef)
{
    char buf[128];
    if (GetString((char*)&buf, dataRef, 128) == 0) {
        if (strcmp(buf, "true")) {
            return 1;
        } else {
            if (strcmp(buf, "false")) {
                return 0;
            } else {
                return atoi(buf);
            }
        }
        //return atoi(buf);
    } else {
        return -1;
    }

	//return 1;
	//return GetFGData(dataRef);
}

double GetDouble(const char* dataRef)
{
	return 1.0;
	// return GetFGData(dataRef);
}

float GetFloat(const char* dataRef, int index)
{
	return 1.0;
	//return GetFGData(dataRef, index);
}
int GetInt(const char* dataRef, int offset)
{
	return 1;
	//return GetFGData(dataRef, index);
	
}

int GetString(char* resultBuf, const char* dataRef, int maxlen) {
    // get a string from FlightGear's property tree.
    char outBuf[2048];
    char inBuf[2048];
    char inCh = 0;
    int index = 0;
    int outBytes = 0;
    strcpy_s(outBuf, 2048, "get ");
    strcat_s(outBuf, 2048, dataRef);
    strcat_s(outBuf, 2048, "\r\n");
    strcat_s(outBuf, 2048, "\0");
    outBytes = send(fgSock, outBuf, strlen(outBuf), 0);
    if (outBytes != SOCKET_ERROR) {
        // get the answer!
        while (inCh != '\n') {
            if (recv(fgSock, &inCh, sizeof(inCh), 0) == SOCKET_ERROR) {
                return -1;
            } else {
                if (inCh != '\n' && inCh != '\r') {
                    inBuf[index++] = inCh;
                }
            }
        }
        inBuf[index++] = '\0';
        ZeroMemory(resultBuf, maxlen);
        // now we copy off the maxlen bytes we're going to return.
        if (strlen(inBuf) > (size_t)maxlen) {
            memcpy(resultBuf, inBuf, maxlen);
            
        } else {
            memcpy(resultBuf, inBuf, strlen(inBuf));
        }

        return 0;
    } else {
        return -1;
    }

}

namespace xcread
{

	void StartupSituationName(unsigned char* target) {
		char buf[] = "FlightGear Situation Name";
		XCCopyMemory(target, &buf);
	}

  void GroundElev(unsigned char* target)
  {	// Ground altitude meters x 256

	  float agl = GetFloat("sim/flightmodel/position/y_agl");
	  float msl = GetFloat("sim/flightmodel/position/elevation");

	  int32_t ga = (int32_t)((msl - agl) * 256.0);

	  XCCopyMemory(target, &ga);
  }
	
  void HoursLocal(unsigned char* target)// Updated for FlightGear
  {	// Hour local

	  //float sec = GetFloat("sim/time/local_time_sec");
	  //int iSec = (int)(sec / 3600);
      int iSec = GetInt("/sim/time/real/second");
	  char cSec = (char)iSec;
	  XCCopyMemory(target, &cSec);
  }

  void MinsLocal(unsigned char* target)  // Updated for FlightGear
  {	// minute
	  //float sec = GetFloat("sim/time/local_time_sec");
	  //int iHour = (int)(sec / 3600);
	  //sec -= iHour * 3600;
	  //int min = (int)(sec / 60);
      int min = GetInt("/sim/time/real/minute");
	  char cMin = (char)min;

	  XCCopyMemory(target, &cMin);
  }

  void SecsLocal(unsigned char* target) // Updated for FlightGear
  {	// second
	  //float sec = GetFloat("sim/time/local_time_sec");
	  //int iHour = (int)(sec / 3600);
	  //sec -= iHour * 3600;
	  //int min = (int)(sec / 60);
	  //sec -= min * 60;
      int sec = GetInt("/sim/time/real/second");
	  char cSec = (char)sec;

	  XCCopyMemory(target, &cSec);
  }

  void HoursZulu(unsigned char* target) // Updated for FlightGear
  {	// Hour zulu

	  //fprintf_s(str, "Read hour\n");

		
	  //float sec = GetFloat("sim/time/zulu_time_sec");

	  //fprintf_s(str, "Hour: %f\n", sec);

	  //int iSec = (int)(sec / 3600);
      int iSec = GetInt("/sim/time/utc/hour"); 
	  char cSec = (char)iSec;

	  //fprintf_s(str, "Hour: %d\n", iSec);

	  XCCopyMemory(target, &cSec);
  }

  void MinutesZulu(unsigned char* target)  // Updated for FlightGear
  {	// minute zulu
	  //float sec = GetFloat("sim/time/zulu_time_sec");
	  //int iHour = (int)(sec / 3600);
	  //sec -= iHour * 3600;
	  //int min = (int)(sec / 60);
      int min = GetInt("/sim/time/utc/second"); // FlightGear path
	  char cMin = (char)min;

	  XCCopyMemory(target, &cMin);
  }

  void DayOfYear(unsigned char* target)// Updated for FlightGear
  {	// day in year

	  int day = GetInt("/sim/time/real/day");  // FlightGear path
	  int16_t sDay = (int16_t)day;
	  XCCopyMemory(target, &sDay);
  }

  void Paused(unsigned char* target)  // Updated for FlightGear
  {   // Paused
	  int pause = GetInt("/sim/freeze/clock");  // This will return "true" or "false"
	  int16_t sPause = (int16_t)pause;
	  XCCopyMemory(target, &sPause);
  }

  void MagVar(unsigned char* target)
  {   // mag var
	  float magvar = GetFloat("sim/flightmodel/position/magnetic_variation") / 360.0f * 65536;
	  int16_t sMagVar = -(int16_t)magvar;
	  XCCopyMemory(target, &sMagVar);
  }

  void GS(unsigned char* target)
  {	// Ground speed

	  float gs = GetFloat("sim/flightmodel/position/groundspeed") * 65536.0f;
	  uint32_t iGS = (uint32_t)gs;

	  XCCopyMemory(target, &iGS);

  }

  void TAS(unsigned char* target)
  {	// TAS

	  float ias = MetersToKnot(GetFloat("sim/flightmodel/position/true_airspeed") * 128.0f);

	  //fprintf_s(str, "TAS: %e\n", ias);
	  uint32_t iIAS = (uint32_t)ias;

	  XCCopyMemory(target, &iIAS);
  }

  void IAS(unsigned char* target)
  {	// IAS

	  float ias = GetFloat("/velocities/airspeed-kt") * 128.0f;
	  int32_t iIAS = (int32_t)ias;

	  XCCopyMemory(target, &iIAS);
  }

  void VS(unsigned char* target)
  {	// vertical speed
	  float vs = GetFloat("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	  vs = vs / 60.0f / 3.28084f * 256;
	  int32_t iVs = (int32_t)vs;
	  XCCopyMemory(target, &iVs);
  }	

  void WhiskeyCompass(unsigned char* target) {
	  int64_t wetcompass = 0;
	  XCCopyMemory(target, &wetcompass);
  }

  void ADF2Freq(unsigned char* target)
  {	// adf2 freq
	  int freq = GetInt("sim/cockpit/radios/adf2_freq_hz");
		
	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void Nav2Info(unsigned char* target)
  {   // nav2 identity
	  //int freq = GetInt("sim/cockpit/radios/adf2_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 2);

	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //CopyMemory(target, buf, 6);

  }

  void QNH(unsigned char* target)
  {	// altimeter
	  float altimeter = GetFloat("sim/cockpit/misc/barometer_setting");
	  int16_t sAlt = ((int16_t)InchToHPA(altimeter)) * 16;
	  XCCopyMemory(target, &sAlt);
  }

  void Transponder(unsigned char* target)
  {
	  int16_t xpdr = GetInt("sim/cockpit/radios/transponder_code");
	  int16_t bcd = GetBCD(xpdr);
	  XCCopyMemory(target, &xpdr);
  }

  void OnGround(unsigned char* target)
  {
	  float agl = GetFloat("sim/flightmodel/position/y_agl");
	  int16_t onground = 0;
	  if(agl<4)
	  {
		  onground = 1;
	  }
	  XCCopyMemory(target, &onground);
  }

  void ADF1Freq(unsigned char* target)
  {	// adf1 freq
	  int freq = GetInt("sim/cockpit/radios/adf1_freq_hz");
		
	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void COM1Freq(unsigned char* target)
  {	// com1 freq
	  int freq = GetInt("sim/cockpit/radios/com1_freq_hz") - 10000;
		
	  //fprintf_s(str, "Nav 1 freq: %d\n", freq);

	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void NAV1Freq(unsigned char* target)
  {	// nav1 freq
	  int freq = GetInt("sim/cockpit/radios/nav1_freq_hz") - 10000;
		
	  //fprintf_s(str, "Nav 1 freq: %d\n", freq);

	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void NAV2Freq(unsigned char* target)
  {	// nav2 freq
	  int freq = GetInt("sim/cockpit/radios/nav2_freq_hz") - 10000;
		
	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void LatitudeHi(unsigned char* target)
  {
	  double latitude = GetDouble("sim/flightmodel/position/latitude");

	  latitude = latitude * (10001750.0 * 65536.0 * 65536.0) / 90.0;

	  int64_t lLat = (int64_t) latitude;

	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0560;

	  uint32_t dLo = ((uint32_t*)&lLat)[1];
	  uint32_t dHi = ((uint32_t*)&lLat)[0];

	  XCCopyMemory(lTarget, &lLat);

	  XCCopyMemory(target, &dHi);
  }

  void LatitudeLo(unsigned char* target)
  {
	  double latitude = GetDouble("sim/flightmodel/position/latitude");

	  latitude = latitude * (10001750.0 * 65536.0 * 65536.0) / 90.0;

	  int64_t lLat = (int64_t) latitude;

	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0560;

	  uint32_t dLo = ((uint32_t*)&lLat)[1];
	  uint32_t dHi = ((uint32_t*)&lLat)[0];

	  XCCopyMemory(lTarget, &lLat);

	  XCCopyMemory(target, &dLo);
  }

  void LongitudeHi(unsigned char* target)
  {
	  double longitude = GetDouble("sim/flightmodel/position/longitude");

	  longitude = longitude * (65536.0 * 65536.0 * 65536.0 * 65536.0) / 360.0;

	  int64_t lLon = (int64_t)longitude;
		
	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0568;

	  uint32_t dLo = ((uint32_t*)&lLon)[1];
	  uint32_t dHi = ((uint32_t*)&lLon)[0];

	  XCCopyMemory(lTarget, &lLon);

	  XCCopyMemory(target, &dHi);
  }

  void LongitudeLo(unsigned char* target)
  {
	  double longitude = GetDouble("sim/flightmodel/position/longitude");

	  longitude = longitude * (65536.0 * 65536.0 * 65536.0 * 65536.0) / 360.0;

	  int64_t lLon = (int64_t)longitude;
		
	  BYTE* lTarget = XConnectMemBlock;
	  lTarget += 0x0568;

	  uint32_t dLo = ((uint32_t*)&lLon)[1];
	  uint32_t dHi = ((uint32_t*)&lLon)[0];

	  XCCopyMemory(lTarget, &lLon);

	  XCCopyMemory(target, &dLo);
  }

  void Altitude(unsigned char* target)
  {
	  double elevation = GetDouble("sim/flightmodel/position/elevation");

	  int64_t lAlt = (int64_t)(elevation * 65536.0 * 65536.0);

	  XCCopyMemory(target, &lAlt);
  }

  void Pitch(unsigned char* target)
  {	// pitch
	  float pitch = GetFloat("sim/flightmodel/position/theta");
	  pitch = pitch / 360.0f * (65536.0f * 65536.0f);
	  int32_t iPitch = (int32_t)-pitch;
	  XCCopyMemory(target, &iPitch);
  }

  void Roll(unsigned char* target)
  {	// bank
	  float bank = GetFloat("sim/flightmodel/position/phi");
	  bank = bank / 360.0f * (65536.0f * 65536.0f);
	  int32_t iBank = (int32_t)-bank;
	  XCCopyMemory(target, &iBank);
  }

  void Heading(unsigned char* target)
  {	// heading
	  double hdg = (double)GetFloat("sim/flightmodel/position/psi");

	  hdg = hdg / 360.0 * (65536.0 * 65536.0);

	  //fprintf_s(str, "Heading: %e\n", hdg);

	  uint32_t iHdg = (uint32_t)hdg;

	  //fprintf_s(str, "Heading int: %d\n", iHdg);
	  XCCopyMemory(target, &iHdg);
  }

  void NAV2RwyHdg(unsigned char* target)
  {	// runway heading nav2
	  int16_t crs = (int16_t)(WrapHeading(GetFloat("sim/cockpit/radios/nav2_course_degm") + 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &crs);
  }

  void NAV2Latitude(unsigned char* target)
  {
	  //int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //int32_t lat = (int32_t)(latitude / 90.0 * 10001750.0);

	  //XCCopyMemory(target, &lat);
  }

  void NAV1Latitude(unsigned char* target)
  {
	  //int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //int32_t lat = (int32_t)(latitude / 90.0 * 10001750.0);

	  //XCCopyMemory(target, &lat);
  }

  void NAV2Longitude(unsigned char* target)
  {
	  //int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //int32_t lon = (int32_t)(longitude / 360.0 * (65536.0 * 65536.0));

	  //XCCopyMemory(target, &lon);
  }

  void NAV1Longitude(unsigned char* target)
  {
	  //int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //int32_t lon = (int32_t)(longitude / 360.0 * (65536.0 * 65536.0));

	  //XCCopyMemory(target, &lon);
  }

  void NAV1RwyHdg(unsigned char* target)
  {	// runway heading nav1

	  int16_t crs = (int16_t)(WrapHeading(GetFloat("sim/cockpit/radios/nav1_course_degm") + 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &crs);
  }

  void CenterFuelLevel(unsigned char* target)
  {

	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 1);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 1);

	  float proc = weightkg / (ratio * total);
	  int32_t iProc = (int32_t)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);
  }

  void CenterFuelCapacity(unsigned char* target)
  {

	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 1);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int32_t cap = (int32_t)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void LeftFuelLevel(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 0);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 0);

	  float proc = weightkg / (ratio * total);
	  int32_t iProc = (int32_t)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);

	  //fprintf_s(str, "Fuel total weight: %e\n", total);
  }

  void LeftFuelCapacity(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 0);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int32_t cap = (int32_t)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void RightFuelLevel(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 2);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  float weightkg = GetFloat("sim/flightmodel/weight/m_fuel", 2);

	  float proc = weightkg / (ratio * total);
	  int32_t iProc = (int32_t)(proc * 128.0 * 65536.0);

	  XCCopyMemory(target, &iProc);
  }

  void RightFuelCapacity(unsigned char* target)
  {
	  float ratio = GetFloat("sim/aircraft/overflow/acf_tank_rat", 2);
	  float total = GetFloat("sim/aircraft/weight/acf_m_fuel_tot");
	  int32_t cap = (int32_t)KilogramsToGallons(ratio * total);

	  XCCopyMemory(target, &cap);
  }

  void PitchInput(unsigned char* target)
  {
	  int16_t ratio = (int16_t)(GetFloat("sim/joystick/yoke_pitch_ratio") * 16383.0);
	  XCCopyMemory(target, &ratio);
  }

  void RollInput(unsigned char* target)
  {
	  int16_t ratio = (int16_t)(GetFloat("sim/joystick/yoke_roll_ratio") * 16383.0);
	  XCCopyMemory(target, &ratio);
  }

  void DME1(unsigned char* target)
  {
	  float dist = GetFloat("sim/cockpit2/radios/indicators/nav1_dme_distance_nm");
	  char dme[5];
	  if(dist>99.9)
	  {
		  sprintf_s(dme, "%.0f", dist);
	  }
	  else
	  {
		  sprintf_s(dme, "%00.1f", dist);
	  }
	  dme[4] = 0;
	  CopyMemory(target, dme, 5);
  }

  void DME2(unsigned char* target)
  {
	  float dist = GetFloat("sim/cockpit2/radios/indicators/nav2_dme_distance_nm");
	  char dme[5];
	  if(dist>99.9)
	  {
		  sprintf_s(dme, "%.0f", dist);
	  }
	  else
	  {
		  sprintf_s(dme, "%00.1f", dist);
	  }
	  dme[4] = 0;
	  CopyMemory(target, dme, 5);
  }

  void NAV1Deflection(unsigned char* target)
  {
	  char loc = (char)(GetFloat("sim/cockpit2/radios/indicators/nav1_hdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &loc);
  }

  void NAV1GS(unsigned char* target)
  {
	  char gs = (char)(GetFloat("sim/cockpit2/radios/indicators/nav1_vdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &gs);
  }

  void NAV1FromTo(unsigned char* target)
  {
	  char fromto = (char)GetInt("sim/cockpit2/radios/indicators/nav1_flag_from_to_pilot");
	  XCCopyMemory(target, &fromto);
  }

  void NAV1CDI(unsigned char* target)
  {
	  char flag = (char)GetInt("sim/cockpit/radios/nav1_CDI");
	  XCCopyMemory(target, &flag);
  }

  void NAV1OBS(unsigned char* target)
  {
	  float fObs = GetFloat("sim/cockpit/radios/nav1_obs_degm");
	  //fprintf_s(str, "Nav 1 obs: %f\n", fObs);

	  int16_t obs = (int16_t)fObs;
	  XCCopyMemory(target, &obs);

	  /*fObs = GetFloat("sim/cockpit/radios/nav1_obs_degt");
	  fprintf_s(str, "sim/cockpit/radios/nav1_obs_degt: %f\n", fObs);
	  fObs = GetFloat("sim/cockpit2/radios/actuators/nav1_obs_deg_mag_pilot");
	  fprintf_s(str, "sim/cockpit2/radios/actuators/nav1_obs_deg_mag_pilot: %f\n", fObs);
	  fObs = GetFloat("sim/cockpit2/radios/actuators/nav1_course_deg_mag_pilot");
	  fprintf_s(str, "sim/cockpit2/radios/actuators/nav1_course_deg_mag_pilot: %f\n", fObs);*/

  }

  void NAV1Radial(unsigned char* target)
  {	// nav1 radial
	  int16_t bgn = (int16_t)(WrapHeading(GetFloat("sim/cockpit2/radios/indicators/nav1_bearing_deg_mag") - 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &bgn);
  }

  void NAV2Deflection(unsigned char* target)
  {
	  char loc = (char)(GetFloat("sim/cockpit2/radios/indicators/nav2_hdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &loc);
  }

  void NAV2FromTo(unsigned char* target)
  {
	  char fromto = (char)GetInt("sim/cockpit2/radios/indicators/nav2_flag_from_to_pilot");
	  XCCopyMemory(target, &fromto);
  }

  void NAV2OBS(unsigned char* target)
  {
	  int16_t obs = (int16_t)GetFloat("sim/cockpit/radios/nav2_obs_degm");
	  XCCopyMemory(target, &obs);
  }

  void NAV2Radial(unsigned char* target)
  {	// nav1 radial
	  int16_t bgn = (int16_t)(WrapHeading(GetFloat("sim/cockpit2/radios/indicators/nav2_bearing_deg_mag") - 180) / 360.0 * 65536.0);
	  XCCopyMemory(target, &bgn);
  }

  void NAV2GS(unsigned char* target)
  {
	  char gs = (char)(GetFloat("sim/cockpit2/radios/indicators/nav2_vdef_dots_pilot") / 2.5 * 127);
	  XCCopyMemory(target, &gs);
  }

  void NAV2CDI(unsigned char* target)
  {
	  char flag = (char)GetInt("sim/cockpit/radios/nav2_CDI");
	  XCCopyMemory(target, &flag);
  }

  void LightSwitchBits(unsigned char* target) {
	  // Bit - Name
	  //  0		Navigation
	  //  1		Beacon
	  //  2		Landing
	  //  3		Taxi
	  //  4		Strobes
	  //  5		Instruments
	  //  6		Recognition
	  //  7		Wing
	  //  8     Logo
	  //  9		Cabin
	  int16_t switch_bits = 0;
	  XCCopyMemory(target, &switch_bits);

  }

  void Temp(unsigned char* target)
  {
	  int16_t oat = (int16_t)(GetFloat("sim/weatcher/temperature_ambient_c") * 256.0);
	  XCCopyMemory(target, &oat);
  }

  void WindSpeed(unsigned char* target)
  {	// wind speed
	  int16_t wsp = (int16_t)GetFloat("sim/weather/wind_speed_kt");
	  XCCopyMemory(target, &wsp);
  }

  void WindDir(unsigned char* target)
  {	// wind direction
	  int16_t wdir = (int16_t)(GetFloat("sim/weather/wind_direction_degt") / 360.0 * 65536);
	  XCCopyMemory(target, &wdir);
  }

  void Eng1N1(unsigned char* target)
  {
	  double n1 = (double)GetFloat("sim/cockpit2/engine/indicators/N1_percent", 0);
	  XCCopyMemory(target, &n1);
  }

  void Eng1N2(unsigned char* target)
  {
	  double n2 = (double)GetFloat("sim/cockpit2/engine/indicators/N2_percent", 0);
	  XCCopyMemory(target, &n2);
  }

  void Eng1Throttle(unsigned char* target)
  {
	  int16_t thr = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_thro", 0) * 16384.0);
	  XCCopyMemory(target, &thr);




  }

  void Eng1Temp(unsigned char* target)
  {
	  int16_t egt = (int16_t)(GetFloat("sim/cockpit2/engine/indicators/EGT_deg_C", 0) / 860.0 * 16384.0);
	  XCCopyMemory(target, &egt);
  }

  void Eng1OilPress(unsigned char* target)
  {
	  int16_t oilp = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_oil_press_psi", 0) / 55.0 * 16384.0);
	  XCCopyMemory(target, &oilp);
  }

  void Eng1OilTemp(unsigned char* target)
  {
	  int16_t oilt = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_oil_temp_c", 0) / 140.0 * 16384.0);
	  XCCopyMemory(target, &oilt);
  }

  void Eng1OilQty(unsigned char* target)
  {
	  int32_t oilq = (int32_t)(GetFloat("sim/cockpit2/engine/indicators/oil_quantity_ratio", 0) * 16384.0);
	  XCCopyMemory(target, &oilq);
  }

  void Eng1HydraulicQty(unsigned char* target)
  {
	  int32_t hydq = (int32_t)(GetFloat("sim/cockpit/misc/hydraulic_quantity") / 100.0 * 16384.0);
	  XCCopyMemory(target, &hydq);
  }

  void Eng1FuelFlow(unsigned char* target)
  {
	  double ff = (double)(GetFloat("sim/flightmodel/engine/ENGN_FF_", 0) * 3600 / 0.45359237);
	  XCCopyMemory(target, &ff);
  }

  void Eng2Throttle(unsigned char* target)
  {
	  int16_t thr = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_thro", 1) * 16384.0);
	  XCCopyMemory(target, &thr);
  }

  void Flaps1(unsigned char* target)
  {
	  int32_t flappos = (int32_t)(GetFloat("sim/flightmodel2/controls/flap1_deploy_ratio") * 16383.0);
	  XCCopyMemory(target, &flappos);
  }

  void Flaps2(unsigned char* target)
  {
	  int32_t flappos = (int32_t)(GetFloat("sim/flightmodel2/controls/flap2_deploy_ratio") * 16383.0);
	  XCCopyMemory(target, &flappos);
  }

  void MachSpeed(unsigned char* target)
  {
	  int16_t mach = (int16_t)(GetFloat("sim/flightmodel/misc/machno") * 20480.0);
	  XCCopyMemory(target, &mach);
  }

  void TAT(unsigned char* target)
  {
	  float oat = GetFloat("sim/weatcher/temperature_ambient_c");
	  float mach = GetFloat("sim/flightmodel/misc/machno");

	  float tat = oat * (1 + 0.2f * mach * mach) * 256.0f;
	  int16_t sTat = (int16_t)tat;
	  XCCopyMemory(target, &sTat);
  }

  void Eng2N1(unsigned char* target)
  {
	  double n1 = (double)GetFloat("sim/cockpit2/engine/indicators/N1_percent", 1);
	  XCCopyMemory(target, &n1);
  }

  void Eng2N2(unsigned char* target)
  {
	  double n2 = (double)GetFloat("sim/cockpit2/engine/indicators/N2_percent", 1);
	  XCCopyMemory(target, &n2);
  }

  void Eng2Temp(unsigned char* target)
  {
	  int16_t egt = (int16_t)(GetFloat("sim/cockpit2/engine/indicators/EGT_deg_C", 0) / 860.0 * 16384.0);
	  XCCopyMemory(target, &egt);
  }

  void ButtonSlotsAvailable(unsigned char* target) {
	  // this is hard coded by FSUIPC to 56.
		int32_t button_slots = 56;
		XCCopyMemory(target, &button_slots);
  }

  void MasterAvionicsSwitch(unsigned char* target) {
	  // 0 for off, 1 for on
	  int32_t mas = 1;
	  XCCopyMemory(target, &mas);
  }


  void Eng2OilPress(unsigned char* target)
  {
	  int16_t oilp = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_oil_press_psi", 1) / 55.0 * 16384.0);
	  XCCopyMemory(target, &oilp);
  }

  void Eng2OilTemp(unsigned char* target)
  {
	  int16_t oilt = (int16_t)(GetFloat("sim/flightmodel/engine/ENGN_oil_temp_c", 1) / 140.0 * 16384.0);
	  XCCopyMemory(target, &oilt);
  }

  void Eng2OilQty(unsigned char* target)
  {
	  int32_t oilq = (int32_t)(GetFloat("sim/cockpit2/engine/indicators/oil_quantity_ratio", 1) * 16384.0);
	  XCCopyMemory(target, &oilq);
  }

  void Eng2HydraulicQty(unsigned char* target)
  {
	  int32_t hydq = (int32_t)(GetFloat("sim/cockpit/misc/hydraulic_quantity2") / 100.0 * 16384.0);
	  XCCopyMemory(target, &hydq);
  }

  void Eng2FuelFlow(unsigned char* target)
  {
	  double ff = (double)(GetFloat("sim/flightmodel/engine/ENGN_FF_", 0) * 3600 / 0.45359237);
	  XCCopyMemory(target, &ff);
  }

  void NAV1Info(unsigned char* target)
  {   // nav1 identity

	  //int freq = GetInt("sim/cockpit/radios/nav1_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);


	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //CopyMemory(target, buf, 6);

  }

  void NAV2Info(unsigned char* target)
  {   // nav2 identity
	  //int freq = GetInt("sim/cockpit/radios/nav2_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 1036 + 16 + 32);

	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //CopyMemory(target, buf, 6);

  }

  void ADF1Info(unsigned char* target)
  {   // nav2 identity
	  //int freq = GetInt("sim/cockpit/radios/adf1_freq_hz");
	  //float longitude = (float)GetDouble("sim/flightmodel/position/longitude");
	  //float latitude = (float)GetDouble("sim/flightmodel/position/latitude");

	  ///*fprintf_s(str, "Freq: %d\n", freq);
	  //fprintf_s(str, "Lat: %f\n", latitude);
	  //fprintf_s(str, "Lon: %f\n", longitude);*/

	  //XPLMNavRef ref = XPLMFindNavAid(NULL, NULL, &latitude, &longitude, &freq, 2);

	  //char buf[6];
	  //for(int i = 0;i<6;i++)
		 // buf[i] = 0;


	  //if(ref != XPLM_NAV_NOT_FOUND)
	  //{
		 // XPLMGetNavAidInfo(ref, 
			//				  NULL,	// type
			//				  &latitude,	// lat
			//				  &longitude,	// lon
			//				  NULL,	// height
			//				  &freq,	// freq
			//				  NULL,	// heading
			//				  buf,	// ID
			//				  NULL,	// name
			//				  NULL);	// reg
	  //}

	  //CopyMemory(target, buf, 6);

  }

  void COM2Freq(unsigned char* target)
  {	// com2 freq
	  int freq = GetInt("sim/cockpit/radios/com2_freq_hz") - 10000;
		
	  //fprintf_s(str, "Nav 1 freq: %d\n", freq);

	  int16_t res = (int16_t)GetBCD(freq);
	  XCCopyMemory(target, &res);
  }

  void AircraftType(unsigned char* target) {
    char buf[24];
    if (GetString(&buf[0], "/sim/description", 24) == 0) {
	    XCCopyMemory(target, &buf);
    } 
  }

  void HeightAGL(unsigned char* target)
  {	// ground altitude
	  int32_t agl = (int32_t)(GetFloat("sim/flightmodel/position/y_agl") * 65536.0);
	  XCCopyMemory(target, &agl);
  }

  void HotkeySlotsAvailable(unsigned char* target) {
	  // this is hard coded by FSUIPC to 56.
		int32_t hotkey_slots = 56;
		XCCopyMemory(target, &hotkey_slots);
  }

  void RadioFlags(unsigned char* target)
  {	// radio status flags
		
	  bool goodnav1 = GetInt("sim/cockpit/radios/nav_type", 0) != 0; 
	  bool goodnav2 = GetInt("sim/cockpit/radios/nav_type", 1) != 0;
	  bool goodadf1 = GetInt("sim/cockpit/radios/nav_type", 2) != 0;
	  bool goodadf2 = GetInt("sim/cockpit/radios/nav_type", 3) != 0;
	  bool nav1isils = GetInt("sim/cockpit/radios/nav_type", 0) == 4;
	  bool nav2isils = GetInt("sim/cockpit/radios/nav_type", 1) == 4;
	  bool nav1hasdme = GetInt("sim/cockpit/radios/nav1_has_dme", 1) != 0;
	  bool nav2hasdme = GetInt("sim/cockpit/radios/nav2_has_dme", 1) != 0;

	  int16_t ret = 0;

	  if(goodnav1)
		  ret += 2;
	  if(goodnav2)
		  ret += 4;
	  if(goodadf1)
		  ret += 8;
	  if(nav1hasdme)
		  ret += 16;
	  if(nav2hasdme)
		  ret += 32;
	  if(nav1isils)
		  ret += 64;
	  if(goodadf2)
		  ret += 2048;
	  if(nav2isils)
		  ret += 4096;

	  XCCopyMemory(target, &ret);
  }

  void XConnectVersion(unsigned char* target)
  {	// XConnect Version
	  int32_t version = 0x20000000;
	  XCCopyMemory(target, &version);
  }

  void SimulatorVersion(unsigned char* target) {	
	  // this is what simulator version we're masquerading as.
	  // the last four bytes in the version variable dictate what we're pretending to be.
	  // 0001 - FS98
	  // 0002 - FS2000
	  // 0003 - Combat Flight Simulator 2
	  // 0004 - Combat Flight Simulator 1
	  // 0005 - Reserved
	  // 0006 - FS2002
	  // 0007 - FS2004, "A Century of Flight"
	  // 0008 - FSX
	  // 0009 - ESP (Prepar3D?)

	  int32_t version = 0xFADE0008; // We're FSX!  Really!
	  XCCopyMemory(target, &version);
  }

  void PressureAltitude(unsigned char* target)
  {   // Barometric altitude
	  int32_t alt = (int32_t)GetFloat("sim/flightmodel/misc/h_ind2");
	  XCCopyMemory(target, &alt);
  }

  void Eng1ThrottleInput(unsigned char* target)
  {
	  int16_t ratio = (int16_t)(GetFloat("sim/cockpit2/engine/actuators/throttle_ratio",0) * 16384.0);
	  XCCopyMemory(target, &ratio);
  }

  void Eng2ThrottleInput(unsigned char* target)
  {
	  int16_t ratio = (int16_t)(GetFloat("sim/cockpit2/engine/actuators/throttle_ratio",1) * 16384.0);
	  XCCopyMemory(target, &ratio);
  }

  void StandbyQNH(unsigned char* target)
  {
	  float altimeter = GetFloat("sim/cockpit/misc/barometer_setting2");
	  int16_t sAlt = ((int16_t)InchToHPA(altimeter)) * 16;
	  XCCopyMemory(target, &sAlt);
  }

  void StandbyAltitude(unsigned char* target)
  {
	  int32_t alt = (int32_t)GetFloat("sim/flightmodel/misc/h_ind_copilot2");
	  XCCopyMemory(target, &alt);
  }

} // namespace xcread