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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLines.append(line);
    }
    file.close();
    mPageLoaded = true;

    return true;
}

int UctoDocument::findLine(const QString &regExp) const
{

}

bool UctoDocument::setLine(int numLine, const QString &lineStr)
{
    if (mPageLoaded && numLine > 0 && numLine < mLines.size()) {
        mLines.replace(numLine, lineStr);
        return true;
    }
    else {
        return false;
    }
}

QString UctoDocument::getLine(int numLine) const
{
    if (mPageLoaded && numLine > 0 && numLine < mLines.size()) {
        return mLines.at(numLine);
    }
    else {
        return "empty line";
    }
}

QVector<QString> UctoDocument::getLines(QVector<QString> &dest) const
{

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

UctoDocument::~UctoDocument()
{
    mLines.clear();
    mFileName.clear();
}

