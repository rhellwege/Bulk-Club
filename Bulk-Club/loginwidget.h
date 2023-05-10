#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void updatePermissions(QString loginStatus);

private slots:
    void on_pushButtonLogOut_clicked();

    void on_pushButtonLogin_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
