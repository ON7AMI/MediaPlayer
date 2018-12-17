#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QDir>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>

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
    int PixSwitch;

    QTimer* PixTimer;

    QDir* PixDir;
    QStringList filelist;

    QGraphicsOpacityEffect *effect1, *effect2;
    QPropertyAnimation* animation1, *animation2;

private slots:
    void ShowPictures();

};

#endif // PLAYER_H
