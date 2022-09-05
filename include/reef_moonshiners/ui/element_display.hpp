#ifndef REEF_MOONSHINERS__UI__ELEMENT_DISPLAY_HPP_
#define REEF_MOONSHINERS__UI__ELEMENT_DISPLAY_HPP_

#include <reef_moonshiners/elements.hpp>

#include <memory>

#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QString>
#include <QWidget>

namespace reef_moonshiners::ui
{

class ElementDisplay : public QWidget
{
  Q_OBJECT

public:
  ElementDisplay() = delete;

  explicit ElementDisplay(reef_moonshiners::ElementBase * const element, QWidget * parent = nullptr)
  : QWidget(parent)
  {
    const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now())};
    element->set_concentration(0.0, now);
    m_p_layout = new QHBoxLayout(this);
    m_p_check_box = new QCheckBox(this);
    m_p_name_label = new QLabel(element->get_name().c_str(), this);
    m_p_dose_amount_label = new QLabel(
      QString().setNum(
        (double)element->get_dose(
          now)) + " mL", this);

    m_p_layout->addWidget(m_p_check_box);
    m_p_layout->addWidget(m_p_name_label);
    m_p_layout->addWidget(m_p_dose_amount_label);
  }

  ~ElementDisplay() override = default;

  QHBoxLayout * m_p_layout = nullptr;
  QCheckBox * m_p_check_box = nullptr;
  QLabel * m_p_name_label = nullptr;
  QLabel * m_p_dose_amount_label = nullptr;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__ELEMENT_DISPLAY_HPP_
