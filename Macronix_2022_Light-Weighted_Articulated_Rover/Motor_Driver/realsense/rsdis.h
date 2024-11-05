#ifndef RSDIS_H
#define RSDIS_H
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <iostream>

class Realsense
{
    public:
       	Realsense(void);   //constructor
        float get_dis(rs2::pipeline);  //getting distance from rs

        

};
#endif // !RSDIS_H
