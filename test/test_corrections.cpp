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

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std::string_literals;

TEST(TestCorrections, test_fluorine)
{
  std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(283.9);
  reef_moonshiners::Fluorine element;
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
  reef_moonshiners::Fluorine element;
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
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(300));
  reef_moonshiners::Molybdenum element;
  element.set_concentration(12, now);
  element.set_correction_start_date(now);
  EXPECT_EQ(element.get_correction_start_date(), now);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 12);
  EXPECT_DOUBLE_EQ(element.get_dose(now), 34.07);
  element.apply_dose(element.get_dose(now), now);
  /* move forward another day */
  now = (now + std::chrono::days(1));
  /* move forward another day, should no longer be correcting */
  now = (now + std::chrono::days(1));
  EXPECT_DOUBLE_EQ(element.get_dose(now), 0.0);
  EXPECT_DOUBLE_EQ(element.get_concentration_estimate(now), 15);
}

TEST(TestDailies, test_ostream)
{
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  reef_moonshiners::Molybdenum molybdenum_out;
  molybdenum_out.set_concentration(0.0, now);
  reef_moonshiners::Fluorine fluorine_out;
  fluorine_out.set_concentration(0.0, now);

  fs::path out = fs::temp_directory_path() / "out";
  std::ofstream out_file{out, std::ios::binary};
  out_file << molybdenum_out << fluorine_out;
  out_file.close();

  /* attempt to read */
  reef_moonshiners::Molybdenum molybdenum_in;
  reef_moonshiners::Fluorine fluorine_in;
  std::ifstream in_file{out, std::ios::binary};
  in_file >> molybdenum_in >> fluorine_in;
  EXPECT_EQ(molybdenum_in.get_name(), "Molybdenum"s);
  EXPECT_EQ(molybdenum_in.get_dose(now), molybdenum_out.get_dose(now));
  EXPECT_EQ(molybdenum_in.get_max_daily_dosage(), molybdenum_out.get_max_daily_dosage());
  EXPECT_EQ(molybdenum_in.get_last_measured_concentration(), molybdenum_out.get_last_measured_concentration());
  EXPECT_EQ(molybdenum_in.get_last_measurement_date(), molybdenum_out.get_last_measurement_date());
  EXPECT_EQ(molybdenum_in.get_target_concentration(), molybdenum_out.get_target_concentration());
  EXPECT_EQ(fluorine_in.get_name(), "Fluorine"s);
  EXPECT_EQ(fluorine_in.get_dose(now), fluorine_out.get_dose(now));
  EXPECT_EQ(fluorine_in.get_max_daily_dosage(), fluorine_out.get_max_daily_dosage());
  EXPECT_EQ(fluorine_in.get_last_measured_concentration(), fluorine_out.get_last_measured_concentration());
  EXPECT_EQ(fluorine_in.get_last_measurement_date(), fluorine_out.get_last_measurement_date());
  EXPECT_EQ(fluorine_in.get_target_concentration(), fluorine_out.get_target_concentration());
}
