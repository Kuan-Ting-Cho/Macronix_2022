#include "XIMU.h"
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
using namespace std;

string transform(Strategy);
////////////////////////////////////////
XIMU::XIMU()
{
    set_pose(normal_med);
    set_strategy(start);
    cout << cur_strategy << endl;
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

int *XIMU::get_car_angle(void)
{
    int *array = new int[1];
    cen_angle = 180 - data[5][0] - data[5][3];
    array[0] = cen_angle;
    return array;
}

void XIMU::set_yaw_bias(float a)
{
    pre_yaw_bias = cur_yaw_bias;
    cur_yaw_bias = a;
}

void XIMU::update_pose_strategy(void)
{
    string filename = "/home/potter/2022WonHon/Motor_Driver/src/imu_data.txt";
    int Previous_Line, Current_Line;
    Previous_Line = Current_Line = 0;
    string Last_Line; // Current last_Line
    string Line;
    fstream imu_file; // for read file
    stringstream ss;
    // string str[5]; //Store IMU update data
    float temp = 0;
    int i = 0;
    int record = 0; // Previous line
    int rs = 0;     // wait for rs ready

    /*Read imu data*/
    while (Previous_Line == Current_Line) // Continuous read file
    {
        if (i > 5) // Last_line which was recorded must lower than 5 lines
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
            // str[i] = Last_Line;
            imu_file.close();
            if (record != Current_Line && i < 6) // if current_Line >previous_Line,record current last_line information
            {
                ss << Last_Line; // update imu data information     /*Roll >> Pitch >> Yaw>>Roll >> Pitch >> Yaw*/
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
        Previous_Line = Current_Line = 0; // reset to initial
    }
    /*Clear imu_data*/
    imu_file.open(filename, ios::out | ios::trunc);
    imu_file << "\n";
    imu_file.close();
    sleep(0.5);

    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[2][0], data[2][1], data[2][2], data[2][3], data[2][4], data[2][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[3][0], data[3][1], data[3][2], data[3][3], data[3][4], data[3][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[4][0], data[4][1], data[4][2], data[4][3], data[4][4], data[4][5]);
    printf("\n%.2f %.2f %.2f %.2f %.2f %.2f\n", data[5][0], data[5][1], data[5][2], data[5][3], data[5][4], data[5][5]);

    /*Update car_angle && yaw_angle_bias*/
    cen_angle = 180 - data[5][0] - data[5][3];
    set_yaw_bias(abs(data[5][2] - start_yaw_angle));

    /*Judge whether to go right or go left*/
    if (cur_yaw_bias > 10)
        yaw_ok = 0;
    else
    {
        if (cur_strategy == t_left || cur_strategy == t_right)
        {
            set_strategy(Before_adj_yaw);
        }
        yaw_ok = 1;
    }

    /*strategy update*/
    if (cur_strategy == start)
    {
        start_yaw_angle = (data[5][2] + data[4][2]) / 2; // set initial rear mean yaw angle
        set_strategy(adj_initial_pose);
    }

    else if (cur_strategy == decending || decend == 1)
    {
        if (data[5][0] >= cri_angle)
            set_strategy(adjustment_open);
        else if (abs(data[5][0] - data[5][3]) < 7)
        {
            set_strategy(on_floor);
            decend = 0;
            initial_ok = 0;
        }
        else
            set_strategy(decending);
    }

    else if ((cur_strategy == on_floor || cur_strategy == on_top || cur_strategy == t_right || cur_strategy == t_left) && yaw_ok == 0)
    {

        if (abs(data[5][0] - data[5][3] <= 7))
        {
            if (cur_strategy != t_right || cur_strategy != t_left)
            {
                Before_adj_yaw = cur_strategy;
                set_strategy(t_right);
            }

            if (cur_yaw_bias > pre_yaw_bias)
            {
                if (cur_strategy == t_right)
                    set_strategy(t_left);
                else if (cur_strategy == t_left)
                    set_strategy(t_right);
            }
        }
        else
        {
            yaw_ok = 1;
            cout << "ERROR!!!!!!!!!!!!" << endl;
        }
    }

    else if (cur_strategy == on_top || cur_strategy == on_floor_again)
    {
        if (data[5][0] - data[5][3] >= 15) // on top decending
        {
            set_strategy(decending);
            decend = 1;
        }
        else if (data[5][3] - data[5][0] >= 15) // on top climbing
        {
            set_strategy(climbing);
            decend = 0;
        }
        else
            set_strategy(on_floor_again);
    }

    else if ((cur_strategy == on_floor || cur_strategy == adj_initial_pose) && initial_ok == 0)
    {
        set_strategy(adj_initial_pose);
    }

    else if (cur_strategy == keep_climbing || cur_strategy == keep_climbing2)
    {
        if ((data[5][3] - data[5][0]) < 10)
        {
            set_strategy(on_top);
        }
        else if (data[5][3] < 80)
        {
            set_strategy(keep_climbing2);
        }
        else if (data[5][3] >= 80)

        {
            set_strategy(keep_climbing);
        }
    }

    else if (cur_strategy != climbing && cur_strategy != attached && cur_strategy != keep_climbing && cur_strategy != keep_climbing2 && (data[5][3] - data[5][0]) <= 7 && initial_ok == 1)
    {
        set_strategy(on_floor);
    }

    else if (data[5][0] < data[5][3] && data[5][3] < cri_angle) // uncertain logic//
    {
        if (data[5][0] <= data[3][0] && data[5][0] > 10)
        {
            set_strategy(climbing);
        }
        else if (data[5][0] < 15) // front_wheel on top
        {
            if (cur_strategy == unknown)
            {
                set_strategy(pre_strategy);
            }
            else if (cur_strategy == attached) /*Remove adjustment_close strategy*/
            {
                set_strategy(keep_climbing);
            }

            else if (cur_strategy == climbing)
            {
                set_strategy(attached);
            }
            else if (cur_strategy == attached || cur_strategy == adjustment_close)
            {
                set_strategy(adjustment_close);
            }
            else if (cur_strategy == adjustment_open)
            {
                set_strategy(attached);
            }
            else
            {
                set_strategy(climbing);
            }
        }
        else
        {
            set_strategy(climbing);
        }
    }
    else if (data[5][0] < data[5][3] && data[5][3] > cri_angle) // may fall down
    {
        set_strategy(adjustment_open);
    }
    else if (data[5][0] > data[5][3])
    {
        if (data[5][0] > data[3][0])
        {
            set_strategy(decending);
        }
    }
    else // For situation that is not in above
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
    show[1] = transform(cur_strategy);
    show[2] = transform(pre_strategy);

    std::cout << "Pose: " << show[0] << "\nCur_strategy: " << show[1] << "\nPre_strategy: "
              << show[2] << "\nCen_angle " << cen_angle << "\nInitial_ok= " << initial_ok << "\ndecend= "
              << decend << "\nyaw_ok= " << yaw_ok << "\ncur_yaw_bias= " << cur_yaw_bias << "\npre_yaw_bias= " << pre_yaw_bias << endl;
}

int *XIMU::command(void)
{
    int *array = new int[5];
    update_pose_strategy();
    show_pose_strategy();

    if (cur_strategy == unknown) // Stop conditions
    {
        array[0] = 1;
    }
    else if (pre_strategy == adjustment_open && cur_strategy != adjustment_open)
    {
        array[0] = 1;
    }
    else if (pre_strategy == adjustment_close && cur_strategy != adjustment_close)
    {
        array[0] = 1;
    }

    array[1] = cur_pose;
    array[2] = cur_strategy;
    array[3] = pre_strategy;
    array[4] = cen_angle;
    return array;
}

void XIMU::Record(void)
{
    // Record current time
    time_t current_time;
    current_time = time(NULL);

    string filename = "/home/xiang/2022WonHon/Motor_Driver/src/record.txt";
    ofstream imu_file; // for read file
    imu_file.open(filename, ios::out | ios::app);
    imu_file << "Pitch: \n";
    for (int i = 2; i < 6; i++)
    {
        imu_file << data[i][0] << " " << data[5][3] << "\n";
    }
    imu_file << "Yaw: \n";
    for (int i = 2; i < 6; i++)
    {
        imu_file << data[i][2] << " " << data[i][5] << "\n";
    }
    imu_file << "Cur_strategy: \n";
    imu_file << transform(cur_strategy) << "\n";
    imu_file << "Pre_strategy: \n";
    imu_file << transform(pre_strategy) << "\n";
    imu_file << "Cur_yaw_bias: \n";
    imu_file << cur_yaw_bias << "\n";
    imu_file << "Pre_yaw_bias\n";
    imu_file << pre_yaw_bias << "\n";
    imu_file.close();
}

string transform(Strategy a)
{
    switch (a)
    {
    case 0:
        return ("start");
        break;
    case 1:
        return ("adj_initial_pose ");
        break;
    case 2:
        return ("on_floor ");
        break;
    case 3:
        return ("climbing ");
        break;
    case 4:
        return ("attached ");
        break;
    case 5:
        return ("adjustment_open ");
        break;
    case 6:
        return ("adjustment_close ");
        break;
    case 7:
        return ("keep_climbing ");
        break;
    case 8:
        return ("keep_climbing2 ");
        break;
    case 9:
        return ("on_top ");
        break;
    case 10:
        return ("decending ");
        break;
    case 11:
        return ("on_floor_again ");
        break;
    case 12:
        return ("turn_right");
        break;
    case 13:
        return ("turn_left");
        break;
    case 14:
        return ("unknown");
        break;
    default:
        return ("unknown stategy. ");
        break;
    }
}
