// CONTROL GHOSTS

#include <math.h>

struct pos {
  int x, y;
  pos(int a, int b) {
    x = a;
    y = b;
  }
};

struct Robot {
  double x, y, theta;
  Robot() {
    x = y = theta = 0;
  }
};


//define pins
#define MOTORR_PWM 9
#define MOTORL_PWM 10
#define MR_dir1 4
#define MR_dir2 5
#define ML_dir1 6
#define ML_dir2 7
#define EncoderR 2
#define EncoderL 3

#define MAX_SAT_R 7.92
#define MAX_SAT_L 9

#define Ts 50

#define R 0.031
#define L 0.115
#define Cr 2.55
#define Cl 2.25
#define PULSES_PER_REV_R 55
#define PULSES_PER_REV_L 30


#define Kp 10
#define Ki 0
#define Kd 0
double integrator, lastError;
double PulseCountR, PulseCountL;

#define N_GOALS 3
#define TOLERANCE 0.05
//pos goals[N_GOALS] = { pos(1, 1), pos(1, 2), pos(2, 2), pos(3, 2), pos(3, 3) };
pos goals[N_GOALS] = { pos(1, 1), pos(1,0), pos(0,0)};
int goal_idx;

Robot robot;





//Function prototyping
bool checkGoal(pos goal, Robot robot);  //check if the robot reaches the goal ?
double theta_now(pos goal, Robot robot);  //Theta of the goal in respect of Robot coordinates
double PID_action(double error);  //PID controller: take the angle error and return omega
void applyVoltage(double VR, double VL);  //apply voltage to left and right motor - handel the direction of motor
void EncoderRClicks();  //
void EncoderLClicks();  //





void setup() {
  Serial.begin(9600);

  //Motor and H bridge pins
  pinMode(MOTORR_PWM, OUTPUT);
  pinMode(MOTORL_PWM, OUTPUT);
  pinMode(MR_dir1, OUTPUT);
  pinMode(MR_dir2, OUTPUT);
  pinMode(ML_dir1, OUTPUT);
  pinMode(ML_dir2, OUTPUT);

  //intialize encoders' pin and attach the interupt
  pinMode(EncoderR, INPUT);
  pinMode(EncoderL, INPUT);
  digitalWrite(EncoderR, HIGH);
  digitalWrite(EncoderL, HIGH);
  attachInterrupt(digitalPinToInterrupt(EncoderR), EncoderRClicks, RISING);
  attachInterrupt(digitalPinToInterrupt(EncoderL), EncoderLClicks, RISING);
}




void loop() {
  double theta_error = theta_now(goals[goal_idx], robot);
  double w = PID_action(theta_error);
  double v = 0.4;
  double VL = (v - w * L / 2) / (R * Cl);
  double VR = (v + w * L / 2) / (R * Cr);
  if(VL > MAX_SAT_L) VL = MAX_SAT_L;
  else if(VL<-MAX_SAT_L) VL = -MAX_SAT_L;
  if(VR > MAX_SAT_R) VR = MAX_SAT_R;
  else if(VR<-MAX_SAT_R) VR = -MAX_SAT_R;
  applyVoltage(VR, VL);

  double DR = 2 * PI * R * PulseCountR / PULSES_PER_REV_R;
  double DL = 2 * PI * R * PulseCountL / PULSES_PER_REV_L;
  double DC = (DR + DL) / 2;
  PulseCountR = PulseCountL = 0;

  robot.x = robot.x + DC * cos(robot.theta);
  robot.y = robot.y + DC * sin(robot.theta);
  robot.theta = robot.theta + (DR - DL) / L;

  goal_idx += checkGoal(goals[goal_idx], robot);
  
  if (goal_idx == N_GOALS) {
    analogWrite(MOTORR_PWM, 0);
    analogWrite(MOTORL_PWM, 0);
    while(1){}
  }

  Serial.print("VL: ");
  Serial.println(VL);
  Serial.print("VR: ");
  Serial.println(VR);
  
  Serial.print("X: ");
  Serial.println(robot.x);
  Serial.print("Y: ");
  Serial.println(robot.y);
  Serial.print("theta: ");
  Serial.println(robot.theta);
  
  delay(Ts);
}





double theta_now(pos goal, Robot robot) {
  return atan2(goal.y - robot.y, goal.x - robot.x) - robot.theta;
}



double PID_action(double error) {
  double u, derr;

  integrator += error;
  derr      = error - lastError;
  lastError = error;
  u = Kp * error + Ki * integrator + Kd * derr;

  return u;
}



void applyVoltage(double VR, double VL) {
  analogWrite(MOTORR_PWM, map(abs(VR), 0, MAX_SAT_R, 0, 220));
  analogWrite(MOTORL_PWM, map(abs(VL), 0, MAX_SAT_L, 0, 255));
  if (VR > 0) {
    digitalWrite(MR_dir1, LOW);
    digitalWrite(MR_dir2, HIGH);
  } else {
    digitalWrite(MR_dir1, HIGH);
    digitalWrite(MR_dir2, LOW);
  }

  if (VL > 0) {
    digitalWrite(ML_dir1, HIGH);
    digitalWrite(ML_dir2, LOW);
  } else {
    digitalWrite(ML_dir1, LOW);
    digitalWrite(ML_dir2, HIGH);
  }
}



bool checkGoal(pos goal, Robot robot) {
  return ((abs(robot.x - goal.x) <= TOLERANCE) && (abs(robot.y - goal.y) <= TOLERANCE));
}




void EncoderRClicks() {
  PulseCountR++;

}
void EncoderLClicks() {
  PulseCountL++;
}

