#include "updatechecker.h"

UpdateChecker::UpdateChecker(int updateInterval, QDateTime dateLastChecked, QObject *parent):
    QObject(parent)
{
    _latestError = QNetworkReply::NoError;
    _timer.setSingleShot(true);
    _currentVersion = TexmakerVersion::getCurrentVersion();
    _timeout = 10000;
    _checked = false;
    _updateInterval = updateInterval;
    if(dateLastChecked.isValid()) {
        _lastChecked = dateLastChecked;
    } else {
        _lastChecked = QDateTime::fromMSecsSinceEpoch(1000);
    }
    connect(&_timer, SIGNAL(timeout()), this, SLOT(stopVersionCheck()));
}

void UpdateChecker::setTimeout(unsigned int timeout)
{
    _timeout = timeout;
}

unsigned int UpdateChecker::getTimeout() const
{
    return _timeout;
}

const TexmakerVersion &UpdateChecker::getCurrentVersion() const
{
    return _currentVersion;
}

const TexmakerVersion &UpdateChecker::getWebVersion() const
{
    return _webVersion;
}

QNetworkReply::NetworkError UpdateChecker::getLatestError() const
{
    return _latestError;
}

bool UpdateChecker::isErrorPresent() const
{
    return _latestError != QNetworkReply::NoError;
}

const QDateTime& UpdateChecker::getDateLastChecked() const
{
    return _lastChecked;
}

bool UpdateChecker::isUpdateCheckRequired() const
{
    if(_updateInterval == 0) {
        //this means, that auto update is disabled
        return false;
    }
    const qint64 updateInt = _updateInterval;
    if(_lastChecked.daysTo(QDateTime::currentDateTime()) >= updateInt) {
        return true;
    }

    return false;
}

bool UpdateChecker::hasBeenChecked() const
{
    return _checked;
}

void UpdateChecker::openDownloadPage()
{
    QDesktopServices::openUrl(QUrl("http://www.xm1math.net/texmaker/download.html"));
}

void UpdateChecker::checkForNewVersion()
{
    _checked = true;
    _timer.start(_timeout);
    _reply = _manager.get(QNetworkRequest(QUrl("http://www.xm1math.net/texmaker/version.txt")));
    connect(_reply, SIGNAL (finished()),this, SLOT(processWebResult()));
}

void UpdateChecker::stopVersionCheck()
{
    disconnect(_reply, SIGNAL (finished()),this, SLOT(processWebResult()));
    _reply->abort();
    setLatestError(QNetworkReply::TimeoutError, tr("Timeout error"));
}

void UpdateChecker::checkForNewVersionIfRequired()
{
    if(isUpdateCheckRequired()) {
        checkForNewVersion();
    }
}

void UpdateChecker::gotoDownloadPage()
{
    openDownloadPage();
}

void UpdateChecker::processWebResult()
{
    _timer.stop();
    if (QNetworkReply::NoError == _reply->error()) {
        _webVersion.parseString(QString(_reply->readAll()));

        if(_webVersion.isValid()) {
            emit availableWebVersion(_webVersion); //fire event because the web version is valid
            setLatestError(QNetworkReply::NoError);
            _lastChecked = QDateTime::currentDateTime();

            //check if an update is required
            if(_currentVersion.isValid()) {
                if(_webVersion > _currentVersion){
                    emit newVersionAvailable();
                }else if(_webVersion == _currentVersion) {
                    emit currentVersionIsLatest();
                }
            }
        } else {
            setLatestError(QNetworkReply::UnknownContentError, tr("Unable to parse web version")); //because parsing may gone wrong
        }
    } else {
        setLatestError(_reply->error(), _reply->errorString());
    }
}

void UpdateChecker::setLatestError(QNetworkReply::NetworkError networkError, QString errorDescription, bool autoFireSignal)
{
    _latestError = networkError;
    _latestErrorDescription = errorDescription;

    if(networkError != QNetworkReply::NoError) {
        _webVersion = TexmakerVersion(); // this will invalidate the web version, which is required  by the getWebVersion function
        if(autoFireSignal) {
            emit error(networkError, errorDescription); //NoError will never be fired :)
        }
    }
}
