//---------------------------------------------------------------------------
#include "DYNCTLJOB.h"
#include "PROTOCOL.h"
#include "SMEM.h"

#include "WRITEJOB.h"

DYNCTLJOB dynCtlJob;
//---------------------------------------------------------------------------
DYNCTLJOB::DYNCTLJOB(void)
{
//  for(int i = 0; i < MaxOpenUDPPort; i++)
//    _sUDPMappingLCNinREADJOB[i] = smem.vGetUDPMappingLCNData(i);
}
//---------------------------------------------------------------------------
DYNCTLJOB::~DYNCTLJOB(void)
{
}
