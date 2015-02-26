#include <QCoreApplication>
#include <uctodocument.h>
#include <billingperiod.h>
#include <uctohelper.h>
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
    QString inFileName;
    QString outFileName;
    UctoHelper ucto;
    inFileName = QString("pokus.txt");
    outFileName = QString("out.html");

    bool payerSigned = askForPayerSignature();

    if (!ucto.openFile(inFileName)) {
        std::cerr << "Document could not be loaded." << std::endl;
        return 1;
    }
    std::cout << "Document loaded..." << std::endl;

    if (!ucto.retrieveBillinPeriod()) {
        std::cerr << "Billing periot could not be retrieved from the document." << std::endl;
        return 1;
    }
    std::cout << "Billing period found..." << std::endl;
    BillingPeriod bPeriod = ucto.getBillingPeriod();
    std::cout << bPeriod.startDay() << "." << bPeriod.startMonth() << "." << bPeriod.startYear() << " - "
              << bPeriod.endDay() << "." << bPeriod.endMonth() << "." << bPeriod.endYear() << std::endl;


    if (!ucto.modifySignature(payerSigned)) {
        std::cerr << "Signature could not be modified." << std::endl;
        return 1;
    }
    std::cout << "Signature changed..." << std::endl;

    if (!ucto.includeBillingPeriodMonths()) {
        std::cerr << "Billing period - months could not be included in the document." << std::endl;
        return 1;
    }
    std::cout << "Billing period - months added..." << std::endl;

    if (!ucto.includeBillingPeriodYear()) {
        std::cerr << "Billing period - year could not be included in the document." << std::endl;
        return 1;
    }
    std::cout << "Billing period - year added..." << std::endl;

    if (!ucto.saveFile(outFileName)) {
        std::cerr << "File saving failed." << std::endl;
        return 1;
    }
    std::cout << "Modified document saved..." << std::endl;
    std::cout << "Done" << std::endl;

    return 0;
}
