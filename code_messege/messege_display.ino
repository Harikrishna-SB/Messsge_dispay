#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
 char string1[255]={"   0123456789 ABCDEFGHIJKLMOPQRSTUVWXYZ..--              HELLO WORLD           SHINOD P C  BINDUSHINOD        ELECTRO    "};
 char string2[255]={"   SHINOD P C  BINDUSHINOD   "};
 char string3[255]={"   "};
 int ascii[256];
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 4095; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

byte data[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x06, 0x0e, 0x00 ,0x38, 0x00, 0x37, 0x3f, 
                   0x73, 0xcf, 0x50, 0x6d, 0x78, 0x3e, 0x00, 0x00, 0x00, 0x6e, 0x5b, 0x80, 0x40};

  void sentence(int s,char string[255])
{
int j=0;
for(int k=0;string[k]!='\0';k++)
{Serial.print(string[k]);
 ascii[j]=string[k]-48;
 if(ascii[j]==-16)
 {
  ascii[j]=27;
 }
 if(ascii[j]==-2)
 {
  ascii[j]=43;
 }
 if(ascii[j]==-3)
 {
  ascii[j]=44;
 }
j++;
}
int l;
l=strlen(string);
 
 int i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value,data[ascii[i]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+256,data[ascii[i+1]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+512,data[ascii[i+2]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+768,data[ascii[i+3]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+1024,data[ascii[i+4]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+1280,data[ascii[i+5]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+1536,data[ascii[i+6]]);
  i++;
 }
 i=0;
 for(int value= s;value<=l;value++)
 {
  writeEEPROM(value+1792,data[ascii[i+7]]);
  i++;
 }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);

  // Erase entire EEPROM
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 4096; address += 1) {
    writeEEPROM(address, 0x00);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");


 sentence(0,string1);
 //sentence(2048,string2);
 
 
 Serial.println(" ");
 printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}
