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
depth_clustering::depth_clustering(const PointCloud & cloud, bool filter, float angle_threshold):
        // _cloud(cloud),
        _filter(filter),
        _params(params())
{
    _angle_threshold = (angle_threshold / 180 * M_PI);
    _cloud = cloud;
    show_depth_image = cv::Mat(_params.numBeam, _params.numCols, CV_8UC3, cv::Scalar(0, 0, 0));
    fillVector();
    FillCosSin();
    _depthImage = cv::Mat::zeros(_params.numBeam, _params.numCols, cv::DataType<float>::type);
    sinColAlpha = sin(_params.horizontal_step);
    cosColAlpha = cos(_params.horizontal_step);
    sinRowAlpha = sin(_params.vehicle_step);
    cosRowAlpha = cos(_params.vehicle_step);  

    Neighborhood[0] = PixelCoord(-1, 0);
    Neighborhood[1] = PixelCoord(1, 0);
    Neighborhood[2] = PixelCoord(0, -1);
    Neighborhood[3] = PixelCoord(0, 1);   

    numCluster = 0; 
}


void depth_clustering::fillVector(/*std::vector<double> & input, const Direction & direction*/)
{

    _row_angles.assign(_params.row_angles.begin(), _params.row_angles.end());

    // float rad = _params.start_angle;
    // for (int i = 0; i < _params.numBeam; ++i)
    // {
    //     _row_angles.emplace_back(rad);
    //     rad += _params.vehicle_step;
    // }

    // _row_angles.assign(_params.raw_angles.begin(), _params.raw_angles.end());
    double rad_horizontal = _params.start_angle_horizontal;
    for (int i = 0; i < _params.numCols; ++i)
    {
        _col_angles.emplace_back(rad_horizontal);
        rad_horizontal += _params.horizontal_step;
    }

    assert(_row_angles.size() == _params.numRows);
    assert(_col_angles.size() == _params.numCols);

    // fprintf(stderr, "_row_angles\n");
    // for (auto it = _row_angles.begin(); it != _row_angles.end(); ++it)
    // {
    //     fprintf(stderr, "%f ", *it / M_PI * 180);
    // }
    // fprintf(stderr, "\n\n\n\n");
    // fprintf(stderr, "numBeam[%d], numRows[%d], calculate[%d]\n", 
    //         _params.numBeam,
    //         _params.numRows,
    //         (int)((_params.end_angle - _params.start_angle) / 0.4)
    // );
    // fprintf(stderr, "start_angle[%f], end_angle[%f]\n", _params.start_angle, _params.end_angle);
}

void depth_clustering::FillCosSin()
{
    _row_angles_cosines.clear();
    _row_angles_sines.clear();

    _row_angles_sines.assign(_params.row_angle_sines.begin(), _params.row_angle_sines.end());
    _row_angles_cosines.assign(_params.row_angle_cosines.begin(), _params.row_angle_cosines.end());

    // for (const auto & angle : _row_angles)
    // {
    //     _row_angles_sines.emplace_back(sin(angle));
    //     _row_angles_cosines.emplace_back(cos(angle));
    // }
}

// 根据 角度， 查找 Row 索引
size_t depth_clustering::RowFromAngle(float angle_rows)
{
    size_t found = 0;
    // found = std::upper_bound(_row_angles.begin(), _row_angles.end(), angle_rows) -
    //     _row_angles.begin();
    found = std::upper_bound(_row_angles.begin(), _row_angles.end(), angle_rows, std::greater<float>())
         - _row_angles.begin();
    
    if (found == 0) return found;
    if (found == _row_angles.size()) return found - 1;

    double diff_next = std::abs(_row_angles[found] - angle_rows);
    double diff_prev = std::abs(angle_rows - _row_angles[found - 1]);

    return diff_next < diff_prev ? found : found - 1;
    
}

size_t depth_clustering::ColFromAngle(float angle_cols)
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

