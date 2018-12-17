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
    QRect screenres = QApplication::desktop()->screenGeometry(1/*screenNumber*/);
    this->move(QPoint(screenres.x(), screenres.y()));
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->showFullScreen();
    this->showFullScreen();

    VorigePersWissel = -1;
    PixTeller = 0;
    PixSwitch = 0;

    effect1 = new QGraphicsOpacityEffect(this);
    effect2 = new QGraphicsOpacityEffect(this);

    ui->lbPix_1->setGraphicsEffect(effect1);
    PixDir = new QDir();
    PixTimer = new QTimer(this);
    connect(PixTimer, SIGNAL(timeout()), this, SLOT(ShowPictures()));
    PixTimer->start(15000);
    ui->lbPix_1->setWindowFlags(Qt::Window);
    ui->lbPix_1->setScaledContents(true);
    ui->lbPix_1->showFullScreen();
    ShowPictures();

}

Player::~Player()
{
    PixTimer->stop();
    delete ui;
    delete PixTimer;

}

void Player::ShowPictures()
{
    QString m_sSettingsFile = QApplication::applicationDirPath() + "\\appSettings.ini";
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);

    QString AfbeeldingMap = settings.value("Diverse/KassaAfbeeldingenMap", "").toString();

    if (AfbeeldingMap.isEmpty())
    {
        AfbeeldingMap = "/Show";
        settings.setValue("Diverse/KassaAfbeeldingenMap", AfbeeldingMap );

    }

    QString path;
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
    }

    if (PixTeller <= PixDir->entryList().count())
    {
        path =PixDir->dirName();
        QString filename = QString("%1/%2").arg(AfbeeldingMap).arg(PixDir->entryList()[--PixTeller]);

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
