import datetime

class QueryGenerator(object):
    @staticmethod
    def generateFindUser(slackId: str):
        return { "slackId" : slackId }

    @staticmethod
    def generateAddUser(name: str, slackId: str):
        return { "name" : name, "slackId" : slackId, "lastUseDate" : datetime.date.today().strftime("%m/%d/%Y") }

    @staticmethod
    def generateAddList(id: str, listName: str):
        return { "userId" : id, "listName" : listName }