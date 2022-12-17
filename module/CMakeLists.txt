set(classes
  vtkF3DODBReader
  )

set(private_headers
  vtkAbqIncludes.h)

vtk_module_find_package(
  PACKAGE AbqSDK
)

vtk_module_add_module(F3D::AbaqusODBReader
  FORCE_STATIC
  CLASSES ${classes}
  PRIVATE_HEADERS ${private_headers})

vtk_module_link(F3D::AbaqusODBReader 
  PRIVATE 
    ${ABQ_LIB_TARGETS}
    )