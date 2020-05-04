import pymongo

class Database:
    def __init__(self, monogUri: str):
        self.name   = "database"
        self.client = pymongo.MongoClient(monogUri)
        self.userdb = self.client["UserInfo"]
        self.users  = self.userdb["Users"]

        for x in self.users.find({}, {"_id": 0, "name": 1}):
            print(x)