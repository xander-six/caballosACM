
//    _____  .__                                    .___            
//   /  _  \ |  |   ____ ___  ________    ____    __| _/___________ 
//  /  /_\  \|  | _/ __ \\  \/  /\__  \  /    \  / __ |/ __ \_  __ \
// /    |    \  |_\  ___/ >    <  / __ \|   |  \/ /_/ \  ___/|  | \/
// \____|__  /____/\___  >__/\_ \(____  /___|  /\____ |\___  >__|   
//         \/          \/      \/     \/     \/      \/    \/       
//        ________                                                  
//        \______ \   ____   ____   ____  __________    https://github.com/xander-six/caballosACM.git            
//         |    |  \ /  _ \ /    \ /  _ \/  ___/  _ \   Universidad de Talca            
//         |    `   (  <_> )   |  (  <_> )___ (  <_> )  18-07-2023            
//        /_______  /\____/|___|  /\____/____  >____/   Gestor de cabaallos            
//                \/            \/           \/                     

#include "Caballo.hpp"
#include "json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


//*********************************************************************************
void guardarCaballos(const std::vector<Caballo>& caballos) {
    nlohmann::json j;

    for (const auto& caballo : caballos) {
        nlohmann::json caballoJson;
        caballoJson["nombre"] = caballo.nombre;
        caballoJson["tipo_caballo"] = caballo.tipo_caballo;
        caballoJson["padre"] = caballo.padre != nullptr ? caballo.padre->rie : -1;  // Almacenar el rie del padre
        caballoJson["madre"] = caballo.madre != nullptr ? caballo.madre->rie : -1;  // Almacenar el rie de la madre
        caballoJson["rie"] = caballo.rie;
        caballoJson["haras"] = caballo.haras;
        caballoJson["sexo"] = caballo.sexo;
        caballoJson["vivo"] = caballo.vivo;

        // Serializar los hijos recursivamente
        nlohmann::json hijosJson;
        for (const auto* hijo : caballo.hijos) {
            if (hijo != nullptr) {
                hijosJson.push_back(hijo->nombre);
            }
        }
        caballoJson["hijos"] = hijosJson;

        j.push_back(caballoJson);
    }

    std::ofstream file("caballos.json");
    file << j.dump(4);  // Guarda el archivo con formato indentado y legible
    file.close();

    std::cout << "El vector de caballos se ha guardado en el archivo 'caballos.json'." << std::endl;
}

std::vector<Caballo> cargarCaballos() {
    std::vector<Caballo> caballos;

    std::ifstream file("caballos.json");
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo 'caballos.json'. El vector de caballos estará vacío." << std::endl;
        return caballos;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    for (const auto& caballoJson : j) {
        Caballo caballo;
        caballo.nombre = caballoJson["nombre"];
        caballo.tipo_caballo = caballoJson["tipo_caballo"];
        caballo.rie = caballoJson["rie"];
        caballo.haras = caballoJson["haras"];
        caballo.sexo = caballoJson["sexo"];
        caballo.vivo = caballoJson["vivo"];

        // Inicializar los atributos de padre, madre y hijos como nulos o vacíos
        caballo.padre = nullptr;
        caballo.madre = nullptr;
        caballo.hijos.clear();

        caballos.push_back(caballo);
    }

    std::cout << "El vector de caballos se ha cargado desde el archivo 'caballos.json'." << std::endl;

    return caballos;
}
//*********************************************************************************




//____________________________________________________________________________________________________

                            //AGREGAR EJEMPLAR

Caballo agregar_ejemplar(){
    Caballo caballo;
    std::cout << "Ingrese el nombre del caballo: ";
    std::getline(std::cin, caballo.nombre);

    std::cout <<"Igrese el RIE del caballo (solo numeros): ";
    std::cin>>caballo.rie; 
    std::cin.ignore();

    std::cout << "Ingrese el tipo de caballo: ";
    std::getline(std::cin, caballo.tipo_caballo);

    std::cout << "Ingrese el nombre del haras: ";
    std::getline(std::cin, caballo.haras);

    std::cout << "Ingrese el sexo (M/F): ";
    std::getline(std::cin, caballo.sexo);

    caballo.padre = nullptr;
    caballo.madre = nullptr;
    return caballo;
}


