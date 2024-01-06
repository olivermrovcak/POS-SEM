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
public:
    DownloadManager();
    ~DownloadManager();
    void addDownload(std::shared_ptr<Download> download); // Use shared_ptr for memory management
    void removeDownload(Download download);

    std::vector<std::string> getDownloadStatuses() {
        std::vector<std::string> statuses;
        for (auto& download : downloads) {
            statuses.push_back("30");
        }
        return statuses;
    }



};


#endif //POS_SEM_DOWNLOADMANAGER_H
