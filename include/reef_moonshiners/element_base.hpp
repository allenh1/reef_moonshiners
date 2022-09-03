#ifndef REEF_MOONSHINERS__ELEMENT_BASE_HPP_
#define REEF_MOONSHINERS__ELEMENT_BASE_HPP_

#include <string>
#include <chrono>

namespace reef_moonshiners
{

class ElementBase
{
public:
  ElementBase() = default;
  explicit ElementBase(const std::string & _name, const double _element_concentration);
  virtual ~ElementBase() = default;

  /**
   * @brief access the element name
   * @return element name
   */
  const std::string & get_name();

  /**
   * @brief Set the element name
   * @param _name Name of the element
   */
  void set_name(const std::string & _name);

  /**
   * @brief Returns the assumed concentration
   * @return Assumed concentration of this element, in micrograms per liter
   */
  virtual double get_current_concentration_estimate() const = 0;

  /**
   * @brief Get the dosage for a given day
   *
   * @param day Date upon which the dose will occur
   *
   * @return Dosage in milliliters
   */
  virtual double get_dose(const std::chrono::year_month_day & day) = 0;

  virtual void apply_dose(const double _dose, const std::chrono::year_month_day & _date) = 0;

  /**
   * @brief set the concentration at a given time
   *
   * This is the concentration from an ICP test result.
   *
   * @param _concentration Concentration is micrograms per Liter
   * @param _date date the sample was taken
   */
  void set_concentration(const double _concentration, const std::chrono::year_month_day & _date);

  double get_tank_size() const;

  double get_max_daily_dosage() const;

  double get_last_measured_concentration() const;

  const std::chrono::year_month_day & get_last_measurement_date() const;

  double get_target_concentration() const;

  double get_element_concentration() const;

protected:
  double _get_concentration_after_dose(
    const double _dose_ml, const double _current_concentration,
    const double _element_concentration);

private:
  /// name of the element
  std::string m_name;
  /// concentration in micrograms per liter
  double m_estimated_concentration = 0.0;
  /// last measurement date
  std::chrono::year_month_day m_last_measurement;
  /// last measured concentration
  double m_last_measured_concentration = 0.0;
  /// maximum dosage for this element
  double m_max_dosage = 0.0;
  /// tank size (liters)
  inline static double m_tank_size = 0.0;
  /// element concentration in micrograms per liter
  double m_element_concentration = 0.0;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENT_BASE_HPP_
