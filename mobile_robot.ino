// CONTROL GHOSTS
/*
   Includes
*/
#include <math.h>
#define SC 0.66
//#define SC 1



//Define pins
#define MOTORR_PWM 9
#define MR_dir1 4
#define MR_dir2 5
#define MOTORL_PWM 10
#define ML_dir1 6
#define ML_dir2 7
#define EncoderR 2
#define EncoderL 3

//Max voltage for Right&Left motor
//Note: since the Right motor is faster than Left one, at 7.92 volt in R motor gives the same speed as L motor in at 9 volt.
#define MAX_SAT_R 7.92
#define MAX_SAT_L 9

#define Ts 50


#define R 0.031 //raidus of the wheels
#define L 0.115 //distance between 2 wheels
#define Cr 2.55 //Rigth_motor_speed = 2.55 * voltage
#define Cl 2.25 //Left_motor_speed  = 2.25 * voltage

#define PULSES_PER_REV_R 55
#define PULSES_PER_REV_L 33.5

#define Kp 10
#define Ki 0
#define Kd 0


// data types to store goals as x,y point
struct pos {
  double x, y;
  pos(double a, double b) {
    x = a * SC;
    y = b * SC;
  }
};

// Robot location and direction
struct Robot {
  double x, y, theta;
  Robot() {
    x = y = theta = 0;
  }
};



double integrator, lastError;
volatile double PulseCountR, PulseCountL;
int DirL = 1, DirR = 1; // used to know the rotation direction (clockwise or anti-clockwise)



//range of tolerance for reaching the goal.
#define TOLERANCE 0.05 
#define N_GOALS 1
pos goals[N_GOALS] = { pos(1, 0) }; 

//#define N_GOALS 10
//pos goals[N_GOALS] = {pos(1,0), pos(2,0), pos(3,0)};
//pos goals[N_GOALS] = {pos(1,0),pos(2,1),pos(3,2),pos(4,3),pos(4,4),pos(3,5),pos(2,5),pos(1,4),pos(1,3),pos(0,0)};
int goal_idx; //point to the current goal


// create our robot 
Robot robot;



//Function prototyping
bool checkGoal(pos goal, Robot robot);  //check if the robot reaches the goal ?
double theta_now(pos goal, Robot robot);  //Theta of the goal in respect of Robot coordinates
double PID_action(double error);  //PID controller: take the angle error and return omega
void applyVoltage(double VR, double VL);  //apply voltage to left and right motor - handel the direction of motor
void EncoderRClicks();  
void EncoderLClicks(); 



void setup() {
  Serial.begin(9600);

  //Motor and H-bridge pins
  pinMode(MOTORR_PWM, OUTPUT);
  pinMode(MOTORL_PWM, OUTPUT);
  pinMode(MR_dir1, OUTPUT);
  pinMode(MR_dir2, OUTPUT);
  pinMode(ML_dir1, OUTPUT);
  pinMode(ML_dir2, OUTPUT);

  //intialize encoders' pins and attach the interupt
  pinMode(EncoderR, INPUT);
  pinMode(EncoderL, INPUT);
  digitalWrite(EncoderR, HIGH);
  digitalWrite(EncoderL, HIGH);
  attachInterrupt(digitalPinToInterrupt(EncoderR), EncoderRClicks, RISING);
  attachInterrupt(digitalPinToInterrupt(EncoderL), EncoderLClicks, RISING);
}




void loop() {
  double theta_error, w, v, VL, VR;
  
  double DR = 2 * PI * R * (PulseCountR / PULSES_PER_REV_R); //DirR equalls to 1 or -1 depend on the rotation direction of the motor
  double DL = 2 * PI * R * (PulseCountL / PULSES_PER_REV_L);
  double DC = (DR + DL) / 2;
  PulseCountR = PulseCountL = 0;

  robot.x = robot.x + DC * cos(robot.theta);
  robot.y = robot.y + DC * sin(robot.theta);
  robot.theta = robot.theta + (DR - DL) / L;

  
  if(  checkGoal(goals[goal_idx], robot) ) {
      goal_idx ++;
      analogWrite(MOTORR_PWM, 0);
      analogWrite(MOTORL_PWM, 0);
      delay(1000);
      
  }
  
  if (goal_idx == N_GOALS) {
    analogWrite(MOTORR_PWM, 0);
    analogWrite(MOTORL_PWM, 0);
    while (1) {}
  }

  //Serial.print("VL: ");
  //Serial.println(VL);
  //Serial.print("VR: ");
  //Serial.println(VR);

  
  theta_error = theta_now(goals[goal_idx], robot);
  
  Serial.print("X: ");
  Serial.println(robot.x/SC);
  Serial.print("Y: ");
  Serial.println(robot.y/SC);

  Serial.print("DR: ");
  Serial.println(DR);
  Serial.print("DL: ");
  Serial.println(DL);
  
  Serial.print("theta robot: ");
  Serial.println(robot.theta*180/PI);
  Serial.print("theta error: ");
  Serial.println(theta_error*180/PI);

  
  w = PID_action(theta_error);
  v = 0.5;
  VL = (v - w * L / 2) / (R * Cl);
  VR = (v + w * L / 2) / (R * Cr);
  
  if (VL > MAX_SAT_L) VL = MAX_SAT_L;
  else if (VL < -MAX_SAT_L) VL = -MAX_SAT_L;
  
  if (VR > MAX_SAT_R) VR = MAX_SAT_R;
  else if (VR < -MAX_SAT_R) VR = -MAX_SAT_R;
  
  applyVoltage(VR, VL);

  delay(Ts);
}





double theta_now(pos goal, Robot robot) {
  return atan2(goal.y - robot.y, goal.x - robot.x) - robot.theta;
}



double PID_action(double error) {
  double u, derr;

  integrator = integrator + error * Ts;
  derr      = error - lastError;
  lastError = error;
  u = Kp * error + Ki * integrator + Kd * derr;

  return u;
}



void applyVoltage(double VR, double VL) {
  if (VR > 0) {
    DirR = 1;//note: since we use an one channel encoder, to know the direction of rotation we store the direction.
    digitalWrite(MR_dir1, LOW);
    digitalWrite(MR_dir2, HIGH);
  } else {
    DirR = -1;//reverse direction used later in encder speed measurement
    digitalWrite(MR_dir1, HIGH);
    digitalWrite(MR_dir2, LOW);
  }

  if (VL > 0) {
    DirL = 1;
    digitalWrite(ML_dir1, HIGH);
    digitalWrite(ML_dir2, LOW);
  } else {
    DirL = -1;
    digitalWrite(ML_dir1, LOW);
    digitalWrite(ML_dir2, HIGH);
  }
  
  analogWrite(MOTORR_PWM, map(abs(VR), 0, MAX_SAT_R, 0, 200));
  analogWrite(MOTORL_PWM, map(abs(VL), 0, MAX_SAT_L, 0, 255));
}



bool checkGoal(pos goal, Robot robot) {
  return ((abs(robot.x - goal.x) <= TOLERANCE) && (abs(robot.y - goal.y) <= TOLERANCE));
}


void EncoderRClicks() {
  if(DirR == 1)
    PulseCountR++;
  else
    PulseCountR--;

}
void EncoderLClicks() {
  if(DirL == 1)
    PulseCountL++;
  else
    PulseCountL--;
}

