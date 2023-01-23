class Master{
  private:
  //wskaźnik na obiekty warstwy#1

  //zmienne pomocnicze
  uint8_t tab_prod[4];
  public:
  uint8_t stan;  
  
  public:
  //semaforty
  uint8_t *sem_dla_stacji0;
  uint8_t *req_dla_stacji1;
  uint8_t *req_dla_stacji2;
  
  uint8_t sem_od_stacji1;
  uint8_t sem_od_stacji2;

  
  Master(void){
    sem_dla_stacji0=&empty;
    req_dla_stacji1=&empty;
    req_dla_stacji2=&empty;
    stan=1;
    tab_prod[0]=0;
    tab_prod[1]=0;
    tab_prod[2]=0;
    tab_prod[3]=0;    
    }

    
  void automat_stanu(void){
    switch(stan){
      case'B':
      if(tab_prod[1]){
        stan=0;
        tab_prod[2]=0;
        tab_prod[1]=0;   
        stan=0;
        Szlaban1.set_predefined(1);
        Szlanban2.set_predefined(2);
        Tasmociag.rotate(-16000); 
      }else if(tab_prod[2]){
        stan=0;
        tab_prod[2]=0;
        tab_prod[1]=0;
        Tasmociag.rotate(-8000); 
      }else{
        stan=1;
      }
      break;

      case 0:
        if(!Tasmociag.state()){
          stan=1;
        }
      break;  
            
    }
    
    if(status_maszyny) return;
    switch(stan){
      case 1://oczekiwanie na nowe zamówienie lub kontynuacja produkcji
      *sem_dla_stacji0=0;
      *req_dla_stacji1=0;
      *req_dla_stacji2=tab_prod[2];
      if( tab_prod[1] || tab_prod[2]){
        ++stan;
        ++licznik_cykli; 
        Tasmociag.enable();
      }else if(tab_prod[0] && !tab_prod[1]){
        tab_prod[1]=tab_prod[0]; 
        tab_prod[0]=0;
        Tasmociag.disable();
      }else if(tab_prod[3]){
        stan=6;
        ++licznik_cykli;
        Tasmociag.enable();
        Tasmociag.rotate(-5000);
             
      }
      break;

      case 2://oczekiwanie na dojazd aut na aktywne stacje
      *sem_dla_stacji0=tab_prod[1]>0;
      *req_dla_stacji1=tab_prod[1];
      *req_dla_stacji2=tab_prod[2];
      Tasmociag.rotate(-2000);
      if( (!Odbiciowy1.input() || !tab_prod[1]) && (!Odbiciowy2.input() || !tab_prod[2]) ){
        ++stan;
      }
      break;

      case 3://oczekiwanie na zakończenie produkcji na aktywnych stacjach
      *sem_dla_stacji0=tab_prod[1]>0;
      *req_dla_stacji1=tab_prod[1];
      *req_dla_stacji2=tab_prod[2];
      if( (sem_od_stacji1 || !tab_prod[1]) && (sem_od_stacji2 || !tab_prod[2]) ){
        ++stan;      
      }
      break;

      case 4://oczekiwanie na zjazd aut z aktywnych stacji
      *sem_dla_stacji0=0;
      *req_dla_stacji1=0;
      *req_dla_stacji2=0;
      Tasmociag.rotate(-2000);
      if( Odbiciowy1.input() && Odbiciowy2.input()){

        ++stan;    
      }
      break; 

      case 5://oczekiwanie na dojazd tasmociagu + przekazanie requesta na nowa stacje
      *sem_dla_stacji0=0;
      *req_dla_stacji1=0;
      *req_dla_stacji2=0;
      if(!Tasmociag.state()){
        tab_prod[3]=tab_prod[2];
        tab_prod[2]=tab_prod[1];
        tab_prod[1]=tab_prod[0];  
        tab_prod[0]=0;
                 
        stan=1;         
      }
      break;

      case 6:
      *sem_dla_stacji0=0;
      *req_dla_stacji1=0;
      *req_dla_stacji2=0;
      if(!Tasmociag.state()){
        tab_prod[3]=0;
        stan=1;
      }
      
    
    
    }
    
 
      
  }
     int16_t get_prod_table(void){
      return tab_prod[0]+tab_prod[1]*10+tab_prod[2]*100+tab_prod[3]*1000;
    }
      int16_t request(uint8_t req){
        if(!tab_prod[0]);
        tab_prod[0]=req;
      }
    void base(void){
      stan='B';
    }
  
};
