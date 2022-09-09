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

#ifndef REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_ENTRY_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_ENTRY_WINDOW_HPP_

#include <QCalendarWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>

namespace reef_moonshiners::ui::icp_import_dialog
{

class ATIEntryWindow : public QWidget
{
  Q_OBJECT

public:
  explicit ATIEntryWindow(QWidget * parent = nullptr);
  ~ATIEntryWindow() override = default;

  QPushButton * get_back_button() const;

  Q_SIGNAL void next_button_pressed(const QString &, const QDate &);

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_button_layout = nullptr;
  QHBoxLayout * m_p_ati_entry_layout = nullptr;
  QLabel * m_p_analysis_id_label = nullptr;
  QLabel * m_p_collection_label = nullptr;
  QPushButton * m_p_next_button = nullptr;
  QPushButton * m_p_back_button = nullptr;
  QLineEdit * m_p_ati_id_entry = nullptr;
  QCalendarWidget * m_p_calendar_widget = nullptr;
};

}  // namespace reef_moonshiners::ui::icp_import_dialog

#endif  // REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ATI_ENTRY_WINDOW_HPP_
