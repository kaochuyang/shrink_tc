#include "jsoncpp_to_java.h"

jsoncpp_to_java::jsoncpp_to_java()
{
    //ctor
}

jsoncpp_to_java::~jsoncpp_to_java()
{
    //dtor
}

void jsoncpp_to_java::comment_complement(char *pInput, char *pOutput, char *pSrc, char *pDst)
{


    char *pi, *po, *p;
    int nSrcLen, nDstLen, nLen;

    // «ü¦V?¤J¦r²Å¦êªº´å?«ü?.
    pi = pInput;
    // «ü¦V?¥X¦r²Å¦êªº´å?«ü?.
    po = pOutput;
    // ?ºâ³Q´À?¦ê©M´À?¦êªº?«×.
    nSrcLen = strlen(pSrc);
    nDstLen = strlen(pDst);

    // ¬d§äpi«ü¦V¦r²Å¦ê¤¤²Ä¤@¦¸¥X?´À?¦êªº¦ì¸m,¦}ªð¦^«ü?(§ä¤£¨ì?ªð¦^null).
    p = strstr(pi, pSrc);
    if(p)
    {
        // §ä¨ì.
        while(p)
        {
            // ?ºâ³Q´À?¦ê«e?¦r²Å¦êªº?«×.
            nLen = (int)(p - pi);
            // Î`¨î¨ì?¥X¦r²Å¦ê.
            memcpy(po, pi, nLen);
            memcpy(po + nLen, pDst, nDstLen);
            // ¸õ?³Q´À?¦ê.
            pi = p + nSrcLen;
            // ?¾ã«ü¦V?¥X¦êªº«ü?¦ì¸m.
            po = po + nLen + nDstLen;
            // ??¬d§ä.
            p = strstr(pi, pSrc);
        }
        // Î`¨î³Ñ§E¦r²Å¦ê.
        strcpy(po, pi);
    }
    else
    {
        // ?¦³§ä¨ì?­ì?Î`¨î.
        strcpy(po, pi);
    }

}
