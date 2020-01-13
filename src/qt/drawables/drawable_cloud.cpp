#include "drawable_cloud.h"
#include <math.h>
void DrawableCloud::Draw() const
{
    // fprintf(stderr, "DrawableCloud::Draw()\n");
    if (!_cloud_ptr)
    {
        throw std::runtime_error("DrawableCloud has no cloud to draw.");
    }

    // fprintf(stderr, "DrawableCloud::Draw() after _cloud_ptr check\n");

    glPushMatrix();
    glPointSize(_pointSize);
    glBegin(GL_POINTS);
    // glColor3f(_color[0], _color[1], _color[2]);    
    // fprintf(stderr, "there has about %ld points\n", _cloud_ptr->size());
    // bool multiColor = ((*_cloud_ptr)[0].classID != -1);
    bool multiColor = (_numCluster != -1);
    for (const auto & point : _cloud_ptr->points())
    {
        if (!multiColor)
        {
            glColor3f(_color[0], _color[1], _color[2]);
        }
        else
        {
            int classID = point.classID % _param.RANDOM_COLORS.size();
            // fprintf(stderr, "classID[%d]\n", classID);
            glColor3f((float)_param.RANDOM_COLORS[classID][0] / 255,
                      (float)_param.RANDOM_COLORS[classID][1] / 255,
                      (float)_param.RANDOM_COLORS[classID][2] / 255
                    );
        }        
        auto real_point = point.AsEigenVector();
        // fprintf(stderr, "(%f, %f, %f)\n", real_point.x(), real_point.y(), real_point.z());
        glVertex3f(real_point.x(), real_point.y(), real_point.z());
    }
    glEnd();
    glPopMatrix();    
}

DrawableCloud::Ptr DrawableCloud::FromCloud(const Cloud::ConstPtr& cloud,
                                            const Eigen::Vector3f& color,
                                            const GLfloat & pointSize,
                                            const int numCluster)
{
    return std::make_shared<DrawableCloud>(DrawableCloud(cloud, color, pointSize, numCluster));
}
