#ifndef UCTODOCUMENT_H
#define UCTODOCUMENT_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

class UctoDocument
{
private:
    QVector<QString> mLines;
    bool mPageLoaded;
    QString mFileName;

public:
    UctoDocument();
    UctoDocument(const UctoDocument &doc);

    bool loadDocument(const QString &fileName);
    int findLine(const QString &regExpStr) const;
    bool setLine(int numLine, const QString &lineStr);
    QString getLine(int numLine) const;
    QVector<QString> getLines(QVector<QString> &dest) const;
    int getNumLines() const;
    QString getFileName() const;
    bool isLoaded() const;
    bool saveDocument(const QString &fileName) const;

    ~UctoDocument();
};

#endif // UCTODOCUMENT_H
