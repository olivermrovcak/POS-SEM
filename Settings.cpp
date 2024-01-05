//
// Created by Oliver Mrovcak on 05/01/2024.
//

#include "Settings.h"


Settings::Settings() {
    this->savePath = loadSavePath("../settings.txt");
}

Settings::~Settings() {

}

std::string Settings::loadSavePath(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        // Assuming the path is on the first line
        std::getline(file, line);
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return line;
}