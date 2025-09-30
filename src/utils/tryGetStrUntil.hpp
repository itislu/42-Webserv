#pragma once
#ifndef TRY_GET_STR_UNTIL_HPP
#define TRY_GET_STR_UNTIL_HPP

#include <string>
#include <vector>

std::string tryGetStrUntil(std::vector<unsigned char> buffer,
                           std::size_t iStart,
                           std::vector<unsigned char> tokenSequenze);

#endif // TRY_GET_STR_UNTIL_HPP
