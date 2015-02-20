#include "uctodocument.h"

UctoDocument::UctoDocument()
{
    mPages = QVector<QString>();
    mPageLoaded = false;
    mFileName = QString();
}

UctoDocument::UctoDocument(const UctoDocument &doc)
{
    mPages = doc.getPages(mPages);
    mPageLoaded = doc.isLoaded();
    mFileName = doc.getFileName();
}

bool UctoDocument::loadDocument(const QString &fileName)
{
    if (mPageLoaded) {
        mPages.clear();
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
        mPages.append(line);
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
    if (mPageLoaded && numLine > 0 && numLine < mPages.size()) {
        mPages.replace(numLine, lineStr);
        return true;
    }
    else {
        return false;
    }
}

QString UctoDocument::getLine(int numLine) const
{
    if (mPageLoaded && numLine > 0 && numLine < mPages.size()) {
        return mPages.at(numLine);
    }
    else {
        return "empty line";
    }
}

QVector<QString> UctoDocument::getPages(QVector<QString> &dest) const
{

}

int UctoDocument::getNumPages() const
{
    return mPages.size();
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
    mPages.clear();
    mFileName.clear();
}

