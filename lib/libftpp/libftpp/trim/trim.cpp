/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:22:40 by dpotsch           #+#    #+#             */
/*   Updated: 2025/09/11 12:33:38 by dpotsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trim.hpp"
#include <algorithm>

static bool isNoSpace(int chr)
{
  return std::isspace(chr) == 0;
}

std::string& trim(std::string& str)
{
  // trim left
  std::string::iterator itBegin =
    std::find_if(str.begin(), str.end(), isNoSpace);
  if (itBegin == str.end()) {
    str = std::string("");
    return str;
  }
  str.erase(str.begin(), itBegin);

  // trim right
  std::string::reverse_iterator itEnd =
    std::find_if(str.rbegin(), str.rend(), isNoSpace);
  str.erase(itEnd.base(), str.end());
  return str;
}
