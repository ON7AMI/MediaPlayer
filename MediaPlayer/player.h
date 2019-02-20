#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QDir>
#include <QThread>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>
#include <QCoreApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QEventLoop>


namespace Ui {
class Player;
}

class Player : public QMainWindow
{
    Q_OBJECT


public:
    explicit Player(QWidget *parent = 0);
    ~Player();

private:
    Ui::Player *ui;

    int VorigePersWissel;
    int PixTeller;
    int VidTeller;
    int VidAantal;
    int PixSwitch;
    int SecondenTeller;
    int SecondenWissel;

    QTimer* PixTimer;

    QDir* PixDir;
    QDir* VidDir;
    QStringList filelist;
    QList<int> ori, shuf;

    QGraphicsOpacityEffect *effect1, *effect2;
    QPropertyAnimation* animation1, *animation2;

    QVideoWidget* videoWidget;
    QMediaPlayer* player;

    void ShowPictures();

private slots:
    void SecondenTimer();
};

#endif // PLAYER_H
