add_contract(reward reward ${CMAKE_CURRENT_SOURCE_DIR}/src/reward.cpp)
target_include_directories(reward.wasm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(reward.wasm PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian)