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

void DownloadManager::resumeDownload(int index) {
    if (index >= 0 && index < downloads.size()) {
        std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector
        downloads[index]->resume();  // Resume the specific download
    }
}

void DownloadManager::pauseDownload(int index) {
    if (index >= 0 && index < downloads.size()) {
        std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector
        downloads[index]->pause();  // Resume the specific download
    }
}

void DownloadManager::addDownload(std::shared_ptr<Download> download) {
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


