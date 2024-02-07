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

#include <reef_moonshiners/rubidium_element.hpp>

namespace reef_moonshiners
{

Rubidium::Rubidium()
: DailyElement("Rubidium", 1000E3, 1000E3, 0.2, 0.2, 350)
{
}

double Rubidium::get_target_concentration() const
{
  switch (m_dosing_frequency) {
    case RubidiumSelection::DAILY:
      return m_daily_concentration;
    case RubidiumSelection::MONTHLY:
      return m_monthly_concentration;
    case RubidiumSelection::QUARTERLY:
      return m_quarterly_concentration;
    case RubidiumSelection::INITIAL:
      return m_initial_concentration;
  }
  return 0.0;
}

void Rubidium::set_dosing_frequency(RubidiumSelection _dosing_frequency)
{
  m_dosing_frequency = _dosing_frequency;
}

RubidiumSelection Rubidium::get_dosing_frequency() const
{
  return m_dosing_frequency;
}

void Rubidium::set_initial_dose_date(const std::chrono::year_month_day & date)
{
  m_initial_rubidium_dose_date = date;
}

std::chrono::year_month_day Rubidium::get_initial_dose_date() const
{
  return m_initial_rubidium_dose_date;
}

double Rubidium::get_dose(const std::chrono::year_month_day & date) const
{
  double dose = 0.0;
  switch (m_dosing_frequency) {
    case RubidiumSelection::DAILY:
      /* we are just a normal daily element, unless it's the initial date */
      if (date == m_initial_rubidium_dose_date) {
        /* temporarily swap the dosing frequency */
        m_dosing_frequency = RubidiumSelection::INITIAL;
        dose = this->DailyElement::get_dose(date);
        m_dosing_frequency = RubidiumSelection::DAILY;
      } else {
        dose = this->DailyElement::get_dose(date);
      }
      break;
    case RubidiumSelection::MONTHLY:
      if (date == m_initial_rubidium_dose_date) {
        m_dosing_frequency = RubidiumSelection::INITIAL;
        dose = this->DailyElement::get_dose(date);
        m_dosing_frequency = RubidiumSelection::MONTHLY;
      } else if (date.day() == m_initial_rubidium_dose_date.day()) {
        dose = this->DailyElement::get_dose(date);
      }
      break;
    case RubidiumSelection::QUARTERLY:
      {
        /* your days are fixed on the start of the quarter */
        if (date == m_initial_rubidium_dose_date) {
          m_dosing_frequency = RubidiumSelection::INITIAL;
          dose = this->DailyElement::get_dose(date);
          m_dosing_frequency = RubidiumSelection::QUARTERLY;
        } else if (date.day() == std::chrono::day(1) && date.month() == std::chrono::January) {
          dose = this->DailyElement::get_dose(date);
        } else if (date.day() == std::chrono::day(1) && date.month() == std::chrono::April) {
          dose = this->DailyElement::get_dose(date);
        } else if (date.day() == std::chrono::day(1) && date.month() == std::chrono::July) {
          dose = this->DailyElement::get_dose(date);
        } else if (date.day() == std::chrono::day(1) && date.month() == std::chrono::October) {
          dose = this->DailyElement::get_dose(date);
        }
      }
    case RubidiumSelection::INITIAL:
      break;
  }
  return dose;
}

void Rubidium::write_to(std::ostream & stream) const
{
  this->DailyElement::write_to(stream);
  binary_out(stream, static_cast<uint8_t>(m_dosing_frequency));
  binary_out(stream, m_initial_rubidium_dose_date);
}

void Rubidium::read_from(std::istream & stream)
{
  this->DailyElement::read_from(stream);
  uint8_t freq;
  binary_in(stream, freq);
  m_dosing_frequency = static_cast<RubidiumSelection>(freq);
  binary_in(stream, m_initial_rubidium_dose_date);
}

}  // namespace reef_moonshiners
