#ifndef CPLANINFO_H
#define CPLANINFO_H

#include <pthread.h>

/*OT990422
#define CRITICAL_SMALL_GREEN           4
#define CRITICAL_SMALL_YELLOW          2
#define CRITICAL_SMALL_ALLRED          2
#define CRITICAL_SMALL_PEDGREEN_FLASH  0
#define CRITICAL_SMALL_PEDRED          0
*/
#define CRITICAL_SMALL_GREEN           0
#define CRITICAL_SMALL_YELLOW          0
#define CRITICAL_SMALL_ALLRED          0
#define CRITICAL_SMALL_PEDGREEN_FLASH  0
#define CRITICAL_SMALL_PEDRED          0


//----------------------------------------------------------
struct SSubPlanInfo
{
  unsigned short int _green;
  unsigned short int _min_green;
  unsigned short int _max_green;
  unsigned short int _yellow;
  unsigned short int _allred;
  unsigned short int _pedgreen_flash;
  unsigned short int _pedred;

   short int _green_compensation;
  unsigned short int _yellow_compensation;
  unsigned short int _allred_compensation;
  short int _pedgreen_flash_compensation;
  short int _pedred_compensation;

  //jacky20140418
  unsigned short int _percentage[2];

  short int compensated_green(const bool & shorten_cycle){
    if(shorten_cycle){
      if((_green-_green_compensation) < _min_green)
            return _min_green;
      else
        return ( ((_green-_green_compensation)>CRITICAL_SMALL_GREEN)?
                (_green-_green_compensation):CRITICAL_SMALL_GREEN );
    }
    else{
      if(_max_green == 0) return (  (_green+_green_compensation) );
      else{
          if((_green+_green_compensation) > _max_green )
            return _max_green;
          else
            return (  (_green+_green_compensation) );
      }
    }
  }
  short int compensated_yellow(const bool & shorten_cycle){
    if(shorten_cycle)
      return ( ((_yellow-_yellow_compensation)>CRITICAL_SMALL_YELLOW)?
                (_yellow-_yellow_compensation):CRITICAL_SMALL_YELLOW );
    else
      return (  (_yellow+_yellow_compensation) );
  }
  short int compensated_allred(const bool & shorten_cycle){
    if(shorten_cycle)
      return ( ((_allred-_allred_compensation)>CRITICAL_SMALL_ALLRED)?
                (_allred-_allred_compensation):CRITICAL_SMALL_ALLRED );
    else
      return (  (_allred+_allred_compensation) );
  }
  short int compensated_pedgreen_flash(const bool & shorten_cycle){
    if(shorten_cycle)
      return ( ((_pedgreen_flash-_pedgreen_flash_compensation)>CRITICAL_SMALL_PEDGREEN_FLASH)?
                (_pedgreen_flash-_pedgreen_flash_compensation):CRITICAL_SMALL_PEDGREEN_FLASH );
    else
      return (  (_pedgreen_flash+_pedgreen_flash_compensation) );
  }
  short int compensated_pedred(const bool & shorten_cycle){
    if(shorten_cycle)
      return ( ((_pedred-_pedred_compensation)>CRITICAL_SMALL_PEDRED)?
                (_pedred-_pedred_compensation):CRITICAL_SMALL_PEDRED );
    else
      return (  (_pedred+_pedred_compensation) );
  }

  short int compensated_phase_time(const bool & shorten_cycle){
    return (   compensated_green(shorten_cycle)
             + compensated_yellow(shorten_cycle)
             + compensated_allred(shorten_cycle)
             + compensated_pedgreen_flash(shorten_cycle)
             + compensated_pedred(shorten_cycle) );
  }

  SSubPlanInfo(void):_green(0), _min_green(CRITICAL_SMALL_GREEN), _max_green(9999),
                     _yellow(0), _allred(0), _pedgreen_flash(0), _pedred(0),
                     _green_compensation(0),
                     _yellow_compensation(0),
                     _allred_compensation(0),
                     _pedgreen_flash_compensation(0),
                     _pedred_compensation(0){}
};

//----------------------------------------------------------
class CPlanInfo
{
  public:
    static pthread_mutex_t _plan_mutex;

    unsigned short int _planid;
    unsigned short int _dir;
    unsigned short int _phase_order;
    unsigned short int _subphase_count;
    unsigned short int _cycle_time;
    const unsigned short int calculated_cycle_time(void){
      unsigned short int sum=0;
      for(int i=0;i< _subphase_count;i++){
        sum += (   _ptr_subplaninfo[i]._green
                 + _ptr_subplaninfo[i]._yellow
                 + _ptr_subplaninfo[i]._allred
                 + _ptr_subplaninfo[i]._pedgreen_flash
                 + _ptr_subplaninfo[i]._pedred );
      }
      return sum;
    }

    const unsigned short int compensated_cycle_time(const bool & shorten_cycle){
      unsigned short int sum=0;
      for(int i=0;i< _subphase_count;i++)
        sum += _ptr_subplaninfo[i].compensated_phase_time(shorten_cycle);
      return sum;
    }

    unsigned short int _offset;
    SSubPlanInfo _ptr_subplaninfo[8];  //amount according to _subphase_coun
    //SSubPlanInfo _ptr_subplaninfo[16];  //amount according to _subphase_coun
    bool _shorten_cycle;

    CPlanInfo(void);
    CPlanInfo &operator=(const CPlanInfo &plan_info);

    //jacky20140418
    unsigned short int _east;
    unsigned short int _west;
    unsigned short int _th1_east;
    unsigned short int _th1_west;
    unsigned short int _th2;
    unsigned short int _time_east;
    unsigned short int _time_west;
    unsigned short int _BF13phasecount;

};
//----------------------------------------------------------
#endif
