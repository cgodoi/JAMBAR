/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __AVR_ATmega2560__
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void sirveArchivo(EthernetClient miCliente, File miArchivo);
void procesaAutomatizaciones();
void XML_responseAutomatizaciones(EthernetClient micliente);
void envioMensajeSerialRF(String mensaje);
void ProcesaMensajeHijo();
void SetLEDs(void);
void ProcesaHTTPLEDS(String strMensajeHTTP);
void XML_estadoLuz(EthernetClient micliente);
void XML_estadoLuz2(EthernetClient micliente, byte sector);
void inicializaMisLuces();
void controldehorasOn();
void guardaLogLuz(byte luz, String log, byte eslogweb );
void guardarEstadoLuces();
void LeeMemoriaLuces();
void StrClear(char *str, char length);
int string2int(String string);
char StrContains(char *str, char *sfind);
int freeRam();
void XML_enviafechahora(EthernetClient micliente);
String mesNumeroaNombre(byte mesnro);
void controlFechaHora();
boolean ControlaSessiones(EthernetClient miCliente, IPAddress ipBuf);
void iniciasessionip(IPAddress ipremota);
boolean valUserPass(String strMensajeHTTP);
void cerrarsession(IPAddress ipremota);
boolean sessionvalida(IPAddress ipremota);
boolean esAccesoLocal(IPAddress ipremota);
String iptraeNombreUsuario(IPAddress ipremota);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\ArduinoMegaWebServerLuzCasa.ino"
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\EEPROMAnything.h"
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\webserverControlLuz.ino"
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\webserverGenerales.cpp"
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\webserverGenerales.ino"
#include "M:\ARDUINO\PROYECTOS\AUTOM\ArduinoMegaWebServerLuzCasa\webserverSesion.ino"
#endif
