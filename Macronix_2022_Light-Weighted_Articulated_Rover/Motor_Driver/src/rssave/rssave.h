#ifndef RSSAVE_H
#define RSSAVE_H
#include </home/potter/projects/librealsense/include/librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <fstream>                                                         // File IO
#include <iostream>                                                        // Terminal IO
#include <sstream>                                                         // Stringstreams
#include <ctime>

// 3rd party header for writing png files
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class rssave
{
public:
    rssave(void);
    void save_photo(rs2::pipeline);
};
#endif // !RSSAvE_H