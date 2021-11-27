//Bibliotecas
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <JPEGDecoder.h>
#include <HTTPClient.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <RotaryEncoder.h>

//Definições
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

//Iniciar Telas
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite gr = TFT_eSprite(&tft);
TFT_eSprite flag = TFT_eSprite(&tft);

//Declaração de Variaveis
WiFiManager wifiManager;//Objeto de manipulação do wi-fi
WiFiUDP Udp;

unsigned int localPort = 20777;
uint8_t  packetBuffer[1500];

float BestLap, CurrentLap, LastLap, VoltaRapida, Acelerar;
int InLap,CarPos,TotalLap,Session, FT;

int myGear,mySpeed,myTyres[4];
int ERS_Mode=0,ERS=0,MGU_H=0,D_Lap=0;
int ContLap = 0,LapC=0, LapL=0,logo=0,TimerFlag,FlagStart=0;
int SecFlag=0, Pit=0, SaidaPit=0,ERS_Status=0;

const int PIN_RED   = 0;
const int PIN_GREEN = 1;
const int PIN_BLUE  = 2;

//Minhas Bibliotecas
#include "F1_Logo.h"
#include "UDP_F1_2021.h"
#include "Sistema.h"
#include "Padrao.h"
#include "WiFi_Config.h"
#include "Telemetria.h"

void setup() {
//Iniciando App:
 Serial.begin(115200);
// Iniciar Led e Botôes
  setLED();
  setButton();
//Inicia Display  
  tft.init();
  tft.invertDisplay( false );
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK); 
//Inicia Logo Tipo
  LogoTipo();
//Iniciar conesão WiFi
  InitWiFi();
  StatusWiFi();
//Iniciar UDP  
  Msg_Open("F1 Telemetria - Aviso","Iniciando servidor UDP.....");  
  setColor(0, 255, 0);//GREEN  
  Udp.begin(localPort);
  Msg_Close(2000);
  setColor(0, 0, 0);//OFF
  LogoTipo(); 
}

void loop() {  
//Iniciar Telemetria
 Telemetria();  
}
