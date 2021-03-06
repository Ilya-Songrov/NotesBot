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

#include "content.h"

#include <QMetaEnum>
#include <QPair>

QVector<QPair<Content::PlaceCommand, QString> > Content::vecPlaceCommand;

Content::Content(QObject *parent) : QObject(parent)
{

}

void Content::initContent()
{
    vecPlaceCommand = {
        {   Content::PlaceCommand(Content::Place::MultiPlace, Content::NoCommand), {} },

        {   Content::PlaceCommand(Content::Place::Start, Content::Start_ChooseLanguage), "/start" },

        {   Content::PlaceCommand(Content::Place::Notes, Content::Notes_AddNote), "🗒 " + QObject::tr("Add note") },
        {   Content::PlaceCommand(Content::Place::Notes, Content::Notes_RemoveNote), "🗒 " + QObject::tr("Remove note") },
        {   Content::PlaceCommand(Content::Place::Notes, Content::Notes_CreateGroup),  QObject::tr("Create group") },
        {   Content::PlaceCommand(Content::Place::Notes, Content::Notes_RemoveGroup),  QObject::tr("Remove group") },

        {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_Additional), "🔹 " + QObject::tr("Additional") },
        {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_ShowHistory), QObject::tr("Show history") + " 📍" },
        {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_DeleteHistory), QObject::tr("Delete history") + " 📍" },
        {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_Developer), QObject::tr("Developer") + " 📍" },

        {   Content::PlaceCommand(Content::Place::MultiPlace, Content::MultiPlace_Help), "/help" },
    };
}

Content::PlaceCommand Content::getPlaceCommand(const QString &command)
{
    for (const auto &pair: qAsConst(vecPlaceCommand)) {
        if (pair.second == command) {
            return pair.first;
        }
    }
    return {};
}

Content::PlaceCommand Content::getPlaceCommand(const std::string &command)
{
    return getPlaceCommand(QString::fromStdString(command));
}

QString Content::getCommandStr(const Content::Command command)
{
    for (const auto &pair: qAsConst(vecPlaceCommand)) {
        if (pair.first.command == command) {
            return pair.second;
        }
    }
    return {};
}
