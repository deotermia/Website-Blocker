#pragma once
#include <string>

namespace Config {
    const std::string APP_NAME = "Site Blocker";
    const std::string APP_VERSION = "2.0";

#ifdef _WIN32
    const std::string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#else
    const std::string HOSTS_PATH = "/etc/hosts";
#endif

    const std::string BACKUP_EXTENSION = ".backup";
    const std::string SETTINGS_FILE = "siteblocker.ini";
}