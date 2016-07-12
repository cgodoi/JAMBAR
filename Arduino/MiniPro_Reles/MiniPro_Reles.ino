//Definir LED,s que maneja este HIJO
String strLedControlados ="SWICH;ESTADO;LED01;LED02;LED03;LED04;LED05;LED06;LED07;LED08;SWICH0;SWICH1";

int incomingByte = 0;	// para el byte leido
String strRFMensaje="";
char c;
String strMensajeEntrada ="MRF"; //mensaje RF desde MADRE
String strMensajeSalida ="HRF"; //mensaje de hijo a madre (central)
String LEDprocesar="";
int intAccion = 0;
int blnSwitchNo = 0; //indicador si sistema permite uso manual, osea switch de pared 0 si permite, 1 no permite
byte hayCambios =0;

//Variables 
int totLuces = 8;
int relesChan1[] = {30,32,34,36,38,40,42,44};
int relesChan2[] = {31,33,35,37,39,41,43,45};
int paredSw[] =    {22,23,24,25,26,27,28,29};

int swParedEstadoActual[] = {0,0,0,0,0,0,0,0};
int swParedEstadoAnterior[] = {0,0,0,0,0,0,0,0};

#define intLargoRF 30 

void setup() {
	Serial3.begin(9600);	// abre el puerto serie a 9600 bps
   for (byte idLuz = 0; idLuz < totLuces; idLuz++)  {
          pinMode(relesChan1[idLuz], OUTPUT);      
          pinMode(relesChan2[idLuz], OUTPUT);
          pinMode(paredSw[idLuz],INPUT);
          digitalWrite(relesChan2[idLuz],HIGH);
        }
        
        //INICIALIZA ESTADO INICIAL DE SW DE PARED
  for (byte idLuz = 0; idLuz < totLuces; idLuz++)  {
    swParedEstadoAnterior[idLuz] = digitalRead(paredSw[idLuz]);
     if (swParedEstadoAnterior[idLuz] == 0 )
      {
        digitalWrite(relesChan1[idLuz],HIGH);
      }else
      {
         digitalWrite(relesChan1[idLuz],LOW);        
      }
      
}
}

void loop() {
 	// envia datos solamente cuando recibe datos
if (blnSwitchNo == 0){
  
    for (int idLuz = 0; idLuz < totLuces; idLuz++)  {
      swParedEstadoActual[idLuz] = digitalRead(paredSw[idLuz]);
    }  
    
   /* for (int idLuz = 0; idLuz < totLuces; idLuz++)  {
      Serial3.print("Estado SENSOR SWITCH NRO ");
      Serial3.print(idLuz +1 );
      Serial3.print(" : ");
      Serial3.println(swParedEstadoActual[idLuz]);
    }*/  
    
    for (int idLuz = 0; idLuz < totLuces; idLuz++)  {      
    if (swParedEstadoActual[idLuz] != swParedEstadoAnterior[idLuz]) 
      {
        Serial3.print("Estado Actual: ");
        Serial3.print(swParedEstadoActual[idLuz]);
        Serial3.print(" / Estado anterior:");
        Serial3.println(swParedEstadoAnterior[idLuz]);

        if (digitalRead(relesChan1[idLuz]) == LOW)
         {  
          digitalWrite(relesChan1[idLuz],HIGH);
         }else
         {
          digitalWrite(relesChan1[idLuz],LOW);
         }
          InformaEstadoaCentral(idLuz);
       hayCambios = 1;
       swParedEstadoAnterior[idLuz]  = swParedEstadoActual[idLuz] ; 
      }
    }

    if (hayCambios == 1) {
  //    InformaEstadoaCentral();
       hayCambios = 0;
    }
} 


if (Serial3.available() > 0) {
              c= Serial3.read();
              if (c == '\n' ){
                strRFMensaje+="Captado";
                char mensajeRF[intLargoRF] ;
                strRFMensaje.toCharArray(mensajeRF,intLargoRF);
//                 Serial3.println(mensajeRF);
                 //Valida mensaje y procese a realizar accion
                 if (BlnMensajeEsValido()==true){
                       subEjecutaAccion();
                 }
                strRFMensaje ="";
              }else{
                strRFMensaje+= c;
              }
	}
}

//Valida si mensaje es valido y contiene elementos que procesa este modulo (strLedControlados)
boolean BlnMensajeEsValido(){
boolean blnValido = false;
   //Mesaje es de Madre
//Serial3.println(strRFMensaje.indexOf(strMensajeEntrada));
  if (strRFMensaje.indexOf(strMensajeEntrada)!=-1){
//    Serial3.println(strRFMensaje.substring(3,8));
//    Serial3.println(strRFMensaje.substring(9,10));

    LEDprocesar = strRFMensaje.substring(3,8);
    //Valido si mensaje trae informacion de LED valido
    if (strLedControlados.indexOf(LEDprocesar)!=-1){
      intAccion = string2int(strRFMensaje.substring(9,10));
      blnValido = true; //mensaje trae informacion de led de este modulo/hijo
    }else{
      blnValido = false;//mensaje tiene un led o control no valido para este modulo/hijo
    }
  }
  else
  {
    blnValido=false;//Mensaje no contiene formato estandar
  }
  
  return blnValido;

}


void subEjecutaAccion(){
 int relayTrabajar = 0;
 int luzTrabajar = 0;
  
 //Mensaje es de Central y pide estado, entrega estado y termina ciclo
  if (strRFMensaje.indexOf("ESTADO")!=-1){
    InformaEstadoaCentral(-1);
    return;
  }
  
  if (strRFMensaje.indexOf("SWICH0")!=-1){
    blnSwitchNo = 0;
        Serial3.println("SWICH 0 OK ");
    return;
  }
  
  if (strRFMensaje.indexOf("SWICH1")!=-1){
    blnSwitchNo = 1;
        Serial3.println("SWICH 1 OK ");
    return;
  }
  
  //Determina relay a trabajar
  luzTrabajar = string2int(LEDprocesar.substring(3,5))-1;
  relayTrabajar = relesChan1[luzTrabajar];
   
  if (relayTrabajar==0){
    if (strLedControlados.indexOf("SWICH")!=-1){
      //Permite o no uso de switch de pared
      blnSwitchNo = intAccion;      
    }   
  }else{
  
   if (intAccion==1){
    Serial3.println("PRENDO " + luzTrabajar);
      if (relayTrabajar!=0){
        digitalWrite(relayTrabajar,HIGH);
      }
   }
   
   else if(intAccion==0){
    Serial3.println("APAGO " + luzTrabajar);
      if (relayTrabajar!=0){
       digitalWrite(relayTrabajar,LOW);
       }    
   }
   
   intAccion = -1;
   LEDprocesar="";
  }
}

void InformaEstadoaCentral(int luzfecta)
{
  if (luzfecta == -1){
    for (int idLuz = 0; idLuz < totLuces; idLuz++)  {
      delay(20);
      if (digitalRead(relesChan1[idLuz]) == HIGH){
          Serial3.println(strMensajeSalida+"LED0"+String(idLuz)+"=1");
        }else{
          Serial3.println(strMensajeSalida+"LED0"+String(idLuz)+"=0");
        }
    }
  }else{
      if (digitalRead(relesChan1[luzfecta]) == HIGH){
          Serial3.println(strMensajeSalida+"LED0"+String(luzfecta)+"=1");
        }else{
          Serial3.println(strMensajeSalida+"LED0"+String(luzfecta)+"=0");
        }
  }

}

int string2int(String string){
  char cadena[string.length()+1];
  string.toCharArray(cadena, string.length()+1);
  return atol(cadena);
}
