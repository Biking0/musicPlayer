#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QEventLoop>
//网络请求
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

//多媒体流播放库
#include <QMediaPlayer>
//用户登录
#include "logindlg.h"
#include "channelswidget.h"
#include "lyrics.h"
#include "volpannel.h"
#include "widget.h"
#include "aristpicture.h"
#include "getsongplaylistid.h"
#include "getsongreallink.h"
#include "getaristpic.h"
#include "getlrc.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

//按钮信号
public slots:
    //设置按钮
    void on_settingtoolButton_clicked();
    //音量按钮
    void on_voltoolButton_clicked();
    //显示或隐藏歌曲类型分类
//    void on_showChanneltoolButton_clicked();
    //歌曲类型分类子按钮
    void channelChangedSlot(CHANNEL_INFO channel);
    //下一首
    void on_nexttoolButton_clicked();
    //上一首
    void on_prevtoolButton_clicked();
    //暂停
    void pauseSlot(bool pause);
    //下载
    void on_downloadtoolButton_clicked();

private:
    Ui::Widget *ui;
    void closeEvent(QCloseEvent *event);


private:
    //流媒体对象
    QMediaPlayer *m_pPlayer;
    LoginDlg *m_userLonIndlg;
    CHANNEL_INFO m_currentChannelId;
    ChannelsWidget *m_channelWidget;
    QList <QString> m_songIdList;
    int m_iCurrentSongIndex;
    SONG_INFO m_currentSongInfo;
    //设置歌曲状态，改变播放按钮状态
    bool CurrentState;
    //鼠标事件
    bool m_Drag;

    QNetworkCookieJar *m_cookJar;
    void play(int index);
    void nextSong();
    void prevSong();
    void initSongIdList();
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(qint64 position);
    void volChannged(qint64 position);

    //添加鼠标事件
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void channelInfoSignal(QString channel);
private slots:
    void on_playButton_clicked();
    void on_quitButton_clicked();
};

#endif // WIDGET_H


