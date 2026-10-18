idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES mesh_discovery_contract mesh_retained_persistence mesh_startup_lifecycle
)
