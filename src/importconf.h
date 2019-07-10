#ifndef IMPORTCONF_H
#define IMPORTCONF_H

#include <QDialog>

namespace Ui
{
    class importConf;
}

class importConf : public QDialog
{
    Q_OBJECT

public:
    explicit importConf(QWidget *parent = nullptr);
    /**
     * @brief savefromFile
     * @param path
     * @param alias
     * @param subId subscribe id
     */
    void savefromFile(QString path, QString alias, const int subId = 0);
    ~importConf();

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
signals:
    void updateConfTable();

private:
    Ui::importConf *ui;
};

#endif // IMPORTCONF_H
