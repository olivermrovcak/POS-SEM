#include <ncurses.h>
#undef timeout
#include <vector>
#include "Menu.h"
#include "Download.h"
#include "Settings.h"
#include "DownloadManager.h"

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
    DownloadManager downloadManager;

    // Create main menu and submenus
    Menu mainMenu("WELCOME!\nChoose from menu by pressing enter\n", {"Download", "Manage Directories", "Settings", "Exit"});
    Menu ftpMenu("FTP Options\n", {"Option 1", "Option 2", "Back"});

    Menu ftpCredentialsMenu("FTP Credentials\nEnter the following details:\n",
                            {"Hostname","Path to file" ,"Port", "Username", "Password", "Download file from ftp", "Back"});

    // Set up inputs for FTP credentials
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

    // Set up actions for FTP credentials
    ftpCredentialsMenu.setAction(5, [&]() {
        std::shared_ptr<Download> download = std::make_shared<Download>("FTP", "klokanek.endora.cz", "/Users/olivermrovcak/", "/web/index.html", "olivergg", "Heslo5.", 1);
        downloadManager.addDownload(download);
    });

    ftpCredentialsMenu.setAction(6, [&]() { mainMenu.display(); });

    mainMenu.setAction(0, [&]() { ftpCredentialsMenu.display(); });
    mainMenu.setAction(4, [&]() { endwin(); exit(0); });

    ftpMenu.setAction(2, [&]() { mainMenu.display(); });
    ftpMenu.setAction(0, [&]() {
        Download download("FTP", "klokanek.endora.cz", "/Users/olivermrovcak/", "/web/index.html", "olivergg", "Heslo5.", 1);
    });

    // Link the main menu with the FTP credentials submenu
    mainMenu.setAction(0, [&]() { ftpCredentialsMenu.display(); });

    while (true) {
        // Get the current download statuses
        auto statuses = downloadManager.getDownloadStatuses();
        mainMenu.setDownloadStatuses(statuses);

        // Display the menu, which now includes download statuses
        mainMenu.display();
    }


    endwin();
    return 0;
}


