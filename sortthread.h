#ifndef SORTTHREAD_H
#define SORTTHREAD_H

#include <QObject>
#include <QThread>
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

class SortThread : public QThread
{
    Q_OBJECT

    void CheckForPause();

signals:
    void ReadyToDraw(QVector<int> a, QVector<QColor> col);

public:
    SortThread(Ui::MainWindow *ui);
    void run() override;

    void SelectionSort();
    void QuickSort(int i, int j);
    void BubbleSort();
    void InsertionSort();
    void MergeSort(int i, int j);
    void UpdateWhileSoring(QVector<int> pos);
    void UpdateSize();
    void Shuffle();
    void Pause();
    void Resume();

    Ui::MainWindow *ui;
    QVector<int> a;
    QVector<QColor> col;
    QGraphicsScene *scene;
    QMutex mutex;
    bool pause;
    QWaitCondition waitCond;
private slots:
    void on_pbStart_clicked();
    void on_pbShuffle_clicked();
};

#endif // SORTTHREAD_H
