#ifndef RETURN_TYPE_H
#define RETURN_TYPE_H

enum eReturnType_t
{
    RET_OK,
    RET_NOT_OK,

    RET_NO_LOGGED_USERS,

    RET_INVALID
};

typedef enum eReturnType_t returnType_t;

#endif