#include "HostsManager.h"
#include <fstream>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
    const std::string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#else
    const std::string HOSTS_PATH = "/etc/hosts";
#endif

HostsManager::HostsManager()
{
    refresh();
}

std::vector<std::string> HostsManager::getBlockedSites()
{
    return blockedSites;
}

void HostsManager::refresh()
{
    loadBlockedSites();
}

void HostsManager::loadBlockedSites()
{
    blockedSites.clear();
    std::ifstream file(HOSTS_PATH);
    if (!file.is_open()) {
        std::cout << "Cannot open hosts file for reading" << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        // Look for blocking entries
        if (line.find("127.0.0.1") == 0 ||
            line.find("::1") == 0 ||
            line.find("0.0.0.0") == 0) {

            size_t pos = line.find_last_of(' ');
            if (pos != std::string::npos) {
                std::string site = line.substr(pos + 1);

                // Skip localhost entries and remove duplicates
                if (site != "localhost" &&
                    site.find("localhost") == std::string::npos &&
                    std::find(blockedSites.begin(), blockedSites.end(), site) == blockedSites.end()) {
                    blockedSites.push_back(site);
                }
            }
        }
    }

    std::cout << "Loaded " << blockedSites.size() << " blocked sites" << std::endl;
}