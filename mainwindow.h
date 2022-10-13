#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QPushButton>
#include <QMutexLocker>
#include <QMutex>

class Worker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *pushButton1;
    QPushButton *pushButton2;
    QThread workerThread;
    Worker *worker;

private slots:
    void pushButton1Clicked();
    void pushButton2Clicked();
    void handleResults(const QString &);

signals:
    void startWork(const QString &);
};


class Worker : public QObject                   //继承QObject的类
{
    Q_OBJECT

public:
    void stopWork();

private:
    QMutex lock;        //互斥锁
    bool isCanRun;      //标志位

public slots:
    void doWork1(const QString &parameter);

signals:
    void resultReady(const QString &result);
};

#endif // MAINWINDOW_H
