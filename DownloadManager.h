//
// Created by Oliver Mrovcak on 05/01/2024.
//

#ifndef POS_SEM_DOWNLOADMANAGER_H
#define POS_SEM_DOWNLOADMANAGER_H


#include <vector>
#include "Download.h"
#include "thread"
#include "Settings.h"

class DownloadManager {
private:
    std::vector<std::shared_ptr<Download>> downloads;
    std::vector<std::shared_ptr<Download>> scheduledDownloads;
    std::vector<std::thread> threads;
    std::mutex downloadMutex;
public:
    DownloadManager();
    ~DownloadManager();
    void addDownload(std::shared_ptr<Download> download);
    void cleanupCompletedDownloads();
    std::vector<std::shared_ptr<Download>> getDownloads();
    void loadSchedule();
    void scheduleDownload(std::string hostname, std::string savePath, std::string downloadPath, std::string username, std::string password, int priority, std::string protocol, std::string time);
    void removeDownload(Download download);
    void addScheduledDownload(std::shared_ptr<Download> download);
    void removeScheduledDownload(Download download);
    void checkAndInitiateScheduledDownloads();
    void saveDownload(std::string url, std::string path, unsigned long size);
    void resumeDownload(int index);
    void pauseDownload(int index);
    int getDownloadCount();
};


#endif //POS_SEM_DOWNLOADMANAGER_H
