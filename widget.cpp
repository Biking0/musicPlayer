//主界面

#include "widget.h"
#include "ui_widget.h"
#include "lyrics.h"
#include "aristpicture.h"
#include "downloadfile.h"

#include <QPropertyAnimation>
#include <QSettings>
#include <QDir>
#include <QHotkey>

#include<QPalette>
#include<QBitmap>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    //设置无边框
    this->setWindowOpacity(0.5); //窗口整体透明度，0-1 从全透明到不透明
    this->setWindowFlags(Qt::FramelessWindowHint); //设置无边框风格
//    this->setAttribute(Qt::WA_TranslucentBackground); //设置背景透明，允许鼠标穿透

    // 设置外围widget的样式：border
    QWidget *widgetMain = new QWidget();
    widgetMain->setObjectName("widgetMain");
    widgetMain->setStyleSheet("ui#widgetMain{border: 1px solid #FF00FF; border-radius: 5px;}");

    //设置背景图片
//    QPixmap bgImages(":/img/back.png");
//    QPalette bgPalette=this->palette();
//    bgPalette.setBrush(QPalette::Background,bgImages);
//    this->setPalette(bgPalette);
//    setMask(bgImages.mask());

    //窗口大小适应控件大小
    //layout()->setSizeConstraint(QLayout::SetFixedSize);
    //空指针异常，重新设计布局
//    layout()->setAlignment( Qt::AlignHCenter);

    //设置标题栏为红色
    ui->titleWidget->setGeometry(0, 0, 800, 50);
    QPalette pal(ui->titleWidget->palette());
    pal.setColor(QPalette::Background, QColor(170,0,0));
    ui->titleWidget->setAutoFillBackground(true);
    ui->titleWidget->setPalette(pal);
    ui->titleWidget->show();

    m_cookJar = new QNetworkCookieJar;
    m_userLonIndlg = new LoginDlg(0, m_cookJar);
    m_channelWidget = new ChannelsWidget(0, m_cookJar);
    m_pPlayer = new QMediaPlayer(this);
    m_iCurrentSongIndex = 0;

    //隐藏音量进度条
    ui->volSlider->hide();
    //隐藏分类窗口
    //ui->channelwidget->hide();

    //退出按鈕
    ui->quitButton->setToolTip("退出");
    ui->quitButton->setStyleSheet("background-color:transparent;");

    //登录信号槽
    qDebug()<<"登录信号槽";
    connect(m_userLonIndlg, SIGNAL(loginSuccess(QString)), ui->aristPicWidget, SLOT(userLoninSlot(QString)));
    //歌曲分类信息槽
    connect(this, SIGNAL(channelInfoSignal(QString)), ui->aristPicWidget, SLOT(channelChangedSlot(QString)));
    //切换分类信息槽
    connect(ui->channelwidget, SIGNAL(channelChanged(CHANNEL_INFO)), this, SLOT(channelChangedSlot(CHANNEL_INFO)));
    //播放状态槽
    connect(m_pPlayer, &QMediaPlayer::mediaStatusChanged, this, &Widget::mediaStatusChanged);
    //播放进度条槽
    connect(m_pPlayer, &QMediaPlayer::positionChanged, this, &Widget::positionChanged);
    //播放进度条槽
    connect(m_pPlayer, &QMediaPlayer::durationChanged, this, &Widget::durationChanged);
    //歌词滚动槽
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), ui->lycWidget, SLOT(setTickLrc(qint64)));
    //音量进度条控制槽
    connect(ui->volSlider,  &QSlider::valueChanged, this, &Widget::volChannged);
    //移动播放进度条
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &Widget::setPosition);
    //暂停按钮槽
    connect(ui->aristPicWidget, SIGNAL(pauseSignal(bool)), this, SLOT(pauseSlot(bool)));
    //显示cd图片
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), ui->aristPicWidget, SLOT(setDefultPic(qint64)));    
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), ui->aristPicWidget, SLOT(positionChanged(qint64)));

    qDebug()<<"设置音量进度条大小";
    ui->volSlider->setRange(0, 100);
    ui->volSlider->setValue(60);

    //设置歌曲频道信息
    qDebug()<<"设置歌曲频道信息";
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    int iChannel = settings.value("ChannelIndex").toInt();
    int iCheck = settings.value("RememberMe").toInt();
    settings.endGroup();

    //登录
    if(iCheck)
    {
        m_userLonIndlg->logIn();
    }
    //选择频道列表
    ui->channelwidget->channelSelected(iChannel);
}
//播放暂停
void Widget::pauseSlot(bool pause)
{
    if(pause)
        m_pPlayer->pause();
    else
        m_pPlayer->play();
}
//音量
void Widget::volChannged(qint64 position)
{
    m_pPlayer->setVolume(position);
}


//播放状态
void Widget::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia /*|| status == QMediaPlayer::NoMedia*/)
    {
        qDebug() << "---------------------------";
        qDebug() << "播放完成";
        nextSong();
    }
}
//进度条
void Widget::durationChanged(qint64 duration)
{
    ui->progressSlider->setRange(0, duration);
}

void Widget::setPosition(qint64 position)
{
    m_pPlayer->setPosition(position);
}

void Widget::positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
}

//频道改变信号
void Widget::channelChangedSlot(CHANNEL_INFO channel)
{
    m_songIdList.clear();
    m_iCurrentSongIndex = 0;


    m_currentChannelId.channelId = channel.channelId;
    m_currentChannelId.channelName = channel.channelName;

    //初始化某个频道列表下的所有歌曲
    initSongIdList();
}

