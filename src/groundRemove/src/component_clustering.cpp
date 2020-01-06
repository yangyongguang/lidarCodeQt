#include "component_clustering.h"


cluster::cluster():params_(params())
{
    std::atomic_init(&hasPoint, 0);
    numCluster = 0;
    numGrid = 200;  // 网格的数量
    roiM = 100;  // 100 miter 
    kernelSize = 3; // 也就是 8 连通

    grid = mapGrid(numGrid, std::vector<int>(numGrid, hasPoint));
}

cluster::cluster(const int & roiM,
            const int & numGrid,
            const int & numCluster,
            const int & kernelSize,
            PointCloud & input,
            const params & inputParam
            ):
        roiM(roiM),
        numGrid(numGrid),
        numCluster(numCluster),
        kernelSize(kernelSize),
        cloud(input),
        params_(inputParam)

{
    std::atomic_init(&hasPoint, 0);
    grid = mapGrid(numGrid, std::vector<int>(numGrid, hasPoint));
}

void cluster::mapCloud2Grid()
{
    std::vector<std::thread> threads(params_.numThread);
    // std::cout << "numThread " << params_.numThread << "\n";
    int num_pre_thread = cloud.size() / params_.numThread; 

    // std::cout << "params_.n_threads " << params_.n_threads << std::endl;  
    for (int idx = 0; idx < params_.numThread - 1; ++idx)
    {
        // std::cout << "thread in for :" << idx << std::endl;
        int start_idx = idx * num_pre_thread;
        int end_idx = (idx + 1) * num_pre_thread - 1;
        threads[idx] = std::thread(&cluster::mapCloud2GridThread, 
                                    this, start_idx, end_idx);        
    }

    const size_t start_idx = num_pre_thread * (params_.numThread - 1);
    const size_t end_idx = cloud.size() - 1;
    threads[params_.numThread - 1] = std::thread(&cluster::mapCloud2GridThread, 
                                    this, start_idx, end_idx);

    // 等待所有的线程结束任务
    for (auto it = threads.begin(); it != threads.end(); ++it)
    {
        it->join();
    }

}

void cluster::mapCloud2GridThread(const unsigned int & start_idx, const unsigned int & end_idx)
{
    // printf("cloud size %ld\n", cloud.points.size());
    // printf("start idx %d, end_idx %d\n" , start_idx, end_idx);
    // printf("mapCloud2GridThread\n");
    // printf("numGrid %d, roiM %d\n", numGrid, roiM);
    for (int idx = start_idx; idx <= end_idx; ++idx)
    {
        point point(cloud[idx].x(), cloud[idx].y(), cloud[idx].z());
        double x = point.x();
        double y = point.y();
        
        double xC = x + roiM / 2;
        double yC = y + roiM / 2;
        // std::cout << "xC " << xC << " yC " << yC << '\n';
        if (xC < 0 || xC >= roiM || yC < 0 || yC >= roiM) 
            continue;
        int row = std::floor(xC / roiM * numGrid);
        int col = std::floor(yC / roiM * numGrid);

        // std::cout << "row " << row << " col " << col << '\n';
        // std::cout << "grid size " << grid[0].size() << " "<< grid.size() << "\n";
        grid[row][col] = -1; // 表示有点了
    }
}

void cluster::componentClustering()
{    
    findComponent();    
}

void cluster::findComponent()
{
    mapCloud2Grid();
    // 便利每一个网格
    for (int row = 0; row < numGrid; ++row)
    {
        for (int col = 0; col < numGrid; ++col)
        {
            if (grid[row][col] != -1)
                continue;
            numCluster++; // 每次搜索一次会搜索直到结束， 所以 ID 会加一
            search(row, col);            
        }
    }
}

void cluster::search(int Row, int Col)
{
    int mean = kernelSize / 2;
    for (int kI = -mean; kI <= mean; ++kI)
    {
        if (kI + Row < 0 || kI + Row >= numGrid) continue;
        for (int kJ = -mean; kJ <= mean; ++kJ)
        {
            if (kJ + Col < 0 || kJ + Col >= numGrid) continue;
            if (grid[kI + Row][kJ + Col] == -1)
            {
                grid[kI + Row][kJ + Col] = numCluster;
                search(kI + Row, kJ + Col);
            }
        }
    }
}

void cluster::getClusterImg(cv::Mat & img)
{
    cv::Mat res = cv::Mat(numGrid, numGrid, CV_8UC3, cv::Scalar(0, 0, 0));
    for (int idx = 0; idx < cloud.size(); ++idx)
    {
        point point(cloud[idx].x(), cloud[idx].y(), cloud[idx].z());
        double x = point.x();
        double y = point.y();
        double z = point.z();

        double xC = x + roiM / 2;
        double yC = y + roiM / 2;

        if (xC < 0 || xC >= roiM || yC < 0 || yC >= roiM)
            continue;
        
        int xI = floor(numGrid * xC / roiM);
        int yI = floor(numGrid * yC / roiM);

        int clusterNum = grid[xI][yI];
        if (clusterNum != 0)
        {
            res.at<cv::Vec3b>(xI, yI)[0] = (500 * clusterNum) % 255;
            res.at<cv::Vec3b>(xI, yI)[1] = (100 * clusterNum) % 255;
            res.at<cv::Vec3b>(xI, yI)[2] = (150 * clusterNum) % 255;
        }
    }

    img = res;
}

