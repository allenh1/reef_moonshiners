#include <reef_moonshiners/element_base.hpp>

namespace reef_moonshiners
{

ElementBase::ElementBase(const std::string & _name)
: m_name(_name)
{}

const std::string & ElementBase::get_name()
{
  return m_name;
}

void ElementBase::set_name(const std::string & _name)
{
  m_name = _name;
}

double ElementBase::get_tank_size() const
{
  return m_tank_size;
}

double ElementBase::get_max_daily_dosage() const
{
  return m_max_dosage;
}

double ElementBase::get_last_measured_concentration() const
{
  return m_last_measured_concentration;
}

double ElementBase::get_target_concentration() const
{
  return m_target_concentration;
}

const std::chrono::year_month_day & ElementBase::get_last_measurement_date()
{
  return m_last_measurement;
}

double ElementBase::_get_concentration_after_dose(
  const double _dose_ml,
  const double _prior_concentration)
{
  /* added amount of element in micrograms */
  const double added_micrograms_of_element = (_dose_ml * 1E3) * m_element_concentration;
  /* current amount of element in micrograms */
  const double current_micrograms_of_element = _prior_concentration * m_tank_size;
  const double total_micrograms = added_micrograms_of_element + current_micrograms_of_element;
  /* assuming negligible added water due to evaporation, we use the tank volume */
  return total_micrograms / m_tank_size;
}

}  // namespace reef_moonshiners
