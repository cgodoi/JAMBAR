//lEE LA LLAMADA AJAX Y PROCESA EL/LOS LED
void SetLEDs(void)
{
	// Serial.println("PROCESAR LED");
	ProcesaHTTPLEDS(HTTPajaxReq);

}

void ProcesaHTTPLEDS(String strMensajeHTTP)
{//FORMATO LED00=0/LED00=1

	String strInterno = "";
	String strMiLed = "";

	byte mestadoLuz = 0;
	byte  midluz = 0;
	int ubicacionAMP = 0;

	Serial.println(strMensajeHTTP);
	strInterno = strMensajeHTTP;
	ubicacionAMP = strInterno.indexOf("&");
	while (ubicacionAMP  >-1) {
		ubicacionAMP = ubicacionAMP + 1;
		strMiLed = strInterno.substring(ubicacionAMP, ubicacionAMP + 7);
		if (strMiLed.indexOf("nocache") != -1){
			break;
		}
		midluz = string2int(strMiLed.substring(3, 5));
		mestadoLuz = string2int(strMiLed.substring(6, 7));

		if (MisLuces[midluz - 1].estado != mestadoLuz){
			MisLuces[midluz - 1].estado = mestadoLuz;
			envioMensajeSerialRF(strMiLed);
			if (mestadoLuz == 0)
			{
				guardaLogLuz(midluz - 1, " Apagada por Usuario",1);
			}
			else{ 
				guardaLogLuz(midluz - 1, " Encendida por Usuario",1);
			}
		}
		strInterno.replace("&" + strMiLed, "");
		ubicacionAMP = strInterno.indexOf("&");

	}

}

void XML_estadoLuz(EthernetClient micliente)
{

	byte count;                 // used by 'for' loops

	micliente.println("Content-Type: text/xml");
	micliente.println("Connection: keep-alive");
	micliente.println();
	micliente.print("<inputs>");
	// lee estado de leds
	for (count = 0; count < MY_LUZ-1; count++) {
		micliente.print("<LED>");
		/*if (LED_state[count] == 1) {
			micliente.print("on");
		}
		else {
			micliente.print("off");
		}*/
		micliente.println("</LED>");
	}
	micliente.print("</inputs>");
}

void XML_estadoLuz2(EthernetClient micliente, byte sector)
{

	byte count;

	micliente.println("Content-Type: text/xml");
	micliente.println("Connection: keep-alive");
	micliente.println();
	micliente.print("<inputs>");
	// lee estado de leds
	for (count = 0; count < MY_LUZ-1; count++) {
		if (sector == MisLuces[count].idsector){
			micliente.print("<LUZ>");

			micliente.print("<ID>");
			micliente.print(MisLuces[count].id);
			micliente.print("</ID>");

			micliente.print("<SECTOR>");
			micliente.print(MisLuces[count].idsector);
			micliente.print("</SECTOR>");

			micliente.print("<NOMBRE>");
			micliente.print(MisLuces[count].nombre);
			micliente.print("</NOMBRE>");
			micliente.print("<WATS>0");
			micliente.print("</WATS>");

			micliente.print("<ESTADO>");
			if (MisLuces[count].estado == 1) {
				micliente.print("on");
			}
			else {
				micliente.print("off");
			}
			micliente.print("</ESTADO>");

			micliente.print("<HORASON>");
			micliente.print(MisLuces[count].horasOn);
			micliente.print("</HORASON>");

			micliente.print("<LOG>");
			micliente.print(MisLuces[count].ultimoLog);
			micliente.print("</LOG>");

			micliente.println("</LUZ>");
		}
	}
	micliente.print("</inputs>");
}

void inicializaMisLuces(){
	//nowei= EEPROM_readAnything(0, MisLuces);

	MisLuces[0].id = 1;
	MisLuces[0].idsector = 1;
	strcpy(MisLuces[0].nombre, "Living");
	MisLuces[0].estado = 0;
	MisLuces[0].horasOn = 0;
	strcpy(MisLuces[0].ultimoLog, "");
	
	MisLuces[1].id = 2;
	MisLuces[1].idsector = 2;
	strcpy(MisLuces[1].nombre, "Faro de entrada");
	MisLuces[1].estado = 0;
	MisLuces[1].horasOn = 0;
	strcpy(MisLuces[1].ultimoLog, "");

        MisLuces[2].id = 3;
	MisLuces[2].idsector = 2;
	strcpy(MisLuces[2].nombre, "Huerta");
	MisLuces[2].estado = 0;
	MisLuces[2].horasOn = 0;
	strcpy(MisLuces[2].ultimoLog, "");

        MisLuces[3].id = 4;
	MisLuces[3].idsector = 3;
	strcpy(MisLuces[3].nombre, "Cama grande");
	MisLuces[3].estado = 0;
	MisLuces[3].horasOn = 0;
	strcpy(MisLuces[3].ultimoLog, "");

        MisLuces[4].id = 5 ;
	MisLuces[4].idsector = 3;
	strcpy(MisLuces[4].nombre, "Cine / taller");
	MisLuces[4].estado = 0;
	MisLuces[4].horasOn = 1;
	strcpy(MisLuces[4].ultimoLog, "");
  
        MisLuces[5].id = 6;
	MisLuces[5].idsector = 3;
	strcpy(MisLuces[5].nombre, "Lavanderia y planchado");
	MisLuces[5].estado = 0;
	MisLuces[5].horasOn = 1;
	strcpy(MisLuces[5].ultimoLog, "");

        MisLuces[6].id = 7;
	MisLuces[6].idsector = 4;
	strcpy(MisLuces[6].nombre, "Lavamanos");
	MisLuces[6].estado = 0;
	MisLuces[6].horasOn = 0;
	strcpy(MisLuces[6].ultimoLog, "");

        MisLuces[7].id = 8;
	MisLuces[7].idsector = 5;
	strcpy(MisLuces[7].nombre, "Pasillo");
	MisLuces[7].estado = 0;
	MisLuces[7].horasOn = 0;
	strcpy(MisLuces[7].ultimoLog, "");

        // nowei = EEPROM_writeAnything(0, MisLuces);
	 //Serial.print("epromrad:");
	 //Serial.println(nowei);

}


void controldehorasOn()
{
	byte count;
	byte hayCambios= 0;
		// lee recorre luces
    for (count = 0; count < MY_LUZ-1; count++) {
		if (MisLuces[count].estado == 1){
			MisLuces[count].horasOn = MisLuces[count].horasOn + 1;
			hayCambios = 1;
		}
	}

	if (hayCambios == 1){
		guardarEstadoLuces();
	}
		
}

void guardaLogLuz(byte luz, String log, byte eslogweb ){
	String Logcompleto;
	if (eslogweb == 1){
		Logcompleto = log +' '+ iptraeNombreUsuario(ipBuf);
		
	}
	else
	{
		Logcompleto = log;
	}

	Logcompleto = Logcompleto + " el " + RTC.now().day() + ' ' + mesNumeroaNombre(RTC.now().month()) + ' ' + RTC.now().year();
	Logcompleto = Logcompleto + " a las " + RTC.now().hour() + ':' + RTC.now().minute() + "hrs.";
	strcpy(MisLuces[luz].ultimoLog, Logcompleto.c_str());

	//guardarEstadoLuces();
	
}

void guardarEstadoLuces()
{
//	EEPROM_limpiar();
//	EEPROM_writeAnything(0, MisLuces);
}

void LeeMemoriaLuces()
{
	//EEPROM_readAnything(0,MisLuces);
}
