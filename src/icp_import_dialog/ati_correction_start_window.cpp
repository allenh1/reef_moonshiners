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

#include <reef_moonshiners/ui/icp_import_dialog/ati_correction_start_window.hpp>

namespace reef_moonshiners::ui::icp_import_dialog
{

ATICorrectionStartWindow::ATICorrectionStartWindow(QWidget * parent)
: QWidget(parent)
{
  m_p_correction_label = new QLabel(tr("Select Corrections Start Date:"));
  m_p_iodine_label = new QLabel();
  m_p_vanadium_label = new QLabel();
  m_p_okay_button = new QPushButton(tr("&Okay"));
  m_p_back_button = new QPushButton(tr("&Back"));
  m_p_iodine_checkbox = new QCheckBox();
  m_p_iodine_checkbox->setStyleSheet(tr("QCheckBox::indicator {width: 30px; height: 30px}"));
  m_p_vanadium_checkbox = new QCheckBox();
  m_p_vanadium_checkbox->setStyleSheet(tr("QCheckBox::indicator {width: 30px; height: 30px}"));

  m_p_button_layout = new QHBoxLayout();
  m_p_button_layout->addWidget(m_p_back_button);
  m_p_button_layout->addWidget(m_p_okay_button);

  m_p_iodine_layout = new QHBoxLayout();
  m_p_iodine_layout->addWidget(m_p_iodine_label);
  m_p_iodine_layout->addWidget(m_p_iodine_checkbox);

  m_p_vanadium_layout = new QHBoxLayout();
  m_p_vanadium_layout->addWidget(m_p_vanadium_label);
  m_p_vanadium_layout->addWidget(m_p_vanadium_checkbox);

  m_p_calendar_widget = new QCalendarWidget();

  m_p_main_layout = new QVBoxLayout(this);
  m_p_main_layout->addWidget(m_p_correction_label);
  m_p_main_layout->addWidget(m_p_calendar_widget);
  m_p_main_layout->addLayout(m_p_button_layout);

  QObject::connect(
    m_p_okay_button, &QPushButton::clicked,
    [this]() {
      if (m_p_iodine_checkbox->checkState() == Qt::Checked) {
        if (m_iodine_low) {
          Q_EMIT (increase_iodine());
        } else if (m_iodine_high) {
          Q_EMIT (decrease_iodine());
        }
      }
      if (m_p_vanadium_checkbox->checkState() == Qt::Checked) {
        if (m_vanadium_low) {
          Q_EMIT (increase_vanadium());
        } else if (m_vanadium_high) {
          Q_EMIT (decrease_vanadium());
        }
      }
      Q_EMIT (okay_button_pressed(m_p_calendar_widget->selectedDate()));
    });
}

QPushButton * ATICorrectionStartWindow::get_back_button() const
{
  return m_p_back_button;
}

void ATICorrectionStartWindow::set_iodine_increase(const bool increase)
{
  m_iodine_low = increase;
  if (m_iodine_low) {
    m_p_iodine_label->setText(tr("Iodine is too low. Increase?"));
    m_p_main_layout->insertLayout(0, m_p_iodine_layout);
  }
}

void ATICorrectionStartWindow::set_vanadium_increase(const bool increase)
{
  m_vanadium_low = increase;
  if (m_vanadium_low) {
    m_p_vanadium_label->setText(tr("Vanadium is too low. Increase?"));
    m_p_main_layout->insertLayout(0, m_p_vanadium_layout);
  }
}

void ATICorrectionStartWindow::set_iodine_decrease(const bool decrease)
{
  m_iodine_high = decrease;
  if (m_iodine_high) {
    m_p_iodine_label->setText(tr("Iodine is too high. Decrease?"));
    m_p_main_layout->insertLayout(0, m_p_iodine_layout);
  }
}

void ATICorrectionStartWindow::set_vanadium_decrease(const bool decrease)
{
  m_vanadium_high = decrease;
  if (m_vanadium_high) {
    m_p_vanadium_label->setText(tr("Vanadium is too high. Decrease?"));
    m_p_main_layout->insertLayout(0, m_p_vanadium_layout);
  }
}

}  // namespace reef_moonshiners::ui::icp_import_dialog
