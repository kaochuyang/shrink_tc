#ifndef JSONCPP_TO_JAVA_H
#define JSONCPP_TO_JAVA_H
#include <string>
#include <unistd.h>
using namespace std;
class jsoncpp_to_java
{
    public:
        jsoncpp_to_java();
        virtual ~jsoncpp_to_java();
        void comment_complement(char *pInput, char *pOutput, char *pSrc, char *pDst);

    protected:
    private:
};

#endif // JSONCPP_TO_JAVA_H
