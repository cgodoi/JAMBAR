//Limpia un string
void StrClear(char *str, char length)
{
	for (int i = 0; i < length; i++) {
		str[i] = 0;
	}
}

//String a integer
int string2int(String string){
	char cadena[(string.length() + 1)];
	string.toCharArray(cadena, string.length() + 1);
	return atol(cadena);
}

// busca por el string sfind en el string str, 1 si encuentra 0 si no
char StrContains(char *str, char *sfind)
{
	char found = 0;
	char index = 0;
	char len;

	len = strlen(str);

	if (strlen(sfind) > len) {
		return 0;
	}
	while (index < len) {
		if (str[index] == sfind[found]) {
			found++;
			if (strlen(sfind) == found) {
				return 1;
			}
		}
		else {
			found = 0;
		}
		index++;
	}

	return 0;
}

//Muestra ram disponible
int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void XML_enviafechahora(EthernetClient micliente)
{
	RTC_DS1307 RTC;
	DateTime now = RTC.now();
	int numMes = 0;
	numMes = now.month();
	micliente.println("Content-Type: text/xml");
	micliente.println("Connection: keep-alive");
	micliente.println();
	micliente.print("<?xml version = \"1.0\" ?>");
	micliente.print("<inputs>");
	micliente.print("<time>");
	micliente.print(now.hour(), DEC);
	micliente.print(":");
	micliente.print(now.minute(), DEC);
	micliente.print("  ");
	micliente.print(now.day(), DEC);
	micliente.print(" ");
	micliente.print(mesNumeroaNombre(numMes));
	micliente.print(" ");
	micliente.print(now.year(), DEC);
	micliente.println("</time>");
	micliente.print("</inputs>");
	micliente.stop();
}

String mesNumeroaNombre(byte mesnro){
	//char* arrmeses[12] = { "ENERO", "FEBRERO", "MARZO", "ABRIL", "MAYO", "JUNIO", "JULIO", "AGOSTO", "SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
	char* arrmeses[13] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };

	return arrmeses[mesnro - 1];
}

void controlFechaHora(){
	RTC_DS1307 RTC;
	DateTime horafechaActual = RTC.now();

	if (horafechaActual.year() == horafechaAnterior.year()){
		if (horafechaActual.month() == horafechaAnterior.month()){
			if (horafechaActual.day() == horafechaAnterior.day()){
				if (horafechaActual.hour() > horafechaAnterior.hour()){
						//sumar hora control luz
						controldehorasOn();
						horafechaAnterior = horafechaActual;

				}
			}
		}

	}
}
