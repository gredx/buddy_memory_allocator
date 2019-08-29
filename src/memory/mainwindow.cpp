#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    heapSize = heapRadio =  256*1024;
    minBlockSize = blockRadio =  4;
    maxBlockSize = minBlockSize * (1<<(LIST_NUM-1));

    table = new QTableView();
    model = new QStandardItemModel();

    connect(ui->pushButton,&QPushButton::click,this,&MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_3,&QPushButton::click,this,&MainWindow::on_pushButton_2_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete table;
    delete model;
}
void MainWindow::print_vector(vector<node> &v)
{
    for(auto it = v.begin();it!=v.end();it++)
    {
        cout<<it->startPos<<" ";
    }cout<<endl;
}

void MainWindow::init(int heap_size,int min_block_size)
{
    usedList.clear();
    for(int i=0;i<LIST_NUM;i++) freeList[i].clear();
    heapSize = heap_size;
    minBlockSize = min_block_size;
    maxBlockSize = minBlockSize * (1<<(LIST_NUM-1));
    int index = 9;  // 块是最小块的倍数
    int block = minBlockSize*(1<<index);
    int processPos = 1;

    while(processPos+minBlockSize<=heapSize)
    {
        if(processPos+block<=heapSize){
            freeList[index].push_back(node(processPos,block));
            processPos += block;
        }else{
            while(processPos+block>heapSize&&index>=0){
                index--;
                block = minBlockSize * (1<<index);
            }

        }
    }
}

int MainWindow::divide(int index)
{
    if(index>=LIST_NUM) return 0;
    if(freeList[index].size()!=0){
        sort(freeList[index].begin(),freeList[index].end());
        node now = freeList[index][0];
        freeList[index].erase(freeList[index].begin());
        int block_size = now.blockSize/2;
        int start1 = now.startPos,start2 = start1+block_size;
        freeList[index-1].push_back(node(start1,block_size));
        freeList[index-1].push_back(node(start2,block_size));
        return 1;
    }else return divide(index+1);
}

int MainWindow::getMemory(int index)
{
    if(index>=LIST_NUM) {
        cout<<"无法获得更大的内存"<<endl;
        return 0;
    }
    if(freeList[index].size()!=0){
        // 直接分配
        sort(freeList[index].begin(),freeList[index].end());
        node node = freeList[index][0];
        freeList[index].erase(freeList[index].begin());
        usedList.push_back(node);
        return node.startPos;
    }else {
        int flag =  divide(index+1);
        if(flag==0) return 0; //无内存可分配
        return getMemory(index); // 分出了内存
    }
}

int MainWindow::my_malloc(int requestSize)
{
    // 根据需要的大小，从伙伴堆中分配最合适的内存
    int mallocSize=0;   // 存储幂次
    if(maxBlockSize<requestSize) {
        cout<<"没有足够的内存供分配"<<endl;
        cout<<"可分配的最大内存大小为:"<<maxBlockSize<<endl;
        return 0;
    }
    while((1<<mallocSize)*minBlockSize<requestSize) mallocSize++;
    // 从空闲表中获取一块内存
    int flag = getMemory(mallocSize);
}

void MainWindow::merge_block(int startPos,int blockSize)
{
    int index = (int)log2(blockSize/minBlockSize);
    cout<<"index "<<index<<endl;

    if(index>=LIST_NUM) return;
    auto it = freeList[index].begin();
    if(index<9){
        if(startPos%(2*blockSize)==1){
            it = find_if(freeList[index].begin(),freeList[index].end(),node(startPos+blockSize));
            if(it!=freeList[index].end()){
                cout<<"position will be merged:";
                cout<<it->startPos<<endl;

            }
        }else if(startPos%(2*blockSize)==blockSize+1){
            it = find_if(freeList[index].begin(),freeList[index].end(),node(startPos-blockSize));
            if(it!=freeList[index].end()){
                cout<<"position will be merged:";
                cout<<it->startPos<<endl;
                startPos = startPos - blockSize;
            }
        }
    }

    if(it==freeList[index].end()||index==9){
        freeList[index].push_back(node(startPos,blockSize));
        sort(freeList[index].begin(),freeList[index].end());
    }else {
        freeList[index].erase(it);
        merge_block(startPos,blockSize*2);
    }
}

int MainWindow::my_free(int pos)
{
    node now = usedList[pos];
    usedList.erase(usedList.begin()+pos);
    cout<<"position to be free:"<<now.startPos<<endl;
    merge_block(now.startPos,now.blockSize);
    return now.startPos;
}




void MainWindow::on_pushButton_clicked()
{
    // 初始化
    init(heapRadio,blockRadio);
}

void MainWindow::on_pushButton_3_clicked()
{
    // 申请内存,随机生成一个值(
    randomSize = rand()%(maxBlockSize/2) + 1;
    int flag = my_malloc(randomSize);
    ui->label_3->setText("申请内存大小:"+QString::number(randomSize));
    ui->label_4->setText("分配的内存块起始位置:"+QString::number(flag));

}

void MainWindow::on_pushButton_2_clicked()
{
       // 释放内存
    int size = usedList.size();
    if(size==0) {
           ui->label_5->setText("没有可释放的内存块");
    }else {
        int pos =  rand()%size;
        ui->label_5->setText("释放内存块的起始位置："+QString::number(usedList[pos].startPos)+"  内存块大小:"+QString::number(usedList[pos].blockSize));
        int x = my_free(pos);
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    model->clear();
    table->setModel(model);
    QStringList head = { "已分配"   };
    for(int i=0;i<LIST_NUM;i++){
        head.append(QString::number(minBlockSize*(1<<i)));
    }
    model->setHorizontalHeaderLabels(head);

    QStandardItem *newItem = 0;
    for(auto i=0;i<usedList.size();i++){
        newItem = new QStandardItem(QString::number(usedList[i].startPos)+","+QString::number(usedList[i].blockSize));
        model->setItem(i,0,newItem);
    }

    for(int i=0;i<LIST_NUM;i++){
        for(int j=0;j<freeList[i].size();j++){
            newItem = new QStandardItem(QString::number(freeList[i][j].startPos));
            model->setItem(j,i+1,newItem);
        }
    }
    table->show();
}


void MainWindow::on_radioButton_clicked()
{
    heapRadio = 256*1024;
}

void MainWindow::on_radioButton_2_clicked()
{
    heapRadio = 512*1024;
}

void MainWindow::on_radioButton_3_clicked()
{
    blockRadio = 1;
}

void MainWindow::on_radioButton_4_clicked()
{
    blockRadio = 2;
}

void MainWindow::on_radioButton_5_clicked()
{
    blockRadio = 4;
}
