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

#include <cstdio>
#include <reef_moonshiners/correction_element.hpp>

namespace reef_moonshiners
{

CorrectionElement::CorrectionElement(
  const std::string & _name, const double _element_concentration,
  const double _target_concentration,
  const double _max_adjustment)
: ElementBase(_name, _element_concentration, _target_concentration, _max_adjustment)
{
}

double CorrectionElement::get_current_concentration_estimate() const
{
  /* concentration after applying the corrective doses since our last measurement */
  const std::chrono::year_month_day now{
    std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
  return get_concentration_estimate(now);
}

double CorrectionElement::get_concentration_estimate(const std::chrono::year_month_day & date) const
{
  double cummulative_dose_ml = 0.0;
  for (auto iter = this->get_last_measurement_date(); iter < date;
    iter = iter + std::chrono::days(1))
  {
    if (auto dose_iter = m_dosed_amounts.find(iter); dose_iter != m_dosed_amounts.end()) {
      cummulative_dose_ml += dose_iter->second;
    }
  }
  return round_places<0>(
    this->_get_concentration_after_dose(
      cummulative_dose_ml,
      this->get_last_measured_concentration()));
}

void CorrectionElement::apply_dose(const double _dose, const std::chrono::year_month_day & _date)
{
  m_dosed_amounts[_date] = _dose;
}

void CorrectionElement::set_correction_start_date(
  const std::chrono::year_month_day & _correction_start_date)
{
  m_correction_start_date = _correction_start_date;
}

const std::chrono::year_month_day & CorrectionElement::get_correction_start_date() const
{
  return m_correction_start_date;
}

double CorrectionElement::get_dose(const std::chrono::year_month_day & day) const
{
  const double maximum_dose = this->_max_daily_dosage_l(this->get_last_measured_concentration());
  const double total_dose_l = (this->get_tank_size() / this->get_element_concentration()) *
    (this->get_target_concentration() - this->get_last_measured_concentration());
  const double correction_dose_daily =
    truncate_places<2>((total_dose_l / std::ceil(total_dose_l / maximum_dose)) * 1E3);
  if (day >= m_correction_start_date &&
    (day - m_correction_start_date <
    std::chrono::days((uint64_t)std::ceil(total_dose_l / maximum_dose))))
  {
    return correction_dose_daily;
  }
  return 0.0;
}

void CorrectionElement::write_to(std::ostream & stream) const
{
  this->ElementBase::write_to(stream);
  binary_out(stream, m_correction_start_date);
  binary_out(stream, m_dosed_amounts);
}

void CorrectionElement::read_from(std::istream & stream)
{
  this->ElementBase::read_from(stream);
  binary_in(stream, m_correction_start_date);
  binary_in(stream, m_dosed_amounts);
}

/* stream operators */

std::ostream & operator<<(std::ostream & stream, const CorrectionElement & element)
{
  element.write_to(stream);
  return stream;
}

std::istream & operator>>(std::istream & stream, CorrectionElement & element)
{
  element.read_from(stream);
  return stream;
}

}  // namespace reef_moonshiners
