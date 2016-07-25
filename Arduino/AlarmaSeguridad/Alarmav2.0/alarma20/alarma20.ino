#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "NotasMusicales.h"
#include "EEPROMcontrol.h"
#include "DatosUsuario.h"

//Configuracion Uso de Pines
const byte constLedRfOk             = 2; //indicador de que a lo menos otro moduloesta disponible
const int constParlantePin        = A2; //ANALOGO
const byte constLcdLuzPin           = 9; 
const byte constControlRFAlarmaPin  = 10; 
const byte constBotonAlarmaPin      = 11; 
const byte constMenuPin             = 12; 
const byte constLedInterno          = 13; 
const byte constBtnSinUso          = 14;  //sin uso


//Configuracion Antena RF
const int  constSerialBaudios = 9600;

//Configuracion Panel LCD
const byte constLcdRs = 3;
const byte constLcdEnable = 4;
const byte constLcdD4 = 5;
const byte constLcdD5 = 6;
const byte constLcdD6 = 7;
const byte constLcdD7 = 8;
const byte constLcdCaracteres = 16;
const byte constLcdLineas = 2;

//Configuracion Estados
const byte constEstadoIdle=0; //haciendo nada
const byte constEstadoEnviando=1; //enviando alarma serial
const byte constEstadoRecibiendo=2; //recibiendo mensaje serial
const byte constEstadoRevisando=3;  //revisando ultimo mensaje recibido
const byte constEstadoReenviando=4; //re enviando un mensaje serial
const byte constEstadoAlarma=5; //activando alarma en modulo
const byte constEstadoRepitiendoEnvio=6; //repite envio de mensaje hasta que reciba ack
const byte constEstadoReenviadoEstatus=7; //recibe estatus, pero si no estaba esperandolo, se reenvia
byte EstadoActual = constEstadoIdle; //estado actual, idle
byte esperoRespuestaEstatus = 0;
char esperanRespuestaEstatus = '0'; /*indica si mensaje recibido espera o no respuesta*/

//Configuracion Sistema
const unsigned int constMaxTextoMensaje = 256;
byte byteInicioContenidoMensaje = 12; //menaje inicio + modulo emisor
const char * constMensajeIni = "MSGINI";
const char * constMensajeFin = "MSGFIN";
String gUltMensajeEnviado ="";
String gUltMensajeRecibido ="";


byte finmodo1 = 0;
byte finmodo0 = 0;
unsigned long tiempoPrevioLed = 0;
byte ledactual = 0;
int  intUbicacionMsg = 0;
//control inactividad
unsigned long previoMillisInac = 0;        // will store last time LED was updated
const unsigned long tiempoEsperaInactividad = 5000; 
unsigned long tiempoInterEnvioMsjPrevio =0; /*enviar mensaje cada 3 segundos hasta el ack*/
const unsigned long tiempoInterEnvioMsj =3000; /*enviar mensaje cada 3 segundos hasta el ack*/
unsigned long tiempoMensajePingPrevio =0; /*envio mensaje de estado para saber si existen modulos "conectados"*/
const unsigned long tiempoMensajePing =30000; /*enviar mensaje cada 30 segundos hasta el ping*/

/*control entrada botones*/
int leeRFBtnAlarma;
int RFEstadoBoton;             // the current reading from the input pin
int ultimoRFEstadoBoton = LOW;   // the previous reading from the input pin
unsigned long RFultimosalto = 0;  // the last time the output pin was toggled

int leeBtnAlarma;
int alarmaEstadoBoton;             // the current reading from the input pin
int ultimoalarmaEstadoBoton = LOW;   // the previous reading from the input pin
unsigned long alarmaultimosalto = 0;  // the last time the output pin was toggled

int leeBtnMenu;
int MenuEstadoBoton;             // the current reading from the input pin
int ultimoMenuEstadoBoton = LOW;   // the previous reading from the input pin
unsigned long Menuultimosalto = 0;  // the last time the output pin was toggled

long debounceDelay = 50;    // the debounce time; increase if the output flickers


LiquidCrystal panelLcd(constLcdRs, constLcdEnable, constLcdD4, constLcdD5, constLcdD6, constLcdD7);

void setup() {
   pinMode(constLcdLuzPin, OUTPUT);
   pinMode(constLedRfOk, OUTPUT);
   pinMode(constBotonAlarmaPin, INPUT_PULLUP);
   pinMode(constControlRFAlarmaPin, INPUT);
   pinMode(constMenuPin, INPUT_PULLUP);
   panelLcd.begin(constLcdCaracteres, constLcdLineas);
//Iniciando protocolo de comunicacion serial
   Serial.begin(constSerialBaudios);
   /*Serial.setTimeout(100);*/
   gUltMensajeEnviado.reserve(200);
   gUltMensajeRecibido.reserve(200);
   strMsjSTATUS.reserve(10);
//LeeMensajeEPROM();
  /*verificar estado*/
  envioMensajeEstatus(1);
}

