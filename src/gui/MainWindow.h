#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QMenu>

class Blocker;
class HostsManager;
class Settings;
class SettingsDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddSite();
    void onRemoveSite();
    void refreshList();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showSettings();
    void applyTheme(const QString& theme);
    void flushDNS();

private:
    void setupUI();
    void setupTrayIcon();
    void applyStyle();

    QListWidget *sitesList;
    QLineEdit *siteInput;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *settingsButton;
    QPushButton *flushDnsButton;
    QLabel *statusLabel;
    QLabel *titleLabel;
    QLabel *listLabel;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    Blocker *blocker;
    HostsManager *hostsManager;
    Settings *settings;
    SettingsDialog *settingsDialog;
};