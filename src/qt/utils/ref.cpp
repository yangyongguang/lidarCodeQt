//
// Created by phyorch on 24/10/18.
//

#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core_c.h"
#include <boost/format.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace std;

string input_path = "/home/phyorch/MasterResearch/Data/2011_09_26/data/2011_09_26_drive_0048_sync/velodyne_points/data/0000000020.bin";
string image_path = "/home/phyorch/MasterResearch/Data/2011_09_26/data/2011_09_26_drive_0048_sync/image_02/data/0000000020.png";
string output_path = "/home/phyorch/MasterResearch/Data/0000000000.pcd";
string depth_map_path = "/home/phyorch/MasterResearch/Data/depth_map.jpg";
string depth_image_path = "/home/phyorch/MasterResearch/Data/depth_image.jpg";







void readKittiPclBinData(std::string &in_file, std::string& out_file) {
    // load point cloud
    std::fstream input(in_file.c_str(), std::ios::in | std::ios::binary);
    if (!input.good()) {
        std::cerr << "Could not read file: " << in_file << std::endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, std::ios::beg);

    pcl::PointCloud<pcl::PointXYZI>::Ptr points(new pcl::PointCloud<pcl::PointXYZI>);

    int i;
    for (i = 0; input.good() && !input.eof(); i++) {
        pcl::PointXYZI point;
        input.read((char *) &point.x, 3 * sizeof(float));
        input.read((char *) &point.intensity, sizeof(float));
        if (i >= 250111 && i < 255000) {
            cout << i << "   " << point.x << "  " << point.y << "  " << point.z << "    " << point.intensity << endl;
        }
        points->push_back(point);
    }
    input.close();
//    g_cloud_pub.publish( points );

    std::cout << "Read KTTI point cloud with " << i << " points, writing to " << out_file << std::endl;
    pcl::PCDWriter writer;

    // Save DoN features
    writer.write<pcl::PointXYZI>(out_file, *points, false);
}


void projectPoint(cv::Mat &depth_map, pcl::PointXYZI &point, Eigen::Matrix3f &R, Eigen::Vector3f &t){
    float fx = 984.2439;
    float fy = 980.8141;
    float cx = 690;
    float cy = 233.1966;
    Eigen::Vector3f p(point.x, point.y, point.z);
    //Eigen::Matrix3f R_ = R.transpose();
    //Eigen::Vector3f t_ = -R_ * t;
    Eigen::Vector3f p_transfered = R * p + t;
    //Eigen::Vector3f p_transfered = R * p + t;
    int u = int(fx / p_transfered[2] * p_transfered[0] + cx);
    int v = int(fy / p_transfered[2] * p_transfered[1] + cy);
    if(0<=u && 0<=v && u<depth_map.cols && v<depth_map.rows){
        //cout << "Point " << test << "is a valid point." << v << " " << u << endl;
        depth_map.at<uchar>(v,u) = sqrt(point.x*point.x + point.y*point.y);  //255
        //cout << depth_map.at<float>(v,u);
    }
}


void projectData(std::string &in_file, std::string& out_file, cv::Mat &depth_map, Eigen::Matrix3f R, Eigen::Vector3f t) {
    // load point cloud
    std::fstream input(in_file.c_str(), std::ios::in | std::ios::binary);
    if (!input.good()) {
        std::cerr << "Could not read file: " << in_file << std::endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, std::ios::beg);

    pcl::PointCloud<pcl::PointXYZI>::Ptr points(new pcl::PointCloud<pcl::PointXYZI>);

    int i;
    for (i = 0; input.good() && !input.eof(); i++) {
        pcl::PointXYZI point;
        input.read((char *) &point.x, 3 * sizeof(float));
        input.read((char *) &point.intensity, sizeof(float));
        projectPoint(depth_map, point, R, t);
//        if(i>80000 && i<85000){
//            projectPoint(depth_map, point, R, t);
//            //cout << point.x << "  " << point.y << "  " << point.z << "  " << point.intensity << endl;
//        }

    }
    input.close();

    std::cout << "Read KTTI point cloud with " << i << " points, writing to " << out_file << std::endl;
}



