find_path(
  IMGUI_INCLUDE_DIR "imgui.h"
  /usr/include
  D:/ProgrammingProjects/imgui-1.89.4)

SET(IMGUI_FOUND "NO")
IF(IMGUI_INCLUDE_DIR)
    SET(IMGUI_FOUND "YES")
ENDIF()