//_______________________________________________________________________________________________________________________

                            //REGISTRAR MUERTE

std::vector<Caballo> registar_muerte(std::vector<Caballo> original_caballos){
    std::vector<Caballo> caballos = original_caballos;
    // Mostrar la lista de caballos con sus posiciones
    std::cout << "Caballos registrados:" << std::endl;
    for (int i = 0; i < caballos.size(); ++i) {
        std::cout << i << ". " << caballos[i].nombre << std::endl;
    }

    // Solicitar al usuario que seleccione un caballo por su posición
    int seleccion;
    std::cout << "Ingrese la posicion del caballo a marcar como muerto: ";
    std::cin >> seleccion;
    std::cin.ignore();

    if (seleccion >= 0 && seleccion < caballos.size()) {
        caballos[seleccion].vivo = false;
        std::cout << "El caballo en la posicion " << seleccion << " ha sido marcado como muerto." << std::endl;
    } else {
        std::cout << u8"La posición ingresada esta fuera de rango." << std::endl;
    }
    std::cout<<"OPERACION FINALIZADA\n"<<std::endl;
      
    return caballos;
}


                            //GENEALOGIA ASCENDENTE
void obtenerAncestros(Caballo* individuo) {
    if (individuo == nullptr) {
        return; // Caso base: el individuo no tiene ancestros
    }

    std::cout << "Nombre del individuo: " << individuo->nombre << std::endl;

    // Recursivamente obtener los ancestros del padre y de la madre
    obtenerAncestros(individuo->padre);
    obtenerAncestros(individuo->madre);
}
//_______________________________________________________________________________________________________________________

                            //ASIGNAR PADRE O MADRE

bool compararAscendenciaGenetica(Caballo* caballo1, Caballo* caballo2, int fin, int niv_recursion) {
    if (caballo1 == nullptr || caballo2 == nullptr || niv_recursion == fin) {
        return false;
    }

    bool coincidenciaGeneracionActual = false;
    
    // Compara el padre y la madre de la generación actual
    if (caballo1->padre == caballo2->padre && caballo1->madre == caballo2->madre) {
        coincidenciaGeneracionActual = true;
    }
    
    // Compara las generaciones anteriores de forma recursiva
    bool coincidenciaGeneracionAnterior = compararAscendenciaGenetica(caballo1->padre, caballo2->padre, fin, niv_recursion+1) ||
                                          compararAscendenciaGenetica(caballo1->madre, caballo2->madre, fin, niv_recursion+1);
    
    return coincidenciaGeneracionActual || coincidenciaGeneracionAnterior;
}


