find_path(
  GLAD_INCLUDE_DIR "glad/glad.h"
  /usr/include
  D:/ProgrammingProjects/glad/include)

SET(GLAD_FOUND "NO")
IF(GLAD_INCLUDE_DIR)
    SET(GLAD_FOUND "YES")
ENDIF()
