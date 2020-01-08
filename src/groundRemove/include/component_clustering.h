#include <iostream>
#include <array>
#include <atomic>
#include <vector>

#include "param.h"
#include <thread>
#include "bin.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "cloud.h"
#include "pixel_coords.h"

typedef std::vector<std::vector<int>> mapGrid;
typedef Cloud PointCloud;
class cluster
{
public:
    params params_;
    cluster();
    cluster(const int & roiM,
            const int & numGrid,
            const int & numCluster,
            const int & kernelSize,
            PointCloud & input,
            const params & inputParam = params()
            );

    void mapCloud2Grid();
    void mapCloud2GridThread(const unsigned int & start_idx,
                             const unsigned int & end_idx);

    void findComponent();
    void componentClustering();
    void search(int Row, int Col);

    inline int getNumCluster(){return numCluster;}
    void getClusterImg(cv::Mat & img);

    ///////////////////  use pcl ///////////////////////////
    void makeClusteredCloud(PointCloud & clusterCloud);
private:
    int numCluster;
    int numGrid;
    int roiM;
    int kernelSize;
    mapGrid grid;
    std::atomic<int> hasPoint;
    PointCloud cloud;
    std::vector<int> clusterVecID;
};

class depth_clustering
{
public:
    enum class Direction {HORIZONTAL, VERTICAL};
    depth_clustering(PointCloud & cloud, float angle_threshold = 10.0 / 180 * M_PI);

    cv::Mat getVisualizeDepthImage();

    void createDepthImage();
    // 填充需要查找的表
    void fillVector();

    size_t RowFromAngle(float angle_rows);
    size_t ColFromAngle(float angle_cols);

    int mapToColor(float val);

    // 填充一些必要的计算
    void FillCosSin();

    cv::Mat CreateAngleImage(const cv::Mat & depth_image);
    
    // 修补深度图 深度间隔超过 depth_threshold 的将不再修补 step 是修补 kernel
    cv::Mat RepairDepth(const cv::Mat& no_ground_image, int step,
                                    float depth_threshold);
                                    
    void depthCluster();

    cv::Mat ApplySavitskyGolaySmoothing(const cv::Mat & image, int window_size);
    cv::Mat GetSavitskyGolayKernel(int window_size) const;
    cv::Mat visualzieDiffAngleImage();
    // cv::Mat ZeroOutGroundBFS(const cv::Mat & image,
    //                          const cv::Mat & angle_image,
    //                          const float & threshold,
    //                          int kernel_size) const;
    void LabelOneComponent(uint16_t label, const PixelCoord & start, const cv::Mat & angle_image);
    cv::Mat GetUniformKernel(int window_size, int type) const;

    void ComputeLabels();

    void LabelOneComponent(uint16_t label, const PixelCoord & start);
    int16_t WrapCols(int16_t col) const; 

    float DiffAt(const PixelCoord & from, const PixelCoord & to) const;

    cv::Mat visSegmentImage();
    cv::Mat visAngleImage();

private:
    params _params;
    PointCloud _cloud;
    cv::Mat show_depth_image;

    std::vector<float> _row_angles;
    std::vector<float> _col_angles;    

    // 存储距离的
    cv::Mat _depthImage;

    // 存储 Row 和 col 的各种值 避免重复计算
    std::vector<float> _row_angles_sines;
    std::vector<float> _row_angles_cosines;

    // 需要提前计算的变量

    // std::vector<float> _row_alphas;
    // std::vector<float> _col_alphas;
    // 这里我们有自己的固定的角度差
    // 水平方向为 _params.vehicle_step
    // 垂直方向为 _params.horizontal_step
    // 标记好的图像
    cv::Mat _label_image;
    cv::Mat _bata_image;

    float sinRowAlpha;
    float cosRowAlpha;
    float sinColAlpha;
    float cosColAlpha;

    // 四临域
    std::array<PixelCoord, 4> Neighborhood;
    float _angle_threshold;

    uint16_t numCluster;
};
