//
// Created by Oliver Mrovcak on 05/01/2024.
//

#include "Settings.h"


Settings::Settings() {
    this->savePath = loadSavePath();
}

Settings::~Settings() {

}

void Settings::saveDownload(std::string url, std::string path, unsigned long size) {
    std::ofstream file;
    file.open("../history.txt", std::ios_base::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open history.txt for appending." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    file << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << " - " << url << " " << path << " size:" << size << std::endl;
    file.close();
}

std::string Settings::loadSavePath() {
    std::ifstream file("../settings.txt");
    std::string line;
    if (file.is_open()) {
        // Assuming the path is on the first line
        std::getline(file, line);
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    return line;
}

std::vector<std::string> Settings::loadHistory() {
    std::ifstream file("../history.txt");
    std::vector<std::string> lines;
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: history.txt" << std::endl;
    }
    return lines;
}

std::string Settings::getSavePath() {
    return this->savePath;
}
