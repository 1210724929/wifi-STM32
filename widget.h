#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void to_connect();
    void su_connect();

    void recv_env();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();


private:
    Ui::Widget *ui;
    QTcpSocket *sockfd;
    QTimer *timer;
    int tem,hum,stat;
};

#endif // WIDGET_H
