#define EncoderR 2
#define EncoderL 3

#define ENA 9
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define ENB 10

#define T 1000
#define PULSES_PER_TURN_L 30
#define PULSES_PER_TURN_R 55

#define R 0.031

void EncoderRClicks();
void EncoderLClicks();

int PulseCountR, PulseCountL;

void setup(){
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  //intialize encoders' pin and attach the interupt
  pinMode(EncoderR, INPUT);
  pinMode(EncoderL, INPUT);
  
  digitalWrite(EncoderR, HIGH);
  digitalWrite(EncoderL, HIGH);
  attachInterrupt(digitalPinToInterrupt(EncoderR), EncoderRClicks, RISING);
  attachInterrupt(digitalPinToInterrupt(EncoderL), EncoderLClicks, RISING);

}

void loop() {
/*
  double RPM_R = (PulseCountR * (60000.f / T)) / (PULSES_PER_TURN_R);
  double RPM_L = (PulseCountL * (60000.f / T)) / (PULSES_PER_TURN_L);
  PulseCountR = PulseCountL = 0;

  double VR = 2*PI*R*RPM_R/60;
  double VL = 2*PI*R*RPM_L/60;

  double SlopeR = .0;
  double SlopeL = .06;

  int voltR = (int)(0.37/SlopeR);
  voltR = map(voltR, 0, 7.92, 0, 220);
  
  int voltL = (int)(0.37/SlopeL);
  voltL = map(voltL, 0, 9, 0, 255);

  // RIGHT
  analogWrite(ENA, 220);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // LEFT
  analogWrite(ENB, 255);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);
  
  Serial.print("Left: ");
  Serial.println(RPM_L);Serial.println(VL);
  Serial.print("Right: ");
  Serial.println(RPM_R);Serial.println(VR);
  */

  

  
  delay(T);
}



void EncoderRClicks(){
  PulseCountR++;
  Serial.print("Right: ");
  Serial.println(PulseCountR);
  
}
void EncoderLClicks(){
  PulseCountL++;
    Serial.print("Left: ");
  Serial.println(PulseCountL);
}