cv::Mat depth_clustering::CreateAngleImage(const cv::Mat & depth_image)
{
    cv::Mat angle_image = cv::Mat::zeros(depth_image.size(), cv::DataType<float>::type);
    cv::Mat x_mat = cv::Mat::zeros(depth_image.size(), cv::DataType<float>::type);
    cv::Mat y_mat = cv::Mat::zeros(depth_image.size(), cv::DataType<float>::type);

    const auto & sines_vec = _row_angles_sines;
    const auto & cosines_vec = _row_angles_cosines;

    // 环形图像
    float  dx, dy;
    x_mat.row(0) = depth_image.row(0) * cosines_vec[0];
    y_mat.row(0) = depth_image.row(0) * sines_vec[0];

    // 
    for (int r = 1; r < angle_image.rows; ++r)
    {
        x_mat.row(r) = depth_image.row(r) * cosines_vec[r];
        y_mat.row(r) = depth_image.row(r) * sines_vec[r];

        for ( int c = 0; c < angle_image.cols; ++c)
        {
            dx = fabs(x_mat.at<float>(r, c) - x_mat.at<float>(r - 1, c));
            dy = fabs(y_mat.at<float>(r, c) - y_mat.at<float>(r - 1, c));
            angle_image.at<float>(r, c) = atan2(dy, dx);
        }
    }
    return angle_image;
}

void depth_clustering::createDepthImage()
{
    for (int idx = 0; idx < _cloud.size(); ++idx)
    {
        point point(_cloud[idx].x(), _cloud[idx].y(), _cloud[idx].z());
        double dist_to_sensor = sqrt(point.x() * point.x() + point.y() * point.y() + point.z() * point.z());
        double angle_rows = asin(point.z() / dist_to_sensor);
        double angle_cols = atan2(point.y(), point.x());

        // fprintf(stderr, "angle:%f\n", angle_rows);
        size_t bin_rows = RowFromAngle(angle_rows);
        size_t bin_cols = ColFromAngle(angle_cols);
        // fprintf(stderr, "(%f, %f, %f)[%d, %d]\n", _cloud[idx].x(), _cloud[idx].y(),_cloud[idx].z(), bin_rows, bin_cols);
        _depthImage.at<float>(bin_rows, bin_cols) = dist_to_sensor;
        // 存储每个点映射到的坐标
        pointToImageCoord.emplace_back(std::make_pair(bin_rows, bin_cols));

    }     
    
    if (_cloud.size() < 200)
    {
        for(int idx = 0; idx < _cloud.size(); ++idx)
        {
            fprintf(stderr, "point id(%d) ---> (%f, %f, %f)(%d, %d)[%d], angle[%f]\n", idx, 
                        _cloud[idx].x(), _cloud[idx].y(), _cloud[idx].z(),
                        pointToImageCoord[idx].first, pointToImageCoord[idx].second,
                        _cloud[idx].classID,
                        std::atan2(_cloud[idx].y(), _cloud[idx].x()) / M_PI * 180);
        }
    }
    // fprintf(stderr, "\n\n\n\n\n\n\n");
}

cv::Mat depth_clustering::getVisualizeDepthImage()
{
    cv::Mat res;
    for (int r = 0; r < _depthImage.rows; ++r)
    {
        for (int c = 0; c < _depthImage.cols; ++c)
        {
            float dist_to_sensor = _depthImage.at<float>(r, c);
            int value = mapToColor(dist_to_sensor);
            // 没有点映射的位置
            if (dist_to_sensor < 0.001f)
                continue;
            show_depth_image.at<cv::Vec3b>(r, c)[0] = value;
            show_depth_image.at<cv::Vec3b>(r, c)[1] = 255;
            show_depth_image.at<cv::Vec3b>(r, c)[2] = 255;
        }
    }
    cv::cvtColor(show_depth_image, res, cv::COLOR_HSV2BGR);
    cv::flip(res, res, 1);
    return res;
}

