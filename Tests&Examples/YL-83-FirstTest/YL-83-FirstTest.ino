int sensor_pin = A0;

void setup() {
  pinMode(sensor_pin, INPUT);
  Serial.begin(9600);
  Serial.println("***************");
  Serial.println("* Messstation *");
  Serial.println("***************");  
  messenRegen();
  ESP.deepSleep(5000000);
}

void messenRegen() {
  unsigned int messwert = analogRead(sensor_pin);
  Serial.println(String(messwert));
  /*Serial.println("******************* Regen Messung *******************");
  if(messwert < 256) {
    Serial.println("Wolkenbruch");
  }
  else
  if(messwert < 512) {
    Serial.println("Starkregen");
  }
  else
  if(messwert < 768) {
    Serial.println("Leichtregen");
  }
  else {
    Serial.println("Trocken => Kein Regen");    
  }
  Serial.println();
  */
}

void loop() {

}
