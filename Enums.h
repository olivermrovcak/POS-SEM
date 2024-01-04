//
// Created by Oliver Mrovcak on 04/01/2024.
//

#ifndef POS_SEM_ENUMS_H
#define POS_SEM_ENUMS_H

enum class State {
    DOWNLOADING,
    PAUSED,
    STOPPED
};

enum class Protocol {
    HTTP,
    HTTPS,
    FTP,
    FTPS
};

enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

enum class Type {
    FILE,
    DIRECTORY
};

#endif //POS_SEM_ENUMS_H
