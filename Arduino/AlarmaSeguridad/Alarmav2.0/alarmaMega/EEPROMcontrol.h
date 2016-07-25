#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_graba(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_lee(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

void EEPROM_limpiar(){
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
}
