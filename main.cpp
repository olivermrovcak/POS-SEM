#include <ncurses.h>

#undef timeout

#include <vector>
#include "Menu.h"
#include "Download.h"
#include "Settings.h"
#include "DownloadManager.h"
#include "DownloadDisplayMenu.h"

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

bool checkSettingsFileExists() {
    std::ifstream file("../settings.txt");  // Change this to your actual settings file path
    return file.good();  // Returns true if the file exists and is accessible
}

int main() {
    initialize_ncurses();

    Settings settings;
    DownloadManager downloadManager;

    const std::chrono::seconds cleanupInterval(5);

    // Create and start the cleanup thread
    std::thread cleanupThread([&]() {
        while (true) { // You might want a condition here to stop the thread when the program is exiting
            downloadManager.cleanupCompletedDownloads(); // Call the cleanup method
            std::this_thread::sleep_for(cleanupInterval); // Sleep for the cleanup interval
        }
    });

    // Create main menu and submenus
    Menu mainMenu("WELCOME!\nChoose from menu by pressing enter\n",
                  {"Download", "Currently downloading", "History", "Download manager", "Settings", "Exit"});
    Menu downloadMenu("Choose protocol\n", {"HTTP", "HTTPS", "FTP", "FTPS", "Back"});
    Menu ftpCredentialsMenu("FTP Credentials\nEnter the following details:\n",
                            {"Hostname", "Path to file", "Port", "Username", "Password", "Download file from ftp",
                             "Back"});
    DownloadDisplayMenu downloadDisplayMenu("Downloads\nPress esc to exit.\n", downloadManager);

    Menu managerMenu("Download manager\n", {});

    Menu downloadHistoryMenu("Download history\n", {});

    //download menu
    downloadMenu.setAction(2, [&]() {
      ftpCredentialsMenu.display();
    });
    downloadMenu.setAction(4, [&]() {
        mainMenu.display();
    });

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
//        std::shared_ptr<Download> download = std::make_shared<Download>("FTP", ftpCredentialsMenu.getInputAsString(0),
//                                                                        settings.getSavePath().c_str(), ftpCredentialsMenu.getInputAsString(1),
//                                                                        ftpCredentialsMenu.getInputAsString(3), ftpCredentialsMenu.getInputAsString(4), 1);
        std::shared_ptr<Download> download = std::make_shared<Download>("FTP", "klokanek.endora.cz", settings.getSavePath().c_str(), "/web/style.css", "olivergg", "Heslo5.", 1);
        downloadManager.addDownload(download);
        settings.saveDownload(download->getHostname(), download->getDownloadPath(), download->getSize());
    });

    ftpCredentialsMenu.setAction(6, [&]() { mainMenu.display(); });

    //MAIN MENU
    mainMenu.setAction(0, [&]() { downloadMenu.display(); });
    mainMenu.setAction(1, [&]() { downloadDisplayMenu.display(); });
    mainMenu.setAction(2, [&]() {
        std::vector<std::string> history;
        for (int i = 0; i < settings.loadHistory().size(); ++i) {
            history.push_back(settings.loadHistory()[i]);
        }
        history.emplace_back("back");
        downloadHistoryMenu.setOptions(history);
        downloadHistoryMenu.setAction(history.size() - 1, [&]() { mainMenu.display(); });
        downloadHistoryMenu.display();
    });

    //DOWNLOAD MANAGER
    mainMenu.setAction(3, [&]() {
        std::vector<std::vector<Menu>> managerSubMenus;  // Create a vector to hold submenus for each download
        std::vector<std::string> managerOptions;
        for (int i = 0; i < downloadManager.getDownloads().size(); ++i) {
            // Create a submenu for each download
            managerSubMenus.push_back(std::vector<Menu>());
            managerOptions.push_back(downloadManager.getDownloads()[i]->getDownloadPath());

            // Add the submenu with 'Pause' and 'Resume' options
            managerSubMenus.back().emplace_back("Download Options\n",
                                                std::vector<std::string>{"Pause", "Resume", "Back"});
            int index = i;  // Capture the current index

            // Set 'Pause' action
            managerSubMenus.back().back().setAction(0, [&, index]() {
                downloadManager.pauseDownload(index);
                managerMenu.display();  // Display the manager menu again after action
            });

            // Set 'Resume' action
            managerSubMenus.back().back().setAction(1, [&, index]() {
                downloadManager.resumeDownload(index);
                managerMenu.display();  // Display the manager menu again after action
            });

            // Set 'Back' action
            managerSubMenus.back().back().setAction(2, [&]() {
                managerMenu.display();  // Simply go back to the manager menu
            });
        }

        // Setting up action for each download to open its submenu
        for (int i = 0; i < managerOptions.size(); ++i) {
            int index = i;  // Capture the current index
            managerMenu.setAction(index, [&, index]() {
                managerSubMenus[index].back().display();  // Display the corresponding submenu
            });
        }

        managerOptions.emplace_back("Back");
        managerMenu.setAction(managerOptions.size() - 1, [&]() { mainMenu.display(); });
        managerMenu.setOptions(managerOptions);
        managerMenu.display();
    });
    mainMenu.setAction(4, [&]() {
        endwin();
        exit(0);
    });

    mainMenu.display();

    endwin();

    cleanupThread.join();

    return 0;
}


