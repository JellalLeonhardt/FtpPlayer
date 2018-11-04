#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "ftpmanager.hpp"

namespace Ui {
class loginWindow;
}

class loginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = 0);
    ~loginWindow();
    void startPlay();

private slots:
    void on_pushLogin_clicked();

    void on_pushCancel_clicked();

private:
    Ui::loginWindow *ui;
    ftpManager* ftp;
};

#endif // LOGINWINDOW_H