void cluster::makeClusteredCloud(PointCloud & clusterCloud)
{
    for (int idx = 0; idx <clusterCloud.size(); ++idx)
    {
        point point(clusterCloud[idx].x(), clusterCloud[idx].y(), clusterCloud[idx].z());
        double x = point.x();
        double y = point.y();
        double z = point.z();

        double xC = x + roiM / 2;
        double yC = y + roiM / 2;

        if (xC < 0 || xC >= roiM || yC < 0 || yC >= roiM)
            continue;
        
        int xI = floor(numGrid * xC / roiM);
        int yI = floor(numGrid * yC / roiM);

        int clusterNum = grid[xI][yI];
        clusterCloud[idx].classID = clusterNum;
    }
}



/// depth_clustering
depth_clustering::depth_clustering(PointCloud & cloud):
        _cloud(cloud),
        _params(params())
{
    _depth_image = cv::Mat(_params.numBeam, _params.numCols, CV_8UC3, cv::Scalar(0, 0, 0));
}

void depth_clustering::fillVector(/*std::vector<double> & input, const Direction & direction*/)
{
    double rad = _params.start_angle;
    for (int i = 0; i < _params.numBeam; ++i)
    {
        _row_angles.emplace_back(rad);
        rad += _params.vehicle_step;
    }

    double rad_horizontal = _params.start_angle_horizontal;
    for (int i = 0; i < _params.numCols; ++i)
    {
        _col_angles.emplace_back(rad_horizontal);
        rad_horizontal += _params.horizontal_step;
    }

    assert(_row_angles.size() == _params.numRows);
    assert(_col_angles.size() == _params.numCols);


}

size_t depth_clustering::RowFromAngle(double angle_rows)
{
    size_t found = 0;
    found = std::upper_bound(_row_angles.begin(), _row_angles.end(), angle_rows) -
        _row_angles.begin();
    
    if (found == 0) return found;
    if (found == _row_angles.size()) return found - 1;

    double diff_next = std::abs(_row_angles[found] - angle_rows);
    double diff_prev = std::abs(angle_rows - _row_angles[found - 1]);

    return diff_next < diff_prev ? found : found - 1;
    
}

size_t depth_clustering::ColFromAngle(double angle_cols)
{
    size_t found = 0;
    // printf("angle_cols %f\n", angle_cols);

    found = std::upper_bound(_col_angles.begin(), _col_angles.end(), angle_cols) -
        _col_angles.begin();
    // printf("found %d\n", found);
    if (found == 0) return found;
    if (found == _col_angles.size()) return found - 1;

    double diff_next = std::abs(_col_angles[found] - angle_cols);
    double diff_prev = std::abs(angle_cols - _col_angles[found - 1]);
    return diff_next < diff_prev ? found : found - 1;
}

void depth_clustering::createDepthImage()
{
    fillVector(/*_row_angles/*, Direction::VERTICAL*/);    

    // for (auto it = _row_angles.begin(); it != _row_angles.end(); ++it)
    //     printf("%f ", *it);
    // printf("\n\n\n\n");

    // for (auto it = _col_angles.begin(); it != _col_angles.end(); ++it)
    //     printf("%f ", *it);
    // printf("horizontal_step %f\n", _params.horizontal_step);
    // printf("\n\n\n\n");

    for (int idx = 0; idx < _cloud.size(); ++idx)
    {
        point point(_cloud[idx].x(), _cloud[idx].y(), _cloud[idx].z());
        double dist_to_sensor = sqrt(point.x() * point.x() + point.y() * point.y() + point.z() * point.z());
        double angle_rows = asin(point.z() / dist_to_sensor);
        double angle_cols = atan2(point.y(), point.x());

        size_t bin_rows = RowFromAngle(angle_rows);
        size_t bin_cols = ColFromAngle(angle_cols);
        // if (bin_cols < 5)
        // printf("(%d, %d) = %d\n", bin_rows, bin_cols, static_cast<int>(dist_to_sensor * 500) % 255);
        // _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[0] = 
        //     static_cast<int>(dist_to_sensor * 500) % 255;
        // _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[1] = 
        //     static_cast<int>(dist_to_sensor * 500) % 255;
        // _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[2] = 
        //     static_cast<int>(dist_to_sensor * 500) % 255;

        int value = mapToColor(dist_to_sensor);
        auto color = _params.RANDOM_COLORS[value];
        _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[0] = color[0];
        _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[1] = color[1];
        _depth_image.at<cv::Vec3b>(bin_rows, bin_cols)[2] = color[2];
    }     

    // 翻转图像
    // for (int i = 0; i < _depth_image.rows; ++i)
    // {
    //     for (int j = 0; j < _depth_image.cols; ++j)
    //     {
    //         printf("%d ", _depth_image.at<uchar>(i, j));
    //     }
    //     printf("\n");
    // } 
}

int depth_clustering::mapToColor(double val)
{
    // cv::Vec3b res;
    if (val <= _params.min_dist) val = _params.min_dist;
    if (val >= _params.max_dist) val = _params.max_dist;
    int value = static_cast<int>((val - _params.min_dist) / _params.dist_length * 255);
    // res = {value, value, value};
    return value;
    // printf("val %f   value %d\n", val, value);
}
