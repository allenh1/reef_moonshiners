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
#include <QCalendarWidget>
#include <QDockWidget>
#include <QToolBar>

#include <reef_moonshiners/elements.hpp>

#include <reef_moonshiners/ui/element_display.hpp>

namespace reef_moonshiners::ui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget * parent = nullptr);
  ~MainWindow() override = default;

protected:
  void _fill_element_list();
  void _populate_list_layout();

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QVBoxLayout * m_p_list_layout = nullptr;
  QLabel * m_p_dose_label = nullptr;
  QDockWidget * m_p_calendar_widget = nullptr;
  QWidget * m_p_central_widget = nullptr;
  QWidget * m_p_list_widget = nullptr;
  QCalendarWidget * m_p_calendar = nullptr;
  QToolBar * m_p_toolbar = nullptr;
  QAction * m_p_import_action = nullptr;
  QAction * m_p_settings_action = nullptr;
  QAction * m_p_calendar_action = nullptr;
  QAction * m_p_about_action = nullptr;

  std::unordered_map<std::unique_ptr<reef_moonshiners::ElementBase>, ElementDisplay *> m_elements;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__MAIN_WINDOW_HPP_
