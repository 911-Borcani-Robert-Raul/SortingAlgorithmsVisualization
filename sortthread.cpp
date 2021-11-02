#include "sortthread.h"
#include <algorithm>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QThread>
#include <QWidget>

#define DefaultRectColor QColor(155, 155, 155)
#define SelectedRectColor QColor(0, 102, 0)
#define QuickSortRectPivot Qt::black

SortThread::SortThread(Ui::MainWindow *ui)
    : ui{ui}, pause{false}
{
}

void SortThread::run()      // Acesta functie este locul din care incepe sa ruleze noul thread.
{
    switch (ui->cbSortMethod->currentIndex())
    {
    case 0: QuickSort(0, a.size() - 1);
            break;
    case 1: SelectionSort();
            break;
    case 2: BubbleSort();
            break;
    case 3: InsertionSort();
            break;
    case 4: MergeSort(0, a.size() - 1);
            break;
    }

    for (int i = 0; i < a.size(); ++i)
        col[i] = Qt::green;

    emit ReadyToDraw(a, col);

    for (int i = 0; i < a.size(); ++i)
        col[i] = DefaultRectColor;
}

void SortThread::UpdateWhileSoring(QVector<int> pos)    // Aceasta functie imi redeseneaza fereastra pe masura ce sirul este sortat
{                                                       // pentru a putea vedea progresul sortarii.
    for (int i = 0; i < pos.size(); ++i)
        col[pos[i]] = SelectedRectColor;

    emit ReadyToDraw(a, col);
    QThread::msleep(20);

    for (int i = 0; i < pos.size(); ++i)
        col[pos[i]] = DefaultRectColor;
}

void SortThread::CheckForPause()
{
    mutex.lock();
    if (pause)
    {
        waitCond.wait(&mutex);
    }
    mutex.unlock();
}

void SortThread::SelectionSort()
{
    for (int i = 0; i < a.size(); ++i)
        for (int j = i + 1; j < a.size(); ++j)
        {
            CheckForPause();

            if (a[i] > a[j])
                std::swap(a[i], a[j]);

            UpdateWhileSoring({i, j});
        }
}

void SortThread::QuickSort(int i, int j)
{
    if (i >= j)
        return;

    int pos = (i + j) / 2;
    int pivot = a[pos];
    int i1{i}, i2{j};

    while (i <= j)
    {
        while (i <= j && a[i] < pivot)
        {
            CheckForPause();

            ++i;
            UpdateWhileSoring({i, j});
        }

        while (i <= j && a[j] > pivot)
        {
            CheckForPause();

            --j;
            UpdateWhileSoring({i, j});
        }

        if (i <= j)
        {
            std::swap(a[i], a[j]);
            UpdateWhileSoring({i, j});
            ++i; --j;
        }
    }

    if (i1 < j)
        QuickSort(i1, j);
    if (i < i2)
        QuickSort(i, i2);
}

void SortThread::BubbleSort()
{
    bool sorted{false};

    int cnt{0};
    while (!sorted)
    {
        sorted = true;
        ++cnt;

        for (int i = 0; i < a.size() - cnt; ++i)
        {
            CheckForPause();

            UpdateWhileSoring({i, i + 1});

            if (a[i] > a[i + 1])
            {
                std::swap(a[i], a[i + 1]);
                sorted = false;

                UpdateWhileSoring({i, i + 1});
            }
        }
    }
}

void SortThread::InsertionSort()
{
    int i = 1;

    while (i < a.size())
    {
        CheckForPause();

        int j = i;

        UpdateWhileSoring({j, i});

        while (j > 0 && a[j - 1] > a[j])
        {
            CheckForPause();

            std::swap(a[j - 1], a[j]);
            j = j - 1;

            UpdateWhileSoring({j, i});
        }

        ++i;
    }
}

void SortThread::MergeSort(int i, int j)
{
    CheckForPause();

    if (i == j)
        return;
    if (i + 1 == j)
    {
        if (a[i] > a[j])
        {
            std::swap(a[i], a[j]);
            UpdateWhileSoring({j});
        }
        return;
    }

    int mid = (i + j) / 2;
    MergeSort(i, mid);
    MergeSort(mid + 1, j);

    QVector<int> b(j - i + 1);
    for (int k = i; k <= j; ++k)
        b[k - i] = a[k];

    int i1 = 0, i2 = mid + 1 - i, k = i;
    while (i1 <= mid - i && i2 <= j - i)
    {
        CheckForPause();

        if (b[i1] < b[i2])
        {
            a[k++] = b[i1++];
        }
        else
        {
            a[k++] = b[i2++];
        }

        UpdateWhileSoring({k - 1});
    }

    while (i1 <= mid - i)
    {
        a[k++] = b[i1++];
        UpdateWhileSoring({k - 1});
    }
    while (i2 <= j - i)
    {
        a[k++] = b[i2++];
        UpdateWhileSoring({k - 1});
    }
}

void SortThread::UpdateSize()
{
    a = QVector<int>(ui->sbSize->value());
    col = QVector<QColor>(ui->sbSize->value());
    for (int i = 0; i < a.size(); ++i)
    {
        a[i] = i + 1;
        col[i] = DefaultRectColor;
    }

    Shuffle();
}

void SortThread::Shuffle()
{
    std::random_shuffle(a.begin(), a.end());
    emit ReadyToDraw(a, col);
}

void SortThread::Pause()
{
    mutex.lock();       // Este nevoie de un mutex pentru a accesa variabila pause in timp ce si thread-ul de sortare functioneaza
    pause = true;
    mutex.unlock();
}

void SortThread::Resume()
{
    mutex.lock();
    pause = false;
    mutex.unlock();
    waitCond.wakeAll();
}

void SortThread::on_pbStart_clicked()
{
    run();
}

void SortThread::on_pbShuffle_clicked()
{
    Shuffle();
}
