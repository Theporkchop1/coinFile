#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <mypushbutton.h>
#include<QTimer>
#include <QSoundEffect> //多媒体模块下的音效头文件
#include <QAudioDevice>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320,588);
    ui->menubar->setSizeIncrement(320,20);

    //设置图标
    setWindowIcon(QIcon("://res/Coin0001.png"));

    //设置标题
    setWindowTitle("CoinFlie");

    //退出按钮实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //开始按钮音效
    QSoundEffect * startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile("://res/TapButtonSound.wav"));

    //开始按钮实现
    MyPushButton * startBtn = new MyPushButton("://res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    //监听选择关卡的返回按钮信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();//将选择关卡场景隐藏
        this->show();//重新显示主场景
    });

    //监听
    connect(startBtn,&MyPushButton::clicked,[=](){
        //播放开始音效
        startSound->play();
        //弹起特效
        startBtn->zoom1();
        startBtn->move(this->width()*0.5 - startBtn->width()*0.5,this->height()*0.7);
        startBtn->zoom2();

        //延时进入到选择关卡场景中
        QTimer::singleShot(400,this,[=](){
            //进入到选择关卡场景
            //设置chooseScene场景位置
            chooseScene->setGeometry(this->geometry());
            //自身隐藏
            this->hide();
            //显示选择管卡场景
            chooseScene->show();
        });
    });
}

    void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("://res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,25,this->width(),this->height(),pix);

    //画背景上图标
    pix.load("://res/Title.png");
    pix = pix.scaled(pix.width()*0.7,pix.height()*0.7);
    painter.drawPixmap(10,35,pix);
}

MainScene::~MainScene()
{
    delete ui;
}
