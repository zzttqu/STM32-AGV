typedef struct _Data_Transer_
{
    unsigned char buffer[64];
    struct _Speed_
    {
        unsigned char Data_Header;
        short X_speed;
        short Y_speed;
        short Z_speed;
        unsigned char Data_Tail;
    } Speed;
} Data_Transer;

