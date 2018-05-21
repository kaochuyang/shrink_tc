#ifndef CREVERSETIME_H
#define CREVERSETIME_H

#include <pthread.h>

//----------------------------------------------------------
class CWeekDayRevType
{
  public:
    unsigned short int _reverse_time_type;  //ReverseTimeType is 0~7 for weekday
    unsigned short int _weekday;  //weekday is defined as {1-7,11-17} in P_92
                                  //and is re-defined as {0-6,7-13} accordingly
    CWeekDayRevType(void);
};
//----------------------------------------------------------
class CHoliDayRevType
{
  public:
    unsigned short int _reverse_time_type;  //SegmentType is 8~20 for holiday
    unsigned short int _start_year;
    unsigned short int _start_month;
    unsigned short int _start_day;
    unsigned short int _end_year;
    unsigned short int _end_month;
    unsigned short int _end_day;

    CHoliDayRevType(void);
};
//----------------------------------------------------------
class CReverseTimeInfo
{
  public:
    unsigned short int _reverse_time_type;

    unsigned short int usiDirectIn;
    unsigned short int usiHourStartIn;
    unsigned short int usiMinStartIn;
    unsigned short int usiHourEndIn;
    unsigned short int usiMinEndIn;

    unsigned short int usiDirectOut;
    unsigned short int usiHourStartOut;
    unsigned short int usiMinStartOut;
    unsigned short int usiHourEndOut;
    unsigned short int usiMinEndOut;

    unsigned short int usiClearTime;
    unsigned short int usiFlashGreen;
    unsigned short int usiGreenTime;

    unsigned char      ucNonRevLight;  // 0:don't light, 1:InCityLight, 2:OutCityLight

    static pthread_mutex_t _rev_mutex;

    CReverseTimeInfo(void);
    CReverseTimeInfo &operator=(const CReverseTimeInfo &reversetime_info);
};
//----------------------------------------------------------


#endif
