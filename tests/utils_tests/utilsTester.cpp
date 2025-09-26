/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsTester.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:08:26 by dpotsch           #+#    #+#             */
/*   Updated: 2025/09/24 14:52:52 by dpotsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "utils/parseArgs.hpp"
#include <gtest/gtest.h>

TEST(TESTER_NAME, arraySize2)
{
  EXPECT_EQ(parseArgs(), 1);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
