#include <QCoreApplication>
#include <uctodocument.h>
#include <billingperiod.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UctoDocument doc;
    BillingPeriod bPeriod;
    QString fileName;

    fileName = QString("pokus.txt");

    if (doc.loadDocument(fileName)) {
        std::cout << "File " << fileName.toStdString() << " loaded.\n";
        std::cout << "\t Numlines: " << doc.getNumLines() << "\n";
        std::cout << "\t line 3: " << doc.getLine(3).toStdString() << "\n";

        bool payerSigned = false;
        char response;
        do {
            std::cout << "Poplatnik podepsal prohlaseni? (a/n): ";
            std::cin >> response;
        } while(response != 'a' && response != 'n');
        payerSigned = (response == 'a') ? true : false;

        int line = doc.findLine("za obdob.");
        if (line == -1) {
            std::cout << "Zuctovaci obdobi nenalezeno" << std::endl;
        }
        else {

            std::cout << "Zuctovaci obdobi na radce " << line << std::endl;
            if (!bPeriod.findPeriod(doc.getLine(line))) {
                std::cerr << "Zuctovaci obdobi nenalezeno." << std::endl;
                return 1;
            }
            std::cout << "Zdanovaci obdobi: ";
            if (bPeriod.startYear() != bPeriod.endYear()) {
                std::cout << bPeriod.startYear() << " - " << bPeriod.endYear();
            }
            else {
                std::cout << bPeriod.startYear();
            }
            std::cout << std::endl << "Mesice: " << bPeriod.startMonth() << " - " << bPeriod.endMonth() << std::endl;


        }

    }
    else {
        std::cerr << "File loading: " << fileName.toStdString() << "failed.\n";
        return 1;
    }

    return 0;
}
