cmake_minimum_required(VERSION 3.20)
project(ed2_tp1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ed2_tp1
        Main.cpp
        Main.hpp
        ArvoreB.cpp
        ArvoreB.hpp
        GeradorDeArquivo.cpp
        GeradorDeArquivo.h
        ManipulacaoDeArquivos.cpp
        ManipulacaoDeArquivos.h
        AcessoSequencialIndexado.cpp
        AcessoSequencialIndexado.h
        ArvoreB_estrela.cpp
        ArvoreB_estrela.hpp
        ArvoreBinaria.cpp
        ArvoreBinaria.h
        )
