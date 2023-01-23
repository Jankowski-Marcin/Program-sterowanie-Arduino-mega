class Robot_komunikacja{
  public:
  uint8_t sem_od_karuzeli1;
  uint8_t sem_od_karuzeli2;
  
  uint8_t *sem_dla_stacji1;
  uint8_t *sem_dla_stacji2;

  uint8_t bufor;

  Timer watchdog;
  uint8_t status_stacji;
  
  Robot_komunikacja(void){
    bufor=0;
    sem_od_karuzeli1=0;
    sem_od_karuzeli2=0;

    *sem_dla_stacji1=&empty;
    *sem_dla_stacji2=&empty;
    status_stacji=0;
    
  }
  void rutyna(void){
    if(Serial2.available()){
      bufor=Serial2.read()-'0';
      if(bufor>=0 && bufor<=3){
      watchdog.in(10);
      
      *sem_dla_stacji1=bufor & 1;
      *sem_dla_stacji2=bufor & 2;

      bufor=sem_od_karuzeli1 | (sem_od_karuzeli2<<1);
      Serial2.print(bufor);
      }
    }
    if(watchdog.out()){
      status_stacji=2;
    }else status_stacji=0;
  }
  
};
