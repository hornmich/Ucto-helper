#ifndef UCTODOCUMENT_H
#define UCTODOCUMENT_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

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
    int findLine(const QString &regExp) const;
    bool setLine(int numLine, const QString &lineStr);
    QString getLine(int numLine) const;
    QVector<QString> getLines(QVector<QString> &dest) const;
    int getNumLines() const;
    QString getFileName() const;
    bool isLoaded() const;

    ~UctoDocument();
};

#endif // UCTODOCUMENT_H
