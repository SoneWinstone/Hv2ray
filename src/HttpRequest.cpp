#include <QDebug>
#include <QByteArray>
#include "HttpRequest.h"

HttpRequest::HttpRequest(QObject *parent) : QObject(parent)
{
}

HttpRequest::~HttpRequest()
{
    accessManager.disconnect();
}

void HttpRequest::setUrl(const QString &url)
{
    QUrl qUrl = QUrl(url);
    if (!qUrl.isValid()) {
        qDebug() << "URL is invalid!\n";
    }
    request.setUrl(qUrl);
}

void HttpRequest::setHeader(const QByteArray& key, const QByteArray& value)
{
    request.setRawHeader(key, value);
}

void HttpRequest::get(const QString &url)
{
    this->setUrl(url);
//    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    reply = accessManager.get(request);

    connect(reply, &QNetworkReply::finished, this, &HttpRequest::onRequestFinished);
    connect(reply, &QNetworkReply::readyRead, this, &HttpRequest::onReadyRead);
}

void HttpRequest::post(const QString &url, const QByteArray &data)
{
    this->setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    reply = accessManager.post(request, data);

    connect(reply, &QNetworkReply::finished, this, &HttpRequest::onRequestFinished);
    connect(reply, &QNetworkReply::readyRead, this, &HttpRequest::onReadyRead);
}

void HttpRequest::put(const QString &url, const QByteArray &data)
{
    this->setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    reply = accessManager.put(request, data);

    connect(reply, &QNetworkReply::finished, this, &HttpRequest::onRequestFinished);
    connect(reply, &QNetworkReply::readyRead, this, &HttpRequest::onReadyRead);
}

void HttpRequest::del(const QString &url)
{
    this->setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    reply = accessManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished, this, &HttpRequest::onRequestFinished);
    connect(reply, &QNetworkReply::readyRead, this, &HttpRequest::onReadyRead);
}

void HttpRequest::login(const QString &url, const QByteArray &data)
{
    this->setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    reply = accessManager.post(request, data);

    connect(reply, &QNetworkReply::finished, this, &HttpRequest::onRequestFinished);
    connect(reply, &QNetworkReply::readyRead, this, &HttpRequest::onReadyRead);
}

void HttpRequest::onRequestFinished()
{
    emit httpRequestFinished(this->data);
}

void HttpRequest::onReadyRead()
{
    this->data += reply->readAll();
}
