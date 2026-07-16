#MyFunction.cmake - вспомогательные функции для CMake
#
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

function(setup_my_executable EXE_NAME SOURCE_FILE LIB_NAME)
    
    message(STATUS ">>> Настройка сборки программы [${EXE_NAME}] из [${SOURCE_FILE}]...")

    add_executable(${EXE_NAME} ${SOURCE_FILE})
    target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})

    set_target_properties(${EXE_NAME} PROPERTIES
        COMMENT "Сборка исполняемого файла ${EXE_NAME} завершена успешно!"
    )

endfunction()