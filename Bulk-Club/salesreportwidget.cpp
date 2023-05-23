#include "salesreportwidget.h"
#include "ui_salesreportwidget.h"

#include "customdialog.h"

SalesReportWidget::SalesReportWidget(QWidget *parent, BulkClubDatabase* db) :
    QWidget(parent),
    ui(new Ui::SalesReportWidget)
{
    ui->setupUi(this);
    this->db = db;

    // set up the models for the table views:
    modelItems = new SalesReportItemsModel(this, db);
    proxyItems = new FilterItemsProxy(this, db); // set up the proxy for sorting and filtering the model
    proxyItems->setSourceModel(modelItems);
    //proxyItems->setFilterKeyColumn(0); // filter based on the date
    //proxyItems->setFilterFixedString(ui->dateEdit->text());
    proxyItems->setFilterDate(ui->dateEdit->text());
    proxyItems->setFilterMemberType(ui->comboBoxFilter->currentText());
    updateTotalRevenue();
    ui->tableViewItemsSold->setModel(proxyItems);

    modelShoppers = new SalesReportShoppersModel(this, db);
    modelShoppers->updateUnique(ui->dateEdit->text());
    proxyShoppers = new QSortFilterProxyModel(this);
    proxyShoppers->setSourceModel(modelShoppers);
    proxyShoppers->setFilterKeyColumn(1); // set filter by type
    countShoppers();
    ui->tableViewShoppers->setModel(proxyShoppers);
}

SalesReportWidget::~SalesReportWidget()
{
    delete ui;
}

void SalesReportWidget::updateTotalRevenue()
{
    totalRevenue = 0;
    for (int row = 0; row < proxyItems->rowCount(); ++row)
    {
        QModelIndex idx = proxyItems->index(row, 0);
        totalRevenue += (*db->transactions())[proxyItems->mapToSource(idx).row()].total(); // get the actual transaction from the filtered index
    }
    // subtract losses from tax:
    totalRevenue = totalRevenue + (totalRevenue * TAX_RATE);
    QString fmtTotal = QString("Total Revenue (Including tax): $%1").arg(totalRevenue);
    ui->labelTotalRevenue->setText(fmtTotal);
}

void SalesReportWidget::countShoppers()
{
    regularCount = 0;
    executiveCount = 0;
    for (int row = 0; row < proxyShoppers->rowCount(); ++row)
    {
        int id = modelShoppers->at(proxyShoppers->mapToSource(proxyShoppers->index(row,0)).row());
        Member *member = db->members()->findId(id);
        if (member == nullptr) continue;
        QString memberType = member->type;
        //qDebug() << "testing: " << memberType;
        if (memberType.contains("Executive")) executiveCount++;
        else if (memberType == "Regular") regularCount++;
    }
    QString fmtCount = QString("# Executive Members: %1\t# Regular Members: %2").arg(executiveCount).arg(regularCount);
    ui->labelUniqueShoppers->setText(fmtCount);
}

void SalesReportWidget::on_dateEdit_userDateChanged(const QDate &date)
{
    proxyItems->setFilterDate(ui->dateEdit->text());
    updateTotalRevenue();

    modelShoppers->updateUnique(ui->dateEdit->text());
    countShoppers();
}


void SalesReportWidget::on_comboBoxFilter_currentIndexChanged(int index)
{
    // TODO: make custom sorting filter class for shoppers like I did with items
    QString curText = ui->comboBoxFilter->currentText();
    if (curText == "Any")
    {
        proxyShoppers->setFilterWildcard("*");

    }
    else
    {
        proxyShoppers->setFilterFixedString(curText);
    }
    countShoppers();

    proxyItems->setFilterMemberType(curText);
    updateTotalRevenue();
}

void SalesReportWidget::dbUpdated()
{
    qDebug() << "Database updated in sales report.";
    modelItems->reset();
    modelShoppers->reset();
    countShoppers();
    updateTotalRevenue();
}


void SalesReportWidget::on_buttonAddTransaction_clicked()
{
    CustomDialog d("Add Transaction", this);
    Transaction transaction;

    string date;
    string memberID;
    string item;
    string price;
    string qty;

    d.addLineEdit("Date: ", &date);
    d.addLineEdit("ID: ", &memberID);
    d.addLineEdit("Item: ", &item, "Full name");
    d.addLineEdit("Price: ", &price);
    d.addLineEdit("Qty: ", &qty);

    d.exec();

    if (d.wasCancelled()) return;
    transaction.date = QString::fromStdString(date);
    transaction.memberID = atoi(memberID.c_str());
    transaction.item = QString::fromStdString(item);
    transaction.price = atof(price.c_str());
    transaction.qty = atoi(qty.c_str());
    db->addTransaction(transaction);
}

