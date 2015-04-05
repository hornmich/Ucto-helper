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
    fileName = QDir::homePath();
}

bool Configuration::loadConfiguration()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "UctoDocument.loadDocument: Error while openning file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
        return false;
    }

    QTextStream in(&file);
    in.setCodec("Windows-1250");
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (file.error() != 0) {
            std::cerr << "UctoDocument.loadDocument: Error while reading file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
            file.close();
            return false;
        }
        //mLines.append(line);
    }
    file.close();

}

bool Configuration::saveConfiguration()
{

}

Configuration::~Configuration()
{

}
