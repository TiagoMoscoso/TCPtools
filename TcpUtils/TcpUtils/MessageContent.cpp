#ifndef MESSAGECONTENT_H
#define MESSAGECONTENT_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstring> // Para usar strncpy

#define BUFFER_SIZE 1024

using namespace std;

class MessageContent {
public:
    unsigned short int type;
    unsigned short int orig_uid;
    unsigned short int dest_uid;
    unsigned short int text_len;
    string messagestr;
    char message[1024];

    MessageContent(int t, int orig, int dest, int txt_len, const char* msg)
        : type(t), orig_uid(orig), dest_uid(dest), text_len(txt_len) {
        strncpy(message, msg, sizeof(message) - 1);
        message[sizeof(message) - 1] = '\0';
    }

    MessageContent(char* binary_cstr)
    {
        istringstream iss(binary_cstr);

        type = deserializeType(iss);
        orig_uid = deserializeOrigin_uid(iss);
        dest_uid = deserializeDest_uid(iss);
        text_len = deserializeText_len(iss);;
        messagestr = deserializeMessage(iss);

    }

    void serialize(ostringstream& out, MessageContent msgContent) {
        out << msgContent.type << " " << msgContent.orig_uid << " " << msgContent.dest_uid << " " << msgContent.text_len << " " << msgContent.message;
    }

    int deserializeType(istringstream& iss) {
        int type;
        iss >> type;
        return type;
    }

    int deserializeOrigin_uid(istringstream& iss) {
        int origi_uid;
        iss >> origi_uid;
        return origi_uid;
    }

    int deserializeDest_uid(istringstream& iss) {
        int deserializeDest_uid;
        iss >> deserializeDest_uid;
        return deserializeDest_uid;
    }

    int deserializeText_len(istringstream& iss) {
        int deserializeText_len;
        iss >> deserializeText_len;
        return deserializeText_len;
    }

    string deserializeMessage(istringstream& iss) {
        string message;
        iss >> message;
        return message;
    }
};

#endif 
