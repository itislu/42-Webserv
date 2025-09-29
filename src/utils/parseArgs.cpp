/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseArgs.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:09:31 by dpotsch           #+#    #+#             */
/*   Updated: 2025/09/29 12:23:52 by dpotsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseArgs.hpp"
#include "libftpp/trim/trim.hpp"
#include <iostream>
#include <string>

int parseArgs() {
  float x = 10.34;
  if (x / 0) {
    std::cout << "alarm" << std::endl;
  }

  std::cout << "test\n";
  std::string test("   test   ");
  trim(test);
  std::cout << "parse args: " << test << '\n';
  return 1;
}

