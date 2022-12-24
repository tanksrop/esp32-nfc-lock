#ifndef Commands_H
#define Commands_H

#include <Arduino.h>
#include "users.h"

class serialCommands{
private:
  const char* error_msgs[5] = {"Success!", "User doesn't exist!", "Invalid user permissions!", "Userlist Full", "no users remaining"};
  uint8_t error;

public:
  void commands(){
    String command = Serial.readStringUntil(' ');
    if(command.equals("show")){
      Serial.print("\n\n\n\n");
      Serial.println(F("id\tuid\t\t\tpermission_level\tname"));
      for(int i = 0; i < 20; i++)
      {
        Serial.print(i +1);
        Serial.print(F("\t"));
        Serial.print(userlist.data[i].uid);
        Serial.print(F("\t\t\t"));
        Serial.print(userlist.data[i].perm);
        Serial.print(F("\t"));
        Serial.println(userlist.data[i].name);
        }
      }
    else if (command.equals("add")){
      String uid = Serial.readStringUntil(',');
      String permission_level = Serial.readStringUntil(',');
      String name = Serial.readStringUntil(',');
      error = userlist.createUser(uid, permission_level.toInt(), name); //test user creation
      Serial.println(error_msgs[error]);
    }
    else if (command.equals("save")){
      error = userlist.save();
      Serial.println(error_msgs[error]);
    }
    else if (command.equals("load")){
      error = userlist.load();
      Serial.println(error_msgs[error]);
    }
    else if (command.equals("remove")){
      int id = Serial.readStringUntil(',').toInt();
      error = userlist.removeUser(id);
      Serial.println(error_msgs[error]);
    }
    else if (command.equals("perm")){
      int id = Serial.readStringUntil(',').toInt();
      int perm = Serial.readStringUntil(',').toInt();
      error = userlist.modifyPermissions(id,perm);
      Serial.println(error_msgs[error]);
    }
    else if (command.equals("help")){
      Serial.println("");
      Serial.println("List of commands:");
      Serial.println("show");
      Serial.println("add,uid,permission level,name");
      Serial.println("perm,id,permission level");
      Serial.println("remove,id");
      Serial.println("save,");
      Serial.println("load,");
      Serial.println("");
    }
    else {
      while(Serial.available()){  //is there anything to read?
	    char getData = Serial.read();  //if yes, read it
      }   
    }
  };
};

#endif 