std::vector<Caballo> asignar_padres(std::vector<Caballo> original_caballos) {
    std::vector<Caballo> caballos = original_caballos;

    // Mostrar la lista de caballos con sus posiciones
    std::cout << "Caballos registrados:" << std::endl;
    for (int i = 0; i < caballos.size(); ++i) {
        std::cout << i << ". " << caballos[i].nombre << " (Sexo: " << caballos[i].sexo << ")" << std::endl;
    }

    int posicionCaballo;
    std::cout << "Ingrese la posición del caballo a asignarle padre o madre: ";
    std::cin >> posicionCaballo;
    std::cin.ignore();

    if (posicionCaballo >= 0 && posicionCaballo < caballos.size()) {
        std::cout << u8"Usted seleccionó al caballo " << caballos[posicionCaballo].nombre << std::endl;
        std::cout << "1. Asignar madre\n2. Asignar padre\nRESPUESTA: ";
        int opcion;
        std::cin >> opcion;
        while (opcion != 1 && opcion != 2) {
            std::cout << u8"Opción invalida. Por favor, seleccione nuevamente: ";
            std::cin >> opcion;
        }

        // MADRE
        if (opcion == 1) {
            std::cout << "La madre ya se encuentra registrada? (s/n): ";
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore();

            if (respuesta == "s") {
                // Muestra todas las yeguas registradas
                std::vector<Caballo> lista_hembras;
                int posicionMadre;
                std::cout << "Lista de yeguas:" << std::endl;
                int k = 0;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "F") {
                        std::cout << k << ". " << caballos[i].nombre << " RIE: " << caballos[i].rie << std::endl;
                        lista_hembras.push_back(caballos[i]);
                        k++;
                    }
                }

                std::cout << u8"Indique el numero de la madre: ";
                std::cin >> posicionMadre;
                std::cin.ignore();

                if (posicionMadre >= 0 && posicionMadre < lista_hembras.size()) {
                    // Comparar ascendencia genealógica
                    Caballo madre = caballos[posicionMadre];
                    Caballo* padre = caballos[posicionCaballo].padre;

                    bool existeCoincidencia = compararAscendenciaGenetica(padre, &madre, 3, 0);

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealogica de la madre coincide con la del padre." << std::endl;
                    } else {
                        std::cout << u8"Éxito: La ascendencia genealogica de la madre no coincide con la del padre." << std::endl;
                        madre.hijos.push_back(&caballos[posicionCaballo]);
                        caballos[posicionCaballo].madre = &madre;
                        std::cout << u8"Asignación de madre exitosa." << std::endl;
                    }
                } else {
                    std::cout << u8"Numero de madre invalido." << std::endl;
                }
            } else {
                std::cout << u8"La madre se registrara como un nuevo caballo." << std::endl;
                Caballo madre = agregar_ejemplar();
                madre.hijos.push_back(&caballos[posicionCaballo]);
                caballos.push_back(madre);
                caballos[posicionCaballo].madre = &caballos[caballos.size() - 1];
                std::cout << "MADRE REGISTRADA\n" << std::endl;
            }
        } else if (opcion == 2) {
            std::cout << u8"El padre ya se encuentra registrado? (s/n): ";
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore();

            if (respuesta == "s") {
                // Muestra todos los caballos registrados que sean machos
                int posicionPadre;
                std::vector<Caballo> lista_machos;
                int k = 0;
                std::cout << u8"Lista de sementales:" << std::endl;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "M") {
                        std::cout << k << ". " << caballos[i].nombre << " RIE: " << caballos[i].rie << std::endl;
                        lista_machos.push_back(caballos[i]);
                        k++;
                    }
                }

                std::cout << u8"Indique el numero del padre: ";
                std::cin >> posicionPadre;
                std::cin.ignore();

                if (posicionPadre >= 0 && posicionPadre < lista_machos.size()) {
                    // Comparar ascendencia genealógica
                    Caballo padre = caballos[posicionPadre];
                    Caballo* madre = caballos[posicionCaballo].madre;

                    bool existeCoincidencia = compararAscendenciaGenetica(&padre, madre, 3, 0);

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealogica del padre coincide con la de la madre." << std::endl;
                    } else {
                        std::cout << u8"Exito: La ascendencia genealogica del padre no coincide con la de la madre." << std::endl;
                        padre.hijos.push_back(&caballos[posicionCaballo]);
                        caballos[posicionCaballo].padre = &padre;
                        std::cout << u8"Asignacion de padre exitosa." << std::endl;
                    }
                } else {
                    std::cout << u8"Numero de padre invalido." << std::endl;
                }
            } else {
                std::cout << u8"El padre se registrara como un nuevo caballo." << std::endl;
                Caballo padre = agregar_ejemplar();
                padre.hijos.push_back(&caballos[posicionCaballo]);
                caballos.push_back(padre);
                caballos[posicionCaballo].padre = &caballos[caballos.size() - 1];
                std::cout << "PADRE REGISTRADO\n" << std::endl;
            }
        }
    } else {
        std::cout << u8"La posicion ingresada esta fuera de rango.\n" << std::endl;
    }

    return caballos;
}
//_____________________________________________________________________________________________________________________


//_________________________________________________________________________________________________________________

                            //GENEALOGIA DESCENDENTE

void mostrarDescendientes(Caballo* caballo, int i = 0) {
    if (caballo != nullptr) {
        if(i == 0)
            std::cout << "Descendientes de " << caballo->nombre << ":" << std::endl;
        
        if (caballo->hijos.empty()) {
            std::cout << "No tiene descendientes registrados." << std::endl;
            return;
        }

        for (const auto& descendiente : caballo->hijos) {
            std::cout << descendiente->nombre << std::endl;
            mostrarDescendientes(descendiente, i+1);
        }
    }
}
//________________________________________________________________________________________________________________________


