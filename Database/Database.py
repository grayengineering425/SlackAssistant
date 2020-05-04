import pymongo

from QueryGenerator import QueryGenerator
from User           import User
from UserList       import UserList

class Database:
    def __init__(self, monogUri: str):
        self.name   = "database"
        self.client = pymongo.MongoClient(monogUri)
        self.userdb = self.client["UserInfo"]
        self.users  = self.userdb["Users"]
        self.lists  = self.userdb["Lists"]

    def findUser(self, slackId: str) -> User:
        findUserQuery = QueryGenerator.generateFindUser(slackId)

        userDoc = self.users.find_one(findUserQuery)

        if not userDoc:
            return None
        else:
            foundUser = User(userDoc["_id"], userDoc["name"], userDoc["slackId"], userDoc["lastUseDate"])

            return foundUser

    def findList(self, listName: str, userId: str) -> UserList:


    def addUser(self, name: str, slackId: str) -> User:
        addUserQuery = QueryGenerator.generateAddUser(name, slackId)

        self.users.insert_one(addUserQuery)

        #TODO
        return self.findUser(slackId)

    def addList(self, userId: str, listName: str) -> None:
        addListQuery = QueryGenerator.generateAddList(userId, listName)

        self.lists.insert_one(addListQuery)

    def addItemToList(self, item: str, list: str) -> None:
        addItemQuery = QueryGenerator.generateAddItemToList(item, list)

        self.listItems.insert_one(addItemQuery)
