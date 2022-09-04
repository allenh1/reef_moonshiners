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

#ifndef REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
#define REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_

#include <reef_moonshiners/element_base.hpp>

#include <unordered_map>

namespace std
{
template<>
struct hash<std::chrono::year_month_day>
{
  size_t operator()(const std::chrono::year_month_day & date) const
  {
    std::chrono::sys_time<std::chrono::days> days{std::chrono::sys_days{date}};
    return days.time_since_epoch().count();
  }
};

namespace chrono
{

/**
 * @brief addition operator overload
 *
 * I legit don't understand how this didn't make the standard...
 *
 * @param lhs date to add to
 * @param rhs number of days to increase it
 *
 * @return rhs days after lhs
 */
constexpr year_month_day operator+(const year_month_day & lhs, const days & rhs)
{
  return year_month_day(sys_days{lhs} + rhs);
}

constexpr days operator-(const year_month_day & lhs, const year_month_day & rhs)
{
  return sys_days{lhs} - sys_days{rhs};
}
}  // namespace chrono
}  // namespace std

namespace reef_moonshiners
{

class CorrectionElement : public ElementBase
{
public:
  explicit CorrectionElement(
    const std::string & _name, const double _element_concentration,
    const double _target_concentration, const double _max_adjustment);
  ~CorrectionElement() override = default;

  double get_dose(const std::chrono::year_month_day &) const override;

  double get_current_concentration_estimate() const override;

  /**
   * @brief Access date on which we started corrections
   * @return Date corrections started
   */
  const std::chrono::year_month_day & get_correction_start_date() const;

  void set_correction_start_date(const std::chrono::year_month_day & _correction_start_date);

  /**
   * @brief Test function for future doses
   *
   * There is no assumed consumption of this element.
   *
   * @param date Date to which we iterate
   *
   * @return Concentration of this element on date, after doses have applied
   */
  double get_concentration_estimate(const std::chrono::year_month_day & date) const;

  void apply_dose(const double _dose, const std::chrono::year_month_day & _date) override;

private:
  constexpr double _concentration_after_dose(const double dose_l);

  std::chrono::year_month_day m_correction_start_date;

  /// map of date -> mL dosed
  std::unordered_map<std::chrono::year_month_day, double> m_dosed_amounts;
};

}  // namespace reef_moonshiners

#define CORRECTION_ELEMENT(name, element_concentration, target_concentration, max_adjustment) \
  struct name : public CorrectionElement { \
    name() : CorrectionElement( \
        #name, (element_concentration), (target_concentration), \
        (max_adjustment)) {} }

#endif  // REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
