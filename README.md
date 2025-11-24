# Site Blocker

Website blocking application with a modern graphical interface.

## Features
- Block/unblock websites via hosts file
- Modern Qt-based GUI
- Dark/Light theme support
- System tray integration

## Requirements
- Qt 6.0+
- C++17 compiler
- Administrator/root privileges

## 🚨 Building Instructions

### Option 1: Qt Creator (Recommended for 99% of users)
1. Download Qt Creator: https://www.qt.io/download-qt-installer
2. Install Qt 6.x and Qt Creator
3. Open `CMakeLists.txt` in Qt Creator
4. Click **Configure Project**
5. Click **Build** (Ctrl+B)
6. Run as **Administrator**

### Option 2: Manual Build (Advanced users only)
Requires: Qt 6.x, CMake, C++ compiler installed and configured

## ⚠️ Important Notes
- Run as **Administrator** for blocking to work
- Use **Firefox** for best compatibility (Chrome may ignore hosts file)
- Blocking works via system hosts file

### Usage
1. Run as Administrator.
2. Enter site domain (e.g. example.com).
3. Click "Block Site".
4. Use Firefox for best compatibility.

## A few words from the author

> I decided to start my life anew and get out of my "comfort zone" by limiting myself to my daily joys, thereby giving myself discomfort. It was not convenient for me to constantly edit hosts in order to block a site for myself, for example YouTube, and I decided to simplify my life for myself and my friends so that they could simply block or unblock the site in two clicks. The application is far from ideal, as this is the first time I have written such high-level applications using the new Qt framework.
