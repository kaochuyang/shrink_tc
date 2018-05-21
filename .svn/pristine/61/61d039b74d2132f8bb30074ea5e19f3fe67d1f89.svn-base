#ifndef screenCommunicationH
#define screenCommunicationH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCommunication:public SCREEN
{
    public:
        ScreenCommunication(void);
        ~ScreenCommunication(void);

        void doKeyWork(BYTE);
        void DisplayCommunication(void);
        void status(void);

   private:
        BYTE BRTpriorityBitmap[3840];
        DISP_WORD IPC[2];
        DISP_WORD SRC1[2];
        DISP_WORD SRC2[2];
        void loadBitmapFromFile(void);
        void initDispWord(void);
        void doKeyF1Work(void);
        void doKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenCommunication screenCommunication;
#endif
