#include "Wire.h"
#include "src/notes.h"

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUZZER_PIN 4

#define PERSIZE 500
#define COUNTDOWN 500

//Commands are defined here
#define CMD_GET_BPM 1
#define CMD_GET_BTN_STATES 2
#define CMD_START_PLAYING 5
#define CMD_FORCE_STOP 6

const int bpm[] = {116, 200, 190};

volatile bool received = false;
volatile bool btn1C = false;
volatile bool btn2C = false;
volatile int btn1CountDown = 0;
volatile int btn2CountDown = 0;
volatile bool locked = false;
unsigned long lastResetTime;

const int notes[][PERSIZE] PROGMEM = {
  {
    REST, REST, REST, REST,
  
    N_DS4, N_CS4, N_DS4, N_GS4, N_FS4, N_DS4, N_CS4, N_B3,
    N_CS4, N_DS4, N_CS4, N_B3, N_CS4, N_DS4, N_CS4, N_B3,
    N_DS4, N_CS4, N_DS4, N_GS4, N_FS4, N_B3, N_B3, N_B3,
    N_CS4, N_DS4, N_CS4, N_B3, N_AS3, N_B3, REST,
    N_B4, N_B4, N_CS5, N_B4, N_B4, N_CS5,
    N_DS5, N_CS5, N_B4, N_CS5, N_B4,
    N_DS4, N_CS4, N_DS4, N_GS4, N_FS4, N_B3, N_B3,
    N_CS4, N_DS4, N_CS4, N_B3, N_AS3, N_B3, N_CS5, N_B4,
  
    N_CS4, N_DS4, N_FS4, N_B4, N_AS4, N_FS4, N_CS4, N_DS4, N_B3,
    N_FS4, N_B3, N_DS4, N_FS4, N_CS4, N_DS4, N_CS4, N_B3, N_FS3,
    N_CS4, N_DS4, N_FS4, N_B4, N_AS4, N_FS4, N_CS4, N_DS4, N_B3,
    N_B3, N_CS4, N_DS4, N_FS4, N_B4, N_CS5, N_B4, N_FS4, N_DS5, N_E5, N_DS5, N_B4,
  
    N_DS4, N_DS4, N_GS4, N_FS4, N_B3, N_B3, N_B3,
    N_CS4, N_DS4, N_CS4, N_B3, N_CS4, N_DS4, REST,
    N_DS4, N_DS4, N_GS4, N_FS4, N_B3, N_B3, N_B3,
    N_CS4, N_DS4, N_CS4, N_B3, N_AS3, N_B3, REST,
    N_B4, N_B4, N_CS5, N_B4, N_B4, N_CS5,
    N_DS5, N_CS5, N_B4, N_CS5, N_B4,
    N_DS4, N_CS4, N_DS4, N_GS4, N_FS4, N_B3,
    N_CS5, N_DS5, N_CS5, N_B4, N_AS4, N_B4, N_DS5, N_CS5,
  
    N_B4, N_DS5, N_CS5, N_B4, N_DS5, N_CS5,
    N_B4, N_FS4, N_AS4, N_B4, N_DS5, N_CS5,
    N_B4, N_DS4, N_CS5, N_B4, N_FS5, N_GS5,
    N_FS5, N_B4, N_B4, N_CS5, N_DS5, N_DS5, N_CS5,
    N_B4, N_DS5, N_CS5, N_B4, N_DS5, N_CS5,
    N_B4, N_B4, N_FS4, N_AS4, N_B4, N_DS5, N_CS5,
    N_B4, N_DS5, N_CS5, N_B4, N_FS5, N_GS5,
    N_FS5, N_B4, N_B4, N_CS5, N_B4, REST,
  
    REST, REST, REST, REST,
  },
  {
    REST, REST, REST, REST,
    N_G5, N_E5, N_D5, N_G5, N_E5, N_D5, N_G5, N_E5, N_C5, 
    REST, N_G4, N_A4, N_E5, N_D5, N_C5, N_D5, N_C5, N_G5, N_E5,
    N_E5, N_G5, N_A5, N_G5, N_E5, N_D5, N_C5, N_G5,
    N_A5, N_G5, N_E5, N_D5, N_C5, N_G4,
    N_C5, N_E5, N_D5, N_C5, N_E5, N_D5, N_C5,
    N_A2, N_C4, N_D4, N_G4,
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4,
    N_C4, N_C4, N_C4, N_C4, N_C4, N_C4, N_E4, N_D4, N_C4, N_C4, N_C4,
    N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4, N_B3, N_B3,
    N_C4, N_E4, N_D4, N_C4, N_C4, N_C4, N_C4,
    N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4, N_G3, N_G3,
    N_C4, N_C4, N_C4, N_C4, N_C4, N_C4, N_E4, N_D4, N_C4, N_C4, N_C4, N_C4,
    N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4, N_G3,
    N_C4, N_E4, N_D4, N_C4, N_C4,
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4,
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4, 
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_E4, N_D4, N_C4,
    N_D4, N_E4, N_E4, N_C4, N_B3, N_C4, N_C4,
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_D4, N_E4, N_D4, N_C4, N_C4, 
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_D4, N_D4, N_E4, N_D4, N_C4, 
    N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_D4, N_E4, N_D4, N_C4, 
    N_D4, N_D4, N_D4, N_D4, N_D4, N_C4, N_D4, N_E4, N_G4, N_G4, N_C5, N_C5,
  
    N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_C5, N_D5, N_D5, N_E5, N_A4, N_C5,
    N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_E5, N_D5, N_C5, N_C5, N_G4, N_G4, N_C5, N_C5, 
    N_D5, N_D5, N_D5, N_C5, N_D5, N_D5, N_D5, N_D5, N_D5, N_E5, N_C5, 
    N_G5, N_E5, N_E5, N_D5, N_D5, N_D5, N_E5, N_D5, N_C5, N_G4, N_G4, N_C5, N_C5,
    
    N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_C5, N_D5, N_D5, N_E5, N_C5, N_C5, 
    N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_E5, N_D5, N_C5, N_C5, N_C5, N_C5,
    N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_D5, N_C5, N_D5, N_C5, N_C5, N_G5, N_E5,
    N_D5, N_D5, N_D5, N_C5, N_D5, N_D5, N_D5, N_E5, N_D5, N_C5, N_C5,

    REST, REST, REST, REST
 },
 {
    REST, REST, REST, REST,

    N_AF4, N_BF4, N_AF4, N_EF5, N_AF4, N_BF4, N_AF4,
    N_EF5, N_AF4, N_BF4, N_AF4, N_EF5, N_AF4, N_BF4,
    N_AF4, N_BF4, N_AF4, N_EF5, N_AF4, N_BF4, N_AF4,
    N_EF5, N_AF4, N_BF4, N_AF4, N_EF5, N_AF4, N_BF4,
    N_BF4, N_C5, N_EF5,
    N_BF4, N_EF5, N_AF5, N_BF5,
    N_G4, N_AF4, N_EF5,
    N_C5,
    N_BF4, N_C5, N_AF4,
    N_G4, N_AF4, N_F5,
    N_EF5, N_AF4, N_AF4,
    N_G4, N_AF4, N_BF4, N_BF4,
    N_AF4, N_AF4, N_AF4, N_AF4, N_G4, N_F4, N_EF4,
    N_F4, N_EF4, N_EF4,
    N_D4, N_EF4, N_EF4, N_EF4, N_EF4, N_DF4, N_C4, N_BF3,
    N_BF3, N_EF4, N_C4,
    N_AF3, N_AF3, N_EF4, N_EF4, N_EF4, N_EF4, N_AF4, N_AF4,
    N_BF4, N_AF4, N_AF4, N_G4, N_AF4,
    N_BF4, N_C5, N_BF4, N_AF4, N_G4, N_AF4,

    N_AF4, N_AF4, N_AF4, N_AF4, N_AF4, N_G4, N_F4, N_EF4,
    N_F4, N_EF4, N_EF4,
    N_BF3, N_BF3, N_EF4, N_EF4, N_EF4, N_DF4, N_C4, N_BF3,
    N_BF3, N_EF4, N_C4,
    N_AF3, N_AF3, N_EF4, N_EF4, N_EF4, N_AF4, N_AF4,
    N_BF4, N_AF4, N_AF4, N_EF4, N_EF4, N_AF4,
    N_BF4, N_C5, N_BF4, N_AF4, N_G4, N_AF4,

    N_AF4, N_EF4, N_BF4, N_AF4, N_C5,
    N_C5, N_DF5, N_DF5, N_C5, N_BF4, N_AF4,
    N_AF4, N_EF4, N_BF4, N_AF4, N_G4,
    N_AF4, N_BF4, N_AF4,
    N_AF4, N_EF4, N_BF4, N_AF4, N_C5,
    N_C5, N_DF5, N_C5, N_BF4, N_AF4,
    N_AF4, N_F4, N_G4, N_AF4, N_F4, N_AF4, N_C5,
    N_BF4, N_BF4, N_BF4, N_AF4, N_BF4, N_C5,
    N_EF4, N_AF4, N_EF5,

    N_BF4, N_C5, N_BF4, N_AF4,
    N_BF4, N_C5, N_BF4, N_AF4,
    N_BF4, N_C5, N_BF4, N_C5, N_DF5,
    N_DF5, N_EF5, N_DF5, N_C5, N_AF4,
    N_BF4, N_C5, N_AF4,
    N_BF4, N_C5, N_AF4,
    N_BF4, N_AF4, N_BF4, N_C5, N_DF5,
    N_DF5, N_C5, N_BF4, N_AF4, 
    N_AF4, N_AF4, N_EF5, N_EF5, N_EF5, N_AF4, N_AF4, N_AF4,
    N_G4, N_AF4, N_F5, N_EF5, N_AF4,
    N_EF5, N_EF5, N_EF5, N_EF5, N_DF5,
    N_C5, N_DF5, N_C5, N_BF4, N_AF4,
    N_DF4, N_F4, N_G4, N_AF4,
    N_BF4, N_G4, N_AF4, N_BF4, N_F4,
    N_C5, N_C5, N_C5, N_C5, N_DF5, N_C5,
    N_BF4, N_AF4,
    N_EF4, N_AF4, N_EF5,
    N_BF4, N_AF4, N_BF4, N_AF4, N_C5,

    N_AF4, N_AF4, N_G4, N_G4, N_EF4, N_EF4,
    N_EF4, N_F4,
    N_G4, N_G4, N_BF4, N_BF4, N_AF4, N_AF4,
    //
    N_AF4, N_AF4, N_G4, N_G4, N_EF4, N_EF4,
    N_EF4, N_F4,
    N_G4, N_G4, N_BF4, N_BF4, N_AF4, N_AF4,
    //
    N_AF4, N_AF4, N_G4, N_G4, N_AF4, N_AF4,
    N_BF4, N_BF4, N_BF4, N_AF4, N_AF4, N_BF4, N_BF4,
    N_EF5, N_EF5, N_EF5, N_DF5, N_DF5, N_DF5, N_C5, N_C5,
    N_AF4, N_AF4, N_DF5, N_C5, N_AF4,
    N_DF4, N_DF4, N_F4, N_F4, N_F4, N_F4,
    N_EF4, N_EF4, N_G4, N_G4, N_G4, N_G4,
    N_F4, N_F4, N_AF4, N_AF4, N_AF4, N_AF4,
    N_G4, N_AF4, N_AF4, N_BF4, N_BF4, N_EF5, N_EF5,
    N_EF5, N_EF5, N_F5, N_F5, N_G5, N_G5, N_AF5, N_AF5,
    N_BF5, N_EF4, N_AF4, N_EF5,

    N_BF4, N_C5, N_BF4, N_AF4,
    N_BF4, N_C5, N_BF4, N_AF4,
    N_BF4, N_C5, N_BF4, N_C5, N_DF5,
    N_DF5, N_EF5, N_DF5, N_C5, N_AF4,
    N_BF4, N_C5, N_AF4,
    N_BF4, N_C5, N_AF4,
    N_BF4, N_AF4, N_BF4, N_C5, N_DF5,
    N_DF5, N_C5, N_BF4, N_AF4,
    N_AF4, N_AF4, N_EF5, N_EF5, N_EF5, N_AF4, N_AF4, N_AF4,
    N_G4, N_AF4, N_F5, N_EF5, N_AF4,
    N_EF5, N_EF5, N_EF5, N_EF5, N_DF5,
    N_C5, N_DF5, N_C5, N_BF4, N_AF4,
    N_DF4, N_F4, N_G4, N_AF4,
    N_BF4, N_G4, N_AF4, N_BF4, N_F4,
    N_C5, N_C5, N_C5, N_C5, N_DF5, N_C5,
    N_BF4, N_AF4,
    N_DF4, N_EF4, N_AF4, N_EF5,
    N_BF4, N_AF4, N_BF4, N_AF4, N_C5,

    N_BF4, N_C5, N_EF5,
    N_BF4, N_EF5, N_AF5, N_BF5,
    N_G4, N_AF4, N_EF5,
    N_C5,
    N_BF4, N_C5, N_AF4,
    N_G4, N_AF4, N_F5,
    N_EF5, N_AF4,
    N_G4, N_AF4, N_BF4,
    N_AF4,

    REST, REST, REST, REST
 }
};

