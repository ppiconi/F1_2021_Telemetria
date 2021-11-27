//Graficos Padrão do aplicativo F1 Telemetria
class Padrao { //Inicio da Classe

public:
 
 void Modulos(){
  //Speed
  tft.fillRect(10,10,180,40,TFT_LIGHTGREY);
  // Laps and Times
  tft.drawRect(10, 50, 180, 90, TFT_LIGHTGREY);
  //Best Time Lap
  tft.fillRect(290,10,180,40,TFT_LIGHTGREY); 
  //Tyres
  tft.drawRect(290, 50, 180, 90, TFT_LIGHTGREY);
  //ERS Energy
  tft.drawRect(55,243, 370, 30, TFT_LIGHTGREY);
  //Raio
  tft.fillTriangle(11,248,20,248,8,258,TFT_YELLOW);
  tft.fillTriangle(8,258,12,258,20,248,TFT_YELLOW);
  tft.fillTriangle(12,256,22,256,8,266,TFT_YELLOW);
  //ERS D
  tft.drawRect(55, 278, 170, 30, TFT_LIGHTGREY);
  //MGU-H
  tft.drawRect(255, 278, 170, 30, TFT_LIGHTGREY); 
 }
 
 void Speed(uint16_t Kmh){
 //Definindo Tela Speed------------------------------ 
  tft.fillRect(10,10,180,40,TFT_LIGHTGREY);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE,TFT_LIGHTGREY);
  tft.drawRightString(" " + String(Kmh) + " KM/H",165,14,2);  
 }
 
 void Lap(int CurrentLap, uint8_t CarPosition, float CurrentLapTime, float LastLapTime, float BestLapTime){
 //Definindo Tela Lap  
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  //Current Lap  
  tft.drawString("L" + String(CurrentLap) ,16,60,4); 
  // Position Car
  tft.drawRightString("P" + String(CarPosition),186,60,4); 
  // Last Lap Time
  LastLapTime = LastLapTime - BestLapTime;
  tft.drawString(String(LastLapTime/1000,2),16,100,4); 
  // Current Lap Time
  CurrentLapTime = CurrentLapTime - LastLapTime;
  if (CurrentLapTime > 0) {
   tft.setTextColor(TFT_RED,TFT_BLACK); 
   tft.drawRightString("(+" + String(CurrentLapTime/1000,2) + ")",186,100,4); 
  }
  else{
   tft.setTextColor(TFT_GREEN,TFT_BLACK);
   tft.drawRightString("(" + String(CurrentLapTime/1000,2) + ")",186,100,4);   
  }
 }

 void Fecha_Lap(){
  tft.fillRect(11, 51, 178, 88, TFT_BLACK);
 }

 void Gear(int gear){ 
 //Definindo Tela Gear------------------------------
  gr.setColorDepth(8);
  gr.createSprite(99, 130);
  gr.fillSprite(TFT_BLACK);      
  gr.setTextColor(TFT_WHITE,TFT_BLACK); 
  gr.setTextSize(6);
  switch (gear) {
   case 0:     
    gr.drawCentreString("N",gr.width()/2,10,4);
    break;
   case -1:
    gr.drawCentreString("R",gr.width()/2-2,10,4);
    break;
   default:
    gr.drawCentreString(String(gear),gr.width()/2,10,4);
    break; 
  }
  gr.pushSprite(191,5);
  gr.deleteSprite(); 
 }

 void TimerLap(float timerlap){ 
 //Definindo Tela TimerLap---------------------------- 
  tft.fillRect(290,10,180,40,TFT_LIGHTGREY);   
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE,TFT_LIGHTGREY);
  tft.drawCentreString(TempoVolta(timerlap),380,14,2);//DisplayBestTime(timerlap)
 }



 void Tyres(int FrontLeft, int FrontRight, int RearLeft, int RearRight){   
 //Definindo Tela Tyres
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  // Front Left 
  tft.drawString(String(FrontLeft) +"'C " ,296,60,4); 
  // Front Right
  tft.drawRightString(" " + String(FrontRight) +"'C",464,60,4); 
  // Rear Left
  tft.drawString(String(RearLeft) +"'C ",296,100,4); 
  // Rear Right
  tft.drawRightString(" " + String(RearRight) +"'C",464,100,4);
 }

 void Fecha_Tyres(){
  tft.fillRect(291, 51, 178, 88, TFT_BLACK);
 }

 void Energy(int ersmode, int energy, int dlap, int mguh){
 //Definindo Tela Energy------------------------------
  //ERS deployment mode
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK); 
  tft.drawRightString(String(ersmode),50,248,4);
  //ERS energy store in Joules
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  int porcentagem = energy / 3.7;
  tft.drawRightString(" " + String(porcentagem) + "%",475,251,1);
  tft.fillRect(56, 244, energy, 28, TFT_YELLOW);
  tft.fillRect(56 + energy, 244, 368 - energy, 28, TFT_BLACK);  
  // ERS energy Deployed this lap
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.drawRightString("D",50,283,4);
  tft.fillRect(56, 279, dlap, 28, TFT_BLACK);
  tft.fillRect(56 + dlap, 279, 168 - dlap, 28, TFT_WHITE);  
  // ERS energy harvested this lap by MGU-H 
  tft.setTextSize(1); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.drawString("H", 430,283, 4);      
  tft.fillRect(256, 279, mguh , 28, TFT_WHITE);
  tft.fillRect(256 + mguh, 279 ,168 - mguh, 28, TFT_BLACK);
 }

 void Bandeira(String msg, int Color){
  //Definindo Tela Bandeira------------------------------
  flag.setColorDepth(8);
  flag.createSprite(400, 60);
  flag.fillSprite(Color);      
  flag.setTextSize(2);
  flag.setTextColor(TFT_WHITE,Color);
  flag.drawCentreString(msg,flag.width()/2,10,4); 
  flag.pushSprite(40,tft.height()/2);
  flag.deleteSprite();  
 }

 void CloseFlag(){
 //Fecha Bandeira 
  tft.fillRect(40,tft.height()/2,400,60,TFT_BLACK);
 }

 void InLap(float CurrentLap){
   tft.setTextSize(2);
   tft.setTextColor(TFT_GREEN,TFT_BLACK);
   tft.drawCentreString(TempoVolta(CurrentLap),tft.width()/2,tft.height()/2,4);
 }
 
 void SectorLap(int sector, float CurrentLap){
  //Definindo Tela Bandeira------------------------------
  int Color;
  tft.setTextSize(2);
  if (CurrentLap > 0.000){
    Color = TFT_RED;
  }
  else{
    Color = TFT_GREEN;
  } 
   tft.setTextColor(Color,TFT_BLACK);
   tft.drawCentreString("(" + String((CurrentLap/1000),3) + ")",tft.width()/2,tft.height()/2,4);
 }

}; //Fim da Classe Padrão
