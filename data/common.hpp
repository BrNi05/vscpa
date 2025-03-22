#ifndef COMMON_H
#define COMMON_H

enum CMode
{
    C,
    CPP,
    NONE
};

enum CStd
{
    NOT_C,
    C89,
    C90,
    C95,
    C99,
    C11,
    C17,
    C23
};

enum CPPStd
{
    NOT_CPP,
    CPP98,
    CPP03,
    CPP11,
    CPP14,
    CPP17,
    CPP20,
    CPP23
};

#endif // COMMON_H