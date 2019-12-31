#include "utils.h"
#include <boost/filesystem.hpp>
#include <vector>

namespace fs = boost::filesystem;
namespace utils
{

    void ReadKittiBinCloudByPath(const std::string & path, Cloud & cloud)
    {
        std::fstream file(path.c_str(), std::ios::in | std::ios::binary);
        if (file.good())
        {
            file.seekg(0, std::ios::beg);
            float intensity = 0;
            for (int i = 0; file.good() && !file.eof(); ++i)
            {
                point pt;
                file.read(reinterpret_cast<char*>(&pt.x()), sizeof(float));
                file.read(reinterpret_cast<char*>(&pt.y()), sizeof(float));
                file.read(reinterpret_cast<char*>(&pt.z()), sizeof(float));
                file.read(reinterpret_cast<char*>(&intensity), sizeof(float));
                cloud.emplace_back(pt);
            }
            file.close();
        }
    }
    Cloud::Ptr ReadKittiBinCloudByPath(const std::string & path)
    {
        Cloud::Ptr cloud(new Cloud);
        std::fstream file(path.c_str(), std::ios::in | std::ios::binary);
        if (file.good())
        {
            file.seekg(0, std::ios::beg);
            float intensity = 0;
            for (int i = 0; file.good() && !file.eof(); ++i)
            {
                point pt;
                file.read(reinterpret_cast<char*>(&pt.x()), sizeof(float));
                file.read(reinterpret_cast<char*>(&pt.y()), sizeof(float));
                file.read(reinterpret_cast<char*>(&pt.z()), sizeof(float));
                file.read(reinterpret_cast<char*>(&intensity), sizeof(float));
                cloud->emplace_back(pt);
            }
            file.close();
        }
        return cloud;
    }

    void ReadKittiImageByPath(const std::string & path, cv::Mat & img)
    {
        img = cv::imread(path);
        if (img.empty())
        {
            printf("no kitti image was readed!\n");
            return;
        }
    }

    void ReadKittiFileByDir(const std::string & dir, std::vector<std::string> &fileNames)
    {
        // printf("kitti velodyne path :%s\n", dir.c_str());
        fs::path velo_path(dir);
        fs::directory_iterator end;
        for (fs::directory_iterator fileIt(velo_path); fileIt != end; ++fileIt)
        {
            std::string filename =  fileIt->path().string();
            fileNames.emplace_back(filename);         
        }
    }
}