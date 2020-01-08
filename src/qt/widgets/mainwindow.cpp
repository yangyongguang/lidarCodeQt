#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawable_cloud.h"
#include "drawable_line.h"
#include "drawable_selectable_cloud.h"

#include <QColor>
#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QUuid>
#include <QRadioButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent):
    // QWidget(parent),
    BaseViewerWidget(parent),
    ui(new Ui::MainWindow)
    // ui(new Ui::OpenGlFolderPlayer)
{
    infoTextEdit = new QTextEdit;
    this->playCloud = false;
    this->curr_data_idx = 0;
    ui->setupUi(this);
    connect(ui->openFolderBT, SIGNAL(released()), this, SLOT(onOpenFolderToRead()));
    connect(ui->DataIdxVSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderMovedTo(int)));
    connect(ui->playBT, SIGNAL(released()), this, SLOT(onPlayClouds()));
    connect(ui->resetBT, SIGNAL(released()), this, SLOT(onReset()));

    // auto it = ui->groundRB;
    // 定义的控件， 全部都在 ui 中直接找到
    connect(ui->cloudCB, SIGNAL(toggled(bool)), this, SLOT(onUpdateShow()));
    connect(ui->groundCB, SIGNAL(toggled(bool)), this, SLOT(onUpdateShow()));
    connect(ui->obstacleCB, SIGNAL(toggled(bool)), this, SLOT(onUpdateShow()));

    connect(ui->updatePB, SIGNAL(released()), this, SLOT(onUpdate()));

    connect(ui->paramDSB, SIGNAL(valueChanged(double)), this, SLOT(onParamSet()));
    connect(ui->clearSelectionPB, SIGNAL(released()), this, SLOT(onClearSelection()));
    connect(ui->girdNumSB, SIGNAL(valueChanged(int)), this, SLOT(onUpdateShow(int)));
    // connect(ui->infoTab->curr
    // auto it = ui->infoTab->widget(0)->findChild<QRadioButton>("groundRB");

    // connect()
    // 读取数据
    infoTextEdit->clear();
    infoTextEdit->setPlaceholderText("data log");
    infoTextEdit->setReadOnly(true);
    infoTextEdit->document()->setMaximumBlockCount(200);

    
    // ui->CloudViewer->setBackgroundColor(QColor(1.0f, 1.0f, 1.0f));    
    _viewer = ui->CloudViewer;
    // fprintf(stderr, "_viewer = ui->CloudViewer;\n");
    _viewer->installEventFilter(this);
    _viewer->setAutoFillBackground(true);

    ui->resetBT->setEnabled(false);
    ui->DataIdxSBox->setEnabled(false);
    ui->DataIdxVSlider->setEnabled(false);
    ui->playBT->setEnabled(false);

    // 设置尺寸百分比
    // infoshow 的
    // qDebug() << ui->width();
    // ui->infoTab->setTabText(0, "infoShow");
    // ui->infoTab->clear();
    ui->infoTab->insertTab(0, infoTextEdit, tr("infoShow"));
    ui->infoTab->setTabText(1, tr("choose"));    
    ui->infoTab->setTabText(2, tr("params"));
    ui->infoTab->setCurrentIndex(0);
    // 
    ui->cloudCB->setChecked(false);
    ui->groundCB->setChecked(false);
    ui->obstacleCB->setChecked(false);
    ui->insertCB->setChecked(false);
    ui->lineCB->setChecked(false); 
    ui->updatePB->setEnabled(false);

    // param adjust
    ui->paramDSB->setRange(-1000, 1000);
    ui->paramDSB->setSingleStep(0.1);
    ui->paramDSB->setDecimals(3);
    ui->clearSelectionPB->setEnabled(false);

    // 数据 序列显示
    ui->dataSeqSB->setValue(13);
    ui->dataSeqSB->setRange(0, 21);

    // 创建图像显示窗口
    ui->girdNumSB->setValue(400);
    ui->girdNumSB->setSingleStep(10);
    ui->girdNumSB->setRange(10, 1000);

    // int showImageGV_x = (ui->CloudViewer->width() - ui->showImageGV->width()) / 2;
    // ui->showImageGV->move(showImageGV_x, 0);
    // _graphView.reset(new QGraphicsView);
    //     //
    // _graphView->setStyleSheet("padding:0px;border:0px");
    dock_Image = new QDockWidget(tr("Image"), this);
    addDockWidget(Qt::TopDockWidgetArea,dock_Image);
    dock_Image->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dock_Image->setAllowedAreas(Qt::AllDockWidgetAreas);
    imgLabel = new QLabel(dock_Image);
    imgLabel->setScaledContents(true);
    dock_Image->setFloating(true);

    dock_cluster_image = new QDockWidget(tr("Imagecluster"), this);
    addDockWidget(Qt::TopDockWidgetArea,dock_cluster_image);
    dock_cluster_image->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dock_cluster_image->setAllowedAreas(Qt::AllDockWidgetAreas);
    cluster_image = new QLabel(dock_cluster_image);
    cluster_image->setScaledContents(true);
    dock_cluster_image->setFloating(true);

    dockshow_depth_image = new QDockWidget(tr("ImageDepth"), this);
    addDockWidget(Qt::TopDockWidgetArea, dockshow_depth_image);
    dockshow_depth_image->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dockshow_depth_image->setAllowedAreas(Qt::AllDockWidgetAreas);
    depth_image = new QLabel(dockshow_depth_image);
    depth_image->setScaledContents(true);
    dockshow_depth_image->setFloating(true);

    dockshow_depth_image2 = new QDockWidget(tr("ImageDepth2"), this);
    addDockWidget(Qt::TopDockWidgetArea, dockshow_depth_image2);
    dockshow_depth_image2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dockshow_depth_image2->setAllowedAreas(Qt::AllDockWidgetAreas);
    depth_image2 = new QLabel(dockshow_depth_image2);
    depth_image2->setScaledContents(true);
    dockshow_depth_image2->setFloating(true);

}