void mostrarDescendientesSexo(Caballo* caballo, std::string sexo) {
    if (caballo != nullptr) {
        std::cout << "Descendientes de " << caballo->nombre << ":" << std::endl;
        
        if (caballo->hijos.empty()) {
            std::cout << "No tiene descendientes registrados." << std::endl;
            return;
        }

        for (const auto& descendiente : caballo->hijos) {
            if(descendiente->sexo==sexo)
                std::cout << descendiente->nombre << std::endl;
            mostrarDescendientes(descendiente);
        }
    }
}



void determinarCruce(std::vector<Caballo> caballos){
    std::vector<Caballo>lista_machos;
    std::vector<Caballo>lista_hembras;
    std::cout<<"Lista de yeguas:"<<std::endl;
    int k = 0;
    int yegua, macho;
    for (int i = 0; i < caballos.size(); ++i) {
        if (caballos[i].sexo == "F") {
            std::cout << k << ". " << caballos[i].nombre << " RIE: " << caballos[i].rie << std::endl;
            lista_hembras.push_back(caballos[i]);
            k++;
        }
    }
    std::cout<<"Indique el numero de la yegua: ";
    std::cin>>yegua;
    std::cin.ignore();

    std::cout<<"Lista de machos:"<<std::endl;
    k = 0;
    for (int i = 0; i < caballos.size(); ++i) {
        if (caballos[i].sexo == "M") {
            std::cout << k << ". " << caballos[i].nombre << " RIE: " << caballos[i].rie << std::endl;
            lista_machos.push_back(caballos[i]);
            k++;
        }
    }
    std::cout<<"Indique el numero del macho: ";
    std::cin>>macho;
    std::cin.ignore();
    if(compararAscendenciaGenetica(&lista_machos[macho], &lista_hembras[yegua], 3,0)){
        std::cout<<"NO ES POSIBLE APAREAR ESTOS DOS CABALLOS"<<std::endl;
        return;
    }
    std::cout<<"ES POSIBLE APAREAR ESTOS DOS CABALLOS"<<std::endl;
}


