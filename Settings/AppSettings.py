import configparser
import sys
import os
import datetime

class AppSettings():
    def __init__(self):
        self.oathConfigPath = os.getcwd() + "/Settings/BotSettings.ini"

        self.oathConfig = configparser.ConfigParser()
        self.oathConfig.read(self.oathConfigPath)

        self.readOathSettings()

    def updateSetting(self, section, settingId, value):
        if section not in self.oathConfig or settingId not in self.oathConfig[section]:
            return

        self.oathConfig[section][settingId] = value

        with open(self.oathConfigPath, 'w') as configfile:
            self.oathConfig.write(configfile)

        self.readOathSettings()

    def readOathSettings(self):
        #SLACK SETTINGS
        self.slackVerificationToken  = self.oathConfig["SLACK"]["verificationToken"]
        self.slackWebclientToken     = self.oathConfig["SLACK"]["webclientToken"   ]

        #MONGODB SETTINGS
        self.mongoUri                = self.oathConfig["MONGODB"]["uri"]

        #GITHUB SETTINGS
        self.githubLink              = self.oathConfig["GITHUB" ]["link"]