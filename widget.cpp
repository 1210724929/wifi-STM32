#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer=NULL;
    sockfd=NULL;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(to_connect()));
}
void Widget::to_connect()
{

    QString ip="192.168.4.1";
    int port=9000;
    if(sockfd==NULL)
    {
        sockfd=new QTcpSocket();
        sockfd->connectToHost(ip,port);//连接到网址
        connect(sockfd,SIGNAL(connected()),this,SLOT(su_connect()));//连接成功后
        connect(sockfd,SIGNAL(readyRead()),this,SLOT(recv_env()));//准备读取套接字
    }
}
void Widget::su_connect()
{
    qDebug()<<"connect success";

    char req_buf[] = "连接成功";
    ui->label_4->setText(req_buf);
}

void Widget::recv_env()
{
    int ret = 0;
    char recv_buf[7] = {0};
    //sockfd->flush();
    ret=sockfd->read(recv_buf, sizeof(recv_buf));

    ui->label->setText(recv_buf);
    //ui->label->setText(QString::number(ret));
    //对接收到的字符串进行判断处理，例如对接收到的第一个字符比较，E进行温湿度处理，S进行坐姿处理
    char *p=recv_buf;
    //温湿度计数据解析
    if(strstr(recv_buf,"E")!=NULL)
    {
        sscanf(p, "E%dt%dt", &tem, &hum);
        char env_buf[5] = {0};
        sprintf(env_buf, "%d c", tem);
        ui->lineEdit_3->setText(env_buf);//接收数据，显示温度c
        sprintf(env_buf, "%d RH", hum);
        ui->lineEdit_4->setText(env_buf);
    }
    //坐姿状态数据解析
    else if((p=strstr(recv_buf,"S"))!=NULL)
    {
        sscanf(p,"S%dt",&stat);
        if(stat==1)
        {
            ui->label_8->setText("坐姿正确了");
            ui->label_7->setStyleSheet("border-image:url(:/img/right.png)");
        }
        else if(stat==0)
        {
            ui->label_8->setText("坐姿错误了");
            ui->label_7->setStyleSheet("border-image:url(:/img/error.png)");
        }      
    }
     memset(recv_buf,0,sizeof(recv_buf));
     sockfd->flush();
}
void Widget::on_pushButton_2_clicked()//控制灯亮
{
    char req_buf[8] = "LED1_ON";
    sockfd->write(req_buf, sizeof(req_buf));
    sockfd->flush();
}
void Widget::on_pushButton_3_clicked()//控制灯灭
{
    char req_buf[9] = "LED1_OFF";
    sockfd->write(req_buf, sizeof(req_buf));
    sockfd->flush();
}
void Widget::on_pushButton_4_clicked()//控制嗡鸣器
{
    char req_buf[9] = "BUFF_ALE";
    sockfd->write(req_buf, sizeof(req_buf));
    sockfd->flush();
}
void Widget::on_pushButton_5_clicked()//设置时间和温度提醒
{
    char req_buf[6] = {0};
    int text_tem = ui->lineEdit_5->text().toInt();//温度
    int text_set= ui->lineEdit_6->text().toInt();//时间
    sprintf(req_buf,"C%dt%dt",text_tem,text_set);
    if(text_tem<=35&&text_tem>=20&&text_set<=220&&text_set>=10)
    {
        sockfd->write(req_buf, sizeof(req_buf));
    }
    else
    {
        text_tem=27;
        text_set=30;
        ui->lineEdit_5->setText(QString::number(text_tem));
        ui->lineEdit_6->setText(QString::number(text_set));
        sockfd->write(req_buf, sizeof(req_buf));
    }
    sockfd->flush();
}

Widget::~Widget()
{
    delete ui;
}
