/***************************************************************************
 *   copyright       : (C) 2003-2013 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include "updatechecker.h"

#include "ui_versiondialog.h"

class VersionDialog : public QDialog  {
   Q_OBJECT
public:
    /**
     * @brief VersionDialog a simple dialog for checking, if the working version is up to date
     * @param checker a reference to the update checker
     * @param parent the parent wideget, important for memory management
     * @info it's recomended to pass the parent widget
     */
    VersionDialog(UpdateChecker& checker, QWidget *parent=0);

	~VersionDialog();

private slots:
    /**
     * @brief errorOccured if erreor checking goes wrong, connect the error signal with this slot
     * @param error the enum describing the occured error
     * @param errorDescription a human readable representation of the occured error
     */
    void errorOccured(QNetworkReply::NetworkError error, QString errorDescription);

    /**
     * @brief gotWebVersion callback function for a "new" web version
     * @param version the version available on the web
     */
    void gotWebVersion(TexmakerVersion version);

    /**
     * @brief checkForNewVersion triggeres checking for a new web version
     */
    void checkForNewVersion();

    /**
     * @brief onNewVersionAvailable call this function if the web version is a newer one than current running version
     */
    void onNewVersionAvailable();

private:

    /**
     * @brief displayVersionString a convenience version for displaying TexmakerVersion objects
     * @param lineEdit the QLineEdit where the version string should be displayed
     * @param version TexmakerVersion to display
     * @param errorString the human readable string if version is non valid, gets displayed on mouse over as ToolTip
     */
    void displayVersionString(QLineEdit& lineEdit, const TexmakerVersion& version, QString errorString = "");
    void setAvailableVersionIsCurrent(bool isCurrent);

private:
    Ui::VersionDialog ui;
    UpdateChecker& _updateChecker;

};


#endif
