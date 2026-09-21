#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>
#include "GestorEntidades.hpp"
#include "Entidad.hpp"
#include "Componentes.hpp"

class SerializadorEscena {
public:
    static bool GuardarEscena(GestorEntidades& gestor, const std::string& rutaArchivo) {
        std::ofstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "[SerializadorEscena] Error al abrir archivo para guardar: " << rutaArchivo << "\n";
            return false;
        }

        const auto& entidades = gestor.ObtenerTodasLasEntidades();
        for (const Entidad& e : entidades) {
            uint32_t idEntidad = e.ObtenerID();
            archivo << "ENTIDAD " << idEntidad << "\n";

            ComponenteNombre* nombre = gestor.ObtenerNombre(e);
            if (nombre) {
                archivo << "NOMBRE " << nombre->Nombre << "\n";
            }

            ComponenteTransformacion* t = gestor.ObtenerTransformacion(e);
            if (t) {
                archivo << "TRANSFORM " 
                        << t->Posicion.X << " " << t->Posicion.Y << " " << t->Posicion.Z << " "
                        << t->Escala.X << " " << t->Escala.Y << " " << t->Escala.Z << "\n";
            }

            archivo << "FIN_ENTIDAD\n";
        }

        archivo.close();
        std::cout << "[SerializadorEscena] Escena guardada correctamente en: " << rutaArchivo << "\n";
        return true;
    }

    static bool CargarEscena(GestorEntidades& gestor, const std::string& rutaArchivo) {
        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "[SerializadorEscena] Advertencia: No se encontró el archivo de escena: " << rutaArchivo << ". Se iniciará con una escena limpia.\n";
            return false;
        }

        std::string linea;
        Entidad entidadActual(0); 
        std::string nombreLeido = "Entidad";
        Vector3 posLeida(0.0f, 0.0f, 0.0f);
        Vector3 escalaLeida(1.0f, 1.0f, 1.0f);
        bool tieneTransform = false;

        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;

            std::stringstream ss(linea);
            std::string tipo;
            ss >> tipo;

            if (tipo == "ENTIDAD") {
                uint32_t id;
                ss >> id;
                // Creamos la entidad base con un nombre temporal por defecto
                entidadActual = gestor.CrearEntidad("EntidadTemporal");
                nombreLeido = "Entidad_" + std::to_string(id);
                tieneTransform = false;
            } 
            else if (tipo == "NOMBRE") {
                // Capturamos el resto de la línea por si el nombre contiene espacios o identificadores largos
                std::getline(ss, nombreLeido);
                // Quitamos el espacio inicial si lo hubiera después de "NOMBRE "
                if (!nombreLeido.empty() && nombreLeido[0] == ' ') {
                    nombreLeido = nombreLeido.substr(1);
                }

                if (entidadActual.ObtenerID() != 0) {
                    ComponenteNombre* compNombre = gestor.ObtenerNombre(entidadActual);
                    if (compNombre) {
                        compNombre->Nombre = nombreLeido;
                    }
                }
            } 
            else if (tipo == "TRANSFORM") {
                ss >> posLeida.X >> posLeida.Y >> posLeida.Z 
                   >> escalaLeida.X >> escalaLeida.Y >> escalaLeida.Z;
                tieneTransform = true;
            } 
            else if (tipo == "FIN_ENTIDAD") {
                if (entidadActual.ObtenerID() != 0 && tieneTransform) {
                    // Asignamos la transformación leída a la entidad recién creada
                    gestor.AsignarTransformacion(entidadActual, ComponenteTransformacion(posLeida));
                    
                    // Si el componente ya existe de forma interna, aseguramos escala también si aplica
                    ComponenteTransformacion* compTrans = gestor.ObtenerTransformacion(entidadActual);
                    if (compTrans) {
                        compTrans->Posicion = posLeida;
                        compTrans->Escala = escalaLeida;
                    }
                }
                entidadActual = Entidad(0); 
            }
        }

        archivo.close();
        std::cout << "[SerializadorEscena] Escena cargada correctamente desde: " << rutaArchivo << "\n";
        return true;
    }
};
