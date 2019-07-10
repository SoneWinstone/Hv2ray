#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = 0);
//    HttpRequest();
    ~HttpRequest();
    void setUrl(const QString& url);
    void setHeader(const QByteArray& key, const QByteArray& value);
    // get
    void get(const QString& url);
    // insert
    void post(const QString& url, const QByteArray& data);
    // update
    void put(const QString& url, const QByteArray& data);
    // delete
    void del(const QString& url);
    void login(const QString& url, const QByteArray& data);
signals:
    void httpRequestFinished(QByteArray& data);

public slots:
    void onRequestFinished();
    void onReadyRead();

private:
    QByteArray                  data;
    QUrl                        url;
    QNetworkReply              *reply;
    QNetworkRequest             request;
    QNetworkAccessManager       accessManager;
//    QSettings                  *settings;
};

#endif // HTTPREQUEST_H
