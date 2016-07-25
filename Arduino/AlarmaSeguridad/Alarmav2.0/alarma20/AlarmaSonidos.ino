void sonidoAgudoAlarma(){
  tone(constParlantePin, NOTE_D7, 500); //DS mas agudo
 }

void controlLedInterno(byte estado){
	digitalWrite(constLedInterno,estado);
}