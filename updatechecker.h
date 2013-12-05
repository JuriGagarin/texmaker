#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QDebug>
#include <QDesktopServices>
#include "texmakerversion.h"

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = 0);

    void setTimeout(unsigned int timeout);
    unsigned int getTimeout() const;

    const TexmakerVersion& getCurrentVersion() const;


    /**
     * @brief getWebVersion
     * @return the web version, may be non valid if checkForNewVersion was not called before or there is no internet connection
     */
    const TexmakerVersion& getWebVersion() const;

    QNetworkReply::NetworkError getLatestError() const;
    /**
     * @brief isErrorPresent a simple convenience function for checking if an error occured
     * @return false if NoError is available, else true
     */
    bool isErrorPresent() const;

    bool hasBeenChecked() const;

    static void openDownloadPage();

signals:
    void availableWebVersion(TexmakerVersion version);
    void newVersionAvailable(TexmakerVersion version);
    void currentVersionIsLatest();
    void error(QNetworkReply::NetworkError error);

public slots:
    /**
     * @brief checkForNewVersion triggers a check for a new version either you will get an newVersionAvailable, currentVersionIsLatest or an error signal as a result
     */
    void checkForNewVersion();
    /**
     * @brief stopVersionCheck this will stop checkForNewVersion() and fire an error signal with QNetworkReply::TimeoutError as an error
     */
    void stopVersionCheck();

    void gotoDownloadPage();

private slots:
    void processWebResult();

protected:
    virtual void setLatestError(QNetworkReply::NetworkError networkError, bool autoFireSignal = true);

private:
    TexmakerVersion _currentVersion;
    TexmakerVersion _webVersion;
    QNetworkAccessManager _manager;
    QNetworkReply       * _reply;
    QTimer _timer;
    QNetworkReply::NetworkError _latestError;
    unsigned int _timeout;
    bool _checked;

};

#endif // UPDATECHECKER_H
