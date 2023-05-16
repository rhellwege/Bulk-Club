#include "salesreportwidget.h"
#include "ui_salesreportwidget.h"

SalesReportWidget::SalesReportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesReportWidget)
{
    ui->setupUi(this);
}

SalesReportWidget::~SalesReportWidget()
{
    delete ui;
}
