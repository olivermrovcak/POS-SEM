//
// Created by Oliver Mrovcak on 04/01/2024.
//

#ifndef POS_SEM_DOWNLOAD_H
#define POS_SEM_DOWNLOAD_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <regex>
#include <fstream>
#include <ncurses.h>


class Download {

private:
    std::string protocol;
    std::string hostname;
    std::string savePath;
    std::string downloadPath;
    std::string username;
    std::string password;

    int priority;
    unsigned long size = 0;
    unsigned long currentSize = 0;
    bool paused = false;
    bool completed = false;

    std::mutex mtx;
    std::condition_variable cv;

public:
    Download(std::string protocol, std::string hostname, std::string savePath, std::string downloadPath, std::string username, std::string password, int priority);
    ~Download();

    void start();
    void pause();
    void resume();
    void cancel();
    void restart();

    bool isPaused();
    bool isCompleted();

    void setPriority(int priority);
    void setFilename(std::string filename);
    void setSize(unsigned long size);
    void setCompleted(bool completed);

    std::string getNewFileName(const std::string& originalPath);

    const std::string &getUsername() const;
    const std::string &getPassword() const;
    const std::string &getHostname() const;
    const std::string &getSavePath() const;
    const std::string &getDownloadPath() const;
    const std::string &getProtocol() const;
    const std::string &getFilename() ;

    unsigned long getCurrentSize();
    unsigned long getSize();

    void ftpDownload();
    void ftpsDownload();
    void httpDownload();
    void httpsDownload();

    bool parse_pasv_response(const std::string &response, std::string &ip_address, unsigned short &port);
};


#endif //POS_SEM_DOWNLOAD_H
