#pragma once
#include <string>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

/*
1-normal_low = IMU degrees both are below 30 degrees
2-normal_med = IMU degrees both are between 30 - 70 degrees
3-normal_high = IMU degrees both are higher than 70 degrees
*/
enum Pose
{
    normal_low,
    normal_med,
    normal_high,
};
/*
0-adj_initial_pose= Adjust initial pose
1-on_floor= Normal status on the floor
2-climbing = Back_imu' s degree is higher than front
3-attached = Back_imu' s attached to obstacle
4-adjustment_open = Increase center degrees of car
5-adjustment_close= Decrease center degrees of car
6-keep_climbing = climbing until on the top of obstacle
7-keep_climbing2 = climbing until on the top of obstacle /******
8-on_top = As normal status but on the top of obstacle
9-decending = Back_imu' s degree is lower than front
10-on_floor_again= Back to normal status on the floor
11-unknown stattus
*/
enum Strategy
{
    start,
    adj_initial_pose,
    on_floor,
    climbing,
    attached,
    adjustment_open,
    adjustment_close,
    keep_climbing,
    keep_climbing2,
    on_top,
    decending,
    on_floor_again,
    t_right,
    t_left,
    unknown
};

class XIMU
{
public:
    XIMU(void); // Initialize pose as normal_med
    ~XIMU();
    //--------------------------//
    void set_pose(Pose);
    Pose get_pose(void);
    void set_strategy(Strategy);
    Strategy get_strategy(void);
    int *get_car_angle(void);
    int initial_ok = 0; // Judge if the car initial pose
    int decend = 0;     // Judge if the car is decending
    int yaw_ok = 0;     // Judge if the car adjust yaw angle
    //--------------------------//
    void update_pose_strategy(void);
    void show_pose_strategy(void);
    int *command(void);
    void set_yaw_bias(float);
    void Record(void);

private:
    Pose cur_pose;
    Strategy pre_strategy;
    Strategy cur_strategy;
    float cri_angle = 85; // critical angle of Car_climbing status
    float cen_angle;      // center angle
    float data[6][6];     // Store imu data for six times (4 data)
    float start_yaw_angle = 0;
    float pre_yaw_bias = 0;
    float cur_yaw_bias = 0;
    Strategy Before_adj_yaw;
};
