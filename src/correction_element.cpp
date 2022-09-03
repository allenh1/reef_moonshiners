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
  const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
      std::chrono::system_clock::now())};
  double cummulative_dose_ml = 0.0;
  for (auto iter = this->get_last_measurement_date(); iter < now;
    iter = iter + std::chrono::days(1))
  {
    if (auto dose_iter = m_dosed_amounts.find(iter); dose_iter != m_dosed_amounts.end()) {
      cummulative_dose_ml += dose_iter->second;
    }
  }
  return _get_concentration_after_dose(
    cummulative_dose_ml,
    this->get_last_measured_concentration());
}

void CorrectionElement::apply_dose(const double _dose, const std::chrono::year_month_day & _date)
{
  m_dosed_amounts[_date] = _dose;
}

}  // namespace reef_moonshiners
