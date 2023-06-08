#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Video {
protected:
    string titulo;
    int duracion;
    float calificacion;

public:
    Video(const string& titulo, int duracion, float calificacion)
        : titulo(titulo), duracion(duracion), calificacion(calificacion) {
    }

    virtual void mostrar() const = 0;

    const string& getTitulo() const {
        return titulo;
    }

    float getCalificacion() const {
        return calificacion;
    }

    void setCalificacion(float nuevaCalificacion) {
        calificacion = nuevaCalificacion;
    }

    virtual bool filtrarPorGenero(float calificacionMinima, const string& genero) const = 0;
    virtual bool filtrarPorSerie(float calificacionMinima, const string& nombreSerieFiltro) const {
        return calificacion >= calificacionMinima;
    }
};

class Pelicula : public Video {
private:
    string genero;

public:
    Pelicula(const string& titulo, int duracion, float calificacion, const string& genero)
        : Video(titulo, duracion, calificacion), genero(genero) {
    }

    void mostrar() const override {
        cout << "Pelicula - Título: " << titulo << ", Duración: " << duracion << " minutos, Calificación: " << calificacion << ", Género: " << genero << endl;
    }

    bool filtrarPorGenero(float calificacionMinima, const string& generoFiltro) const override {
        return calificacion >= calificacionMinima && (generoFiltro.empty() || genero == generoFiltro);
    }
};

class EpisodioSerie : public Video {
private:
    string nombreSerie;

public:
    EpisodioSerie(const string& titulo, int duracion, float calificacion, const string& nombreSerie)
        : Video(titulo, duracion, calificacion), nombreSerie(nombreSerie) {
    }

    void mostrar() const override {
        cout << "Episodio de Serie - Título: " << titulo << ", Duración: " << duracion << " minutos, Calificación: " << calificacion << ", Serie: " << nombreSerie << endl;
    }

    bool filtrarPorGenero(float calificacionMinima, const string& nombreSerieFiltro) const override {
        return calificacion >= calificacionMinima;
    }

    bool filtrarPorSerie(float calificacionMinima, const string& nombreSerieFiltro) const override {
        return calificacion >= calificacionMinima && (nombreSerieFiltro.empty() || nombreSerie == nombreSerieFiltro);
    }
};

class Videoteca {
private:
    vector<Video*> videos;

public:
    Videoteca() {
    }

    ~Videoteca() {
        for (Video* video : videos) {
            delete video;
        }
    }

    void cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);

        if (!archivo) {
            cout << "No se pudo abrir el archivo." << endl;
            return;
        }

        string linea;

        while (getline(archivo, linea)) {
            istringstream iss(linea);
            string tipo;
            if (getline(iss, tipo, ',')) {
                if (tipo == "Pelicula") {
                    string titulo;
                    string duracionStr;
                    string calificacionStr;
                    string genero;

                    if (getline(iss, titulo, ',') && getline(iss, duracionStr, ',') && getline(iss, calificacionStr, ',') && getline(iss, genero)) {
                        int duracion = stoi(duracionStr);
                        float calificacion = stof(calificacionStr);
                        videos.push_back(new Pelicula(titulo, duracion, calificacion, genero));
                    }
                } else if (tipo == "Episodio de Serie") {
                    string titulo;
                    string duracionStr;
                    string calificacionStr;
                    string nombreSerie;

                    if (getline(iss, titulo, ',') && getline(iss, duracionStr, ',') && getline(iss, calificacionStr, ',') && getline(iss, nombreSerie)) {
                        int duracion = stoi(duracionStr);
                        float calificacion = stof(calificacionStr);
                        videos.push_back(new EpisodioSerie(titulo, duracion, calificacion, nombreSerie));
                    }
                }
            }
        }

        archivo.close();

        cout << "Datos cargados correctamente." << endl;
    }

    void mostrarVideosPorGenero(float calificacionMinima, const string& generoFiltro) const {
        cout << "Videos encontrados:" << endl;
        for (const Video* video : videos) {
            if (video->filtrarPorGenero(calificacionMinima, generoFiltro)) {
                video->mostrar();
            }
        }
    }

    void mostrarVideosPorSerie(float calificacionMinima, const string& nombreSerieFiltro) const {
        cout << "Videos de serie encontrados:" << endl;
        for (const Video* video : videos) {
            if (video->filtrarPorSerie(calificacionMinima, nombreSerieFiltro)) {
                video->mostrar();
            }
        }
    }
};

int main() {
    Videoteca videoteca;

    while (true) {
        cout << "Ingrese una opción:" << endl;
        cout << "1. Cargar archivo de datos" << endl;
        cout << "2. Mostrar videos con cierta calificación y género" << endl;
        cout << "3. Mostrar episodios de una serie con cierta calificación" << endl;
        cout << "4. Mostrar películas con cierta calificación" << endl;
        cout << "5. Calificar un video" << endl;
        cout << "0. Salir" << endl;

        int opcion;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                cout << "Ingrese el nombre del archivo: ";
                string nombreArchivo;
                getline(cin, nombreArchivo);

                videoteca.cargarDesdeArchivo(nombreArchivo);
                break;
            }

            case 2: {
                cout << "Ingrese la calificación mínima: ";
                float calificacionMinima;
                cin >> calificacionMinima;
                cin.ignore();

                cout << "Ingrese el género a filtrar (deje vacío para mostrar todos): ";
                string generoFiltro;
                getline(cin, generoFiltro);

                videoteca.mostrarVideosPorGenero(calificacionMinima, generoFiltro);
                break;
            }

            case 3: {
                cout << "Ingrese la calificación mínima: ";
                float calificacionMinima;
                cin >> calificacionMinima;
                cin.ignore();

                cout << "Ingrese el nombre de la serie a filtrar (deje vacío para mostrar todos): ";
                string nombreSerieFiltro;
                getline(cin, nombreSerieFiltro);

                videoteca.mostrarVideosPorSerie(calificacionMinima, nombreSerieFiltro);
                break;
            }

            case 0:
                return 0;

            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                break;
        }
    }

    return 0;
}