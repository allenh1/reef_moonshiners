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

#include <reef_moonshiners/daily_element.hpp>

#include <cmath>

namespace reef_moonshiners
{

DailyElement::DailyElement(
  const std::string & _name, const double _element_concentration,
  const double _nano_element_concentration,
  const double _target_concentration_low,
  const double _target_concentration_high,
  const double _base_adjustment)
: ElementBase(_name, _element_concentration, _base_adjustment, 1.0),
  m_target_concentration_low(_target_concentration_low),
  m_target_concentration_high(_target_concentration_high),
  m_nano_concentration(_nano_element_concentration)
{
}

double DailyElement::get_dose(const std::chrono::year_month_day &) const
{
  if (this->get_current_concentration_estimate() >= m_target_concentration_high) {
    /* no need to supplement this, we should not be detecting these elements */
    return 0.0;
  }
  const double dose_in_liters =
    ((this->get_target_concentration() * this->get_tank_size()) /
     (m_use_nano_dose ? m_nano_concentration : this->get_element_concentration()));
  return round_places<2>(dose_in_liters * 1E3) * m_multiplier;
}

double DailyElement::get_upper_concentration_bound() const
{
  return m_use_ms_mode ? m_target_concentration_high : 0.0;
}

void DailyElement::update_multiplier_from_ms_results()
{
  if (this->get_last_measured_concentration() < m_target_concentration_low) {
    this->set_multiplier(this->get_multiplier() + 1);
  } else if (this->get_last_measured_concentration() > m_target_concentration_high) {
    this->set_multiplier(this->get_multiplier() - 1);
  }
}

double DailyElement::get_current_concentration_estimate() const
{
  /* under ordinary circumstances, this is constant */
  return this->get_last_measured_concentration();
}


void DailyElement::apply_dose(const double, const std::chrono::year_month_day &)
{
  /* no-op for dailies */
}

double DailyElement::get_multiplier() const
{
  return m_multiplier;
}

void DailyElement::set_multiplier(const double _multiplier)
{
  m_multiplier = _multiplier;
}

bool DailyElement::get_use_nano_dose() const
{
  return m_use_nano_dose;
}

void DailyElement::set_use_nano_dose(const bool _use_nano_dose)
{
  m_use_nano_dose = _use_nano_dose;
}

void DailyElement::set_use_ms_mode(const bool _use_ms_mode)
{
  m_use_ms_mode = _use_ms_mode;
}

bool DailyElement::get_use_ms_mode() const
{
  return m_use_ms_mode;
}

void DailyElement::write_to(std::ostream & stream) const
{
  this->ElementBase::write_to(stream);
  binary_out(stream, m_multiplier);
  int as_int = m_use_nano_dose;
  binary_out(stream, (int)as_int);
  as_int = m_use_ms_mode;
  binary_out(stream, (int)as_int);
}

void DailyElement::read_from(std::istream & stream)
{
  this->ElementBase::read_from(stream);
  binary_in(stream, m_multiplier);
  int nano_dose;
  binary_in(stream, nano_dose);
  m_use_nano_dose = nano_dose;
  int use_ms_mode;
  binary_in(stream, use_ms_mode);
  m_use_ms_mode = use_ms_mode;
}

}  // namespace reef_moonshiners
