#include <Wire.h>
#include <RTClock.h>
#include "ds3231.h"
RTClock rtclock (RTCSEL_LSE); // initialise

const char * weekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char * months[] = {"Dummy", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);
    
    Serial.begin(115200);    
    delay(2000);
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("Test board MakeArduino STM32-F103RE");
    char testdata[] = "TEST"; // data to write
    char testread[sizeof(testdata)+1];
    Wire.begin(); // initialise the connection
    //Wire.setClock(10000);             
    i2c_eeprom_read_buffer(0x50,0, (byte *)testread,sizeof(testdata));
    if(strcmp(testdata, testread) != 0)
    {
      Serial.print("Write \"");
      Serial.print(testdata);
      Serial.println("\" to EEPROM address 0");    
      i2c_eeprom_write_page(0x50, 0, (byte *)testdata, sizeof(testdata)); // write to EEPROM          
      delay(1000);
    }
    Serial.print("Read from EEPROM address 0 :");        
    i2c_eeprom_read_buffer(0x50,0, (byte *)testread,sizeof(testdata));
    /*int addr=0;
    byte b = i2c_eeprom_read_byte(0x50, addr); // access the first address from the memory
    while (b!=0 && addr < sizeof(testdata))
    {
        Serial.print((char)b); //print content to serial port
        testdata[addr] = (char)b;
        addr++; //increase address
        b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    }*/    
    Serial.println(testdata); 
    bool test_fail = false;
    for(int i=0;i<sizeof(testdata);i++){
      if(testread[i] != testdata[i]){
        test_fail = true;
        break;
      }
    }
      
    if(test_fail){
      Serial.println("Test EEPROM : Failed!");
    }
    else{
      Serial.println("Test EEPROM : pass");
    }    

    char s[30];
    Serial.println("Test DS3231 RTC");
    DS3231_init(DS3231_INTCN);
    struct ts t;
    DS3231_get(&t);
    snprintf(s, 30, "%d.%02d.%02d %02d:%02d:%02d", t.year,t.mon, t.mday, t.hour, t.min, t.sec);
    Serial.print("DS3231 RTC timestamp:");
    Serial.println(s);
    Serial.print("read temperature from DS3231 : ");
    Serial.println(DS3231_get_treg(), DEC);

    Serial.println("Test internal RTC use LSE 32.768kHz");    
    tm_t mtt;    
    rtclock.breakTime(rtclock.now(), mtt);
    Serial.print("Internal RTC (LSE) timestamp:");
    snprintf(s, 30,"%s %u %u, %s, %02u:%02u:%02u", months[mtt.month], mtt.day, mtt.year+1970, weekdays[mtt.weekday], mtt.hour, mtt.minute, mtt.second);
    Serial.println(s);

    digitalWrite(LED_BUILTIN,HIGH);
}

void loop()
{
    digitalWrite(LED_BUILTIN,LOW);
    delay(100);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(900);
}

