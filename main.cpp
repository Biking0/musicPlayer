//播放器主函数

#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHotkey>

//主函数入口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    //热键设置
    QHotkey hotkey(QKeySequence("ctrl+alt+Q"), true);//The hotkey will be automatically registered
    qDebug() << "Is Registered: " << hotkey.isRegistered();

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Hotkey Activated - the application will quit now";
        qApp->quit();
    });


    QHotkey hotkeyPrev(QKeySequence("ctrl+left"), true);//The hotkey will be automatically registered
    QObject::connect(&hotkeyPrev, &QHotkey::activated, &w, &Widget::on_prevtoolButton_clicked);


    QHotkey hotkeyNext(QKeySequence("ctrl+right"), true);//The hotkey will be automatically registered
    QObject::connect(&hotkeyNext, &QHotkey::activated, &w, &Widget::on_nexttoolButton_clicked);



    QDesktopWidget *pDesk = QApplication::desktop();
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 8);

    //设置图标
    w.setWindowIcon(QIcon(":/img/baiduFm.png"));
    //设置窗口大小
    w.resize(QSize( 800, 600 ));
    w.setWindowOpacity(1);
//    w.setWindowFlags(Qt::FramelessWindowHint);
//    w.setAttribute(Qt::WA_TranslucentBackground);
//    w.move(200,100);

    w.show();

    return a.exec();
}
