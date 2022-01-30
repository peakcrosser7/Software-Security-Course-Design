#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString hookInjectorStr;
    QString hookedFile;
    Ui::MainWindow *ui;
    QProcess* injectorProcess;
    QList<QStringList> hookPages;
    QString hookOpt;
    int pageIdx;
    int rowCnt=0;
    bool withAlloc=false;

    void setPage();

public slots:

private slots:
    void getHookOpt();
     void on_preButton_clicked();
     void on_nextButton_clicked();
     void on_fileButton_clicked();
     void on_hookButton_clicked();
     void on_HeapAllocCheckBox_clicked(bool checked);
};
#endif // MAINWINDOW_H
