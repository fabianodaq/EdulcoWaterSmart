#include "03_EEprom.h"

void        EEPR_WriteCh32  (int start, const char* data);
void        EEPR_ReadCh32   (int start, char* buffer);
void        EEPR_WriteCh16  (int start, const char* data);
void        EEPR_ReadCh16   (int start, char* buffer);

void        EEPR_WriteByte  (int addr, uint8_t value);
uint8_t     EEPR_ReadByte   (int addr);

void        EEPR_WriteUint16(int addr, uint16_t value);
uint16_t    EEPR_ReadUint16 (int addr);

void        EEPR_WriteInt16 (int addr, int16_t value);
int16_t     EEPR_ReadInt16  (int addr);


void EEPR_WriteCh32(int start, const char* data) {
  for (int i = 0; i < 32; i++) {
    char c = data[i];
    EEPROM.write(start + i, c);
    if (c == '\0') break;  // Fermati appena trovi fine stringa
  }
  EEPROM.commit();
}

void EEPR_ReadCh32(int start, char* buffer) {
  for (int i = 0; i < 32; i++) {
    char c = EEPROM.read(start + i);
    buffer[i] = c;
    if (c == '\0') break;  // Fermati appena trovi fine stringa
  }
  buffer[31] = '\0';  // Sicurezza finale
}

void EEPR_WriteCh16(int start, const char* data) {
  for (int i = 0; i < 16; i++) {
    char c = data[i];
    EEPROM.write(start + i, c);
    if (c == '\0') break;
  }
  EEPROM.commit();
}

void EEPR_ReadCh16(int start, char* buffer) {
  for (int i = 0; i < 16; i++) {
    char c = EEPROM.read(start + i);
    buffer[i] = c;
    if (c == '\0') break;
  }
  buffer[15] = '\0';  // Sicurezza finale
}

// Scrive 1 byte (uint8_t)
void EEPR_WriteByte(int addr, uint8_t value) {
  EEPROM.write(addr, value);
  EEPROM.commit();
}

// Legge 1 byte (uint8_t)
uint8_t EEPR_ReadByte(int addr) {
  return EEPROM.read(addr);
}

// Scrive un intero unsigned 16 bit
void EEPR_WriteUint16(int addr, uint16_t value) {
  EEPROM.write(addr, value & 0xFF);
  EEPROM.write(addr + 1, (value >> 8) & 0xFF);
  EEPROM.commit();
}

// Legge un intero unsigned 16 bit
uint16_t EEPR_ReadUint16(int addr) {
  uint16_t value = EEPROM.read(addr);
  value |= (EEPROM.read(addr + 1) << 8);
  return value;
}

// Scrive un intero signed 16 bit
void EEPR_WriteInt16(int addr, int16_t value) {
  EEPROM.write(addr, value & 0xFF);
  EEPROM.write(addr + 1, (value >> 8) & 0xFF);
  EEPROM.commit();
}

// Legge un intero signed 16 bit
int16_t EEPR_ReadInt16(int addr) {
  uint8_t low = EEPROM.read(addr);
  uint8_t high = EEPROM.read(addr + 1);
  return (int16_t)((high << 8) | low);
}