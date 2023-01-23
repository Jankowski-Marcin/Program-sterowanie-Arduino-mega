void komunikacja_out(void){
  static char stan='A';
  int32_t val=0; 
  switch(stan){
    case 'A':
    val=status_maszyny;
    break;

    case 'B':
    val=get_station_status();
    break;

    case 'C':
    val=digital_sensors();
    break;

    case 'D':
    val=analogRead(KOLOR1);
    break;

    case 'E':
    val=analogRead(KOLOR2);
    break;

    case 'F':
    val=servos_get_pos();
    break;

    case 'G':
    val=Sortownia1.get_elements();
    break;

    case 'H':
    val=Sortownia2.get_elements();
    break;

    case 'I':
    val=licznik_cykli;
    break;

    case 'J':
    val=Linia_produkcyjna.get_prod_table();
    break;    
  }
  RPI.write('[');
  RPI.print(stan);
  RPI.print(val);
  RPI.write(']');
  ++stan;
  if(stan>'K') stan='A';
}
