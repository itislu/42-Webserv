/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseArgs.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:09:31 by dpotsch           #+#    #+#             */
/*   Updated: 2025/09/12 09:08:15 by dpotsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseArgs.hpp"
#include "libftpp/trim/trim.hpp"
#include <iostream>
#include <string>

int parseArgs() {
  std::cout << "test\n";
  std::string test("   test   ");
  trim(test);
  std::cout << "parse args: " << test << '\n';
  return 1;
}

