#include "PlaceNotes.h"

PlaceNotes::PlaceNotes(QObject *parent) : PlaceAbstract(parent)
{

}

void PlaceNotes::slotOnCommand(const Message::Ptr &message, const ChatInfo &chatInfo)
{
    switch (chatInfo.currentCommand) {
    case Content::Notes_AddNote:
        onAddNote(message);
        break;
    case Content::Notes_RemoveNote:
        onRemoveNote(message);
        break;
    case Content::MultiPlace_AnyMessage:
        onAnyMessage(message);
        break;
    default:
        PlaceAbstract::slotOnCommand(message, chatInfo);
    }
}

void PlaceNotes::slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const ChatInfo &chatInfo)
{
    if (chatInfo.currentCommand == Content::Notes_CreateGroup) {
        onCreateGroupCallbackQuery(callbackQuery);
    }
    else if (chatInfo.currentCommand == Content::MultiPlace_AnyCallbackQuery) {
        onAnyCallbackQuery(callbackQuery);
    }
    else{
        PlaceAbstract::slotOnCallbackQuery(callbackQuery, chatInfo);
    }
}

void PlaceNotes::onAddNote(const Message::Ptr &message)
{
    static const auto answer { QObject::tr("Select group:").toStdString() };
    static const auto createGroup = Content::getCommandStr(Content::Notes_CreateGroup);
    auto list = managerDatabase->getListGroups(message->chat->id);
    list.append(createGroup);
    const auto inlineButtonNotes = createOneColumnInlineKeyboardMarkup(list);
    sendInlineKeyboardMarkupMessage(message->chat->id, answer, inlineButtonNotes);
}

void PlaceNotes::onRemoveNote(const Message::Ptr &message)
{
    const auto chat_id = message->chat->id;
    static const auto answer { QObject::tr("Select group:").toStdString() };
    const auto list = managerDatabase->getListGroups(chat_id);
    const auto inlineButtonNotes = createOneColumnInlineKeyboardMarkup(list);
    sendInlineKeyboardMarkupMessage(chat_id, answer, inlineButtonNotes);
}

void PlaceNotes::onAnyMessage(const Message::Ptr &message)
{
    const auto chat_id = message->chat->id;
    if (chatContainsLastCommand(chat_id, Content::Notes_CreateGroup)) {
        static const auto answerCreated { QObject::tr("New group was created").toStdString() };
        static const auto answerNotCreated { QObject::tr("New group was not created").toStdString() };
        const QString group = QString::fromStdString(message->text);
        const bool ret = managerDatabase->addGroup(group, chat_id);
        updateCahtInfoLastGroup(chat_id, ret ? "" : group);
        sendMainMenuMessage(chat_id, ret ? answerCreated : answerNotCreated);
    }
    else if (chatContainsLastCommand(chat_id, Content::Notes_GroupWasSelectedToAdd)) {
        const auto lastGroup = getChatInfo(chat_id).lastGroup;
        if (!lastGroup.isEmpty()) {
            managerDatabase->addNote(message->text, lastGroup, chat_id);
        }
        sendMainMenuMessage(chat_id, getListNotes(lastGroup, chat_id));
    }
    else if (managerDatabase->existsGroup(message->text, chat_id)) {
        updateCahtInfoLastGroup(chat_id, QString::fromStdString(message->text));
        sendMainMenuMessage(chat_id, getListNotes(message->text, chat_id));
    }
    else if (QString::fromStdString(message->text).toLower() == "ping") {
        sendMainMenuMessage(chat_id, "Pong!");
    }
    else{
        const auto lastGroup = getChatInfo(chat_id).lastGroup;
        if (!lastGroup.isEmpty()) {
            managerDatabase->addNote(message->text, lastGroup, chat_id);
            sendMainMenuMessage(chat_id, getListNotes(lastGroup, chat_id));
        }
        else{
            sendMainMenuButtons(chat_id);
        }
    }
}

void PlaceNotes::onCreateGroupCallbackQuery(const CallbackQuery::Ptr &callbackQuery)
{
    static const auto answer { QObject::tr("Write name of group:").toStdString() };
    const auto chat_id = callbackQuery->message->chat->id;
    bot->getApi().answerCallbackQuery(callbackQuery->id);
    bot->getApi().sendMessage(chat_id, answer);
}

void PlaceNotes::onAnyCallbackQuery(const CallbackQuery::Ptr &callbackQuery)
{
    const auto chat_id = callbackQuery->message->chat->id;
    const auto data = QString::fromStdString(callbackQuery->data);
    if (chatContainsLastCommand(chat_id, Content::Notes_AddNote)) {
        static const auto answer { QObject::tr("Write your note:").toStdString() };
        updateCahtInfoCurrentCommandAndLastGroup(chat_id, Content::Command::Notes_GroupWasSelectedToAdd, data);
        bot->getApi().answerCallbackQuery(callbackQuery->id);
        bot->getApi().sendMessage(chat_id, answer);
    }
    else if (chatContainsLastCommand(chat_id, Content::Notes_RemoveNote)) {
        static const auto removeGroup = Content::getCommandStr(Content::Notes_RemoveGroup);
        static const auto answer { QObject::tr("Select the note:").toStdString() };
        const auto vecNotes = managerDatabase->getVecNotes(data, chat_id);
        QList<QPair<QString, QString> > listButtons;
        for (const auto &note: vecNotes) {
            listButtons.append(qMakePair(note.note.left(15) + " ...", note.note_id));
        }
        listButtons.append(qMakePair(removeGroup, data));
        const auto inlineButtonNotes = createOneColumnInlineKeyboardMarkup(listButtons);
        updateCahtInfoCurrentCommandAndLastGroup(chat_id, Content::Command::Notes_GroupWasSelectedToRemove, data);
        bot->getApi().answerCallbackQuery(callbackQuery->id);
        sendInlineKeyboardMarkupMessage(callbackQuery->message->chat->id, answer, inlineButtonNotes);
    }
    else if (chatContainsLastCommand(chat_id, Content::Notes_GroupWasSelectedToRemove)) {
        bool ret = false;
        std::string answer;
        if (getChatInfo(chat_id).lastGroup == data) {
            static const auto answerGroupRemoved { QObject::tr("Group has been removed").toStdString() };
            static const auto answerGroupNotRemoved { QObject::tr("Group has been not removed").toStdString() };
            const QString group = data;
            ret = managerDatabase->deleteAllNotes(group, chat_id);
            updateCahtInfoLastGroup(chat_id, ret ? "" : group);
            answer = ret ? answerGroupRemoved : answerGroupNotRemoved;
        }
        else{
            static const auto answerNoteRemoved { QObject::tr("Note has been removed").toStdString() };
            static const auto answerNoteNotRemoved { QObject::tr("Note has been not removed").toStdString() };
            bool ok;
            const int note_id = QString::fromStdString(callbackQuery->data).toInt(&ok);
            if (ok) {
                ret = managerDatabase->deleteAllNotes(note_id, chat_id);
            }
            answer = ret ? answerNoteRemoved : answerNoteNotRemoved;
        }
        bot->getApi().answerCallbackQuery(callbackQuery->id);
        sendMainMenuMessage(chat_id, answer);
    }
}

std::string PlaceNotes::getListNotes(const QString &group, const int64_t chat_id)
{
    return getListNotes(group.toStdString(), chat_id);
}

std::string PlaceNotes::getListNotes(const std::string &group, const int64_t chat_id)
{
    const QString answer { QObject::tr("List notes") + QString(" (%1):\n").arg(group.c_str()) + managerDatabase->getListNotes(group, chat_id).join('\n') };
    return answer.toStdString();
}
