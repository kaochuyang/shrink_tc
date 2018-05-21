/*
 * ScreenActuateArwenMenu.h
 *
 *  Created on: 2010/10/5
 *      Author: arwen
 */

#ifndef SCREENACTUATEARWENMENU_H_
#define SCREENACTUATEARWENMENU_H_

#include "SCREEN.h"

class ScreenActuateArwenMenu: public SCREEN {
public:
	ScreenActuateArwenMenu();
	virtual ~ScreenActuateArwenMenu();
	void loadBitmapFromFile();
	void DisplayActuateArwenMenu(void);

	void DoKeyWork(BYTE);


private:
    void initDispWord(void);

	DISP_WORD mark[10];
	int cSelect;
    BYTE actuateArwenMenuBitmap[3840];

    void DoKey0Work(void);
    void DoKey1Work(void);
    void DoKey2Work(void);
    void DoKey3Work(void);
    void DoKey4Work(void);
    void DoKey5Work(void);
    void DoKey6Work(void);
    void DoKey7Work(void);
    void DoKey8Work(void);
    void DoKey9Work(void);

    void DoKeyF1Work(void);
    void DoKeyF2Work(void);
    void DoKeyF3Work(void);
    void DoKeyF4Work(void);

    void DoKeyUPWork(void);
    void DoKeyDOWNWork(void);
    void DoKeyLEFTWork(void);
    void DoKeyRIGHTWork(void);
    void DoKeyEnterWork(void);



};
extern ScreenActuateArwenMenu screenActuateArwenMenu;
#endif /* SCREENACTUATEARWENMENU_H_ */
