#include "DownloadDisplayMenu.h"

DownloadDisplayMenu::DownloadDisplayMenu(const std::string &title, DownloadManager &manager )
        : Menu(title, std::vector<std::string>()), downloadManager(manager) {}

void DownloadDisplayMenu::display() {

    auto getDownloadPercentage = [](std::shared_ptr<Download> download) -> std::string {
        if (download->getSize() == 0) {
            return "0%";
        }
        return std::to_string(download->getCurrentSize() * 100 / download->getSize()) + "%";
    };

    while (true) {
        clear();
        mvprintw(1, 10, getTitle().c_str());

        auto downloads = downloadManager.getDownloads(); // Ensure this method exists in DownloadManager
        for (int i = 0; i < downloads.size(); ++i) {
            auto download = downloads[i];
            if (i == getHighlight()) {
                attron(COLOR_PAIR(1));
            }

            printw("%d. %s - %s, %s, %s",
                   i + 1,
                   download->getDownloadPath().c_str(),
                   getDownloadPercentage(download).c_str(),
                   (download->isPaused() && !download->isCompleted()) ? "Paused\n" : "Not Paused",
                   download->isCompleted() ? "Completed\n" : "\n");


            if (i == getHighlight()) {
                attroff(COLOR_PAIR(1));
            }
        }
        refresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        if (getch() == 27) {
            break;
        }
    }
}

void DownloadManager::cleanupCompletedDownloads() {
    std::lock_guard<std::mutex> guard(downloadMutex);  // Protect the downloads vector

    // Iterate through the downloads and remove completed ones
    for (auto it = downloads.begin(); it != downloads.end(); ) {
        if ((*it)->isCompleted()) {
            it = downloads.erase(it);  // Remove and move to the next
        } else {
            ++it;  // Move to the next
        }
    }
}
