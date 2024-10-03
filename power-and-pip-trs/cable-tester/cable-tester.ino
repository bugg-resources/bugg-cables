#define PIN_BUZZER 12
  
#define PIN_M12_IN_VIN 2
#define PIN_M12_GND 3
#define PIN_M12_MIC_IN_N 4
#define PIN_M12_MIC_IN_P 5
#define PIN_M12_GNDA 6
  
#define PIN_CABLE_VIN 7
#define PIN_CABLE_GND 8
#define PIN_CABLE_TIP 9
#define PIN_CABLE_RING 10
#define PIN_CABLE_SLEEVE 11

#define IN_VIN 0
#define GND 1
#define MIC_IN_N 2
#define MIC_IN_P 3
#define GNDA 4

void clear_all(){
  digitalWrite(PIN_CABLE_VIN, HIGH);
  digitalWrite(PIN_CABLE_GND, HIGH);
  digitalWrite(PIN_CABLE_TIP, HIGH);
  digitalWrite(PIN_CABLE_RING, HIGH);
  digitalWrite(PIN_CABLE_SLEEVE, HIGH);
}

void set(uint8_t pin){
  digitalWrite(pin, LOW);
}

void clear(uint8_t pin){
  digitalWrite(pin, HIGH);
}

uint8_t get_connections(){
  uint8_t connected = 0;

  if (digitalRead(PIN_M12_IN_VIN) == false) connected |= 1 << IN_VIN;
  if (digitalRead(PIN_M12_GND) == false) connected |= 1 << GND;
  if (digitalRead(PIN_M12_MIC_IN_N) == false) connected |= 1 << MIC_IN_N;
  if (digitalRead(PIN_M12_MIC_IN_P) == false) connected |= 1 << MIC_IN_P;
  if (digitalRead(PIN_M12_GNDA) == false) connected |= 1 << GNDA; 

  return connected;
}

void print_connections(uint8_t connected){
  if (connected & 1<<IN_VIN) Serial.print(" IN_VIN");
  if (connected & 1<<GND) Serial.print(" GND");
  if (connected & 1<<MIC_IN_N) Serial.print(" MIC_IN_N");
  if (connected & 1<<MIC_IN_P) Serial.print(" MIC_IN_P");
  if (connected & 1<<GNDA) Serial.print(" GNDA");
  if (connected == 0) Serial.print (" NONE");
}

bool check_connections(){
  uint8_t result = 0;
  uint8_t connected;
  char wire_name[][10] = {
    "VIN",
    "GND",
    "TIP",
    "RING",
    "SLEEVE"
  };
  uint8_t output_pin[] = {
    PIN_CABLE_VIN,
    PIN_CABLE_GND,
    PIN_CABLE_TIP,
    PIN_CABLE_RING,
    PIN_CABLE_SLEEVE
  };
  uint8_t correct[] = { 
    1 << IN_VIN,
    1 << GND,
    1 << MIC_IN_P,
    0,
    (1 << GNDA) | (1 << MIC_IN_N)
  };

  for (uint8_t wire = 0; wire < 5; wire++)
  {
    set(output_pin[wire]);
    Serial.print("Wire "); Serial.print(wire_name[wire]); Serial.print(" connected to");
    connected = get_connections();
    print_connections(connected);
    if (connected != correct[wire])
      result += 1;
    clear(output_pin[wire]);  
    Serial.println();
  }
  return result;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Boot");

  pinMode(PIN_M12_IN_VIN, INPUT_PULLUP);
  pinMode(PIN_M12_GND, INPUT_PULLUP);
  pinMode(PIN_M12_MIC_IN_N, INPUT_PULLUP);
  pinMode(PIN_M12_MIC_IN_P, INPUT_PULLUP);
  pinMode(PIN_M12_GNDA, INPUT_PULLUP);
  
  pinMode(PIN_CABLE_VIN, OUTPUT);
  pinMode(PIN_CABLE_GND, OUTPUT);
  pinMode(PIN_CABLE_TIP, OUTPUT);
  pinMode(PIN_CABLE_RING, OUTPUT);
  pinMode(PIN_CABLE_SLEEVE, OUTPUT);
  
  pinMode(PIN_BUZZER, OUTPUT);

  tone(PIN_BUZZER, 1000, 200);

  clear_all();
  
  if (check_connections() == 0){
    Serial.println("Cable is OK");
    tone(PIN_BUZZER, 2000, 200);
  }
  else {
    Serial.println("Cable is DEFECTIVE");
    tone(PIN_BUZZER, 100, 2000);
  }
  
}

void loop() {

}
