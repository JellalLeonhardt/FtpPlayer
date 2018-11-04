#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <iostream>
#include <string>

using namespace std;

loginWindow::loginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginWindow),
    ftp(Q_NULLPTR)
{
    ui->setupUi(this);

    QRegExp regx_port("[0-9]+");
    QValidator *validator_port = new QRegExpValidator(regx_port, ui->linePort );
    ui->linePort->setValidator(validator_port);

    QRegExp regx_ip("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QValidator *validator_ip = new QRegExpValidator(regx_ip, ui->lineIP );
    ui->lineIP->setValidator(validator_ip);

    QRegExp regx_name(".{3,30}");
    QValidator *validator_name = new QRegExpValidator(regx_name, ui->lineName );
    ui->lineIP->setValidator(validator_name);

    QRegExp regx_pass(".{3,30}");
    QValidator *validator_pass = new QRegExpValidator(regx_pass, ui->linePass );
    ui->lineIP->setValidator(validator_pass);

    ui->lineIP->setFocus();
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::startPlay()
{
    cout<< "start play" <<endl;
    MainWindow *m = new MainWindow(ftp, this);
    m->setWindowTitle("播放器");
    m->show();
    this->hide();
}

void loginWindow::on_pushLogin_clicked()
{
    int status = 0;
    if(ui->lineIP->hasAcceptableInput()){
        status += 1;
    }
    if(ui->linePort->hasAcceptableInput()){
        status += 2;
    }
    if(ui->lineName->hasAcceptableInput() && ui->lineName->text() != ""){
        status += 4;
    }
    if(ui->linePass->hasAcceptableInput() && ui->linePass->text() != ""){
        status += 8;
    }
    switch(status){
    case 3:
        ui->label->setText("请输入用户名和密码");
        break;
    case 12:
        ui->label->setText("请输入服务器地址和端口");
        break;
    case 15:
    {
        ui->label->setText("登录中");
        Sleep(200);
        string ip = ui->lineIP->text().toStdString();
        string name = ui->lineName->text().toStdString();
        string passwd = ui->linePass->text().toStdString();
        short port = ui->linePort->text().toShort();
        ftp = new ftpManager(name, passwd, ip, port);
        if(ftp->login()){
            startPlay();
        }
        break;
    }
    default:
        ui->label->setText("请完善信息");
    }
}

void loginWindow::on_pushCancel_clicked()
{
    this->close();
}
