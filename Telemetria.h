// Telemetria F1 2021
Padrao Grafico;

String Sessao[13]={"TOMADA DE TEMPO","TREINO LIVRE - P1","TREINO LIVRE - P2","TREINO LIVRE - P3","TREINO LIVRE CURTO - P5",
                   "CLASSIFICACAO - Q1","CLASSIFICACAO - Q2","CLASSIFICACAO - Q3","CLASSIFICACAO CURTA","OSQ10",
                   "RACE","RACE 2","TESTE DE TEMPO"};
                   
String PitStatus[3]={"NOME","IN PIT","IN PIT AREA"};

String DriveStatus[5]={"IN GARAGE","FLAYING LAP","IN LAP","OUT LAP","ON TRACK"};

void FlagZone(){
  struct MarshalZone *mz; mz = (struct MarshalZone *)&packetBuffer;
  switch (mz->m_zoneFlag){
    case 1: // GREEN
     setColor(0, 255, 0);
     break;
    case 2: // BLUE
     setColor(0, 0, 255);
     break;
    case 3: // YELLOW
     setColor(255, 255, 0);
     break;
    case 4: // RED
     setColor(255, 0, 0); 
     default: //OFF
     setColor(0, 0, 0);
    break;
  }
}

void Session_Data(){
    struct PacketSessionData *sd; sd = (struct PacketSessionData *)&packetBuffer;
    TotalLap=sd->m_totalLaps;
    Session=sd->m_sessionType;
    //Grafico.Bandeira(Sessao[Session],TFT_BLACK);
    //TimerFlag=0;  
}

void Lap_Data(uint8_t myCar){
    struct PacketLapData *L; L = (struct PacketLapData *)&packetBuffer;
    struct LapData *lapdata = &(L->m_lapData[myCar]);
    //Grafico.Bandeira(PitStatus[lapdata->m_pitStatus],TFT_BLACK) ; 
    
    if (lapdata->m_driverStatus==0){
     if (logo==0) {
       tft.fillScreen(TFT_BLACK);        
       //Inicializa os Modulos
       Grafico.Modulos();
       logo=1;
     }
     TimerFlag=0;
     SecFlag=lapdata->m_sector;
     Pit=1;
     FlagStart=0;
     InLap=lapdata->m_currentLapNum-1;
     CarPos=lapdata->m_carPosition;
     Grafico.Bandeira(DriveStatus[lapdata->m_driverStatus],TFT_BLACK);
    }
    else{
      if (Pit==1) {
       Pit=0;
       Grafico.CloseFlag();
       SaidaPit=0;
       SecFlag=3;
      }
      if (logo==0) {
       tft.fillScreen(TFT_BLACK);        
       //Inicializa os Modulos
       Grafico.Modulos();
       logo=1;
      }
      VoltaRapida = lapdata->m_currentLapTimeInMS;
    }
                       
    switch (lapdata->m_sector) {
     default:{
        if (SecFlag == 3){
         Grafico.SectorLap(1,lapdata->m_currentLapTimeInMS);
         FlagStart=1;
         SecFlag = 1;
         SaidaPit=1;
         InLap=lapdata->m_currentLapNum;
         CarPos=lapdata->m_carPosition;
         if (LastLap < BestLap){
          BestLap=LastLap;  
         }      
         LastLap=lapdata->m_lastLapTimeInMS;         
         CurrentLap=lapdata->m_currentLapTimeInMS;
         Grafico.Fecha_Lap();           
        }
      }
      break;
     case 1:{
       if (SecFlag == 1){
         Grafico.SectorLap(2,lapdata->m_sector1TimeInMS);
         FlagStart=1;
         SecFlag=2;
       } 
      }
      break;
     case 2:{
       if (SecFlag == 2){
           SecFlag=3;
           if (SaidaPit == 1){
            Grafico.SectorLap(3,lapdata->m_sector2TimeInMS);
            FlagStart=1;
          }      
        } 
       }
      break;
    }  
}

