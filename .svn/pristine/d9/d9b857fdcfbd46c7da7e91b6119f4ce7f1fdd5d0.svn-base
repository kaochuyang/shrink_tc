#ifndef CIOComH
#define CIOComH

#include <termios.h> //struct termios

#define DETECTOR_TIMEOUT 6050

//---------------------------------------------------------------------
class CIOCom
{
private:
    int fd;
    struct termios oldtio,newtio;

 public:
    bool opened;
    CIOCom();
    ~CIOCom();
    void Open();
    void Close();
    int Read(unsigned char *block, unsigned long &b_length);
};
//---------------------------------------------------------------------
#endif
