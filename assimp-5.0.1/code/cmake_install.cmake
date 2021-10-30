# Install script for directory: F:/Graphic-Engine/assimp-5.0.1/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Graphic-Engine/assimp-5.0.1/lib/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Graphic-Engine/assimp-5.0.1/lib/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Graphic-Engine/assimp-5.0.1/lib/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/Graphic-Engine/assimp-5.0.1/lib/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Graphic-Engine/assimp-5.0.1/bin/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Graphic-Engine/assimp-5.0.1/bin/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Graphic-Engine/assimp-5.0.1/bin/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/Graphic-Engine/assimp-5.0.1/bin/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/anim.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/aabb.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/ai_assert.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/camera.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/color4.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/color4.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/config.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/defs.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Defines.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/cfileio.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/light.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/material.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/material.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/matrix3x3.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/matrix3x3.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/matrix4x4.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/matrix4x4.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/mesh.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/pbrmaterial.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/postprocess.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/quaternion.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/quaternion.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/scene.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/metadata.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/texture.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/types.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/vector2.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/vector2.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/vector3.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/vector3.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/version.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/cimport.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/importerdesc.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Importer.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/DefaultLogger.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/ProgressHandler.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/IOStream.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/IOSystem.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Logger.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/LogStream.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/NullLogger.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/cexport.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Exporter.hpp"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/DefaultIOStream.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/DefaultIOSystem.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SceneCombiner.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/fast_atof.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/qnan.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/BaseImporter.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Hash.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/MemoryIOWrapper.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/ParsingUtils.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/StreamReader.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/StreamWriter.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/StringComparison.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/StringUtils.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SGSpatialSort.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/GenericProperty.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SpatialSort.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SmoothingGroups.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/SmoothingGroups.inl"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/StandardShapes.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/RemoveComments.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Subdivision.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Vertex.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/LineSplitter.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/TinyFormatter.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Profiler.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/LogAux.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Bitmap.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/XMLTools.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/IOStreamBuffer.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/CreateAnimMesh.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/irrXMLWrapper.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/BlobIOSystem.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/MathFunctions.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Macros.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Exceptional.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Compiler/pushpack1.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Compiler/poppack1.h"
    "F:/Graphic-Engine/assimp-5.0.1/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/Graphic-Engine/assimp-5.0.1/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/Graphic-Engine/assimp-5.0.1/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

