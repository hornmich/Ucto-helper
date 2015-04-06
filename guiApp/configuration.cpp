#include "configuration.h"


QString Configuration::getLastOpenPath() const
{
    return lastOpenPath;
}

void Configuration::setLastOpenPath(const QString &value)
{
    lastOpenPath = value;
}

QString Configuration::getLastExportPath() const
{
    return lastExportPath;
}

void Configuration::setLastExportPath(const QString &value)
{
    lastExportPath = value;
}

QString Configuration::getLastSavePath() const
{
    return lastSavePath;
}

void Configuration::setLastSavePath(const QString &value)
{
    lastSavePath = value;
}
Configuration::Configuration()
{
    fileName = QDir::homePath() + QDir::separator() + ".uctohelper.cfg";
    lastExportPath = lastOpenPath = lastSavePath = QDir::homePath();
}

bool Configuration::loadConfiguration()
{
    QString line;
    QString errStr[3] = {"lastOpenPath", "lastSavePath", "lastExportPath"};
    QString* paths[3] = {&lastOpenPath, &lastSavePath, &lastExportPath};
    bool retVal = true;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Configuration.loadConfiguration: Error while openning file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
        return false;
    }

    QTextStream in(&file);
    in.setCodec("Windows-1250");

    for (int i = 0; i < 3; i++) {
        line = in.readLine();
        if (line != NULL) {
            *paths[i] = line;
        }
        else {
            std::cerr << "Configuration.loadConfiguration: failed loading " << errStr[i].toStdString() << std::endl;
            retVal = false;
        }
    }

    file.close();
    return retVal;
}

bool Configuration::saveConfiguration()
{
    QString errStr[3] = {"lastOpenPath", "lastSavePath", "lastExportPath"};
    QString* paths[3] = {&lastOpenPath, &lastSavePath, &lastExportPath};
    bool retVal = true;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Configuration.saveConfiguration: Error while openning file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
        return false;
    }

    QTextStream out(&file);
    out.setCodec("Windows-1250");

    for (int i = 0; i < 3; i++) {
        out << *paths[i] << "\r\n";
        if (file.error() != 0) {
            std::cerr << "Configuration.saveConfiguration:  Error while writting " << errStr[i].toStdString() << ": " << file.errorString().toStdString() << std::endl;
            retVal =  false;
        }
    }

    file.close();
    return retVal;

}

Configuration::~Configuration()
{

}
