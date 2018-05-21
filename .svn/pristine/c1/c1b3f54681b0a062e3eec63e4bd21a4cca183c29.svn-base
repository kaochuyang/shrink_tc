#define threshold_E_default_value 62
#define threshold_D_default_value 46
#define threshold_C_default_value 10
#define threshold_B_default_value 6

#include "CTrafficAnalyzer.h"
/*+++++++++++++++++*/
//#include "keypad.h"
/*-----------------*/
#include <stdio.h>

CTrafficAnalyzer::CTrafficAnalyzer(void)
{
  threshold_E_by_signal = threshold_E_default_value;
  threshold_E = threshold_E_default_value;
  threshold_D = threshold_D_default_value;
  threshold_C = threshold_C_default_value;
  threshold_B = threshold_B_default_value;
//  zone_queue_total[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for(int i=0; i< 16; i++)
    zone_queue_total[i] = 0;

}

//---------------------------------------------------------------------------
unsigned char CTrafficAnalyzer::InsertTargetAndCheckTrafficStatus(const CTarget &target, unsigned short int &STCcurrent_step)
{
  static unsigned char traffic_status=TRAFFIC_NOT_AVAILABLE;

  unsigned short int current_zone;

  static long long int zone_queue[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};; // shoule change to #define ooxx

  static unsigned int continuous_CONGESTED = 0;
  static unsigned int continuous_VACANT = 0;
  static unsigned int continuous_FREEFLOW = 0;
  static unsigned int counter = 0; // every 180 sec reset all continuous_ver
  static unsigned int willBeKickZone = 0;

  /* ++width */
  unsigned short int vehicular_conterminous = 0;
  unsigned short int Top_vehicular_conterminous = 0;
  /* --width */


  bool A = false;  // no car
  bool B = false;  // have car status 1
  bool C = false;  // have car status 2
  bool D = false;  // have car status 3
  bool E = false;  // CONGESTED

  /* ++width */
  bool E2 = false;  // CONGESTED
  /* --width */

////OTMARKPRINTF  printf("Down Crossing current step:%d\n", STCcurrent_step);
////OTMARKPRINTF  printf("Down Crossing current step:%d\n", down_crossing_STC.current_step);

  current_zone = blips2zone(target);  // let blips to zone (32bits -> 16bits)

//-----------------------

for(int i = 0; i < 16; i++)  //zone_queue move to left 1 bit
{
  //<<<CCJ modified
  //willBeKickZone = ((zone_queue[i]&0x8000000000000000)==0) ? 0 : 1;  // 0x80 => 10000000 (1byte)
  willBeKickZone = ((zone_queue[i]&0x8000000000000000LL)==0) ? 0 : 1;  // 0x80 => 10000000 (1byte)
  //CCJ modified>>>
  zone_queue[i] = zone_queue[i] << 1;
  zone_queue_total[i] = (zone_queue_total[i] - willBeKickZone);
}

unsigned short int _forShiftTmp = 0x01; //current_zone conv. to  zone_queue   00000001
unsigned char current_zoneAnd_forShiftTmp_Tmp = 0;
for(int i = 0; i < 16; i++)
{

  current_zoneAnd_forShiftTmp_Tmp = ((current_zone&_forShiftTmp)==0) ? 0 : 1;
  zone_queue[i] = zone_queue[i] + current_zoneAnd_forShiftTmp_Tmp;
  zone_queue_total[i] = zone_queue_total[i] + current_zoneAnd_forShiftTmp_Tmp;

  _forShiftTmp = _forShiftTmp << 1;
}

/* ++width */
for(int i = 0; i < 16; i++)
{
if(zone_queue_total[i] > 2) {
  vehicular_conterminous++;
  if( vehicular_conterminous > Top_vehicular_conterminous) {
    Top_vehicular_conterminous = vehicular_conterminous; }
}
else
  vehicular_conterminous = 0;
}
if(Top_vehicular_conterminous > 5)
{
E2 = true;
}
/* --width */


//----------------------------

  int continuousCarGo = 0;
  int continuousNoCar = 0;

  for(int i = 0; i < 16; i++)
  {
    if(zone_queue_total[i] > threshold_E) {
        continuous_CONGESTED++;
        E = true;
//      //OTMARKPRINTF  printf("===EEEEE=== zone_queue_total[%d]: %d\n", i, zone_queue_total[i]);
        continuousCarGo++;  // for status B
    }
    else if(zone_queue_total[i] > threshold_D)
    {
      continuous_FREEFLOW++;
      D = true;
//    //OTMARKPRINTF  printf("===DDDDD=== zone_queue_total[%d]: %d\n", i, zone_queue_total[i]);
      continuousCarGo++;  // for status B
    }
    else if(zone_queue_total[i] > threshold_C)
    {
      continuous_FREEFLOW++;
      C = true;
//    //OTMARKPRINTF  printf("===CCCCC=== zone_queue_total[%d]: %d\n", i, zone_queue_total[i]);
      continuousCarGo++;  // for status B
    }
    else if(zone_queue_total[i] > 1)
    {
      continuousCarGo++;
      if(continuousCarGo > threshold_B) {
        continuous_FREEFLOW++;
        B = true;
//        //OTMARKPRINTF  printf("===BBBBBB=== zone_queue_total[%d]: %d\n", i, zone_queue_total[i]);
      }
    }
    else
    {
      continuousNoCar++;
      if(continuousNoCar == 16) {
        continuous_VACANT++;
        A = true;
//        //OTMARKPRINTF  printf("===AAAAA=== zone_queue_total[%d]: %d\n", i, zone_queue_total[i]);
      }
    }
  }

  counter++;
  if(counter >=1800)
  {
    continuous_FREEFLOW = 0;
    continuous_CONGESTED = 0;
    continuous_VACANT = 0;
    counter = 0;
  //OTMARKPRINTF  printf("counter >= 1800");
  }

  /* debug
////OTMARKPRINTF  printf("current_zone: %x  willBeKickZone:%d  counter:%d \n",current_zone, willBeKickZone, counter);
// for debug
  for(int j = 0; j < 16; j++)
  {
   //OTMARKPRINTF  printf("zone_queue_total[%d]: %d\n", j, zone_queue_total[j]);
  }
 */

//chk traffic status----------------------------------------------------------------
/* ++width */
if(E2 == true && E == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(3);
/*-----------------*/

for(int i = 0; i < 16; i++)
{
  //<<<CCJ modified
  zone_queue[i]= 0xCCCCCCCCCCCC0000LL;
  //CCJ modified>>>
  zone_queue_total[i] = 24;
}
zone_queue[5]= 0x0000000000000000;
zone_queue_total[5] = 0;

return NOT_STABLE_FREEFLOW;
}

/* --width */
else if(E == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(5);
/*-----------------*/
////OTMARKPRINTF  printf("===double EEEEE===");
  if(STCcurrent_step > 0)  // when down crossing's step is not green light
  {
    if ( threshold_E != threshold_D)  //let congested status change slowly
      threshold_E = threshold_D;
    return STABLE_CONGESTED;
  }
}
else if(D == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(4);
/*-----------------*/
////OTMARKPRINTF  printf("===double DDDDDD===");
  if(STCcurrent_step > 4)  // when down crossing's step is not green light
    return NOT_STABLE_FREEFLOW;
}
else if(C == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(3);
/*-----------------*/
////OTMARKPRINTF  printf("===double CCCCC===");
  if(STCcurrent_step > 0)  // when down crossing's step is not green light
  {
    if ( threshold_E != threshold_E_by_signal)
      threshold_E = threshold_E_by_signal;
    return NOT_STABLE_FREEFLOW;
  }
}
else if(B == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(2);
/*-----------------*/
////OTMARKPRINTF  printf("===double BBBBB===");
  if(STCcurrent_step > 0)  // when down crossing's step is not green light
    return NOT_STABLE_FREEFLOW;
}
else if(A == true)
{
/*+++++++++++++++++*/
//  keypad.keypadPort.doChangeVInfoWork(1);
/*-----------------*/
////OTMARKPRINTF  printf("===double AAAAA===");
  if(STCcurrent_step > 0)  // when down crossing's step is not green light
    return STABLE_VACANT;
}
else
  return traffic_status;
}
///chk traffic status-------------------------------------------------------------end

