
// SELF BALANCING ARDUINO ROBOT WITH STEPPER MOTORS CONTROLLED BASED ON BROBOT EVO 2

// Date: 15/11/2018
// Updated: 02/02/2019
// License: GPL v2

// Original Project URL: http://jjrobots.com/b-robot-evo-2-much-more-than-a-self-balancing-robot (Features,documentation,build instructions,how it works, SHOP,...)
// Modified version: https://github.com/fredvaz/B-ROBOT_EVO2
// Simulink: https://github.com/fredvaz/self_balancing_robot/tree/cd_version

// The board needs at least 10-15 seconds with no motion (robot steady) at beginning to give good values... Robot move slightly when it´s ready!
// MPU6050 IMU connected via I2C bus. Angle estimation using complementary filter (fusion between gyro and accel)
// Angle calculations and control part is running at 100Hz

// The robot is OFF when the angle is high (robot is horizontal). When you start raising the robot it
// automatically switch ON and start a RAISE UP procedure.
// To switch OFF the robot you could manually put the robot down on the floor (horizontal)

// We have a PI controller for speed control and a PD controller for stability (robot angle)
// The output of the control (motors speed) is integrated so it´s really an acceleration not an speed.

// INITIALIZATION
void setup()
{
    // STEPPER PINS ON JJROBOTS BROBOT BRAIN BOARD
    pinMode(4, OUTPUT);    // ENABLE MOTORS
    pinMode(7, OUTPUT);    // STEP MOTOR 1 PORTE,6
    pinMode(8, OUTPUT);    // DIR MOTOR 1  PORTB,4
    pinMode(11, OUTPUT);   // STEP MOTOR 2 PORTB,7
    pinMode(12, OUTPUT);   // DIR MOTOR 2  PORTD,6
    digitalWrite(4, HIGH); // Disbale motors
    pinMode(10, OUTPUT);   // Servo1 (arm)
    pinMode(13, OUTPUT);   // Servo2

    Serial.begin(115200); // Serial output to console
    Serial1.begin(115200);

    // Initialize I2C bus (MPU6050 is connected via I2C)
    Wire.begin();

    //

    // Serial.println("JJROBOTS");
    delay(200);
    // Serial.println("Don't move for 10 sec...");
    MPU6050_setup(); // setup MPU6050 IMU
    delay(500);

    // Calibrate gyros
    MPU6050_calibrate();
}

// MAIN LOOP
void loop()
{

    // New IMU data? CONTROL LOOP
    if (MPU6050_newData())
    {
        MPU6050_read_3axis();
        loop_counter++;
        slow_loop_counter++;
        dt = (timer_value - timer_old) * 0.000001; // dt in seconds
        // dt = (timer_value - timer_old) * 0.001; // dt in seconds - ADDED
        timer_old = timer_value;

        angle_adjusted_Old = angle_adjusted;
        // Get new orientation angle from IMU (MPU6050)
        float MPU_sensor_angle = MPU6050_getAngle(dt);
        angle_adjusted = MPU_sensor_angle + angle_offset;
        if ((MPU_sensor_angle > -15) && (MPU_sensor_angle < 15))
            angle_adjusted_filtered = angle_adjusted_filtered * 0.99 + MPU_sensor_angle * 0.01;

    } // End of new IMU data
}