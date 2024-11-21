#include "./MotorUnion/MotorUnion.h"
#include "./XIMU/XIMU.h"
#include "./rssave/rssave.h"
#include "./rsdis/rsdis.h"
#include <unistd.h> /* UNIX standard function definitions */
#include <stdio.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    /////////*Automatic parameter*////////////
    XIMU x_imu;
    rssave photo;
    rsdis distance;
    /*Mx106 Vmax 210  Mx28  Vmax 230*/
    MotorUnion motor_driver({0, 2, 4, 6, 8}, {"Mx28", "Mx28", "Mx28", "Mx28", "Mx106"});

    int speed = 50;
    int b_speed = 75;
    int center_speed = 150;
    int *Command;
    float dis = 0;
    rs2::pipeline p; // used for realsense
    p.start();

    //////////*Manual parameter*//////////
    int Pre_Line = 0; // initialize
    int Cur_Line = 0; // initialize
    int Stop_Key = 0; // initialize
    int *Manual_Cmd;
    string Line = "Line";                 // initialize
    string Last_Line = "No any command."; // initialize
    ifstream test;

    while (1)
    {
        test.open("/home/potter/2022WonHon/Motor_Driver/src/test.txt");
        while (Pre_Line == Cur_Line)
        {
            while (test.peek() != EOF)
            {
                getline(test, Last_Line); // find Last_Line
                Cur_Line++;
            }
            if (Cur_Line == 0) // if no command in test.txt
            {
                Cur_Line++;
            }
        }
        test.close();
        Manual_Cmd = motor_driver.ConvertToASCII(Last_Line);
        if (Manual_Cmd[0] == 65) //'A' ASCII  (Automatic)
        {
            // change mode  edit
            string filename = "/home/potter/2022WonHon/Motor_Driver/src/record.txt";
            ofstream imu_file;
            imu_file.open(filename, ios::out | ios::app);
            imu_file << "**********Change mode**********\n";
            imu_file.close();

            Stop_Key = 0;
            Pre_Line = Cur_Line = 0; // reset to initial
            while (1)                // Last_Line == 'A'
            {
                dis = distance.get_dis(p1);
                cout << "Distance:: " << dis << endl;

                Line = Last_Line; // record Line = Last_Line
                test.open("/home/potter/2022WonHon/Motor_Driver/src/test.txt");
                while (Pre_Line == Cur_Line)
                {
                    while (test.peek() != EOF)
                    {
                        getline(test, Last_Line); // find Last_Line
                        Cur_Line++;
                    }
                    if (Cur_Line == 0) // if no command in test.txt
                    {
                        Cur_Line++;
                    }
                }
                test.close();

                if (Line != Last_Line)
                {
                    cout << "Manual_mode." << endl;
                    break;
                }

                /*Command[5] = {0, 0, 0, 0, 0}; {stop, pose, cur_strategy, pre_strategy, center_angle}*/
                Command = x_imu.command();
                x_imu.Record();

                if (dis <= 0.3)
                {
                    motor_driver.TurnRight_withtime(50, 4);
                }

                else
                {
                    /*Stop condition*/
                    if (Command[0] == 1) // stop
                    {
                        motor_driver.Stop();
                        cout << "stop." << endl;
                    }
                    /*Strategy implement*/
                    if (Command[2] == adj_initial_pose)
                    {
                        if (Command[4] < 80)
                        {

                            motor_driver.Change_angle(49, center_speed); // 1:open 2:close
                            cout << "change angle open." << endl;
                            sleep(0.3);
                        }
                        else if (Command[4] > 95)
                        {
                            motor_driver.Change_angle(50, center_speed); // 1:open 2:close
                            cout << "change angle close." << endl;
                            sleep(0.3);
                        }
                        else
                        {
                            motor_driver.Stop();
                            x_imu.set_strategy(on_floor);
                            x_imu.initial_ok = 1;
                        }
                    }
                    else if (Command[2] == t_left)
                    {
                        motor_driver.TurnLeft(25);
                    }
                    else if (Command[2] == t_right)
                    {
                        motor_driver.TurnRight(25);
                    }
                    else if (Command[2] == keep_climbing)
                    {

                        if (Command[3] != keep_climbing)
                        {
                            motor_driver.Stop();
                        }

                        motor_driver.GoStraight(speed);
                        cout << "keep climbing." << endl;
                    }
                    else if (Command[2] == keep_climbing2)
                    {
                        motor_driver.Individual(25, -50);
                        motor_driver.Change_cen_angle(2, center_speed); // 1:open 2:close
                        cout << "keep climbing2." << endl;
                    }
                    else if (Command[2] == climbing)
                    {
                        motor_driver.GoStraight(speed);
                        cout << "go climbing." << endl;
                    }
                    else if (Command[2] == attached)
                    {
                        motor_driver.GoStraight(speed);
                        cout << "go attached." << endl;
                        sleep(1);
                    }
                    else if (Command[2] == adjustment_open)
                    {
                        motor_driver.Change_angle(49, center_speed);
                        cout << "change angle open." << endl;
                    }
                    else if (Command[2] == adjustment_close)
                    {
                        motor_driver.Change_angle(50, center_speed);
                        cout << "change angle close." << endl;
                    }
                    else if (Command[2] == on_top)
                    {
                        // motor_driver.Stop();
                        if (Command[4] < 80)
                        {
                            motor_driver.Change_angle(49, center_speed);
                            cout << "change angle open." << endl;
                            sleep(0.3);
                        }
                        else if (Command[4] > 95)
                        {
                            motor_driver.Change_angle(50, center_speed);
                            cout << "change angle close." << endl;
                            sleep(0.3);
                        }
                        else
                        {
                            motor_driver.Stop();
                            x_imu.set_strategy(on_floor_again);
                            x_imu.decend = 0;
                        }
                    }
                    else if (Command[2] == decending)
                    {
                        motor_driver.GoStraight(speed);
                        cout << "go decending." << endl;
                    }
                    else if (Command[2] == on_floor_again)
                    {
                        motor_driver.GoStraight(speed);
                        cout << "on floor again." << endl;
                    }
                    else if (Command[3] == Command[2])
                    {
                        motor_driver.GoStraight(speed);
                        cout << "go straight." << endl;
                    }

                    Pre_Line = Cur_Line = 0; // reset to initial
                    photo.save_photo(p);
                }
            }
        }
        else if (Manual_Cmd[0] == 112 && Stop_Key == 0) // Press 'p' && Have yet to stop
        {
            motor_driver.Stop();
            Stop_Key = 1; // Car is stop
        }
        else if (Last_Line != Line)
        {
            cout << "-------------------------------------" << endl;
            Stop_Key = 0; // Car will move
            int speed_mode = Manual_Cmd[0];
            int speed;
            int Angle;
            int key;
            int time;

            switch (speed_mode)
            {
            case 99: // 'c' ASCII
                speed = 210;
                Angle = Manual_Cmd[1];
                motor_driver.Change_angle(Angle, speed);
                key = 0;
                break;
            case 49: // '1' ASCII
                speed = 77;
                key = Manual_Cmd[1];
                break;
            case 50: // '2' ASCII
                speed = 150;
                key = Manual_Cmd[1];
                break;
            case 51: // '3' ASCII
                speed = 210;
                key = Manual_Cmd[1];
                break;
            default:
                speed = 0;
                key = 0;
                cout << "No this mode." << endl;
                break;
            }

            switch (key)
            {
            case 0:
                break;
            case 100: // 'd' ASCII
                motor_driver.TurnRight(speed);
                break;
            case 97: // 'a' ASCII
                motor_driver.TurnLeft(speed);
                break;
            case 119: // 'w' ASCII
                motor_driver.GoStraight(speed);
                break;
            case 115: // 's' ASCII
                motor_driver.GoBack(speed);
                break;
            default:
                cout << "No this direction." << endl;
                break;
            }
        }
        Line = Last_Line;        // record Line = Last_Line  /***
        Pre_Line = Cur_Line = 0; // Line = Last_Line;reset to initial         /***

        photo.save_photo(p);
    }

    delete[] Manual_Cmd;
    delete[] Command;
}