//---------------------------------------------------------------------------
unsigned short int CTrafficAnalyzer::blips2zone(const CTarget &target)
{
  unsigned short int blips_tmp0 = 0;
  unsigned short int blips_tmp1 = 0;
  unsigned short int blips_tmp2 = 0;
  unsigned short int blips_tmp3 = 0;

  unsigned short int _target_zone = 0;
  unsigned int square = 0x8000;  // binary 1000000000000000 for mask

  for(int i=0;i<4;i++)
  {
    blips_tmp0 += ((target._target_packet[i]&0xC0)==0) ? 0 : 1;  // 11000000
    blips_tmp1 += ((target._target_packet[i]&0x30)==0) ? 0 : 1;  // 00110000
    blips_tmp2 += ((target._target_packet[i]&0x0C)==0) ? 0 : 1;  // 00001100
    blips_tmp3 += ((target._target_packet[i]&0x03)==0) ? 0 : 1;  // 00000011

    if(blips_tmp0 > 0){
      _target_zone += square;
      blips_tmp0 = 0;
    }
    square = square >> 1;
    if(blips_tmp1 > 0){
      _target_zone += square;
      blips_tmp1 = 0;
    }
    square = square >> 1;
    if(blips_tmp2 > 0){
      _target_zone += square;
      blips_tmp2 = 0;
    }
    square = square >> 1;
    if(blips_tmp3 > 0){
      _target_zone += square;
      blips_tmp3 = 0;
    }
    square = square >> 1;

  }
  return _target_zone;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void CTrafficAnalyzer::zone2zone_queue(unsigned char current_zone, unsigned int *zone_queue, unsigned int *zone_queue_total)
{

}
