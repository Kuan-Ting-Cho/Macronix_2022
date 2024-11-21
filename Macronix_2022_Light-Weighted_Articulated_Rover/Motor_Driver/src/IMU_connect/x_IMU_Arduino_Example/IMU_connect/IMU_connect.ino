/*
    x_IMU_Arduino_Example.ino
    Author: Seb Madgwick

    Example usage of x-IMU C++ library.  Also uses the quaternion library to
    convert the received quaternion to Euler angles.

    Requires two hardware serial modules: one to receive from the x-IMU and one
    to transmit the decoded data to be displayed on computer.

    x-IMU settings:
    Auxiliary Port > Auxiliary Port Mode: "UART"
    Auxiliary Port > UART > Baud Rate: 115200
    Auxiliary Port > UART > Hardware Flow Control: Off

    Hardware connections:
    x-IMU GND -> Arduino MEGA GND
    x-IMU EXT -> Arduino MEGA 5V
    x-IMU AX2 -> Arduino MEGA RX1

    Tested with "arduino-1.0.3" and "Arduino MEGA".
*/

//------------------------------------------------------------------------------
// Includes

#include "Quaternion.h"
#include "XimuReceiver.h"

//------------------------------------------------------------------------------
// Variables

XimuReceiver ximuReceiver1;
XimuReceiver ximuReceiver2;

//------------------------------------------------------------------------------
// Functions

void setup() {
    Serial.begin(115200);   // for sending data to computer
    Serial1.begin(115200);  // for receiving data from x-IMU
    Serial2.begin(115200);  // for receiving data from x-IMU
}

void loop() {
    ErrorCode e = ERR_NO_ERROR;
      delay(100);
    // Process recieved data
    while(Serial1.available() > 0) {
        e = ximuReceiver1.processNewChar(Serial1.read());
    }
    while(Serial2.available() > 0) {
        e = ximuReceiver2.processNewChar(Serial2.read());
    }

    // Print error code (receive error)
    /*if(e != ERR_NO_ERROR) {
        Serial.print("ERROR: ");
        Serial.print(e);
        Serial.println("\r");
    }*/

    // Print battery and thermometer data
    /*if(ximuReceiver.isBattAndThermGetReady()) {
        BattAndThermStruct battAndThermStruct = ximuReceiver.getBattAndTherm();
        Serial.print("battery = ");
        Serial.print(battAndThermStruct.battery);
        Serial.print(", thermometer = ");
        Serial.print(battAndThermStruct.thermometer);
        Serial.println("\r");
    }

    // Print sensor data
    if(ximuReceiver.isInertialAndMagGetReady()) {
        InertialAndMagStruct inertialAndMagStruct = ximuReceiver.getInertialAndMag();
        Serial.print("gyrX = ");
        Serial.print(inertialAndMagStruct.gyrX);
        Serial.print(", gyrY = ");
        Serial.print(inertialAndMagStruct.gyrY);
        Serial.print(", gyrZ = ");
        Serial.print(inertialAndMagStruct.gyrZ);
        Serial.print(", accX = ");
        Serial.print(inertialAndMagStruct.accX);
        Serial.print(", accY = ");
        Serial.print(inertialAndMagStruct.accY);
        Serial.print(", accZ = ");
        Serial.print(inertialAndMagStruct.accZ);
        Serial.print(", magX = ");
        Serial.print(inertialAndMagStruct.magX);
        Serial.print(", magY = ");
        Serial.print(inertialAndMagStruct.magY);
        Serial.print(", magZ = ");
        Serial.print(inertialAndMagStruct.magZ);
        Serial.println("\r");
    }*/

    // Print quaternion data as Euler angles
    if(ximuReceiver1.isQuaternionGetReady()||ximuReceiver2.isQuaternionGetReady()) {
        //Serial1
        QuaternionStruct quaternionStruct1 = ximuReceiver1.getQuaternion();
        Quaternion quaternion1 = Quaternion(quaternionStruct1.w, quaternionStruct1.x, quaternionStruct1.y, quaternionStruct1.z);
        EulerAnglesStruct eulerAnglesStruct1 = quaternion1.getEulerAngles();
        //Serial2
        QuaternionStruct quaternionStruct2 = ximuReceiver2.getQuaternion();
        Quaternion quaternion2 = Quaternion(quaternionStruct2.w, quaternionStruct2.x, quaternionStruct2.y, quaternionStruct2.z);
        EulerAnglesStruct eulerAnglesStruct2 = quaternion2.getEulerAngles();
        Serial.print(" ");
        Serial.print(abs(eulerAnglesStruct1.roll-0.5)-180);
        Serial.print(" ");
        Serial.print(eulerAnglesStruct1.pitch);
        Serial.print(" ");
        Serial.print(eulerAnglesStruct1.yaw);
        Serial.print(" ");
        Serial.print(eulerAnglesStruct2.roll-20.3);
        Serial.print(" ");
        Serial.print(eulerAnglesStruct2.pitch);
        Serial.print(" ");
        Serial.println(eulerAnglesStruct2.yaw);
        
    }
}
