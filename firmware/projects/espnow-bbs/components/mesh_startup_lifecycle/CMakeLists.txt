idf_component_register(
    SRCS
        "src/espnow_bbs_mesh_startup_lifecycle.c"
    INCLUDE_DIRS "include"
    REQUIRES mesh_runtime_task_shell
)
