#pragma once
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

// ==========================================
// GESTOR DE AUDIO MULTIPLATAFORMA (WINDOWS / LINUX)
// ==========================================
class GestorAudio {
public:
    // Reproduce un archivo de audio formato WAV de forma asíncrona (con opción de bucle)
    static void ReproducirSonidoWAV(const std::string& rutaArchivo, bool enBucle = false) {
#if defined(_WIN32) || defined(_WIN64)
        DWORD opciones = SND_FILENAME | SND_ASYNC;
        if (enBucle) opciones |= SND_LOOP;
        PlaySoundA(rutaArchivo.c_str(), NULL, opciones);
#elif defined(__linux__)
        // En Linux utiliza aplay de manera no bloqueante ejecutándose en segundo plano
        std::string comando = "aplay -q " + rutaArchivo + (enBucle ? " &" : " &"); // Nota: Para bucles reales en Linux se requeriría lógica adicional, por defecto se lanza asíncrono
        system(comando.c_str());
#else
        (void)rutaArchivo;
        (void)enBucle;
#endif
    }

    // Detiene inmediatamente cualquier sonido o pista de audio en reproducción
    static void DetenerTodoElAudio() {
#if defined(_WIN32) || defined(_WIN64)
        PlaySoundA(NULL, NULL, 0);
#elif defined(__linux__)
        system("killall aplay 2>/dev/null");
#endif
    }
};
