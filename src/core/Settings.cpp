#include "Settings.h"

Settings::Settings() : settings("SiteBlocker", "Settings") {}

bool Settings::getAutoStart() const
{
    return settings.value("auto_start", false).toBool();
}

void Settings::setAutoStart(bool enabled)
{
    settings.setValue("auto_start", enabled);
}

QString Settings::getTheme() const
{
    return settings.value("theme", "light").toString();
}

void Settings::setTheme(const QString& theme)
{
    settings.setValue("theme", theme);
}

QString Settings::getLanguage() const
{
    return settings.value("language", "en").toString();
}

void Settings::setLanguage(const QString& language)
{
    settings.setValue("language", language);
}

void Settings::addBlockedSite(const QString& site)
{
    QStringList sites = getBlockedSites();
    if (!sites.contains(site)) {
        sites.append(site);
        saveBlockedSites(sites);
    }
}

QStringList Settings::getBlockedSites() const
{
    return settings.value("blocked_sites").toStringList();
}

void Settings::saveBlockedSites(const QStringList& sites)
{
    settings.setValue("blocked_sites", sites);
}

void Settings::save()
{
    settings.sync();
}