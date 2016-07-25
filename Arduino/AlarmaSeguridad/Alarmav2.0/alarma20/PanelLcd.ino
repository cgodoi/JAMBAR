void dimLuzLcd_nodelay(byte modo, byte duracion, byte accel)
{
  byte ledmax = 255;
  byte ledmin = 0;
  unsigned long tiempoactualled = millis();
  int valorLuz;
  if (modo == 1){//llega a 255 prende
    finmodo0 = 1;
    if (tiempoactualled - tiempoPrevioLed  > duracion){
      if ((ledactual + accel) > ledmax){
        //termino de proceso
        valorLuz = ledmax;
        finmodo1 = 1;
        finmodo0 = 0;
      }else
      {
      valorLuz = ledactual + accel;
      }
      ledactual = valorLuz;
      tiempoPrevioLed = millis();
      analogWrite(constLcdLuzPin, valorLuz);
    }
  }
  if (modo == 0){//baja a 0 apaga
    finmodo1 = 1;
    if (tiempoactualled  - tiempoPrevioLed  > duracion){
      if ((ledactual - accel) < ledmin){
        //termino de proceso
        valorLuz = ledmin;
        finmodo1 = 0;
        finmodo0 = 1;
      }
      else
      {
        valorLuz = ledactual - accel;
      }
      ledactual = valorLuz;
      tiempoPrevioLed = millis();
      analogWrite(constLcdLuzPin, valorLuz);
    }
  }
}

void LcdModoAlarma(){
  if (finmodo1 == 0){
    sonidoAgudoAlarma();
    dimLuzLcd_nodelay(1, 20, 10);
    panelLcd.setCursor(0, 0);
    panelLcd.print(F("    ALARMA"));
    panelLcd.setCursor(0, 1);
    panelLcd.print(F("   ENTRANTE!"));
  }
  if (finmodo0 == 0){
    sonidoAgudoAlarma();
    dimLuzLcd_nodelay(0, 20, 10);
    panelLcd.clear();
  }
}

void LcdPrende(){
    analogWrite(constLcdLuzPin, 255);
}
void LcdApaga(){
      limpiaPanelLCD();
     analogWrite(constLcdLuzPin, 0);
}

void limpiaPanelLCD(){
  panelLcd.clear();
  }
  
 void EscribeLCD(String texto1, String texto2)
 {
    LcdPrende();
    panelLcd.clear();
    panelLcd.setCursor(0, 0);
    panelLcd.print(texto1);
    panelLcd.setCursor(0, 1);
    panelLcd.print(texto2);
  }

 void Inactividad(){
   unsigned long actualMillis = millis();
   if(actualMillis - previoMillisInac >= tiempoEsperaInactividad) {
      previoMillisInac = actualMillis;   
      intUbicacionMsg =  0;
      LcdApaga();
    }
  } 
