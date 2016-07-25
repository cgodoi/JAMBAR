//DATOS USUARIOS COMPUESTO DE TROZOS DE 16 CARACTERES
String strMsjSTATUS = "SISTEMAON";

const char * strIdModulo= "MA0001";
const String strDatosModulo = "NOMBRE:          Omar Lorca SozaDIRECCION:          R.Fabres 161TELEFONO 1:              2964332TELEFONO 2:             82985501";


//const char * strIdModulo= "MA0002";
//const char * strDatosModulo = "NOMBRE:          Alonso Almuna PDIRECCION:            CALLES 162TELEFONO 1:              2554552TELEFONO 2:             85485541";


int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}