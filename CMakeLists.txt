cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

project(pc_tema3)

include_directories(Dependencies/nlohmann)

include_directories(./Headers)

include_directories(./Headers/Events)

add_executable(main
        Sources/main.cpp
        Sources/Session.cpp
        Sources/HttpRequest.cpp
        Sources/Handler.cpp
        Sources/Events/Login.cpp
        Sources/Events/Register.cpp
        Sources/Events/Logout.cpp
        Sources/Events/Exit.cpp
        Sources/Events/EnterLibrary.cpp
        Sources/Events/GetBook.cpp
        Sources/Events/GetBooks.cpp
        Sources/Events/AddBook.cpp
        Sources/Events/DeleteBook.cpp
        )