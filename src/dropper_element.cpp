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

#include <reef_moonshiners/dropper_element.hpp>

namespace reef_moonshiners
{

DropperElement::DropperElement(
  const std::string & _name,
  const double _low_concentration,
  const double _high_concentration)
: DailyElement(_name, 0.0, _low_concentration, 0.0),
  m_high_concentration(_high_concentration)
{
  this->set_dosing_unit(DosingUnit::DROPS);
}

void DropperElement::set_multiplier(const double)
{
  DailyElement::set_multiplier(1.0);
}

void DropperElement::set_drops(const size_t _drops)
{
  m_drops = _drops;
}

double DropperElement::get_dose(const std::chrono::year_month_day &) const
{
  return static_cast<double>(m_drops);
}

bool DropperElement::is_low() const
{
  return this->get_current_concentration_estimate() < this->get_target_concentration();
}

bool DropperElement::is_high() const
{
  return this->get_current_concentration_estimate() > m_high_concentration;
}

void DropperElement::write_to(std::ostream & stream) const
{
  this->ElementBase::write_to(stream);
  binary_out(stream, m_drops);
}

void DropperElement::read_from(std::istream & stream)
{
  this->ElementBase::read_from(stream);
  binary_in(stream, m_drops);
}

}  // namespace reef_moonshiners
