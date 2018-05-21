#ifndef CTrafficAnalyzerH
#define CTrafficAnalyzerH

#include "CTarget.h"

#define TRAFFIC_NOT_AVAILABLE 0x00
#define NOT_STABLE_FREEFLOW   0x01
#define STABLE_VACANT         0x10
#define STABLE_CONGESTED      0x11

#define TARGET_BULK_AMOUNT                          10
#define DETERMINE_STABLE_TARGET_THRESHOLD            5
#define PERIODIC_TARGET_BULK_AMOUNT                 10
#define DETERMINE_TRAFFIC_PERIODIC_TARGET_THRESHOLD  3
#define DETERMINE_TRAFFIC_TARGET_AMOUNT_THRESHOLD    1

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class CTrafficAnalyzer
{
  public:

      int threshold_E_by_signal;
      int threshold_E;
      int threshold_D;
      int threshold_C;
      int threshold_B;
      unsigned int zone_queue_total[16];

//    CTrafficAnalyzer(){}
      CTrafficAnalyzer(void);

    //invoked whenever got a RTMS TARGET(0x18) protocol
    unsigned char InsertTargetAndCheckTrafficStatus(const CTarget &target, unsigned short int &STCcurrent_step);

  private:

    unsigned short int blips2zone(const CTarget &target);
    void zone2zone_queue(unsigned char , unsigned int *, unsigned int *);
//    unsigned char chkTrafficStatus(bool, bool, bool, bool, bool, unsigned char, unsigned short int);

};
//---------------------------------------------------------------------------
#endif
