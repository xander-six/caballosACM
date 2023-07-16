#include "Caballo.hpp"
#include "json.hpp"
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

        // Buscar los padres en función del rie almacenado en el archivo JSON
        int riePadre = caballoJson["padre"];
        if (riePadre != -1) {
            auto it = std::find_if(caballos.begin(), caballos.end(), [riePadre](const Caballo& c) {
                return c.rie == riePadre;
            });
            if (it != caballos.end()) {
                caballo.padre = &(*it);
            }
        }

        int rieMadre = caballoJson["madre"];
        if (rieMadre != -1) {
            auto it = std::find_if(caballos.begin(), caballos.end(), [rieMadre](const Caballo& c) {
                return c.rie == rieMadre;
            });
            if (it != caballos.end()) {
                caballo.madre = &(*it);
            }
        }

        // Cargar los hijos recursivamente
        const nlohmann::json& hijosJson = caballoJson["hijos"];
        for (const auto& nombreHijo : hijosJson) {
            // Buscar el hijo en el vector de caballos por su nombre
            for (auto& hijo : caballos) {
                if (hijo.nombre == nombreHijo) {
                    caballo.hijos.push_back(&hijo);
                    // Establecer el padre y la madre del hijo en función de la lógica de tu programa
                    break;
                }
            }
        }

        caballos.push_back(caballo);
    }

    std::cout << "El vector de caballos se ha cargado desde el archivo 'caballos.json'." << std::endl;

    return caballos;
}


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


std::vector<Caballo> asingar_padres(std::vector<Caballo> original_caballos) {
    std::vector<Caballo> caballos = original_caballos;

    // Mostrar la lista de caballos con sus posiciones
    std::cout << "Caballos registrados:" << std::endl;
    for (int i = 0; i < caballos.size(); ++i) {
        std::cout << i << ". " << caballos[i].nombre << std::endl;
    }

    int posicionCaballo;
    std::cout << "Ingrese la posición del caballo a asignarle padre o madre: ";
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

        //MADRE
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
                int k=0;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "F") {
                        std::cout << k << ". " << caballos[i].nombre << " RIE: "<< caballos[i].rie<<std::endl;
                        lista_hembras.push_back(caballos[i]);
                        k++;
                    }
                }

                std::cout << u8"Indique el número de la madre: ";
                std::cin >> posicionMadre;
                std::cin.ignore();

                if (posicionMadre >= 0 && posicionMadre < lista_hembras.size()) {
                    // Comparar ascendencia genealógica
                    Caballo madre = caballos[posicionMadre];
                    Caballo* padre = caballos[posicionCaballo].padre;

                    bool existeCoincidencia = compararAscendenciaGenetica(padre, &madre, 3, 0);

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealógica de la madre coincide con la del padre." << std::endl;
                    } else {
                        std::cout << u8"Éxito: La ascendencia genealógica de la madre no coincide con la del padre." << std::endl;
                        madre.hijos.push_back(&caballos[posicionCaballo]);
                        caballos[posicionCaballo].madre = &madre;
                        
                    }
                } else {
                    std::cout << u8"Número de madre inválido." << std::endl;
                }
            } else {
                std::cout << u8"La madre se registrará como un nuevo caballo." << std::endl;
                Caballo madre = agregar_ejemplar();
                madre.hijos.push_back(&caballos[posicionCaballo]);
                caballos.push_back(madre);
                caballos[posicionCaballo].madre = &caballos[caballos.size() - 1];
                std::cout << "MADRE REGISTRADA\n" << std::endl;
            }
        } else if (opcion == 2) {
            std::cout << u8"¿El padre ya se encuentra registrado? (s/n): ";
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore();

            if (respuesta == "s") {
                // Muestra todos los caballos registrados que sean machos
                int posicionPadre;
                std::vector<Caballo>lista_machos;
                int k=0;
                std::cout << u8"Lista de sementales:" << std::endl;
                for (int i = 0; i < caballos.size(); ++i) {
                    if (caballos[i].sexo == "M") {
                        std::cout << k << ". " << caballos[i].nombre << " RIE: "<< caballos[i].rie<<std::endl;
                        lista_machos.push_back(caballos[i]);
                        k++;
                    }
                }

                std::cout << u8"Indique el número del padre: ";
                std::cin >> posicionPadre;
                std::cin.ignore();

                if (posicionPadre >= 0 && posicionPadre < lista_machos.size()) {
                    // Comparar ascendencia genealógica
                    Caballo padre = caballos[posicionPadre];
                    Caballo* madre = caballos[posicionCaballo].madre;

                    bool existeCoincidencia = compararAscendenciaGenetica(&padre, madre, 3, 0);

                    if (existeCoincidencia) {
                        std::cout << u8"Error: La ascendencia genealógica del padre coincide con la de la madre.\n" << std::endl;
                    } else {
                        std::cout << u8"Éxito: La ascendencia genealógica del padre no coincide con la de la madre.\n" << std::endl;
                        padre.hijos.push_back(&caballos[posicionCaballo]);
                        caballos[posicionCaballo].padre = &padre;
                    }
                } else {
                    std::cout << u8"Número de padre inválido." << std::endl;
                }
            } else {
                std::cout << u8"El padre se registrará como un nuevo caballo." << std::endl;
                Caballo padre = agregar_ejemplar();
                padre.hijos.push_back(&caballos[posicionCaballo]);
                caballos.push_back(padre);
                caballos[posicionCaballo].padre = &caballos[caballos.size() - 1];
                std::cout << "PADRE REGISTRADO\n" << std::endl;
            }
        }
    } else {
        std::cout << u8"La posición ingresada está fuera de rango.\n" << std::endl;
    }

    return caballos;
}
//_____________________________________________________________________________________________________________________

                            //GENEALOGIA ASCENDENTE