const float beats[][PERSIZE] PROGMEM = {
  {
    1, 1, 1, 1,
  
    0.75, 0.25, 0.5, 0.5, 1, 0.25, 0.25, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
    0.75, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 1, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 2,
    0.75, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
  
    0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 1,
    0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25,
  
    1, 0.75, 0.25, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 0.75, 0.25, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 1, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 2,
    0.75, 0.25, 0.5, 0.5, 1, 1,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
  
    1, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 1,
  
    1, 1, 1, 1, 
    
    -1
  },
  {
    1, 1, 1, 1,
    1, 1, 1.5, 1.5, 1, 1.5, 1.5, 1, 4,
    1, 1, 1, 1, 3, 1, 1.5, 1.5, 1, 6,
    1, 1, 1.5, 1.5, 1.5, 1.5, 1, 1,
    1.5, 1.5, 1.5, 1.5, 1, 1,
    1.5, 1.5, 1, 1.5, 1.5, 1, 4,
    1, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 3,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 1,
    1, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 2, 0.5, 0.5,
    1, 0.5, 1, 0.5, 3, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5,
    0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 2, 1,
    1, 0.5, 1, 0.5, 5,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    1.5, 0.5, 1.5, 0.5, 1.5, 0.5, 2,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1,
    1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5,
  
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 1,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5, 
    
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3,

    1, 1, 1, 1,

    -1
  },
  {
    1, 1, 1, 1,

    0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1.5, 1.5, 1,

    1, 1, 1, 1,
    1.5, 1.5, 1,
    4,
    1.5, 1.5, 1,
    1.5, 1.5, 1,
    1.5, 2, 0.5,
    1, 1, 0.5, 1.5,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 1.5, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 4.5,

    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 4.5,

    0.5, 1, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 1, 1, 1, 1,
    0.5, 0.5, 2.5,
    0.5, 1, 1, 1, 1,
    0.5, 1, 0.75, 0.75, 0.5,
    2, 1, 1,
    1, 1, 1, 1,
    1.5, 0.5, 0.5, 0.5, 0.5, 5.5,
    1, 1, 1,
    1, 2, 0.5, 0.5, 
    1, 2, 0.5, 0.5,
    1, 1, 0.5, 1, 1,
    0.5, 1, 0.5, 0.5, 1,
    1, 2, 1,
    1, 2, 1,
    1, 0.5, 1, 1, 1,
    0.5, 1, 0.5, 1.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 1, 1, 0.5,
    1, 0.5, 0.5, 1, 1,
    1, 0.5, 1, 0.5, 1,
    1, 1, 1, 1,
    1, 1, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 4,
    1, 1, 1,
    1, 0.5, 1, 0.5, 1,

    0.5, 1, 0.5, 1, 0.5, 2.5,
    1, 1,
    0.5, 1, 0.5, 1, 0.5, 4.5,
    //
    0.5, 1, 0.5, 1, 0.5, 2.5,
    1, 1,
    0.5, 1, 0.5, 1, 0.5, 4.5,
    //
    1, 0.5, 1, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1.5, 0.5, 0.5, 0.5, 1,
    0.5, 1, 1, 0.5, 0.5, 0.5,
    0.5, 1, 1, 0.5, 0.5, 0.5,
    0.5, 1, 1, 0.5, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 1, 1, 1,

    1, 2, 0.5, 0.5,
    1, 2, 0.5, 0.5,
    1, 1, 0.5, 1, 1, 
    0.5, 1, 0.5, 0.5, 1,
    1, 2, 1,
    1, 2, 1,
    1, 0.5, 1, 1, 1,
    0.5, 1, 0.5, 1.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 1, 1, 0.5,
    1, 0.5, 0.5, 1, 1,
    1, 0.5, 1, 0.5, 1,
    1, 1, 1, 1,
    1, 1, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 3,
    1, 1, 1, 1,
    1, 0.5, 1, 0.5, 1,
    1.5, 1.5, 1,
    1, 1, 1, 1,
    1.5, 1.5, 1,
    4,
    1.5, 1.5, 1,
    1.5, 1.5, 1,
    1.5, 2.5,
    1.5, 1, 1.5,
    4,

    1, 1, 1, 1,

    -1
  }
};

