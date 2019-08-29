#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bits/stdc++.h>
#include <QStandardItemModel>
#include <QTableView>
using namespace std;
const int LIST_NUM = 10;
namespace Ui {
class MainWindow;
}
struct node
{
    int startPos,blockSize;
    node(){}
    node(int pos,int size)
    {
        startPos = pos;
        blockSize = size;
    }

    node(int startPos)
    {
        this->startPos = startPos;
    }

    friend bool operator <(const node& a,const node& b)
    {
        return a.startPos < b.startPos;
    }

    bool operator()(node &a)
    {
        if(startPos==a.startPos)return true;
        else return false;
    }
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print_vector(vector<node> &v);
    void init(int heap_size,int min_block_size);
    int divide(int index);
    int getMemory(int index);
    int my_malloc(int requestSize);
    void merge_block(int startPos,int blockSize);
    int my_free(int pos);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_2_clicked();
    void on_pushButton_2_clicked();
    void on_radioButton_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

private:
    Ui::MainWindow *ui;

    int heapSize,minBlockSize,maxBlockSize;
    vector<node> freeList[LIST_NUM];
    vector<node> usedList;
    int heapRadio,blockRadio;
    int randomSize;
    QTableView *table;
    QStandardItemModel *model;
};

#endif // MAINWINDOW_H
