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

#ifndef REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_CORRECTION_START_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_CORRECTION_START_WINDOW_HPP_

#include <QCalendarWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>

namespace reef_moonshiners::ui::icp_import_dialog
{

class ATICorrectionStartWindow : public QWidget
{
  Q_OBJECT

public:
  explicit ATICorrectionStartWindow(QWidget * parent = nullptr);
  ~ATICorrectionStartWindow() override = default;

  QPushButton * get_back_button() const;

  Q_SIGNAL void okay_button_pressed(const QDate &);

  Q_SIGNAL void increase_iodine();
  Q_SIGNAL void decrease_iodine();

  void set_iodine_increase(const bool increase);
  void set_iodine_decrease(const bool decrease);

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_button_layout = nullptr;
  QHBoxLayout * m_p_iodine_layout = nullptr;
  QLabel * m_p_correction_label = nullptr;
  QCalendarWidget * m_p_calendar_widget = nullptr;
  QPushButton * m_p_okay_button = nullptr;
  QPushButton * m_p_back_button = nullptr;
  QLabel * m_p_iodine_label = nullptr;
  QCheckBox * m_p_iodine_checkbox = nullptr;

  bool m_iodine_low = false;
  bool m_iodine_high = false;
};

}  // namespace reef_moonshiners::ui::icp_import_dialog

#endif  // REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_CORRECTION_START_WINDOW_HPP_
