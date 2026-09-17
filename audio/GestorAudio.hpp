#pragma once
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

class GestorAudio {
public:
    static void ReproducirSonidoWAV(const std::string& rutaArchivo, bool enBucle = false) {
#if defined(_WIN32) || defined(_WIN64)
        DWORD opciones = SND_FILENAME | SND_ASYNC;
        if (enBucle) opciones |= SND_LOOP;
        PlaySoundA(rutaArchivo.c_str(), NULL, opciones);
#elif defined(__linux__)
        std::string comando = "aplay -q " + rutaArchivo + " &";
        system(comando.c_str());
#endif
    }

    static void DetenerTodoElAudio() {
#if defined(_WIN32) || defined(_WIN64)
        PlaySoundA(NULL, NULL, 0);
#elif defined(__linux__)
        system("killall aplay 2>/dev/null");
#endif
    }
};
