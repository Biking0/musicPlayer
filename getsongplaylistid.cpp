//获取歌曲播放列表

#include "getsongplaylistid.h"

const QString playListUrl = "http://fm.baidu.com/dev/api/?tn=playlist&format=json&id=";

//设置cookie浏览器信息
GetSongPlayListId::GetSongPlayListId(QObject *parent, QNetworkCookieJar *cookie) : QObject(parent)
{

    m_cookJar = cookie;
}

//获取歌曲播放列表
QList<QString> GetSongPlayListId::getSongIdList(QString channelId)
{
    qDebug() << "开始获取歌曲播放列表";
    QNetworkRequest networkRequest;
    QNetworkAccessManager *networkManager = new QNetworkAccessManager;

    networkRequest.setUrl(QUrl(playListUrl + channelId));

    networkManager->setCookieJar(m_cookJar);

    QNetworkReply *reply = networkManager->get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    QList<QString> songId;
    QByteArray buf = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonRes = QJsonDocument::fromJson(buf, &jsonError);;

    if(jsonError.error == QJsonParseError::NoError)
    {
        if(jsonRes.isObject())
        {
            QVariantMap result = jsonRes.toVariant().toMap();

            foreach(QVariant song, result["list"].toList())
            {
                QVariantMap songMap = song.toMap();
                QString id = songMap["id"].toString();

                //如果歌曲ID有效
                if(!id.isEmpty())
                {
                    songId.append(id);
                }

            }
        }
    }

//    qDebug() << buf;

    return songId;
}

