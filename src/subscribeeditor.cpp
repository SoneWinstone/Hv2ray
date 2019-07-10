#include "subscribeeditor.h"
#include <QFile>
#include <QDebug>

#include "ui_subscribeeditor.h"

#include "mainwindow.h"
#include "db.h"
#include "vmess.h"
#include "HttpRequest.h"

subscribeeditor::subscribeeditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::subscribeeditor),
    request(this),
    iSelectRow(-1)
{
    ui->setupUi(this);

    table = ui->tableSubscribeURL;
    init();
}

subscribeeditor::~subscribeeditor()
{
    delete ui;
}

void subscribeeditor::init()
{
    table->setAutoScroll(true);
//    table->resizeColumnsToContents();
    // hide 'id' column
    table->hideColumn(table->columnCount() - 1);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setMinimumWidth(100);
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    updateSubscribes();

    connect(ui->btnSubscribeURLAdd, &QPushButton::clicked, this, &subscribeeditor::on_btnSubscribeURLAdd_triggered);
    connect(ui->btnSubscribeUpdate, &QPushButton::clicked, this, &subscribeeditor::on_btnSubscribeUpdate_triggered);
    connect(ui->btnSubscribeDel, &QPushButton::clicked, this, &subscribeeditor::on_btnSubscribeDel_triggered);
    connect(ui->btnSave, &QPushButton::clicked, this, &subscribeeditor::saveSubscribes);
    connect(&request, &HttpRequest::httpRequestFinished, this, &subscribeeditor::onHttpRequestFinished);
    connect(table, &QTableWidget::itemChanged, this, &subscribeeditor::onItemChanged);
    connect(table, &QTableWidget::itemPressed, this, &subscribeeditor::onItemPressed);
//    connect(ui->btnSubscribeURLAdd, &QPushButton::clicked, [](bool) {});
}

void subscribeeditor::updateSubscribes() {
    DB database;
    database.query("select COUNT(*) from subscribes;");
    if (database.myQuery.first()) {
        int rows = database.myQuery.value(0).toInt();
        table->setRowCount(rows);
        database.query("select * from subscribes;");
        if (database.myQuery.first()) {
            for (int i = 0; i < rows; ++i) {
                table->setItem(i, 0, new QTableWidgetItem(database.myQuery.value(2).toString()));
                table->setItem(i, 1, new QTableWidgetItem(database.myQuery.value(1).toString()));
                table->setItem(i, 2, new QTableWidgetItem(database.myQuery.value(0).toString()));
                if (i < rows - 1) {
                    database.myQuery.next();
                }
            }
        }
    }
}

void subscribeeditor::saveSubscribes()
{
    DB db;
    for (int row : isetChangedRowIndex) {
        QStringList args;
        args << table->item(row, 0)->text()
             << table->item(row, 1)->text()
             << table->item(row, 2)->text();
        db.prepare("update subscribes set name = ?, url = ? where id = ?", args);
//        updateSubscribes();
    }
}

int subscribeeditor::getCurrentSubId()
{
    if (-1 != iSelectRow) {
        return table->item(iSelectRow, 2)->text().toInt();
    }
    return iSelectRow;
}

const QString subscribeeditor::getCurrentSubAdd()
{
    assert(-1 != iSelectRow);
    return table->item(iSelectRow, 1)->text();
}

void subscribeeditor::deleteOldSubVmess(const int subId)
{
    DB db;
    QStringList ids;

    QString queryString = "select id from confs where subscribe_id = " + QString(subId);
    db.query(queryString);
    if (db.myQuery.first()) {
        do {
            ids << db.myQuery.value(0).toString();
        } while (db.myQuery.next());
    }

    queryString = "delete from confs where id in (" + ids.join(',') + ")";
    db.query(queryString);
    if (ids.size() > 0) {
        for (QString id : ids) {
            QString rmFile = "conf/" + id + ".conf";
            QFile::remove(rmFile);
        }
    }

//    emit updateConfTableSignal();
}

void subscribeeditor::deleteOldSubVmess(const QString &subAdd)
{
    DB db;
    QStringList ids;

    QString queryString = "select id from confs where subscribe_id in (select id from subscribes where url = '" + subAdd +"')";
    db.query(queryString);
    if (db.myQuery.first()) {
        do {
            ids << db.myQuery.value(0).toString();
        } while (db.myQuery.next());
    }

    queryString = "delete from confs where subscribe_id in (select id from subscribes where url = '" + subAdd +"')";

    db.query(queryString);
    if (ids.size() > 0) {
        for (QString id : ids) {
            QString rmFile = "conf/" + id + ".conf";
            QFile::remove(rmFile);
        }
    }

//    emit updateConfTableSignal();
}

void subscribeeditor::on_btnSubscribeURLAdd_triggered(bool checked)
{
    Q_UNUSED(checked);
    QString url = ui->leSubscribeURL->text().trimmed();

    DB database;
    QStringList args;
    if (ui->leSubName->text().trimmed().isEmpty()) {
        args << "anonymous";
    } else {
        args << ui->leSubName->text().trimmed();
    }
    args << url;

    database.prepare("insert into subscribes (name, url) values (?, ?)", args);

    updateSubscribes();
}

void subscribeeditor::on_btnSubscribeDel_triggered()
{
    if (-1 != iSelectRow) {
        DB database;
        database.query("delete from subscribes where id = " + table->item(iSelectRow, table->columnCount() - 1)->text());

        updateSubscribes();
    }
}

void subscribeeditor::on_btnSubscribeUpdate_triggered()
{
    if (-1 != iSelectRow) {
        QString url = table->item(iSelectRow, 1)->text().trimmed();
        request.get(url);
    }
}

void subscribeeditor::onItemPressed(QTableWidgetItem *item)
{
    iSelectRow = item->row();
}

void subscribeeditor::onItemChanged(QTableWidgetItem *item)
{
    Q_UNUSED(item);

    isetChangedRowIndex.insert(iSelectRow);
}

void subscribeeditor::onHttpRequestFinished(QByteArray &data)
{
    QString vmesses = QByteArray::fromBase64(data, QByteArray::Base64UrlEncoding);
    QStringList vmessList = vmesses.split("\n");
    if (vmessList.size() > 0 && vmessList[0].startsWith("vmess")) {
        deleteOldSubVmess(getCurrentSubAdd());
    }
    DB db;
    vmess *inVmess = new vmess(this);
    for (QString vmessStr : vmessList) {
        if (vmessStr.startsWith("vmess://")) {
            inVmess->addVmess(vmessStr, getCurrentSubId());
        }
    }

    data.clear();
}

void subscribeeditor::updateConfTable()
{
    emit updateConfTableSignal();
}
