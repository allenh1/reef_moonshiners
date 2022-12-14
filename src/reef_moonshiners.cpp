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

#include <QApplication>
#include <QtWidgets>

#include <reef_moonshiners/element_base.hpp>
#include <reef_moonshiners/ui/main_window.hpp>

int main(int argc, char ** argv)
{
  QApplication app{argc, argv};
  app.setOrganizationDomain("allen-software.com");
  app.setApplicationName("Reef Moonshiners");
  reef_moonshiners::ui::MainWindow main_window;
  app.setActiveWindow(&main_window);
  main_window.show();
  return app.exec();
}
