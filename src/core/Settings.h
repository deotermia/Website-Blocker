#pragma once

#include <QString>
#include <QSettings>
#include <QStringList>

class Settings {
public:
    Settings();

    bool getAutoStart() const;
    void setAutoStart(bool enabled);

    QString getTheme() const;
    void setTheme(const QString& theme);

    QString getLanguage() const;
    void setLanguage(const QString& language);

    void addBlockedSite(const QString& site);
    QStringList getBlockedSites() const;
    void saveBlockedSites(const QStringList& sites);

    void save();

private:
    QSettings settings;
};