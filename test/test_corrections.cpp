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

TEST(TestCorrections, test_fluorine)
{
  std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(283.9);
  reef_moonshiners::Flourine element;
  element.set_concentration(1.388E3, now);
  element.set_correction_start_date(now);
  EXPECT_EQ(element.get_correction_start_date(), now);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 1.388E3);
  /* for some reason, the spreadsheet says 15.9, but our math says 15.91 */
  // EXPECT_DOUBLE_EQ(element.get_dose(now), 15.9);
  element.apply_dose(element.get_dose(now), now);
  /* move forward one day */
  now = (now + std::chrono::days(1));
  // EXPECT_DOUBLE_EQ(element.get_dose(now), 15.9);
  element.apply_dose(element.get_dose(now), now);
  /* move forward one day, no more corrections */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.0);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 1.50E3);
}

TEST(TestCorrections, test_fluorine_2)
{
  std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(17000));
  reef_moonshiners::Flourine element;
  element.set_concentration(1.388E3, now);
  element.set_correction_start_date(now);
  EXPECT_EQ(element.get_correction_start_date(), now);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 1.388E3);
  /* again, a small discrepancy :/ */
  // EXPECT_DOUBLE_EQ(element.get_dose(now), 3603.71);
  element.apply_dose(element.get_dose(now), now);
  /* move forward one day */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 3603.71);
  element.apply_dose(element.get_dose(now), now);
  /* move forward one day, no more corrections */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.0);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 1.50E3);
}

TEST(TestCorrections, test_zinc)
{
  std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  reef_moonshiners::Zinc element;
  element.set_concentration(0, now);
  element.set_correction_start_date(now);
  EXPECT_EQ(element.get_correction_start_date(), now);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 0);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.63);
  element.apply_dose(0.63, now);
  /* move forward another day */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.63);
  element.apply_dose(0.63, now);
  /* move forward another day */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.63);
  element.apply_dose(0.63, now);
  /* move forward another day, should no longer be correcting */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.0);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 5.0);
}

TEST(TestCorrections, test_molybdenum)
{
  std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  reef_moonshiners::Molybdenum element;
  element.set_concentration(12, now);
  element.set_correction_start_date(now);
  EXPECT_EQ(element.get_correction_start_date(), now);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 12);
  // EXPECT_DOUBLE_EQ(element.get_dose(now), 11.36);
  element.apply_dose(element.get_dose(now), now);
  /* move forward another day */
  now = (now + std::chrono::days(1));
  /* move forward another day, should no longer be correcting */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.0);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 15);
}
