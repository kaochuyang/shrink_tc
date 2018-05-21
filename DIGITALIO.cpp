#include "DIGITALIO.h"
#include "SMEM.h"

#include <stdio.h>
#include <sys/perm.h>
// #include <asm/io.h>
#include <sys/io.h>

#include <stdio.h>

//here select ICOP & ARBOR
DIGITALIO digitalIO;
// DIGITALIO_ICOP6047 digitalIO;
// DIGITALIO_Vortex86SX digitalIO;


//---------------------------------------------------------------------------
DIGITALIO::DIGITALIO()
{
try{
    pthread_mutex_init(&mutexDIO,NULL);
  } catch(...){}
}
//---------------------------------------------------------------------------
DIGITALIO::~DIGITALIO()
{
}
//---------------------------------------------------------------------------
bool DIGITALIO::GetAuthority(unsigned long ioAddressIN, unsigned long ioAddressOUT)
{
try {
  char c;

/*
  unsigned char ucMBType;
  switch(ucMBType) {
    case(MB_ARBOR):
    break;

    case(MB_ICOP6047):
    break;

    case(MB_ICOP6115):
    break;

    default:
    break;
  }
*/

    unsigned char ucMBType;
    unsigned char ucTmp;
    long lTime = 0x20L * 50000L;
    ucMBType = smem.vGetMotherBoardType();

    printf("init Digital getAuthority, MB type is %d\n", ucMBType);

    switch(ucMBType) {
      case(MB_ARBOR):
      DATAIN_PORT=0x200;
      DATAOUT_PORT=0x208;
      break;

      case(MB_ICOP6047):
      DATAIN_PORT = 0x2e;
      DATAOUT_PORT= 0x2f;
      break;

      case(MB_ICOP6115):
//TMDVer      DATAIN_PORT = 0x78;
//TMDVer      DATAOUT_PORT= 0x79;

//ForTC      DATAIN_PORT = 0x79;
      DATAIN_PORT = 0x78;
      DATAOUT_PORT= 0x78;
      break;

      default:
      break;
    }
    haveGetINPower=false;
    haveGetOUTPower=false;

    defaultSignal=0;
    temp=0;
/* add for watchdog */
    iIOPLResult = iopl(3);

    switch(ucMBType) {
      case(MB_ARBOR):
      if (ioperm(ioAddressIN,1,1)==0) {
        DATAIN_PORT=ioAddressIN;
        haveGetINPower=true;                //turn on the flag
        defaultSignal = inb(DATAIN_PORT);
      }
      if (ioperm(ioAddressOUT,1,1)==0) {
        DATAOUT_PORT=ioAddressOUT;
        haveGetOUTPower=true;
        outb(0,DATAOUT_PORT);
        if (haveGetINPower) {
//        return true;
        }
      } else { return false; }

      ioperm(0x444,1,1);
      break;

      case(MB_ICOP6047):
      ioperm(0x84a,1,1);               //set Watchdog ioperm
      ioperm(0x84b,1,1);
      ioperm(0x841,1,1);

      outb_p(5, 0x084a);               //initial Watchdog
      outb_p(0x9c, 0x84b);             //initial Watchdog


      if (ioperm(0x2e ,1,1)==0) {
          haveGetINPower=true;                //turn on the flag
          printf("ioperm(0x2e) ok\n");
      } else { printf("ioperm(0x2e) failure\n"); }
      if (ioperm(0x2f, 1,1)==0) {
          haveGetOUTPower=true;
          printf("ioperm(0x2f) ok\n");

          //test defalut out
          if (haveGetINPower) {
            outb_p(0x87, 0x2e);                                                     //w697uf_unlock_config_reg()
            outb_p(0x87, 0x2e);                                                     //w697uf_unlock_config_reg()

            outb_p(0x2a, 0x2e);                                                     //w697uf_unlock_config_reg()
            outb_p(0x00, 0x2f);                                                     //w697uf_unlock_config_reg()
            outb_p(0x07, 0x2e);                                                     //w697uf_unlock_config_reg()
            outb_p(0x09, 0x2f);                                                     //w697uf_unlock_config_reg()
            outb_p(0x30, 0x2e);                                                     //w697uf_unlock_config_reg()
            outb_p(0x03, 0x2f);                                                     //w697uf_unlock_config_reg()

            outb_p(0xf3, 0x2e);
            outb_p(0xff, 0x2f);                                                     //set gp 8-15 as input
            outb_p(0xf0, 0x2e);
            outb_p(0x00, 0x2f);                                                     //set gp 0-7 as output

            outb_p(0xf1, 0x2e);                                                     //write a empty data
            outb_p(0x00, 0x2f);

//          return true;
          }
      } else  { return false; }

      break;

      case(MB_ICOP6115):
// 50 sec
      ioperm(0x68,1,1);               //set Watchdog ioperm
      ioperm(0x69,1,1);
      ioperm(0x6A,1,1);
      ioperm(0x6B,1,1);
      ioperm(0x6C,1,1);
      ioperm(0x6D,1,1);

      //WDT0
      ioperm(0x22, 1, 1);
      ioperm(0x23, 1, 1);


//GPIO
      ioperm(0x78,1,1);
      ioperm(0x79,1,1);
      ioperm(0x7A,1,1);
      ioperm(0x7B,1,1);
      ioperm(0x7C,1,1);
      ioperm(0x98,1,1);
      ioperm(0x99,1,1);
      ioperm(0x9A,1,1);
      ioperm(0x9B,1,1);
      ioperm(0x9C,1,1);

      ioperm(0x67,1,1);
      //WDT0
        outb_p(0x3b, 0x22);
        outb_p( (lTime >> 16) & 0xff, 0x23);
        outb_p(0x3a, 0x22);
        outb_p( (lTime >>  8) & 0xff, 0x23);
        outb_p(0x39, 0x22);
        outb_p( (lTime >>  0) & 0xff, 0x23);
//Reset system
        outb_p(0x38, 0x22);
        c = inb(0x23);
        c &= 0x0f;
        c |= 0xd0;
        outb_p(0x38, 0x22);
        outb_p(c, 0x23);
//Enable Watchdog
        outb_p(0x37, 0x22);
        c = inb(0x23);
        c |= 0x40;
        outb_p(0x37, 0x22);
        outb_p(c, 0x23);
        outb_p(0x13, 0x22);
        outb_p(0x00, 0x23);
/*------*/


//Set WatchDog
      ucTmp = (lTime >> 16) & 0xff;
      outb_p(ucTmp, 0x6c);

      ucTmp = (lTime >> 8) & 0xff;
      outb_p(ucTmp, 0x6b);

      ucTmp = (lTime >> 0) & 0xff;
      outb_p(ucTmp, 0x6a);

      //Reset system.
      outb_p(0xD0, 0x69);

      //Enable watchdog timer
      ucTmp = inb(0x68);
      ucTmp |= 0x40;
      outb_p(ucTmp, 0x68);

//Set GPIO
//TMDver      outb_p(0x00, 0x98);  //Set GPIO Port0[7-0] as input mode
//TMDver      ucTmp = inb(0x78);   //read port0
//ForTC      outb_p(0xFF, 0x98);  //Set GPIO Port0[7-0] as ioutput mode
//ForTC      outb_p(0xFF, 0x98);  //Set GPIO Port0[7-0] as ioutput mode
      outb_p(0x01, 0x98);  //Set GPIO Port0[0] as ioutput mode, other is  inport

//TMDVer      outb_p(0xFF, 0x99);  //Set GPIO Port1[7-0] as output mode
      outb_p(0x00, 0x99);  //Set GPIO Port1[7-0] as input mode
      ucTmp = inb(0x79);   //read port0

      outb_p(0xFF, 0x9A);  //Set GPIO Port2[7-0] as output mode
      outb_p(0x00, 0x9B);  //Set GPIO Port3[7-0] as input mode
      ucTmp = inb(0x7B);   //read port3

      haveGetINPower=true;                //turn on the flag
      haveGetOUTPower=true;
//      return true;

      break;

      default:
      break;
    }

    WriteDigitalIO(0xFF);
    return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool DIGITALIO::ReleaseAuthority(void)
{
try {

    unsigned char ucMBType;
    ucMBType = smem.vGetMotherBoardType();
    switch(ucMBType) {
      case(MB_ARBOR):
      if (ioperm(DATAIN_PORT,1,0)==0) {       //\uFFFD\uFFFD\uFFFD\uFFFD§å\uFFFDæ¬\uFFFD
      haveGetINPower=false;               //turn off the flag
      }
      if (ioperm(DATAOUT_PORT,1,0)==0) {
          haveGetOUTPower=false;
          if (haveGetINPower==false) return true;
      } else return false;

      break;

      case(MB_ICOP6047):
      if (ioperm(DATAIN_PORT,1,0)==0) {       //\uFFFD\uFFFD\uFFFD\uFFFD§å\uFFFDæ¬\uFFFD
      haveGetINPower=false;               //turn off the flag
      }
      if (ioperm(DATAOUT_PORT,1,0)==0) {
          haveGetOUTPower=false;
          if (haveGetINPower==false) return true;
      } else return false;
      break;

      case(MB_ICOP6115):
      ioperm(0x68,1,0);
      ioperm(0x69,1,0);
      ioperm(0x6A,1,0);
      ioperm(0x6B,1,0);
      ioperm(0x6C,1,0);
      ioperm(0x6D,1,0);

      ioperm(0x78,1,0);
      ioperm(0x79,1,0);
      ioperm(0x7A,1,0);
      ioperm(0x7B,1,0);
      ioperm(0x7C,1,0);
      ioperm(0x98,1,0);
      ioperm(0x99,1,0);
      ioperm(0x9A,1,0);
      ioperm(0x9B,1,0);
      ioperm(0x9C,1,0);
      haveGetINPower=false;               //turn off the flag
      haveGetOUTPower=false;
      break;

      default:
      break;
    }

  } catch(...) {}
}
//---------------------------------------------------------
DATA_Bit DIGITALIO::vReadDigitalIO(void)
{
try {

    unsigned char ucMBType;
    ucMBType = smem.vGetMotherBoardType();
    switch(ucMBType) {
      case(MB_ARBOR):
      if (haveGetINPower) {
          pthread_mutex_lock(&mutexDIO);
          IOByte.DBit=inb(DATAIN_PORT);
          pthread_mutex_unlock(&mutexDIO);
          return IOByte;
      }
      break;

      case(MB_ICOP6047):
      if (haveGetINPower) {
          pthread_mutex_lock(&mutexDIO);
          IOByte.DBit=inb(DATAIN_PORT);
          outb_p(0xf4, 0x2e);
          IOByte.DBit = inb(0x2f);
          pthread_mutex_unlock(&mutexDIO);
          return IOByte;
      }
      break;

      case(MB_ICOP6115):
      if (haveGetINPower) {
          pthread_mutex_lock(&mutexDIO);
          IOByte.DBit=inb(DATAIN_PORT);
          pthread_mutex_unlock(&mutexDIO);
          return IOByte;
      }
      break;

      default:
      break;
    }

    return IOByte;
  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO::WriteDigitalIO(unsigned char CharOUT)
{
try {

    unsigned char ucMBType;
    ucMBType = smem.vGetMotherBoardType();
    switch(ucMBType) {
      case(MB_ARBOR):
      if (haveGetOUTPower) {
          printf("Digital Out:%3X\n",CharOUT);
          pthread_mutex_lock(&mutexDIO);
          outb(CharOUT, DATAOUT_PORT);
          pthread_mutex_unlock(&mutexDIO);
          return true;
      }
      break;

      case(MB_ICOP6047):
      if (haveGetOUTPower) {
          printf("Digital Out:%3X\n",CharOUT);
          pthread_mutex_lock(&mutexDIO);
          outb_p(0xf1, 0x2e);
          outb_p(CharOUT, 0x2f);
          pthread_mutex_unlock(&mutexDIO);
          return true;
      }
      break;

      case(MB_ICOP6115):
      if (haveGetOUTPower) {
          printf("Digital Out:%3X\n",CharOUT);
          pthread_mutex_lock(&mutexDIO);
          outb(CharOUT, DATAOUT_PORT);
          pthread_mutex_unlock(&mutexDIO);
          return true;
      }
      break;

      default:
      break;
    }

    return false;

  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO::vRefresfWatchDog0x444(void)
{
try {
//DebugMsg  printf("iIOPLResult:%d\n", iIOPLResult);

  char c;

  unsigned char ucMBType;
  ucMBType = smem.vGetMotherBoardType();
  switch(ucMBType) {
    case(MB_ARBOR):
    if(iIOPLResult == 0) {
      pthread_mutex_lock(&mutexDIO);
      outb_p(0x70 ,0x444);
//DEBUG      printf("outb_p 0x444 ing\n");
      pthread_mutex_unlock(&mutexDIO);
      return true;
    }

    break;

    case(MB_ICOP6047):
    if(iIOPLResult == 0) {
      pthread_mutex_lock(&mutexDIO);
      outb_p(0xc0, 0x841);
      pthread_mutex_unlock(&mutexDIO);
//DEBUG      printf("outb_p 0x841 ing\n");
      return true;
    }

    break;

    case(MB_ICOP6115):
    if(iIOPLResult == 0) {
      pthread_mutex_lock(&mutexDIO);
      outb_p(0x00, 0x67);
//      printf("Write 0x00, 0x67\n");

      outb_p(0x13, 0x22);
      outb_p(0xc5, 0x23);
      outb_p(0x3c, 0x22);
      c = inb(0x23);
      outb_p(0x3c, 0x22);
      outb_p( c | 0x40, 0x23);
      outb_p(0x13, 0x22);
      outb_p(0x00, 0x23);

      pthread_mutex_unlock(&mutexDIO);
//DEBUG      printf("outb_p 0x67 ing\n");
      return true;
    }
    break;

    default:
    break;
  }

  return false;

  } catch(...) {}
}

//========================================================================
//========================================================================
//DIGITALIO_ICOP6047 Winbond Super I/O
//---------------------------------------------------------------------------
DIGITALIO_ICOP6047::DIGITALIO_ICOP6047()
{
try{
    pthread_mutex_init(&mutexDIO,NULL);

    DATAIN_PORT = 0x2e;
    DATAOUT_PORT= 0x2f;

    haveGetINPower=false;                   //å°\uFFFD\uFFFDå¾\uFFFD\uFFFD\uFFFD§å\uFFFDLPT\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD
    haveGetOUTPower=false;

    defaultSignal=0;
    temp=0;

/* add for watchdog */
    iIOPLResult = iopl(3);

  } catch(...){}
}
//---------------------------------------------------------------------------
DIGITALIO_ICOP6047::~DIGITALIO_ICOP6047()
{
}
//---------------------------------------------------------------------------
bool DIGITALIO_ICOP6047::GetAuthority(unsigned long ioAddressIN, unsigned long ioAddressOUT)
{
try {

    ioperm(0x84a,1,1);               //set Watchdog ioperm
    ioperm(0x84b,1,1);
    ioperm(0x841,1,1);

    outb_p(5, 0x084a);               //initial Watchdog
    outb_p(0x9c, 0x84b);             //initial Watchdog


    if (ioperm(0x2e ,1,1)==0) {
        haveGetINPower=true;                //turn on the flag
        printf("ioperm(0x2e) ok\n");
    } else { printf("ioperm(0x2e) failure\n"); }
    if (ioperm(0x2f, 1,1)==0) {
        haveGetOUTPower=true;
        printf("ioperm(0x2f) ok\n");

        //test defalut out
        if (haveGetINPower) {
          outb_p(0x87, 0x2e);                                                     //w697uf_unlock_config_reg()
          outb_p(0x87, 0x2e);                                                     //w697uf_unlock_config_reg()

          outb_p(0x2a, 0x2e);                                                     //w697uf_unlock_config_reg()
          outb_p(0x00, 0x2f);                                                     //w697uf_unlock_config_reg()
          outb_p(0x07, 0x2e);                                                     //w697uf_unlock_config_reg()
          outb_p(0x09, 0x2f);                                                     //w697uf_unlock_config_reg()
          outb_p(0x30, 0x2e);                                                     //w697uf_unlock_config_reg()
          outb_p(0x03, 0x2f);                                                     //w697uf_unlock_config_reg()

          outb_p(0xf3, 0x2e);
          outb_p(0xff, 0x2f);                                                     //set gp 8-15 as input
          outb_p(0xf0, 0x2e);
          outb_p(0x00, 0x2f);                                                     //set gp 0-7 as output

          outb_p(0xf1, 0x2e);                                                     //write a empty data
          outb_p(0x00, 0x2f);

        return true;
        }
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool DIGITALIO_ICOP6047::ReleaseAuthority(void)
{
try {
    if (ioperm(DATAIN_PORT,1,0)==0) {       //\uFFFD\uFFFD\uFFFD\uFFFD§å\uFFFDæ¬\uFFFD
    haveGetINPower=false;               //turn off the flag
    }
    if (ioperm(DATAOUT_PORT,1,0)==0) {
        haveGetOUTPower=false;
        if (haveGetINPower==false) return true;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------
DATA_Bit DIGITALIO_ICOP6047::vReadDigitalIO(void)
{
try {
    if (haveGetINPower) {
        pthread_mutex_lock(&mutexDIO);
        IOByte.DBit=inb(DATAIN_PORT);
        outb_p(0xf4, 0x2e);
        IOByte.DBit = inb(0x2f);
        pthread_mutex_unlock(&mutexDIO);
        return IOByte;
    }
    return IOByte;
  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO_ICOP6047::WriteDigitalIO(unsigned char CharOUT)
{
try {
    if (haveGetOUTPower) {
        printf("Digital Out:%3X\n",CharOUT);
        pthread_mutex_lock(&mutexDIO);
        outb_p(0xf1, 0x2e);
        outb_p(CharOUT, 0x2f);
        pthread_mutex_unlock(&mutexDIO);
        return true;
    }

    return false;

  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO_ICOP6047::vRefresfWatchDog0x444(void)
{
try {
//DebugMsg  printf("iIOPLResult:%d\n", iIOPLResult);
  if(iIOPLResult == 0) {
    pthread_mutex_lock(&mutexDIO);
    outb_p(0xc0, 0x841);
    pthread_mutex_unlock(&mutexDIO);
    return true;
  }
  return false;

  } catch(...) {}
}


//========================================================================
//========================================================================
//DIGITALIO_ICOP_Vortex86SX
//---------------------------------------------------------------------------
DIGITALIO_Vortex86SX::DIGITALIO_Vortex86SX()
{
try{
    pthread_mutex_init(&mutexDIO,NULL);

    DATAIN_PORT = 0x78;
    DATAOUT_PORT= 0x79;

    haveGetINPower=false;                   //å°\uFFFD\uFFFDå¾\uFFFD\uFFFD\uFFFD§å\uFFFDLPT\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD
    haveGetOUTPower=false;

    defaultSignal=0;
    temp=0;

/* add for watchdog */
    iIOPLResult = iopl(3);

  } catch(...){}
}
//---------------------------------------------------------------------------
DIGITALIO_Vortex86SX::~DIGITALIO_Vortex86SX()
{
}
//---------------------------------------------------------------------------
bool DIGITALIO_Vortex86SX::GetAuthority(unsigned long ioAddressIN, unsigned long ioAddressOUT)
{
try {

    long lTime = 0x20L * 50000L;
    unsigned char ucTmp;

// 50 sec
    ioperm(0x68,1,1);               //set Watchdog ioperm
    ioperm(0x69,1,1);
    ioperm(0x6A,1,1);
    ioperm(0x6B,1,1);
    ioperm(0x6C,1,1);
    ioperm(0x6D,1,1);

//GPIO
    ioperm(0x78,1,1);
    ioperm(0x79,1,1);
    ioperm(0x7A,1,1);
    ioperm(0x7B,1,1);
    ioperm(0x7C,1,1);
    ioperm(0x98,1,1);
    ioperm(0x99,1,1);
    ioperm(0x9A,1,1);
    ioperm(0x9B,1,1);
    ioperm(0x9C,1,1);

//Set WatchDog
    ucTmp = (lTime >> 16) & 0xff;
    outb_p(ucTmp, 0x6c);

    ucTmp = (lTime >> 8) & 0xff;
    outb_p(ucTmp, 0x6b);

    ucTmp = (lTime >> 0) & 0xff;
    outb_p(ucTmp, 0x6a);

    //Reset system.
    outb_p(0xD0, 0x69);

    //Enable watchdog timer
    ucTmp = inb(0x68);
    ucTmp |= 0x40;
    outb_p(ucTmp, 0x68);

//Set GPIO
    outb_p(0x00, 0x98);  //Set GPIO Port0[7-0] as input mode
    ucTmp = inb(0x78);   //read port0

    outb_p(0xFF, 0x99);  //Set GPIO Port1[7-0] as output mode

    outb_p(0xFF, 0x9A);  //Set GPIO Port2[7-0] as output mode
    outb_p(0x00, 0x9B);  //Set GPIO Port3[7-0] as input mode
    ucTmp = inb(0x7B);   //read port3



        haveGetINPower=true;                //turn on the flag
        haveGetOUTPower=true;
        return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool DIGITALIO_Vortex86SX::ReleaseAuthority(void)
{
try {
    ioperm(0x68,1,0);
    ioperm(0x69,1,0);
    ioperm(0x6A,1,0);
    ioperm(0x6B,1,0);
    ioperm(0x6C,1,0);
    ioperm(0x6D,1,0);

    ioperm(0x78,1,0);
    ioperm(0x79,1,0);
    ioperm(0x7A,1,0);
    ioperm(0x7B,1,0);
    ioperm(0x7C,1,0);
    ioperm(0x98,1,0);
    ioperm(0x99,1,0);
    ioperm(0x9A,1,0);
    ioperm(0x9B,1,0);
    ioperm(0x9C,1,0);




    haveGetINPower=false;               //turn off the flag
    haveGetOUTPower=false;
  } catch(...) {}
}
//---------------------------------------------------------
DATA_Bit DIGITALIO_Vortex86SX::vReadDigitalIO(void)
{
try {
    if (haveGetINPower) {
        pthread_mutex_lock(&mutexDIO);
        IOByte.DBit=inb(DATAIN_PORT);
        pthread_mutex_unlock(&mutexDIO);
        return IOByte;
    }
    return IOByte;
  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO_Vortex86SX::WriteDigitalIO(unsigned char CharOUT)
{
try {
    if (haveGetOUTPower) {
        printf("Digital Out:%3X\n",CharOUT);
        pthread_mutex_lock(&mutexDIO);
        outb(CharOUT, DATAOUT_PORT);
        pthread_mutex_unlock(&mutexDIO);
        return true;
    }

    return false;

  } catch(...) {}
}
//---------------------------------------------------------
bool DIGITALIO_Vortex86SX::vRefresfWatchDog0x444(void)
{
try {
//DebugMsg  printf("iIOPLResult:%d\n", iIOPLResult);
  if(iIOPLResult == 0) {
    pthread_mutex_lock(&mutexDIO);
    outb_p(0x00, 0x67);
    pthread_mutex_unlock(&mutexDIO);
    return true;
  }
  return false;

  } catch(...) {}
}

