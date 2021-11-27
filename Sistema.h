void SD_CARD(){
  digitalWrite(5, HIGH); // SD Card
}

void setButton(){
  RotaryEncoder encoder1(27, 14,RotaryEncoder::LatchMode::TWO03); //share cinza 14 branco 27
  RotaryEncoder encoder2(12, 13,RotaryEncoder::LatchMode::TWO03); //ps amarelo 13 verde 12
  RotaryEncoder encoder3(32, 33,RotaryEncoder::LatchMode::TWO03); //option cinza 33 branco 32
}

int Encoder(RotaryEncoder encoder){
  static int pos =0;
  encoder.tick(); 
  int newPos = encoder.getPosition();
  if (pos != newPos){
   pos = newPos;
   return newPos;   
  }
}

void setColor(int R, int G, int B) {
  ledcWrite(PIN_RED,   R);
  ledcWrite(PIN_GREEN, G);
  ledcWrite(PIN_BLUE,  B);
}

void Aceleracao(int pinLed, float valor){
  dacWrite(pinLed,valor);// Ligar na porta D25 DCA
}

void setLED(){
 pinMode(25,OUTPUT); // Set Led Aceleração Pin 25
 pinMode(26,OUTPUT); // Set Led RED
 pinMode(3,OUTPUT); // Set Led GREEN
 pinMode(22,OUTPUT); // Set Led BLUE
 ledcAttachPin(26, PIN_RED);//Atribuimos o pino 2 ao canal 0.
 ledcAttachPin(3, PIN_GREEN);//Atribuimos o pino 2 ao canal 0.
 ledcAttachPin(22, PIN_BLUE);//Atribuimos o pino 2 ao canal 0.
 ledcSetup(PIN_RED, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
 ledcSetup(PIN_GREEN, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
 ledcSetup(PIN_BLUE, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
}

void TxtString(String text){
  int wordStart = 0;//35
  int wordEnd = 0;
  int lin;
  lin = ((text.length()*20)/380)*10;
  lin =  ((tft.height()/2) - lin)+10;
  if (text.length() > 35){
   while ( (text.indexOf('/', wordStart) >= 0) && ( wordStart <= text.length())) {
    wordEnd = text.indexOf('/', wordStart + 1);
    uint16_t len = tft.textWidth(text.substring(wordStart, wordEnd));
    if (tft.getCursorX() + len >= tft.width()) {
      tft.println();
      if (wordStart > 0) wordStart++;
    }
    tft.setCursor(50,lin);
    tft.print(text.substring(wordStart, wordEnd));
    wordStart = wordEnd;
    lin=lin+20;
   }
  }
  else{
   tft.setCursor(50,lin);
   tft.print(text); 
  }
}

void Janela(String info){
  tft.fillRect(40,95,400,150,TFT_LIGHTGREY);
  tft.drawRect(40,95,400,150,TFT_WHITE);
  tft.fillRect(41,96,398,20,TFT_BLUE);
  tft.drawLine(41, 116, 439, 116, TFT_WHITE); 
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLUE);
  tft.drawString(info,45,97,2); 
}

void Msg_Open(String info, String mensagem){
//Definindo Tela Alerta------------------------------ 
  
  Janela(info); 
  tft.setTextColor(TFT_BLACK,TFT_LIGHTGREY);
  tft.setTextSize(1);
  tft.setTextFont(4);
  TxtString(mensagem);
}

void Msg_Close(int tempo){
  delay(tempo);
  tft.fillRect(40,95,400,130,TFT_BLACK);
}

String TempoVolta(float volta){
    unsigned long currentMillis;
    unsigned long seconds;
    unsigned long minutes;
    String readableTime;
    if ( volta > 0){
     currentMillis = volta;
     seconds = currentMillis / 1000;
     minutes = seconds / 60;
     currentMillis %= 1000;
     seconds %= 60;
     minutes %= 60;
     if (minutes < 10) {
     readableTime += "0";
      }
     readableTime += String(minutes) + ":";

     if (seconds < 10) {
      readableTime += "0";
     }  

     readableTime += String(seconds) + ","; 
     if (currentMillis < 10) {
      readableTime += "00";
     } 
     if (currentMillis < 100) {
      readableTime += "0";
     }       
     readableTime += String(currentMillis); 
    }
    else{
     readableTime="00:00,000";
    }
    return  readableTime;
}
void DisplaySCDelta(float delta){

    String str_delta;

    if (delta > 0)
    {
      str_delta += "+";
      //tft.setTextColor(TFT_WHITE, 0x0725);
    }
    else
      //tft.setTextColor(TFT_WHITE, TFT_RED);


    str_delta += floor(delta * 10.f) / 10.f;;
    str_delta += "s";

    //WriteCentered(240, 25, str_delta, 3);

   // tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
}
  
/* Load Imagem */

void renderJPEG(int xpos, int ypos) {
  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;
  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);
  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;
  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();
  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;
  // read each MCU block until there are no more
  while (JpegDec.read()) {    
    // save a pointer to the image block
    pImg = JpegDec.pImage ;
    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;
    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;
    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;
    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }
    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;
    tft.startWrite();
    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
    {
      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setAddrWindow(mcu_x, mcu_y, win_w, win_h);
      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        // Push each pixel to the TFT MCU area
        tft.pushColor(*pImg++);
      }
    }
    else if ( (mcu_y + win_h) >= tft.height()) JpegDec.abort(); // Image has run off bottom of screen so abort decoding
    tft.endWrite();
  }
  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;
}

void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos) {

  int x = xpos;
  int y = ypos;

  JpegDec.decodeArray(arrayname, array_size);
  
  renderJPEG(x, y);  
}

void LogoTipo(){
  //Inicia Logo Tipo
  drawArrayJpeg(F1_Logo, sizeof(F1_Logo), 0, 0);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_TRANSPARENT);
  tft.drawString("F1 Telemetria - versao 1.21",10,10,1);
}
