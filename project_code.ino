#include <Bluepad32.h>

#define leftMotorPin1 14
#define leftMotorPin2 12

#define rightMotorPin1 32
#define rightMotorPin2 33

#define leftMotorDriveEnable 25
#define rightMotorDriveEnable 26

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void setup()
{
    Serial.begin(115200);

    BP32.setup(&onConnectedController, &onDisconnectedController);

    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);

    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);

    pinMode(leftMotorDriveEnable, OUTPUT);
    pinMode(rightMotorDriveEnable, OUTPUT);

    stopMotors();
}

void loop()
{
    BP32.update();

    if (myControllers[0] != nullptr)
    {
        processGamepad(myControllers[0]);
    }
    else
    {
        stopMotors();
    }
}

void processGamepad(ControllerPtr controller)
{
    
    int x = controller->axisX();
    int y = -controller->axisY();

    
    const int deadZone = 150;

    if (abs(x) < deadZone)
        x = 0;

    if (abs(y) < deadZone)
        y = 0;

    
    if (x == 0 && y == 0)
    {
        stopMotors();
        return;
    }

    
    int leftMotor = y + x;
    int rightMotor = y - x;

    
    leftMotor = constrain(leftMotor, -1023, 1023);
    rightMotor = constrain(rightMotor, -1023, 1023);

    
    if (leftMotor > 0)
    {
        digitalWrite(leftMotorPin1, LOW);
        digitalWrite(leftMotorPin2, HIGH);
    }
    else if (leftMotor < 0)
    {
        digitalWrite(leftMotorPin1, HIGH);
        digitalWrite(leftMotorPin2, LOW);
    }
    else
    {
        digitalWrite(leftMotorPin1, LOW);
        digitalWrite(leftMotorPin2, LOW);
    }

   
    if (rightMotor > 0)
    {
        digitalWrite(rightMotorPin1, LOW);
        digitalWrite(rightMotorPin2, HIGH);
    }
    else if (rightMotor < 0)
    {
        digitalWrite(rightMotorPin1, HIGH);
        digitalWrite(rightMotorPin2, LOW);
    }
    else
    {
        digitalWrite(rightMotorPin1, LOW);
        digitalWrite(rightMotorPin2, LOW);
    }

    
    int leftSpeed = map(abs(leftMotor), 0, 512, 0, 255);
    int rightSpeed = map(abs(rightMotor), 0, 512, 0, 255);

    analogWrite(leftMotorDriveEnable, leftSpeed);
    analogWrite(rightMotorDriveEnable, rightSpeed);

    
    Serial.print("X: ");
    Serial.print(x);

    Serial.print(" Y: ");
    Serial.print(y);

    Serial.print(" Left: ");
    Serial.print(leftMotor);

    Serial.print(" Right: ");
    Serial.println(rightMotor);
}

void stopMotors()
{
    analogWrite(leftMotorDriveEnable, 0);
    analogWrite(rightMotorDriveEnable, 0);

    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);

    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
}

void onConnectedController(ControllerPtr ctl)
{
    bool foundEmptySlot = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (myControllers[i] == nullptr)
        {
            Serial.printf(
                "CALLBACK: Controller connected, index=%d\n",
                i
            );

            ControllerProperties properties = ctl->getProperties();

            Serial.printf(
                "Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                ctl->getModelName().c_str(),
                properties.vendor_id,
                properties.product_id
            );

            myControllers[i] = ctl;
            foundEmptySlot = true;

            break;
        }
    }

    if (!foundEmptySlot)
    {
        Serial.println(
            "CALLBACK: Controller connected, but no empty slot"
        );
    }
}

void onDisconnectedController(ControllerPtr ctl)
{
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (myControllers[i] == ctl)
        {
            Serial.printf(
                "CALLBACK: Controller disconnected from index=%d\n",
                i
            );

            myControllers[i] = nullptr;
            foundController = true;

            stopMotors();

            break;
        }
    }

    if (!foundController)
    {
        Serial.println(
            "CALLBACK: controller disconnected, but not found"
        );
    }
}