typedef struct _Speed_Receiver_
{
    unsigned char buffer[64];
    struct _Speed1_
    {
        unsigned char Data_Header;
        float X_speed;
        float Y_speed;
        float Z_speed;
        unsigned char Data_Tail;
    } Speed;
} Speed_Receiver;

typedef struct _Speed_Reporter_
{
    unsigned char buffer[64];
    struct _Speed2_
    {
        unsigned char Data_Header;
        short X_speed;
        short Y_speed;
        short Z_speed;
        unsigned char Data_Tail;
    } Speed;
} Speed_Reporter;