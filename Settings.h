//
// Created by Oliver Mrovcak on 05/01/2024.
//

#ifndef POS_SEM_SETTINGS_H
#define POS_SEM_SETTINGS_H

#include <string>
#include <fstream>
#include <iostream>


class Settings {
private:
    std::string savePath;
public:
    Settings();
    ~Settings();

    void saveDownload(std::string url, std::string path);
    std::vector<std::string> loadHistory();
    std::string loadSavePath();
    void setSavePath(std::string savePath);
    std::string getSavePath();
};


#endif //POS_SEM_SETTINGS_H
