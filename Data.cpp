//
// Created by cilan on 3/20/2017.
//

#include "Data.h"

std::ofstream& operator<<(std::ofstream& save, Data data) {
    save.write(reinterpret_cast<char*>(&data.levelid), sizeof(data.levelid));
    return save;
}

std::ifstream& operator>>(std::ifstream& save, Data data) {
    save.read(reinterpret_cast<char*>(&data.levelid), sizeof(data.levelid));
    return save;
}
