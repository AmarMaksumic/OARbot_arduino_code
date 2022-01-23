#define top_lim 3
#define bot_lim 4 

void setup() {
  pinMode(top_lim, INPUT_PULLUP);
  pinMode(bot_lim, INPUT_PULLUP);
  Serial.begin(115200);

}

void loop() {
  delay(20);
  Serial.print( 2 * digitalRead(top_lim) + 1 * digitalRead(bot_lim) );
}
