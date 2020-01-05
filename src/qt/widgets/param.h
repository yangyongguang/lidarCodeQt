#include <string>
#include <math.h>
class params
{
public:
    params(){};
    int numThread = 8;
    double start_angle = -24.0 / 180 * M_PI;
    double end_angle = 2.0 / 180 * M_PI;
    size_t numBeam = 64;
    double vehicle_step = (end_angle - start_angle) / static_cast<double>(numBeam);
    size_t numCols = 870;
    size_t numRows = 64;

    double start_angle_horizontal = -M_PI;
    double end_angle_horizontal = M_PI;
    double horizontal_step = static_cast<double>(2 * M_PI / numCols);
    double min_dist = 3.4;
    double max_dist = 120;
    double dist_length = max_dist - min_dist;

    // 文件读取参数
    int sequences = 13;
    // int sequences = 21;
    std::string kitti_img_dir = 
        "/media/yyg/YYG/lidarVisualization/data_odometry_color/dataset/sequences/" +
            std::to_string(sequences) + "/image_2";

    std::string kitti_velo_dir = 
        "/media/yyg/YYG/lidarVisualization/data_odometry_velodyne/dataset/sequences/" +
            std::to_string(sequences) + "/velodyne";

    std::string kitti_base_img_dir = "/media/yyg/YYG/lidarVisualization/data_odometry_color/dataset/sequences/";
    std::string kitti_base_velo_dir = "/media/yyg/YYG/lidarVisualization/data_odometry_velodyne/dataset/sequences/";
};