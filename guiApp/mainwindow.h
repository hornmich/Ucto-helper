#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QtPrintSupport/QPrinter>
#include <QPrintDialog>
#include "../common/uctohelper.h"
#include <QWebSettings>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UctoHelper uHelper;
    UctoHelper uHelperModified;
    void changeSignature(bool isSigned);

public slots:
    void openFile();
    void setSigned();
    void setNotSigned();
    void saveAsPdf();
    void saveAsHtml();
    void print();
};

#endif // MAINWINDOW_H
