#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT

    //执行动画的标志
    static bool isAnimation;
public:
    // explicit MyCoin(QWidget *parent = nullptr);

    //参数表示传进来的是金币参数还是银币参数
    MyCoin(QString btnImg);

    //金币的属性
    int posX;//X坐标
    int posY;//Y坐标
    bool flag;//正反标志

    //改变正反面标志
    void changeFlag();
    QTimer *timer1;//正->反
    QTimer *timer2;//反->正
    int min = 1;
    int max = 8;

    //是否胜利
    bool isWin = false;

    //重写按下和释放
    void mousePressEvent(QMouseEvent *e);
signals:
};

#endif // MYCOIN_H
