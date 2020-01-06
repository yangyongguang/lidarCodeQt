#include <iostream>
#include <array>
#include <atomic>
#include <vector>

#include "param.h"
#include <thread>
#include "bin.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "cloud.h"

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
    depth_clustering(PointCloud & cloud);
    void createDepthImage();
    // 填充需要查找的表
    void fillVector();

    size_t RowFromAngle(double angle_rows);
    size_t ColFromAngle(double angle_cols);

    int mapToColor(double val);
    inline cv::Mat getDepthMat() {cv::Mat res; cv::flip(_depth_image, res, -1);return res;}

private:
    params _params;
    PointCloud _cloud;
    cv::Mat _depth_image;

    std::vector<double> _row_angles;
    std::vector<double> _col_angles;    
};
