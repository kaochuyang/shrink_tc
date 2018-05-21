#include "CReverseTime.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t CReverseTimeInfo::_rev_mutex=PTHREAD_MUTEX_INITIALIZER;
//----------------------------------------------------------
CReverseTimeInfo::CReverseTimeInfo(void):
_reverse_time_type(0), usiDirectIn(0), usiHourStartIn(0), usiMinStartIn(0),
usiHourEndIn(0), usiMinEndIn(0), usiDirectOut(0), usiHourStartOut(0),
usiMinStartOut(0), usiHourEndOut(0), usiMinEndOut(0), usiClearTime(0),
usiFlashGreen(0), usiGreenTime(0), ucNonRevLight(0)
{
}
//----------------------------------------------------------
CWeekDayRevType::CWeekDayRevType(void):_reverse_time_type(0), _weekday(1)
{
}
//----------------------------------------------------------
CHoliDayRevType::CHoliDayRevType(void):_reverse_time_type(0), _start_year(0), _start_month(0), _start_day(0)
                                                       , _end_year(0)  , _end_month(0)  , _end_day(0)

{
}
//----------------------------------------------------------
CReverseTimeInfo &CReverseTimeInfo::operator=(const CReverseTimeInfo &reversetime_info)
{
try{
  if(this!=&reversetime_info){
    _reverse_time_type  = reversetime_info._reverse_time_type;

    usiDirectIn     = reversetime_info.usiDirectIn;
    usiHourStartIn  = reversetime_info.usiHourStartIn;
    usiMinStartIn   = reversetime_info.usiMinStartIn;
    usiHourEndIn    = reversetime_info.usiHourEndIn;
    usiMinEndIn     = reversetime_info.usiMinEndIn;

    usiDirectOut    = reversetime_info.usiDirectOut;
    usiHourStartOut = reversetime_info.usiHourStartOut;
    usiMinStartOut  = reversetime_info.usiMinStartOut;
    usiHourEndOut   = reversetime_info.usiHourEndOut;
    usiMinEndOut    = reversetime_info.usiMinEndOut;

    usiClearTime    = reversetime_info.usiClearTime;
    usiFlashGreen   = reversetime_info.usiFlashGreen;
    usiGreenTime    = reversetime_info.usiGreenTime;

    ucNonRevLight   = reversetime_info.ucNonRevLight;

  }
  return *this;
}
catch(...){ perror("ERROR: CSegmentInfo::operator=\n");  return *this;}
}
//----------------------------------------------------------
