//下载歌曲

#include "downloadfile.h"

DownLoadFile::DownLoadFile(QObject *parent) : QObject(parent)
{

}

//获取歌曲地址，开始下载
void DownLoadFile::getMp3File(QString fileName, QString url)
{
    qDebug() << "开始下载歌曲";
    //开始网络请求
    QNetworkRequest networkRequest;
    QNetworkAccessManager networkManager;

    networkRequest.setUrl(QUrl(url));

    QNetworkReply *reply = networkManager.get(networkRequest);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();

    //将返回数据写入文件
    QFile file;
    file.setFileName(fileName);
    bool bOk = file.open(QIODevice::WriteOnly);
    if(bOk)
    {
        file.write(reply->readAll());
    }
}

