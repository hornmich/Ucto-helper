#include <QCoreApplication>
#include <uctodocument.h>
#include <billingperiod.h>
#include <iostream>

bool askForPayerSignature() {
    char response;
    do {
        std::cout << "Poplatnik podepsal prohlaseni? (a/n): ";
        std::cin >> response;
    } while(response != 'a' && response != 'n');

    return (response == 'a') ? true : false;
}

void printDoc(const UctoDocument &doc, int start, int end) {
    if (end > doc.getNumLines()) {
        end = doc.getNumLines();
    }
    for (int i = start; i < end; i++) {
        std::cout << doc.getLine(i).toStdString() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UctoDocument doc;
    BillingPeriod bPeriod;
    QString fileName;

    fileName = QString("pokus.txt");
    if (!doc.loadDocument(fileName)) {
        std::cerr << "File loading: " << fileName.toStdString() << "failed.\n";
        return 1;
    }
    std::cout << "File " << fileName.toStdString() << " loaded.\n";
    std::cout << "\t Numlines: " << doc.getNumLines() << "\n";
    std::cout << "\t line 3: " << doc.getLine(3).toStdString() << "\n";

    bool payerSigned = askForPayerSignature();

    int lineBPeriodSource = doc.findLine("za obdob.");
    if (lineBPeriodSource == -1) {
        std::cout << "Zuctovaci obdobi nenalezeno" << std::endl;
        return 1;
    }

    std::cout << "Zuctovaci obdobi na radce " << lineBPeriodSource << std::endl;
    if (!bPeriod.findPeriod(doc.getLine(lineBPeriodSource))) {
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


    int linePayerSigned = doc.findLine("podepsal/nepodepsal");
    if (linePayerSigned == -1) {
        std::cout << "Poplatnik podepsal/nepodepsal ... nenalezeno." << std::endl;
        return 1;
    }

    QString linePayerSignedStr = doc.getLine(linePayerSigned);
    linePayerSignedStr.replace("podepsal/nepodepsal", (payerSigned) ? "     podepsal      " : "    nepodepsal     ");
    doc.setLine(linePayerSigned, linePayerSignedStr);

    int lineBPerYearDest = doc.findLine("prohl..en. - na zda.ovac. obdob.");
    if (lineBPerYearDest == -1) {
        std::cout << "Kolonka zdanovaci obdobi na rok nenalezena." << std::endl;
        return 1;
    }

    QString lineBPerYearStr = doc.getLine(lineBPerYearDest);
    if (bPeriod.startYear() != bPeriod.endYear()) {
        lineBPerYearStr.replace(" ........... ",bPeriod.startYearStr() + (QString)("-") + bPeriod.endYearStr());
    }
    else {
        lineBPerYearStr.replace("...........", (QString)("    ") + bPeriod.startYearStr() + (QString)("   "));
    }
    doc.setLine(lineBPerYearDest, lineBPerYearStr);


    int lineBPerMonthsDest = doc.findLine("na tyto m.s.ce zda.ovac.ho obdob.");
    if (lineBPerMonthsDest == -1) {
        std::cout << "Kolonka zdanovaci obdobi mesice nenalezena." << std::endl;
        return 1;
    }
    QString lineBPerMonthsStr = doc.getLine(lineBPerMonthsDest);
    lineBPerMonthsStr.replace("...........","   " + bPeriod.startMonthStr() + " - " + bPeriod.endMonthStr() + "  ");
    doc.setLine(lineBPerMonthsDest, lineBPerMonthsStr);

    printDoc(doc, 0, doc.getNumLines());

    if (doc.saveDocument("out.html")) {
        std::cout << "File saved." << std::endl;
    }
    else {
        std::cerr << "File saving failed." << std::endl;
        return 1;
    }


    return 0;
}
