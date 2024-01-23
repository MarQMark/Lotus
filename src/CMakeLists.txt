project("Lotus")

add_executable(Lotus
        main.cpp

        systems/PlayerMovementSystem.cpp
        systems/NetworkingClientSystem.cpp
        systems/NetworkingServerSystem.cpp
        systems/EnemySystem.cpp
        systems/CollisionSystem.cpp
        systems/PhysicsSystem.cpp
        systems/CameraSystem.cpp
        systems/EffectSystem.cpp
        systems/TriggerSystem.cpp
        systems/PlayerAnimationSystem.cpp
        systems/PlayerStateSystem.cpp
        systems/AttackAnimationSystem.cpp
        systems/HealthbarSystem.cpp
        systems/BgSpriteSystem.cpp
        systems/ResourceLoadingSystem.cpp
        systems/TitleScreenSystem.cpp

        map/Map.cpp

        util/Spawner.cpp
        util/ResourceManager.cpp
        util/AnimationManager.cpp
)

#copy res to target folder
add_custom_target(copy_res ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying shaders into binary directory")
add_dependencies(Lotus copy_res)

target_include_directories(Lotus
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/kikan/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/kikan/external/glfw/include
)

target_link_directories(${PROJECT_NAME} PUBLIC external/kikan/external/glfw/src)

target_link_directories(${PROJECT_NAME} PUBLIC external/kikan/src)
target_link_directories(${PROJECT_NAME} PUBLIC external/kikan/src/renderer)
target_link_directories(${PROJECT_NAME} PUBLIC external/kikan/src/ecs)

target_link_libraries(Lotus Kikan)