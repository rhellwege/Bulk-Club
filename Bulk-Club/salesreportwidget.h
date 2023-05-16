#ifndef SALESREPORTWIDGET_H
#define SALESREPORTWIDGET_H

#include <QWidget>


namespace Ui {
class SalesReportWidget;
}

class SalesReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SalesReportWidget(QWidget *parent = nullptr);
    ~SalesReportWidget();

private:
    Ui::SalesReportWidget *ui;
};

#endif // SALESREPORTWIDGET_H
