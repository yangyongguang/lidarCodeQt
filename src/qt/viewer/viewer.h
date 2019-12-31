#ifndef SRC_INCLUDE_VIEWER_H
#define SRC_INCLUDE_VIEWER_H

#include <memory>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLViewer/qglviewer.h>
#include <mutex>
#include <vector>
#include "drawable.h"

#include <QMouseEvent>
#include <QPoint>

class Viewer : public QGLViewer
{
    // Q_OBJECT  这里一定不能定义 Q_OBJECT 这个选项， 很容易报错、
    // 根据 depth_clustering 的逻辑来， 就不会报错
public:
    explicit Viewer(QWidget *parent = 0) : QGLViewer(parent) {}
    void AddDrawable(Drawable::Prt drawable);
    void Clear();
    ~Viewer() override {}
    void getClickedPoint(double & x, double & y);
protected:
    void draw() override;
    void init() override;

    virtual void mousePressEvent(QMouseEvent *e);
    // 寻找我们点击的点
    virtual void postSelection(const QPoint &point);

private:
    std::vector<Drawable::Prt> _drawables;
    mutable std::mutex _cloud_mutex;

    qglviewer::Vec selectedPoint;
};
#endif



