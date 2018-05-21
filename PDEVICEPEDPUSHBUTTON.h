#ifndef PDEVICEPEDPUSHBUTTON_H_INCLUDED
#define PDEVICEPEDPUSHBUTTON_H_INCLUDED
#include "var.h"
class PDEVICEPEDPUSHBUTTON
{
    public:
    PDEVICEPEDPUSHBUTTON();
    ~PDEVICEPEDPUSHBUTTON();
    bool DoWorkViaPDevice(MESSAGEOK);
    bool ProcessEAC6(MESSAGEOK);//EA46回報行人按鈕狀況
    bool ProcessEAD6(MESSAGEOK);//EA46回報交大行人按鈕狀況

    bool SendPEDLightForCHU(unsigned int device,unsigned int);
    private:
    unsigned short int ucSWVarSec[20];
};

#endif // PDEVICEPEDPUSHBUTTON_H_INCLUDED
