#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <iostream>
#include "groundRemove/include/param.h"
#include <QColor>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QFileDialog>
#include <QTextCursor>

#include "utils.h"
#include "groundRemove/include/cloud.h"

#include <QKeyEvent>

#include "viewer.h"
#include "base_viewer_widget.h"

// groundRemove 的代码
#include "groundRemove/include/groundRemove.h"
#include "groundRemove/include/component_clustering.h"

#include <QTextEdit>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace Ui {
class Widget;
}

// class Widget : public QWidget
class Widget : public BaseViewerWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    // log 光标移动到最后
    void moveCursorToEnd();
    // std::vector<std::string> fileNameBin;
    // std::vector<std::string> fileNameImage;
    Cloud cloud;

    // Viewer *_viewer = nullptr;
    // groundRemove 的参数
    GroundSegmentationParams params_groundRemove;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void onOpenFolderToRead();
    /**
     *  @param 
     *  @brief 播放和暂停
     */
    void onPlayClouds();
    void onSliderMovedTo(int cloud_number);
    void onReset();

    // 重新显示， 当改变形式的时候
    void onUpdateShow();
    void onUpdateShow(int num);
    void onUpdate();

    // 改变参数类型， 为了调整参数
    void onParamSet();

    // 清除选择的 ID
    void onClearSelection();

private:
    // Ui::Widget *ui;
    std::unique_ptr<Ui::Widget> ui;

    params _params;
    std::vector<std::string> _file_names_velo;
    std::vector<std::string> _file_names_img;
    
    Cloud::Ptr _cloud;

    // 是播放还是暂停 
    bool playCloud;

    // 当前数据索引
    size_t curr_data_idx;

    // 总计所少个数据
    size_t numData;

    // new widget
    QTextEdit *infoTextEdit = nullptr;

    // 显示图像
    std::unique_ptr<QGraphicsScene> _scene = nullptr;


    // 新建一个窗口试试
};

#endif // WIDGET_H
