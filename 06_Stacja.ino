class Stacja{
  private:
  //wskaźniki na obiekty pierwszej warstwy
  Servo_motor *Zapadka;
  Servo_motor *Szlaban;
  Interrupt_pin *Odbiciowy;
  Timer Tim;
 public:
  //zmienne pomocnicze
  uint8_t stan;

 
//semafory

  uint8_t sem_od_robota;
  uint8_t req_od_mastera;
  
  uint8_t *req_dla_karuzeli;
  uint8_t *sem_dla_mastera;

  uint8_t status_stacji;

  
  

  Stacja(Servo_motor *zapadka, Servo_motor *szlaban, Interrupt_pin *odbiciowy){
    //przypisanie wskaźników na obiekty pierwszej warstwy
    Zapadka=zapadka;
    Szlaban=szlaban;
    Odbiciowy=odbiciowy;
    
    //inicjalizacja stanu początkowego

    stan='M';
    
    //inicjalizacja semaforów
    sem_od_robota=0;
    req_od_mastera=0;
    
    
    req_dla_karuzeli=&empty;
    sem_dla_mastera=&empty;

    status_stacji=1;
  }

  void bazowanie(void){
    Szlaban->set_predefined(0);
    Zapadka->set_predefined(0);
  }
  void automat_stanu(void){
    //////////////////////////////////////////AUTOMAT STANU TRYB MANUAL
    switch(stan){
      case 'E':
      status_stacji=2;
      break;

      case 'M':
      status_stacji=1;
      break;

      case 'B':
      status_stacji=1;
      Szlaban->set_predefined(0);
      Zapadka->set_predefined(0);
      stan=1;
      break;
    }
    if(status_maszyny) return;



    
    switch(stan){
      case 1:
        *req_dla_karuzeli=0;
        *sem_dla_mastera=0;
        if(req_od_mastera){
          Odbiciowy->intr_on();
          ++stan;
        }
      break;

      case 2:
        *req_dla_karuzeli=req_od_mastera;
        *sem_dla_mastera=0;
        if(sem_od_robota){
          ++stan;
          if(!Tasmociag.state()){
            Zapadka->set_predefined(1);
            Odbiciowy->intr_off();
            Tim.in(8);
            ++stan;          
          } 
        }
      break;

      case 3:
        *req_dla_karuzeli=0;
        *sem_dla_mastera=0;
        if(!Tasmociag.state()){
          Zapadka->set_predefined(1);
          Odbiciowy->intr_off();
          Tim.in(8);
          ++stan;          
        }
      break;

      case 4:
        *req_dla_karuzeli=0;
        *sem_dla_mastera=1;
        if(Tim.out()){
          Zapadka->set_predefined(0);  
          Szlaban->set_predefined(0);
          ++stan;
        }
      break;
      
      case 5:
        *req_dla_karuzeli=0;
        *sem_dla_mastera=1;
        if(Odbiciowy->input() && !req_od_mastera){
          stan=1;  
        }
      break; 
    }     
  }
  void clr_err(void){
    if(stan=='E')stan='M';
    }
  void base(void){
    if(stan=='M')stan='B';  
  }
  void go_manual(void){
    if(stan=='E' || stan=='B' || stan==0)return;
    stan='M';
    status_stacji=1;  
  }
  void go_auto(void){
    if(stan!='M' && stan!=1) return;
    stan=1;  
    status_stacji=0;
  }
  
  
};
