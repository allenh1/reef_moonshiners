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

double DailyElement::get_dose(const std::chrono::year_month_day &)
{
  if (this->get_current_concentration_estimate() >= this->get_target_concentration()) {
    /* no need to supplement this, we should not be detecting these elements */
    return 0.0;
  }
  const double dose_in_liters =
    ((this->get_target_concentration() * this->get_tank_size()) /
    (this->get_element_concentration() - this->get_target_concentration()));
  return round_places<2>(dose_in_liters * 1E3);
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


}  // namespace reef_moonshiners
