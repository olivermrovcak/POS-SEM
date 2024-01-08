//
// Created by Oliver Mrovcak on 08/01/2024.
//

#ifndef POS_SEM_FILEMANAGER_H
#define POS_SEM_FILEMANAGER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <ncurses.h>

namespace fs = std::filesystem;

class FileManager {
public:
    FileManager();
    void listDirectoryContents(const std::string& path);
    void removeDirectory(const std::string& path);
    void createDirectory(const std::string& path);
    void changeDirectory(const std::string& path);
    void printToScreen(const std::string& message);

private:
    fs::path currentPath;
    void printPathError(const std::string& path);
};

#endif //POS_SEM_FILEMANAGER_H
