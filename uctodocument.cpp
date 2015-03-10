#include "uctodocument.h"

UctoDocument::UctoDocument()
{
    mLines = QVector<QString>();
    mPageLoaded = false;
    mFileName = QString();
}

UctoDocument::UctoDocument(const UctoDocument &doc)
{
    mLines = doc.getLines(mLines);
    mPageLoaded = doc.isLoaded();
    mFileName = doc.getFileName();
}

bool UctoDocument::loadDocument(const QString &fileName)
{
    if (mPageLoaded) {
        mLines.clear();
        mPageLoaded = false;
        mFileName.clear();
    }

    mFileName = fileName;
    QFile file(mFileName);
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
        mLines.append(line);
    }
    file.close();
    mPageLoaded = true;

    return true;
}

int UctoDocument::findLine(const QString &regExpStr) const
{
    if (mPageLoaded) {
        QRegExp regexp;
        regexp.setPattern(regExpStr);

        for (int i = 0; i < mLines.count(); i++) {
            int pos = 0;
            QString line = mLines.at(i);
            if (regexp.indexIn(line, pos, regexp.CaretAtZero) != -1) {
                return i;
            }
        }
    }
    std::cerr << "UctoDocument.findLine: No documant loaded." << std::endl;
    return -1;
}

bool UctoDocument::setLine(int numLine, const QString &lineStr)
{
    if (mPageLoaded) {
        if (numLine > 0 && numLine < mLines.size()) {
            mLines.replace(numLine, lineStr);
            return true;
        }
        else {
            std::cerr << "UctoDocument.setLine: Line number " << numLine << " out of bounds [0, " << mLines.size() <<"]."  << std::endl;
            return false;
        }
    }
    else {
        std::cerr << "UctoDocument.setLine: No document loaded."  << std::endl;
        return false;
    }
}

QString UctoDocument::getLine(int numLine) const
{
    if (mPageLoaded) {
        if (numLine >= 0 && numLine < mLines.size()) {
            return mLines.at(numLine);
        }
        else {
            std::cerr << "UctoDocument.getLine: Line number " << numLine << " out of bounds [0, " << mLines.size() <<"]."  << std::endl;
            return "";
        }
    }
    else {
        std::cerr << "UctoDocument.getLine: No document loaded."  << std::endl;
        return "";
    }
}
/* TODO: implement */
QVector<QString> UctoDocument::getLines(QVector<QString> &dest) const
{
    return mLines;
}

int UctoDocument::getNumLines() const
{
    return mLines.size();
}

QString UctoDocument::getFileName() const
{
    return mFileName;
}

bool UctoDocument::isLoaded() const
{
    return mPageLoaded;
}

bool UctoDocument::saveDocument(const QString &fileName) const
{
    if (!mPageLoaded) {
        std::cerr << "UctoDocument.saveDocument: No document loaded." << std::endl;
        return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << ": Error while opening file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
        return false;
    }

    QTextStream out(&file);
    out.setCodec("Windows-1250");
    for (int i = 0; i < getNumLines(); i++) {
        out << getLine(i) << "\r\n";
        if (file.error() != 0) {
            std::cerr << ": Error while writting file " << fileName.toStdString() << ": " << file.errorString().toStdString() << std::endl;
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

UctoDocument::~UctoDocument()
{
    mLines.clear();
    mFileName.clear();
}

