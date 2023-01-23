 class Stacja_podajnik{
  private:
    //wskaźnik na obiekt warstwy pierwszej
    Servo_motor *Podajnik;
    // timer
    Timer Tim;
    //stan
    uint8_t stan;
    public:
    uint8_t sem_od_mastera;

    Stacja_podajnik(Servo_motor *podajnik){
      Podajnik=podajnik;
      stan=1;
      sem_od_mastera=0;
    }
    void automat_stanu(void){
      switch(stan){
        case 1:
        if(sem_od_mastera){
          Podajnik->set_predefined(1);
          ++stan;
          Tim.in(20);
        }
        break;

        case 2:
        if(Tim.out()){
          Podajnik->set_predefined(0);
          ++stan;
        }
        break;

        case 3:
        if(!sem_od_mastera){
          stan=1;
        }
      }        
    }

    
}; 
