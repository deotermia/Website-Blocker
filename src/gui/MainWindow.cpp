#include "MainWindow.h"
#include "../core/Blocker.h"
#include "../core/HostsManager.h"
#include "../core/Settings.h"
#include "components/SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , blocker(new Blocker())
    , hostsManager(new HostsManager())
    , settings(new Settings())
{
    setupUI();
    setupTrayIcon();
    applyStyle();
    refreshList();
}

MainWindow::~MainWindow()
{
    delete blocker;
    delete hostsManager;
    delete settings;
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    central->setObjectName("centralWidget");
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    /* Header */
    QHBoxLayout *headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("Site Blocker");
    titleLabel->setObjectName("titleLabel");

    settingsButton = new QPushButton("Settings");
    settingsButton->setFixedSize(100, 40);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(settingsButton);

    /* Input section */
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(12);

    siteInput = new QLineEdit();
    siteInput->setPlaceholderText("Enter site domain (example.com)");
    siteInput->setFixedHeight(45);

    addButton = new QPushButton("Block Site");
    addButton->setFixedSize(130, 45);

    inputLayout->addWidget(siteInput);
    inputLayout->addWidget(addButton);

    /* Sites list */
    listLabel = new QLabel("Blocked Sites");
    listLabel->setObjectName("listLabel");

    sitesList = new QListWidget();
    sitesList->setMinimumHeight(300);

    /* Buttons */
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(10);

    removeButton = new QPushButton("Unblock Selected");
    removeButton->setFixedHeight(45);

    flushDnsButton = new QPushButton("Clear DNS Cache");
    flushDnsButton->setFixedHeight(45);

    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addWidget(flushDnsButton);

    /* Status */
    statusLabel = new QLabel("Ready");
    statusLabel->setObjectName("statusLabel");

    /* Assemble layout */
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(inputLayout);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(listLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(sitesList);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(statusLabel);

    setWindowTitle("Site Blocker");
    setMinimumSize(550, 650);
    setMaximumSize(700, 800);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddSite);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveSite);
    connect(flushDnsButton, &QPushButton::clicked, this, &MainWindow::flushDNS);
    connect(siteInput, &QLineEdit::returnPressed, this, &MainWindow::onAddSite);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::showSettings);
}

void MainWindow::setupTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Site Blocker");

    trayMenu = new QMenu(this);
    trayMenu->addAction("Show", this, &QWidget::show);
    trayMenu->addAction("Settings", this, &MainWindow::showSettings);
    trayMenu->addSeparator();
    trayMenu->addAction("Quit", qApp, &QCoreApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
}

void MainWindow::applyStyle()
{
    QString theme = settings->getTheme();
    applyTheme(theme);
}

void MainWindow::applyTheme(const QString& theme)
{
    QString accentColor = "#3498db";
    QString accentHover = "#2980b9";
    QString backgroundColor = theme == "dark" ? "#2c3e50" : "#ecf0f1";
    QString cardColor = theme == "dark" ? "#34495e" : "white";
    QString textColor = theme == "dark" ? "white" : "#2c3e50";
    QString borderColor = theme == "dark" ? "#4a6278" : "#bdc3c7";
    QString placeholderColor = theme == "dark" ? "#95a5a6" : "#7f8c8d";

    setStyleSheet(QString(R"(
        /* Main window */
        QMainWindow {
            background: %1;
        }

        /* Central widget */
        QWidget {
            background: %1;
            color: %4;
        }

        /* Buttons */
        QPushButton {
            background: %2;
            color: white;
            border: none;
            border-radius: 8px;
            font-weight: bold;
            font-size: 14px;
            padding: 10px 15px;
        }
        QPushButton:hover {
            background: %3;
        }
        QPushButton:pressed {
            background: #21618c;
        }

        /* Input field */
        QLineEdit {
            padding: 12px 15px;
            border: 2px solid %6;
            border-radius: 8px;
            background: %5;
            color: %4;
            font-size: 14px;
            selection-background-color: %2;
        }
        QLineEdit:focus {
            border-color: %2;
        }
        QLineEdit::placeholder {
            color: %7;
            font-style: italic;
        }

        /* List widget */
        QListWidget {
            border: 2px solid %6;
            border-radius: 8px;
            background: %5;
            color: %4;
            font-size: 14px;
            outline: none;
        }
        QListWidget::item {
            padding: 12px 15px;
            border-bottom: 1px solid %6;
            background: %5;
        }
        QListWidget::item:selected {
            background: %2;
            color: white;
            border-radius: 4px;
            margin: 2px;
        }
        QListWidget::item:hover {
            background: %1;
        }

        QLabel {
            background: transparent;
            color: %4;
            font-size: 14px;
        }

        QLabel[objectName="titleLabel"] {
            font-size: 24px;
            font-weight: bold;
            color: %4;
        }

        QLabel[objectName="listLabel"] {
            font-size: 16px;
            font-weight: bold;
            color: %4;
        }

        QLabel[objectName="statusLabel"] {
            background: %5;
            color: %4;
            padding: 12px;
            border-radius: 8px;
            border: 1px solid %6;
            font-size: 13px;
        }

        QDialog {
            background: %5;
            color: %4;
        }
        QDialog QLabel {
            background: transparent;
            color: %4;
            font-size: 14px;
        }
        QDialog QCheckBox {
            color: %4;
            font-size: 14px;
        }
        QDialog QComboBox {
            color: %4;
            background: %5;
            border: 2px solid %6;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
        }
        QDialog QComboBox:focus {
            border-color: %2;
        }

        QMenu {
            background: %5;
            color: %4;
            border: 1px solid %6;
            border-radius: 8px;
            padding: 5px;
        }
        QMenu::item {
            padding: 8px 20px;
            background: transparent;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background: %2;
            color: white;
        }
    )").arg(backgroundColor).arg(accentColor).arg(accentHover)
      .arg(textColor).arg(cardColor).arg(borderColor).arg(placeholderColor));
}

