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

#ifndef REEF_MOONSHINERS__UI__MAIN_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QDate>
#include <QCalendarWidget>
#include <QDockWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QToolBar>
#include <QStandardPaths>

#include <reef_moonshiners/elements.hpp>

#include <reef_moonshiners/ui/element_display.hpp>
#include <reef_moonshiners/ui/settings_window.hpp>
#include <reef_moonshiners/ui/icp_import_dialog/icp_selection_window.hpp>
#include <reef_moonshiners/ui/icp_import_dialog/ati_entry_window.hpp>
#include <reef_moonshiners/ui/icp_import_dialog/ati_correction_start_window.hpp>

namespace reef_moonshiners::ui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget * parent = nullptr);
  ~MainWindow() override = default;

protected:
  using IcpSelection = reef_moonshiners::ui::icp_import_dialog::IcpSelection;

  void _fill_element_list();

  Q_SLOT void _refresh_elements();

  Q_SLOT void _activate_settings_window();
  Q_SLOT void _activate_calendar_window();
  Q_SLOT void _activate_icp_import_dialog();

  Q_SLOT void _update_refugium_state(int state);
  Q_SLOT void _update_tank_size(const QString & text);
  Q_SLOT void _update_iodine_drops(int drops);
  Q_SLOT void _update_vanadium_drops(int drops);
  Q_SLOT void _update_rubidium_selection(reef_moonshiners::RubidiumSelection rubidium_selection);
  Q_SLOT void _update_rubidium_start_date(QDate rubidium_start_date);

  Q_SLOT void _handle_next_icp_selection_window(reef_moonshiners::ui::icp_import_dialog::IcpSelection icp_selection);
  Q_SLOT void _handle_back_ati_entry_window();
  Q_SLOT void _handle_next_ati_entry_window(const QString & text, const QDate & date);
  Q_SLOT void _handle_okay_ati_correction_start_window(const QDate & date);
  Q_SLOT void _handle_back_ati_correction_start_window();
  Q_SLOT void _handle_increase_iodine();
  Q_SLOT void _handle_decrease_iodine();
  Q_SLOT void _handle_increase_vanadium();
  Q_SLOT void _handle_decrease_vanadium();
  Q_SLOT void _handle_item_clicked(QListWidgetItem * p_item);

  Q_SLOT void _save();
  bool _load();

private:
  constexpr static size_t m_save_file_version = 2;  /* increment when changes happen to the format */
  int m_refugium_state = Qt::Unchecked;

  QVBoxLayout * m_p_main_layout = nullptr;
  QVBoxLayout * m_p_list_layout = nullptr;
  QLabel * m_p_dose_label = nullptr;
  QDockWidget * m_p_calendar_widget = nullptr;
  QWidget * m_p_central_widget = nullptr;
  QCalendarWidget * m_p_calendar = nullptr;
  QToolBar * m_p_toolbar = nullptr;
  QAction * m_p_import_action = nullptr;
  QAction * m_p_settings_action = nullptr;
  QAction * m_p_calendar_action = nullptr;
  QAction * m_p_about_action = nullptr;

  SettingsWindow * m_p_settings_window = nullptr;
  icp_import_dialog::IcpSelectionWindow * m_p_icp_selection_window = nullptr;
  icp_import_dialog::ATIEntryWindow * m_p_ati_entry_window = nullptr;
  icp_import_dialog::ATICorrectionStartWindow * m_p_ati_correction_start_window = nullptr;

  QWidget * m_p_active_window = nullptr;
  QWidget * m_p_active_icp_selection_window = nullptr;
  QAction * m_p_active_action = nullptr;

  QListWidget * m_p_list_widget = nullptr;

  std::unordered_map<std::unique_ptr<reef_moonshiners::DailyElement>, ElementDisplay *> m_elements;
  std::unordered_map<std::unique_ptr<reef_moonshiners::DropperElement>,
    ElementDisplay *> m_dropper_elements;
  reef_moonshiners::Iodine * m_p_iodine_element = nullptr;
  reef_moonshiners::Vanadium * m_p_vanadium_element = nullptr;
  std::unique_ptr<reef_moonshiners::Rubidium> m_p_rubidium_element = nullptr;
  ElementDisplay * m_p_rubidium_display = nullptr;
  std::unordered_map<std::unique_ptr<reef_moonshiners::CorrectionElement>,
    ElementDisplay *> m_correction_elements;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__MAIN_WINDOW_HPP_
