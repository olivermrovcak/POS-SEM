#include <ncurses.h>
#undef timeout
#include <vector>
#include "Menu.h"
#include "Download.h"
#include "Settings.h"

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

int main() {
    initialize_ncurses();

    Settings settings;

    // Create main menu and submenus
    Menu mainMenu("WELCOME!\nChoose from menu by pressing enter\n", {"Download", "Manage Directories", "Settings", "Exit"});
    Menu settingsMenu("Settings\n", {"Set download directory", "Set number of simultaneous downloads", "Back"});

    Menu ftpCredentialsMenu("FTP Credentials\nEnter the following details:\n",
                            {"Hostname","Path to file" ,"Port", "Username", "Password", "Download file from ftp", "Back"});

    // FTP MENU
    ftpCredentialsMenu.setInput(0, "Enter Hostname");
    ftpCredentialsMenu.setAction(0, [&]() { ftpCredentialsMenu.getInput(0); });
    ftpCredentialsMenu.setInput(1, "Path to file");
    ftpCredentialsMenu.setAction(1, [&]() { ftpCredentialsMenu.getInput(1); });
    ftpCredentialsMenu.setInput(2, "Enter Port");
    ftpCredentialsMenu.setAction(2, [&]() { ftpCredentialsMenu.getInput(2); });
    ftpCredentialsMenu.setInput(3, "Enter Username");
    ftpCredentialsMenu.setAction(3, [&]() { ftpCredentialsMenu.getInput(3); });
    ftpCredentialsMenu.setInput(4, "Enter Password");
    ftpCredentialsMenu.setAction(4, [&]() { ftpCredentialsMenu.getInput(4); });

    ftpCredentialsMenu.setAction(5, [&]() {
        Download download("FTP", ftpCredentialsMenu.getInputAsString(0), settings.getSavePath().c_str(),
                          ftpCredentialsMenu.getInputAsString(1), ftpCredentialsMenu.getInputAsString(3), ftpCredentialsMenu.getInputAsString(4), 1);
    });
    ftpCredentialsMenu.setAction(6, [&]() {
        mainMenu.display();
    });

    // HLAVNE MENU
    mainMenu.setAction(0, [&]() { ftpCredentialsMenu.display(); });
    mainMenu.setAction(2, [&]() { settingsMenu.display(); });
    mainMenu.setAction(3, [&]() { endwin(); exit(0); });

    // SETTINGS MENU
    settingsMenu.setAction(0, [&]() { settingsMenu.getInput(0); });
    settingsMenu.setAction(2, [&]() { mainMenu.display(); });

    // Display the main menu
    mainMenu.display();

    endwin();
    return 0;
}


