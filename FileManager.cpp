//
// Created by Oliver Mrovcak on 08/01/2024.
//

#include "FileManager.h"

FileManager::FileManager() {
    currentPath = fs::current_path();
}

void FileManager::printToScreen(const std::string& message) {
    printw("%s\n", message.c_str()); // ncurses print function
    refresh(); // Refresh the screen to show the update
}

void FileManager::listDirectoryContents(const std::string& path) {
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path))
                printToScreen(entry.path().filename());
            getch();
        } else {
            printPathError(path);
        }
    } catch (const std::exception& e) {
        printToScreen(e.what());
        getch();
    }
}

void FileManager::removeDirectory(const std::string& path) {
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            fs::remove_all(path);
            printToScreen("Directory removed: " + path);
            getch();
        } else {
            printPathError(path);
        }
    } catch (const std::exception& e) {
        printToScreen(e.what());
        getch();
    }
}

void FileManager::createDirectory(const std::string& path) {
    try {
        if (!fs::exists(path)) {
            fs::create_directory(path);
            printToScreen("Directory created: " + path);
            getch();
        } else {
            std::cerr << "Directory already exists: " << path << std::endl;
        }
    } catch (const std::exception& e) {
        printToScreen(e.what());
        getch();
    }
}

void FileManager::changeDirectory(const std::string& path) {
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            currentPath = path;
            fs::current_path(currentPath);
            std::cout << "Changed directory to: " << currentPath << std::endl;
        } else {
            printPathError(path);
        }
    } catch (const std::exception& e) {
        printToScreen(e.what());
        getch();
    }
}

void FileManager::printPathError(const std::string& path) {
    std::cerr << "Invalid path: " << path << std::endl;
}
