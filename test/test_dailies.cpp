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
  // reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(300));
  // EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  // EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.11);
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
  // reef_moonshiners::ElementBase::set_tank_size(reef_moonshiners::gallons_to_liters(300));
  // EXPECT_DOUBLE_EQ(iron_element.get_current_concentration_estimate(), 0.0);
  // EXPECT_DOUBLE_EQ(iron_element.get_dose(now), 0.11);
}
