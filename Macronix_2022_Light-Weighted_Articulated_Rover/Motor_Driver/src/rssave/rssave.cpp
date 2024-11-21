#include "rssave.h"

rssave::rssave(void)
{

}

void rssave::save_photo(rs2::pipeline pipe)
{
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    // Start streaming with default recommended configuration
    // pipe.start();

    // Record current time
    time_t current_time;
    current_time = time(NULL);

    // Capture 30 frames to give autoexposure, etc. a chance to settle
    for (auto i = 0; i < 2; ++i) pipe.wait_for_frames();

    // Wait for the next set of frames from the camera. Now that autoexposure, etc.
    // has settled, we will write these to disk
    for (auto&& frame : pipe.wait_for_frames())
    {
        // We can only save video frames as pngs, so we skip the rest
        if (auto vf = frame.as<rs2::video_frame>())
        {
            auto stream = frame.get_profile().stream_type();
            // Use the colorizer to get an rgb image for the depth stream
            if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);

            // Write images to disk
            std::stringstream png_file;
            if(vf.get_profile().stream_name()=="Color")
            {
                png_file << "rs-" << current_time << ".png";
                stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
                            vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
                std::cout << "Saved " << png_file.str() << std::endl;
            }
        }
    }
    //pipe.stop();
}