////标题栏
//#include<titlewidget.h>

//#include <QDebug>

//TitleWidget::TitleWidget(QWidget *parent) :
//    QFrame(parent),
//    ui(new Ui::TitleWidget)
//{
//    ui->setupUi(this);

//}

//TitleWidget::~TitleWidget()
//{
//    delete ui;
//}

////鼠标按下事件
//void TitleWidget::mousePressEvent(QMouseEvent *event)
//{
//    // 如果是鼠标左键按下
//        if(event->button() == Qt::LeftButton){

//            // 使鼠标指针暂时变为小手抓取的样子
//            QCursor cursor;
//            cursor.setShape(Qt::ClosedHandCursor);
//            QApplication::setOverrideCursor(cursor);

//            // 获取指针位置和窗口位置的差值，以便移动时使用
//            offset = event->globalPos() - pos();
//        }

//        // 如果是鼠标右键按下
//        else if(event->button() == Qt::RightButton){

//            // 使用自定义的图片作为鼠标指针
//            QCursor cursor(QPixmap("../yafeilinux.png"));
//            QApplication::setOverrideCursor(cursor);
//        }
//}

////鼠标移动事件
//void TitleWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    if (m_Drag && (event->buttons() && Qt::LeftButton)) {
//            move(event->globalPos() - m_DragPosition);
//            event->accept();
//        }
//}

////鼠标释放事件
//void TitleWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    m_Drag = false;
//}











