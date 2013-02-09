
#include <OneWire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int CarbonDioxide; 

OneWire  ds(7);       
byte i;
byte present = 0;
byte data[12];
byte addr[8];
  
int HighByte, LowByte, SignBit, Whole, Fract, TReading, Tc_100, FWhole;

void setup(void) {
  
 lcd.begin(4,20);              
  lcd.clear();                  
  lcd.setCursor(0,0);  
  
    if ( !ds.search(addr)) {
      lcd.clear(); lcd.print("No more addrs");
      delay(1000);
      ds.reset_search();
      return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      lcd.clear(); lcd.print("CRC not valid!");
      delay(1000);
      return;
  }
}

void getTemp() {
  int foo, bar;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  
  
  if (SignBit) {
    TReading = -TReading;
  }
  Tc_100 = (6 * TReading) + TReading / 4;    
  Whole = Tc_100 / 100;          
  Fract = Tc_100 % 100;
  if (Fract > 49) {
    if (SignBit) {
      --Whole;
    } else {
      ++Whole;
    }
  }

  if (SignBit) {
    bar = -1;
  } else {
    bar = 1;
  }
  foo = ((Whole * bar) * 18);      
  FWhole = (((Whole * bar) * 18) / 10) + 32;
  if ((foo % 10) > 4) {            
       ++FWhole;
  }
}

void printTemp(void) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp is: ");
  lcd.setCursor(0,1);   
  
  if (SignBit) {  
     lcd.print("-");
  }
  lcd.print(Whole);
  lcd.print(" C / ");
  lcd.print(FWhole);
  lcd.print(" F");
  lcd.setCursor(0,2);
  lcd.print("CarbonDioxide:");
  lcd.print(CarbonDioxide);
  lcd.setCursor(0,3);
   lcd.createChar(14, tl);
  lcd.createChar(13, tr);
  lcd.createChar(12, bl);
  lcd.createChar(11, br);
  lcd.setCursor(14,3);
  lcd.print("=");
  lcd.setCursor(13,3);
  lcd.print("=");
lcd.setCursor(12,3);
  lcd.print("=");
lcd.setCursor(11,3);
  lcd.print("=");
lcd.setCursor(10,3);
  lcd.print("=");
lcd.setCursor(9,3);
  lcd.print("=");
lcd.setCursor(8,3);
  lcd.print("=");
lcd.setCursor(7,3);
  lcd.print("=");
lcd.setCursor(6,3);
  lcd.print("=");
lcd.setCursor(5,3);
  lcd.print("=");
lcd.setCursor(4,3);
  lcd.print("=");
lcd.setCursor(3,3);
  lcd.print("=");
lcd.setCursor(2,3);
  lcd.print("=");
  lcd.setCursor(15,3);
  lcd.print("F");
  lcd.setCursor(0,3);
  lcd.print("E");
}

void loop(void) {
  lcd.clear();
  lcd.setCursor(0,0);
  CarbonMonoxide = analogRead(0);
  lcd.print(CarbonMonoxide, DEC);
  getTemp();
  printTemp();
  delay(10000);
}