void Car_Telemetria(uint8_t myCar){
    struct PacketCarTelemetryData *p; p = (struct PacketCarTelemetryData *)&packetBuffer;
    struct CarTelemetryData *telemetry = &(p->m_carTelemetryData[myCar]);
    myGear     = telemetry->m_gear;
    mySpeed    = telemetry->m_speed; 
    if (myTyres[0] != telemetry->m_tyresSurfaceTemperature[0]) {
      FT=1;
    }
    if (myTyres[1] != telemetry->m_tyresSurfaceTemperature[1]) {
      FT=1;
    }
    if (myTyres[2] != telemetry->m_tyresSurfaceTemperature[2]) {
      FT=1;
    }

    if (myTyres[3] != telemetry->m_tyresSurfaceTemperature[3]) {
      FT=1;
    }
    myTyres[0] = telemetry->m_tyresSurfaceTemperature[0];
    myTyres[1] = telemetry->m_tyresSurfaceTemperature[1];
    myTyres[2] = telemetry->m_tyresSurfaceTemperature[2];
    myTyres[3] = telemetry->m_tyresSurfaceTemperature[3]; 
    Acelerar = telemetry->m_revLightsPercent * 2.55; 
};

void Car_Status(uint8_t myCar){
    struct PacketCarStatusData *c; c = (struct PacketCarStatusData *)&packetBuffer;
    struct CarStatusData *carstatus = &(c->m_carStatusData[myCar]);   
                      
    if (carstatus->m_ersStoreEnergy > 0) {
      ERS = carstatus->m_ersStoreEnergy / 370;
      ERS = carstatus->m_ersStoreEnergy / ERS;
    }
    else{
      ERS=0;
    }
    if (ERS > 368) {
      ERS = 338;
    }
    if (carstatus->m_ersDeployedThisLap > 0){
      D_Lap = carstatus->m_ersDeployedThisLap / 170;
      D_Lap = carstatus->m_ersDeployedThisLap / D_Lap;
    }
    else{
      D_Lap=0;
     }
    if (D_Lap > 168) {
      D_Lap = 168;
    }
    if (carstatus->m_ersHarvestedThisLapMGUH > 0) {
      MGU_H = carstatus->m_ersHarvestedThisLapMGUH / 170;
      MGU_H = carstatus->m_ersHarvestedThisLapMGUH / MGU_H;
    }
    else{
      MGU_H=0;
    }
    if (MGU_H > 168) {
      MGU_H = 168;
    }  
    if (carstatus->m_ersDeployMode != ERS_Mode){
     switch (carstatus->m_ersDeployMode){
      case 1:
          Grafico.Bandeira("MEDIUM",TFT_ORANGE);
          FlagStart=1;
          ERS_Mode = 1;
        break;
      case 2:
          Grafico.Bandeira("OVERTAKE",TFT_GREEN);
          FlagStart=1;
          ERS_Mode = 2;
        break;
      default:
         ERS_Mode=0;
        break;
     } 
    }             
}

void Telemetria(){

// iniciando pacotes    
 int packetSize = Udp.parsePacket();
 //Limpa a Area central
 if (FlagStart==1){
   TimerFlag++;
   if (TimerFlag==3000) {
     Grafico.CloseFlag();
     TimerFlag=0;
     FlagStart=0;
   }
 }
 if (packetSize > 0) {
           
    int packet_length = Udp.read(packetBuffer, sizeof(packetBuffer)); 
    
    if (packet_length >= sizeof(PacketHeader)) {
      
      struct PacketHeader *header = (struct PacketHeader *)&packetBuffer;
      if (header->m_packetFormat == 2021) {
        
        switch (header->m_packetId) {
          //case 1:{ //Pacote Session
           //Session_Data(); }    
          // break;           
          case 2: {// Pacote Lap Data
            Lap_Data(header->m_playerCarIndex);
            Grafico.TimerLap(BestLap);
            Grafico.Lap(InLap,CarPos,CurrentLap,LastLap,BestLap);
            if (Pit==0) {
              if(FlagStart==0){
                if (VoltaRapida > 0) {            
                 Grafico.InLap(VoltaRapida);
                }
              }
              FlagZone();
            }
            else{
             setColor(0, 0, 0);
            }
           } 
           break;  
          case 6: {//Telemetry data for all cars
            Car_Telemetria(header->m_playerCarIndex);
            Grafico.Gear(myGear);
            Grafico.Speed(mySpeed);
            Grafico.Tyres(myTyres[0],myTyres[1],myTyres[2],myTyres[3]);
            if (FT == 1) {
              Grafico.Fecha_Tyres();
              FT=0;
            }
            Aceleracao(25,Acelerar);
           } 
           break;  
          case 7: {//Car Status Data
            Car_Status(header->m_playerCarIndex);
            Grafico.Energy(ERS_Mode,ERS,D_Lap,MGU_H);
           }      
           break;
          case 8:
              tft.fillScreen(TFT_BLACK);
              LogoTipo();
              logo=0;
           break; 
        }
      }
    }
  }
}
