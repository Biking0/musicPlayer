//网络请求获取歌曲分类
#ifndef GETBAIDUCHANNEL_H
#define GETBAIDUCHANNEL_H

#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QEventLoop>
//获取浏览器Cookie信息
#include <QNetworkCookieJar>

//分类信息格式结构体
typedef struct _CHANNEL_INFO
{
    QString channelId;
    QString channelName;
}CHANNEL_INFO;

class GetBaiduChannel : public QFrame
{
    Q_OBJECT

public:
    //显示构造函数,参数类型强制转换
    explicit GetBaiduChannel(QFrame *parent = 0, QNetworkCookieJar *cookie = NULL);

//signals:

public slots:

public:
    //返回歌曲分类列表
    QList<CHANNEL_INFO> getMusicChannel();

private:
    QNetworkCookieJar *m_cookJar;
};

#endif // GETBAIDUCHANNEL_H
