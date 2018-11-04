#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"

MainWindow::MainWindow(ftpManager *_ftp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ftp(_ftp)
{
    ui->setupUi(this);
    ui->actionRefresh->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));
    treeModel = new QStandardItemModel(ui->listSelected);

    player = new QMediaPlayer(this);
    player->setNotifyInterval(2000);
    player->setVideoOutput(ui->video);
    ui->video->setMediaPlayer(player);
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));

    QPalette pal(ui->video->palette());
    pal.setColor(QPalette::Background, Qt::black);
    ui->video->setAutoFillBackground(true);
    ui->video->setPalette(pal);

    root = ftp->getList("/");
    buildModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTree(std::vector<std::string> _root, QStandardItem *_item)
{
    QStandardItem * tmp;
    tmp = new QStandardItem();
    tmp->setText(tr(".."));
    tmp->setEditable(false);
    _item->appendRow(tmp);
    std::string name;
    for(std::vector<std::string>::iterator i = _root.begin(); i != _root.end(); i++){
        name = (*i).substr(2);
        tmp = new QStandardItem();
        tmp->setText(name.c_str());
        tmp->setEditable(false);
        tmp->setWhatsThis(_item->whatsThis() + tr("/") + QString::fromStdString(name));
        _item->appendRow(tmp);
        if((*i).at(0) == 'd'){
            tmp->setToolTip(tr("d"));
            tmp->setIcon(*(new QIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon))));
            std::cout << "check : " + tmp->whatsThis().toStdString() << std::endl;
            buildTree(ftp->getList(tmp->whatsThis().toStdString()), tmp);
        }
        else if((*i).find_last_of('.') != (unsigned int)-1 && name.substr((*i).find_last_of('.') - 1) == "mp4" ){
            tmp->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight)));
        }
    }
}

void MainWindow::onStateChanged(QMediaPlayer::State state)
{
    ui->pushPlay->setEnabled(!(state == QMediaPlayer::PlayingState));
    ui->pushPause->setEnabled((state == QMediaPlayer::PlayingState));
    ui->pushStop->setEnabled((state == QMediaPlayer::PlayingState));
}

void MainWindow::onDurationChanged(qint64 duration)
{
    ui->horizontalSliderProcess->setMaximum(duration);
    int secs = duration / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    durationTime = QString::asprintf("%d:%d", mins, secs);
    ui->labelProcess->setText(positionTime + "/" + durationTime);
}

void MainWindow::onPositionChanged(qint64 position)
{
    if(ui->horizontalSliderProcess->isSliderDown()){
        return;
    }
    ui->horizontalSliderProcess->setSliderPosition(position);
    int secs = position / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    positionTime = QString::asprintf("%d:%d", mins, secs);
    ui->labelProcess->setText(positionTime + "/" + durationTime);
}

void MainWindow::buildModel()
{
    delete treeModel;
    treeModel = new QStandardItemModel(ui->listSelected);
    treeModel->clear();
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("目录"));
    QStandardItem *item = new QStandardItem(tr("/"));
    item->setEditable(false);
    item->setWhatsThis("");
    item->setToolTip(tr("d"));
    treeModel->appendRow(item);
    buildTree(root, item);
    ui->listSelected->setModel(treeModel);
    ui->listSelected->setRootIndex(item->index());
    ui->listSelected->setWordWrap(true);
    ui->listSelected->setTextElideMode(Qt::ElideNone);
}

void MainWindow::on_pushFile_clicked()
{
    QStandardItem *item = treeModel->itemFromIndex(treeModelIndex);
    std::string where = item->whatsThis().toStdString();
    ui->labelWhat->setText(item->text());
    player->setMedia(QUrl(QString::fromStdString(ftp->getUrl(where))));
    //player->setMedia(QUrl("./1538573915926.mp4"));
}

void MainWindow::on_listSelected_clicked(const QModelIndex &index)
{
    treeModelIndex = index;
}

void MainWindow::on_listSelected_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = treeModel->itemFromIndex(index);
    if(item->toolTip() == "d") ui->listSelected->setRootIndex(index);
    if(item->text() == ".." && item->parent() != nullptr && item->parent()->parent() != nullptr) {
        ui->listSelected->setRootIndex(item->parent()->parent()->index());
    }
}

void MainWindow::on_pushFullScreen_clicked()
{
    ui->video->setFullScreen(true);
    //player->setTime(30);
}

void MainWindow::on_pushPlay_clicked()
{
    player->play();
}

void MainWindow::on_pushPause_clicked()
{
    player->pause();
}

void MainWindow::on_pushStop_clicked()
{
    player->stop();
}

void MainWindow::on_horizontalSliderVolume_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::on_pushVoice_clicked()
{
    bool mute = player->isMuted();
    player->setMuted(!mute);
    if(!mute){
        ui->pushVoice->setIcon(QIcon(":/icons/icons/mute.bmp"));
    }
    else{
        ui->pushVoice->setIcon(QIcon(":/icons/icons/volumn.bmp"));
    }
}

void MainWindow::on_horizontalSliderProcess_valueChanged(int value)
{
    player->setPosition(value);
}

void MainWindow::on_actionRefresh_triggered()
{
    buildModel();
}

void MainWindow::on_pushList_clicked()
{
    static bool status = true;
    status = !status;
    if(status){
        ui->pushList->setIcon(QIcon(":/icons/icons/610.bmp"));
        ui->listSelected->show();
    }
    else{
        ui->pushList->setIcon(QIcon(":/icons/icons/612.bmp"));
        ui->listSelected->hide();
    }
}
