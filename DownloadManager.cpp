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

//cancell download
void DownloadManager::cancelDownload(int index) {
    if (index >= 0 && index < downloads.size()) {
        std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector
        downloads[index]->cancel();  // Resume the specific download
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

void DownloadManager::cleanupCompletedDownloads() {
    std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector

    // Iterate through the downloads and remove completed ones
    for (auto it = downloads.begin(); it != downloads.end(); ) {
        if ((*it)->isCompleted() || (*it)->isCancelled()) {
            it = downloads.erase(it);  // Remove and move to the next
        } else {
            ++it;  // Move to the next
        }
    }
}

void DownloadManager::addScheduledDownload(std::shared_ptr<Download> download) {
    std::lock_guard<std::mutex> guard(downloadMutex);
    scheduledDownloads.push_back(download);
}

void DownloadManager::removeScheduledDownload(Download download) {
    std::lock_guard<std::mutex> guard(downloadMutex);
    for (auto it = scheduledDownloads.begin(); it != scheduledDownloads.end(); ) {
        if ((*it)->getDownloadPath() == download.getDownloadPath()) {
            it = scheduledDownloads.erase(it);  // Remove and move to the next
        } else {
            ++it;  // Move to the next
        }
    }
}

void DownloadManager::checkAndInitiateScheduledDownloads() {
    std::lock_guard<std::mutex> guard(downloadMutex);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    for (auto it = scheduledDownloads.begin(); it != scheduledDownloads.end(); ) {
        std::string scheduledTime = (*it)->getScheduledTime();

        std::tm scheduled_tm = {};
        std::istringstream ss(scheduledTime);
        ss >> std::get_time(&scheduled_tm, "%H:%M");

        if (ss.fail()) {
            std::cout << "Time parsing failed for: " << scheduledTime << std::endl;
            ++it;  // Skip the current iteration if the time is not parsed correctly
            continue;
        }

        // Check if the scheduled hour and minute are earlier than or equal to the current time
        if ((scheduled_tm.tm_hour < now_tm.tm_hour) ||
            (scheduled_tm.tm_hour == now_tm.tm_hour && scheduled_tm.tm_min <= now_tm.tm_min)) {
            threads.emplace_back(&Download::start, *it);  // Start the download in a new thread
            downloads.push_back(*it);  // Add to the active downloads list
            it = scheduledDownloads.erase(it);  // Remove from the scheduled list and move to the next
        } else {
            ++it;  // Move to the next if the scheduled time is later than the current time
        }
    }
}