const int notesLen = sizeof(notes) / (sizeof(int) * PERSIZE);

volatile bool stopPlaying = false;
volatile bool readyToPlay = false;
volatile bool isPlaying = false;
volatile int songId = 0;
volatile int idx = 0;

volatile bool masterWaitingForSelfCheckResult = false;
volatile bool masterWaitingForBPM = false;
volatile bool masterWaitingForBtnStates = false;

volatile unsigned long playTime;
volatile unsigned int freq;

/*
void test() {
  for(int i=0;i<PERSIZE;++i) {
    float beat = pgm_read_float(beats[2] + i);
    if(beat < 0) {
      noTone(BUZZER_PIN);
      break;
    }
    int note = pgm_read_word(notes[2] + i);
    float playTime = 1000 * beat / bpm[2] * 60;
    tone(BUZZER_PIN, note, playTime-20);
    delay(playTime);
  }
}
*/

void receiveEvent(int bytes) {
  byte command = Wire.read();
  received = true;
  if (command == CMD_GET_BPM)
    masterWaitingForBPM = true;
  if (command == CMD_START_PLAYING) {
    if (Wire.available()) {
      songId = Wire.read();
      idx = 0;
      readyToPlay = true;
    }
  }
  if (command == CMD_GET_BTN_STATES) 
    masterWaitingForBtnStates = true;
  if(command == CMD_FORCE_STOP) {
    noTone(BUZZER_PIN);
    isPlaying = false;
    idx = 0;
  }
  while(Wire.available())
    Wire.read();
}

