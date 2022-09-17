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

#include <QDate>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>

namespace reef_moonshiners::ui
{

class ElementDisplay final : public QListWidgetItem
{
  // Q_OBJECT

public:
  ElementDisplay() = delete;

  explicit ElementDisplay(
    reef_moonshiners::ElementBase * const element,
    QListWidget * _parent_list)
  : QListWidgetItem(_parent_list, QListWidgetItem::UserType),
    m_p_element(element),
    m_p_parent_list(_parent_list)
  {
    const std::chrono::year_month_day now{std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now())};
    element->set_concentration(0.0, now);
    // this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFlags(this->flags() | Qt::ItemIsUserCheckable);
    this->setCheckState(Qt::Unchecked);
    this->listWidget()->addItem(this);
  }

  ~ElementDisplay() final = default;

  QVariant data(int role) const final
  {
    switch (role) {
      case Qt::DisplayRole:
        {
          if ((nullptr == m_p_element)) {
            return QVariant{};
          } else if (0.0 == m_p_element->get_dose(m_selected_date)) {
            return QVariant{};
          }
          return m_text;
        }
        break;
      case Qt::CheckStateRole:
        return m_checked ? Qt::Checked : Qt::Unchecked;
        break;
    }
    return QVariant{};
  }

  void set_checked(const bool _checked)
  {
    if (m_checked && _checked) {
      return;
    } else if (!m_checked && _checked) {
      m_checked = _checked;
      setCheckState(Qt::Checked);
    } else if (!m_checked && !_checked) {
      return;
    } else if (m_checked && !_checked) {
      m_checked = _checked;
      setCheckState(Qt::Unchecked);
    }
  }

  void update_dosage(const QDate & _date)
  {
    if (nullptr == m_p_element) {
      fprintf(stderr, "m_p_element == nullptr\n");
      return;
    }
    int year, month, day;
    _date.getDate(&year, &month, &day);
    m_selected_date = std::chrono::year_month_day{
      std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
    if (0.0 == m_p_element->get_dose(m_selected_date)) {
      if (nullptr == this->listWidget()) {
        return;
      }
      this->listWidget()->takeItem(listWidget()->row(this));
    } else if (nullptr == this->listWidget()) {
      m_p_parent_list->addItem(this);
    }
    const QString element_name{m_p_element->get_name().c_str()};
    QString dose_amount{};
    dose_amount.setNum(m_p_element->get_dose(m_selected_date));
    dose_amount += QString(" ") + QString(m_p_element->get_dosing_unit_str().c_str());
    m_text = element_name + QString("\t") + dose_amount;
    this->setData(Qt::DisplayRole, m_text);
  }

  reef_moonshiners::ElementBase * m_p_element = nullptr;
  QListWidget * m_p_parent_list;
  std::chrono::year_month_day m_selected_date;
  bool m_checked = false;
  QString m_text;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__ELEMENT_DISPLAY_HPP_
