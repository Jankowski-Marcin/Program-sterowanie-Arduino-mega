class Sortownia{
  private: 

  //wskaźniki na obiekty pierwszej warstwy
  Step_motor *Karuzela;
  Servo_motor *Podajnik;
  Arduino_pin *Magnetyczny;
  Interrupt_pin *Stykowy;
  Color_sensor *Koloru;  
  
  //timery
  Timer Tim;
  Timer Mag;
  Timer Sty;
  public: //private:
  //zmienne pomocnicze
  uint8_t stan;
  
  int8_t poz_uzu; //pozycja podajnika
  int8_t poz_rob; //pozycja robot
  int8_t rotacja=0; 

  public:
  //semafory
  int8_t tab_elementow[8];
  uint8_t req_od_stacji; 
  uint8_t *sem_dla_robota;

  //dane do odczytu
  uint8_t status_stacji;
  
  
  Sortownia(Step_motor *karuzela, Servo_motor *podajnik, Arduino_pin *magnetyczny, Interrupt_pin *stykowy, Color_sensor *koloru, int8_t robot_offset){
    //przypisanie wskaźników na obiekty warstwy pierwszej
    Karuzela=karuzela;
    Podajnik=podajnik;
    Magnetyczny=magnetyczny;
    Stykowy=stykowy;
    Koloru=koloru;

    //inicjalizacja zmiennych pomocniczych
    for(uint8_t i;i<8;++i){
      tab_elementow[i]=0;
    }
    status_stacji=1;
    
    stan='M';
    poz_uzu=0;
    poz_rob=robot_offset;
    rotacja=0;

    //inicjalizacja semaforów
    req_od_stacji=0;
    sem_dla_robota=&empty;
  }
 
  int8_t szukaj(int8_t zacznij_od, uint8_t cel){
    for(uint8_t i=0;i<4;++i){ 
      if(tab_elementow[(zacznij_od+i)%8]==cel)
        return i;
      if(tab_elementow[(zacznij_od-i+7)%8]==cel)
        return -1-i;  
    }
      return 40;
  }
  void obroc(int8_t o){
    rotacja=o;
    Karuzela->rotate(800*o);
  }
  void aktualizuj_pozycje(void){
    poz_rob=(poz_rob+rotacja+8)%8;
    poz_uzu=(poz_uzu+rotacja+8)%8;
    rotacja=0;
  }
  
  void dodaj(uint8_t pozycja){
    int8_t kolor=Koloru->get_color();
    tab_elementow[pozycja]=kolor;
  }

  void odejmij(uint8_t pozycja){
    tab_elementow[pozycja]=0;
  }

  ////////////////////////////////////////////////Rutyna do wywołania w pętli
  void automat_stanu(void){
     //zmienne pomocnicze
    if(stan==1)dodaj(poz_uzu);
    if(*sem_dla_robota && !req_od_stacji)odejmij(poz_rob);
    
    int8_t mv_req=szukaj(poz_rob,req_od_stacji); //ruch potrzebny na spełnienie requesta
    int8_t mv_uzu=szukaj(poz_uzu,0);             //ruch potrzebny na uzupełnienie slota
    int8_t mv_spr=szukaj(poz_uzu,-1);
    int8_t w1 = req_od_stacji && mv_req && mv_req<40;      //zbiorczy warunek pierwsz
    int8_t w2 = (!req_od_stacji || mv_req==40) && mv_uzu && mv_uzu<40; //zbiorczy warunek drugi
  
    //Czujniki  
    if(Magnetyczny->input()){
      Mag.in(5);
    }
    if(!Stykowy->input()){
      Sty.in(5);    
    }

    //////////////////////////////////////////////AUTOMAT STANU TRYB MANUAL
    switch (stan){
      ////////////////////////////////////////////stan błędu karuzeli  
      case 'E':                                 
      *sem_dla_robota=0;
      status_stacji=2;
      Karuzela->disable();
      break;

      ///////////////////////////////////////////sterowanie ręczne
      case 'M':                                
      *sem_dla_robota=0;
      status_stacji=1;
      break;

      ///////////////////////////////////////////uruchomienie bazowanie
      case 'B':                               
      *sem_dla_robota=0;
      status_stacji=1;
      Karuzela->enable();
      Karuzela->rotate(800);
      if(Sty.out()){
   
        Stykowy->intr_on(); 
        stan=0;
        for(uint8_t i=0;i<8;++i){
          tab_elementow[1]=0;
        }
      }
      break;

      //////////////////////////////////////////Bazowanie
      case 0:                                 
      *sem_dla_robota=0;
      status_stacji=1;
      if(!Karuzela->state()&&!Sty.out()){
        stan=1;
         
      }else if(!Karuzela->state()&&Sty.out()){
        stan='E'; 
      }
      break;
      
      }
     if(status_maszyny) return;

     switch (stan){

      case 1:                               //oczekiwanie na pozycji
      *sem_dla_robota=req_od_stacji&&!mv_req;
      status_stacji=0;
      
      if(Sty.out()){                        //przejście do stanu błędu pozycji karuzeli 
        stan='E';
      }else if(w1){                         //obrót do odbioru req
        stan=2;
        obroc(mv_req);
      }else if(w2){                         //obrót do uzu
        stan=2;
        obroc(mv_uzu);
      }else if(Mag.out()&&!mv_uzu){
        Podajnik->set_predefined(1);
        Tim.in(5);
        stan=4;
      }else if(mv_spr<40 && mv_spr){
        stan=2;
        obroc(mv_spr);
      }
      break;
      
      case 2:                                      //oczekiwanie na odjazd slota
      *sem_dla_robota=0;
      status_stacji=0;
      if(!Karuzela->state()){                      //przejście do stanu błędu pozycji karuzeli
        stan='E'; 
      }else if(Sty.out()){                         //potwierdzenie odjazdu slota              
        stan=3; 
      }
      break;

      case 3:                                     //oczekiwanie na zakończenie obrotu
      *sem_dla_robota=0;
      status_stacji=0;
      if(!Karuzela->state() && Sty.out()){        //przejście do stanu błędu pozycji karuzeli 
        stan='E';
      }                                           //potwierdzenie obrotu karuzeli >> powrót do stanu pierwszego
      else if(!Karuzela->state() && !Sty.out()){
        aktualizuj_pozycje();
        stan=1;
      }
      break;

      case 4:                                     //sekwencja dodawania elementu połowiczne wypchnięcie
      *sem_dla_robota=req_od_stacji&&!mv_req;
      status_stacji=0;
      if(Tim.out()){
        Podajnik->set_predefined(2);
        Tim.in(10);
        stan=5;
      }
      break;
      
      case  5:                                   //sekwencja dodawania elementu całkowite wypchnięcie    
      *sem_dla_robota=req_od_stacji&&!mv_req;
      status_stacji=0;
      if(Tim.out()){
          Podajnik->set_predefined(0);
          stan=1;
      }
      break;
    }

  }

  void clr_err(void){
    if(stan=='E')stan='M';
    }
  void base(void){
    if(stan=='M' || stan=='E'){
      stan='B'; 
      for(uint8_t i;i<8;++i){
        tab_elementow[i]=0;
      } 
    }
  }
  void go_manual(void){
    if(stan=='E' || stan=='B' || stan==0)return;
    stan='M';
    status_stacji=1;  
  }
  void go_auto(void){
    if(stan!='M' && stan!=1) return;
    stan=1;  
    Karuzela->enable();
    status_stacji=0;
  }
   
  uint16_t get_elements(void){
    uint16_t val=0;
    uint8_t elementy[4]={0,0,0,0};
    for(uint8_t i=0; i<8; ++i){
      val=tab_elementow[i];
      if(val>0)
      ++elementy[val-1];
    }
    val=elementy[0];
    val=val*10+elementy[1];
    val=val*10+elementy[2];
    val=val*10+elementy[3];
    return val;
  }
  
};
