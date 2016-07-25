void dimLuzLcd_nodelay(byte modo, byte duracion, byte accel)
{
  unsigned long tiempoactualled = millis();
  int valorLuz;
  if (modo == 1){//llega a 255
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
  if (modo == 0){//baja a 0
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
    //sonidoAgudoAlarma();
    dimLuzLcd_nodelay(1, 20, 10);
  }
  if (finmodo0 == 0){
    //sonidoAgudoAlarma();
    dimLuzLcd_nodelay(0, 20, 10);
  }
}

void LcdPrende(){
   dimLuzLcd_nodelay(1, 20, 10);
}

void limpiaPanelLCD(){
  panelLcd.clear();
  }
  
 void EscribeLCD(String texto)
 {
    analogWrite(constLcdLuzPin, 255);
    panelLcd.clear();
    panelLcd.setCursor(0, 0);
    panelLcd.print(texto);
  }
