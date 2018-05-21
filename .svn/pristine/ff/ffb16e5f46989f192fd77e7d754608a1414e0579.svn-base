
//---------------------------------------------------------------------------
//----------------------------------------------------------
struct _Bit00{
  unsigned char b1:1;
  unsigned char b2:1;
  unsigned char b3:1;
  unsigned char b4:1;
  unsigned char b5:1;
  unsigned char b6:1;
  unsigned char b7:1;
  unsigned char b8:1;
};

union unionByte{
  _Bit00 block;
  unsigned char ucByte;
};


class CTools{
  public:
    unsigned short int vVer30LightToCCTLight(unsigned char);
    unsigned char vCCTLightToVer30Light(unsigned short int);
    int vCCTLightToVer30SignalMapLight_5F0F(const int, const unsigned short int);

    unsigned char vGuessLightStep(int, unsigned char);

};

extern CTools oTools;
