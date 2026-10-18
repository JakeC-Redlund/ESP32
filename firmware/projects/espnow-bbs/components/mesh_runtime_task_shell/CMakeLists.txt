idf_component_register(
    SRCS
        "src/espnow_bbs_mesh_runtime_task_shell.c"
    INCLUDE_DIRS "include"
    REQUIRES mesh_discovery_contract freertos
)
