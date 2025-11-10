/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstefane <lstefane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:48:55 by lstefane          #+#    #+#             */
/*   Updated: 2025/11/10 11:14:45 by lstefane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TESTER

#include <exception>
#include <gtest/gtest.h>

TEST(BTC_Initialize, CanInitialize)
{
  EXPECT_NO_THROW(ex.parseDataBase());
}

/* ------------------------------- FILE TESTS ------------------------------- */

TEST(BTC_File, MissingFile)
{
  EXPECT_THROW(calculateExchange("notfound.csv"), std::exception);
}

/* ------------------------------- INPUT TESTS ----------------------------- */

TEST(BTC_File, dateFormat01)
{
  EXPECT_NO_THROW(calculateExchange("testfiles/date_format01.csv"));
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
