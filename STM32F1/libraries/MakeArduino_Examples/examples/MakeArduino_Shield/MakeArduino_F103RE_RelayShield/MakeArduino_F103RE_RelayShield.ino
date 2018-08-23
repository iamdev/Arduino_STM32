#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>
#include <ds3231.h>

#define TFT_USE_SPI
#define TFT_BL     PB0
#define TFT_CS     PB12
#define TFT_RST    PB1  // you can also connect this to the Arduino reset
#define TFT_DC     PB2
#define TFT_SCLK   PB13
#define TFT_MOSI   PB15
#define RGB(r,g,b)  ((((r>>3)&0x1F)<<11)|(((g>>2)&0x3F)<<5)|((b>>3)&0x1F))
//R=1111100000000000
//G=0000011111100000
#ifdef TFT_USE_SPI
SPIClass spi2(2);
Adafruit_ST7735 tft = Adafruit_ST7735(&spi2,TFT_CS, TFT_DC, TFT_RST);
#else
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#endif

#define INPUT_DELAY 100
#define ON  LOW
#define OFF HIGH
int INPUT_PINS[] = {PC2,PC3};
int RELAY_PINS[] = {PC5,PC4};
int relay_state[] = {OFF,OFF};
long input_state[]={0,0};

#define BUFF_MAX 128
uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;
void parse_serial_cmd(char *cmd, int cmdsize);
long l_date = 0;

void blink(int n,int t1=500,int t2=500){
  pinMode(LED_BUILTIN,OUTPUT);
  for(int i=0;i<n;i++){
    digitalWrite(LED_BUILTIN,LOW);
    delay(t1);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(t2);
  }
}

void drawState(int n, int state){
  //tft.drawRect(34,90+(n*35),60,30,RGB(255,255,255));
  tft.fillRect(5+n*66,100,52,52,state==ON?0x3F<<5:0x1F<<11);  
  tft.setTextColor(0);
  tft.setTextSize(5);
  tft.setCursor(5+n*66+15,108);
  tft.print(n+1);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  
  pinMode(TFT_BL,PWM);
  pwmWrite(TFT_BL,0);
  delay(1000);
  for(int i=0;i<2;i++)pinMode(INPUT_PINS[i],INPUT);
  for(int i=0;i<2;i++){
    pinMode(RELAY_PINS[i],OUTPUT);  
    digitalWrite(RELAY_PINS[i],HIGH);
  }  

#ifdef TFT_USE_SPI
  spi2.begin();
  spi2.setClockDivider(4);  
#endif  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(0);
  delay(100);
  pwmWrite(TFT_BL,0xFFFF/4);
  
  drawState(0,OFF);
  drawState(1,OFF);
  blink(5,200,200);
}

void parse_serial_cmd(char *cmd, int cmdsize)
{
    uint8_t i;
    uint8_t reg_val;
    char buff[BUFF_MAX];
    struct ts t;

    //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
    //Serial.print(buff);

    // TssmmhhWDDMMYYYY aka set time
    if (cmd[0] == 84 && cmdsize == 16) {
        //T355720619112011
        t.sec = inp2toi(cmd, 1);
        t.min = inp2toi(cmd, 3);
        t.hour = inp2toi(cmd, 5);
        t.wday = cmd[7] - 48;
        t.mday = inp2toi(cmd, 8);
        t.mon = inp2toi(cmd, 10);
        t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
        DS3231_set(t);
        Serial.println("OK");
    } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
        Serial.print("aging reg is ");
        Serial.println(DS3231_get_aging(), DEC);
    } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
        DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
        //ASSMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
        }
        uint8_t flags[5] = { 0, 0, 0, 0, 0 };
        DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
        DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
        //BMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
        }
        uint8_t flags[5] = { 0, 0, 0, 0 };
        DS3231_set_a2(time[0], time[1], time[2], flags);
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
        Serial.print("temperature reg is ");
        Serial.println(DS3231_get_treg(), DEC);
    } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
        reg_val = DS3231_get_sreg();
        reg_val &= B11111100;
        DS3231_set_sreg(reg_val);
    } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
        reg_val = DS3231_get_addr(0x5);
        Serial.print("orig ");
        Serial.print(reg_val,DEC);
        Serial.print("month is ");
        Serial.println(bcdtodec(reg_val & 0x1F),DEC);
    } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
        DS3231_set_aging(0);
    } else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
        Serial.print("status reg is ");
        Serial.println(DS3231_get_sreg(), DEC);
    } else {
        Serial.print("unknown command prefix ");
        Serial.println(cmd[0]);
        Serial.println(cmd[0], DEC);
    }
}

void scanInput(){
  for(int i=0;i<2;i++){
    if(!digitalRead(INPUT_PINS[i])){
      if(input_state[i]==0){
        input_state[i] = millis();  
      }
    }else{
      input_state[i] = 0;
    }
  }
}

int chkInput(int b){
  if(input_state[b]>0){
      return millis() - input_state[b];
  }
  return -1;
}
long dt_ms=0;
void loop() {  
  scanInput();
  for(int i=0;i<2;i++){
    if(chkInput(i)>INPUT_DELAY){
      if(relay_state[i]==OFF){
        digitalWrite(RELAY_PINS[i],ON);
        relay_state[i] = ON;
        drawState(i,ON);
        delay(10);
      }
    }else{
      if(relay_state[i]==ON){
        digitalWrite(RELAY_PINS[i],OFF);
        relay_state[i] = OFF;  
        drawState(i,OFF);
        delay(10);
      }    
    }
  }
  delay(10);
  if((millis()-dt_ms)>1000){
    dt_ms = millis();
    struct ts t;
    DS3231_get(&t); 
    char buf[10];
    long d = t.year*1000+t.mon*100+t.mday;
    tft.setTextSize(2);
    tft.setTextColor(0xFFFF);
    if(d!= l_date){ 
      l_date = d;
      snprintf(buf, 30, "%02d/%02d/%04d",t.mday, t.mon, t.year);
      tft.fillRect(0,5,128,18,0);    
      tft.setCursor(4,6);
      tft.print(buf);
    }
    snprintf(buf, 30, "%02d:%02d:%02d",t.hour, t.min, t.sec);
    tft.fillRect(9,24,110,16,0);    
    tft.setCursor(10,25);
    tft.print(buf);
  }

  if (Serial.available() > 0) {
    char in = Serial.read();
  
    if ((in == 10 || in == 13) && (recv_size > 0)) {
        parse_serial_cmd(recv, recv_size);
        recv_size = 0;
        recv[0] = 0;
    } else if (in < 48 || in > 122) {;       // ignore ~[0-9A-Za-z]
    } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
        // drop
        recv_size = 0;
        recv[0] = 0;
    } else if (recv_size < BUFF_MAX - 2) {
        recv[recv_size] = in;
        recv[recv_size + 1] = 0;
        recv_size += 1;
    }
  }
}

