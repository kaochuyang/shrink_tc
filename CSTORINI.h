#ifndef CStorINIH
#define CStorINIH

#include "inifile.h"

//--------------------------------------------------------------------------
class CStorINI
{
    public:
      CStorINI(void);
      ~CStorINI(void);

      int vLoadIntFromINI(char *, char *, int);
      bool vSaveIntToINI(char *, char *, int);

      float vLoadFloatFromINI(char *, char *, float);
      bool vSaveFloatToINI(char *, char *, float);

      bool vSaveStringToINI(char *, char *, char *);
      char *vLoadStringFromINI(char *, char *, char *);

      int vLoadSegmentIntFromINI(char *, char *, int );//Arwen add 110210
      bool vSaveIntSegmentToINI(char *, char *, int);

      //---XML ARWEN ++ 1001006
      bool vSave_XML_HEAD(char *Section, int typemode , const std::string& fileName);
      bool vSave_XML_INT(char *Section, char *Key, int iValue,const std::string& fileName);
      bool vSave_XML_HEX(char *Section, char *Key, int iValue,const std::string& fileName);

    private:
      CIniFile ini;
      CIniFile Segini;

      CIniFile xml;//--XML ARWEN ++ 1001006
};
//--------------------------------------------------------------------------
extern CStorINI oStorINI;
#endif
