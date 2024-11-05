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
0-on_floor= Normal status on the floor
1-climbing = Back_imu' s degree is higher than front
2-attached = Back_imu' s attached to obstacle
3-adjustment_open = Increase center degrees of car  
4-adjustment_close= Decrease center degrees of car 
5-on_top = As normal status but on the top of obstacle
6-decending = Back_imu' s degree is lower than front
7-unknown stattus
*/
enum Strategy
{
    on_floor,
    climbing,
    attached,
    adjustment_open,
    adjustment_close,
    on_top,
    decending,
    unknown
};

class XIMU
{
public:
    XIMU(void); //Initialize pose as normal_med
    ~XIMU();
    //--------------------------//
    void set_pose(Pose);
    Pose get_pose(void);
    void set_strategy(Strategy);
    Strategy get_strategy(void);
    float get_car_angle(void);
    //--------------------------//
    void update_pose_strategy(void);
    void show_pose_strategy(void);
    int *command(void);

private:
    Pose cur_pose;
    Strategy pre_strategy;
    Strategy cur_strategy;
    float cri_angle = 80; //critical angle of Car_climbing status
    float cen_angle;      //center angle
    float cri_cen_angle;  //center critical angle
};