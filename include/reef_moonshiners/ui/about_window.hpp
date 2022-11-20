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

#ifndef REEF_MOONSHINERS__UI__ABOUT_WINDOW_HPP_
#define REEF_MOONSHINERS__UI__ABOUT_WINDOW_HPP_

#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QWidget>

namespace reef_moonshiners::ui
{

class AboutWindow final : public QWidget
{
  Q_OBJECT

public:
  explicit AboutWindow(QWidget * parent = nullptr);
  ~AboutWindow() final = default;

  void resizeEvent(QResizeEvent * event) final;

private:
  QPixmap m_moonshiner_logo;

  QVBoxLayout * m_p_main_layout = nullptr;
  QLabel * m_p_about_label = nullptr;
  QLabel * m_p_image_label = nullptr;
  QLabel * m_p_faq_link_label = nullptr;
  QLabel * m_p_handbook_link_label = nullptr;
  QLabel * m_p_faq_label = nullptr;
};

}  // namespace reef_moonshiners::ui

#endif  // REEF_MOONSHINERS__UI__ABOUT_WINDOW_HPP_
