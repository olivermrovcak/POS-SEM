//
// Created by Oliver Mrovcak on 04/01/2024.
//

#include <string>
#include <vector>
#include <map>
#include <ncurses.h>
#undef timeout  // Add this line right after including ncurses.h
#include <string>
#include <functional>
#include <thread>
#include <chrono>

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
    std::vector<std::string> downloadstatuses;

protected:
    std::string getTitle();
    int getHighlight();

public:
    Menu(const std::string &title, const std::vector<std::string> &options);
    ~Menu();

    void setOptions(const std::vector<std::string> &options);
    virtual void display();
    void setAction(int optionIndex, std::function<void()> action);
    void executeAction();
    void setInput(int optionIndex, const std::string &inputPrompt);
    std::string getInput(int optionIndex);
    void setDownloadProgress(int progress);
    std::string getOption(int optionIndex);
    std::string getInputAsString(int optionIndex);

};


#endif //POS_SEM_MENU_H
