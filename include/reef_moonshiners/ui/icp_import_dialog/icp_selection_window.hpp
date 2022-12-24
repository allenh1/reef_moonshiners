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

#ifndef REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ICP_SELECTION_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ICP_SELECTION_WINDOW_HPP_

#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

namespace reef_moonshiners::ui::icp_import_dialog
{

enum class IcpSelection : uint8_t
{
  ATI_ICP_OES = 0,
  OCEAMO_ICP_MS = 1,
};

class IcpSelectionWindow : public QWidget
{
  Q_OBJECT

public:
  explicit IcpSelectionWindow(QWidget * parent = nullptr);
  ~IcpSelectionWindow() override = default;

  Q_SIGNAL void next_button_pressed(IcpSelection);

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_button_layout = nullptr;
  QHBoxLayout * m_p_selection_layout = nullptr;
  QLabel * m_p_selection_label = nullptr;
  QComboBox * m_p_combo_box = nullptr;
  QPushButton * m_p_next_button = nullptr;
  QSpacerItem * m_p_button_spacer = nullptr;
};

}  // namespace reef_moonshiners::ui::icp_import_dialog

#endif  // REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__ICP_SELECTION_WINDOW_HPP_
