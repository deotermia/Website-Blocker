#pragma once
#include <vector>
#include <string>

/* Manages blocked sites list */
class HostsManager {
public:
    HostsManager();
    std::vector<std::string> getBlockedSites();
    void refresh();

private:
    void loadBlockedSites();
    std::vector<std::string> blockedSites;
};