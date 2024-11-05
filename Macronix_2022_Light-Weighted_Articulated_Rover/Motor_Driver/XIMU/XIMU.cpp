#include "XIMU.h"
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
////////////////////////////////////////
XIMU::XIMU()
{
    set_pose(normal_med);
    set_strategy(on_floor);
}
XIMU::~XIMU()
{
}

////////////////////////////////////////

void XIMU::set_pose(Pose a)
{
    cur_pose = a;
}

Pose XIMU::get_pose(void)
{
    return cur_pose;
}

void XIMU::set_strategy(Strategy a)
{
    pre_strategy = cur_strategy;
    cur_strategy = a;
}

Strategy XIMU::get_strategy(void)
{
    return cur_strategy;
}

float XIMU::get_car_angle(void)
{
    return cen_angle;
}

void XIMU::update_pose_strategy(void)
{
    string filename = "/home/potter/2022WonHon/Motor_Driver/src/imu_data.txt";
    float data[6][6]; //Store imu data for five times (6 data)
    int Previous_Line, Current_Line;
    Previous_Line = Current_Line = 0;
    string Last_Line; //Current last_Line
    string Line;
    fstream imu_file; //for read file
    stringstream ss;
    //string str[5]; //Store IMU update data
    float temp = 0;
    int i = 0;
    int record = 0; //Previous line

    /*Clear imu_data*/
    // clear_file.open(filename, ios::out | ios::trunc);
    // clear_file.close();

    /*adjust bias*/
    float bias[6][6] =

        {
            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},

            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},

            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},

            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},

            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},

            {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
        };

    /*Read imu data*/
    while (Previous_Line == Current_Line) //Continuous read file
    {
        if (i > 5) //Last_line which was recorded must lower than 5 lines
        {
            i = 0;
            break;
        }
        imu_file.open(filename, ios::in);
        while (Previous_Line == Current_Line)
        {
            while (imu_file.peek() != EOF)
            {
                getline(imu_file, Line);
                if (Line.length() >= 37)
                {
                    Last_Line = Line;
                }
                Current_Line++;
            }
            //str[i] = Last_Line;
            imu_file.close();
            if (record != Current_Line && i < 6) //if current_Line >previous_Line,record current last_line information
            {
                ss << Last_Line; //update imu data information     /*Roll >> Pitch >> Yaw>>Roll >> Pitch >> Yaw*/
                for (int j = 0; j < 6; j++)
                {
                    ss >> temp;
                    data[i][j] = abs(temp);
                }
                ss.clear();
                i++;
            }
        }
        record = Current_Line;            // record current_line as previous_Line
        Previous_Line = Current_Line = 0; //reset to initial
    }
    /*Clear imu_data*/
    imu_file.open(filename, ios::out | ios::trunc);
    imu_file << "\n";
    imu_file.close();
    sleep(0.5);

    /*Add bias*/
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            data[i][j] = data[i][j] + bias[i][j];
        }
    }
    /*Show Last_Line data and length*/ /*str[] should be opened*/
    // cout << str[0] << endl;
    // cout << str[0].length() << endl;
    // cout << str[1] << endl;
    // cout << str[1].length() << endl;
    // cout << str[2] << endl;
    // cout << str[2].length() << endl;
    // cout << str[3] << endl;
    // cout << str[3].length() << endl;
    // cout << str[4] << endl;
    // cout << str[4].length() << endl;

    /*Show IMU all data*/
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[0][0], data[0][1], data[0][2], data[0][3], data[0][4], data[0][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[1][0], data[1][1], data[1][2], data[1][3], data[1][4], data[1][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[2][0], data[2][1], data[2][2], data[2][3], data[2][4], data[2][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[3][0], data[3][1], data[3][2], data[3][3], data[3][4], data[3][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[4][0], data[4][1], data[4][2], data[4][3], data[4][4], data[4][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[5][0], data[5][1], data[5][2], data[5][3], data[5][4], data[5][5]);
    /*Show IMU Roll data*/
    // printf("\n%.2f %.2f \n", data[0][0], data[0][1]);
    // printf("\n%.2f %.2f \n", data[0][0], data[0][1]);
    // printf("\n%.2f %.2f \n", data[1][0], data[1][1]);
    // printf("\n%.2f %.2f \n", data[2][0], data[2][1]);
    // printf("\n%.2f %.2f \n", data[3][0], data[3][1]);
    // printf("\n%.2f %.2f \n", data[4][0], data[4][1]);

    /*Update car_angle*/
    if (get_strategy() != climbing && get_strategy() != decending)
    {
        cen_angle = 180 - data[5][0] - data[5][3];
    }

    /*Update pose and strategy*/
    /*pose update*/
    if (get_strategy() != climbing)
    {
        if (data[5][0] <= 30 && data[5][3] <= 32)
        {
            set_pose(normal_low);
        }
        else if (data[5][0] > 30 && data[5][0] <= 70 && data[5][3] > 32 && data[5][3] <= 72)
        {
            set_pose(normal_med);
        }
        else if (data[5][0] > 70 && data[5][0] <= 85 && data[5][3] > 70 && data[5][3] <= 87)
        {
            set_pose(normal_high);
        }
    }
    else
    {
        //calculate pose//
    }

    /*strategy update*/
    if (cur_strategy != climbing && cur_strategy != attached && (data[5][3] - data[5][0]) < 10)
    {
        set_strategy(on_floor);
    }
    else if (data[5][0] < data[5][3] && data[5][3] < cri_angle) //uncertain logic//
    {
        if (data[5][0] < data[4][0] && data[4][0] < data[3][0] && data[5][0] > 10)
        {
            set_strategy(climbing);
        }
        else if (pre_strategy == unknown)
        {
            set_strategy(climbing);
        }
        else if (data[5][0] < 10) //front_wheel on top
        {
            if (pre_strategy == climbing || pre_strategy == unknown)
            {
                set_strategy(attached);
            }
            else if (pre_strategy == attached || pre_strategy == adjustment_close)
            {
                set_strategy(adjustment_close);
            }
            else if (pre_strategy == adjustment_open)
            {
                set_strategy(attached);
            }
        }
        else
        {
            set_strategy(unknown);
        }
    }
    else if (data[5][0] < data[5][3] && data[5][3] > cri_angle)
    {
        if (data[5][0] < 10 && (pre_strategy == climbing || pre_strategy == attached))
        {
            set_strategy(adjustment_close);
        }
        else if (data[5][0] > 10 && pre_strategy == adjustment_close)
        {
            set_strategy(adjustment_close);
        }
        else
        {
            set_strategy(adjustment_open);
        }
    }
    else if (pre_strategy == climbing || pre_strategy == attached)
    {
        if ((data[5][3] - data[5][0]) < 10)
        {
            set_strategy(on_top);
        }
    }
    else if (data[5][0] > data[5][3])
    {
        if (data[5][0] > data[4][0])
        {
            set_strategy(decending);
        }
    }
    else //For situation that is not in above
    {
        set_strategy(unknown);
    }
}

void XIMU::show_pose_strategy(void)
{
    string show[3];
    switch (cur_pose)
    {
    case 0:
        show[0] = "normal_low ";
        break;
    case 1:
        show[0] = "normal_med ";
        break;
    case 2:
        show[0] = "normal_high ";
        break;
    default:
        cout << "unknown pose. " << endl;
        break;
    }
    switch (cur_strategy)
    {
    case 0:
        show[1] = "on_floor ";
        break;
    case 1:
        show[1] = "climbing ";
        break;
    case 2:
        show[1] = "attached ";
        break;
    case 3:
        show[1] = "adjustment_open ";
        break;
    case 4:
        show[1] = "adjustment_close ";
        break;
    case 5:
        show[1] = "on_top ";
        break;
    case 6:
        show[1] = "decending ";
        break;
    case 7:
        show[1] = "unknown ";
        break;
    default:
        cout << "unknown stategy. " << endl;
        break;
    }
    switch (pre_strategy)
    {
    case 0:
        show[2] = "on_floor ";
        break;
    case 1:
        show[2] = "climbing ";
        break;
    case 2:
        show[2] = "attached ";
        break;
    case 3:
        show[2] = "adjustment_open ";
        break;
    case 4:
        show[2] = "adjustment_close ";
        break;
    case 5:
        show[2] = "on_top ";
        break;
    case 6:
        show[2] = "decending ";
        break;
    case 7:
        show[2] = "unknown ";
        break;
    default:
        cout << "unknown stategy. " << endl;
        break;
    }
    cout << "Pose: " << show[0] << "\nCur_strategy: " << show[1] << "\nPre_strategy: " << show[2] << "\nCar_angle " << cen_angle << endl;
}

int *XIMU::command(void)
{
    int *array = new int[5];
    update_pose_strategy();
    show_pose_strategy();
    if (pre_strategy != cur_strategy || cur_strategy == unknown || cen_angle < cri_cen_angle) //Stop conditions
    {
        array[0] = 1;
    }
    array[1] = cur_pose;
    array[2] = cur_strategy;
    array[3] = pre_strategy;
    array[4] = cen_angle;
    return array;
}