int depth_clustering::mapToColor(float val)
{
    // cv::Vec3b res;
    if (val <= _params.min_dist) val = _params.min_dist;
    if (val >= _params.max_dist) val = _params.max_dist;
    // int value = static_cast<int>((val - _params.min_dist) / _params.dist_length * 255);
    int value = static_cast<int>((val - _params.min_dist) / _params.dist_length * 180);
    // res = {value, value, value};
    return value;
    // printf("val %f   value %d\n", val, value);
}


cv::Mat depth_clustering::RepairDepth(const cv::Mat& no_ground_image, int step,
                                    float depth_threshold) 
{
    cv::Mat inpainted_depth = no_ground_image.clone();
    for (int c = 0; c < inpainted_depth.cols; ++c) 
    {
        for (int r = 0; r < inpainted_depth.rows; ++r) 
        {
            float& curr_depth = inpainted_depth.at<float>(r, c);
            // 说明这里没有点映射过来
            if (curr_depth < 0.001f) 
            {
                int counter = 0;
                float sum = 0.0f;
                // i 是走向后面的， j 是走向后面的， 并且 都在同一列上面，进行插值
                for (int i = 1; i < step; ++i) 
                {
                    if (r - i < 0) 
                    {
                        continue;
                    }
                    for (int j = 1; j < step; ++j) 
                    {
                        if (r + j > inpainted_depth.rows - 1) 
                        {
                            continue;
                        }
                        const float& prev = inpainted_depth.at<float>(r - i, c);
                        const float& next = inpainted_depth.at<float>(r + j, c);
                        if (prev > 0.001f && next > 0.001f &&
                            fabs(prev - next) < depth_threshold) 
                        {
                            sum += prev + next;
                            counter += 2;
                        }
                    }
                }
                if (counter > 0) 
                {
                    curr_depth = sum / counter;
                }
            }
        }
    }
    return inpainted_depth;
}

// 主函数
void depth_clustering::depthCluster()
{
    createDepthImage();
    _depthImage = RepairDepth(_depthImage, 5, 1.0f);
    if (_filter)
        _depthImage = ApplySavitskyGolaySmoothing(_depthImage, 5);   
    int numPixel = 0;
    // for(int row = 0; row < _depthImage.rows; ++row)
    // {
    //     for (int col = 0; col < _depthImage.cols; ++col)
    //     {
    //         if (_depthImage.at<float>(row, col) > 0.001f)
    //         {
    //             numPixel++;
    //         }
    //     }
    // }
    // fprintf(stderr, "has no empty pixel: %d\n", numPixel);
    // auto angle_image = CreateAngleImage(_depthImage);
    // _depthImage = ApplySavitskyGolaySmoothing(angle_image, 5); 
    // 为depthImage 打标签
    ComputeLabels();
    // 给点云打标签 classID
    // LabelCloud();
    fprintf(stderr, "cluster :%d\n", numCluster);
    // fprintf(stderr, "\n\n\n\n\n\n\n\n");
}

cv::Mat depth_clustering::ApplySavitskyGolaySmoothing(const cv::Mat & image, int window_size)
{
    cv::Mat kernel = GetSavitskyGolayKernel(window_size);
    cv::Mat smoothed_image;  // init an empty smoothed image
    cv::filter2D(image, smoothed_image, -1, kernel, cv::Point(-1, -1),
                0, cv::BORDER_REFLECT101);
    return smoothed_image;
}

