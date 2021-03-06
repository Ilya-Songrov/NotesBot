/**************************************************************************
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/.
**
**************************************************************************/

#ifndef APPTRANSLATOR_H
#define APPTRANSLATOR_H

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QTranslator>


#define VERSION_APP_TRANSLATOR_MAJOR    "1"
#define VERSION_APP_TRANSLATOR_MINOR    "0"
#define VERSION_APP_TRANSLATOR_BUILD    "4"
#define APP_TRANSLATOR_VERSION QString(VERSION_APP_TRANSLATOR_MAJOR) + "." + QString(VERSION_APP_TRANSLATOR_MINOR) + "." + QString(VERSION_APP_TRANSLATOR_BUILD)

class AppTranslator : public QObject
{
    Q_OBJECT

public:
    explicit AppTranslator(const QString &pathTranslationFile, QObject *parent = nullptr);

    void setNewLanguage(const QString &pathTranslationFile);
private:
    void loadTranslator(const QString &file);
    void testTranslator();
};

#endif // APPTRANSLATOR_H
