
void stop_Karuzela1(void){
  
  Karuzela1.rotate(20);
}


void stop_Karuzela2(void){
  
  Karuzela2.rotate(20);
}

void zamknij_Szlaban1(void){
  Szlaban1.set_predefined(1);
}

void zamknij_Szlaban2(void){
  Szlaban2.set_predefined(1);
}

void Setup_layer1(void){
  ////////////////////////////////////////////Silniki_krokowe
  Step_motor::timer4_setup();
  Tasmociag.set_res(2);
  Tasmociag.set_freq(125);
  ///////////////////////////////////////////Timery
  Timer::timer3_setup();
  
  ////////////////////////////////////////////czujniki koloru 
  Kolor1.add_color(970,1025);
  Kolor1.add_color(20,200);
  Kolor1.add_color(250,400);
  Kolor1.add_color(430,680);
  Kolor1.add_color(800,950);

  Kolor2.add_color(920,1025);
  Kolor2.add_color(20,120);
  Kolor2.add_color(150,300);
  Kolor2.add_color(300,500);
  Kolor2.add_color(800,920);

  /////////////////////////////////////////////serwa
  Podajnik1.attach_predefined(&pozycje_P1[0],2);
  Podajnik2.attach_predefined(&pozycje_P2[0],3);
  Podajnik3.attach_predefined(&pozycje_P3[0],3);

  Zapadka1.attach_predefined(&pozycje_Z1[0],2);
  Zapadka2.attach_predefined(&pozycje_Z2[0],2);  

  Szlaban1.attach_predefined(&pozycje_S1[0],2);
  Szlaban2.attach_predefined(&pozycje_S2[0],2);    

  Servo_motor::timer5_setup();

  ////////////////////////////////////////////Pozostałe czujniki
  Stykowy1.mode(0,0);
  Stykowy2.mode(0,0);

  Magnetyczny1.mode(0,0);
  Magnetyczny2.mode(0,0);

  Odbiciowy1.mode(0,0);
  Odbiciowy2.mode(0,0);

  /////////////////////////////////////////Przerwania zewnetrzne
  Stykowy1.attach2intr(&stop_Karuzela1,2);
  Stykowy2.attach2intr(&stop_Karuzela2,2);

  Odbiciowy1.attach2intr(&zamknij_Szlaban1,2);
  Odbiciowy2.attach2intr(&zamknij_Szlaban2,2);
  
}
uint8_t digital_sensors(void){
  uint8_t val=0;
  val|=Magnetyczny1.input();
  val<<=1;
  val|=Magnetyczny2.input();
  val<<=1;
  val|=Stykowy1.input();
  val<<=1;
  val|=Stykowy2.input();
  val<<=1;
  val|=Odbiciowy1.input();
  val<<=1;
  val|=Odbiciowy2.input();
  return val;
}

uint16_t servos_get_pos(void){
  uint16_t val=0;
  val|=Podajnik1.get_pos();
  val<<=2;
  val|=Podajnik2.get_pos();
  val<<=2;
  val|=Podajnik3.get_pos();
  val<<=2;
  val|=Zapadka1.get_pos();
  val<<=2;
  val|=Zapadka2.get_pos();
  val<<=2;
  val|=Szlaban1.get_pos();
  val<<=2;
  val|=Szlaban2.get_pos();
  return val;
}
