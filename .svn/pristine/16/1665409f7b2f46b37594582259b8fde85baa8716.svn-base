#ifndef CTargetH
#define CTargetH

//---------------------------------------------------------------------------
class CTarget
{
  public:
    CTarget(){}
    bool stable_or_not;
    unsigned char _target_packet[4];

    bool operator==(const CTarget &rhs){
      for(unsigned short i=0;i<4;i++){
        if(_target_packet[i]==rhs._target_packet[i]) continue;
        else return false;
      }
      return true;
    }

    CTarget &operator=(const CTarget &rhs_target){
      if(this!=&rhs_target){
        this->SetTarget(rhs_target._target_packet);
        this->stable_or_not = rhs_target.stable_or_not;
      }
      return *this;
    }

    void SetTarget(const unsigned char *rhs_target_packet){
      for(unsigned short i=0;i<4;i++) _target_packet[i]=rhs_target_packet[i];
    }

    unsigned short int CalculatedTargetAmount(void){
      unsigned short int amount=0;
      for(int i=0;i<4;i++){
        amount += ((_target_packet[i]&0x01)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x02)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x04)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x08)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x10)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x20)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x40)==0) ? 0 : 1;
        amount += ((_target_packet[i]&0x80)==0) ? 0 : 1;
      }
      return amount;
    }

};
//---------------------------------------------------------------------------
#endif
