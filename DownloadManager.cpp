//
// Created by Oliver Mrovcak on 05/01/2024.
//

#include <vector>
#include "Download.h"
#include "DownloadManager.h"

DownloadManager::DownloadManager() {

}

DownloadManager::~DownloadManager() {
    // Join all threads on destruction to ensure they complete
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void DownloadManager::addDownload(std::shared_ptr<Download> download) {
    // Start the download in a new thread
    std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector
    threads.emplace_back(&Download::start, download);
    downloads.push_back(download);
}

std::vector<std::shared_ptr<Download>> DownloadManager::getDownloads() {
    std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector
    return downloads;
}

int DownloadManager::getDownloadCount() {
    return downloads.size();
}
