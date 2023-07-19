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

#ifndef REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__OCEAMO_MS_ENTRY_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__OCEAMO_MS_ENTRY_WINDOW_HPP_

#include <QCalendarWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QPdfDocument>
#include <QFileDialog>

namespace reef_moonshiners::ui::icp_import_dialog
{

class OceamoMSEntryWindow : public QWidget
{
  Q_OBJECT

public:
  explicit OceamoMSEntryWindow(QWidget * parent = nullptr);
  ~OceamoMSEntryWindow() override = default;

  QPushButton * get_back_button() const;

  Q_SIGNAL void next_button_pressed(const QDate &);

  void show_pdf_load_error_message(
    const QString & text,
    const QPdfDocument::Error error = QPdfDocument::Error::None);
  void hide_pdf_load_error_message();

private:
  QVBoxLayout * m_p_main_layout = nullptr;
  QHBoxLayout * m_p_button_layout = nullptr;
  QHBoxLayout * m_p_oceamo_ms_entry_layout = nullptr;
  QLabel * m_p_analysis_id_label = nullptr;
  QLabel * m_p_collection_label = nullptr;
  QLabel * m_p_verify_input_label = nullptr;
  QPushButton * m_p_next_button = nullptr;
  QPushButton * m_p_back_button = nullptr;
  QFileDialog * m_p_file_dialog = nullptr;
  QCalendarWidget * m_p_calendar_widget = nullptr;
  QString m_ms_analysis_file;
  bool m_error_message_showing = false;
};

}  // namespace reef_moonshiners::ui::icp_import_dialog

#endif  // REEF_MOONSHINERS__UI__ICP_IMPORT_DIALOG__OCEAMO_MS_ENTRY_WINDOW_HPP_
