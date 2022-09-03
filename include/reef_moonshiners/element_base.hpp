#ifndef REEF_MOONSHINERS__ELEMENT_BASE_HPP_
#define REEF_MOONSHINERS__ELEMENT_BASE_HPP_

#include <string>
#include <cmath>
#include <chrono>

namespace reef_moonshiners
{

constexpr double gallons_to_liters(const double gallons)
{
  return std::ceil(gallons * 3.78541 * 10) / 10;
}

template<size_t places>
constexpr double round_places(const double d)
{
  return round_places<places - 1>(d * 10) / 10;
}

template<>
constexpr double round_places<1>(const double d)
{
  return std::ceil(d * 10) / 10;
}

class ElementBase
{
public:
  ElementBase() = delete;
  /**
   * @brief Construct an element
   * @param _name Name of the element
   * @param _element_concentration Supplement concentration in micrograms per liter
   * @param _target_concentration Targeted concentration in micrograms per liter
   * @param _max_dose Maximum adjustment in micrograms per liter per day
   */
  explicit ElementBase(
    const std::string & _name, const double _element_concentration,
    const double _target_concentration, const double _max_adjustment);
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

  static void set_tank_size(const double _tank_size);

protected:
  double _get_concentration_after_dose(
    const double _dose_ml,
    const double _prior_concentration) const;

private:
  /// name of the element
  std::string m_name;
  /// concentration in micrograms per liter
  double m_estimated_concentration = 0.0;
  /// last measurement date
  std::chrono::year_month_day m_last_measurement;
  /// last measured concentration
  double m_last_measured_concentration = 0.0;
  /// tank size (liters)
  inline static double m_tank_size = 0.0;
  /// element concentration in micrograms per liter
  const double m_element_concentration;
  /// target concentration in micrograms per liter
  const double m_target_concentration;
  /// maximum adjustment for this element (micrograms per liter per day)
  const double m_max_adjustment;
};

}  // namespace reef_moonshiners

#endif  // REEF_MOONSHINERS__ELEMENT_BASE_HPP_