//初始化歌曲播放列表
void Widget::initSongIdList()
{
    //第二步,获取某个频道下的歌曲ID列表
    GetSongPlayListId getPlayListId(this, m_cookJar);
    m_songIdList = getPlayListId.getSongIdList(m_currentChannelId.channelId);

    qDebug() << "频道:" << m_currentChannelId.channelName << m_currentChannelId.channelId<< "共有歌曲" << m_songIdList.size();


    QString info = m_currentChannelId.channelName + " 共有歌曲: " + QString::number(m_songIdList.size());
    emit channelInfoSignal(info);
    play(0);
}

//销毁用户登录窗口
Widget::~Widget()
{
    delete m_userLonIndlg;
    delete ui;
}
//设置按钮，显示登录框
void Widget::on_settingtoolButton_clicked()
{
    m_userLonIndlg->showDlg();
}
//关闭登录框
void Widget::closeEvent(QCloseEvent *)
{
    m_userLonIndlg->close();
}
//音量进度条的显示与隐藏
void Widget::on_voltoolButton_clicked()
{
    if(ui->volSlider->isHidden())
    {
        ui->volSlider->show();
    }
    else
        ui->volSlider->hide();
}
//上一首按钮按下
void Widget::on_prevtoolButton_clicked()
{
    qDebug() << "上一首按钮按下";
    prevSong();
}
//下一首按钮按下
void Widget::on_nexttoolButton_clicked()
{
    qDebug() << "下一首按钮按下";
    nextSong();
}

//显示歌曲分类信息
//void Widget::on_showChanneltoolButton_clicked()
//{
//    if(ui->channelwidget->isHidden())
//    {
//        ui->channelwidget->show();

//        QPropertyAnimation *anim = new QPropertyAnimation(ui->channelwidget, "geometry");
//        anim->setDuration(1500);

//        anim->setEasingCurve(QEasingCurve::OutCubic);
//        ui->channelwidget->setVisible(true);
//        connect(anim, &QPropertyAnimation::finished, [this] () {
//            ui->channelwidget->setVisible(false);
//        });
//        anim->start(QPropertyAnimation::DeleteWhenStopped);

//    }
//    else
//        ui->channelwidget->hide();
//}

//上一首
void Widget::prevSong()
{
    qDebug() << "上一曲1";

    if(m_iCurrentSongIndex <= 0 || m_iCurrentSongIndex >= m_songIdList.size() - 1)
    {
        initSongIdList();

        qDebug() << m_iCurrentSongIndex;

        return;
    }

    m_iCurrentSongIndex--;

//    //判断是否为第一首，true返回,false播放上一首
//    if(m_iCurrentSongIndex<=0)
//        {
//        return;
//    }

    play(m_iCurrentSongIndex);
}

//下一首歌曲
void Widget::nextSong()
{
    qDebug() << "下一首" << m_iCurrentSongIndex << m_songIdList.size();
    if(m_iCurrentSongIndex < 0 || m_iCurrentSongIndex >= m_songIdList.size() - 1)
    {
        initSongIdList();
        return;
    }

    m_iCurrentSongIndex++;

    play(m_iCurrentSongIndex);
}

//音乐播放，获取歌词
void Widget::play(int index)
{
    if(index > m_songIdList.size())
        return;

    GetSongRealLink getSongLink;
    m_currentSongInfo = getSongLink.getSongRealLinkById(m_songIdList.at(index));

    qDebug()<<"歌曲播放地址："<<m_currentSongInfo.songRealLink;
    m_pPlayer->setMedia(QUrl(m_currentSongInfo.songRealLink));
    m_pPlayer->play();


    //获取歌词
    GetLrc getLrc;
    QString lrc = getLrc.getLrc(m_currentSongInfo.lrcLink);
    ui->lycWidget->currentSongChannged(m_currentSongInfo.songName, m_currentSongInfo.artistName, m_currentSongInfo.albumName);
    ui->lycWidget->setLrc(lrc);

    //显示歌手图片
    GetAristPic getPic;
    QPixmap picMap = getPic.getAristPic(m_currentSongInfo.songPicRadio);

    qDebug()<<"-----------------------";
    qDebug()<<m_currentSongInfo.songPicRadio;
    qDebug()<<picMap;

    ui->aristPicWidget->setAristPic(picMap);
}

//下载歌曲
void Widget::on_downloadtoolButton_clicked()
{
    QString fileName;
    DownLoadFile downMp3;

    QDir dir;
    if(!dir.exists("./download"))
    {
        dir.mkdir("./download");
    }
    fileName = "./download/" + m_currentSongInfo.songName + ".mp3";

    qDebug() << fileName;

    downMp3.getMp3File(fileName, m_currentSongInfo.songRealLink);

    qDebug() << "下载完成";
}

//点击播放按钮
void Widget::on_playButton_clicked()
{

    //判断歌曲状态，更改播放按钮图标
    if(CurrentState)
    {
        qDebug() << "点击暂停按钮";
        //设置按钮图标
        ui->playButton->setIcon(QIcon(":/img/play.svg"));
        //暂停音乐
        m_pPlayer->pause();
        //设置歌曲状态开关
        this->CurrentState=false;
    }
    else
    {
        qDebug() << "点击播放按钮";
        ui->playButton->setIcon(QIcon(":/img/pause.svg"));
        m_pPlayer->play();
        this->CurrentState=true;
    }
}

//退出程序
void Widget::on_quitButton_clicked()
{
    qDebug() << "退出程序";
    this->close();
}