void MainWindow::onAddSite()
{
    QString site = siteInput->text().trimmed();
    if (site.isEmpty()) {
        statusLabel->setText("Please enter a site");
        return;
    }

    if (blocker->blockSite(site.toStdString())) {
        siteInput->clear();
        hostsManager->refresh();
        refreshList();
        statusLabel->setText("✓ Site blocked: " + site);
        settings->addBlockedSite(site);
    } else {
        QMessageBox::warning(this, "Error", "Failed to block site. Run as Administrator?");
        statusLabel->setText("✗ Failed to block site");
    }
}

void MainWindow::onRemoveSite()
{
    QList<QListWidgetItem*> selected = sitesList->selectedItems();
    if (selected.isEmpty()) {
        statusLabel->setText("Please select a site to unblock");
        return;
    }

    QString site = selected.first()->text();
    if (blocker->unblockSite(site.toStdString())) {
        hostsManager->refresh();
        refreshList();
        statusLabel->setText("✓ Site unblocked: " + site);
    } else {
        QMessageBox::warning(this, "Error", "Failed to unblock site");
        statusLabel->setText("✗ Failed to unblock site");
    }
}

void MainWindow::refreshList()
{
    sitesList->clear();
    auto sites = hostsManager->getBlockedSites();
    for (const auto& site : sites) {
        sitesList->addItem(QString::fromStdString(site));
    }

    if (sites.empty()) {
        statusLabel->setText("No sites blocked");
    } else {
        statusLabel->setText(QString::number(sites.size()) + " sites blocked");
    }
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        show();
        activateWindow();
    }
}

void MainWindow::showSettings()
{
    SettingsDialog dialog(this);
    connect(&dialog, &SettingsDialog::themeChanged, this, &MainWindow::applyTheme);
    dialog.exec();
}

void MainWindow::flushDNS()
{
#ifdef _WIN32
    statusLabel->setText("Clearing DNS cache...");

    // Nuclear DNS flush
    system("ipconfig /flushdns");
    system("netsh interface ip delete arpcache");
    system("netsh winsock reset catalog");
    system("netsh int ip reset reset.log");

    // Restart DNS Client service
    system("net stop dnscache");
    system("net start dnscache");

    // Additional DNS cache clearing
    system("nbtstat -R");
    system("nbtstat -RR");

    statusLabel->setText("✓ DNS cache cleared - RESTART BROWSER");

    QMessageBox::information(this, "DNS Cleared",
        "DNS cache has been aggressively cleared.\n\n"
        "FORCE CLOSE your browser completely (from Task Manager) and restart it.\n"
        "If still not working, restart your computer.");
#else
    statusLabel->setText("DNS clearing not supported on this OS");
#endif
}