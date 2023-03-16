#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(350,200);
    setWindowFlags(Qt::FramelessWindowHint);    //无边框
    setAttribute(Qt::WA_NoMouseReplay);
    setAttribute(Qt::WA_TranslucentBackground);

    // 获取鼠标的位置并移动窗口到该位置
    QPoint pos = QCursor::pos();
    move(pos);
}

Widget::~Widget()
{
    delete ui;
}

//绘制圆角背景
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

//窗口外点击关闭窗口
bool Widget::event(QEvent *event)
{
    //当前事件的类型是窗口失去激活的事件类型（即窗口不再是活动窗口），那么执行下面的代码块。
    if (QEvent::WindowDeactivate == event->type())
    {
        this->close();
    }
    return QWidget::event(event);
}

