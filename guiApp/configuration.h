#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QFile>
#include <QString>
#include <QDir>
#include <iostream>
#include <QTextStream>

class Configuration
{
private:
    QString lastOpenPath;
    QString lastExportPath;
    QString lastSavePath;
    QString fileName;
public:
    Configuration();
    bool loadConfiguration();
    bool saveConfiguration();
    ~Configuration();
    QString getLastOpenPath() const;
    void setLastOpenPath(const QString &value);
    QString getLastExportPath() const;
    void setLastExportPath(const QString &value);
    QString getLastSavePath() const;
    void setLastSavePath(const QString &value);
};

#endif // CONFIGURATION_H
