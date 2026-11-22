#define RXD1 18
#define TXD1 17

void setup() {
  // put your setup code here, to run once:
Serial2.begin(9600, SERIAL_8N1, RXD1, TXD1);
Serial.begin(9600);
Serial.write("Arduino Mega Serial1 test -  for loopback test connect pin 18 to pin 19\n");
  Serial.write("RS232: Mega pin 18 TXD to TTL/RD232 Tx and pin 19 RXD to TTL/RS232 Rx\n");
  Serial.write("RS232 - loopback connect 9-pin D-type pin 2 Tx to pin 3 Rx\n");
  delay(100);
}

void loop(){
  Serial2.write("gab\n");
    delay(300);
}