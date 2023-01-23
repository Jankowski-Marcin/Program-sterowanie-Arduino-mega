ę  //biblioteki
#include <Arduino_pin.h>
#include <Interrupt_pin.h>
#include <Step_motor.h>
#include <Servo_motor_mega.h>
#include <Timer.h>

//makra
#define ODB1 2
#define ODB2 3
#define KOLOR1 A0
#define KOLOR2 A2
#define STYK1 20
#define STYK2 21
#define MAG1 51
#define MAG2 53 

#define PC Serial
#define RPI Serial1 
#define ROBOT.Serial2


//zmienna do przypisania wskźników
uint8_t empty=0;

//zmienne do debugowania
uint8_t bufor;
uint8_t bufor_out;
uint32_t poczatek;
uint32_t koniec;
