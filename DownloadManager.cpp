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
    threads.emplace_back(&Download::start, download);
    downloads.push_back(download);
}
