#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , hookInjectorStr("E:/Computer/C_Language/SoftwSec/Debug/Injector.exe")
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->TableWidget->verticalHeader()->hide();
    ui->TableWidget->setColumnWidth(0,190);
    ui->TableWidget->setColumnWidth(1,400);
    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

//获取注射器进程输出
void MainWindow::getHookOpt(){
    auto bytes=injectorProcess->readAllStandardOutput();    //进程输出
    qDebug()<<bytes<<Qt::endl<<Qt::endl;
    hookOpt.append(bytes);  //添加到hookOpt
//    qDebug()<<hookOpt;

    //若已经获取完本次所有输出
    if(hookOpt.endsWith("$HOOKFIN$")){
        hookOpt.replace("$HOOKFIN$","");
        //分页
        QStringList hookPageStrs=hookOpt.split("$HOOKEND$");
        //分行
        for(auto& page:hookPageStrs){
            if(page=="") continue;
//            qDebug()<<page<<endl;
            hookPages.append(page.split("\r\n"));
        }

        if(hookPages.size()){
            pageIdx=0;
            setPage();
        }
        qDebug()<<"hookpages.size:"<<hookPages.size()<<endl;
    }
}

//设置页面
void MainWindow::setPage(){
    ui->TableWidget->clearContents();   //清空列表
    int i=0;    //行号
    //遍历并更新列表
    for(auto& it:hookPages[pageIdx]){
        if(it=="") continue;
        if(i==rowCnt){  //若输出大于当前列表行数则插入新行
            ui->TableWidget->insertRow(i);
            ++rowCnt;
        }
        //设置列表项
        QStringList kv=it.split(": ");
        QTableWidgetItem *qtKey=new QTableWidgetItem(kv[0]);
        ui->TableWidget->setItem(i,0,qtKey);
        QTableWidgetItem *qtVal=new QTableWidgetItem(kv[1]);
        ui->TableWidget->setItem(i,1,qtVal);
        ++i;
    }

    //设置进度条
    if(pageIdx!=hookPages.size()-1){
        ui->progressBar->setValue(100./hookPages.size()*(pageIdx+1));
    }else{
        ui->progressBar->setValue(100);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

//上一页
void MainWindow::on_preButton_clicked() {
    if(hookPages.size()==0 || pageIdx==0){
        return;
    }
    --pageIdx;
    setPage();
}

//下一页
void MainWindow::on_nextButton_clicked() {
    if(pageIdx>=hookPages.size()-1){
        return;
    }
    ++pageIdx;
    setPage();
}

//打开文件按钮
void MainWindow::on_fileButton_clicked(){
    hookedFile = QFileDialog::getOpenFileName(
                this,
                tr("Open Exe file"),
                "E:\\Computer\\C_Language\\SoftwSec",
                tr("Executable File(*.exe)")
                );
    ui->textBrowser->clear();   //清空文本框
    ui->textBrowser->insertPlainText(hookedFile);   //更新文本框为当前路径
//    qDebug()<<hookedFile;
}

//Hook运行按钮
void MainWindow::on_hookButton_clicked(){
    if(hookedFile==""){     //未选择目标文件
        QMessageBox::information(this, "WAINING","Need to choose a hookedFile!");
        return;
    }else if(hookedFile==hookInjectorStr){  //目标文件为注射器
        QMessageBox::information(this, "WAINING","HookedFile cannot be Injector itself!");
        return;
    }

    QStringList args(hookedFile);   //添加目标文件参数
    if(withAlloc) args.append("WithAlloc"); //添加WithAlloc参数

    //启动注射器进程
    injectorProcess = new QProcess();
    injectorProcess->start(hookInjectorStr,args);
    if(!injectorProcess->waitForStarted()){
        qDebug()<<"Injector run fail\n";
    }

    ui->progressBar->reset();   //清空进度条
    hookPages.clear();  //清空页数
    hookOpt.clear();    //清空输出
    qDebug()<<"pagebefor:"<<hookPages.size()<<endl;
    //注射器进程输出触发getHookOpt函数
    connect(injectorProcess,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(getHookOpt())
            );

}

//是否打开HeapAlloc
void MainWindow::on_HeapAllocCheckBox_clicked(bool checked) {
    if(checked){
        withAlloc=true;
    }else{
        withAlloc=false;
    }
}
