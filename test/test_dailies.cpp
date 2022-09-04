// Copyright 2022 Hunter L. Allen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <reef_moonshiners/elements.hpp>

TEST(TestDailes, test_iron)
{
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(75));
  reef_moonshiners::Iron iron_element;
  /* suppose we measured 0.0 for the concentration */
  iron_element.set_concentration(0.0, now);
  EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.03);
  /* change tank size to 50 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(50));
  EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.02);
  /* change tank size to 100 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.04);
  /* change tank size to 300 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(300));
  EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.11);
}

TEST(TestDailes, test_selenium)
{
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(75));
  reef_moonshiners::Selenium element;
  /* suppose we measured 0.0 for the concentration */
  element.set_concentration(0.0, now);
  EXPECT_DOUBLE_EQ(element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.06);
  /* change tank size to 50 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(50));
  EXPECT_DOUBLE_EQ(element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.04);
  /* change tank size to 100 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  EXPECT_DOUBLE_EQ(element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.08);
  /* change tank size to 300 gallons */
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(300));
  EXPECT_DOUBLE_EQ(element.get_current_concentration_estimate(), 0.0);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.23);
}
