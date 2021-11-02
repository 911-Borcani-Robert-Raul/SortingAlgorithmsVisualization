#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include "sortthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Draw(QVector<int> a, QVector<QColor> col);

private slots:
    void on_FinishedSorting();

    void on_pbShuffle_clicked();

    void on_sbSize_valueChanged(const QString &arg1);

    void on_pbStart_clicked();

    void on_pbPause_clicked();

    void on_pbResume_clicked();

private:
    void SetStyleSheets();

private:
    Ui::MainWindow *ui;
    SortThread* sortThread;         // Sortarea are loc pe un thread diferit, astfel incat sa nu se blocheze aplicatia in timpul sortarii
    QGraphicsScene* scene;
};

#endif // MAINWINDOW_H
