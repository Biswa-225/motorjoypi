#define IN1 5
#define IN2 4
#define ENA 6

#define IN3 8
#define IN4 7
#define ENB 9

#define ENC_LEFT 2
#define ENC_RIGHT 3

volatile long left_count = 0;
volatile long right_count = 0;

int pwm_left = 0;
int pwm_right = 0;

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Encoder pins
  pinMode(ENC_LEFT, INPUT_PULLUP);
  pinMode(ENC_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_RIGHT), countRight, RISING);

  Serial.println("✅ Arduino ready");
}

void loop() {
  static unsigned long last_time = 0;
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    parseJoystick(input);
  }

  // Calculate encoder counts every 100 ms
  if (millis() - last_time >= 100) {
    long l = left_count;
    long r = right_count;
    left_count = 0;
    right_count = 0;

    Serial.print("ENC_L: "); Serial.print(l);
    Serial.print(" | ENC_R: "); Serial.println(r);
    last_time = millis();
  }
}

void countLeft() {
  left_count++;
}

void countRight() {
  right_count++;
}

void parseJoystick(String data) {
  int lxIndex = data.indexOf("LX:");
  int lyIndex = data.indexOf("LY:");

  if (lxIndex != -1 && lyIndex != -1) {
    int lx = data.substring(lxIndex + 3, data.indexOf(",", lxIndex)).toInt();
    int ly = data.substring(lyIndex + 3).toInt();

    driveWithJoystick(lx, ly);
  }
}

void driveWithJoystick(int lx, int ly) {
  int base_speed = map(abs(ly), 0, 100, 0, 255);
  int turn = map(lx, -100, 100, -100, 100);

  int left = constrain(ly + turn, -100, 100);
  int right = constrain(ly - turn, -100, 100);

  setMotor(left, right);
}

void setMotor(int left_percent, int right_percent) {
  setSingleMotor(IN1, IN2, ENA, left_percent);
  setSingleMotor(IN3, IN4, ENB, right_percent);
}

void setSingleMotor(int in1, int in2, int en, int speed_percent) {
  int pwm = map(abs(speed_percent), 0, 100, 0, 255);

  if (speed_percent > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (speed_percent < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  analogWrite(en, pwm);
}
