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

TEST(TestDailies, test_ostream)
{
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(100));
  reef_moonshiners::Selenium selenium_out;
  selenium_out.set_concentration(0.0, now);
  reef_moonshiners::Iron iron_out;
  iron_out.set_concentration(0.0, now);
  iron_out.set_multiplier(2.0);

  fs::path out = fs::temp_directory_path() / "out";
  std::ofstream out_file{out, std::ios::binary};
  static constexpr size_t out_version = 3;
  reef_moonshiners::binary_out(out_file, out_version);
  out_file << selenium_out << iron_out;
  out_file.close();

  /* attempt to read */
  reef_moonshiners::Selenium selenium_in;
  reef_moonshiners::Iron iron_in;
  std::ifstream in_file{out, std::ios::binary};
  size_t input_version;
  reef_moonshiners::binary_in(in_file, input_version);
  EXPECT_EQ(input_version, out_version);
  reef_moonshiners::ElementBase::set_load_version(input_version);
  in_file >> selenium_in >> iron_in;
  EXPECT_EQ(selenium_in.get_name(), "Selenium"s);
  EXPECT_EQ(selenium_in.get_multiplier(), selenium_out.get_multiplier());
  EXPECT_EQ(selenium_in.get_dose(now), selenium_out.get_dose(now));
  EXPECT_EQ(selenium_in.get_max_daily_dosage(), selenium_out.get_max_daily_dosage());
  EXPECT_EQ(selenium_in.get_last_measured_concentration(), selenium_out.get_last_measured_concentration());
  EXPECT_EQ(selenium_in.get_last_measurement_date(), selenium_out.get_last_measurement_date());
  EXPECT_EQ(selenium_in.get_target_concentration(), selenium_out.get_target_concentration());
  EXPECT_EQ(iron_in.get_name(), "Iron"s);
  EXPECT_EQ(iron_in.get_multiplier(), iron_out.get_multiplier());
  EXPECT_EQ(iron_in.get_dose(now), iron_out.get_dose(now));
  EXPECT_EQ(iron_in.get_max_daily_dosage(), iron_out.get_max_daily_dosage());
  EXPECT_EQ(iron_in.get_last_measured_concentration(), iron_out.get_last_measured_concentration());
  EXPECT_EQ(iron_in.get_last_measurement_date(), iron_out.get_last_measurement_date());
  EXPECT_EQ(iron_in.get_target_concentration(), iron_out.get_target_concentration());
}

TEST(TestDailes, test_selenium_ms)
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
