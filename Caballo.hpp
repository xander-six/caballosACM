// Caballo.hpp
#include <string>
#include <vector>

#ifndef CABALLO_HPP
#define CABALLO_HPP

class Caballo {
public:
  std::string nombre;
  std::string tipo_caballo;
  Caballo* padre;
  Caballo* madre;
  int rie;
  std::string haras;
  std::string sexo;
  std::vector<Caballo*> hijos;;
  bool vivo=true;
};

#endif


//CONSULTAS

//DEBEN SER AMBOS PADRES DEL TIPO CABALLO O SOLO EL PADRE