#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <array>
#include <string_view>

enum CMode
{
    C,
    CPP,
};

enum CStd
{
    C89,
    C90,
    C95,
    C99,
    C11,
    C17,
    C23
};

constexpr std::array<std::pair<CStd, std::string_view>, 8> CStdMap = {
{
    {C89, "c89"},
    {C90, "c90"},
    {C95, "c95"},
    {C99, "c99"},
    {C11, "c11"},
    {C17, "c17"},
    {C23, "c23"}
}};

enum CPPStd
{
    CPP98,
    CPP03,
    CPP11,
    CPP14,
    CPP17,
    CPP20,
    CPP23
};

constexpr std::array<std::pair<CPPStd, std::string_view>, 8> CPPStdMap = {
{
    {CPP98, "c++98"},
    {CPP03, "c++03"},
    {CPP11, "c++11"},
    {CPP14, "c++14"},
    {CPP17, "c++17"},
    {CPP20, "c++20"},
    {CPP23, "c++23"}
}};

#endif // COMMON_H