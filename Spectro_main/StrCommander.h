#ifndef STR_COMMANDER
#define STR_COMMANDER

#include <Arduino.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>


// Variable types
enum type_enum {
  // Variables
  INT,
  FLOAT,
  BOOL,

  // Functions
  INT_INT_INT,
  FLOAT_FLOAT,
  _CCHAR,  // const char
  STR_,    // c++ string
};


/*
Split a sring according to a delimiter.

string (std::string): string to split
delim (char): character where to split the string

return (std::vector<std::string>): a vector containing the splitted string
*/
std::vector<std::string> split(std::string *str, char delim) {
  std::vector<std::string> result;
  std::string item;

  int begin = 0;
  int end;
  do {
    end = str->find_first_of(delim, begin);
    result.push_back(str->substr(begin, end - begin));
    begin = end + 1;
  } while (end != -1);

  return result;
}


// Returns a string with everything in lower case passed by value (the original string is not modified)
std::string to_lower(std::string str) {
  for (char &c : str) {
    c = std::tolower(c);
  }
  return str;
}

// Determines wether a string is true (1), false (0), or invalid (-1)
byte is_true(std::string const &str) {
  std::string_view v1{ to_lower(str) };

  if (v1 == "true") return 1;
  if (v1 == "yes") return 1;
  if (v1 == "on") return 1;
  if (v1 == "1") return 1;

  if (v1 == "false") return 0;
  if (v1 == "no") return 0;
  if (v1 == "off") return 0;
  if (v1 == "0") return 0;

  return 2;
}

// Count the number of occurence of a character in a string
int str_count(std::string const &str, char c) {
  return std::count(str.begin(), str.end(), c);
}


struct str_cmd_struct {
  void *var;
  type_enum type;
};


class StrCommander {
private:
  std::map<std::string, str_cmd_struct> *_var_table;  // Tableau d'alias pour les variables
  std::map<std::string, str_cmd_struct> *_fun_table;  // Tableau d'alias pour les fonctions


  // Obtenir la valeur d'une variable
  std::string _get_var(std::string *noun) {
    std::map<std::string, str_cmd_struct>::iterator ent = _var_table->find(*noun);

    if (ent == _var_table->end()) {
      return "unfound alias '" + *noun + "'";
    }

    str_cmd_struct entry = ent->second;

    switch (entry.type) {
      case INT:
        {
          int *var = static_cast<int *>(entry.var);
          return std::to_string(*var);
        }
        break;

      case FLOAT:
        {
          float *var = static_cast<float *>(entry.var);
          return std::to_string(*var);
        }
        break;

      case BOOL:
        {
          bool *var = static_cast<bool *>(entry.var);
          return std::to_string(*var);
        }
        break;

      default:
        return "'" + *noun + "' is of unknown type.";
        break;
    }
    return "";
  };


  // Spécifier la valeur d'une variable
  std::string _set_var(std::string *noun, std::string *args) {
    std::map<std::string, str_cmd_struct>::iterator ent = _var_table->find(*noun);

    if (ent == _var_table->end()) {
      return "unfound alias '" + *noun + "'";
    }

    str_cmd_struct entry = ent->second;

    switch (entry.type) {
      case INT:
        {
          int *var = static_cast<int *>(entry.var);
          *var = std::stoi(*args);
        }
        break;

      case FLOAT:
        {
          float *var = static_cast<float *>(entry.var);
          *var = std::stof(*args);
        }
        break;

      case BOOL:
        {
          bool *var = static_cast<bool *>(entry.var);

          switch (is_true(*args)) {
            case 1: *var = true; break;
            case 0: *var = false; break;
            case 2: return "invalid bool conversion of '" + *args + "'."; break;
          }
        }
        break;

      default:
        return "'" + *noun + "' is of unknown type.";
        break;
    }
    return "";
  };


  // Appeler une fonction
  std::string _call_fun(std::string *noun, std::string *args) {
    std::map<std::string, str_cmd_struct>::iterator ent = _fun_table->find(*noun);

    if (ent == _fun_table->end()) {
      return "unfound alias '" + *noun + "'";
    }

    str_cmd_struct entry = ent->second;

    switch (entry.type) {
      case INT_INT_INT:
        {
          int n = str_count(*args, '&');
          if (n != 1) {
            return "'" + *noun + "' expected 2 arguments. Got " + std::to_string(n + 1) + " instead.";
          }

          std::vector<std::string> _args = split(args, '&');

          int a1 = std::stoi(_args[0]);
          int a2 = std::stoi(_args[1]);

          int (*fun)(int, int);
          fun = reinterpret_cast<int (*)(int, int)>(entry.var);

          return std::to_string(fun(a1, a2));
        }
        break;

      case FLOAT_FLOAT:
        {
          float a = std::stof(*args);

          float (*fun)(float);
          fun = reinterpret_cast<float (*)(float)>(entry.var);

          return std::to_string(fun(a));
        }
        break;

      case _CCHAR:
        {
          const char *a = args->c_str();

          void (*fun)(const char *);
          fun = reinterpret_cast<void (*)(const char *)>(entry.var);

          fun(a);

          return "";
        }
        break;

      case STR_:
        {
          std::string (*fun)();
          fun = reinterpret_cast<std::string (*)()>(entry.var);

          return fun();
        }
        break;

      default:
        return "'" + *noun + "' is of unknown type.";
        break;
    }
  };


public:
  StrCommander(){};  // Instancier un objet

  // Spécifier le tableau d'alias pour les variables
  void set_var_table(std::map<std::string, str_cmd_struct> *var_table) {
    _var_table = var_table;
  };

  // Spécifier le tableau d'alias pour les fonctions
  void set_fun_table(std::map<std::string, str_cmd_struct> *fun_table) {
    _fun_table = fun_table;
  };

  // Exécute une commande
  std::string execute_command(std::string *command) {
    std::vector<std::string> tokens = split(command, ' ');
    std::string return_msg;
    std::string test;

    if (tokens.size() < 2) {
      return "Expected at least 2 tokens. Got " + std::to_string(tokens.size());
    }

    std::string verb = tokens[0];
    std::string noun = tokens[1];

    if (verb.compare("get") == 0) {
      return _get_var(&noun);
    }

    else if (verb.compare("set") == 0) {
      if (tokens.size() < 2) {
        return "Expected 3 tokens. Got " + std::to_string(tokens.size());
      }

      std::string args = tokens[2];
      return _set_var(&noun, &args);
    }

    else if (verb.compare("call") == 0) {
      if (tokens.size() < 2) {
        return "Expected 3 tokens. Got " + std::to_string(tokens.size());
      }

      std::string args = tokens[2];
      return _call_fun(&noun, &args);
    }

    else {
      return "unknown command '" + verb + "'";
    }
  };
};

#endif
