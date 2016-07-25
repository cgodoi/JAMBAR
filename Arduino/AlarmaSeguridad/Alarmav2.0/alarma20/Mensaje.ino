//Funciones relacionadas con mensajes

byte mensajeValido(char * pMensaje){
  int validador = 0;// NOK
  String strMensaje;
  strMensaje.reserve(200);
  strMensaje="";
  strMensaje.concat(pMensaje);

  int iMsgIni = strMensaje.indexOf(constMensajeIni);
  int iMsgFin = strMensaje.indexOf(constMensajeFin);
  //Serial.print("strMensaje:");
  //Serial.println(strMensaje);
  if ((iMsgIni >= 0) && (iMsgFin >= 0)){
    if (strMsjSTATUS.indexOf(strMensaje.substring(6,iMsgFin-1))>-1){
      validador=5; /*mensaje de estatus*/
      esperanRespuestaEstatus = strMensaje.charAt(iMsgFin-1);
      /*1, espera respuesta, 0 no espera respuesta*/
      return validador;
    }

    if (gUltMensajeRecibido.indexOf(strMensaje.substring(0,iMsgFin))>-1){
      validador= 1;
      return validador;
     } 

     /*RECIBO DE VUELTA EL MENSAJE ENVIADO*/
    if (gUltMensajeEnviado.indexOf(strMensaje.substring(0,iMsgFin))>-1){
      validador= 2;
      return validador;
    } 
     
  }else{
    validador= 4; //no es valido formaot
    return validador;
  }

 return validador;
}

void EnviaMensajeAlarma(byte reenvio){
  //si es envio de alamra, o si es expansion de una recibida
  String mMensajeEnviar ="";
  String mensajeConModuloEmisor ="";

  mMensajeEnviar.reserve(200);
  mensajeConModuloEmisor.reserve(160);

  if (reenvio==1) /*reenviar mensaje recibido*/
  {
    mensajeConModuloEmisor = gUltMensajeRecibido.substring(0,gUltMensajeRecibido.indexOf(constMensajeFin));
    //mMensajeEnviar = mensajeConModuloEmisor+constMensajeFin+strIdModulo;
    mMensajeEnviar.concat(mensajeConModuloEmisor);
    mMensajeEnviar.concat(constMensajeFin);
    mMensajeEnviar.concat(strIdModulo);
    gUltMensajeEnviado = mMensajeEnviar;
  }
  if (reenvio==0)/*enviar nuevo mensaje*/
  {
    char * strMilis =  millisAString(millis());
    //mMensajeEnviar = constMensajeIni + strIdModulo+ strDatosModulo +strMilis+ constMensajeFin;
    mMensajeEnviar.concat(constMensajeIni);
    mMensajeEnviar.concat(strIdModulo);
    mMensajeEnviar.concat(strDatosModulo);
    mMensajeEnviar.concat(strMilis);
    mMensajeEnviar.concat(constMensajeFin);
    gUltMensajeEnviado = mMensajeEnviar;
  }

  //dejamos el ultimo enviado, para saber si tenemos que reenviar en el futuro*/
    Serial.println(gUltMensajeEnviado);
}

void MuestraMensajeAlarma(){
  //REVISAR
  byte step = 16;
  String mContenidoMensaje ="";

  mContenidoMensaje.reserve(200);
  panelLcd.clear();

  if (gUltMensajeRecibido.length() == 0){
    EscribeLCD(F("    NO HAY     "),F("   MENSAJES    "));
    return;
  }

  mContenidoMensaje = gUltMensajeRecibido.substring(byteInicioContenidoMensaje, gUltMensajeRecibido.indexOf(constMensajeFin)-8);//-7 para ocultar idenfificador de mensaje


  if ((intUbicacionMsg+step) > mContenidoMensaje.length()){
    intUbicacionMsg= 0;
  }

  panelLcd.setCursor(0, 0);
  panelLcd.print(mContenidoMensaje.substring(intUbicacionMsg, intUbicacionMsg + step));
  
  intUbicacionMsg += step;     

  if ((intUbicacionMsg+step) > mContenidoMensaje.length()){
    intUbicacionMsg= 0;    
  }

  panelLcd.setCursor(0, 1);
  panelLcd.print(mContenidoMensaje.substring(intUbicacionMsg, intUbicacionMsg + step));
  intUbicacionMsg += step;     

 }



void ctrSistemaConectado()
{
  if ((millis() - tiempoMensajePingPrevio) > tiempoMensajePing) {
          //bajo led indicador de coneccion
          digitalWrite(constLedRfOk, 0);
          tiempoMensajePingPrevio = millis();
          envioMensajeEstatus(1);
  }
  if ((esperanRespuestaEstatus=='1' && esperoRespuestaEstatus==1)
     ||
     (esperanRespuestaEstatus=='1' && esperoRespuestaEstatus==0))
      {
        esperanRespuestaEstatus='0';
        esperoRespuestaEstatus=0;
        envioMensajeEstatus(esperoRespuestaEstatus);
   }
}

void envioMensajeEstatus(byte esperoEstatus)
{
   String mMensajeEnviar  = "";
   mMensajeEnviar.reserve(50);
    mMensajeEnviar.concat(constMensajeIni);
    mMensajeEnviar.concat(strMsjSTATUS);
    mMensajeEnviar.concat(esperoEstatus);
    mMensajeEnviar.concat(constMensajeFin);
    /*envio mensaje para saber si tengo respuesta de algun modulo*/
    Serial.println(mMensajeEnviar);
    esperoRespuestaEstatus=esperoEstatus;
    tiempoMensajePingPrevio = millis();
}


void GuardaAlarmaEEPROM(){
  char chrMensaje[constMaxTextoMensaje];
  byte bytesalida = 0;
  gUltMensajeRecibido.toCharArray(chrMensaje, constMaxTextoMensaje);
  bytesalida = EEPROM_graba(0, chrMensaje);
}

bool LeeAlarmaEEPROM(){
  String strMensajeEEPROM="";
  char chrMensaje[constMaxTextoMensaje];
  EEPROM_lee(0, chrMensaje);
  strMensajeEEPROM = chrMensaje;
  //return mensajeValido(strMensajeEEPROM);
}


char * millisAString(unsigned long pTime)
{
 static char str[12];
 long h = pTime/ 3600;
 pTime = pTime % 3600;
 int m = pTime / 60;
 int s = pTime % 60;
 sprintf(str, "%04ld%02d%02d", h, m, s);
 return str;
}