MainWindow::~MainWindow()
{
    // delete ui;
    delete infoTextEdit;
    delete dock_Image;
    delete imgLabel;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Right:
      ui->DataIdxSBox->setValue(ui->DataIdxSBox->value() + 1);
      break;
    case Qt::Key_Left:
      ui->DataIdxSBox->setValue(ui->DataIdxSBox->value() - 1);
      break;
  }
}

void MainWindow::onOpenFolderToRead()
{
    int dataSeq = ui->dataSeqSB->value();
    std::string kitti_img_dir = 
        "/media/yyg/YYG/lidarVisualization/data_odometry_color/dataset/sequences/" +
            std::to_string(dataSeq) + "/image_2";

    std::string kitti_velo_dir = 
        "/media/yyg/YYG/lidarVisualization/data_odometry_velodyne/dataset/sequences/" +
            std::to_string(dataSeq) + "/velodyne";

    // QString folder_name = QFileDialog::getExistingDirectory(this, "打开数据路径",
    // QString::fromStdString(_params.kitti_velo_dir));
    // QString folder_name = QString::fromStdString(_params.kitti_velo_dir);
    // if (folder_name.size() == 0) return; // no dir choosed
    //ui->infoTextEdit->append(QString::fromStdString("Picked path: " + folder_name.toStdString()));
    moveCursorToEnd();

    // qDebug() << "Picked path:" << folder_name;

    _file_names_velo.clear();
    _file_names_img.clear();

    // utils::ReadKittiFileByDir(folder_name.toStdString(), _file_names_velo);
    utils::ReadKittiFileByDir(kitti_velo_dir, _file_names_velo);
    utils::ReadKittiFileByDir(kitti_img_dir, _file_names_img);
    // utils::ReadKittiFileByDir(_params.kitti_img_dir, _file_names_img);
    std::sort(_file_names_velo.begin(), _file_names_velo.end()); // 对文件夹排序
    std::sort(_file_names_img.begin(), _file_names_img.end());
    numData = _file_names_velo.size();
    if (numData > 0)
    {
        infoTextEdit->append(QString::fromStdString(_file_names_velo[0]));
        infoTextEdit->append(QString::fromStdString(_file_names_img[0]));
    }
    moveCursorToEnd();
    ui->DataIdxVSlider->setMaximum(numData - 1);
    ui->DataIdxSBox->setMaximum(numData - 1);
    _viewer->update();
    ui->DataIdxSBox->setEnabled(true);
    ui->DataIdxVSlider->setEnabled(true);
    ui->playBT->setEnabled(true);
    ui->updatePB->setEnabled(true);
    ui->clearSelectionPB->setEnabled(true);
}

