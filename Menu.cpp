//
// Created by Oliver Mrovcak on 04/01/2024.
//

#include "Menu.h"

Menu::Menu(const std::string &title, const std::vector<std::string> &options) : title(title), options(options) {
    highlight = 0;
}

Menu::~Menu() {

}

void Menu::display () {
    while (true) {
        clear();
        mvprintw(0, 0, title.c_str());

        for (size_t i = 0; i < options.size(); ++i) {
            if (i == highlight) {
                attron(COLOR_PAIR(1));
                printw("-> %s\n", options[i].c_str());
                attroff(COLOR_PAIR(1));
            } else {
                printw("   %s\n", options[i].c_str());
            }
        }
        refresh();

        int choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + options.size()) % options.size();
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % options.size();
                break;
            case 10:
                clear();
                if (actions.find(highlight) != actions.end()) {
                    actions[highlight]();
                }
                return;
        }
    }
}

void Menu::setAction(int optionIndex, std::function<void()> action) {
    actions[optionIndex] = action;
}

void Menu::executeAction() {
    if (actions.find(highlight) != actions.end()) {
        actions[highlight]();
    }
}
