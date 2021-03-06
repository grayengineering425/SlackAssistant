from typing import Tuple

from Chat           import Chat
from AppSettings    import AppSettings
from Database       import Database
from User           import User

class Server:
    def __init__(self, appSettings: AppSettings):
        self.appSettings    = appSettings
        self.chat           = Chat      (appSettings)
        self.database       = Database  (appSettings.mongoUri)

    def handleAppMention(self, payload):
        channel = payload["channel" ]
        slackId = payload["user"    ]
        text    = payload["text"    ]

        displayName = self.chat.getDisplayName(slackId)

        user = self.database.findUser(slackId)

        if not user:
            user = self.database.addUser(slackId, displayName)

        if not user:
            return

        self.parseAppMention(text, user)
        
        #message     = "Hello " + displayName + ", nice to meet you! Use /musicbotinfo to learn more about me!"
        #status      = self.chat.sendMessage     (channel, message)

    def parseAppMention(self, text: str, user: User) -> None:
        tokenCreate   = "create list"
        tokenAdd      = "add "
        tokenRemove   = "remove "
        tokenToList   = "to list "
        tokenFromList = "from list "

        tokenIndex = text.find(tokenCreate)

        if tokenIndex != -1:
            listNameIndex   = tokenIndex + len(tokenCreate)
            listName        = text[listNameIndex:]

            listName = listName.rstrip()
            listName = listName.lstrip()

            listName = listName.lower()

            self.database.addList(user.id, listName)

            return

        list, item = self.getListAndItem(text, tokenAdd, tokenToList)

        if list is not None and item is not None:
            self.database.addItemToList(item, list)

        list, item = self.getListAndItem(text, tokenRemove, tokenFromList)

        if list is not None and item is not None:
            return

    def getListAndItem(self, text: str, tokenFirst: str, tokenSecond: str) -> Tuple[str, str]:
        tokenFirstIndex = text.find(tokenFirst)

        if tokenFirstIndex != -1:
            tokenSecondIndex = text.find(tokenSecond)

            if tokenSecondIndex != -1:
                listName = text[tokenSecondIndex + len(tokenSecond):]
                itemName = text[tokenFirstIndex  + len(tokenFirst ):tokenSecondIndex-1]

                listName = listName.rstrip()
                itemName = itemName.rstrip()

                return listName, itemName

        return None, None