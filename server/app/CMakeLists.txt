add_library(app STATIC app_monitor.cpp)
target_link_libraries(app PUBLIC workflow wfrest)