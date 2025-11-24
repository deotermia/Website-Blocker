#include "Blocker.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

#ifdef _WIN32
    const std::string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
#else
    const std::string HOSTS_PATH = "/etc/hosts";
#endif

Blocker::Blocker() {}

bool Blocker::blockSite(const std::string& site)
{
    std::string normalized = normalizeDomain(site);
    std::cout << "Blocking: " << normalized << std::endl;

    std::ofstream file(HOSTS_PATH, std::ios::app);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot open hosts file!" << std::endl;
        return false;
    }

    file << "127.0.0.1 " << normalized << "\n";
    file << "::1 " << normalized << "\n";
    file << "0.0.0.0 " << normalized << "\n";
    file << "127.0.0.1 www." << normalized << "\n";
    file << "::1 www." << normalized << "\n";
    file << "0.0.0.0 www." << normalized << "\n";
    file.close();

    #ifdef _WIN32
        system("ipconfig /flushdns");
    #endif

    std::cout << "SUCCESS: " << normalized << " blocked with triple protection" << std::endl;
    return true;
}

bool Blocker::unblockSite(const std::string& site)
{
    std::string normalized = normalizeDomain(site);
    std::cout << "Unblocking: " << normalized << std::endl;

    std::ifstream inFile(HOSTS_PATH);
    if (!inFile.is_open()) {
        std::cout << "ERROR: Cannot open hosts file!" << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inFile, line)) {
        if (line.find(normalized) == std::string::npos) {
            lines.push_back(line);
        } else {
            std::cout << "Removed line: " << line << std::endl;
        }
    }
    inFile.close();

    std::ofstream outFile(HOSTS_PATH);
    if (!outFile.is_open()) {
        std::cout << "ERROR: Cannot write to hosts file!" << std::endl;
        return false;
    }

    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    #ifdef _WIN32
        system("ipconfig /flushdns");
    #endif

    std::cout << "Successfully unblocked: " << normalized << std::endl;
    return true;
}

bool Blocker::isBlocked(const std::string& site)
{
    std::ifstream file(HOSTS_PATH);
    if (!file.is_open()) return false;

    std::string normalized = normalizeDomain(site);
    std::string line;

    while (std::getline(file, line)) {
        // Remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Trim
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        // Check blocking entry
        if ((line.find("127.0.0.1") == 0 || line.find("::1") == 0) &&
            line.find(normalized) != std::string::npos) {
            return true;
        }
    }

    return false;
}

bool Blocker::modifyHostsFile(bool block, const std::string& site)
{
    std::string normalized = normalizeDomain(site);
    std::cout << (block ? "Blocking: " : "Unblocking: ") << normalized << std::endl;

    std::ifstream inFile(HOSTS_PATH);
    if (!inFile.is_open()) {
        std::cout << "ERROR: Cannot open hosts file!" << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    // Read all lines
    while (std::getline(inFile, line)) {
        std::string checkLine = line;

        // Remove comments for checking
        size_t commentPos = checkLine.find('#');
        if (commentPos != std::string::npos) {
            checkLine = checkLine.substr(0, commentPos);
        }

        // Trim
        checkLine.erase(0, checkLine.find_first_not_of(" \t"));
        checkLine.erase(checkLine.find_last_not_of(" \t") + 1);

        // Check if this is our site
        if (!checkLine.empty() &&
            (checkLine.find("127.0.0.1") == 0 ||
             checkLine.find("::1") == 0 ||
             checkLine.find("0.0.0.0") == 0) &&
            checkLine.find(normalized) != std::string::npos) {
            found = true;
            if (!block) {
                // Skip this line (unblock)
                continue;
            }
        }

        lines.push_back(line);
    }
    inFile.close();

    // Add if blocking and not found
    if (block && !found) {
        lines.push_back("127.0.0.1 " + normalized);
        lines.push_back("::1 " + normalized);
        lines.push_back("0.0.0.0 " + normalized);
    }

    // Write back
    std::ofstream outFile(HOSTS_PATH);
    if (!outFile.is_open()) {
        std::cout << "ERROR: Cannot write to hosts file!" << std::endl;
        return false;
    }

    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    // DNS flushing
    #ifdef _WIN32
        system("ipconfig /flushdns");
    #endif

    std::cout << "SUCCESS: " << normalized << (block ? " blocked" : " unblocked") << std::endl;
    return true;
}

std::string Blocker::normalizeDomain(const std::string& site)
{
    std::string result = site;

    // To lowercase
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    // Remove protocol
    size_t pos = result.find("://");
    if (pos != std::string::npos) {
        result = result.substr(pos + 3);
    }

    // Remove www.
    if (result.find("www.") == 0) {
        result = result.substr(4);
    }

    // Remove path
    pos = result.find('/');
    if (pos != std::string::npos) {
        result = result.substr(0, pos);
    }

    return result;
}