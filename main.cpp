#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <uctodocument.h>
#include <billingperiod.h>
#include <uctohelper.h>
#include <iostream>

#define SUCCESS 0
#define ERR_DOC_LOADING 1
#define ERR_BILLING_PERIOD_SEARCH 2
#define ERR_SIGNATURE_MODIFICATION 3
#define ERR_BILLING_PERIOD_MONTHS_INCLUSION 4
#define ERR_BILLING_PERIOD_YEARS_INCLUSION 5
#define ERR_FILE_SAVING 6

#define ERR_PROCESSING 1
#define ERR_SAVING 2

enum SignatureMode {
    PAYER_SIGNED,
    PAYER_NOT_SIGNED,
    INTERACTIVE
};

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

void processCmdLineArgs(const QCoreApplication &a, QCommandLineParser &parser, QString &inFileName, QString &outFileName,
                        enum SignatureMode &signatureMode, bool &verbose) {
    parser.setApplicationDescription("Ucto Helper - an application to help you with automatization of forms filling.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to process."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination file name."));

    QCommandLineOption payerSignedOpt("s", QCoreApplication::translate("main", "The payer has signed the announcement."));
    parser.addOption(payerSignedOpt);
    QCommandLineOption payerNotSignedOpt("u", QCoreApplication::translate("main", "The payer has not signed the announcement."));
    parser.addOption(payerNotSignedOpt);
    QCommandLineOption verboseOpt("verbose", QCoreApplication::translate("main", "Show progress messages."));
    parser.addOption(verboseOpt);

    // An option with a value
    QCommandLineOption outFileOpt(QStringList() << "o" << "outfile",
                                  QCoreApplication::translate("main", "Store result in file <output>."),
                                  QCoreApplication::translate("main", "output"));
    parser.addOption(outFileOpt);
    parser.parse(a.arguments());
    parser.process(a);

    bool payerSignedSet = parser.isSet(payerSignedOpt);
    bool payerNotSignedSet = parser.isSet(payerNotSignedOpt);
    verbose = parser.isSet(verboseOpt);

    outFileName = "";
    if (parser.isSet(outFileOpt)) {
        outFileName = parser.value(outFileOpt);
    }

    QStringList posArgs = parser.positionalArguments();
    if (posArgs.size() > 0) {
        inFileName = posArgs.at(0);
    }
    else {
        inFileName = "";
    }
    if ((payerSignedSet && payerNotSignedSet) || (!payerSignedSet && !payerNotSignedSet)) {
        signatureMode = INTERACTIVE;
    }
    else if (payerSignedSet) {
        signatureMode = PAYER_SIGNED;
    }
    else if (payerNotSignedSet) {
        signatureMode =  PAYER_NOT_SIGNED;
    }
    else {
        signatureMode = INTERACTIVE;
    }
}

void preDocumentExecution(QString &inFileName, bool &payerSigned, enum SignatureMode &signatureMode) {
    if (inFileName == "") {
        std::cout << "Input file name: ";
        char cbuf[256];
        std::cin.getline(cbuf, 256);
        inFileName = cbuf;
    }

    if (signatureMode == INTERACTIVE) {
        payerSigned = askForPayerSignature();
    }
    else {
        payerSigned = (signatureMode == PAYER_SIGNED) ? true : false;
    }
}

int documentExecution(const QString &inFileName, UctoHelper &ucto, bool payerSigned, bool verbose ) {
    if (!ucto.openFile(inFileName)) {
        std::cerr << "Document could not be loaded." << std::endl;
        return ERR_DOC_LOADING;
    }
    if (verbose) {
        std::cout << "Document loaded..." << std::endl;
    }

    if (!ucto.retrieveBillinPeriod()) {
        std::cerr << "Billing period could not be retrieved from the document." << std::endl;
        return ERR_BILLING_PERIOD_SEARCH;
    }
    if (verbose) {
        std::cout << "Billing period found..." << std::endl;
    }
    BillingPeriod bPeriod = ucto.getBillingPeriod();
    if (verbose) {
        std::cout << bPeriod.startDay() << "." << bPeriod.startMonth() << "." << bPeriod.startYear() << " - "
                  << bPeriod.endDay() << "." << bPeriod.endMonth() << "." << bPeriod.endYear() << std::endl;
    }


    if (!ucto.modifySignature(payerSigned)) {
        std::cerr << "Signature could not be modified." << std::endl;
        return ERR_SIGNATURE_MODIFICATION;
    }
    if (verbose) {
        std::cout << "Signature changed..." << std::endl;
    }
    if (!ucto.includeBillingPeriodMonths()) {
        std::cerr << "Billing period - months could not be included in the document." << std::endl;
        return ERR_BILLING_PERIOD_MONTHS_INCLUSION;
    }
    if (verbose) {
        std::cout << "Billing period - months added..." << std::endl;
    }

    if (!ucto.includeBillingPeriodYear()) {
        std::cerr << "Billing period - year could not be included in the document." << std::endl;
        return ERR_BILLING_PERIOD_MONTHS_INCLUSION;
    }
    if (verbose) {
        std::cout << "Billing period - year added..." << std::endl;
    }
    return SUCCESS;
}

int postDocumentExecution(const QString &outFileName, const UctoHelper &ucto, bool verbose) {
    if (outFileName == "") {
        printDoc(ucto.getDocument(), 0, ucto.getDocument().getNumLines());
    }
    else {
        if (!ucto.saveFile(outFileName)) {
            std::cerr << "File saving failed." << std::endl;
            return ERR_FILE_SAVING;
        }
        if (verbose) {
            std::cout << "Modified document saved to " << outFileName.toStdString() << std::endl;
        }
    }
    if (verbose) {
        std::cout << "Done" << std::endl;
    }
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    QString inFileName;
    QString outFileName;
    UctoHelper ucto;
    bool payerSigned = false;
    bool verbose = false;

    QCoreApplication a(argc, argv);
    a.setApplicationVersion("1.0");
    a.setApplicationName("Ucto Helper");

    QCommandLineParser parser;
    SignatureMode signatureMode;
    processCmdLineArgs(a, parser, inFileName, outFileName, signatureMode, verbose);

    preDocumentExecution(inFileName, payerSigned, signatureMode);
    if (documentExecution(inFileName, ucto, payerSigned, verbose) != SUCCESS) {
        std::cerr << "Document processing failed." << std::endl;
        return ERR_PROCESSING;
    }

    if (postDocumentExecution(outFileName, ucto, verbose) != SUCCESS) {
        std::cerr << "Document saving failed." << std::endl;
        return ERR_SAVING;
    }
    return 0;
}
