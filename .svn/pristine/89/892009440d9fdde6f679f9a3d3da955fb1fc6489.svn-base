#ifndef CPHASEINFO_H
#define CPHASEINFO_H

#include <pthread.h>

//----------------------------------------------------------
class CPhaseInfo
{
  public:

    static pthread_mutex_t _phase_mutex;

    unsigned short int _phase_order;
    unsigned short int _signal_map;  //according to direction
    unsigned short int _subphase_count;
    unsigned short int _signal_count;
    unsigned short int _total_step_count;
    unsigned short int calculated_total_step_count(void){
      unsigned short int step_no=0;
      for(unsigned short int i=0;i<_subphase_count;i++) step_no+=_ptr_subphase_step_count[i];
      return step_no;
    }
    unsigned short int step_no_of_all( const unsigned short int &subphase_no
                                     , const unsigned short int &subphase_step_no){
      unsigned short int step_no=0;
      for(unsigned short int i=0;(i<subphase_no)&&(i<_subphase_count);i++) step_no+=_ptr_subphase_step_count[i];
      step_no+=subphase_step_no;
      return step_no;
    }
//mallocFuck    unsigned short int *_ptr_subphase_step_count;  //amount according to _signal_count
//mallocFuck    unsigned short int ***_ptr_subphase_step_signal_status;  //amount according to *_ptr_subphase_step_count
    unsigned short int _ptr_subphase_step_count[8];  //amount according to _signal_count
    unsigned short int _ptr_subphase_step_signal_status[8][5][8];  //amount according to *_ptr_subphase_step_count

    CPhaseInfo(void);
    CPhaseInfo &operator=(const CPhaseInfo &phase_info);
};
//----------------------------------------------------------
#endif
