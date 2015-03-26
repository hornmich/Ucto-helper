#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->rbNotSigned, SIGNAL(clicked()), this, SLOT(setNotSigned()));
    connect(ui->rbSigned, SIGNAL(clicked()), this, SLOT(setSigned()));
    connect(ui->btnSaveAsPDF, SIGNAL(clicked()), this, SLOT(saveAsPdf()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(saveAsHtml()));
    connect(ui->btnPrint, SIGNAL(clicked()), this, SLOT(print()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Otevrit formular"), "/home/", tr("Ucto HTML (*.html *.htm)"));
    if (!uHelper.openFile(fileName)) {
        std::cerr << "File: " << fileName.toStdString() << " could not be opened." << std::endl;
    }
    else {
        ui->btnPrint->setEnabled(true);
        ui->btnSave->setEnabled(true);
        ui->btnSaveAsPDF->setEnabled(true);
        ui->webView->setEnabled(true);
        ui->rbNotSigned->setEnabled(true);
        ui->rbSigned->setEnabled(true);

        setNotSigned();
    }
}

void MainWindow::setSigned()
{
    changeSignature(true);
}

void MainWindow::setNotSigned()
{
    changeSignature(false);
}

void MainWindow::saveAsPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Ulozit jako PDF"), "/home/", tr("PDF file (*.pdf)"));
    QPrinter printer;
    /*QWebSettings *webSettings = ui->webView->settings();
    webSettings->setFontSize(QWebSettings::MinimumFontSize, 12);
    webSettings->setFontSize(QWebSettings::DefaultFontSize, 12);*/
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    ui->webView->print(&printer);
}

void MainWindow::saveAsHtml()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Ulozit jako HTML"), "/home/", tr("HTML file (*.html)"));
    if (!uHelperModified.saveFile(fileName)) {
        std::cerr << "Failed saving file " << fileName.toStdString() << std::endl;
    }
    else {
        std::cout << "File saved: " << fileName.toStdString() << std::endl;
    }
}

void MainWindow::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        ui->webView->print(&printer);
        std::cout << "Document sent to printer." << std::endl;
    }
}

void MainWindow::changeSignature(bool isSigned)
{
    int docStat = 0;
    uHelperModified = uHelper;
    if (!uHelperModified.modifySignature(isSigned)) {
        std::cerr << "Document does not contain pattern for signature." << std::endl;
        docStat |= 1 << 0;
    }
    if (!uHelperModified.retrieveBillinPeriod()) {
        std::cerr << "Document does not contain pattern for billing period." << std::endl;
        docStat |= 1 << 1;
    }
    if (!uHelperModified.includeBillingPeriodMonths()) {
        std::cerr << "Unable to include Biling Period Months." << std::endl;
        docStat |= 1 << 3;
    }
    if (!uHelperModified.includeBillingPeriodYear()) {
        std::cerr << "Unable to include Biling Period Year." << std::endl;
        docStat |= 1 << 4;
    }

    if (docStat != 0) {
        QMessageBox::warning(this, tr("Ucto helper"),
                             tr("V dokumentu se nepodarilo najit mista pro upravu.\n"
                                "Nepokousite se otevrit uz upraveny soubor?\n"
                                " Chyba: 0x") + QString::number(docStat, 16).toUpper(),
                             QMessageBox::Ok);
        ui->btnSave->setEnabled(false);
        ui->rbNotSigned->setEnabled(false);
        ui->rbSigned->setEnabled(false);
    }


    QString html;
    for (int i = 0; i < uHelperModified.getDocument().getNumLines(); i++) {
        html.append(uHelperModified.getDocument().getLine(i)+"\n");
    }
    ui->webView->setHtml(html);
    std::cout << "signature changed: " << isSigned << std::endl;

}
