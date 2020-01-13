#ifndef SRC_QT_UTILS_CLOUD_H
#define SRC_QT_UTILS_CLOUD_H
#include <iostream>

#include <algorithm>
#include <list>
#include <vector>
#include <Eigen/Core>
#include <memory>

class point{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    point() {_point = Eigen::Vector3f::Zero();_intensity = 0;}
    explicit point(float x, float y, float z): _point(x, y, z) {}
    // explicit point(const point & pt);

    virtual ~point(){}

    // 不需要改值的声明
    inline float x() const { return _point.x(); }
    inline float y() const { return _point.y(); }
    inline float z() const { return _point.z(); }
    inline float i() const { return _intensity; }

    // 需要改值的申明
    inline float& x() { return _point.x(); }
    inline float& y() { return _point.y(); }
    inline float& z() { return _point.z(); }
    inline float& i() { return _intensity; }

    inline const Eigen::Vector3f& AsEigenVector() const {return _point; }
    inline Eigen::Vector3f& AsEigenVector() { return _point; }

    void operator=(const point& other);
    void operator=(const Eigen::Vector3f& other);
    bool operator==(const point& other) const;
public:
    int classID = -1;
private:
    Eigen::Vector3f _point;
    float _intensity;
};

class Cloud{
public:
    Cloud(){}
    // explicit Cloud(const Cloud & cloud) {}
    // explicit Cloud(Cloud & cloud) {}
    inline size_t size() const { return _points.size(); }
    inline bool empty() const { return _points.empty(); }
    inline void reserve(size_t size) { _points.reserve(size); }
    inline void push_back(const point& pt) { _points.push_back(pt); }
    inline void emplace_back(const point& pt) {_points.emplace_back(pt);}
    inline point & operator[](int idx) {return _points[idx];}
    inline const point & operator[](int idx) const { return _points[idx];}

    inline const std::vector<point> & points() const {return _points;}
    
    // inline std::vector<point>::iterator begin() {return _points.begin();}
    // inline std::vector<point>::iterator end() {return _points.end();}
    inline std::vector<point>::iterator begin() {return _points.begin();}
    inline std::vector<point>::iterator end() {return _points.end();}

    // std::vector<point> _points;
    inline void clear(){_points.clear();}

    typedef std::shared_ptr<Cloud> Ptr;
    typedef std::shared_ptr<const Cloud> ConstPtr;
private:
    std::vector<point> _points;
};
#endif