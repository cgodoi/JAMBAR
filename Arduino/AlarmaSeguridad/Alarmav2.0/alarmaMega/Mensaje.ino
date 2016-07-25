//Funciones relacionadas con mensajes

bool mensajeValido(String strMensaje){
  //Revisar si largo de mensaje es correcto y verificar si existe indicador de fin mensaje
  if (strMensaje.length()<= constMaxTextoMensaje){
      if (strMensaje.indexOf(constMensajeFin) > 0){
        return true;    
      }else{
        return false;
        }
  }
  else
  {
    return false;
  }
}

void EnviaAlarma(bool reenvio){
  //si es envio de alamra, o si es expansion de una recibida
  if (reenvio){
    Serial3.print(F("RENVIO"));
    //EscribeLCD("CTM");
  }else{
    //EscribeLCD("enviando");
    Serial3.print(constMensajeIni  + strDatosModulo + constMensajeFin);
    //Serial.print(contador);
    //contador = contador  +1; 
    //Serial.print(misDatosModulo[0]);
    //Serial.print(misDatosModulo[1]);
    //Serial.print(misDatosModulo[2]);
    //Serial.print(misDatosModulo[3]);
    //Serial.print(constMensajeFin);
    //EscribeLCD("enviado");
  }

  Serial3.println();
  delay(100);
}

void MuestraMensajeAlarma(){
  //REVISAR
  byte step = 16;
  String strALcd="";
    
  if (strMensajeAlarma.length() == 0){
    //no hay mensaje que mostrar
    return;
  }

  panelLcd.clear();

  if ((intUbicacionMsg + step) > strMensajeAlarma.length()){
    intUbicacionMsg = 16;
  }

  strALcd = strMensajeAlarma.substring(intUbicacionMsg, intUbicacionMsg + step);
  intUbicacionMsg += step;

  panelLcd.setCursor(0, 0);
  panelLcd.print(strALcd);
      
  if ((intUbicacionMsg + step) > strMensajeAlarma.length()){
    intUbicacionMsg = 16;
  }
  strALcd = strMensajeAlarma.substring(intUbicacionMsg, intUbicacionMsg + step);
  
  panelLcd.setCursor(0, 15);
  panelLcd.print(strALcd);
 }

void GuardaAlarmaEEPROM(){
  char chrMensaje[constMaxTextoMensaje];
  byte bytesalida = 0;
  strMensajeAlarma.toCharArray(chrMensaje, constMaxTextoMensaje);
  bytesalida = EEPROM_graba(0, chrMensaje);
}

bool LeeAlarmaEEPROM(){
  String strMensajeEEPROM="";
  char chrMensaje[constMaxTextoMensaje];
  EEPROM_lee(0, chrMensaje);
  strMensajeEEPROM = chrMensaje;
  return mensajeValido(strMensajeEEPROM);
}

