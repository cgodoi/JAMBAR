boolean ControlaSessiones(EthernetClient miCliente, IPAddress ipBuf){
	//return true;
	File IndexArchivo;
	Serial.println(F("Validando Session...."));
	if (!sessionvalida(ipBuf)){
		Serial.println(F("Session no valida"));
		cerrarsession(ipBuf);
		return false;
	}
	else
	{
		return true;
	}
}

void iniciasessionip(IPAddress ipremota){
	DateTime horalogin = RTC.now();
	byte sessiones = 0;
	byte bvalido = 0; //1 valido 0 invalido
	//Valida Usuario y password

	if (valUserPass(HTTPajaxReq) == true){

		//Registro de session (para una usuario y password validos)
		while (sessiones < cicloSessionUsuarios) {
			if (ipConectadas[sessiones] == ipremota) {
				ipConectadasHoraLogin[sessiones] = horalogin;
				return;
			}
			sessiones++;
		}

		//nueva session
		if (sessionusadas >= cicloSessionUsuarios){
			sessionusadas = 0;
		}
		else
		{
			sessionusadas++;
		}

		ipConectadas[sessionusadas] = ipremota;
		ipConectadasHoraLogin[sessionusadas] = horalogin;
	}
}

boolean valUserPass(String strMensajeHTTP)
{//FORMATO ?user:XXXXX&pass:SDFGHJKL&

	String strInterno = "";
	byte valorLed = 0;
	String strUsuario = "";
	String strPassword = "";

	byte  ubicacionAMP = 0;
	byte ubicacionAMP2 = 0;
	boolean bsalida = false;
	char bufferUsuario[30];
	char bufferPass[30];
	char bufferuserpass[30];

	strInterno = strMensajeHTTP;
	ubicacionAMP = strInterno.indexOf("?user:");
	ubicacionAMP2 = strInterno.indexOf("&pass:");
	strUsuario = strInterno.substring(ubicacionAMP + 6, ubicacionAMP2);
	strUsuario.toCharArray(bufferUsuario, 30);

	ubicacionAMP2 = strInterno.indexOf("&pass:");
	ubicacionAMP = strInterno.indexOf("&nocache");
	strPassword = strInterno.substring(ubicacionAMP2 + 6, ubicacionAMP);
	strPassword.toCharArray(bufferPass, 30);


	for (int i = 0; i < cicloSessionUsuarios; i++)
	{
		strcpy_P(bufferuserpass, (char*)pgm_read_word(&(tabla_usuarios[i])));
		//VALIDA USUARIO, SI EXISTE, VALIDA PASSWORD
		if (strcmp(bufferUsuario, bufferuserpass) == 0){
			Serial.println(bufferUsuario);
			Serial.println(bufferuserpass);
			strcpy_P(bufferuserpass, (char*)pgm_read_word(&(tabla_claves[i])));
			Serial.println(bufferPass);
			Serial.println(bufferuserpass);
			//VALIDANDO PASSWORD
			if (strcmp(bufferPass, bufferuserpass) == 0){
				Serial.println(F("Usuario es valido"));
				bsalida = true;
				break;
			}

		}
	}

	return bsalida;
}

void cerrarsession(IPAddress ipremota){
	int sessiones = 0;
/*	while (sessiones < cicloSessionUsuarios) {
		if (ipConectadas[sessiones] == ipremota) {
			IPAddress ipnula;
			DateTime datenula;
			ipConectadas[sessiones] = ipnula;
			ipConectadasHoraLogin[sessiones] = datenula;
			Serial.println(F("ip encontrada, session eliminada"));
			return;
		}
		sessiones++;
	}*/
}

boolean sessionvalida(IPAddress ipremota){
	RTC_DS1307 RTC;
	Serial.print(F("IP REMOTA A VALIDAR "));
	Serial.println(ipremota);
	int sessiones = 0;
	DateTime tiempoactual = RTC.now();

	if (esAccesoLocal(ipremota)){
		return true;
	}

/*	while (sessiones < cicloSessionUsuarios) {
		if (ipConectadas[sessiones] == ipremota) {
			DateTime finsession(ipConectadasHoraLogin[sessiones].unixtime() + sessiontimeout);
			Serial.print(F("si la ip es valida saldra aca-> "));
			Serial.println(ipConectadas[sessiones]);
			Serial.print(F("Fin de session: "));
			Serial.print(finsession.hour());
			Serial.print(F(":"));
			Serial.println(finsession.minute());

			Serial.print(F("Tiempo actual: "));
			Serial.print(tiempoactual.hour());
			Serial.print(F(":"));
			Serial.println(tiempoactual.minute());

			if ((finsession.hour() <= tiempoactual.hour()) && (finsession.minute() <= tiempoactual.minute()))
			{
				Serial.print(F("Session "));
				Serial.print(ipConectadas[sessiones]);
				Serial.print(F(" VENCIDA"));
				return false;
			}else
			{
			return true;
			}
			//  finsession = ipConectadasHoraLogin[sessiones] + sessiontimeout;
		}
		sessiones++;
	}*/

	return true;
}

boolean esAccesoLocal(IPAddress ipremota){
	
	if (bIp[0] == ipremota[0])
  {
		if (bIp[1] == ipremota[1])
	{
			if (bIp[2] == ipremota[2]){
				return true;
		}
	}
  }

	return false;
}

String iptraeNombreUsuario(IPAddress ipremota){
	if (esAccesoLocal(ipremota)){
		return "Local";
	}else
	{
	return "Via web";
	}
}
