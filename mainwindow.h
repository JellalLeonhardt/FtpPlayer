#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QScrollBar>
#include <QFileDialog>
#include <QTextStream>
#include <QModelIndex>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPalette>
#include <QIcon>
#include <QDebug>
#include <iostream>
#include <vector>
#include <ftpmanager.hpp>
#include <qmyvideowidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ftpManager *_ftp, QWidget *parent);
    ~MainWindow();
    void buildTree(std::vector<std::string> _root, QStandardItem *_item);

private slots:
    void on_pushFile_clicked();

    void on_listSelected_clicked(const QModelIndex &index);

    void on_listSelected_doubleClicked(const QModelIndex &index);

    void on_pushFullScreen_clicked();

    void onStateChanged(QMediaPlayer::State state);

    void onDurationChanged(qint64 duration);

    void onPositionChanged(qint64 position);

    void on_pushPlay_clicked();

    void on_pushPause_clicked();

    void on_pushStop_clicked();

    void on_horizontalSliderVolume_valueChanged(int value);

    void on_pushVoice_clicked();

    void on_horizontalSliderProcess_valueChanged(int value);

    void on_actionRefresh_triggered();

    void on_pushList_clicked();

private:
    void buildModel();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *treeModel;
    QModelIndex treeModelIndex;

    QMediaPlayer *player;
    QString durationTime;
    QString positionTime;

    ftpManager *ftp;
    std::vector<std::string> root;
};

#endif // MAINWINDOW_H
