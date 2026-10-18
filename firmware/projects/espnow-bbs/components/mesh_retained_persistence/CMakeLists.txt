idf_component_register(
    SRCS
        "src/espnow_bbs_mesh_retained_persistence_codec.c"
        "src/espnow_bbs_mesh_retained_persistence_nvs.c"
    INCLUDE_DIRS "include"
    REQUIRES mesh_discovery_contract
    PRIV_REQUIRES nvs_flash
)
