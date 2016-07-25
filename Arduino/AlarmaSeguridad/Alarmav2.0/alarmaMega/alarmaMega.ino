#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "NotasMusicales.h"
#include "EEPROMcontrol.h"
#include "DatosUsuario.h"

//Configuracion Uso de Pines
const byte constLcdLuzPin = 9;
const byte constBotonAlarmaPin  = 11;
const byte constMenuPin = 10;
const byte constControlRFAlarmaPin = 40;
const byte constParlantePin  = 23;

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
const byte constEstadoNormal=0;
const byte constEstadoAlarmaEntrante=1;
const byte constEstadoAlarmaSaliente=2;
const byte constEstadoRevisa=3;
const byte constEstadoAlarmaNotifica=4;

//Configuracion Sistema
const unsigned int constMaxTextoMensaje = 256;
const String constMensajeIni = "MSGINI";
const String constMensajeFin = "MSGFIN";
byte EstadoActual = 0;
byte finmodo1 = 0;
byte finmodo0 = 0;
unsigned long tiempoPrevioLed = 0;
const byte ledmax = 255;
byte ledmin = 0;
byte ledactual = 0;
int  intUbicacionMsg = 16;

int contador = 0;

/*control entrada botones*/
int leeRFBtnAlarma;
int RFEstadoBoton;             // the current reading from the input pin
int ultimoRFEstadoBoton = LOW;   // the previous reading from the input pin
long RFultimosalto = 0;  // the last time the output pin was toggled

int leeBtnAlarma;
int alarmaEstadoBoton;             // the current reading from the input pin
int ultimoalarmaEstadoBoton = LOW;   // the previous reading from the input pin
long alarmaultimosalto = 0;  // the last time the output pin was toggled

int leeBtnMenu;
int MenuEstadoBoton;             // the current reading from the input pin
int ultimoMenuEstadoBoton = LOW;   // the previous reading from the input pin
long Menuultimosalto = 0;  // the last time the output pin was toggled


long debounceDelay = 50;    // the debounce time; increase if the output flickers

//Variables usadas en recepcion de caracteres antena
char   chrEntrada;
String strMensajeAlarmaTemp = "" ;
String strMensajeAlarma = "" ;

LiquidCrystal panelLcd(constLcdRs, constLcdEnable, constLcdD4, constLcdD5, constLcdD6, constLcdD7);

void setup() {
  pinMode(constLcdLuzPin, OUTPUT);
  pinMode(constBotonAlarmaPin, INPUT_PULLUP);
  pinMode(constControlRFAlarmaPin, INPUT);
  pinMode(constMenuPin, INPUT_PULLUP);
  panelLcd.begin(constLcdCaracteres, constLcdLineas);
  //Iniciando protocolo de comunicacion serial
  Serial3.begin(constSerialBaudios);
  Serial3.println("INICIANDO");
//  LeeMensajeEPROM();
}

void loop() {

  

  //Captura de mensaje RF*******************************************/
  if (Serial3.available() > 0) 
    {
      chrEntrada = Serial3.read();
      if (chrEntrada == '\n')
         {
          if (mensajeValido){
             EstadoActual = constEstadoAlarmaEntrante;
             strMensajeAlarma = strMensajeAlarmaTemp;
            }else{
              strMensajeAlarmaTemp ="";
              }
          }else{
            strMensajeAlarmaTemp += chrEntrada;
          }
    }
    /****************************************************************/

    /*Presionado de Botones******************************************/
  //LECTURA BOTON DESDE CONTROL RF
  leeRFBtnAlarma = digitalRead(constControlRFAlarmaPin);
  if (leeRFBtnAlarma != ultimoRFEstadoBoton) {
    RFultimosalto = millis();
  }
  if ((millis() - RFultimosalto) > debounceDelay) {
    // if the button state has changed:
    if (leeRFBtnAlarma != RFEstadoBoton) {
      RFEstadoBoton = leeRFBtnAlarma;

      //solo se activa estado en modo high
      if (RFEstadoBoton == HIGH) {
        EstadoActual = constEstadoAlarmaSaliente;
        digitalWrite(constParlantePin,HIGH);
      }
    }
  }
  ultimoRFEstadoBoton = leeRFBtnAlarma;
  //FIN LECTURA BOTON DESDE RF
  

  //LECTURA BOTON DESDE CONTROL ALARMA INTERNA
  leeBtnAlarma = digitalRead(constBotonAlarmaPin);
  if (leeBtnAlarma != ultimoalarmaEstadoBoton) {
    alarmaultimosalto = millis();
  }
  if ((millis() - alarmaultimosalto) > debounceDelay) {
    // if the button state has changed:
    if (leeBtnAlarma != alarmaEstadoBoton) {
      alarmaEstadoBoton = leeBtnAlarma;

      //solo se activa estado en modo llow --revisar
      if (alarmaEstadoBoton == LOW) {
        EstadoActual = constEstadoAlarmaSaliente;
      }
    }
  }
  ultimoalarmaEstadoBoton = leeBtnAlarma;
  //FIN LECTURA BOTON DESDE CONTROL ALARMA INTERNA


    //LECTURA BOTON DESDE BTN MENU
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
        EstadoActual = constEstadoRevisa;
      }
    }
  }
  ultimoMenuEstadoBoton = leeBtnMenu;
  //FIN LECTURA BOTON DESDE BTN MENU
  
        /*if (digitalRead(constControlRFAlarmaPin) == HIGH){
          EstadoActual = constEstadoAlarmaSaliente;
      }
      if (digitalRead(constBotonAlarmaPin) == LOW){
          EstadoActual = constEstadoAlarmaSaliente;
      }
      if (digitalRead(constMenuPin) == LOW){
          EstadoActual = constEstadoRevisa;
      }*/
      
    /****************************************************************/
    //control estados************************************************/
    if (EstadoActual == constEstadoNormal ){
      //estado normal
      //Inactividad();
      //EscribeLCD(String(EstadoActual));
      //delay(300);
    }
    if (EstadoActual == constEstadoAlarmaNotifica ){
      // EscribeLCD(String(EstadoActual));
      //delay(300);
      LcdModoAlarma(); //Activa luces y sonido      
    }
       
    if (EstadoActual == constEstadoAlarmaEntrante ){
    //  GuardaAlarmaEEPROM(); //almanecena alarma en memoria
      //EscribeLCD(String(EstadoActual));
      //delay(300);
          
      EstadoActual = constEstadoAlarmaNotifica;
      EnviaAlarma(true); //expande alarma recibida a modulos cercanos
    }   
    
    if (EstadoActual == constEstadoAlarmaSaliente ){
      //envia alarma
      //EscribeLCD(String(EstadoActual));
      //delay(300);
      EstadoActual = constEstadoNormal;
      EnviaAlarma(false); //Envia mensaje de alerta a demas modulos
      digitalWrite(constParlantePin,LOW);      

    }   
    
    if (EstadoActual == constEstadoRevisa ){
      //revisando ultima alarma recibida, si existe, esto es rotatorio
      //EscribeLCD(String(EstadoActual));
      //delay(300);
      LcdPrende();
      MuestraMensajeAlarma();
      EstadoActual = constEstadoNormal;
    }   
    /****************************************************************/
  
}
