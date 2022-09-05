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

#ifndef REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

namespace reef_moonshiners::ui
{

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget * parent = nullptr)
  : QWidget(parent)
  {
    m_p_tank_size_edit = new QLineEdit(tr("75.0"), this);
    m_p_tank_size_label = new QLabel(tr("Tank Size (gallons):"), this);
    m_p_refugium_label = new QLabel(tr("Refugium:"), this);
    m_p_refugium_checkbox = new QCheckBox(this);


    m_p_tank_size_layout = new QHBoxLayout();
    m_p_tank_size_layout->addWidget(m_p_tank_size_label);
    m_p_tank_size_layout->addWidget(m_p_tank_size_edit);

    m_p_refugium_layout = new QHBoxLayout();
    m_p_refugium_layout->addWidget(m_p_refugium_label);
    m_p_refugium_layout->addWidget(m_p_refugium_checkbox);

    m_p_main_layout = new QVBoxLayout();
    m_p_main_layout->addLayout(m_p_tank_size_layout);
    m_p_main_layout->addLayout(m_p_refugium_layout);

    this->setLayout(m_p_main_layout);
  }

  ~SettingsWindow() override = default;

  QLineEdit * get_tank_size_edit() const
  {
    return m_p_tank_size_edit;
  }

  QCheckBox * get_refugium_checkbox() const
  {
    return m_p_refugium_checkbox;
  }

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_refugium_layout = nullptr;
  QHBoxLayout * m_p_tank_size_layout = nullptr;
  QLineEdit * m_p_tank_size_edit = nullptr;
  QLabel * m_p_tank_size_label = nullptr;
  QLabel * m_p_refugium_label = nullptr;
  QCheckBox * m_p_refugium_checkbox = nullptr;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__SETTINGS_WINDOW_HPP_
