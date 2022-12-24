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

#include <reef_moonshiners/ui/icp_import_dialog/oceamo_ms_entry_window.hpp>

#include <QMainWindow>

namespace reef_moonshiners::ui::icp_import_dialog
{

OceamoMSEntryWindow::OceamoMSEntryWindow(QWidget * parent)
: QWidget(parent)
{
  m_p_analysis_id_label = new QLabel(tr("Analysis ID:"));
  m_p_next_button = new QPushButton(tr("&Next"));
  m_p_back_button = new QPushButton(tr("&Back"));

  m_p_oceamo_ms_entry_layout = new QHBoxLayout();
  m_p_oceamo_ms_entry_layout->addWidget(m_p_analysis_id_label);
  // m_p_ati_entry_layout->addWidget(m_p_ati_id_entry);

  m_p_button_layout = new QHBoxLayout();
  m_p_button_layout->addWidget(m_p_back_button);
  m_p_button_layout->addWidget(m_p_next_button);

  m_p_calendar_widget = new QCalendarWidget();
  m_p_collection_label = new QLabel(tr("Select Collection Date:"));

  m_p_main_layout = new QVBoxLayout(this);
  m_p_main_layout->addLayout(m_p_oceamo_ms_entry_layout);
  m_p_main_layout->addWidget(m_p_collection_label);
  m_p_main_layout->addWidget(m_p_calendar_widget);
  m_p_main_layout->addLayout(m_p_button_layout);

  QObject::connect(
    m_p_next_button, &QPushButton::clicked,
    [this, parent]() {
      /* open file dialog */
      QFileDialog * dialog = new QFileDialog(parent);
      reinterpret_cast<QMainWindow *>(parent)->setCentralWidget(dialog);
      QObject::connect(
        dialog, &QFileDialog::fileSelected,
        [this, parent](const QString & file) {
          m_ms_analysis_file = file;
          fprintf(stderr, "selected file: '%s'\n", file.toStdString().c_str());
          reinterpret_cast<QMainWindow *>(parent)->takeCentralWidget();
          reinterpret_cast<QMainWindow *>(parent)->setCentralWidget(this);
          Q_EMIT (next_button_pressed(m_ms_analysis_file, m_p_calendar_widget->selectedDate()));
        });
    });
}

QPushButton * OceamoMSEntryWindow::get_back_button() const
{
  return m_p_back_button;
}

void OceamoMSEntryWindow::show_pdf_load_error_message(const QString & text, const QPdfDocument::Error error)
{
  if (!m_error_message_showing) {
    QString content{text};
    static_cast<void>(error);  /* TODO(allenh1): show the error in text */
    content += ": ";
    m_p_verify_input_label = new QLabel(content);
    m_p_main_layout->insertWidget(1, m_p_verify_input_label);
    m_error_message_showing = true;
  }
}

void OceamoMSEntryWindow::hide_pdf_load_error_message()
{
  if (m_error_message_showing) {
    QLayoutItem * item = m_p_main_layout->takeAt(1);
    delete item->widget();
    delete item;
    m_error_message_showing = false;
    m_p_verify_input_label = nullptr;
  }
}

}  // namespace reef_moonshiners::ui::icp_import_dialog