// onPlayClouds 函数会改变 VSlider 的值， 所以会触发控件对应的槽函数 onSliderMovedTo 
// 所以在这个函数里面读取点云 并且显示即可

void MainWindow::onPlayClouds()
{
    if (this->playCloud == false)
    {
        this->playCloud = true;
        ui->playBT->setText("stop");
    }
    else
    {
        ui->playBT->setText("start");
        this->playCloud = false;
    }

    for (int i = curr_data_idx; i < ui->DataIdxVSlider->maximum(); ++i)
    {
        if (!playCloud) return;
        curr_data_idx = i; // recselectObjectIDsord current data Idx
        ui->DataIdxVSlider->setValue(i);
        ui->CloudViewer->update();
        QApplication::processEvents();
    }
}

void MainWindow::onReset()
{
    ui->resetBT->setEnabled(false);
    curr_data_idx = 0;
    ui->DataIdxVSlider->setValue(curr_data_idx);
    ui->DataIdxSBox->setValue(curr_data_idx);
}

void MainWindow::onSliderMovedTo(int cloud_number)
{
    if(_file_names_velo.empty())
        return;
    this->curr_data_idx = cloud_number;
    ui->DataIdxSBox->setValue(curr_data_idx);
    ui->DataIdxVSlider->setValue(curr_data_idx);
    
    const auto &file_name = _file_names_velo[cloud_number];
    _cloud = utils::ReadKittiBinCloudByPath(file_name);
    infoTextEdit->append("read bin file from: " + QString::fromStdString(file_name));
    infoTextEdit->append("current frame is: " + QString::number(cloud_number));
    moveCursorToEnd();

    // 添加点云显示
    // _viewer->Clear();
    // _viewer->AddDrawable(DrawableCloud::FromCloud(_cloud));
    // _viewer->update();

    if (this->curr_data_idx == numData - 1)
        ui->resetBT->setEnabled(true);

    // 去地
    // fprintf(stderr, "<<<<<<<<<<<<<<<-------------------------------\n");
    SegmentaionNode groundRemove(params_groundRemove);
    // fprintf(stderr, "params_groundRemove.line_search_angle : %f\n", params_groundRemove.line_search_angle);
    // fprintf(stderr, "params_groundRemove.max_slope : %f\n", params_groundRemove.max_slope);
    

    // 查看点击的位置 跟新点击的位置
    // double poseX, poseY;
    // _viewer->getClickedPoint(poseX, poseY);
    // fprintf(stderr, "clicked(%f, %f)\n", poseX, poseY);
    // groundRemove.setClickedPoint(poseX, poseY);
    //////
    // 获取选择的 ID
    Cloud::Ptr debugCloud(new Cloud);
    if (_viewer->selection.size())
    {
        groundRemove.setSelectObjectID(_viewer->selection);

        // 开启 debug 模式
        for (const int & elem : _viewer->selection)
        {
            debugCloud->emplace_back((*_cloud)[elem]);
        }        
    }
    // 结束
    Cloud::Ptr ground_cloud(new Cloud);
    Cloud::Ptr obstacle_cloud(new Cloud);
    // Cloud ground_cloud, obstacle_cloud;
    
    groundRemove.scanCallBack(*_cloud, *ground_cloud, *obstacle_cloud);
    // fprintf(stderr, "------------------------------->>>>>>>>>>>>>>>\n");

    // 聚类 ------------------------------------------------------
    int numGrid = ui->girdNumSB->value();
    int roiM = 100, numCluster = 0, kernelSize = 3;
    cluster cluster(roiM, numGrid, numCluster, kernelSize, *obstacle_cloud,  _params);
    cluster.componentClustering();

    // depth_clustering -----------------------------------------
    // 开启调试模式
    if (debugCloud->size())
    {
        fprintf(stderr, "has choosed %d debug point\n", debugCloud->size());
    }
    else
    {
        debugCloud = obstacle_cloud;
    }
    // depth_clustering depthCluster(* _cloud);
    depth_clustering depthCluster(*debugCloud);
    depthCluster.depthCluster();
    // cv::Mat depthImage = depthCluster.getVisualizeDepthImage();
    cv::Mat visImage = depthCluster.visSegmentImage();
    cv::Mat depthImage = depthCluster.getVisualizeDepthImage();
    // 直接聚类
    

    // 添加点云显示
    _viewer->Clear();
    GLfloat pointSize(1.8);
    Cloud::Ptr insertCloud(new Cloud);
    cv::Mat visClusterImg;    
    cluster.getClusterImg(visClusterImg);
    cluster.makeClusteredCloud(*obstacle_cloud);
    // fprintf(stderr, "numClister :%d\n", cluster.getNumCluster());
    _viewer->drawSelectableCloud = DrawSelectAbleCloud(_cloud);
    if (ui->cloudCB->isChecked())
    {
        // _viewer->AddDrawable(DrawableCloud::FromCloud(_cloud));
        _viewer->AddDrawable(DrawSelectAbleCloud::FromCloud(_cloud));
        // 为 viewer 的 drawSelectableCloud 赋值
        // _viewer->selection.clear();
    }

    if (ui->groundCB->isChecked())
    {
        Eigen::Vector3f color;
        color << 0.0, 1.0, 0.0;
        _viewer->AddDrawable(DrawableCloud::FromCloud(ground_cloud, color, pointSize));
    }

    if (ui->obstacleCB->isChecked())
    {
        Eigen::Vector3f color;
        color << 1.0, 0.0, 0.0;
        _viewer->AddDrawable(DrawableCloud::FromCloud(obstacle_cloud, color, pointSize));
    }

    if (ui->clusterCB->isChecked())
    {
        Eigen::Vector3f color;
        color << 0.5, 0.5, 0.3;
        _viewer->AddDrawable(DrawableCloud::FromCloud(obstacle_cloud, color, pointSize, cluster.getNumCluster()));
    }
    // 是否显示插入点
    if (ui->insertCB->isChecked())
    {
        groundRemove.getInsertedPoint(*_cloud, *insertCloud);
        Eigen::Vector3f color;
        color << 0, 0, 1;
        GLfloat pointSize(3);
        _viewer->AddDrawable(DrawableCloud::FromCloud(insertCloud, color, pointSize));
    }

    // 可视化线段的点
    Cloud::Ptr lines_cloud(new Cloud);
    if (ui->lineCB->isChecked())
    {
        groundRemove.getLinesPoint(*lines_cloud);
        Eigen::Vector3f color;
        color << 1.0, 1.0, 0.0;
        _viewer->AddDrawable(DrawableLine::FromCloud(lines_cloud, color));
    }

    // 显示图像
    cv::Mat imgShowCV;
    utils::ReadKittiImageByPath(_file_names_img[cloud_number], imgShowCV);    
    
    cv::cvtColor(imgShowCV, imgShowCV, cv::COLOR_BGR2RGB);

    imgShowCV.resize(imgShowCV.rows, imgShowCV.cols);
    QImage qimage = utils::MatToQImage(imgShowCV);
    dock_Image->resize(qimage.width(), qimage.height());
    imgLabel->setPixmap(QPixmap::fromImage(qimage));
    imgLabel->resize(qimage.width(), qimage.height());

    QImage qimage_cluster = utils::MatToQImage(visClusterImg);
    dock_cluster_image->resize(qimage_cluster.width() , qimage_cluster.height());
    cluster_image->setPixmap(QPixmap::fromImage(qimage_cluster));
    cluster_image->resize(qimage_cluster.width(), qimage_cluster.height());

    QImage qimage_depth = utils::MatToQImage(visImage);
    dockshow_depth_image->resize(qimage_depth.width() * 2, qimage_depth.height() * 2);
    depth_image->setPixmap(QPixmap::fromImage(qimage_depth));
    depth_image->resize(qimage_depth.width() * 2, qimage_depth.height() * 2);

    QImage qimage_depth2 = utils::MatToQImage(depthImage);
    dockshow_depth_image2->resize(qimage_depth2.width() * 2, qimage_depth2.height() * 2);
    depth_image2->setPixmap(QPixmap::fromImage(qimage_depth2));
    depth_image2->resize(qimage_depth2.width() * 2, qimage_depth2.height() * 2);
    _viewer->update();

}

