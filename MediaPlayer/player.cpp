#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include "player.h"
#include "ui_player.h"



#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QDialog>
#include <QDateTime>
#include <QDesktopWidget>


Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    videoWidget = new QVideoWidget;
    player = new QMediaPlayer;

    QRect screenres = QApplication::desktop()->screenGeometry(1/*screenNumber*/);
    this->move(QPoint(screenres.x(), screenres.y()));
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //this->showFullScreen();
    this->showFullScreen();

    SecondenWissel = 10;
    SecondenTeller = 1;

    VorigePersWissel = -1;
    PixTeller = 0;
    PixSwitch = 0;
    VidTeller = 0;

    effect1 = new QGraphicsOpacityEffect(this);
    effect2 = new QGraphicsOpacityEffect(this);

    ui->lbPix_1->setGraphicsEffect(effect1);
    PixDir = new QDir();
    VidDir = new QDir();
    PixTimer = new QTimer(this);
    connect(PixTimer, SIGNAL(timeout()), this, SLOT(SecondenTimer()));
    PixTimer->start(1000);

    ui->lbPix_1->setWindowFlags(Qt::Window);
    ui->lbPix_1->setScaledContents(true);
    ui->lbPix_1->showFullScreen();

    ShowPictures();

}

Player::~Player()
{
    PixTimer->stop();
    delete ui;
    delete videoWidget;
    delete player;
    delete PixTimer;

}

void Player::SecondenTimer()
{
    if(player->state() != QMediaPlayer::PlayingState)
    {
        player->stop();
        videoWidget->hide();

        if(SecondenTeller % SecondenWissel == 0)
        {
            ShowPictures();
            SecondenTeller = 1;
        }
        else
        {
            SecondenTeller++;
        }
    }
}

void Player::ShowPictures()
{
    QString m_sSettingsFile = QApplication::applicationDirPath() + "/appSettings.ini";
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);

    QString AfbeeldingMap = settings.value("Diverse/KassaAfbeeldingenMap", "").toString();
    int rest, v;


    if (AfbeeldingMap.isEmpty())
    {
        AfbeeldingMap = "P:/slides";
        // AfbeeldingMap = "/media/p/slides";
        // settings.setValue("Diverse/KassaAfbeeldingenMap", AfbeeldingMap );
    }

    QString path;
    QString vidpath;

    if (PixTeller == 0)
    {
        QStringList extentions;
        extentions.append("*.jpg");
        extentions.append("*.gif");
        extentions.append("*.png");
        PixDir->setNameFilters(extentions);
        PixDir->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        PixDir->setPath(AfbeeldingMap);
        PixTeller = PixDir->entryList().count();
        ori.clear();
        shuf.clear();
        for (int i = 0; i < PixTeller; i++)
            ori.append(i);

        srand (time(NULL));


        rest = ori.count();
        while (rest > 0)
        {
            v = rand() % rest;
            shuf.append(ori.at(v));
            ori.removeAt(v);
            rest = ori.count();
        }

        QStringList vidextentions;
        vidextentions.append("*.mp4");
        VidDir->setNameFilters(vidextentions);
        VidDir->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QString vidpath = AfbeeldingMap + "/../video";
        VidDir->setPath(vidpath);
        VidTeller = VidDir->entryList().count();
        VidAantal = VidTeller;
    }


    if (PixTeller % 5 == 0)
    {
        // Om de vijf foto's een video
        vidpath =VidDir->dirName();
        QString filename = QString("%1/../video/%2").arg(AfbeeldingMap).arg(VidDir->entryList()[--VidTeller]);
        if (VidTeller == 0)
            VidTeller = VidAantal;

        // QString filename = "P:\\slides\\..\\Video\\VloeistofFull_NL.mp4";

        // QVideoWidget* videoWidget;
        // videoWidget = new QVideoWidget;
        player->setVideoOutput(videoWidget);
        player->setMedia(QUrl::fromLocalFile(filename));


        videoWidget->setFullScreen(true);
        videoWidget->show();
        player->play();

    }

    if (PixTeller <= PixDir->entryList().count())
    {
        path =PixDir->dirName();
        QString filename = QString("%1/%2").arg(AfbeeldingMap).arg(PixDir->entryList()[shuf.at(--PixTeller)]);

        //if (!filename.isEmpty())
        animation1 = new QPropertyAnimation(effect1, "opacity");
        animation2 = new QPropertyAnimation(effect2, "opacity");

        QFileInfo check_file(filename);
        if (check_file.exists() && check_file.isFile())
        {
            QPixmap image(filename);

            ui->lbPix_1->setPixmap(image.scaled(ui->lbPix_1->width(), ui->lbPix_1->height(), Qt::KeepAspectRatio));
            //ui->lbPix_1->setVisible(true);
            //ui->lbPix_2->setVisible(true);
            animation2->setDuration(1000);
            animation2->setStartValue(1.0);
            animation2->setEndValue(0.0);
            //animation2->setEasingCurve(QEasingCurve::OutQuad);
            animation2->start();
            //ui->lbPix_1->setVisible(false);
            animation1->setDuration(1000);
            animation1->setStartValue(0.0);
            animation1->setEndValue(1.0);
            //animation1->setEasingCurve(QEasingCurve::OutQuad);
            animation1->start();
        }
    }
    else
    {
        PixTeller = 0;
    }

}