cv::Mat depth_clustering::GetSavitskyGolayKernel(int window_size) const 
{
    if (window_size % 2 == 0) 
    {
        throw std::logic_error("only odd window size allowed");
    }
    bool window_size_ok = window_size == 5 || window_size == 7 ||
                        window_size == 9 || window_size == 11;
    if (!window_size_ok) {
    throw std::logic_error("bad window size");
    }
    // below are no magic constants. See Savitsky-golay filter.
    cv::Mat kernel;
    switch (window_size) {
    case 5:
        kernel = cv::Mat::zeros(window_size, 1, CV_32F);
        kernel.at<float>(0, 0) = -3.0f;
        kernel.at<float>(0, 1) = 12.0f;
        kernel.at<float>(0, 2) = 17.0f;
        kernel.at<float>(0, 3) = 12.0f;
        kernel.at<float>(0, 4) = -3.0f;
        kernel /= 35.0f;
        return kernel;
    case 7:
        kernel = cv::Mat::zeros(window_size, 1, CV_32F);
        kernel.at<float>(0, 0) = -2.0f;
        kernel.at<float>(0, 1) = 3.0f;
        kernel.at<float>(0, 2) = 6.0f;
        kernel.at<float>(0, 3) = 7.0f;
        kernel.at<float>(0, 4) = 6.0f;
        kernel.at<float>(0, 5) = 3.0f;
        kernel.at<float>(0, 6) = -2.0f;
        kernel /= 21.0f;
        return kernel;
    case 9:
        kernel = cv::Mat::zeros(window_size, 1, CV_32F);
        kernel.at<float>(0, 0) = -21.0f;
        kernel.at<float>(0, 1) = 14.0f;
        kernel.at<float>(0, 2) = 39.0f;
        kernel.at<float>(0, 3) = 54.0f;
        kernel.at<float>(0, 4) = 59.0f;
        kernel.at<float>(0, 5) = 54.0f;
        kernel.at<float>(0, 6) = 39.0f;
        kernel.at<float>(0, 7) = 14.0f;
        kernel.at<float>(0, 8) = -21.0f;
        kernel /= 231.0f;
        return kernel;
    case 11:
        kernel = cv::Mat::zeros(window_size, 1, CV_32F);
        kernel.at<float>(0, 0) = -36.0f;
        kernel.at<float>(0, 1) = 9.0f;
        kernel.at<float>(0, 2) = 44.0f;
        kernel.at<float>(0, 3) = 69.0f;
        kernel.at<float>(0, 4) = 84.0f;
        kernel.at<float>(0, 5) = 89.0f;
        kernel.at<float>(0, 6) = 84.0f;
        kernel.at<float>(0, 7) = 69.0f;
        kernel.at<float>(0, 8) = 44.0f;
        kernel.at<float>(0, 9) = 9.0f;
        kernel.at<float>(0, 10) = -36.0f;
        kernel /= 429.0f;
        return kernel;
    }
    return kernel;
}

cv::Mat depth_clustering::visualzieDiffAngleImage()
{
    cv::Mat colors = cv::Mat::zeros(_depthImage.rows, _depthImage.cols, CV_8UC3);

    float max_dist = 20.0f;
    for (int r = 0; r < _depthImage.rows; ++r) 
    {
        for (int c = 0; c < _depthImage.cols; ++c) 
        {
            if (_depthImage.at<float>(r, c) < 0.01f) 
            {
                continue;
            }
            uint8_t row_color = 255 * (_depthImage.at<float>(r, c) / max_dist);
            uint8_t col_color = 255 * (_depthImage.at<float>(r, c) / max_dist);
            cv::Vec3b color(255 - row_color, 255 - col_color, 0);
            colors.at<cv::Vec3b>(r, c) = color;
        }   
    }
    return colors;
}


// cv::Mat depth_clustering::ZeroOutGroundBFS(const cv::Mat & image,
//                              const cv::Mat & angle_image,
//                              const float & threshold,
//                              int kernel_size) const
// {
//     cv::Mat res = cv::Mat::zeros(image.size(), CV_32F);
//     cv::Mat image_labeler = cv::Mat::zeros(image.size(), cv::DataType<uint16_t>::type);

//     for (int c = 0; c < image.cols; ++c)
//     {
//         int r = image.rows - 1;
//         while (r > 0 && image.at<float>(r, c) < 0.001f)
//         {
//             --r;
//         }

//         PixelCoord current_coord = PixelCoord(r, c);
//         uint16_t current_label = image_labeler.at<uint16_t>(current_coord.row, current_coord.col);

