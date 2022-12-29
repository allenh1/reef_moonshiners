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

#ifndef REEF_MOONSHINERS__DAILY_ELEMENT_HPP_
#define REEF_MOONSHINERS__DAILY_ELEMENT_HPP_

#include <reef_moonshiners/element_base.hpp>

namespace reef_moonshiners
{

class DailyElement : public ElementBase
{
public:
  explicit DailyElement(
    const std::string & _name, const double _element_concentration,
    const double _nano_element_concentration,
    const double _target_concentration_low,
    const double _target_concentration_high,
    const double _max_adjustment,
    const double _base_adjustment);
  ~DailyElement() override = default;

  double get_dose(const std::chrono::year_month_day &) const override;

  double get_nano_dose() const;

  void set_use_nano_dose(const bool _use_nano_dose);

  bool get_use_nano_dose() const;

  double get_current_concentration_estimate() const override;

  double get_multiplier() const;

  virtual void set_multiplier(const double _multiplier);

  void apply_dose(const double, const std::chrono::year_month_day & _date) override;

  void set_use_ms_mode(const bool _use_ms_mode);

  bool get_use_ms_mode() const;

  void write_to(std::ostream & stream) const override;

  void read_from(std::istream & stream) override;

private:
  double m_multiplier = 1.0;
  double m_nano_concentration = 0.0;
  double m_base_adjustment = 0.0;
  /* these provide a range for the concentration to fall within */
  double m_target_concentration_low = 0.0;
  double m_target_concentration_high = 0.0;
  bool m_use_nano_dose = false;
  bool m_use_ms_mode = false;
};

#define DAILY_ELEMENT(name, element_concentration, nano_element_concentration, target_concentration, \
    base_adjustment) \
  struct name : public DailyElement { \
    name() : DailyElement( \
        #name, (element_concentration), (nano_element_concentration), (target_concentration_low), \
        (target_concentration_high), (base_adjustment), (1.0)) {} }
}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__DAILY_ELEMENT_HPP_