void MainWindow::moveCursorToEnd()
{
    QTextCursor cursor = infoTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    infoTextEdit->setTextCursor(cursor);
    infoTextEdit->append("---------------------------------------------------");
}


void MainWindow::onUpdateShow()
{
    // if (ui->obstacleCB->isChecked() || ui->groundCB->isChecked())
    //     ui->cloudCB->setChecked(false);
    // if (ui->cloudCB->isChecked())
    // {
    //     ui->groundCB->setChecked(false);
    //     ui->obstacleCB->setChecked(false);
    // }
    
    // qDebug() << "some this occur" << endl;
    
    for (int i = curr_data_idx; i < ui->DataIdxVSlider->maximum(); ++i)
    {
        if (!playCloud) return;
        curr_data_idx = i; // record current data Idx
        ui->DataIdxVSlider->setValue(i);
        ui->CloudViewer->update();
        QApplication::processEvents();
    }
}

void MainWindow::onUpdateShow(int num)
{
    onSliderMovedTo(curr_data_idx);
}

void MainWindow::onUpdate()
{
    onSliderMovedTo(curr_data_idx);
}


void MainWindow::onParamSet()
{
    // fprintf(stderr, "current value %f", ui->paramDSB->value());
    // fprintf(stderr, "   curren paramID %d", ui->paramSB->value());
    int paramID = ui->paramSB->value();
    double paramValue = ui->paramDSB->value();
    switch (paramID)
    {
        case (0): params_groundRemove.line_search_angle = paramValue;           break;
        case (1): params_groundRemove.max_slope = paramValue;                   break; 
        case (2): params_groundRemove.tHmin = paramValue;                       break; 
        case (3): params_groundRemove.tHmax = paramValue;                       break; 
        case (4): params_groundRemove.tHDiff = paramValue;                      break; 
        case (5): params_groundRemove.hSensor = paramValue;                     break; 
        case (6): params_groundRemove.r_min_bin = paramValue;                   break; 
        case (7): params_groundRemove.r_max_bin = paramValue;                   break; 
        case (8): params_groundRemove.r_min_square = paramValue;                break; 
        case (9): params_groundRemove.r_max_square = paramValue;                break; 
        case (10): params_groundRemove.n_bins = paramValue;                     break; 
        case (11): params_groundRemove.n_segments = paramValue;                 break; 
        // case (12):
        case (13):params_groundRemove.max_dist_to_line = paramValue;            break; 
        case (14):params_groundRemove.visualize = paramValue;                   break;
        case (15): params_groundRemove.max_error_square = paramValue;           break; 
        case (16): params_groundRemove.long_threshold = paramValue;             break; 
        case (17): params_groundRemove.max_long_height = paramValue;            break; 
        case (18): params_groundRemove.max_start_height = paramValue;           break; 
        case (19): params_groundRemove.sensor_height = paramValue;              break; 
        // case (20): 
        // case (21):
        case (22):params_groundRemove.min_split_dist = paramValue;              break; 
        case (23):params_groundRemove.theta_start = paramValue;                 break; 
        case (24): params_groundRemove.theta_end = paramValue;                  break; 
        case (25): params_groundRemove.angle_resolution = paramValue;           break; 
        default:
            break;
    }
    // update show
    onUpdate();
}

void MainWindow::onClearSelection()
{
    // fprintf(stderr, "current elsection id :\n");
    // for (auto & elem : _viewer->selection) fprintf(stderr, "\n%d ",elem);
    _viewer->selection.clear();
}


// 为了窗口跟随， 移动到中间， 重写的事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
    dock_Image->resize(imgLabel->width(), imgLabel->height());
    dock_cluster_image->resize(cluster_image->width(), cluster_image->height());
    dockshow_depth_image->resize(depth_image->width(), depth_image->height());
    // int showImageGV_x = (ui->CloudViewer->width() - ui->showImageGV->width()) / 2;
    // ui->showImageGV->move(showImageGV_x, 0);

}