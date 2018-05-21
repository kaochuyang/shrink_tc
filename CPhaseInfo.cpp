#include "CPhaseInfo.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t CPhaseInfo::_phase_mutex=PTHREAD_MUTEX_INITIALIZER;

//----------------------------------------------------------
CPhaseInfo::CPhaseInfo(void):_phase_order(0x99), _subphase_count(0), _signal_count(0), _total_step_count(0)
{
}
//----------------------------------------------------------
CPhaseInfo &CPhaseInfo::operator=(const CPhaseInfo &phase_info) // Remember copy assignment operator should never be inlined
{
try{
  if(this!=&phase_info){
//mallocFuck
/*
    if(_ptr_subphase_step_count){  //if(_ptr_subphase_step_count!=NULL), so that this CPhaseInfo is not empty
        if(_subphase_count==0||_signal_count==0) perror("ERROR: CPhaseInfo::operator=\n");
      for(int j=0;j<_subphase_count;j++){
        for(int k=0;k<_ptr_subphase_step_count[j];k++){
          free(_ptr_subphase_step_signal_status[j][k]);
        }
        free(_ptr_subphase_step_signal_status[j]);
      }
      free(_ptr_subphase_step_count);
      free(_ptr_subphase_step_signal_status);
      _phase_order=0x99; _subphase_count=0; _signal_count=0; _total_step_count=0;
    }
*/

    _phase_order      = phase_info._phase_order;
    _signal_map       = phase_info._signal_map;
    _subphase_count   = phase_info._subphase_count;
    _signal_count     = phase_info._signal_count;
    _total_step_count = phase_info._total_step_count;

//mallocFuck    _ptr_subphase_step_count    = (unsigned short int *)  malloc(sizeof(unsigned short int)   *_subphase_count);
//mallocFuck    _ptr_subphase_step_signal_status = (unsigned short int ***)malloc(sizeof(unsigned short int **)*_subphase_count);
    for( int j=0; j<_subphase_count; j++ ){
      _ptr_subphase_step_count[j] = phase_info._ptr_subphase_step_count[j];
//mallocFuck      _ptr_subphase_step_signal_status[j] = (unsigned short int **)malloc(sizeof(unsigned short int*)*_ptr_subphase_step_count[j]);
      for( int k=0; k<_ptr_subphase_step_count[j]; k++ ){
//mallocFuck        _ptr_subphase_step_signal_status[j][k] = (unsigned short int *)malloc(sizeof(unsigned short int)*_signal_count);
        for(int l=0;l<_signal_count;l++)
          _ptr_subphase_step_signal_status[j][k][l] = phase_info._ptr_subphase_step_signal_status[j][k][l];
      }
    }
  }
  return *this;
}
catch(...){ perror("ERROR: CPhaseInfo::operator=\n");  return *this;}
}
//----------------------------------------------------------
