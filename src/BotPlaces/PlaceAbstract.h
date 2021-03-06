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

#pragma once

#include <QObject>
#include <QDebug>
#include <QtSql>

#include "../Content/content.h"
#include "../Content/chatactions.h"
#include "../GlobalData/GlobalData.h"

#include <tgbot/tgbot.h>
using namespace TgBot;

class PlaceAbstract : public QObject
{
    Q_OBJECT
public:
    explicit PlaceAbstract(QObject *parent = nullptr);

    virtual void slotOnCommand(const Message::Ptr &message, const ChatActions &chatActions);
    virtual void slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const ChatActions &chatActions);

protected:
    ReplyKeyboardMarkup::Ptr createOneColumnReplyKeyboardMarkup(const QStringList &listButtons, const bool resizeKeyboard = true, const bool oneTimeKeyboard = false);
    ReplyKeyboardMarkup::Ptr createReplyKeyboardMarkup(const QVector<QStringList> &vecLayouts, const bool resizeKeyboard = true, const bool oneTimeKeyboard = false);

    InlineKeyboardMarkup::Ptr createOneColumnInlineKeyboardMarkup(const QStringList &listButtons);
    InlineKeyboardMarkup::Ptr createOneColumnInlineKeyboardMarkup(const QList<QPair<QString, QString> > &listButtonsNameData);
    InlineKeyboardMarkup::Ptr createInlineKeyboardMarkup(const QVector<QStringList> &vecLayouts);

    ReplyKeyboardMarkup::Ptr getMainMenuButtons(const int64_t chat_id);
    void sendMainMenuButtons(const std::int64_t chat_id);
    void sendMainMenuMessage(const std::int64_t chat_id, const std::string &message);
    void sendInlineKeyboardMarkupMessage(const std::int64_t chat_id, const std::string &message, const InlineKeyboardMarkup::Ptr inlineKeyboardMarkup);

    inline void setChatActions(const std::int64_t chat_id, const ChatActions &chatActions){ managerDb->setChatActions(chatActions, chat_id); }
    inline ChatActions getChatActions(const std::int64_t chat_id){ return managerDb->getChatActions(chat_id); }
    void updateChatActionsLastGroup(const std::int64_t chat_id, const QString &lastGroup);
    void updateChatActionsCurrentCommand(const std::int64_t chat_id, const Content::Command currentCommand);
    void updateChatActionsCurrentCommandAndLastGroup(const std::int64_t chat_id, const Content::Command currentCommand, const QString &lastGroup);

    inline bool chatContainsLastCommand(const std::int64_t chat_id, const Content::Command command){ return getChatActions(chat_id).lastCommand == command; }
};

