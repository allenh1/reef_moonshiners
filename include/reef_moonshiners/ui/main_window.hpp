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

#include <reef_moonshiners/elements.hpp>

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
  QVBoxLayout m_main_layout;
  QVBoxLayout m_list_layout;
  QLabel m_dose_label;
  QWidget m_central_widget;
  QCalendarWidget m_calendar;

  std::vector<std::unique_ptr<reef_moonshiners::ElementBase>> m_elements;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__MAIN_WINDOW_HPP_
