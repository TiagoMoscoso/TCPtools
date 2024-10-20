# CMakeList.txt: projeto de CMake para ClientTCP, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
add_executable (ClientTCP "ClientTCP.cpp" "ClientTCP.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ClientTCP PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
