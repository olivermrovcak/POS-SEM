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

        mvprintw(0, 10, title.c_str());

        for (size_t i = 0; i < options.size(); ++i) {
            if (i == highlight) {
                attron(COLOR_PAIR(1));
                if (inputs.find(i) != inputs.end() && !inputs[i].empty()) {
                    // Display the option with the user's input in the specified format.
                    printw("-> %s: \"%s\"\n", options[i].c_str(), inputs[i].c_str());
                } else {
                    // Display the option without input.
                    printw("-> %s\n", options[i].c_str());
                }
                attroff(COLOR_PAIR(1));
            } else {
                if (inputs.find(i) != inputs.end() && !inputs[i].empty()) {
                    // Display the option with the user's input in the specified format.
                    printw("   %s: \"%s\"\n", options[i].c_str(), inputs[i].c_str());
                } else {
                    // Display the option without input.
                    printw("   %s\n", options[i].c_str());
                }
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
            case 10:  // Enter key pressed
                if (inputs.find(highlight) != inputs.end()) {
                    getInput(highlight);
                } else if (actions.find(highlight) != actions.end()) {
                    actions[highlight]();
                }
                break;
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

void Menu::setInput(int optionIndex, const std::string &inputPrompt) {
    inputs[optionIndex] = "";
    options[optionIndex] = inputPrompt + ": ";
}

std::string Menu::getInput(int optionIndex) {
    if (inputs.find(optionIndex) != inputs.end()) {
        echo();
        // Move the cursor to the position where the input will begin.
        mvprintw(2 + optionIndex, options[optionIndex].length() + 3, "");

        char buf[100];
        getnstr(buf, sizeof(buf) - 1);

        // Save the input.
        inputs[optionIndex] = buf;
        noecho();

        // Redraw the menu to display the updated option text with the input.
        display();
    }
    return inputs[optionIndex];
}

void Menu::setDownloadProgress(int progress) {
    downloadProgress = progress;
}

std::string Menu::getOption(int optionIndex) {
    return options[optionIndex];
}

std::string Menu::getInputAsString(int optionIndex) {
    return inputs[optionIndex];
}


