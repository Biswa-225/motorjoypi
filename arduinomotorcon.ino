// === Motor Pins ===
#define IN1 7     // Left motor forward
#define IN2 6     // Left motor reverse
#define IN3 5     // Right motor forward
#define IN4 4     // Right motor reverse

// === Encoder Pins ===
#define LEFT_C1 2    // Interrupt pin
#define LEFT_C2 4
#define RIGHT_C1 3   // Interrupt pin
#define RIGHT_C2 5

volatile long left_count = 0;
volatile long right_count = 0;

// Encoder ISR handlers
void leftEncoderISR() {
  int b = digitalRead(LEFT_C2);
  if (b == HIGH)
    left_count++;
  else
    left_count--;
}

void rightEncoderISR() {
  int b = digitalRead(RIGHT_C2);
  if (b == HIGH)
    right_count++;
  else
    right_count--;
}

void setup() {
  Serial.begin(9600);

  // Motor pin setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Encoder pin setup
  pinMode(LEFT_C1, INPUT_PULLUP);
  pinMode(LEFT_C2, INPUT_PULLUP);
  pinMode(RIGHT_C1, INPUT_PULLUP);
  pinMode(RIGHT_C2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_C1), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_C1), rightEncoderISR, CHANGE);

  Serial.println("✅ Ready for joystick input");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "F") {
      moveForward();
    } else if (cmd == "B") {
      moveBackward();
    } else if (cmd == "L") {
      turnLeft();
    } else if (cmd == "R") {
      turnRight();
    } else if (cmd == "S") {
      stopMotors();
    }

    // Report encoder counts
    Serial.print("L: ");
    Serial.print(left_count);
    Serial.print(" | R: ");
    Serial.println(right_count);
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
