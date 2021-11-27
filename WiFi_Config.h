//callback que indica que o ESP entrou no modo AP
void configModeCallback(WiFiManager *myWiFiManager) { 
  String texto = "Entrou no modo de configuracao/ ";
  texto+= WiFi.softAPIP(), texto+= "/ "; //imprime o IP do AP
  texto+= myWiFiManager->getConfigPortalSSID(); //imprime o SSID criado da rede
  Msg_Open("F1 Telemetria - Aviso",texto);
}
//Callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback() {
  Msg_Open("F1 Telemetria - Aviso","Configuração salva");
}
//Inicia WiFi
void InitWiFi(){  
   //callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback);
  //callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFi.reconnect();
}

// WiFi_Config Menu
void Config_WiFi() {
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.println("Abertura Portal"); //Abre o portal
  wifiManager.resetSettings();       //Apaga rede salva anteriormente
  if(!wifiManager.startConfigPortal("ESP32-CONFIG", "") ){ //Nome da Rede e Senha gerada pela ESP
     Msg_Open("F1 Telemetria - Aviso","Falha ao conectar a Rede WiFi!");
     setColor(255, 0, 0);//RED
     Msg_Close(2000);
     ESP.restart(); //Reinicia ESP após não conseguir conexão na rede
  }
  else{       //Se caso conectar 
     Msg_Open("F1 Telemetria - Aviso","Conectado a Rede WiFi!");
     setColor(0, 255, 0);//GREEN
     Msg_Close(2000);
     ESP.restart(); //Reinicia ESP após conseguir conexão na rede 
   }
}

// Status WiFi
void StatusWiFi(){
  if(WiFi.status() == WL_CONNECTED){
    Msg_Open("F1 Telemetria - Aviso","Conectado a Rede WiFi!");
    setColor(0, 255, 0);//GREEN
    Msg_Close(2000);
  }
  else{
    Msg_Open("F1 Telemetria - Aviso","Rede WiFi nao conectada!");
    setColor(255, 0, 0);//RED
    Msg_Close(2000);
    Msg_Open("F1 Telemetria - Aviso","Tentanto se conectar a Rede WiFi!");
    if(!WiFi.SSID()){
      Config_WiFi();
    }
    else {
     wifiManager.autoConnect();//Função para se autoconectar na rede    
     Msg_Close(2000);
    }
  }        
}
