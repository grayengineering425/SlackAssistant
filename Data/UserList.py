from datetime import datetime

class UserList:
    def __init__(self, id: str, userId: str, name: str, lastUseDate: str):
        self.id             = id
        self.userId         = userId
        self.name           = name
        self.lastUseDate    = datetime.strptime(lastUseDate, "%m/%d/%Y")

    def print(self):
        print(self.id         )
        print(self.userId     )
        print(self.name       )
        print(self.lastUseDate)