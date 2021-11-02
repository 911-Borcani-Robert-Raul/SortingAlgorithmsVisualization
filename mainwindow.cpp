#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sortthread.h"
#include <algorithm>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QThread>
#include <QMessageBox>

#define DefaultRectColor QColor(155, 155, 155)
#define SelectedRectColor QColor(0, 102, 0)
#define QuickSortRectPivot Qt::black
#define DefaultStyleSheet "font-size: 20px;"

void MainWindow::SetStyleSheets()
{
    ui->lblArraySize->setStyleSheet(DefaultStyleSheet);
    ui->lblSortMethod->setStyleSheet(DefaultStyleSheet);
    ui->cbSortMethod->setStyleSheet(DefaultStyleSheet);
    ui->pbShuffle->setStyleSheet(DefaultStyleSheet);
    ui->sbSize->setStyleSheet(DefaultStyleSheet);
    ui->pbStart->setStyleSheet(DefaultStyleSheet);
    ui->pbResume->setStyleSheet(DefaultStyleSheet);
    ui->pbPause->setStyleSheet(DefaultStyleSheet);
}

void MainWindow::Draw(QVector<int> a, QVector<QColor> col)
{
    int W = ui->graphicsView->width();
    int H = ui->graphicsView->height();
    int w = W / ui->sbSize->value();
    int h = H / a.size();

    QBrush brush;
    QPen pen(Qt::black);

    scene = new QGraphicsScene(this);       // Desenez totul pe un obiect de tip QGraphicsScene
    for (int i = 0; i < a.size(); ++i)
    {
        brush = QBrush(col[i]);
        scene->addRect(i * w, H - a[i] * h, w, a[i] * h, pen, brush);
    }

    ui->graphicsView->setScene(scene);      // si setez scena pe graphicsView-ul de pe fereastra,
    repaint();                              // dupa care redesenez fereastra.
}

void MainWindow::on_FinishedSorting()
{
    ui->pbShuffle->setEnabled(true);
    ui->pbStart->setEnabled(true);
    ui->pbPause->setEnabled(false);
    ui->pbResume->setEnabled(false);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sortThread(new SortThread(ui)),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    setWindowTitle("Algoritmi de sortare");
    setWindowIcon(QIcon(":/icons/icon.ico"));

    SetStyleSheets();

    ui->sbSize->setMaximum(250);
    ui->sbSize->setMinimum(1);
    ui->sbSize->setValue(1);
    ui->pbPause->setEnabled(false);
    ui->pbResume->setEnabled(false);

    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QVector<QColor>>("QVector<QColor>");
    connect(sortThread, &SortThread::ReadyToDraw,
            this, &MainWindow::Draw);
    connect(sortThread, &SortThread::finished,
            this, &MainWindow::on_FinishedSorting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbShuffle_clicked()
{
    sortThread->Shuffle();
}

void MainWindow::on_sbSize_valueChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    sortThread->UpdateSize();
}

void MainWindow::on_pbStart_clicked()
{
    ui->pbShuffle->setEnabled(false);
    ui->pbStart->setEnabled(false);
    ui->pbPause->setEnabled(true);
    ui->pbResume->setEnabled(true);

    sortThread->start();
}

void MainWindow::on_pbPause_clicked()
{
    sortThread->Pause();
}

void MainWindow::on_pbResume_clicked()
{
    sortThread->Resume();
}