void mostrarNivel(Caballo* caballo, int nivel) {
    if (caballo != nullptr) {
        if (nivel >= 1) {
            std::cout << "Nivel " << nivel << ": " << caballo->nombre << std::endl;

            mostrarNivel(caballo->madre, nivel - 1);
            mostrarNivel(caballo->padre, nivel - 1);
        }
    }
}

void genealogiaAscendente(std::vector<Caballo>caballos, int posicionCaballo, int numNiveles) {
    if (posicionCaballo >= 0 && posicionCaballo < caballos.size()) {
        const Caballo& caballo = caballos[posicionCaballo];

        std::cout << "Genealogía ascendente de " << caballo.nombre << ":" << std::endl;

        std::cout << "Nivel 1: " << caballo.nombre << std::endl;

        mostrarNivel(caballo.madre, numNiveles);
        mostrarNivel(caballo.padre, numNiveles);
    } else {
        std::cout << "La posición ingresada está fuera de rango." << std::endl;
    }
}

//_________________________________________________________________________________________________________________

                            //GENEALOGIA DESCENDENTE

void determinarGenealogiaDescendente(const Caballo* individuo, int nivel) {
  // Imprimir el nombre del individuo con indentación basada en el nivel
  std::cout << "Nivel "<< nivel << ": "<< individuo->nombre << std::endl;

  // Verificar si el individuo tiene hijos
  if (!individuo->hijos.empty()) {
    // Recorrer los hijos del individuo de manera recursiva
    for (const auto* hijo : individuo->hijos) {
      determinarGenealogiaDescendente(hijo, nivel + 1);
    }
  }
}
//________________________________________________________________________________________________________________________


void genealogiaDescendenteSexo(const Caballo* individuo, int nivel, std::string sexo) {
  // Imprimir el nombre del individuo con indentación basada en el nivel
  if(individuo->sexo==sexo)
    std::cout << "Nivel "<< nivel << ": "<< individuo->nombre << std::endl;

  // Verificar si el individuo tiene hijos
  if (!individuo->hijos.empty()) {
    // Recorrer los hijos del individuo de manera recursiva
    for (const auto* hijo : individuo->hijos) {
      determinarGenealogiaDescendente(hijo, nivel + 1);
    }
  }
}

void menu() {
    Caballo caballo;
    std::vector<Caballo> caballos = cargarCaballos();
    bool primera_iteracion = true;
    int opcion;
    int posicionCaballo;


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
        if (!primera_iteracion||caballos.size()>0){
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
                    std::cout << "Caballos registrados:" << std::endl;
                    for (int i = 0; i < caballos.size(); ++i) {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                    std::cout << "Ingrese la posición del caballo: ";
                    std::cin >> posicionCaballo;
                    std::cin.ignore();
                    genealogiaAscendente(caballos,posicionCaballo,0);
                    break;
                case 5:
                    std::cout << "Caballos registrados:" << std::endl;
                    for (int i = 0; i < caballos.size(); ++i) {
                        std::cout << i << ". " << caballos[i].nombre << std::endl;
                    }
                    
                    std::cout << "Ingrese la posición del caballo: ";
                    std::cin >> posicionCaballo;
                    std::cin.ignore();
                    determinarGenealogiaDescendente(&caballos[posicionCaballo],0);
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
                        genealogiaDescendenteSexo(&caballos[posicionCaballo], 0, "M");
                    else{
                        genealogiaDescendenteSexo(&caballos[posicionCaballo],0,"F");
                    }
                    break;

                case 7:
                  
                  
                    break;
                case 8:
                    
                case 9:
                    std::cout << "Saliendo del menu..." << std::endl;
                    guardarCaballos(caballos);
                    break;

                default:
                    std::cout << "Opcion invalida. Por favor, ingrese una opcion valida." << std::endl;
                    break;
                
            }

        }else{
            if(opcion == 1){
                caballo=agregar_ejemplar();
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
