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

    std::string loadSavePath(const std::string& filename);
    void setSavePath(std::string savePath);
    std::string getSavePath();
};


#endif //POS_SEM_SETTINGS_H
