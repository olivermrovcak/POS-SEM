#ifndef POS_SEM_DOWNLOADDISPLAYMENU_H
#define POS_SEM_DOWNLOADDISPLAYMENU_H

#include "Menu.h"
#include "DownloadManager.h"

class DownloadDisplayMenu : public Menu {
private:
    DownloadManager &downloadManager;

public:
    DownloadDisplayMenu(const std::string &title, DownloadManager &manager);
    void display() override; // Override the display method to show downloads and their progress.

};

#endif //POS_SEM_DOWNLOADDISPLAYMENU_H
