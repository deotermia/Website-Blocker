#pragma once

#include <QDialog>

class QComboBox;
class QPushButton;

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

signals:
    void themeChanged(const QString& theme);

private slots:
    void onSave();

private:
    QComboBox *themeCombo;
    QPushButton *saveButton;
};