#include "SettingsDialog.h"
#include "../../core/Settings.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *themeLabel = new QLabel("Theme:", this);

    themeCombo = new QComboBox(this);
    themeCombo->addItems({"Light", "Dark"});
    themeCombo->setFixedHeight(35);

    saveButton = new QPushButton("Save", this);
    saveButton->setFixedHeight(40);

    layout->addWidget(themeLabel);
    layout->addWidget(themeCombo);
    layout->addStretch();
    layout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::onSave);

    Settings settings;
    themeCombo->setCurrentText(settings.getTheme() == "dark" ? "Dark" : "Light");
}

void SettingsDialog::onSave()
{
    Settings settings;

    QString theme = themeCombo->currentText().toLower();
    settings.setTheme(theme);
    emit themeChanged(theme);

    accept();
}