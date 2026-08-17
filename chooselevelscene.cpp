#include "chooselevelscene.h"
#include <QMenuBar>
#include <QMenu>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include "playscene.h"
#include <QSoundEffect>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap("://res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡");

    //创建菜单栏
    QMenuBar * bar = new QMenuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");
    //点击实现退出
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡音效
    QSoundEffect * chooseSound = new QSoundEffect(this);
    chooseSound->setSource(QUrl::fromLocalFile("://res/TapButtonSound.wav"));
    //返回按钮音效
    QSoundEffect * backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile("://res/BackButtonSound.wav"));

    //返回按钮
    MyPushButton * backBtn = new MyPushButton("://res//BackButton.png","://res//BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"点击了返回";
        //告诉主场景我返回了,主场景监听chooselevelscene返回按钮
        //播放返回按钮音效
        backSound->play();
        //延时返回
        QTimer::singleShot(200,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //创建选择关卡的按钮
    for(int i = 0 ; i < 20 ;i++)
    {
        MyPushButton * menuBtn = new MyPushButton("://res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move( 25 + i%4 * 70 , 150 + i/4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            //播放选择关卡音效
            chooseSound->play();

            QString str = QString("您选择的是第%1关").arg(1+i);
            qDebug()<<str;

            //进入到游戏场景
            this->hide(); //将选关场景隐藏掉
            play  = new PlayScene(i+1);

            //设置游戏场景初始位置
            play->setGeometry(this->geometry());
            play->show();

            connect(play,&PlayScene::choosePlaySceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = nullptr;
            });
        });

        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(1+i));
        label->move( 25 + i%4 * 70 , 150 + i/4 * 70);
        //设置label上的文字对齐方式
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透 51好属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

//重写绘图事件
void ChooseLevelScene::paintEvent(QPaintEvent *event)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("://res/OtherSceneBg.png");
    painter.drawPixmap(0,25,this->width(),this->height(),pix);

    //加载标题
    pix.load("://res/Title.png");
    painter.drawPixmap(20,35,pix);
}
