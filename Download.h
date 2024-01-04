//
// Created by Oliver Mrovcak on 04/01/2024.
//

#ifndef POS_SEM_DOWNLOAD_H
#define POS_SEM_DOWNLOAD_H

#include <string>
#include <iostream>


class Download {

    std::string protocol;
    std::string filename;
    std::string localFilename;
    std::string hostname;
    std::string path;
    std::string filepath;
    std::string username;
    std::string password;
    int priority;
    double size;
    double currentSize = 0;
    bool paused = false;

public:
    Download(std::string protocol, std::string hostname, std::string savePath, std::string downloadPath, std::string username, std::string password, int priority);
    ~Download();

};


#endif //POS_SEM_DOWNLOAD_H
