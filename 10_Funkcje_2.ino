void setup_layer2(void){
  //wiązanie semaforów
  
  Sortownia1.sem_dla_robota=&Robot.sem_od_karuzeli1;
  Sortownia2.sem_dla_robota=&Robot.sem_od_karuzeli2;

  Stacja1.req_dla_karuzeli=&Sortownia1.req_od_stacji;
  Stacja1.sem_dla_mastera=&Linia_produkcyjna.sem_od_stacji1;
  
  Stacja2.req_dla_karuzeli=&Sortownia2.req_od_stacji;
  Stacja2.sem_dla_mastera=&Linia_produkcyjna.sem_od_stacji2;

  Robot.sem_dla_stacji1=&Stacja1.sem_od_robota;
  Robot.sem_dla_stacji2=&Stacja2.sem_od_robota;

  Linia_produkcyjna.sem_dla_stacji0=&Stacja0.sem_od_mastera;
  Linia_produkcyjna.req_dla_stacji1=&Stacja1.req_od_mastera;
  Linia_produkcyjna.req_dla_stacji2=&Stacja2.req_od_mastera; 



 
}

void automaty_stanu(void){

  Sortownia1.automat_stanu();
  Sortownia2.automat_stanu();

  Stacja0.automat_stanu();
  Stacja1.automat_stanu();
  Stacja2.automat_stanu();

  Linia_produkcyjna.automat_stanu();
  Robot.rutyna(); 
  
}

int16_t get_station_status(void){
  int16_t val=0;
  val+=Stacja1.status_stacji;
  val=val*10+Stacja2.status_stacji;
  val=val*10+Sortownia1.status_stacji;
  val=val*10+Sortownia2.status_stacji;
  val=val*10+Robot.status_stacji;
  return val;
}

void servos_set_pos(uint16_t val){
  if(Stacja2.status_stacji==1)Szlaban2.set_predefined(val&3);
  val>>=2;
  if(Stacja1.status_stacji==1)Szlaban1.set_predefined(val&3);
  val>>=2;
  if(Stacja2.status_stacji==1)Zapadka2.set_predefined(val&3);
  val>>=2;
  if(Stacja1.status_stacji==1)Zapadka1.set_predefined(val&3);
  val>>=2;
  if(Sortownia2.status_stacji==1)Podajnik3.set_predefined(val&3);
  val>>=2;
  if(Sortownia1.status_stacji==1)Podajnik2.set_predefined(val&3);
  val>>=2;
  Podajnik1.set_predefined(val&3);
}
