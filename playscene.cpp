#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QFont>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include<QSoundEffect>

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}

PlayScene::PlayScene(int levelNumber)
{
    QString str = QString("进入了第%1关").arg(levelNumber);
    qDebug()<<str;
    this->levelIndex = levelNumber;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap("://res/Coin0001"));
    //设置标题
    QString str1 = QString("第%1关").arg(levelNumber);
    this->setWindowTitle(str1);

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

    //返回按钮音效
    QSoundEffect * backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile("://res/BackButtonSound.wav"));
    //翻金币音效
    QSoundEffect * flipSound = new QSoundEffect(this);
    flipSound->setSource(QUrl::fromLocalFile("://res/ConFlipSound.wav"));
    //胜利音效
    QSoundEffect * winSound = new QSoundEffect(this);
    winSound->setSource(QUrl::fromLocalFile("://res/LevelWinSound.wav"));

    //返回按钮
    MyPushButton * backBtn = new MyPushButton("://res//BackButton.png","://res//BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"点击了返回";
        //告诉主场景我返回了,主场景监听chooselevelscene返回按钮
        //返回按钮音效播放
        backSound->play();
        //延时返回
        QTimer::singleShot(200,this,[=](){
            emit this->choosePlaySceneBack();
        });
    });

    //显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str2 = QString("Level:%1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(str2);
    // label->setFixedSize();
    // label->move(100,500);
    label->setGeometry(30, this->height() - 50 , 120, 50);


    DataConfig config;
    //初始化每个关卡的二维数组
    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片
    QLabel * winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load("://res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5,-tmpPix.height());

    //显示金币背景的图案
    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++)
        {
            QLabel * label1 = new QLabel;
            label1->setParent(this);
            label1->setGeometry(0,0,50,50);
            label1->setPixmap(QPixmap("://res/BoardNode.png"));
            label1->move(57 + i*50,200 + j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = "://res/Coin0001.png";
            }
            else
            {
                //显示银币
                str = "://res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50,204 + j*50);

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];//1正面 0反面

            //将金币放入金币的二维数组方便维护
            coinBtn[i][j] = coin;

            //点击金币 翻转
            connect(coin,&MyCoin::clicked,[=](){
                flipSound->play();
                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                //周围金币翻转
                //右侧金币
                if(coin->posX + 1 <= 3)
                {
                    coinBtn[coin->posX+1][coin->posY]->changeFlag();
                    this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                }
                //左侧金币
                if(coin->posX - 1 >= 0)
                {
                    coinBtn[coin->posX-1][coin->posY]->changeFlag();
                    this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 0 ? 1 : 0;
                }
                //上侧金币
                if(coin->posY - 1 >= 0)
                {
                    coinBtn[coin->posX][coin->posY-1]->changeFlag();
                    this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1] == 0 ? 1 : 0;
                }
                //下侧金币
                if(coin->posY + 1 <= 3)
                {
                    coinBtn[coin->posX][coin->posY+1]->changeFlag();
                    this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1] == 0 ? 1 : 0;
                }

                //判断是否胜利
                this->isWin = true;
                for(int i = 0 ; i < 4 ; i++)
                {
                    for(int j = 0 ; j < 4 ; j++)
                    {
                        if(coinBtn[i][j]->flag == false)//有一个是反面就失败
                        {
                            this->isWin = false;
                            break;
                        }
                    }
                }
                if(this->isWin == true)
                {
                    //胜利了
                    winSound->play();
                    qDebug()<<"游戏胜利";
                    //将所有按钮胜利标志改为true
                    for(int i = 0 ; i < 4 ; i++)
                    {
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            coinBtn[i][j]->isWin = true;
                        }
                    }

                    //将胜利图片弹出
                    QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                    //设置时间间隔
                    animation->setDuration(1000);
                    //设置开始位置
                    animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                    //设置结束位置
                    animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                    //设置曲线
                    animation->setEasingCurve(QEasingCurve::OutBounce);
                    //执行动画
                    animation->start();
                }
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("://res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,25,this->width(),this->height(),pix);

    //加载标题
    pix.load("://res/Title.png");
    pix = pix.scaled(pix.width()*0.7,pix.height()*0.7);
    painter.drawPixmap(20,35,pix);
}