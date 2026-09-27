#pragma once

// Incluimos las abstracciones base del motor
#include "Ventana.hpp"
#include "ContextoGrafico.hpp"

// Selector de plataforma en tiempo de compilación para Win32 y Linux (X11)
#if defined(_WIN32) || defined(_WIN64)
    #include "VentanaWin32.hpp"
    #include "ContextoOpenGLWin32.hpp"
    
    using VentanaPlataforma = VentanaWin32;
    using ContextoOpenGLPlataforma = ContextoOpenGLWin32;

#elif defined(__linux__)
    #include "VentanaX11.hpp"
    #include "ContextoOpenGLLinux.hpp"
    
    using VentanaPlataforma = VentanaX11;
    using ContextoOpenGLPlataforma = ContextoOpenGLLinux;

#else
    #error "[Plataforma] Error crítico: Sistema operativo no soportado por el motor nativo."
#endif
