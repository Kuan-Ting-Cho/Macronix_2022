#ifndef RSDIS_H
#define RSDIS_H
#include </home/potter/projects/librealsense/include/librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <iostream>

class rsdis
{
public:
    rsdis(void);                  // constructor
    float get_dis(rs2::pipeline); // getting distance from rs
};
#endif // !RSDIS_H
