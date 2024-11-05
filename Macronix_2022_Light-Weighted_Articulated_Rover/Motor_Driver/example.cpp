#include "./MotorUnion/MotorUnion.h"
#include "./XIMU/XIMU.h"
#include "./realsense/rsdis.h"
#include <unistd.h> /* UNIX standard function definitions */
#include <stdio.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{

    Realsense realsense; // realsense
    XIMU x_imu;
    MotorUnion motor_driver({0, 2, 4, 6, 8}, {"Mx28", "Mx28", "Mx28", "Mx28", "Mx106"}); // Mx106 Vmax 210  //Mx28  Vmax 230                                                                   //initialize
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p;
    int speed = 75;
    int center_speed = 150;
    int *Command;
    int dis = 0;
    // x_imu.update_pose_strategy();
    // x_imu.show_pose_strategy();
    while (1)
    {
        dis = realsense.get_dis(p);
        Command = x_imu.command(); // Command[5] = {0, 0, 0, 0, 0}; //{stop, pose, cur_strategy, pre_strategy, center_angle}
        // cout << Command[0] << endl;
        // cout << Command[1] << endl;
        // cout << Command[2] << endl;
        // cout << Command[3] << endl;
        // cout << Command[4] << endl;

        if (Command[0] == 1) // stop
        {
            motor_driver.Stop();
            cout << "stop." << endl;
        }
        // else if (Command[2] == on_floor)
        // {
        //     motor_driver.GoStraight(speed);
        // }
        else if (Command[3] == Command[2] && Command[2] != adjustment_open && Command[2] != adjustment_close)
        {
            motor_driver.GoBack(speed);
            cout << "go straight." << endl;
        }
        else if (Command[2] == climbing)
        {
            motor_driver.GoBack(speed);
            cout << "go straight." << endl;
        }
        else if (Command[2] == attached)
        {
            motor_driver.GoBack(speed);
            sleep(1);
            cout << "go straight." << endl;
        }
        else if (Command[2] == adjustment_open)
        {
            motor_driver.Change_angle(1, center_speed); // 1:open 2:close
            cout << "change angle open." << endl;
            sleep(5);
            cout << "stop." << endl;
            sleep(3);
        }
        else if (Command[2] == adjustment_close)
        {
            motor_driver.Change_angle(2, center_speed); // 1:open 2:close
            cout << "change angle close." << endl;
            sleep(5);
            cout << "stop." << endl;
            sleep(3);
        }
        else if (Command[2] == on_top)
        {
            if (Command[4] < 100) // if center_angle < 100 degrees,adjustment_open
            {
                motor_driver.Change_angle(1, center_speed);
                cout << "change angle open." << endl;
                // sleep(5);
                motor_driver.Stop();
                cout << "stop." << endl;
                // sleep(3);
            }
            else // if center_angle > 100 degrees ,decending
            {
                motor_driver.GoBack(speed);
                cout << "go straight." << endl;
            }
        }
        else if (Command[2] == decending)
        {
            motor_driver.GoBack(speed);
            cout << "go straight." << endl;
        }
        // else if (Command[2] == unknown) //Have been defined in x_imu.command()
        // {
        // }
    }
    delete[] Command;
}
