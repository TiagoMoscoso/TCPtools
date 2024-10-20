# CMakeList.txt: projeto de CMake para ServerTCP, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
add_executable (ServerTCP "ServerTCP.cpp" "ServerTCP.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ServerTCP PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
