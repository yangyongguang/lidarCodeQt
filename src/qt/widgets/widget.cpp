#include "widget.h"
#include "ui_widget.h"
#include "drawable_cloud.h"
#include "drawable_line.h"

#include <QColor>
#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QUuid>
#include <QRadioButton>

Widget::Widget(QWidget *parent):
    // QWidget(parent),
    BaseViewerWidget(parent),
    ui(new Ui::Widget)
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
    // connect(ui->infoTab->curr
    // auto it = ui->infoTab->widget(0)->findChild<QRadioButton>("groundRB");

    // connect()
    // 读取数据
    infoTextEdit->clear();
    infoTextEdit->setPlaceholderText("data log");
    infoTextEdit->setReadOnly(true);
    infoTextEdit->document()->setMaximumBlockCount(200);

    _viewer = ui->CloudViewer;
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
    ui->obstacleCB->setChecked(true);
    ui->insertCB->setChecked(false);
    ui->lineCB->setChecked(false); 
    ui->updatePB->setEnabled(false);

    // param adjust
    ui->paramDSB->setRange(-1000, 1000);
    ui->paramDSB->setSingleStep(0.1);
    ui->paramDSB->setDecimals(3);
}

Widget::~Widget()
{
    // delete ui;
    delete infoTextEdit;
}

void Widget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Right:
      ui->DataIdxSBox->setValue(ui->DataIdxSBox->value() + 1);
      break;
    case Qt::Key_Left:
      ui->DataIdxSBox->setValue(ui->DataIdxSBox->value() - 1);
      break;
  }
}

void Widget::onOpenFolderToRead()
{
    QString folder_name = QFileDialog::getExistingDirectory(this, "打开数据路径",
    QString::fromStdString(_params.kitti_velo_dir));
    if (folder_name.size() == 0) return; // no dir choosed
    //ui->infoTextEdit->append(QString::fromStdString("Picked path: " + folder_name.toStdString()));
    moveCursorToEnd();

    qDebug() << "Picked path:" << folder_name;

    _file_names_velo.clear();
    _file_names_img.clear();

    utils::ReadKittiFileByDir(folder_name.toStdString(), _file_names_velo);
    std::sort(_file_names_velo.begin(), _file_names_velo.end()); // 对文件夹排序
    numData = _file_names_velo.size();
    if (numData > 0)
        infoTextEdit->append(QString::fromStdString(_file_names_velo[0]));
    moveCursorToEnd();
    ui->DataIdxVSlider->setMaximum(numData - 1);
    ui->DataIdxSBox->setMaximum(numData - 1);
    _viewer->update();
    ui->DataIdxSBox->setEnabled(true);
    ui->DataIdxVSlider->setEnabled(true);
    ui->playBT->setEnabled(true);
    ui->updatePB->setEnabled(true);
}

// onPlayClouds 函数会改变 VSlider 的值， 所以会触发控件对应的槽函数 onSliderMovedTo 
// 所以在这个函数里面读取点云 并且显示即可

void Widget::onPlayClouds()
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
        curr_data_idx = i; // record current data Idx
        ui->DataIdxVSlider->setValue(i);
        ui->CloudViewer->update();
        QApplication::processEvents();
    }
}

void Widget::onReset()
{
    ui->resetBT->setEnabled(false);
    curr_data_idx = 0;
    ui->DataIdxVSlider->setValue(curr_data_idx);
    ui->DataIdxSBox->setValue(curr_data_idx);
}

void Widget::onSliderMovedTo(int cloud_number)
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
    fprintf(stderr, "<<<<<<<<<<<<<<<-------------------------------\n");
    SegmentaionNode groundRemove(params_groundRemove);

    // 查看点击的位置 跟新点击的位置
    double poseX, poseY;
    _viewer->getClickedPoint(poseX, poseY);
    fprintf(stderr, "clicked(%f, %f)\n", poseX, poseY);
    groundRemove.setClickedPoint(poseX, poseY);
    //////

    Cloud::Ptr ground_cloud(new Cloud);
    Cloud::Ptr obstacle_cloud(new Cloud);
    // Cloud ground_cloud, obstacle_cloud;
    
    groundRemove.scanCallBack(*_cloud, *ground_cloud, *obstacle_cloud);
    fprintf(stderr, "------------------------------->>>>>>>>>>>>>>>\n");
    // 添加点云显示
    _viewer->Clear();
    Cloud::Ptr insertCloud(new Cloud);
    if (ui->cloudCB->isChecked())
    {
        _viewer->AddDrawable(DrawableCloud::FromCloud(_cloud));
    }

    if (ui->groundCB->isChecked())
    {
        Eigen::Vector3f color;
        color << 0.0, 1.0, 0.0;
        _viewer->AddDrawable(DrawableCloud::FromCloud(ground_cloud, color));
    }

    if (ui->obstacleCB->isChecked())
    {
        Eigen::Vector3f color;
        color << 1.0, 0.0, 0.0;
        GLfloat pointSize(1.8);
        _viewer->AddDrawable(DrawableCloud::FromCloud(obstacle_cloud, color, pointSize));
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
    _viewer->update();

}

void Widget::moveCursorToEnd()
{
    QTextCursor cursor = infoTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    infoTextEdit->setTextCursor(cursor);
    infoTextEdit->append("---------------------------------------------------");
}

void Widget::onUpdateShow()
{
    if (ui->obstacleCB->isChecked() || ui->groundCB->isChecked())
        ui->cloudCB->setChecked(false);
    if (ui->cloudCB->isChecked())
    {
        ui->groundCB->setChecked(false);
        ui->obstacleCB->setChecked(false);
    }
    
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

void Widget::onUpdate()
{
    onSliderMovedTo(curr_data_idx);
}

void Widget::onParamSet()
{
    fprintf(stderr, "current value %f\n", ui->paramDSB->value());
}