class IMessage
{
    protected:
        unsigned short int type;
        unsigned short int orig_uid;
        unsigned short int dest_uid;
        unsigned short int text_len;
        unsigned char text[141];

    virtual void setType(unsigned short int t) = 0;
    virtual unsigned short int getType() const = 0;

    virtual void setOrigUID(unsigned short int uid) = 0;
    virtual unsigned short int getOrigUID() const = 0;

    virtual void setDestUID(unsigned short int uid) = 0;
    virtual unsigned short int getDestUID() const = 0;

    virtual void setText(const char* msg) = 0;
    virtual const unsigned char* getText() const = 0;

    virtual void printMessage() const = 0;

    virtual ~IMessage() {}
};