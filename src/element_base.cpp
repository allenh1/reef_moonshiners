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

#include <reef_moonshiners/element_base.hpp>

#include <fstream>

using namespace std::string_literals;

namespace reef_moonshiners
{

ElementBase::ElementBase(
  const std::string & _name, const double _element_concentration,
  const double _target_concentration, const double _max_adjustment)
: m_name(_name),
  m_element_concentration(_element_concentration),
  m_target_concentration(_target_concentration),
  m_max_adjustment(_max_adjustment)
{}

const std::string & ElementBase::get_name() const
{
  return m_name;
}

void ElementBase::set_name(const std::string & _name)
{
  m_name = _name;
}

void ElementBase::set_tank_size(const double _tank_size)
{
  m_tank_size = _tank_size;
}

double ElementBase::get_tank_size()
{
  return m_tank_size;
}

double ElementBase::get_max_daily_dosage() const
{
  return m_max_adjustment;
}

double ElementBase::get_last_measured_concentration() const
{
  return m_last_measured_concentration;
}

double ElementBase::get_target_concentration() const
{
  return m_target_concentration;
}

double ElementBase::get_element_concentration() const
{
  return m_element_concentration;
}

void ElementBase::set_dosing_unit(DosingUnit _dosing_unit)
{
  m_dosing_unit = _dosing_unit;
}

DosingUnit ElementBase::get_dosing_unit() const
{
  return m_dosing_unit;
}

std::string ElementBase::get_dosing_unit_str() const
{
  switch (m_dosing_unit) {
    case DosingUnit::ML:
      return "mL"s;
    case DosingUnit::DROPS:
      return "drops"s;
  }
}

const std::chrono::year_month_day & ElementBase::get_last_measurement_date() const
{
  return m_last_measurement;
}

void ElementBase::set_concentration(
  const double _concentration,
  const std::chrono::year_month_day & _date)
{
  m_last_measured_concentration = _concentration;
  m_last_measurement = _date;
}

double ElementBase::_get_concentration_after_dose(
  const double _dose_ml,
  const double _prior_concentration) const
{
  /* added amount of element in micrograms */
  const double added_micrograms_of_element = (_dose_ml * 1E-3) * m_element_concentration;
  /* current amount of element in micrograms */
  const double current_micrograms_of_element = _prior_concentration * m_tank_size;
  const double total_micrograms = added_micrograms_of_element + current_micrograms_of_element;
  /* assuming negligible added water due to evaporation, we use the tank volume */
  return total_micrograms / m_tank_size;
}

double ElementBase::_max_daily_dosage_l(const double concentration) const
{
  /* max adjustment is in ug / (L * day) */
  return (m_max_adjustment * m_tank_size) /
         (m_element_concentration - concentration - m_max_adjustment);
}

void ElementBase::write_to(std::ostream & stream) const
{
  binary_out(stream, m_name);
  binary_out(stream, m_estimated_concentration);
  binary_out(stream, m_last_measurement);
  binary_out(stream, m_last_measured_concentration);
  binary_out(stream, m_element_concentration);
  binary_out(stream, m_target_concentration);
  binary_out(stream, m_max_adjustment);
  binary_out(stream, m_dosing_unit);
}

void ElementBase::read_from(std::istream & stream)
{
  binary_in(stream, m_name);
  binary_in(stream, m_estimated_concentration);
  binary_in(stream, m_last_measurement);
  binary_in(stream, m_last_measured_concentration);
  binary_in(stream, m_element_concentration);
  binary_in(stream, m_target_concentration);
  binary_in(stream, m_max_adjustment);
  if (m_load_version >= 1) {
    binary_in(stream, m_dosing_unit);
  }
}

/* stream operators */

std::ostream & operator<<(std::ostream & stream, const ElementBase & element)
{
  element.write_to(stream);
  return stream;
}

std::istream & operator>>(std::istream & stream, ElementBase & element)
{
  element.read_from(stream);
  return stream;
}

}  // namespace reef_moonshiners
