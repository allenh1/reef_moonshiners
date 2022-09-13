# Copyright 2022 Hunter L. Allen
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

macro(generate_pro_file sources headers qt_components qt_resources)
  set(SOURCES "SOURCES =")
  foreach(s ${sources})
    set(SOURCES "${SOURCES} \\\n ${s}")
  endforeach()

  set(HEADERS "HEADERS =")
  foreach(h ${headers})
    set(HEADERS "${HEADERS}\\\n  ${h}")
  endforeach()

  set(RESOURCES "RESOURCES =")
  foreach(r ${qt_resources})
    set(RESOURCES "${RESOURCES}\\\n  ${r}")
  endforeach()

  foreach(c ${qt_components})
    string(TOLOWER "${c}" c)
    set(COMPONENTS "${COMPONENTS} ${c}")
  endforeach()

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/reef_moonshiners.pro.in reef_moonshiners.pro
    @ONLY
  )
endmacro()
