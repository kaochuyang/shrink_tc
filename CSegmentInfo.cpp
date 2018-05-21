#include "CSegmentInfo.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t CSegmentInfo::_segment_mutex=PTHREAD_MUTEX_INITIALIZER;
//----------------------------------------------------------
CSegmentInfo::CSegmentInfo(void):_segment_type(1), _segment_count(0)
{
}
//----------------------------------------------------------
CWeekDaySegType::CWeekDaySegType(void):_segment_type(0), _weekday(1)
{
}
//----------------------------------------------------------
CHoliDaySegType::CHoliDaySegType(void):_segment_type(0), _start_year(0), _start_month(0), _start_day(0)
                                                       , _end_year(0)  , _end_month(0)  , _end_day(0)

{
}
//----------------------------------------------------------
CSegmentInfo &CSegmentInfo::operator=(const CSegmentInfo &segment_info)
{
try{
  if(this!=&segment_info){
//mallocFuck
/*
    if(_ptr_seg_exec_time){  //if(_ptr_plan_exec!=NULL), so that this CSegmentInfo is not empty
        if(_segment_count==0) perror("ERROR: CSegmentInfo::operator=\n");
      free(_ptr_seg_exec_time);
      _segment_type=9999; _segment_count=0;
    }
*/
    _segment_type  = segment_info._segment_type;
    _segment_count = segment_info._segment_count;

//mallocFuck    _ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*_segment_count);
    for( int j=0; j<_segment_count; j++ ){
      _ptr_seg_exec_time[j]._hour     = segment_info._ptr_seg_exec_time[j]._hour;
      _ptr_seg_exec_time[j]._minute   = segment_info._ptr_seg_exec_time[j]._minute;
      _ptr_seg_exec_time[j]._planid   = segment_info._ptr_seg_exec_time[j]._planid;
      _ptr_seg_exec_time[j]._cadc_seg = segment_info._ptr_seg_exec_time[j]._cadc_seg;
    }
//Arwen add 110210
    _actSegmentType1  = segment_info._actSegmentType1;
    _actSegmentType2 = segment_info._actSegmentType2;
    for( int j=0; j<_segment_count; j++ ){
      _ptr_seg_exec_time[j]._compensateMode     = segment_info._ptr_seg_exec_time[j]._compensateMode;
      _ptr_seg_exec_time[j]._openRunnig     = segment_info._ptr_seg_exec_time[j]._openRunnig;
      _ptr_seg_exec_time[j]._actMode     = segment_info._ptr_seg_exec_time[j]._actMode;
    }
    //jacky20140418
    for( int j=0; j<_segment_count; j++ ){
        _ptr_seg_exec_time[j]._onoff = segment_info._ptr_seg_exec_time[j]._onoff;
    }

  }
  return *this;
}
catch(...){ perror("ERROR: CSegmentInfo::operator=\n");  return *this;}
}
//----------------------------------------------------------
