#ifndef Users_H
#define Users_H

#include <Arduino.h>
#include <Preferences.h>
Preferences preferences;

class Users{
  struct data
  {
    String uid;
    int perm;
    String name;
  };

  int max_users = 20;
  int num_current_users = 0;

public:
  data data[20] = {};
    uint8_t createUser(String uid, int perm, String name) {
        if (num_current_users < max_users) {
            data[num_current_users].uid = uid;
            data[num_current_users].perm = perm;
            data[num_current_users].name = name;
            num_current_users++;
            return 0;
        }
        return 3; // max num already hit
    }

    uint8_t removeUser(int id) {
      if (num_current_users) {
        data[id - 1] = {};
        for(int i = id; i < max_users; i++){
          data[(i - 1)] = data[i];
        };
        num_current_users--;
        return 0;
      };
      return 4; // no users to remove
    }

    uint8_t save(){
      preferences.clear();
      preferences.putInt("users",num_current_users);
      for (int i = 0; i < (max_users - 1); i++){ 
        String uid = "uid";
        String perm = "perm";
        String name = "name";
        uid = uid + i;
        perm = perm + i;
        name = name + i;
        preferences.putString(uid.c_str(), data[i].uid);
        preferences.putInt(perm.c_str(), data[i].perm);
        preferences.putString(name.c_str(), data[i].name);
      }
      return 0;
    }
    uint8_t load(){
      num_current_users = preferences.getInt("users");
      for(int i = 0; i < (max_users - 1); i++){
        String uid = "uid";
        String perm = "perm";
        String name = "name";
        uid = uid + i;
        perm = perm + i;
        name = name + i;
        data[i].uid = preferences.getString(uid.c_str());
        data[i].perm = preferences.getInt(perm.c_str());
        data[i].name = preferences.getString(name.c_str());
      };
      return 0;
    }

    uint8_t modifyPermissions(int user, const uint8_t level) {
      data[(user - 1)].perm = level;
      return 0;
    }
    uint8_t hasAccess(String c_uid, int level_to_check) {
        // does the user exist?
      for (int i = 0; i <= num_current_users; ++i) {
          if (data[i].uid == c_uid) {
              // user exists does he have permission?
              if (data[i].perm <= level_to_check){
                  return 0; //user exists and has access
              }
              return 2; // user exists but does not have access level required
          }
        }
        return 1; // if we got here the user doesnt exist
    };
};
#endif 