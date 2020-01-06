#ifndef SRC_DRAWABLECLOUD_H
#define SRC_DRAWABLECLOUD_H
#include "groundRemove/include/param.h"

#include "drawable.h"
#include <memory>

// 绘制点云
class DrawableCloud : public Drawable
{
public:
    using Ptr = std::shared_ptr<DrawableCloud>;
    explicit DrawableCloud(const Cloud::ConstPtr& cloud,
                            const Eigen::Vector3f& color = Eigen::Vector3f::Ones(),
                            const GLfloat & pointSize = 1.0,
                            const int numCluster = -1)
            : _cloud_ptr{cloud}, _color{color}, _pointSize(pointSize), _numCluster(numCluster){}
    
    void Draw() const override;

    static DrawableCloud::Ptr FromCloud(
      const Cloud::ConstPtr& cloud,
      const Eigen::Vector3f& color = Eigen::Vector3f::Ones(),
      const GLfloat & pointSize = 1.0,
      const int numCluster = -1);

    ~DrawableCloud() override {}

private:
    int _numCluster = -1;
    GLfloat _pointSize = 1.0;
    Cloud::ConstPtr _cloud_ptr = nullptr;
    Eigen::Vector3f _color = Eigen::Vector3f::Ones();
    params _param = params();
};
#endif