
// Include the library files

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <Servo.h>

Servo myservo;

// Define the motor pins
#define TrigPin 12
#define EchoPin 14

#define MAX_FORWARD_MOTOR_SPEED 75
#define MAX_MOTOR_TURN_SPEED_ADJUSTMENT 50

#define MIN_DISTANCE 10
#define MAX_DISTANCE 30

#define servoPin 2

#define ENA 255
#define IN1 1
#define IN2 3
#define IN3 15
#define IN4 13
#define ENB 255

#define pir 16

#define IRR 4
#define IRL 0

#define ssid "#*#*#*#*"
#define pass "#*#*#*#*#*"

//#define S1 4
//#define S2 0
#define BOT_Token "-*-*-*-*-*-*"
#define CHAT_ID "-*-*-*-*-*"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_Token, client);


// Variables for the Blynk widget values
int x = 50;
int y = 50;
int Speed = 0;
int obstacle = 0;
int lineFollow = 0;
int alert = 0;
int follow = 0;
int remote = 0;

#define BLYNK_TEMPLATE_ID "TMPLr_Zfej39"
#define BLYNK_TEMPLATE_NAME "Quanta"
#define BLYNK_AUTH_TOKEN "zU9Y42s52ZVSppKWh6TJfos-9t2whaCK"

void setup() {

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //  Serial.printf("\r\n[Wifi]: Connecting");
  //  WiFi.begin(WIFI_SSID, WIFI_PASS);
  //
  //  while (WiFi.status() != WL_CONNECTED) {
  //    Serial.printf(".");
  //    delay(250);
  //  }
  //
  //  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
  //  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  //    Serial.println(WiFi.localIP());
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  myservo.attach(servoPin);
  myservo.write(90);
  //  delay(2000);

  pinMode(IRR, INPUT);
  pinMode(IRL, INPUT);
//  pinMode(S1, INPUT);
//  pinMode(S2, INPUT);

  pinMode(pir, INPUT);


  //  bot.begin();
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert);

  bot.sendMessage(CHAT_ID, "System started", "");

}
BLYNK_WRITE(V0) {
  obstacle = param.asInt();

}
BLYNK_WRITE(V1) {
  lineFollow = param.asInt();

}
BLYNK_WRITE(V5) {
  alert = param.asInt();

}
BLYNK_WRITE(V6) {
  follow = param.asInt();

}
BLYNK_WRITE(V7) {
  remote = param.asInt();

}
// Get the joystick values
BLYNK_WRITE(V3) {
  x = param[0].asInt();

}
BLYNK_WRITE(V4) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V2) {
  Speed = param.asInt();
}

void loop() {

  Blynk.run();

  long duration, distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("CM=");
  Serial.println(distance);
  if (follow == 1) {
    Serial.println("Follow");
    int Right_Value = digitalRead(IRR);
    int Left_Value = digitalRead(IRL);

    if ((Right_Value == 0) && (Left_Value == 0)) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if ((Right_Value == 0) && (Left_Value == 1)) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

    } else if ((Right_Value == 1) && (Left_Value == 0)) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    else if ((Right_Value == 1) && (Left_Value == 1)) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }

    //    else if ((distance > 1 && distance < 5) && (Right_Value == 1) && (Left_Value == 1)) {
    //      digitalWrite(IN1, LOW);
    //      digitalWrite(IN2, HIGH);
    //      digitalWrite(IN3, LOW);
    //      digitalWrite(IN4, HIGH);
    //
    //    }
  }
  else if (remote == 1) {
    smartCar();
  }
  else if (alert == 1) {
    Serial.println("Alert");
    carStop();
    int value = digitalRead(pir);
    if (value == 1) {
      Serial.println("Motion Detected");

      bot.sendMessage(CHAT_ID, "Ghost Detected!!!!!", "");
    }
  }
  else if (lineFollow == 1) {
    Serial.print("Line Follower Mode");
//    if (digitalRead(S1) == LOW  && digitalRead(S2) == LOW)
//    {
//      carForward();
//    }
//    if (digitalRead(S1) == HIGH  && digitalRead(S2) == LOW)
//    {
//      carRight();
//    }
//    if (digitalRead(S1) == LOW  && digitalRead(S2) == HIGH)
//    {
//      carLeft();
//    }
//    if (digitalRead(S1) == HIGH  && digitalRead(S2) == HIGH)
//    {
//      carStop();
//    }
  }
  else if (obstacle == 1) {
    Serial.println("Obstacle");
    long ld, rd;
    Serial.println(distance);
    if (distance <= 15)
    {
      carStop();
      delay(100);
      carBackward();
      delay(1000);
      carStop();
      delay(200);
      myservo.write(0);

      delay(1000);
      rd = dist();
      myservo.write(90);

      delay(200);
      myservo.write(180);
      delay(1000);
      ld = dist();
      myservo.write(90);

      delay(200);
      carStop();
      delay(200);
      if (ld >= rd) {
        carRight();
        delay(850);
        carStop();
        delay(200);
      }
      else {
        carLeft();
        delay(850);
        carStop;
        delay(200);
      }

    }
    else
    {
      carForward();
    }

  }
  else {
    carStop();
  }



}

/**************Motor movement functions*****************/
void smartCar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}

long dist() {
  long duration, distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}

void lookRight()
{
  myservo.write(0);
  delay(1000);
  myservo.write(90);
}

void lookLeft()
{
  myservo.write(180);
  delay(1000);
  myservo.write(90);
}

void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  analogWrite(ENA, abs(rightMotorSpeed));
  analogWrite(ENB, abs(leftMotorSpeed));
}
