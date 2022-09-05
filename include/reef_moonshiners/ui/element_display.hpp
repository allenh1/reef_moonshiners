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

  void update_dosage(const reef_moonshiners::ElementBase * element)
  {
    const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now())};
    m_p_dose_amount_label->setText(QString().setNum((double)element->get_dose(now)) + " mL");
  }

  QHBoxLayout * m_p_layout = nullptr;
  QCheckBox * m_p_check_box = nullptr;
  QLabel * m_p_name_label = nullptr;
  QLabel * m_p_dose_amount_label = nullptr;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__ELEMENT_DISPLAY_HPP_
