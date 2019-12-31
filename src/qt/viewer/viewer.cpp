#include "viewer.h"
#include <QDebug>
using std::mutex;
using std::lock_guard;

void Viewer::AddDrawable(Drawable::Prt drawable)
{
    // fprintf(stderr, "Viewer::AddDrawable()\n");
    lock_guard<mutex> guard(_cloud_mutex);
    _drawables.push_back(drawable);
}

void Viewer::Clear()
{
    lock_guard<mutex> guard(_cloud_mutex);
    _drawables.clear();
}

void Viewer::draw() 
{
    // qDebug() << "window clicked .\n" << endl;
    // fprintf(stderr, "Viewer::draw()\n");
    lock_guard<mutex> guard(_cloud_mutex);
    for (auto& drawable : _drawables) 
    {
        drawable->Draw();
    }
}

// 显示按压事件
void Viewer::mousePressEvent(QMouseEvent *e) 
{
  // Start selection. Mode is ADD with Shift key and TOGGLE with Alt key.
//   rectangle_ = QRect(e->pos(), e->pos());
    // qDebug() << e->pos() << endl;
    QGLViewer::mousePressEvent(e);
}

void Viewer::init()
{
    // fprintf(stderr, "Viewer::init()\n");
    setSceneRadius(100.0);
    setBackgroundColor(QColor(1, 1, 1));
    camera()->showEntireScene();
    glDisable(GL_LIGHTING);
}

void Viewer::postSelection(const QPoint &point)
{
    bool found;
    selectedPoint = camera()->pointUnderPixel(point, found);
    // qDebug() << "point (" << QString(point.x()) + "," + QString(point.y()) + ")" << endl;
    // std::cout << "selectedPoint " << selectedPoint << std::endl;
    // qDebug() <<"selectedPoint " << "(" + QString(selectedPoint.x) + "," +
    //                                      QString(selectedPoint.y) + "," +
    //                                      QString(selectedPoint.z) +")" << endl;

    fprintf(stderr, "(%f, %f , %f ) cilicked\n", selectedPoint.x, selectedPoint.y, selectedPoint.z);
}

void Viewer::getClickedPoint(double &x, double &y)
{
    // auto a = selectedPoint.x;
    // qreal c = selectedPoint.y;
    // x = reinterpret_cast<double>(selectedPoint.x);
    // y = reinterpret_cast<double>(selectedPoint.y);
    x = (double)(selectedPoint.x);
    y = (double)(selectedPoint.y);
}
