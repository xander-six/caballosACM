#include "Caballo.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>



//**********************************************************************************
//VARIABLES GLOBALES
std::vector<std::string> genealogia = {
"Tú (persona central)",
"Padre/Madre",
"Abuelo/Abuela",
"Bisabuelo/Bisabuela",
"Tatarabuelo/Tatarabuela",
"Trastatarabuelo/Trastatarabuela",
"Quintarabuelo/Quintarabuela",
"Sextarabuelo/Sextarabuela",
"Septimarabuelo/Septimarabuela",
"Octarabuelo/Octarabuela"
};

std::vector<std::string> descendencia = {
"Hijo / Hija",
"Nieto / Nieta",
"Bisnieto / Bisnieta",
"Tataranieto / Tataranieta",
"Trastataranieto / Trastataranieta",
"Quintataranieto / Quintataranieta",
"Sextataranieto / Sextataranieta",
"Septimataranieto / Septimataranieta",
"Octotaranieto / Octotaranieta",
"Decimotataranieto / Decimotataranieta"
};


// Función para guardar el vector de caballos en un archivo
void guardarCaballos(const std::vector<Caballo>& caballos, const std::string& archivo) {
    std::ofstream file(archivo, std::ios::binary);
    if (file.is_open()) {
        // Obtener el tamaño del vector de caballos
        size_t numCaballos = caballos.size();

        // Escribir el tamaño en el archivo
        file.write(reinterpret_cast<const char*>(&numCaballos), sizeof(numCaballos));

        // Escribir cada caballo en el archivo
        for (const Caballo& caballo : caballos) {
            // Escribir cada miembro del caballo en el archivo
            file.write(reinterpret_cast<const char*>(&caballo), sizeof(caballo));
        }

        std::cout << "Caballos guardados correctamente en el archivo " << archivo << std::endl;
    } else {
        std::cout << "Error al abrir el archivo " << archivo << " para guardar los caballos" << std::endl;
    }
}


// Función para cargar el vector de caballos desde un archivo
std::vector<Caballo> cargarCaballos(const std::string& archivo) {
    std::vector<Caballo> caballos;
    std::ifstream file(archivo, std::ios::binary);
    if (file.is_open()) {
        // Leer el tamaño del vector de caballos del archivo
        size_t numCaballos;
        file.read(reinterpret_cast<char*>(&numCaballos), sizeof(numCaballos));

        // Leer cada caballo del archivo
        for (size_t i = 0; i < numCaballos; ++i) {
            Caballo caballo;

            // Leer cada miembro del caballo del archivo
            file.read(reinterpret_cast<char*>(&caballo), sizeof(caballo));

            // Agregar el caballo al vector
            caballos.push_back(caballo);
        }

        std::cout << "Caballos cargados correctamente desde el archivo " << archivo << std::endl;
    } else {
        std::cout << "Error al abrir el archivo " << archivo << " para cargar los caballos" << std::endl;
    }
    return caballos;
}


//*********************************************************************************

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





