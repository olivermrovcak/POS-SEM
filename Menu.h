//
// Created by Oliver Mrovcak on 04/01/2024.
//

#include <string>
#include <vector>
#include <map>
#include <ncurses.h>
#include <string>
#include <functional>

#ifndef POS_SEM_MENU_H
#define POS_SEM_MENU_H


class Menu {
private:
    std::string title;
    std::vector<std::string> options;
    int highlight;
    std::map<int, std::function<void()>> actions;
    std::map<int, std::string> inputs;
    int downloadProgress;
    std::vector<std::string> downloadStatuses;
public:
    Menu(const std::string &title, const std::vector<std::string> &options);
    ~Menu();

    void display();
    void setAction(int optionIndex, std::function<void()> action);
    void executeAction();
    void setInput(int optionIndex, const std::string &inputPrompt);
    std::string getInput(int optionIndex);
    void setDownloadProgress(int progress);
    std::string getOption(int optionIndex);
    std::string getInputAsString(int optionIndex);

    void setDownloadStatuses(const std::vector<std::string>& statuses) {
        downloadStatuses = statuses;
    }

    void displayDownloadStatuses() {
        for (const auto& status : downloadStatuses) {
            mvprintw(0, 10, status.c_str());
        }
    }

};


#endif //POS_SEM_MENU_H