void mismoNombre(std::vector<Caballo> caballos) {
    std::string nombre;
    std::cout << "INGRESE EL NOMBRE A BUSCAR: ";
    std::getline(std::cin, nombre);

    // Convertir el nombre ingresado a minúsculas (opcional, para búsqueda insensible a mayúsculas/minúsculas)
    std::transform(nombre.begin(), nombre.end(), nombre.begin(), [](unsigned char c) { return std::tolower(c); });

    bool encontrado = false;
    for (const Caballo& ejemplar : caballos) {
        // Convertir el nombre del caballo actual a minúsculas (opcional, para búsqueda insensible a mayúsculas/minúsculas)
        std::string nombreCaballo = ejemplar.nombre;
        std::transform(nombreCaballo.begin(), nombreCaballo.end(), nombreCaballo.begin(), [](unsigned char c) { return std::tolower(c); });

        if (nombreCaballo == nombre) {
            std::cout << "Caballo: " << ejemplar.nombre << " Haras: " << ejemplar.haras << " RIE: " << ejemplar.rie << std::endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        std::cout << "No se encontro ningún caballo con el nombre " << nombre  << std::endl;
    }
}
void mismoHaras(std::vector<Caballo> caballos) {
    std::string nombre;
    std::cout << "INGRESE EL NOMBRE DEL HARAS A BUSCAR: ";
    std::getline(std::cin, nombre);

    // Convertir el nombre ingresado a minúsculas (opcional, para búsqueda insensible a mayúsculas/minúsculas)
    std::transform(nombre.begin(), nombre.end(), nombre.begin(), [](unsigned char c) { return std::tolower(c); });

    bool encontrado = false;
    for (const Caballo& ejemplar : caballos) {
        // Convertir el nombre del caballo actual a minúsculas (opcional, para búsqueda insensible a mayúsculas/minúsculas)
        std::string nombreCaballo = ejemplar.haras;
        std::transform(nombreCaballo.begin(), nombreCaballo.end(), nombreCaballo.begin(), [](unsigned char c) { return std::tolower(c); });

        if (nombreCaballo == nombre) {
            std::cout << "Caballo: " << ejemplar.nombre << " Haras: " << ejemplar.haras << " RIE: " << ejemplar.rie << std::endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        std::cout << "No se encontro ningún caballo con el nombre " << nombre  << std::endl;
    }
}

void menu() {
    std::vector<Caballo> caballos = cargarCaballos();
    bool primera_iteracion = true;
    int opcion;
    int posicionCaballo;

    do {
        std::cout << "------- MENU -------" << std::endl;
        std::cout << "1. Registrar nuevo ejemplar" << std::endl;
        std::cout << "2. Registrar la muerte de ejemplares" << std::endl;
        std::cout << "3. Asignar padre o madre" << std::endl;
        std::cout << "4. Determinar genealogia ascendente de un individuo especifico" << std::endl;
        std::cout << "5. Determinar genealogia descendente de un individuo especifico" << std::endl;
        std::cout << "6. Determinar genealogia descendente de un sexo especifico para un individuo dado" << std::endl;
        std::cout << "7. Determinar si se pueden cruzar un par potro/yegua" << std::endl;
        std::cout << "8. Mostrar todos los ejemplares que tengan el mismo nombre" << std::endl;
        std::cout << "9. Mostrar todos los ejemplares provenientes de un mismo haras"<<std::endl;
        std::cout << "10. Eliminar todos los caballos"<<std::endl;
        std::cout << "11. Salir" << std::endl;
        std::cout << "Ingrese una opcion: ";
        std::cin >> opcion;
        std::cin.ignore(); 
        if (!primera_iteracion || caballos.size() > 0) {
            switch (opcion) {
                case 1:
                    
                    caballos.push_back(agregar_ejemplar());
                    break;
                case 2:
                    caballos = registar_muerte(caballos);
                    break;
                case 3:
                    caballos = asignar_padres(caballos);
                    break;
                case 4:
                    std::cout << "Caballos registrados:" << std::endl;
                    for (int i = 0; i < caballos.size(); ++i) {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                    std::cout << "Ingrese la posición del caballo: ";
                    std::cin >> posicionCaballo;
                    std::cin.ignore();
                    obtenerAncestros(&caballos[posicionCaballo]);
                    break;
                case 5:
                    std::cout << "Caballos registrados:" << std::endl;
                    for (int i = 0; i < caballos.size(); ++i) {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                    std::cout << "Ingrese la posición del caballo: ";
                    std::cin >> posicionCaballo;
                    std::cin.ignore();
                    mostrarDescendientes(&caballos[posicionCaballo]);
                    break;
                case 6:
                    std::cout << "Caballos registrados:" << std::endl;
                    for (int i = 0; i < caballos.size(); ++i) {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                    std::cout << "Ingrese la posición del caballo: ";
                    std::cin >> posicionCaballo;
                    std::cin.ignore();

                    int n_opcion;
                    std::cout<<"1.Masculino o 2.Femenino: ";
                    std::cin >> n_opcion;
                    std::cin.ignore();
                    if(n_opcion == 1)
                        mostrarDescendientesSexo(&caballos[posicionCaballo],"M");
                    else {
                        mostrarDescendientesSexo(&caballos[posicionCaballo],"F");
                    }
                    break;

                case 7:
                    determinarCruce(caballos);
                    break;

                case 8:
                    mismoNombre(caballos);
                    break;
                case 9:
                    mismoHaras(caballos);
                    break;
                case 10:
                    caballos.clear();
                    break;

                case 11:
                    std::cout << "Saliendo del menu..." << std::endl;
                    guardarCaballos(caballos);
                    break;

                default:
                    std::cout << "Opcion invalida. Por favor, ingrese una opcion valida." << std::endl;
                    break;
            }
        } else {
            if (opcion == 1) {
                primera_iteracion = false;
                caballos.push_back(agregar_ejemplar());
            } else {
                std::cout << "Primero debe registrar un caballo" << std::endl;
            }
        }
    } while (opcion != 9);
}

int main() {
    menu();
    return 0;
}