//         if (current_label > 0)
//         {
//             // 已经标记过了，直接跳过
//             continue;
//         }

//         LabelOneComponent(1, current_coord, angle_image);
//     }

//     kernel_size = std::max(kernel_size - 2, 3);
//     cv::Mat kernel = GetUniformKernel(kernel_size, CV_8U);
//     cv::Mat dilated = cv::Mat::zeros(image_labeler.size(), image_labeler.type);
//     cv::dilate(image_labeler, dilated, kernel);

//     for (int r = 0; r <dilated.rows; ++r)
//     {
//         for (int c = 0; c < dilated.cols; ++c)
//         {
//             if (dilated.at<uint16_t>(r, c) == 0)
//             {
//                 res.at<float>(r, c) = image.at<float>(r, c);
//             }
//         }
//     }

//     return res;
// }

void depth_clustering::LabelOneComponent(uint16_t label, 
                                         const PixelCoord & start,
                                         const cv::Mat & angle_image)
{

}

cv::Mat depth_clustering::GetUniformKernel(int window_size, int type) const 
{
    if (window_size % 2 == 0) {
    throw std::logic_error("only odd window size allowed");
    }
    cv::Mat kernel = cv::Mat::zeros(window_size, 1, type);
    kernel.at<float>(0, 0) = 1;
    kernel.at<float>(window_size - 1, 0) = 1;
    kernel /= 2;
    return kernel;
}

void depth_clustering::ComputeLabels()
{
    _label_image = cv::Mat::zeros(_depthImage.size(), cv::DataType<uint16_t>::type);
    uint16_t label = 1;
    for (int row = 0; row < _label_image.rows; ++row)
    {
        for (int col = 0; col < _label_image.cols; ++col)
        {
            if (_depthImage.at<float>(row, col) < 0.001f)
            {
                // depth is zero, not interested
                continue;
            }
            // else
            // {
            //     fprintf(stderr, "[%d, %d] = %f\n", row, col, _depthImage.at<float>(row, col));
            // }            

            if (_label_image.at<uint16_t>(row, col) > 0)
            {
                // has labeled;
                continue;
            }

            LabelOneComponent(label, PixelCoord(row, col));
            label++;
        }
    }
    // 统计有多少对象
    numCluster = (--label);
}

void depth_clustering::LabelOneComponent(uint16_t label, const PixelCoord & start)
{
    // fprintf(stderr, "[info] NeighborHood: \n");
    // for (auto step : Neighborhood)
    // {
    //     fprintf(stderr, "(%d, %d)\n", step.row, step.col);
    // }

    // fprintf(stderr, "[info] _angle_threshold %f\n", _angle_threshold);
    // fprintf(stderr, "current label %d\n", label);
    std::queue<PixelCoord> labeling_queue;
    labeling_queue.push(start);
    size_t max_queue_size = 0;
    while (!labeling_queue.empty())
    {
        max_queue_size = std::max(labeling_queue.size(), max_queue_size);
        const PixelCoord current = labeling_queue.front();
        labeling_queue.pop();
        uint16_t current_label = _label_image.at<uint16_t>(current.row, current.col);
        if (current_label > 0)
        {
            continue;
        }
        
        // set label
        _label_image.at<uint16_t>(current.row, current.col) = label;
        auto current_depth = _depthImage.at<float>(current.row, current.col);
        if (current_depth < 0.001f)
        {
            // no point here
            continue;
        }

        for (const auto & step : Neighborhood)
        {
            PixelCoord neighbor = current + step;
            if (neighbor.row < 0 || neighbor.row >= _label_image.rows)
            {
                // point does not fit
                continue;
            }

            neighbor.col = WrapCols(neighbor.col);
            uint16_t neigh_label = _label_image.at<uint16_t>(neighbor.row,  neighbor.col);
            if (neigh_label > 0)
            {
                // we have already labeled this one
                continue;
            }          

            auto diff = DiffAt(current, neighbor);
            // fprintf(stderr, "current coord [%d, %d](%f) --> [%d, %d](%f)\n", 
            //         current.row, current.col,_depthImage.at<float>(current.row, current.col), 
            //         neighbor.row, neighbor.col, _depthImage.at<float>(neighbor.row, neighbor.col));
            // fprintf(stderr, "current diff :%f\n", diff);
            if (diff > _angle_threshold)
            {
                labeling_queue.push(neighbor);
            }
        }
    }
}

