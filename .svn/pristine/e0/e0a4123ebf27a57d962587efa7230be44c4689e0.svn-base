#ifndef CSEGMENT_H
#define CSEGMENT_H

#include <pthread.h>

//----------------------------------------------------------
class CWeekDaySegType
{
  public:
    unsigned short int _segment_type;  //SegmentType is 0~7 for weekday
    unsigned short int _weekday;  //weekday is defined as {1-7,11-17} in P_92
                                  //and is re-defined as {0-6,7-13} accordingly
    CWeekDaySegType(void);
};
//----------------------------------------------------------
class CHoliDaySegType
{
  public:
    unsigned short int _segment_type;  //SegmentType is 8~20 for holiday
    unsigned short int _start_year;
    unsigned short int _start_month;
    unsigned short int _start_day;
    unsigned short int _end_year;
    unsigned short int _end_month;
    unsigned short int _end_day;

    CHoliDaySegType(void);
};
//----------------------------------------------------------
struct SSegExecTime             //紀錄時制計畫時間與觸動開關 chou 2013/01/30
{
  unsigned short int _hour;  //amount according to _segment_count
  unsigned short int _minute;  //amount according to _segment_count
  unsigned short int _planid;  //amount according to _segment_count
  unsigned short int _cadc_seg;
  SSegExecTime(void):_cadc_seg(0){}

  //Arwen add 110209 compensate act
  unsigned short int _compensateMode;
  unsigned short int _openRunnig;
  unsigned short int _actMode;

  //jacky20140418
  unsigned short int _onoff;

};
//----------------------------------------------------------
class CSegmentInfo
{
  public:
    unsigned short int _segment_type;
    unsigned short int _segment_count;
//mallocFuck    SSegExecTime *_ptr_seg_exec_time;  //amount according to _segment_countz
    SSegExecTime _ptr_seg_exec_time[32];  //amount according to _segment_countz

    static pthread_mutex_t _segment_mutex;

    CSegmentInfo(void);
    CSegmentInfo &operator=(const CSegmentInfo &segment_info);

//Arwen add 110209 compensate act
    unsigned short int _actSegmentType1;
    unsigned short int _actSegmentType2;
};
//----------------------------------------------------------


#endif
