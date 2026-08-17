#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QPainter>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNumber);

    int levelIndex; //内部成员属性记录所选关卡

    //重写paintevent
    void paintEvent(QPaintEvent *event);

    int gameArray[4][4];//二维数组 维护每个关卡的具体数据

    MyCoin * coinBtn[4][4];

    //是否胜利
    bool isWin;

signals:
    void choosePlaySceneBack();
};

#endif // PLAYSCENE_H
