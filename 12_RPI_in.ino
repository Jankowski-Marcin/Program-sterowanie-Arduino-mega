void fullfill(char etykieta, int32_t val){
  switch(etykieta){
    case 'A':
    if(!val && !get_station_status() && status_maszyny==1) status_maszyny=0;
    if(!val && get_station_status() && status_maszyny==1){
    Stacja1.go_auto();
    Stacja2.go_auto();
    Sortownia1.go_auto();
    Sortownia2.go_auto();
    }
    
    if(val==1) Stacja1.go_auto();
    if(val==2) Stacja2.go_auto();
    if(val==3) Sortownia1.go_auto();
    if(val==4) Sortownia2.go_auto();
    break;

    case 'B':
    if(!val && status_maszyny==2){
      status_maszyny=1;
      Stacja1.clr_err();
      Stacja2.clr_err();
      Sortownia1.clr_err();
      Sortownia2.clr_err();
    }

    if(val==1) Stacja1.clr_err();
    if(val==2) Stacja2.clr_err();
    if(val==3) Sortownia1.clr_err();
    if(val==4) Sortownia2.clr_err();
    break;

    case 'C':
    Linia_produkcyjna.request(val);
    break;

    case 'D':
    if(status_maszyny!=1) return;
    servos_set_pos(val);
    break;

    case 'E':
    if(Sortownia1.status_stacji!=1) return;
    Karuzela1.rotate(val);
    break;

    case 'F':
    if(Sortownia2.status_stacji!=1) return;
    Karuzela2.rotate(val);
    break;

    case 'G':
    if(status_maszyny!=1) return;
    Tasmociag.rotate(val*40);
    break;

    case 'H':
    if(!val){
      Linia_produkcyjna.base();
      Stacja1.base();
      Stacja2.base();
      Sortownia1.base();
      Sortownia2.base();
    }
    if(val==1) Stacja1.base();
    if(val==2) Stacja2.base();
    if(val==3) Sortownia1.base();
    if(val==4) Sortownia2.base();
    break;

    case 'I':
    if(status_maszyny!=1) return;
    if(val==1 && Sortownia1.status_stacji==1) Karuzela1.enable();
    if(val==2 && Sortownia2.status_stacji==1) Karuzela2.enable();
    if(val==3) Tasmociag.enable();
    break;

    case 'J':
    if(status_maszyny!=1) return;
    if(val==1 && Sortownia1.status_stacji==1) Karuzela1.disable();
    if(val==2 && Sortownia2.status_stacji==1) Karuzela2.disable();
    if(val==3) Tasmociag.disable();
    break;
    
    case 'K':
    if(!val && status_maszyny==0) status_maszyny=1;
    if(val==1) Stacja1.go_manual();
    if(val==2) Stacja2.go_manual();
    if(val==3) Sortownia1.go_manual();
    if(val==4) Sortownia2.go_manual();  
    break;  
  }
}

void komunikacja_in(void){
  static uint8_t stan=0;
  static char bufor;
  static char etykieta="";
  static int32_t wartosc=0;
  static int minus=0;
  while(RPI.available()||stan==4){
    bufor = RPI.read();
    switch(stan){
      case 0:
      if(bufor=='['){
      ++stan;
      }
      break;

      case 1:
      if(bufor=='['){
        stan=1;
      }else if(bufor==']'){
        stan=0;
      }else{
        etykieta=bufor;
        ++stan;
      }
      break;
      
      case 2:
      if(bufor=='['){
        stan=1;
      }else if(bufor==']'){
        stan=0;
      }else if(bufor=='-'){
        minus=1;
        ++stan;
        break;
      }else{
        wartosc=wartosc*10+(bufor-'0');
        ++stan;
      }
      break;

      case 3:
      if(bufor=='['){
        stan=1;
      }else if(bufor==']'){
        if(minus){
          fullfill(etykieta,-wartosc);
        }else{
          fullfill(etykieta,wartosc); 
        }
      etykieta="";
      wartosc=0;
      minus=0;
      stan=0; 
        
      }else{
        wartosc=wartosc*10+(bufor-'0');
      }
      break;        
    }
  }  
}
