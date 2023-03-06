typedef union _uart_Float_
{
    float f_data;
    uint8_t byte[4];
} uart_Float;
typedef struct _Speed_Receiver_
{
    unsigned char buffer[64];

    unsigned char Data_Header;
    short X_speed;
    short Y_speed;
    short Z_speed;
    unsigned char Data_Tail;

} Speed_Receiver;

typedef struct _Speed_Reporter_
{
    unsigned char buffer[32];

    unsigned char Data_Header;
    uart_Float X_speed;
    uart_Float Y_speed;
    uart_Float Z_speed;
    unsigned char Data_Tail;

} Speed_Reporter;
