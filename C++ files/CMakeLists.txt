cmake_minimum_required(VERSION 3.28)
project(TournamentBot)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

include_directories("D:/pgsql/include")
link_directories("D:/pgsql/lib")

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})

set(SOURCE_FILES
        main.cpp
        Database.h
        Database.cpp
        Operations.h
        Operations.cpp
        Translate.h
        Translate.cpp
        ParticipantManager.h
        ParticipantManager.cpp
        ResultManager.h
        ResultManager.cpp
        GroupManager.h
        GroupManager.cpp
        CSVManager.h
        CSVManager.cpp
        Participant.h
        SQLCommands.h
        SQLCommands.cpp
        SQLRequests.h
        SQLRequests.cpp
        Operation.h
)

add_executable(TournamentBot ${SOURCE_FILES})

target_link_libraries(TournamentBot ${PostgreSQL_LIBRARIES})
