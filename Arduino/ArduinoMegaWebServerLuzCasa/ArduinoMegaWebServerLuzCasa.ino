/*--------------------------------------------------------------
  Program:      ArduinoMegaWebServerLuzCasa
  Author:       C. Godoi C.
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"
#define __PROG_TYPES_COMPAT__

File misConfig;
RTC_DS1307 RTC;
// Definir Cantidad de luces a controlar
#define MY_LUZ 9
//usuarios conectados simultaneos (4 disponibles)
#define SESSION_USUARIOS 5
int cicloSessionUsuarios = 4;

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte bIp[] = { 192, 168, 60, 130 };
byte bIpCliente[] = { 0, 0, 0, 0 };
IPAddress ipBuf;

EthernetServer server(80);  // create a server at port 80
File webFile;               // the web page file on the SD card
String HTTPajaxReq = "";
String archivoPeticion = "";
char req_index = 0;              // index into HTTP_req buffer
String MensajePromini = "";
char cc;
//Session//
byte  sessiontimeout = 120; //segundos (2 min)
byte sessionusadas = 0; //numero de session en usos
byte sector = 0;

//structura usuarios
typedef struct
{
  String usuario;
  String password;
  byte IpCliente[5];

}  UsuariosWebStruct;

UsuariosWebStruct MisClientes[SESSION_USUARIOS];

//estructura luces
typedef struct
{
  byte id;
  byte idsector;
  char nombre[30];
  byte estado;
  unsigned int horasOn;
  char ultimoLog[250];
}  LucesStruct;

LucesStruct MisLuces[MY_LUZ];

DateTime horafechaAnterior;
IPAddress ipConectadas[SESSION_USUARIOS];
DateTime ipConectadasHoraLogin[SESSION_USUARIOS];
// password acceso a servidor
const char usuario1[] PROGMEM = "yayo";   // "String 0" etc are strings to store - change to suit.
const char usuario2[] PROGMEM = "invitado";
const char clave1[] PROGMEM = "1302";   // "String 0" etc are strings to store - change to suit.
const char clave2[] PROGMEM = "9999";

const char* const tabla_usuarios[] PROGMEM = {
  usuario1,
  usuario2
};
const char* const tabla_claves[] PROGMEM = {
  clave1,
  clave2
};

void setup()
{

  // disable Ethernet chip
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  Wire.begin();
  RTC.begin();

  //RTC.adjust(DateTime(__DATE__, __TIME__));
  Serial.begin(9600);       // for debugging
  Serial1.begin(9600);


  // initialize SD card
  Serial.println("Inicializando tarjeta SD...");
  if (!SD.begin(4)) {
    Serial.println(F("ERROR - tarjeta SD no pudo ser inicializada!"));
    return;    // init failed
  }
  Serial.println(F("OK - tarjeta SD inicializada OK."));

  Ethernet.begin(mac, bIp);  // initialize Ethernet device
  server.begin();           // start to listen for clients


  MisClientes[1].usuario = "yayo";
  MisClientes[1].password = "1302";
  inicializaMisLuces();

  horafechaAnterior = RTC.now();
  //Pide estado a modulos hijos

  Serial1.println(F("MRFESTADO"));

}

void loop()
{

  controlFechaHora();
  EthernetClient client = server.available();  // try to get client
  if (Serial1.available() > 0) {
    cc = Serial1.read();
    if (cc == '\n' ) {
      ProcesaMensajeHijo();
      MensajePromini = "";
    } else {
      MensajePromini += cc;
    }
  }

  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        // limit the size of the stored received HTTP request
        // buffer first part of HTTP request in HTTP_req array (string)
        // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
        HTTPajaxReq += c;
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          ipBuf = client.getRemoteIP();

          if (HTTPajaxReq.indexOf("fechahora") != -1) { //fecha y hora de sistema
            Serial.println("entrando a fecha hora");
            XML_enviafechahora(client);

          } else if (HTTPajaxReq.indexOf("cerrarsession") != -1) { //Inicio de session
            Serial.println("entrando a cerrando session");
            cerrarsession(ipBuf);
            webFile = SD.open("index.htm");
            sirveArchivo(client, webFile);

          } else if (HTTPajaxReq.indexOf("iniciosession") != -1) { //Inicio de session
            // send rest of HTTP header
            Serial.println("entrando a inicio session");
            iniciasessionip(ipBuf);
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            webFile = SD.open("dash.htm");
            sirveArchivo(client, webFile);

          } else if (HTTPajaxReq.indexOf("ajax_inputs") != -1) { //prendido y apagado de leds desde interfaz web
            Serial.println(F("entrando a ajax inputs"));
            //  if  (ControlaSessiones(client, ipBuf) == false){break;}

            //Serial.println(sector);
            if (HTTPajaxReq.indexOf("LED") != -1) {
              SetLEDs();
              client.stop();
            }
            else
            {
              sector = atoi(HTTPajaxReq.substring(HTTPajaxReq.indexOf("ajax_inputs?") + 12, HTTPajaxReq.indexOf("ajax_inputs?") + 13).c_str());
              XML_estadoLuz2(client, sector);
            }


          } else if (HTTPajaxReq.indexOf("automatizacion") != -1) { //creacion o cambios en configuracion de automatizaciones
            Serial.println(F("entrando a automatrixzacion"));
            //    if  (ControlaSessiones(client, ipBuf) == false){break;}
            Serial.println(HTTPajaxReq);
            procesaAutomatizaciones();
            XML_responseAutomatizaciones(client);
          }
          else {  // sirve a cliente las paginas web
            Serial.println(F("entrando peticion webpage"));
            archivoPeticion = HTTPajaxReq.substring(3, HTTPajaxReq.indexOf(" HT"));
            archivoPeticion.replace(" /", "");
            char archivochar[archivoPeticion.length() + 1] ;
            archivoPeticion.toCharArray(archivochar, archivoPeticion.length() + 1);

            if (!SD.exists(archivochar)) {
              Serial.println(F("ERROR - No existe Archivo, envia index "));
              webFile = SD.open("index.htm");//se envia index/login

            } else {
             // if ((archivoPeticion.indexOf(".htm") != -1) && (archivoPeticion.indexOf("index.htm") == -1)) {
                //if (ControlaSessiones(client, ipBuf) == false) {
                //  webFile = SD.open("close.htm");// open web page file
                //  Serial.println(F("ERROR - Sesion no valida, envia close "));
                //}
                //else
                //{
            //      webFile = SD.open(archivochar);// open web page file
               // }
             // }
             // else
             // {
                webFile = SD.open(archivochar);// recursos
             // }
           }

            sirveArchivo(client, webFile);

          }
          req_index = 0;
          HTTPajaxReq = "";
          break;
        }
        // every line of text received from the client ends with \r\n
        //if (c == '\n') {
        //    currentLineIsBlank = true;
        //}
        //else if (c != '\r') {
        //    currentLineIsBlank = false;
        // }
      } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
  } // end if (client)
}

void sirveArchivo(EthernetClient miCliente, File miArchivo) {
  char tBuf[65];
  int clientCount = 0;
  miCliente.println("Content-Type: text/html");
  miCliente.println("Connection: close");
  miCliente.println();
  while (miArchivo.available()) {
    tBuf[clientCount] = miArchivo.read();
    clientCount++;
    tBuf[clientCount] = 0;

    if (clientCount > 63) {
      miCliente.write((byte*)tBuf, 64);
      clientCount = 0;
    }

  }

  if (clientCount > 0) {
    miCliente.write((byte*)tBuf, clientCount);
  }
  miArchivo.close();
  miCliente.stop();
  Serial.print(F("Ram Libre es: "));
  Serial.println(freeRam());

}

void procesaAutomatizaciones()
{
  Serial.println(F("Procesando automatizaciones"));
  misConfig = SD.open("miconf.xml", FILE_WRITE);
  misConfig.println("Probando linea1");
  misConfig.println("Probando linea1 3");
  misConfig.close();
  if (!SD.exists("miconf.xml")) {
    Serial.println(F("ERROR - No existe Archivo miconf.xml"));
  }
}



void XML_responseAutomatizaciones(EthernetClient micliente)
{
  micliente.println("Content-Type: text/xml");
  micliente.println("Connection: keep-alive");
  micliente.println();

  Serial.println(F("Respuesta a automatizaciones"));
}


void envioMensajeSerialRF(String mensaje) {
  int intLargoRF = 30;
  String strPrefixRF = "MRF"; //Mensaje RF, indica que la comunicacion serial debe ser considerada por los modulos.
  char mensajeRF[intLargoRF] ;
  String mensajeAux = strPrefixRF + mensaje;
  mensajeAux.toCharArray(mensajeRF, intLargoRF);

  //Envio de mensaje
  Serial.print(F("MENSAJE ENVIADO A MODULOS _: "));
  Serial.print(mensajeRF);
  Serial1.println(mensajeRF);
  Serial.println();
}

//Valida si mensaje es valido y contiene elementos que procesa este modulo (strLedControlados)
void ProcesaMensajeHijo() {
  String strMensajeHijo = "HRF";
  String strLED = "LED";
  String LEDprocesar = "";
  int intAccion = 0;
  byte luzindice=0;

  //Mensaje es hijo
  if (MensajePromini.indexOf(strMensajeHijo) != -1) {
    Serial.print("MENSAJE DESDE HIJO: ");
    Serial.println(MensajePromini);
    //mensaje contiene informacion de LEDS
    if (MensajePromini.indexOf(strLED) != -1) {
      Serial.print(F("Contiene LED "));
      LEDprocesar = MensajePromini.substring(3, 8);
      Serial.print(LEDprocesar);
      intAccion = string2int(MensajePromini.substring(9, 10));
      Serial.print(F(" Accion:"));
      Serial.print(intAccion);
      Serial.print(F(" item de arreglo a modificar "));
      luzindice = string2int(LEDprocesar.substring(3, 5));
      Serial.println(luzindice);

      //LED_state[string2int(LEDprocesar.substring(3,5))-1]=intAccion;
      MisLuces[luzindice].estado = intAccion;
      if (intAccion == 0)
      {
        guardaLogLuz(luzindice, " Apagada desde switch pared", 0);
      }
      else {
        guardaLogLuz(luzindice, " Encendida por switch pared", 0 );
      }

    }
  }
}
