void projectPoint_kitti(cv::Mat &depth_map, pcl::PointXYZI &point, Eigen::Matrix4f T, Eigen::Matrix4f R, Eigen::Matrix<float,3,4> P){
    Eigen::Vector4f p(point.x, point.y, point.z, 1);
    Eigen::Vector4f p_ = T * p;
    Eigen::Vector4f p__ = R * p_;
    Eigen::Vector3f p_projected = P * p__;
//    p_projected[0] = p_projected[0] / p_projected[2];
//    p_projected[1] = p_projected[1] / p_projected[2];
    int u = int(p_projected[0] / p_projected[2]);
    int v = int(p_projected[1] / p_projected[2]);
    if(0<=u && 0<=v && u<depth_map.cols && v<depth_map.rows){
        //cout << "Point " << test << "is a valid point." << v << " " << u << endl;
        depth_map.at<uchar>(v,u) = sqrt(point.x*point.x + point.y*point.y);  //sqrt(point.x*point.x + point.y*point.y)
    }
}

void projectData_kitti(std::string &in_file, std::string& out_file, cv::Mat &depth_map, Eigen::Matrix4f T, Eigen::Matrix4f R, Eigen::Matrix<float,3,4> P) {
    // load point cloud
    std::fstream input(in_file.c_str(), std::ios::in | std::ios::binary);
    if (!input.good()) {
        std::cerr << "Could not read file: " << in_file << std::endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, std::ios::beg);

    pcl::PointCloud<pcl::PointXYZI>::Ptr points(new pcl::PointCloud<pcl::PointXYZI>);

    int i;
    for (i = 0; input.good() && !input.eof(); i++) {
        pcl::PointXYZI point;
        input.read((char *) &point.x, 3 * sizeof(float));
        input.read((char *) &point.intensity, sizeof(float));
        projectPoint_kitti(depth_map, point, T, R, P);

    }
    input.close();

    std::cout << "Read KTTI point cloud with " << i << " points, writing to " << out_file << std::endl;
}




