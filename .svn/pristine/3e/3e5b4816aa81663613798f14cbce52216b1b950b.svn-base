#include "light.h"
#include <unistd.h>
#include <sys/perm.h>         /*ioperm*/
#include <sys/io.h>         /*outb,inb*/

#define IODELAY 1           /*IO Port Interrupt Time ���iDelay�Ӫ�*/

Light light;
//---------------------------------------------------------------------------
Light::Light (void)
{
    LPTBASEPORT=0x3BC;                    //default IO��m LPT2
    haveGetPower=false;                   //����o�����v
}
//---------------------------------------------------------------------------
Light::~Light (void)
{
}
//---------------------------------------------------------------------------
bool Light::GetAuthority(unsigned long ioAddress)
{
    if (ioperm(ioAddress,3,1)==0) {       //�o�챱���v(LPTBASEPORT+0��Data Port,LPTBASEPORT+1��Status Port,LPTBASEPORT+2��Control Port)
        LPTBASEPORT=ioAddress;            //�o�쥿�TIO��m
        haveGetPower=true;                //turn on the flag
        return true;
    } else return false;
}
//---------------------------------------------------------------------------
bool Light::ReleaseAuthority(void)
{
    if (ioperm(LPTBASEPORT,3,0)==0) {     //�\uFFFD񱱨��v
        haveGetPower=false;               //turn off the flag
        return true;
    } else return false;
}
//---------------------------------------------------------------------------
bool Light::SetTimeOfCard(unsigned char startTime)
{
    if (haveGetPower) {
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);             //�M��Control Port
          usleep(IODELAY);
        outb(0x10,LPTBASEPORT);            //@10���]�w�ɶ���Port
          usleep(IODELAY);
        outb(0x02,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(0x03,LPTBASEPORT+2);          //�eIRQ
          usleep(IODELAY);
        outb(0x02,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(startTime,LPTBASEPORT);       //�]�w���ɶ� 0~255;
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(0x01,LPTBASEPORT+2);          //�eIRQ
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);             //�M��Control Port
        return true;
    } else {
        GetAuthority(LPTBASEPORT);
        return false;
    }
}
//---------------------------------------------------------------------------
/*                                 �O�I����                                */
/*           ��H��  ��H��  �k��   ����   ���\uFFFD  ����   ��    ��          */
/*           LED8    LED7   LED6   LED5   LED4   LED3   LED2  LED1         */
/*                              LightHi | LightLo                          */
//---------------------------------------------------------------------------
bool Light::SetLight(unsigned char address,unsigned char lightLo,unsigned char lightHi)
{
    if (haveGetPower) {
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);             //�M��Control Port
          usleep(IODELAY);
        outb(address,LPTBASEPORT);         //�I�O��m @20 @21 @22 @23 @24 @25
          usleep(IODELAY);
        outb(0x02,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(0x03,LPTBASEPORT+2);          //�eIRQ
          usleep(IODELAY);
        outb(0x02,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(lightLo,LPTBASEPORT);         //LO unsigned char : ����LED 4,3,2,1  0x03���LED1�G 0xC0���LED4�G
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(0x01,LPTBASEPORT+2);          //�eIRQ
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);
          usleep(IODELAY);
        outb(lightHi,LPTBASEPORT);         //HI unsigned char : ����LED 8,7,6,5
          usleep(IODELAY);
        outb(0x01,LPTBASEPORT+2);          //�eIRQ
          usleep(IODELAY);
        outb(0,LPTBASEPORT+2);             //�M��Control Port
        return true;
    } else {
        GetAuthority(LPTBASEPORT);
        return false;
    }
}
//---------------------------------------------------------------------------
bool Light::SetLight(unsigned char *lightBitmap)
{
    if (haveGetPower) {
        SetLight(0x20,lightBitmap[0],lightBitmap[1]);      //LED 4,3,2,1,8,7,6,5
        SetLight(0x21,lightBitmap[2],lightBitmap[3]);      //LoByte,HiByte
        SetLight(0x22,lightBitmap[4],lightBitmap[5]);
        SetLight(0x23,lightBitmap[6],lightBitmap[7]);
        SetLight(0x24,lightBitmap[8],lightBitmap[9]);
        SetLight(0x25,lightBitmap[10],lightBitmap[11]);
        return true;
    }  else {
        GetAuthority(LPTBASEPORT);
        return false;
    }
}
//---------------------------------------------------------------------------
bool Light::SetAllLight(void)
{
    if (haveGetPower) {
        for (unsigned char add=0x20;add<=0x25;add++)
             SetLight(add,0xFF,0xFF);
        return true;
    }  else {
        GetAuthority(LPTBASEPORT);
        return false;
    }
}
//---------------------------------------------------------------------------
bool Light::SetAllDark(void)
{
    if (haveGetPower) {
        for (unsigned char add=0x20;add<=0x25;add++)
             SetLight(add,0,0);
        return true;
    }  else {
        GetAuthority(LPTBASEPORT);
        return false;
    }
}
//---------------------------------------------------------------------------
bool Light::SetCardDefault(void)
{
    if (SetTimeOfCard(0xFE))                   //�]�w����ɶ���254��,�U�@��N�|��w�]��
       return true;
    else return false;
}
//---------------------------------------------------------------------------
bool Light::SayHelloToCard(void)
{
    if (SetTimeOfCard(0))
       return true;
    else return false;
}
//---------------------------------------------------------------------------
