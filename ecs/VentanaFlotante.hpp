#pragma once
#include <string>

struct VentanaFlotante {
    std::string Titulo;
    float X, Y;          // Posición actual en la pantalla del escritorio (ej. 1280x720)
    float Ancho, Alto;   // Dimensiones del panel
    bool EstaArrastrando;
    float OffsetX, OffsetY; // Desfase para evitar que la ventana "salte" al hacer clic

    VentanaFlotante(const std::string& titulo, float x, float y, float ancho, float alto)
        : Titulo(titulo), X(x), Y(y), Ancho(ancho), Alto(alto), 
          EstaArrastrando(false), OffsetX(0.0f), OffsetY(0.0f) {}

    // Evalúa si el mouse hizo clic en la barra de título superior (altura fija de 25 píxeles)
    bool ComprobarInicioArrastre(float mouseX, float mouseY, bool botonPresionado) {
        // La barra de título ocupa el borde superior de la ventana
        if (botonPresionado && 
            mouseX >= X && mouseX <= (X + Ancho) && 
            mouseY >= Y && mouseY <= (Y + 25.0f)) {
            EstaArrastrando = true;
            OffsetX = mouseX - X;
            OffsetY = mouseY - Y;
            return true;
        }
        return false;
    }

    // Actualiza la posición mientras el usuario mantenga presionado el botón
    void ActualizarArrastre(float mouseX, float mouseY, bool botonPresionado) {
        if (EstaArrastrando) {
            if (!botonPresionado) {
                EstaArrastrando = false; // Soltó el botón del ratón
            } else {
                X = mouseX - OffsetX;
                Y = mouseY - OffsetY;
            }
        }
    }
};
