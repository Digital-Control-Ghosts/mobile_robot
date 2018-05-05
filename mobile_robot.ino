// CONTROL GHOSTS ..

struct pos{
  int x, y;
  pos(int a, int b){
    x = a;
    y = b;
  }
};

struct Robot{
  double x, y, theta;
  Robot(){
    x = y = theta = 0;
  }
};


//define pins
#define MOTOR1_PWM 0
#define MOTOR2_PWM 0
#define M1_dir1 0
#define M1_dir2 0
#define M2_dir1 0
#define M2_dir2 0

#define MAX_SAT 5

#define Ts 50

#define R 0.02
#define L 0.15
#define Cr 1
#define Cl 1
#define PULSES_PER_REV 12

#define TOLERANCE 0.5

#define Kp 1
#define Ki 0
#define Kd 0
double integrator, lastError;
double PulseCountR, PulseCountL;

#define N_GOALS 5
pos goals[N_GOALS] = { pos(1,1), pos(1,2), pos(2,2), pos(3,2), pos(3,3) };
int goal_idx;

Robot robot;
void setup() {
  Serial.begin(9600);
}

bool checkGoal(pos goal, Robot robot);
double theta_now(pos goal, Robot robot);
double PID_action(double error);
void applyVoltage(double VR, double VL);

void loop() {
  double theta_error = PI/2 - theta_now(goals[goal_idx], robot);
  double w = PID_action(theta_error);
  double v = 0.1;
  double VL = (v-w*L/2)/(R*Cl);
  double VR = (v+w*L/2)/(R*Cr);
  applyVoltage(VR, VL);

  double DR = 2*PI*R*PulseCountR/PULSES_PER_REV;
  double DL = 2*PI*R*PulseCountL/PULSES_PER_REV;
  double DC = (DR + DL)/2;
  PulseCountR = PulseCountL = 0;

  robot.x = robot.x + DC * cos(robot.theta);
  robot.y = robot.y + DC * sin(robot.theta);
  robot.theta = robot.theta + (DR-DL)/L;

  goal_idx += checkGoal(goals[goal_idx], robot);

  if(goal_idx == N_GOALS){
    return ;
  }


  delay(Ts);
}


// Functions Implementations

double theta_now(pos goal, Robot robot){
    double theta;
  
    return theta;
}

double PID_action(double error){
  double u, derr;

  integrator += error;
  derr      = error - lastError;
  lastError = error;
  u = Kp * error + Ki * integrator + Kd * derr;

  return u;
}

void applyVoltage(double VR, double VL){
  analogWrite(MOTOR1_PWM, map(abs(VR), 0, MAX_SAT, 0, 255));
  analogWrite(MOTOR2_PWM, map(abs(VL), 0, MAX_SAT, 0, 255));
  if(VR>0){
    digitalWrite(M1_dir1,HIGH);
    digitalWrite(M1_dir2,LOW);
  }else{
    digitalWrite(M1_dir1,LOW);
    digitalWrite(M1_dir2,HIGH);
  }

  if(VL>0){
    digitalWrite(M2_dir1,HIGH);
    digitalWrite(M2_dir2,LOW);
  }else{
    digitalWrite(M2_dir1,LOW);
    digitalWrite(M2_dir2,HIGH);
  }
}

bool checkGoal(pos goal, Robot robot){
    return ((abs(robot.x - goal.x) <= TOLERANCE) && (abs(robot.y - goal.y) <= TOLERANCE));
}