void colorTransfer(cv::Mat& depth_map, cv::Mat& depth_image)
{
    // ת��Ϊα��ɫͼ�� �� �Ҷ�ͼ��
    for (int y = 0; y<depth_map.rows; y++)
    {
        for (int x = 0; x<depth_map.cols; x++)
        {
            uchar val = depth_map.at<uchar>(y, x);
            uchar r, g, b;
            if(val==0){
                r = g = b = 0;
            }
            else if(val<=10){
                r = 220;
                g = 20;
                b = 60;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else if(val<=15){
                r = 255;
                g = 255;
                b = 3;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else if(val<=20){
                r = 173;
                g = 255;
                b = 47;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else if(val<=35){
                r = 10;
                g = 255;
                b = 20;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else if(val<=50){
                r = 0;
                g = 220;
                b = 120;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else if(val<=60){
                r = 0;
                g = 100;
                b = 200;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
            else{
                r = 0;
                g = 0;
                b = 255;
                depth_image.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
            }
        }
    }
}



int main() {
    cv::Mat depthImage = cv::imread(image_path);
    cv::Mat depthMap = cv::Mat::zeros(depthImage.rows, depthImage.cols, CV_8UC1);
    //Simple model
//    Eigen::Matrix3f Rotation;
//    Rotation << 7.533745e-03, -9.999714e-01, -6.166020e-04,
//            1.480249e-02, 7.280733e-04, -9.998902e-01,
//            9.998621e-01, 7.523790e-03, 1.480755e-02;
//    Eigen::Vector3f translation(-4.069766e-03, -7.631618e-02, -2.717806e-01);
//    projectData(input_path, output_path, depthMap, Rotation, translation);
//    cv::Mat depthMapNormalized = depthMap;
//    cv::normalize(depthMap, depthMapNormalized);
//    depthMapNormalized = depthMapNormalized * 255;

    //cv::imwrite(depth_map_path, depthMap);

    //KITTI_model
    Eigen::Matrix4f velo_to_cam;
    velo_to_cam << 7.533745e-03, -9.999714e-01, -6.166020e-04, -4.069766e-03,
                   1.480249e-02, 7.280733e-04, -9.998902e-01, -7.631618e-02,
                   9.998621e-01, 7.523790e-03, 1.480755e-02, -2.717806e-01,
                   0, 0, 0, 1;
    Eigen::Matrix4f R_rect_00_extended;
    R_rect_00_extended << 9.999239e-01, 9.837760e-03, -7.445048e-03, 0,
                          -9.869795e-03, 9.999421e-01, -4.278459e-03, 0,
                          7.402527e-03, 4.351614e-03, 9.999631e-01, 0,
                          0, 0, 0, 1;
    Eigen::Matrix<float,3,4> P_rect_00;
    P_rect_00 << 7.215377e+02, 0.000000e+00, 6.095593e+02, 0.000000e+00,
                 0.000000e+00, 7.215377e+02, 1.728540e+02, 0.000000e+00,
                 0.000000e+00, 0.000000e+00, 1.000000e+00, 0.000000e+00;
    projectData_kitti(input_path, output_path, depthMap, velo_to_cam, R_rect_00_extended, P_rect_00);


    //depthMap = depthMap * 3;
    cout << depthMap;
    colorTransfer(depthMap, depthImage);
    cv::imwrite(depth_map_path, depthMap);
    cv::imwrite(depth_image_path, depthImage);

    //readKittiPclBinData(input_path, output_path);
    return 0;
}

/*we need to consider the sky region*/

/*
angle:
0.519409  0.554355  0.455710  0.400444  0.506621  0.442916  0.568372  0.526753  0.605722  0.365441  0.502743  0.434637
  0.568572  0.522340  0.529855  0.413597  0.453218  0.422827  0.499179  0.598111  0.455398  0.388137  0.455406  0.518613
    0.596510  0.370399  0.227018  0.344466  0.355029  0.259436  0.293565  0.314716  0.338815  0.316818  0.317888  0.307076
      0.292745  0.335993  0.253066  0.359625  0.272176  0.328757  0.293442  0.307917  0.336700



0.528240  0.545307  0.461287  0.404318  0.516412  0.452729  0.563052  0.523874  0.613170  0.349830  0.489929  0.450202
  0.567904  0.526212  0.522588  0.431466  0.453852  0.423610  0.489888  0.601413  0.461380  0.385362  0.456275  0.514867
    0.582329  0.365128  0.225443  0.344738  0.355610  0.259598  0.290330  0.314952  0.333940  0.316013  0.320810  0.306619
      0.288632  0.337874  0.251419  0.357611  0.269216  0.325939  0.290026

0.471216  0.530048  0.420359  0.395039  0.501879  0.454710  0.538899  0.533521  0.609379  0.353387  0.488925  0.429639
  0.569384  0.509305  0.514227  0.419583  0.441462  0.422148  0.492471  0.601205  0.462132  0.385176  0.465557  0.515808
    0.607242  0.368220  0.230933  0.339150  0.359365  0.254081  0.296568  0.313916  0.342032  0.311826  0.321368  0.307435
      0.294938  0.339115  0.254194  0.357622  0.273151  0.332381  0.297391  0.311999  0.341062  0.312385  0.265392  

[-0.341494]0.512099  [-0.331712]0.558959  [-0.323768]0.463857  [-0.316732]0.408054  [-0.307989]0.513062  [-0.300167]0.446368  [-0.290541]0.551300  [-0.281568]0.519209  [-0.271106]0.603432
  [-0.264962]0.349470  [-0.256502]0.484047  [-0.248877]0.436573  [-0.238951]0.572880  [-0.229699]0.526993  [-0.220553]0.532118  [-0.213127]0.430349  [-0.205342]0.451635  [-0.197954]0.427629
    [-0.189399]0.492607  [-0.178790]0.606210  [-0.170844]0.458342  [-0.164184]0.383837  [-0.156088]0.464122  [-0.147126]0.516227  [-0.136825]0.586713  [-0.130379]0.368329  [-0.126350]0.229574
      [-0.120407]0.342554  [-0.114073]0.361494  [-0.109572]0.260142  [-0.104383]0.296610  [-0.098908]0.313576  [-0.092954]0.341952  [-0.087429]0.317449  [-0.081771]0.324551  [-0.076436]0.306710
        [-0.071323]0.291337  [-0.065417]0.339492  [-0.060998]0.255171  [-0.054728]0.359003  [-0.049987]0.271624  [-0.044252]0.328628  [-0.039141]0.292793  [-0.033724]0.310402  [-0.027839]0.337170

[-19.530800]0.516282  [-18.992617]0.546746  [-18.538747]0.454776  [-18.128089]0.415429  [-17.622342]0.512300  [-17.178595]0.445899  [-16.617043]0.571301  [-16.092655]0.528351  [-15.486045]0.608852
  [-15.129757]0.359674  [-14.644504]0.488912  [-14.202321]0.437591  [-13.631266]0.572330  [-13.111871]0.518735  [-12.591601]0.530364  [-12.162716]0.423053  [-11.714491]0.455992  [-11.291585]0.422122
    [-10.802301]0.495089  [-10.194973]0.606102  [-9.741399]0.453005  [-9.350552]0.393004  [-8.893289]0.461565  [-8.377329]0.516638  [-7.765063]0.614011  [-7.399145]0.371004  [-7.173808]0.224898
      [-6.832938]0.341843  [-6.472357]0.359798  [-6.215404]0.260412  [-5.922334]0.293273  [-5.608117]0.315562  [-5.275520]0.334164  [-4.960220]0.313966  [-4.641166]0.318414  [-4.333412]0.305659
        [-4.045739]0.289804  [-3.708444]0.336226  [-3.458297]0.250816  [-3.100813]0.357484  [-2.830285]0.270528  [-2.503695]0.326590  [-2.212706]0.290989  [-1.907757]0.304949  [-1.571566]0.336191

[-19.545556]0.500096  [-18.995547]0.544013  [-18.546504]0.448675  [-18.137246]0.421395  [-17.634532]0.514723  [-17.194810]0.454908  [-16.639097]0.553872  [-16.109525]0.532061  [-15.503390]0.612572
  [-15.153804]0.351763  [-14.650945]0.505685  [-14.221010]0.441395  [-13.656969]0.566901  [-13.133008]0.522836  [-12.612541]0.527937  [-12.189212]0.426267  [-11.740696]0.454376  [-11.323422]0.422326
    [-10.831308]0.494111  [-10.233282]0.595756  [-9.776247]0.453150  [-9.388275]0.387991  [-8.930320]0.461204  [-8.416492]0.515439  [-7.828575]0.587896  [-7.462132]0.370369  [-7.239918]0.224607
      [-6.896839]0.342207  [-6.537227]0.358775  [-6.280839]0.258870  [-5.987299]0.292594  [-5.674338]0.313923  [-5.340890]0.333340  [-5.027126]0.314439  [-4.707090]0.319358  [-4.401970]0.304927
        [-4.114132]0.288369  [-3.777979]0.336199  [-3.526258]0.252573  [-3.169436]0.357140  [-2.898840]0.272176  [-2.571641]0.326444  [-2.282581]0.290067  [-1.978164]0.304610

[-22.800442]0.475149  [-22.290271]0.526843  [-21.671191]0.625799  [-21.225486]0.450319  [-20.860130]0.372863  [-20.376662]0.478902  [-19.785677]0.587946  [-19.285449]0.508192  [-18.754026]0.542947
  [-18.323465]0.445875  [-17.929574]0.401485  [-17.423628]0.511871  [-16.992875]0.426651  [-16.440948]0.546886  [-15.927902]0.515387  [-15.335501]0.593694  [-14.976660]0.358361  [-14.498014]0.483572
    [-14.068834]0.432151  [-13.509837]0.561121  [-13.003437]0.505155  [-12.487579]0.517284  [-12.072455]0.418621  [-11.617510]0.459036  [-11.201443]0.421371  [-10.715049]0.488978  [-4.308792]6.403480
      [-6.004535]0.774076  [-6.005191]1.432227  [-6.777494]3.347411  [-7.030611]3.090130  [-7.034746]3.086543  [-4.323399]2.711347  [-4.676661]1.721364  [-4.675299]2.765104  [-4.328321]3.109769
        [-5.047713]3.145571  [-5.053859]3.140072  [-3.975426]4.219986  [-3.650835]0.330000  [-2.964405]0.687784  [-2.302029]7.368922  [-1.635846]8.039659  [-1.336102]0.302299  [-1.035284]8.257282  [-3.104930]5.734458
*/

