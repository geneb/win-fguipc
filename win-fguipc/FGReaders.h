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

#ifndef __XCONNECTLIB__XCREADERS_H__
#define __XCONNECTLIB__XCREADERS_H__

namespace xcread
{
  void StartupSituationName(unsigned char* target);
  void GroundElev(unsigned char* target);
  void HoursLocal(unsigned char* target);
  void MinsLocal(unsigned char* target);
  void SecsLocal(unsigned char* target);
  void HoursZulu(unsigned char* target);
  void MinutesZulu(unsigned char* target);
  void DayOfYear(unsigned char* target);
  void Paused(unsigned char* target);
  void MagVar(unsigned char* target);
  void GS(unsigned char* target);
  void TAS(unsigned char* target);
  void IAS(unsigned char* target);
  void VS(unsigned char* target);
  void WhiskeyCompass(unsigned char* target);
  void ADF2Freq(unsigned char* target);
  void Nav2Info(unsigned char* target);
  void QNH(unsigned char* target);
  void Transponder(unsigned char* target);
  void OnGround(unsigned char* target);
  void ADF1Freq(unsigned char* target);
  void COM1Freq(unsigned char* target);
  void NAV1Freq(unsigned char* target);
  void NAV2Freq(unsigned char* target);
  void LatitudeHi(unsigned char* target);
  void LatitudeLo(unsigned char* target);
  void LongitudeHi(unsigned char* target);
  void LongitudeLo(unsigned char* target);
  void Altitude(unsigned char* target);
  void Pitch(unsigned char* target);
  void Roll(unsigned char* target);
  void Heading(unsigned char* target);
  void NAV2RwyHdg(unsigned char* target);
  void NAV2Latitude(unsigned char* target);
  void NAV1Latitude(unsigned char* target);
  void NAV2Longitude(unsigned char* target);
  void NAV1Longitude(unsigned char* target);
  void NAV1RwyHdg(unsigned char* target);
  void CenterFuelLevel(unsigned char* target);
  void CenterFuelCapacity(unsigned char* target);
  void LeftFuelLevel(unsigned char* target);
  void LeftFuelCapacity(unsigned char* target);
  void RightFuelLevel(unsigned char* target);
  void RightFuelCapacity(unsigned char* target);
  void PitchInput(unsigned char* target);
  void RollInput(unsigned char* target);
  void DME1(unsigned char* target);
  void DME2(unsigned char* target);
  void NAV1Deflection(unsigned char* target);
  void NAV1GS(unsigned char* target);
  void NAV1FromTo(unsigned char* target);
  void NAV1CDI(unsigned char* target);
  void NAV1OBS(unsigned char* target);
  void NAV1Radial(unsigned char* target);
  void NAV2Deflection(unsigned char* target);
  void NAV2FromTo(unsigned char* target);
  void NAV2OBS(unsigned char* target);
  void NAV2Radial(unsigned char* target);
  void NAV2GS(unsigned char* target);
  void NAV2CDI(unsigned char* target);
  void LightSwitchBits(unsigned char* target);
  void Temp(unsigned char* target);
  void WindSpeed(unsigned char* target);
  void WindDir(unsigned char* target);
  void Eng1N1(unsigned char* target);
  void Eng1N2(unsigned char* target);
  void Eng1Throttle(unsigned char* target);
  void Eng1Temp(unsigned char* target);
  void Eng1OilPress(unsigned char* target);
  void Eng1OilTemp(unsigned char* target);
  void Eng1OilQty(unsigned char* target);
  void Eng1HydraulicQty(unsigned char* target);
  void Eng1FuelFlow(unsigned char* target);
  void Eng2Throttle(unsigned char* target);
  void Flaps1(unsigned char* target);
  void Flaps2(unsigned char* target);
  void MachSpeed(unsigned char* target);
  void TAT(unsigned char* target);
  void Eng2N1(unsigned char* target);
  void Eng2N2(unsigned char* target);
  void Eng2Temp(unsigned char* target);
  void ButtonSlotsAvailable(unsigned char* target);
  void MasterAvionicsSwitch(unsigned char* target);
  void Eng2OilPress(unsigned char* target);
  void Eng2OilTemp(unsigned char* target);
  void Eng2OilQty(unsigned char* target);
  void Eng2HydraulicQty(unsigned char* target);
  void Eng2FuelFlow(unsigned char* target);
  void NAV1Info(unsigned char* target);
  void NAV2Info(unsigned char* target);
  void ADF1Info(unsigned char* target);
  void COM2Freq(unsigned char* target);
  void AircraftType(unsigned char* target);
  void HeightAGL(unsigned char* target);
  void HotkeySlotsAvailable(unsigned char* target);
  void RadioFlags(unsigned char* target);
  void XConnectVersion(unsigned char* target);
  void SimulatorVersion(unsigned char* target);
  void PressureAltitude(unsigned char* target);
  void Eng1ThrottleInput(unsigned char* target);
  void Eng2ThrottleInput(unsigned char* target);
  void StandbyQNH(unsigned char* target);
  void StandbyAltitude(unsigned char* target);

} // namespace xcread

#endif // __XCONNECTLIB__XCREADERS_H__