int16_t depth_clustering::WrapCols(int16_t col) const 
{
    // we allow our space to fold around cols
    if (col < 0) 
    {
      return col + _label_image.cols;
    }
    if (col >= _label_image.cols) 
    {
      return col - _label_image.cols;
    }
    return col;
}

float depth_clustering::DiffAt(const PixelCoord & from, const PixelCoord & to) const
{
    const float & current_depth = _depthImage.at<float>(from.row, from.col);
    const float & neighbor_depth = _depthImage.at<float>(to.row, to.col);
    // fprintf(stderr, "current_depth : %f\n", current_depth);
    // fprintf(stderr, "neighbor_depth : %f\n", neighbor_depth);

    float cosAlpha;
    float sinAlpha;
    if (from.row == 0 && to.row == _params.numRows)
    {
        sinAlpha = 0;
        cosAlpha = 1;
    }
    else if (from.row != to.row)
    {
        // fprintf(stderr, "row changed\n");
        sinAlpha = sinRowAlpha;
        cosAlpha = cosRowAlpha; 
        // if (from.row < to.row)
        // {
        //     sinAlpha = _row_angles_sines[from.row];
        //     cosAlpha = _row_angles_cosines[from.row];      
        // }
        // else
        // {
        //     sinAlpha = _row_angles_sines[to.row];
        //     cosAlpha = _row_angles_cosines[to.row];  
        // }
        
    }
    else
    {
        // fprintf(stderr, "col changed\n");
        sinAlpha = sinColAlpha;
        cosAlpha = cosColAlpha;
    }

    // fprintf(stderr, "sinAlpha : %f, cosAlpha : %f\n", sinAlpha, cosAlpha);
    float d1 = std::max(current_depth, neighbor_depth);
    float d2 = std::min(current_depth, neighbor_depth);

    float beta = std::atan2((d2 * sinAlpha), (d1 - d2 * cosAlpha));
    // fprintf(stderr, "beta : %f\n", beta);
    return fabs(beta);
}

cv::Mat depth_clustering::visSegmentImage()
{
    cv::Mat color_image(_label_image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    for (int row = 0; row < _label_image.rows; ++row)
    {
        for (int col = 0; col < _label_image.cols; ++col)
        {
            auto label = _label_image.at<uint16_t>(row, col);
            auto random_color = _params.RANDOM_COLORS[label % _params.RANDOM_COLORS.size()];
            cv::Vec3b color = cv::Vec3b(random_color[0], random_color[1], random_color[2]);
            color_image.at<cv::Vec3b>(row, col) = color;
        }
    }

    cv::flip(color_image, color_image, 1);
    return color_image;
}

void depth_clustering::LabelCloud(Cloud & cloud)
{
    for (int idx = 0; idx < cloud.size(); ++idx)
    {
        // 该点的坐标， 向 _label_image 找索引
        auto cood = pointToImageCoord[idx];
        cloud[idx].classID = _label_image.at<uint16_t>(cood.first, cood.second);
    }

    if (cloud.size() < 200)
    {
        for(int idx = 0; idx < cloud.size(); ++idx)
        {
            fprintf(stderr, "point id(%d) ---> (%f, %f, %f)(%d, %d)[%d], angle[%f]\n", idx, 
                        cloud[idx].x(), cloud[idx].y(), cloud[idx].z(),
                        pointToImageCoord[idx].first, pointToImageCoord[idx].second,
                        cloud[idx].classID,
                        std::atan2(cloud[idx].y(), cloud[idx].x()) / M_PI * 180);
        }
    }
}
