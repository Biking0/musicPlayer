//歌曲分类窗口

#include "channelswidget.h"
#include "ui_channelswidget.h"
#include <QPropertyAnimation>
#include <QSettings>

ChannelsWidget::ChannelsWidget(QWidget *parent, QNetworkCookieJar *cookJar) :
    QFrame(parent),
    ui(new Ui::ChannelsWidget)
{
    qDebug() << "显示歌曲分类窗口";
    ui->setupUi(this);
    setFrameStyle(QFrame::NoFrame);

    m_getChannelList = new GetBaiduChannel(0, cookJar);
    m_gridlayOut = new QGridLayout;

    m_channelList = getChannelList();

    qDebug() << "创建分类信息按钮";
    for(int i = 0, k = 0; i < m_channelList.size() / 4; i++, k += 4)
    {
        for(int j = 0; j < 4; j++)
        {
            QPushButton *btn = new QPushButton(this);

            btn->setText(m_channelList.at(k + j).channelName);
            //qDebug() << m_channelList.at(k + j).channelName;
            m_gridlayOut->addWidget(btn, i, j);
            //qDebug() << k + j;
            m_btnGroup.addButton(btn, k + j);
        }
//        break;
    }

    setLayout(m_gridlayOut);


    connect( &m_btnGroup, SIGNAL(buttonClicked (int)), this, SLOT(channelSelected(int)) );
    qDebug() << "默认分类信息为第一个";
    m_iLastChannelIndex = 0;
}

void ChannelsWidget::channelSelected(int iChannel)
{

    //先还原样式表
    m_btnGroup.button(m_iLastChannelIndex)->setStyleSheet("");

    m_btnGroup.button(iChannel)->setStyleSheet("QPushButton{ font-weight:600;color:#5500ff;}");
    m_iLastChannelIndex = iChannel;
    emit channelChanged(m_channelList.at(iChannel));

    //记录当前频道信息
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QBaiduFm", "QBaiduFm");
    settings.beginGroup("User");
    settings.setValue("ChannelId", m_channelList.at(iChannel).channelId);
    settings.setValue("ChannelName", m_channelList.at(iChannel).channelName);
    settings.setValue("ChannelIndex", iChannel);
    settings.endGroup();
}


ChannelsWidget::~ChannelsWidget()
{
    delete ui;
}

//光标移出分类信息窗口隐藏
//void ChannelsWidget::leaveEvent(QEvent *event)
//{
//    hide();
//}

QList<CHANNEL_INFO> ChannelsWidget::getChannelList()
{
    return m_getChannelList->getMusicChannel();
}
