#ifndef RETURN_TYPE_H
#define RETURN_TYPE_H

enum eReturnType_t
{
    RET_OK,
    RET_NOT_OK,

    //Event Bus
    RET_RECEIVER_EXISTS,
    RET_RECEIVER_NOT_EXISTS,
    RET_EVENT_ID_INVALID,
    RET_NO_RECEIVERS_FOR_EVENT,

    //Websocket Server
    RET_USERNAME_HEADER_MISSING,

    RET_INVALID
};

typedef enum eReturnType_t returnType_t;

#endif