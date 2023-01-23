//zmienne pomocnicze
uint8_t sem_in=0;
uint8_t sem_out=0;


//obiekty klas niskiego poziomu
int8_t slot,pozycja; 

//Serwa
Servo_motor Zapadka1(10,50);
const uint16_t pozycje_Z1[2]={50,270};

Servo_motor Szlaban1(4,300);
const uint16_t pozycje_S1[2]={300,100};


Servo_motor Zapadka2(12,100);
const uint16_t pozycje_Z2[2]={100,450};

Servo_motor Szlaban2(5,300);
const uint16_t pozycje_S2[2]={300,100};

Servo_motor Podajnik1(7,250);
const uint16_t pozycje_P1[2]={250,50};

Servo_motor Podajnik2(13,50);
const uint16_t pozycje_P2[3]={50,150,265};

Servo_motor Podajnik3(6,80);
const uint16_t pozycje_P3[3]={75,200,260};

//krokówki
Step_motor Karuzela1(28,25,26,24,22,11);
Step_motor Karuzela2(38,46,40,42,44,9);
Step_motor Tasmociag(36,29,34,32,30,8);


//czujniki koloru
Color_sensor Kolor1(KOLOR1);
uint8_t kol1;
Color_sensor Kolor2(KOLOR2);
uint8_t kol2;

//piny wejść
Interrupt_pin Stykowy1(STYK1);
bool sty1;
Interrupt_pin Stykowy2(STYK2);
bool sty2;

Arduino_pin Magnetyczny1(MAG1);
bool mag1;
Arduino_pin Magnetyczny2(MAG2);
bool mag2;

Interrupt_pin Odbiciowy1(ODB1);
bool odb1;
Interrupt_pin Odbiciowy2(ODB2);
bool odb2;


//zmienne globalne
uint8_t status_maszyny=1;//0-auto 1-manual  2-awaria

uint16_t licznik_cykli=0;