std::vector<Caballo> asingar_padres(std::vector<Caballo> original_caballos){
    std::vector<Caballo> caballos = original_caballos;
    
    // Mostrar la lista de caballos con sus posiciones
    std::cout << "Caballos registrados:" << std::endl;
    for (int i = 0; i < caballos.size(); ++i) {
        std::cout << i << ". " << caballos[i].nombre << std::endl;
    }

    
    int posicionCaballo;
    std::cout << "Ingrese la posiciOn del caballo a asignarle padre o madre: ";
    std::cin >> posicionCaballo;
    std::cin.ignore();

    if (posicionCaballo >= 0 && posicionCaballo < caballos.size()) {
        std::cout << u8"Usted seleccionó al caballo " << caballos[posicionCaballo].nombre << std::endl;
        std::cout << "1. Asignar madre\n2. Asignar padre: ";
        int opcion;
        std::cin >> opcion;
        while (opcion != 1 && opcion != 2) {
            std::cout << u8"Opción inválida. Por favor, seleccione nuevamente: ";
            std::cin >> opcion;
        }

        if (opcion == 1) {
            std::cout << "La madre ya se encuentra registrada? (s/n): ";
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore();

            if (respuesta == "s") {
                //Muestra todas las yeguas registradas
                int posicionMadre;
                std::cout << "Lista de yeguas:" << std::endl;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "F") {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                }

                std::cout << u8"Indique el número de la madre: ";
                std::cin >> posicionMadre;
                std::cin.ignore();

                if (posicionMadre >= 0 && posicionMadre < caballos.size()) {
                    // Comparar ascendencia genealógica
                    Caballo madre = caballos[posicionMadre];
                    Caballo* padre = caballos[posicionCaballo].padre;

                    bool existeCoincidencia = false;
               
                    if (padre != nullptr) {     //Si el padre está asignado
                        Caballo* abueloPaterno = padre->padre;
                        Caballo* abuelaPaterna = padre->madre;

                        //Compara la ascendencia genética
                        if (abueloPaterno != nullptr) {
                            Caballo* bisabueloPaterno = abueloPaterno->padre;
                            Caballo* bisabuelaPaterna = abueloPaterno->madre;

                            if (bisabueloPaterno != nullptr && bisabueloPaterno == madre.padre) {
                                existeCoincidencia = true;
                            }
                            if (bisabuelaPaterna != nullptr && bisabuelaPaterna == madre.madre) {
                                existeCoincidencia = true;
                            }
                        }

                        if (abuelaPaterna != nullptr) {
                            Caballo* bisabueloMaterno = abuelaPaterna->padre;
                            Caballo* bisabuelaMaterna = abuelaPaterna->madre;

                            if (bisabueloMaterno != nullptr && bisabueloMaterno == madre.padre) {
                                existeCoincidencia = true;
                            }
                            if (bisabuelaMaterna != nullptr && bisabuelaMaterna == madre.madre) {
                                existeCoincidencia = true;
                            }
                        }
                    }

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealógica de la madre coincide con la del padre." << std::endl;
                    } else {
                        std::cout << u8"Éxito: La ascendencia genealógica de la madre no coincide con la del padre." << std::endl;
                        caballos[posicionCaballo].madre = &madre;
                    }
                } else {
                    std::cout << u8"Número de madre inválido." << std::endl;
                }
            } else {
                std::cout << u8"La madre se registrará como un nuevo caballo." << std::endl;
                caballos.push_back(agregar_ejemplar());
                caballos[posicionCaballo].madre = &caballos[caballos.size()-1];
                std::cout<<"MADRE REGISTRADA\n"<<std::endl;
            }
        } else if (opcion == 2) {
            std::cout <<u8"¿El padre ya se encuentra registrado? (s/n): ";
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore();

            if (respuesta == "s") {
                // Muestra todos los caballos registrados que sean machos
                int posicionPadre;
                std::cout << u8"Lista de sementales:" << std::endl;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "M") {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                }

                std::cout << u8"Indique el número del padre: ";
                std::cin >> posicionPadre;
                std::cin.ignore();

                if (posicionPadre >= 0 && posicionPadre < caballos.size()) {
                    // Comparar ascendencia genealógica
                    Caballo padre = caballos[posicionPadre];
                    Caballo* madre = caballos[posicionCaballo].madre;

                    bool existeCoincidencia = false;

                    if (madre != nullptr) {     // Si la madre está asignada
                        Caballo* abueloMaterno = madre->padre;
                        Caballo* abuelaMaterna = madre->madre;

                        // Compara la ascendencia genealógica
                        if (abueloMaterno != nullptr) {
                            Caballo* bisabueloMaterno = abueloMaterno->padre;
                            Caballo* bisabuelaMaterna = abueloMaterno->madre;

                            if (bisabueloMaterno != nullptr && bisabueloMaterno == padre.padre) {
                                existeCoincidencia = true;
                            }
                            if (bisabuelaMaterna != nullptr && bisabuelaMaterna == padre.madre) {
                                existeCoincidencia = true;
                            }
                        }

                        if (abuelaMaterna != nullptr) {
                            Caballo* bisabueloMaterno = abuelaMaterna->padre;
                            Caballo* bisabuelaMaterna = abuelaMaterna->madre;

                            if (bisabueloMaterno != nullptr && bisabueloMaterno == padre.padre) {
                                existeCoincidencia = true;
                            }
                            if (bisabuelaMaterna != nullptr && bisabuelaMaterna == padre.madre) {
                                existeCoincidencia = true;
                            }
                        }
                    }

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealógica del padre coincide con la de la madre.\n" << std::endl;
                    } else {
                        std::cout << u8"Éxito: La ascendencia genealógica del padre no coincide con la de la madre.\n" << std::endl;
                        caballos[posicionCaballo].padre = &padre;
                    }
                } else {
                    std::cout << u8"Número de padre inválido." << std::endl;
                }
            } else {
                std::cout << u8"El padre se registrará como un nuevo caballo." << std::endl;
                caballos.push_back(agregar_ejemplar());
                caballos[posicionCaballo].padre = &caballos[caballos.size() - 1];
                std::cout<<"PADRE REGISTRADO\n"<<std::endl;
            }

        }
    
    } else {
        std::cout << u8"La posición ingresada está fuera de rango.\n" << std::endl;
    }
    return caballos;



}

