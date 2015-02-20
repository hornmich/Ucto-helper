#include <QCoreApplication>
#include <uctodocument.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UctoDocument doc;
    QString fileName;

    fileName = QString("pokus.txt");

    if (doc.loadDocument(fileName)) {
        std::cout << "File " << fileName.toStdString() << " loaded.\n";
        std::cout << "\t Numlines: " << doc.getNumLines() << "\n";
        std::cout << "\t line 3: " << doc.getLine(3).toStdString() << "\n";
    }
    else {
        std::cerr << "File loading: " << fileName.toStdString() << "failed.\n";
        return 1;
    }

    return 0;
}
