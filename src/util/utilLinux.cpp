#ifdef __linux__
#include "util/utilOS.h"

std::string util_open_file_manager(const std::string& type){
    std::string command = std::string("zenity --file-selection --file-filter='" + type + "' --title='Open Project'");

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        printf("[ERROR] Failed to open pipe for Zenity.\n");
        return "";
    }

    char buffer[128];
    std::string selectedFilePath;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        selectedFilePath += buffer;
    }

    if (!selectedFilePath.empty() && selectedFilePath.back() == '\n') {
        selectedFilePath.pop_back();
    }

    pclose(pipe);

    if (!selectedFilePath.empty()) {
        return selectedFilePath;
        printf("Selected file: %s\n", selectedFilePath.c_str());
    } else {
        printf("No file selected.\n");
    }

    return "";
}

#endif