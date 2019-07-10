#ifndef SUBSCRIBEEDITOR_H
#define SUBSCRIBEEDITOR_H

#include <set>
#include <QDialog>
#include <QTableWidget>
#include "HttpRequest.h"

namespace Ui
{
    class subscribeeditor;
}
//class HttpRequest;

class subscribeeditor : public QDialog
{
    Q_OBJECT

public:
    explicit subscribeeditor(QWidget *parent = nullptr);
    ~subscribeeditor();

signals:
    void updateConfTableSignal();

private slots:
    void on_btnSubscribeURLAdd_triggered(bool checked = false);
    void on_btnSubscribeDel_triggered();
    void on_btnSubscribeUpdate_triggered();
    void onItemPressed(QTableWidgetItem * item);
    void onItemChanged(QTableWidgetItem * item);
    void onHttpRequestFinished(QByteArray &data);
    void updateConfTable();

private:
    void init();
    void updateSubscribes();
    void saveSubscribes();
    int  getCurrentSubId();
    const QString getCurrentSubAdd();
    void deleteOldSubVmess(const int subId);
    void deleteOldSubVmess(const QString& subAdd);

private:
    Ui::subscribeeditor *ui;
    QTableWidget        *table;
    HttpRequest         request;
    int                 iSelectRow;
    std::set<int>       isetChangedRowIndex;
};

#endif // SUBSCRIBEEDITOR

