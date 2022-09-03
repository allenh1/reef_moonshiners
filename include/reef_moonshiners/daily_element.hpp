#ifndef REEF_MOONSHINERS__DAILY_ELEMENT_HPP_
#define REEF_MOONSHINERS__DAILY_ELEMENT_HPP_

#include <reef_moonshiners/element_base.hpp>

namespace reef_moonshiners
{

class DailyElement : public ElementBase
{
public:
  explicit DailyElement(const std::string & _name, const double _element_concentration);
  ~DailyElement() override = default;

  double get_dose(const std::chrono::year_month_day &) override;

  double get_current_concentration_estimate() override;

  void apply_dose(const double, const std::chrono::year_month_day & _date) override;

private:
  const double m_daily_dosage;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__DAILY_ELEMENT_HPP_
