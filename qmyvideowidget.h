#ifndef QMYVIDEOWIDGET_H
#define QMYVIDEOWIDGET_H
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QKeyEvent>

class QMyVideoWidget : public QVideoWidget
{
public:
    QMyVideoWidget(QWidget *parent = Q_NULLPTR) :QVideoWidget(parent){}
    ~QMyVideoWidget() = default;
    void setMediaPlayer(QMediaPlayer *player);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QMediaPlayer *thePlayer;

};

#endif // QMYVIDEOWIDGET_H
