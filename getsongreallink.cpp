//获得歌曲地址

#include "getsongreallink.h"

//歌曲加载地址
const QString downLoadLinkUrl = "http://music.baidu.com/data/music/fmlink?type=mp3&rate=320&songIds=";

GetSongRealLink::GetSongRealLink(QObject *parent, QNetworkCookieJar *cookie) : QObject(parent)
{
    m_cookJar = cookie;
}

//根据歌曲id获取歌曲播放地址
SONG_INFO GetSongRealLink::getSongRealLinkById(QString songId)
{
    qDebug() << "开始获取歌曲链接地址";
    QNetworkRequest networkRequest;
    QNetworkAccessManager networkManager;

    networkRequest.setUrl(QUrl(downLoadLinkUrl + songId));
    networkManager.setCookieJar(m_cookJar);

    QNetworkReply *reply = networkManager.get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    //歌曲信息
    SONG_INFO songInfo;
    QByteArray buf = reply->readAll();


    QJsonParseError jsonError;
    QJsonDocument jsonRes = QJsonDocument::fromJson(buf, &jsonError);;


    if(jsonError.error == QJsonParseError::NoError)
    {
        if(jsonRes.isObject())
        {
            QVariantMap result = jsonRes.toVariant().toMap();
            QVariantMap dataList = result["data"].toMap();

            foreach(QVariant data, dataList["songList"].toList())
            {
                QVariantMap linkMap = data.toMap();


                songInfo.songRealLink = linkMap["songLink"].toString();
                songInfo.songName = linkMap["songName"].toString();
                songInfo.artistName = linkMap["artistName"].toString();
                songInfo.songPicSmall = linkMap["songPicSmall"].toString();
                songInfo.songPicRadio = linkMap["songPicRadio"].toString();
                songInfo.albumName = linkMap["albumName"].toString();
                songInfo.lrcLink = linkMap["lrcLink"].toString();
            }
        }
    }

    return songInfo;
}

