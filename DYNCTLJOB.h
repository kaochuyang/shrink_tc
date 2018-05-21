//---------------------------------------------------------------------------
#ifndef DYNCTLJOBH
#define DYNCTLJOBH

#include "var.h"
#include "JOB.h"
//---------------------------------------------------------------------------
class DYNCTLJOB:public JOB
{
    public:
         DYNCTLJOB(void);
        ~DYNCTLJOB(void);
};
//---------------------------------------------------------------------------
extern DYNCTLJOB dynCtlJob;
#endif
