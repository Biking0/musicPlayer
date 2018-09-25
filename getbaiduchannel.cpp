//获取歌曲分类信息

#include "getbaiduchannel.h"

//宏定义，分类信息获取网址
const QString channelUrl = "http://fm.baidu.com/dev/api/?tn=channellist";

GetBaiduChannel::GetBaiduChannel(QFrame *parent, QNetworkCookieJar *cookie) : QFrame(parent)
{
    m_cookJar = cookie;
}

//获取歌曲分类信息
QList<CHANNEL_INFO> GetBaiduChannel::getMusicChannel()
{
    qDebug() << "开始获取歌曲分类信息";
    QList<CHANNEL_INFO> channelInfoList;

    //发送网络请求
    QNetworkRequest networkRequest;
    QNetworkAccessManager *networkManager = new QNetworkAccessManager;

    networkRequest.setUrl(QUrl(channelUrl));

    networkManager->setCookieJar(m_cookJar);

    //获取请求回复
    QNetworkReply *reply = networkManager->get(networkRequest);
    //事件循环
    QEventLoop loop;    
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //释放reply对象
    reply->deleteLater();
    //字节数组，接受返回数据
    QByteArray buf = reply->readAll();


    QJsonParseError jsonError;
    //转化为json格式数据
    QJsonDocument jsonRes = QJsonDocument::fromJson(buf, &jsonError);;

    //如果数据转换过程不出错
    if(jsonError.error == QJsonParseError::NoError)
    {
        //如果jsonRes不为空
        if(jsonRes.isObject())
        {
            //字典键值对
            QVariantMap result = jsonRes.toVariant().toMap();

            foreach(QVariant channel, result["channel_list"].toList())
            {
                QVariantMap channelMap = channel.toMap();


                CHANNEL_INFO channelInfo;
                channelInfo.channelId = channelMap["channel_id"].toString();
                channelInfo.channelName = channelMap["channel_name"].toString();

                channelInfoList.append(channelInfo);

            }
        }
    }

    return channelInfoList;
}
