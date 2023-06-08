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

    virtual bool filtrar(float calificacionMinima, const string& genero) const = 0;
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

    bool filtrar(float calificacionMinima, const string& generoFiltro) const override {
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

    bool filtrar(float calificacionMinima, const string& nombreSerieFiltro) const override {
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
                    if (getline(iss, titulo, ',') &&
                        getline(iss, duracionStr, ',') &&
                        getline(iss, calificacionStr, ',') &&
                        getline(iss, genero)) {
                        int duracion = stoi(duracionStr);
                        float calificacion = stof(calificacionStr);
                        videos.push_back(new Pelicula(titulo, duracion, calificacion, genero));
                    }
                }
                else if (tipo == "Episodio de Serie") {
                    string titulo;
                    string duracionStr;
                    string calificacionStr;
                    string nombreSerie;
                    if (getline(iss, titulo, ',') &&
                        getline(iss, duracionStr, ',') &&
                        getline(iss, calificacionStr, ',') &&
                        getline(iss, nombreSerie)) {
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

    void mostrarVideos(float calificacionMinima, const string& generoFiltro) const {
        cout << "Videos encontrados:" << endl;
        for (const Video* video : videos) {
            if (video->filtrar(calificacionMinima, generoFiltro)) {
                video->mostrar();
            }
        }
    }

    void mostrarEpisodiosSerie(float calificacionMinima, const string& nombreSerieFiltro) const {
        cout << "Episodios encontrados:" << endl;
        for (const Video* video : videos) {
            const EpisodioSerie* episodio = dynamic_cast<const EpisodioSerie*>(video);
            if (episodio && episodio->filtrar(calificacionMinima, nombreSerieFiltro)) {
                episodio->mostrar();
            }
        }
    }

    void calificarVideo(const string& titulo, float calificacion) {
        bool encontrado = false;
        for (Video* video : videos) {
            if (video->getTitulo() == titulo) {
                video->setCalificacion(calificacion);
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            cout << "No se encontró ningún video con el título: " << titulo << endl;
        }
    }
};

int main() {
    Videoteca videoteca;
    videoteca.cargarDesdeArchivo("datos.txt");

    int opcion;
    do {
        cout << "Ingrese una opción:\n";
        cout << "1. Cargar archivo de datos\n";
        cout << "2. Mostrar videos con cierta calificación y género\n";
        cout << "3. Mostrar episodios de una serie con cierta calificación\n";
        cout << "4. Mostrar películas con cierta calificación\n";
        cout << "5. Calificar un video\n";
        cout << "0. Salir\n";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            string nombreArchivo;
            cout << "Ingrese el nombre del archivo: ";
            cin >> nombreArchivo;
            videoteca.cargarDesdeArchivo(nombreArchivo);
            break;
        }
        case 2: {
            float calificacionMinima;
            cout << "Ingrese la calificación mínima: ";
            cin >> calificacionMinima;

            string generoFiltro;
            cout << "Ingrese el género a filtrar (deje vacío para mostrar todos): ";
            cin.ignore();
            getline(cin, generoFiltro);

            videoteca.mostrarVideos(calificacionMinima, generoFiltro);
            break;
        }
        case 3: {
            float calificacionMinima;
            cout << "Ingrese la calificación mínima: ";
            cin >> calificacionMinima;

            string nombreSerieFiltro;
            cout << "Ingrese el nombre de la serie a filtrar (deje vacío para mostrar todos): ";
            cin.ignore();
            getline(cin, nombreSerieFiltro);

            videoteca.mostrarEpisodiosSerie(calificacionMinima, nombreSerieFiltro);
            break;
        }
        case 4: {
            float calificacionMinima;
            cout << "Ingrese la calificación mínima: ";
            cin >> calificacionMinima;

            videoteca.mostrarVideos(calificacionMinima, "");
            break;
        }
        case 5: {
            string titulo;
            cout << "Ingrese el título del video a calificar: ";
            cin.ignore();
            getline(cin, titulo);

            float calificacion;
            cout << "Ingrese la nueva calificación: ";
            cin >> calificacion;

            videoteca.calificarVideo(titulo, calificacion);
            break;
        }
        case 0:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opción inválida. Intente de nuevo." << endl;
        }
        cout << endl;

    } while (opcion != 0);

    return 0;
}