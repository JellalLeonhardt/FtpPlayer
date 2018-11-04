#include <qmyvideowidget.h>

void QMyVideoWidget::setMediaPlayer(QMediaPlayer *player)
{
    thePlayer = player;
}

void QMyVideoWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape && isFullScreen()){
        setFullScreen(false);
        event->accept();
        QVideoWidget::keyPressEvent(event);
    }
}

void QMyVideoWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(thePlayer->state() == QMediaPlayer::PlayingState) thePlayer->pause();
        else thePlayer->play();
    }
    QVideoWidget::mousePressEvent(event);
}
