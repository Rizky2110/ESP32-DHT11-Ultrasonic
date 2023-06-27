#include "../include/spiffs_module.h"

void IFile::write(fs::FS &fs, const char *path, String message)
{
    static char data_save[4096];
    message.toCharArray(data_save, (message.length() + 1));
    snprintf(msgs, 4096, "%s", data_save);
    Serial.print("Write Message : ");
    Serial.println(msgs);
    delay(100);
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }

    delay(100);
    if (file.print(msgs))
    {
        delay(100);
        Serial.println("- file written");
    }
    else
    {
        delay(100);
        Serial.println("- write failed");
    }
    file.close();
}

void IFile::setupFile()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    File fileToWrite = SPIFFS.open("/datalog.txt", FILE_WRITE);

    if (!fileToWrite)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }

    if (fileToWrite.println("Data Log :"))
    {
        Serial.println("Initial File was written");
        ;
    }
    else
    {
        Serial.println("Initial File write failed");
    }

    fileToWrite.close();
}