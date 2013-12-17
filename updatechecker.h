#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QDesktopServices>
#include "texmakerversion.h"

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(int updateInterval, QDateTime dateLastChecked, QObject *parent = 0);

    /**
     * @brief setTimeout sets timeout for update checking, if time rouns out there will be a the error signal emitted
     * @see getTimeout()
     * @param timeout the time in ms
     */
    void setTimeout(unsigned int timeout);

    /**
     * @brief getTimeout returns the current timeout
     * @see setTimeout(unsigned int timeout)
     * @return the current timeout in ms
     */
    unsigned int getTimeout() const;

    /**
     * @brief getCurrentVersion this function will return the current running TexmakerVersion
     * @see TexmakerVersion::getCurrentVersion()
     * @info this is just a convenience function, the returned result should be equal to calling TexmakerVersion::getCurrentVersion()
     * @return
     */
    const TexmakerVersion& getCurrentVersion() const;

    /**
     * @brief getWebVersion a simple function to get the latest available version umber on the web
     * @info make sure to call checkForNewVersion() otherwise this function will return an invalid TexmakerVersion instance
     * @return the web version, may be non valid if checkForNewVersion was not called before or there is no internet connection
     */
    const TexmakerVersion& getWebVersion() const;

    /**
     * @brief getLatestError if an error occured while checking for a new version on the web you will get te type of error with this function
     * @return if no error occured NoError will be returned otherwise the type of error
     */
    QNetworkReply::NetworkError getLatestError() const;

    /**
     * @brief isErrorPresent a simple convenience function for checking if an error occured
     * @see getLatestError()
     * @return false if NoError is available, else true
     */
    bool isErrorPresent() const;

    /**
     * @brief getDateLastChecked simple function to get the DateTime when last check was called
     * @see checkForNewVersion()
     * @return the DateTime when the last check was run with the checkForNewVersion() function
     */
    const QDateTime& getDateLastChecked() const;

    /**
     * @brief isUpdateCheckRequired checks if last update is more days in the past as _lastChecked + _updateInterval (defined within the constructor)
     * @see UpdateChecker(int updateInterval, QDateTime dateLastChecked, QObject *parent = 0)
     * @return if _updateInterval == 0 will always return false, otherwise it checks _updateInterval + _updateInterval < current DateTime
     */
    bool isUpdateCheckRequired() const;

    /**
     * @brief hasBeenChecked determines if checkForNewVersion() has been called on this instance
     * @return true if checkForNewVersion() has been called
     */
    bool hasBeenChecked() const;

    /**
     * @brief openDownloadPage will open the download webpage with the systems default browser
     * @see gotoDownloadPage()
     */
    static void openDownloadPage();

signals:
    /**
     * @brief availableWebVersion wll be emmited if a check for a new version was sucessfull
     * @param version the latest available version on the web
     */
    void availableWebVersion(TexmakerVersion version);

    /**
     * @brief newVersionAvailable when emitted the web version is a newer as the current running version
     */
    void newVersionAvailable();

    /**
     * @brief currentVersionIsLatest the current running version is equal to the latest available version on the web. Nothing has to be done. ;)
     */
    void currentVersionIsLatest();

    /**
     * @brief error while checking for a new version an error occured
     * @param error defines the type of error
     * @param errorDescription a human readable description
     */
    void error(QNetworkReply::NetworkError error, QString errorDescription);

public slots:
    /**
     * @brief checkForNewVersion triggers a check for a new version either you will get an newVersionAvailable, currentVersionIsLatest or an error signal as a result
     */
    void checkForNewVersion();
    /**
     * @brief stopVersionCheck this will stop checkForNewVersion() and fire an error signal with QNetworkReply::TimeoutError as an error
     */
    void stopVersionCheck();

    /**
     * @brief checkForNewVersionIfRequired this function will check if an update check is required and only if true run a check for a new version
     * @see checkForNewVersion()
     * @see isUpdateCheckRequired()
     */
    void checkForNewVersionIfRequired();

    /**
     * @brief gotoDownloadPage same as openDownloadPage, but as a slot
     * @see openDownloadPage()
     */
    void gotoDownloadPage();

private slots:
    /**
     * @brief processWebResult parses the QNetworkReply _reply and wll emit the corresponding signal
     */
    void processWebResult();

protected:
    /**
     * @brief setLatestError manipulates the iternal state of the object if an error occured. Always call this method if something goes wrong otherwise some other functions may not work as expected.
     * @param networkError the type of error which occured
     * @param errorDescription set a human readable error description
     * @param autoFireSignal if true the error signal will automaticly be emitted. If networkError=NoError there will never be a signal fired.
     * @see isErrorPresent()
     * @see QNetworkReply::NetworkError getLatestError()
     */
    virtual void setLatestError(QNetworkReply::NetworkError networkError, QString errorDescription = "", bool autoFireSignal = true);

private:
    TexmakerVersion _currentVersion;
    TexmakerVersion _webVersion;
    QNetworkAccessManager _manager;
    QNetworkReply       * _reply;
    QTimer _timer;
    QNetworkReply::NetworkError _latestError;
    QString _latestErrorDescription;
    unsigned int _timeout;
    bool _checked;

    QDateTime _lastChecked;
    int _updateInterval;

};

#endif // UPDATECHECKER_H
