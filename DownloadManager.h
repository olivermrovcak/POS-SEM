//
// Created by Oliver Mrovcak on 05/01/2024.
//

#ifndef POS_SEM_DOWNLOADMANAGER_H
#define POS_SEM_DOWNLOADMANAGER_H


#include <vector>
#include "Download.h"
#include "thread"

class DownloadManager {
private:
    std::vector<std::shared_ptr<Download>> downloads; // Store pointers to Download objects
    std::vector<std::thread> threads; // Keep track of threads
    std::mutex downloadMutex;
public:
    DownloadManager();
    ~DownloadManager();
    void addDownload(std::shared_ptr<Download> download); // Use shared_ptr for memory management
    void cleanupCompletedDownloads();
    std::vector<std::shared_ptr<Download>> getDownloads();
    void removeDownload(Download download);
    void resumeDownload(int index);
    void pauseDownload(int index);
    int getDownloadCount();


};


#endif //POS_SEM_DOWNLOADMANAGER_H
