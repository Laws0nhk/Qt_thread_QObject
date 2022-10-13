#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);
    pushButton1 =new QPushButton(this);
    pushButton2 =new QPushButton(this);
    pushButton1->setGeometry(300, 200, 80, 40);
    pushButton2->setGeometry(400, 200, 80, 40);
    pushButton1->setText("开启线程");
    pushButton2->setText("打断线程");
    worker = new Worker;
    worker->moveToThread(&workerThread);        //将worker类移至线程workerThread
    //线程完成，销毁对象
    connect(&workerThread, SIGNAL(finished()),
            worker, SLOT(deleteLater()));
    connect(&workerThread, SIGNAL(finished()),
            &workerThread, SLOT(deleteLater()));

    connect(this, SIGNAL(startWork(QString)),       //发送开始工作的信号，开始工作
            worker, SLOT(doWork1(QString)));
    connect(worker, SIGNAL(resultReady(QString)),   //接收到worker发送过来的信号
            this, SLOT(handleResults(QString)));
    connect(pushButton1, SIGNAL(clicked()),         //开始线程
            this, SLOT(pushButton1Clicked()));
    connect(pushButton2, SIGNAL(clicked()),         //打断线程
            this, SLOT(pushButton2Clicked()));
}

MainWindow::~MainWindow()
{
    worker->stopWork();     //打断线程
    workerThread.quit();    //退出线程
    if (workerThread.wait(2000)) {
        qDebug()<<"线程结束"<<endl;
    }
}

void Worker::doWork1(const QString &parameter){     //耗时的工作都放在槽函数下
    isCanRun = true;//标志位，控制死循环
    while (1) {
/* 此{}作用是 QMutexLocker 与 lock 的作用范围，获取锁后，运行完成后即解锁 */
        {
            QMutexLocker locker(&lock);
            if(!isCanRun){
                break;
            }       
        }
/******************************************************************/
        QThread::sleep(2);
        emit resultReady(parameter + "doWork1函数");      //发送信号
    }
    emit resultReady("打断doWork1函数");               //发送信号
}

void Worker::stopWork(){                                    //stopWork
    qDebug() << "打断线程" << endl;
    QMutexLocker locker(&lock);
    isCanRun = false;
}

void MainWindow::pushButton1Clicked()                       //pushBtn1
{
    const QString str = "正在运行";     //parameter初始化为"正在运行"
    if(!workerThread.isRunning()) {    //线程未运行时，开始线程
        workerThread.start();
    }
    emit this->startWork(str);
}

void MainWindow::pushButton2Clicked(){                      //pushBtn2
    if(workerThread.isRunning()) {
        worker->stopWork();
    }
}

void MainWindow::handleResults(const QString & results)     //handleResults
{
    qDebug()<<"线程的状态："<<results<<endl;
}
