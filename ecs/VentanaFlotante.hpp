#pragma once
#include <string>
#include <algorithm>

// ==========================================
// GESTIÓN DE VENTANAS FLOTANTES Y PANELES DEL EDITOR
// ==========================================
struct VentanaFlotante {
    std::string Titulo;
    float X, Y;             // Posición actual en la pantalla del escritorio (ej. resolución 1280x720)
    float Ancho, Alto;      // Dimensiones lógicas del panel
    bool EstaArrastrando;
    float OffsetX, OffsetY; // Desfase calculado para evitar saltos al iniciar el arrastre

    VentanaFlotante(const std::string& titulo, float x, float y, float ancho, float alto)
        : Titulo(titulo), X(x), Y(y), Ancho(ancho), Alto(alto), 
          EstaArrastrando(false), OffsetX(0.0f), OffsetY(0.0f) {}

    // Evalúa si el ratón hizo clic dentro de la barra de título superior (altura fija de 25 píxeles)
    bool ComprobarInicioArrastre(float mouseX, float mouseY, bool botonPresionado) {
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

    // Actualiza la posición de la ventana en tiempo real mientras el botón permanezca presionado
    void ActualizarArrastre(float mouseX, float mouseY, bool botonPresionado) {
        if (EstaArrastrando) {
            if (!botonPresionado) {
                EstaArrastrando = false; // El usuario soltó el botón del ratón
            } else {
                X = mouseX - OffsetX;
                Y = mouseY - OffsetY;
            }
        }
    }

    // Método opcional de utilidad para redimensionar o ajustar límites si fuera necesario
    void EstablecerDimensiones(float ancho, float alto) {
        Ancho = std::max(100.0f, ancho);
        Alto = std::max(50.0f, alto);
    }
};
