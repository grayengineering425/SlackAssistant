from datetime import datetime

class User:
    def __init__(self, id: str, name: str, slackId: str, lastUseDate: str):
        self.id             = id
        self.name           = name
        self.slackId        = slackId
        self.lastUseDate    = datetime.strptime(lastUseDate, "%m/%d/%Y")

    def print(self):
        print(self.id)
        print(self.name)
        print(self.slackId)
        print(self.lastUseDate)