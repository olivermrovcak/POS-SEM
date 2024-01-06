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
    std::vector<std::shared_ptr<Download>> getDownloads();
    void removeDownload(Download download);
    int getDownloadCount();

    std::vector<std::string> getDownloadStatuses() {
        std::vector<std::string> statuses;
        statuses.reserve(downloads.size());
        //reserve alokuje pamat pre vektor, aby sa nemusela alokovat pri kazdom pridani prvku
        for (auto& download : downloads) {
            statuses.push_back("30");
        }
        return statuses;
    }

};


#endif //POS_SEM_DOWNLOADMANAGER_H
