add_executable(gui_genetic
    src/main.cpp
    src/genetic_app.cpp
    src/genetic_frame.cpp
    src/plot.cpp
    src/shared.cpp
    src/genetic_controller.cpp
    src/file_frames.cpp
    src/settings_frames.cpp
    src/genetic_handlers.cpp
)

target_include_directories(gui_genetic PRIVATE ${wxWidgets_INCLUDE_DIRS} include)
target_link_libraries(gui_genetic PRIVATE ${wxWidgets_LIBRARIES} OpenGL::GL OpenGL::GLU glut genetic)
