#include <SCServo.h>

SMS_STS st;

#define S_RXD 18  // RX pin for servo communication
#define S_TXD 19  // TX pin for servo communication

// Servo IDs
#define JOINT1_ID 1
#define JOINT2_ID 2
#define JOINT3_ID 3
#define JOINT4_ID 4

// Start positions
#define JOINT1_START 2048
#define JOINT2_START 2048
#define JOINT3_START 2048
#define JOINT4_START 2048

// Servo settings
#define DEFAULT_SPEED 4000
#define DEFAULT_ACC 4000

// Joint arrays
byte jointIDs[4] = {JOINT1_ID, JOINT2_ID, JOINT3_ID, JOINT4_ID};
s16 jointPositions[4] = {JOINT1_START, JOINT2_START, JOINT3_START, JOINT4_START};
u16 jointSpeeds[4] = {DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED, DEFAULT_SPEED};
byte jointAcc[4] = {DEFAULT_ACC, DEFAULT_ACC, DEFAULT_ACC, DEFAULT_ACC};

uint8_t idx = 0;
uint8_t val_idx = 0; 
char value[5] = "0000";

int current_position[4] = {0,0,0,0};


void setup() {
    Serial.begin(115200);
    Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
    st.pSerial = &Serial1;
    delay(1000);
}

void reach_goal(int jointIndex, int degrees) {
    int degressToMove = 0;
    int degreesToMove = degrees - current_position[jointIndex]; 
    int goal = map(degreesToMove, -360, 360, -4095 * 3, 4095 * 3); // Konwersja z zakresu [0,360] na [0,4095]
    goal = constrain(goal, -4095 * 3, 4095 * 3); // Zapewnienie, że wartość nie wyjdzie poza zakres
    st.WritePosEx(jointIDs[jointIndex], goal, jointSpeeds[jointIndex], jointAcc[jointIndex]);
    current_position[jointIndex] = degrees;

}

void loop() {
    if (Serial.available()) {
        char chr = Serial.read();

        if (chr == 'a') {
            idx = 0;
            val_idx = 0;
        } else if (chr == 'b') {
            idx = 1;
            val_idx = 0;
        } else if (chr == 'c') {
            idx = 2;
            val_idx = 0;
        } else if (chr == 'd') {
            idx = 3;
            val_idx = 0;
        } else if (chr == ',') 
        {
            int val = atoi(value);
            reach_goal(idx, val);

            value[0] = '0';
            value[1] = '0';
            value[2] = '0';
            value[3] = '\0';


        } else {
            value[val_idx++] = chr;
            if (val_idx >= 4) val_idx = 3; // Prevent buffer overflow
        }
    }
}
