#pragma once

#include <string>

class Blocker {
public:
    Blocker();
    bool blockSite(const std::string& site);
    bool unblockSite(const std::string& site);
    bool isBlocked(const std::string& site);

private:
    bool modifyHostsFile(bool block, const std::string& site);
    std::string normalizeDomain(const std::string& site);
};