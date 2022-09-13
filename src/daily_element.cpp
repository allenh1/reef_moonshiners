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
  const double _target_concentration, const double _max_adjustment)
: ElementBase(_name, _element_concentration, _target_concentration, _max_adjustment)
{
}

double DailyElement::get_dose(const std::chrono::year_month_day &) const
{
  if (this->get_current_concentration_estimate() >= this->get_target_concentration()) {
    /* no need to supplement this, we should not be detecting these elements */
    return 0.0;
  }
  const double dose_in_liters =
    ((this->get_target_concentration() * this->get_tank_size()) /
    (this->get_element_concentration() - this->get_target_concentration()));
  return round_places<2>(dose_in_liters * 1E3) * m_multiplier;
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

void DailyElement::write_to(std::ostream & stream) const
{
  this->ElementBase::write_to(stream);
  binary_out(stream, m_multiplier);
}

void DailyElement::read_from(std::istream & stream)
{
  this->ElementBase::read_from(stream);
  binary_in(stream, m_multiplier);
}

}  // namespace reef_moonshiners
