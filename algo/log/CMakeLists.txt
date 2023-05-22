add_library(OnlineDataLog INTERFACE)
target_include_directories(OnlineDataLog INTERFACE .)

install(FILES AlgoFairloggerCompat.h DESTINATION include)
