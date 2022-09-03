#ifndef REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
#define REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_

#include <reef_moonshiners/element_base.hpp>

#include <unorderd_map>

namespace reef_moonshiners
{

class CorrectionElement : public ElementBase
{
public:
  explicit CorrectionElement(const std::string & _name, const double _element_concentration);
  ~CorrectionElement() override = default;

  double get_dose(const std::chrono::year_month_day &) override;

  double get_current_concentration_estimate() const override;

  void apply_dose(const double _dose, const std::chrono::year_month_day & _date) override;

private:
  const double m_daily_dosage;
  /// map of date -> mL dosed
  std::unordered_map<std::chrono::year_month_day, double> m_dosed_amounts;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__CORRECTION_ELEMENT_HPP_