std::vector<Caballo> resgistar_muerte(std::vector<Caballo> original_caballos){
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



void menu() {
    Caballo caballo;
    std::vector<Caballo> caballos = cargarCaballos("caballos.dat");
    bool primera_iteracion = true;
    int opcion;


    do {
        std::cout << "------- MENU -------" << std::endl;
        std::cout << "1. Registrar nuevo ejemplar" << std::endl;
        std::cout << "2. Registrar la muerte de ejemplares" << std::endl;
        std::cout << "3. Asingar padre o madre" << std::endl;
        std::cout << "4. Determinar genealogia ascendente de un individuo especifico" << std::endl;
        std::cout << "5. Determinar genealogia descendente de un individuo especifico" << std::endl;
        std::cout << "6. Determinar genealogia descendente de un sexo especifico para un individuo dado" << std::endl;
        std::cout << "7. Determinar si se pueden cruzar un par potro/yegua" << std::endl;
        std::cout << "8. Mostrar todos los ejemplares que tengan el mismo nombre" << std::endl;
        std::cout << "9. Salir" << std::endl;
        std::cout << "Ingrese una opcion: ";
        std::cin >> opcion;
        std::cin.ignore(); 
        if (!primera_iteracion){
            switch (opcion) {
                case 1:
                    caballo = agregar_ejemplar();
                    caballos.push_back(caballo);
                    break;
                case 2:
                    caballos = resgistar_muerte(caballos);
                    break;
                case 3:
                    caballos = asingar_padres(caballos);
                    break;
                case 4:
                    // Lógica para determinar si se pueden cruzar un par potro/yegua
                    // Implementa la lógica aquí
                    break;
                case 5:
                    // Lógica para registrar nacimientos de nuevos potrillos
                    // Implementa la lógica aquí
                    break;
                case 6:
                    // Lógica para registrar la muerte de ejemplares
                    // Implementa la lógica aquí
                    break;
                case 7:
                    // Lógica para mostrar todos los ejemplares que tengan el mismo nombre
                    // Implementa la lógica aquí
                    break;
                case 8:
                    
                case 9:
                    std::cout << "Saliendo del menu..." << std::endl;
                    guardarCaballos(caballos,"caballos.dat");
                    break;

                default:
                    std::cout << "Opcion invalida. Por favor, ingrese una opcion valida." << std::endl;
                    break;
                
            }

        }else{
            if(opcion == 1){
                agregar_ejemplar();
                primera_iteracion=false;
                caballos.push_back(caballo);
            }else{
                std::cout<<"Primero debe registrar caballo"<<std::endl;
            }
        }
       
    } while (opcion != 9);
}

int main() {

    menu();


    return 0;
}