void requestEvent() {
  if (masterWaitingForBPM) {
    masterWaitingForBPM = false;
    Wire.write(bpm[songId]);
  }
  if(masterWaitingForBtnStates) {
    masterWaitingForBtnStates = false;
    bool btn1 = !digitalRead(BUTTON1_PIN);
    bool btn2 = !digitalRead(BUTTON2_PIN);
    char btnStates[] = {btn1, btn2};
    Wire.write(btnStates, 2);
    btn1C = false;
    btn2C = false;
  }
}

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  if(isPlaying) {
    float beat = pgm_read_float(beats[songId] + idx);
    unsigned long playTime = 1000 * beat / bpm[songId] * 60 - 20;
    freq = pgm_read_word(notes[songId] + idx);
    tone(BUZZER_PIN, freq, playTime);
    unsigned int timerTime = 15625 / (float)bpm[songId] * 60 * beat;
    OCR1A = timerTime;
    ++idx;
    if (beat < 0 || freq < 0) {
      isPlaying = false;
      idx = 0;
    }
  }
}

void p2Btn1Clicked() {
  if (btn1CountDown)
    return;
  btn1C = true;
  btn1CountDown = COUNTDOWN;
}

void p2Btn2Clicked() {
  if (btn2CountDown)
    return;
  btn2C = true;
  btn2CountDown = COUNTDOWN;
}

void timerSetup () {
  cli();
  TCNT1 = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000101;
  TIMSK1 |= B00000010;
  OCR1A = 1;
  sei();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), p2Btn1Clicked, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), p2Btn2Clicked, FALLING);
  Wire.begin(1);
  Wire.setClock(34000);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  lastResetTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(readyToPlay) {
    btn1C = false;
    btn2C = false;
    readyToPlay = false;
    timerSetup();
    isPlaying = true;
  }
  --btn1CountDown;
  --btn2CountDown;
  /*
  if (millis() - lastResetTime > 100) {
    lastResetTime = millis();
    btn1C = false;
    btn2C = false;
  }
  */

}
