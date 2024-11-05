#include "IMessage.cpp"

class CommomMessage : public IMessage
{
    void setType(unsigned short int t) override 
    {
        type = t;
    }

    unsigned short int getType() const override 
    {
        return type;
    }

    void setOrigUID(unsigned short int uid) override 
    {
        orig_uid = uid;
    }

    unsigned short int getOrigUID() const override 
    {
        return orig_uid;
    }

    void setDestUID(unsigned short int uid) override 
    {
        dest_uid = uid;
    }

    unsigned short int getDestUID() const override 
    {
        return dest_uid;
    }
};