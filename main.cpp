#include <ncurses.h>

#undef timeout

#include <vector>
#include "Menu.h"
#include "Download.h"
#include "Settings.h"
#include "DownloadManager.h"
#include "DownloadDisplayMenu.h"
#include "FileManager.h"

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
    FileManager fileManager;

    const std::chrono::seconds cleanupInterval(5);

    std::thread cleanupThread([&]() {
        while (true) {
            downloadManager.cleanupCompletedDownloads();
            downloadManager.checkAndInitiateScheduledDownloads();
            std::this_thread::sleep_for(cleanupInterval);
        }
    });

    Menu mainMenu("WELCOME!\nChoose from menu by pressing enter\n",
                  {"Download", "Currently downloading", "History", "Download manager", "Schedule download","File manager","Exit"});
    Menu scheduleMenu("Schedule download\nFill the inputs\n",
                      {"Enter hostname", "Enter save path", "Enter download path", "Enter username", "Enter password",
                       "Enter priority", "Enter protocol", "Enter time", "Submit schedule", "Back"});
    Menu downloadMenu("Choose protocol\n", {"HTTP", "HTTPS", "FTP", "FTPS", "Back"});
    Menu ftpCredentialsMenu("FTP Credentials\nEnter the following details:\n",
                            {"Hostname", "Path to file", "Port", "Username", "Password", "Download file from ftp",
                             "Back"});
    Menu ftpsCredentialsMenu("FTPS Credentials\nEnter the following details:\n",
                            {"Hostname", "Path to file", "Port", "Username", "Password", "Download file from ftp",
                             "Back"});
    Menu httpsMenu("HTTPS\nEnter the following details:\n",
                   {"Hostname", "Path to file", "Download file from https",
                    "Back"});
    Menu httpMenu("HTTP\nEnter the following details:\n",
                   {"Hostname", "Path to file", "Download file from https",
                    "Back"});

    DownloadDisplayMenu downloadDisplayMenu("Downloads\nPress esc to exit.\n", downloadManager);

    Menu managerMenu("Download manager\n", {});

    Menu downloadHistoryMenu("Download history\n", {});

    Menu fileMenu("File Manager\n\n", {"Enter path: ","List Contents", "Remove Directory", "Create Directory", "Back"});


    //download menu
    downloadMenu.setAction(0, [&]() {
        httpMenu.display();
    });
    downloadMenu.setAction(1, [&]() {
        httpsMenu.display();
    });
    downloadMenu.setAction(2, [&]() {
        ftpCredentialsMenu.display();
    });
    downloadMenu.setAction(3, [&]() {
        ftpsCredentialsMenu.display();
    });
    downloadMenu.setAction(4, [&]() {
        mainMenu.display();
    });

    //schedule menu
    scheduleMenu.setInput(0, "Enter hostname");
    scheduleMenu.setAction(0, [&]() { scheduleMenu.getInput(0); });

    scheduleMenu.setInput(1, "Enter save path");
    scheduleMenu.setAction(1, [&]() { scheduleMenu.getInput(1); });

    scheduleMenu.setInput(2, "Enter download path");
    scheduleMenu.setAction(2, [&]() { scheduleMenu.getInput(2); });

    scheduleMenu.setInput(3, "Enter username");
    scheduleMenu.setAction(3, [&]() { scheduleMenu.getInput(3); });

    scheduleMenu.setInput(4, "Enter password");
    scheduleMenu.setAction(4, [&]() { scheduleMenu.getInput(4); });

    scheduleMenu.setInput(5, "Enter priority");
    scheduleMenu.setAction(5, [&]() { scheduleMenu.getInput(5); });

    scheduleMenu.setInput(6, "Enter protocol");
    scheduleMenu.setAction(6, [&]() { scheduleMenu.getInput(6); });

    scheduleMenu.setInput(7, "Enter time - H:M");
    scheduleMenu.setAction(7, [&]() { scheduleMenu.getInput(7); });

    scheduleMenu.setAction(8, [&]() {
        std::string priority = scheduleMenu.getInputAsString(5);
        std::string username = scheduleMenu.getInputAsString(3);
        std::string password = scheduleMenu.getInputAsString(4);
        std::shared_ptr<Download> download = std::make_shared<Download>(
                scheduleMenu.getInputAsString(6),
                scheduleMenu.getInputAsString(0),
                scheduleMenu.getInputAsString(1),
                scheduleMenu.getInputAsString(2),
                scheduleMenu.getInputAsString(3),
                scheduleMenu.getInputAsString(4), 1,
                scheduleMenu.getInputAsString(7));
        downloadManager.addScheduledDownload(download);
        std::cout << "Download scheduled" << std::endl;
    });

    scheduleMenu.setAction(9, [&]() {
        mainMenu.display(); });

    //ftp menu
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
        std::shared_ptr<Download> download = std::make_shared<Download>("FTP", ftpCredentialsMenu.getInputAsString(0),
                                                                       settings.getSavePath().c_str(), ftpCredentialsMenu.getInputAsString(1),
                                                                      ftpCredentialsMenu.getInputAsString(3), ftpCredentialsMenu.getInputAsString(4), 1);
        downloadManager.addDownload(download);
        settings.saveDownload(download->getHostname(), download->getDownloadPath());
    });

    ftpCredentialsMenu.setAction(6, [&]() { downloadMenu.display(); });

    //ftps menu
    ftpsCredentialsMenu.setInput(0, "Enter Hostname");
    ftpsCredentialsMenu.setAction(0, [&]() { ftpCredentialsMenu.getInput(0); });

    ftpsCredentialsMenu.setInput(1, "Path to file");
    ftpsCredentialsMenu.setAction(1, [&]() { ftpCredentialsMenu.getInput(1); });

    ftpsCredentialsMenu.setInput(2, "Enter Port");
    ftpsCredentialsMenu.setAction(2, [&]() { ftpCredentialsMenu.getInput(2); });

    ftpsCredentialsMenu.setInput(3, "Enter Username");
    ftpsCredentialsMenu.setAction(3, [&]() { ftpCredentialsMenu.getInput(3); });

    ftpsCredentialsMenu.setInput(4, "Enter Password");
    ftpsCredentialsMenu.setAction(4, [&]() { ftpCredentialsMenu.getInput(4); });

    // Set up actions for FTP credentials
    ftpsCredentialsMenu.setAction(5, [&]() {
        std::shared_ptr<Download> download = std::make_shared<Download>("FTPS", ftpCredentialsMenu.getInputAsString(0),
                                                                        settings.getSavePath().c_str(), ftpCredentialsMenu.getInputAsString(1),
                                                                        ftpCredentialsMenu.getInputAsString(3), ftpCredentialsMenu.getInputAsString(4), 1);
        downloadManager.addDownload(download);
        settings.saveDownload(download->getHostname(), download->getDownloadPath());
    });

    ftpsCredentialsMenu.setAction(6, [&]() { downloadMenu.display(); });

    //https menu
    httpsMenu.setInput(0, "Enter Hostname");
    httpsMenu.setAction(0, [&]() { httpsMenu.getInput(0); });

    httpsMenu.setInput(1, "Path to file");
    httpsMenu.setAction(1, [&]() { httpsMenu.getInput(1); });
    httpsMenu.setAction(2, [&]() {
        std::shared_ptr<Download> download = std::make_shared<Download>("HTTPS", httpsMenu.getInputAsString(0), settings.getSavePath().c_str(), httpsMenu.getInputAsString(1), "", "", 1);
        downloadManager.addDownload(download);
        settings.saveDownload(download->getHostname(), download->getDownloadPath());
    });
    httpsMenu.setAction(3, [&]() {
        downloadMenu.display();
    });

    //http menu
    httpMenu.setInput(0, "Enter Hostname");
    httpMenu.setAction(0, [&]() { httpsMenu.getInput(0); });

    httpMenu.setInput(1, "Path to file");
    httpMenu.setAction(1, [&]() { httpsMenu.getInput(1); });
    httpMenu.setAction(2, [&]() {
        std::shared_ptr<Download> download = std::make_shared<Download>("HTTP", httpsMenu.getInputAsString(0), settings.getSavePath().c_str(), httpsMenu.getInputAsString(1), "", "", 1);
        downloadManager.addDownload(download);
        settings.saveDownload(download->getHostname(), download->getDownloadPath());
    });
    httpMenu.setAction(3, [&]() {
        downloadMenu.display();
    });

    //file manager
    fileMenu.setInput(0, "Enter path: ");
    fileMenu.setAction(0, [&]() { fileMenu.getInput(0); });
    fileMenu.setAction(1, [&]() {
        if (fileMenu.getInputAsString(0) == "") {
            fileMenu.getInput(0);
        } else {
            fileManager.listDirectoryContents(fileMenu.getInputAsString(0));
        }
    });
    fileMenu.setAction(2, [&]() {
        if (fileMenu.getInputAsString(0) == "") {
            fileMenu.getInput(0);
        } else {
            fileManager.removeDirectory(fileMenu.getInputAsString(0));
        }
    });
    fileMenu.setAction(3, [&]() {
        if (fileMenu.getInputAsString(0) == "") {
            fileMenu.getInput(0);
        } else {
            fileManager.createDirectory(fileMenu.getInputAsString(0));
        }
    });
    fileMenu.setAction(4, [&]() {
        mainMenu.display();
    });

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

            managerSubMenus.back().emplace_back("Download Options\n",
                                                std::vector<std::string>{"Pause", "Resume","Cancel", "Back"});
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

            managerSubMenus.back().back().setAction(2, [&, index]() {
                downloadManager.cancelDownload(index);
                managerMenu.display();  // Display the manager menu again after action
            });

            // Set 'Back' action
            managerSubMenus.back().back().setAction(3, [&]() {
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
        scheduleMenu.display();
    });
    mainMenu.setAction(5, [&]() {
       fileMenu.display();
    });
    mainMenu.setAction(6, [&]() {
        endwin();
        exit(0);
    });

    mainMenu.display();

    endwin();

    cleanupThread.join();

    return 0;
}