void  serialEvent(){
  byte mMensajeValido = 0;
  byte mEstadoAnterior = EstadoActual;
  String strMensajeAlarmaTemp = "" ;
  char  michar[200];
  controlLedInterno(1);
  if (Serial.available() > 0)
  {
    EstadoActual = constEstadoRecibiendo;
    delay(2);
     strMensajeAlarmaTemp = Serial.readString();
     strMensajeAlarmaTemp.toCharArray(michar,200);
     mMensajeValido = mensajeValido( michar);
     switch (mMensajeValido) {
          case 0: /*mensaje valido para alarme y reenvio*/
             intUbicacionMsg = 0;
             gUltMensajeRecibido = strMensajeAlarmaTemp;
             EstadoActual = constEstadoReenviando;
            break;
          case 2:/*mensaje recibio es ack de mi mensaje enviado*/
            EstadoActual = constEstadoIdle;
            controlLedInterno(0);
            break;
          case 5: /*Control de mensaje de sistema online*/
            strMensajeAlarmaTemp ="";
            controlLedInterno(0);
            /*enciendo led estatus online*/
            digitalWrite(constLedRfOk, 1);
            tiempoMensajePingPrevio = millis();
            break;            
          default: /*mensaje no valido*/
            strMensajeAlarmaTemp ="";
            EstadoActual = mEstadoAnterior;
            controlLedInterno(0);
          break;
        }
   }
}

void mLeeBtnRfAlarma() {
leeRFBtnAlarma = digitalRead(constControlRFAlarmaPin);
  if (leeRFBtnAlarma != ultimoRFEstadoBoton) {
      RFultimosalto = millis();
  }
  if ((millis() - RFultimosalto) > debounceDelay) {
    //if the button state has changed:
   if(leeRFBtnAlarma != RFEstadoBoton) {
      RFEstadoBoton = leeRFBtnAlarma;
      //solo se activa estado en modo high
     if (RFEstadoBoton == HIGH) {
        EstadoActual = constEstadoEnviando;
         controlLedInterno(1);
      }
    }
  }
  ultimoRFEstadoBoton = leeRFBtnAlarma;
}

void mLeeBtnAlarma(){
  leeBtnAlarma = digitalRead(constBotonAlarmaPin);
  if (leeBtnAlarma != ultimoalarmaEstadoBoton){
    alarmaultimosalto = millis();
  }
  if ((millis() - alarmaultimosalto) > debounceDelay){
    // if the button state has changed:
    if (leeBtnAlarma != alarmaEstadoBoton){
      alarmaEstadoBoton = leeBtnAlarma;

      //solo se activa estado en modo llow --revisar
      if (alarmaEstadoBoton == LOW) {
        EstadoActual = constEstadoEnviando;
        controlLedInterno(1);
      }
    }
  }
  ultimoalarmaEstadoBoton = leeBtnAlarma;
}

void mLeeBtnMenu(){
  leeBtnMenu = digitalRead(constMenuPin);
  if (leeBtnMenu != ultimoMenuEstadoBoton) {
    Menuultimosalto = millis();
  }
  if ((millis() - Menuultimosalto) > debounceDelay) {
    // if the button state has changed:
    if (leeBtnMenu != MenuEstadoBoton) {
      MenuEstadoBoton = leeBtnMenu;

      //solo se activa estado en modo llow --revisar
      if (MenuEstadoBoton == LOW) {
        EstadoActual = constEstadoRevisando;
      }
    }
  }
  ultimoMenuEstadoBoton = leeBtnMenu;
}

void loop(){
  /*control indicador led de modulos en linea*/
  ctrSistemaConectado();
/*  Serial.print(F("RAM LIBRE:"));
  Serial.println(freeRam());*/
  /*Presionado de Botones******************************************/
  ////LECTURA BOTON DESDE CONTROL RF
  mLeeBtnRfAlarma();
  ////LECTURA BOTON DESDE CONTROL ALARMA INTERNA
  mLeeBtnAlarma();
 ////LECTURA BOTON DESDE BTN MENU
  mLeeBtnMenu();
  /****************************************************************/
//Serial.print("EstadoActual:");
//Serial.println(EstadoActual);
 //control estados************************************************/
    if (EstadoActual == constEstadoIdle ){
      Inactividad();
    }
    if (EstadoActual == constEstadoRepitiendoEnvio ){
       if ((millis() - tiempoInterEnvioMsjPrevio) > tiempoInterEnvioMsj) {
          //prende leds que indican comunicacion saliente
          controlLedInterno(1);
          EnviaMensajeAlarma(2);
          tiempoInterEnvioMsjPrevio = millis();
          }else{
            controlLedInterno(0);
          }
        /*no cambio estado, lo cambiara solo si me llega un aviso de ack*/
    }
    if (EstadoActual == constEstadoEnviando ){
      previoMillisInac = millis();
      tiempoInterEnvioMsjPrevio = millis();
      //prende leds que indican comunicacion saliente
      EnviaMensajeAlarma(0);
      controlLedInterno(0);
      EstadoActual= constEstadoRepitiendoEnvio;
    }
    if (EstadoActual == constEstadoRecibiendo ){
      previoMillisInac = millis();
      // prende leds que indican comunicaicon entrante
      EstadoActual = constEstadoIdle;
    }
     
    if (EstadoActual == constEstadoRevisando ){
      LcdPrende();
      MuestraMensajeAlarma();
      previoMillisInac = millis();
      EstadoActual = constEstadoIdle;
    }   
    if (EstadoActual == constEstadoReenviando ){
      previoMillisInac = millis();
      //prende  leds que indican comunicacion saliente x reenvio
      EnviaMensajeAlarma(1);
      controlLedInterno(0);
      EstadoActual = constEstadoAlarma;
    }   
    
    if (EstadoActual == constEstadoAlarma ){
      previoMillisInac = millis();
      //activa lcd , sonido, etc
      LcdModoAlarma();
    }   
    /****************************************************************/
  
}
