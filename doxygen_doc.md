# Formatage du code TSI

- [Formatage du code TSI](#formatage-du-code-tsi)
  - [Formatage global du code :](#formatage-global-du-code-)
  - [Formatage Doxygen pour fonctions :](#formatage-doxygen-pour-fonctions-)
  - [Formatage Doxygen pour classes :](#formatage-doxygen-pour-classes-)

## Formatage global du code :
 - Produire des test unitaires dès l'implémentation d'une fonctionnalité
 - Mettre tous les attributs en *private* lors de la création d'une classe
 - Créer les Getters & les Setters pour les attributs des classes en *const*
 - Toutes les variables & attributs: 
   - En [camelCase](https://fr.wikipedia.org/wiki/Camel_case)
   - Doivent avoir un sens *en anglais*
 - Toutes les méthodes :
   - En [PascalCase](https://fr.wikipedia.org/wiki/Camel_case)
   - Doivent aussi avoir un sens
   - Les getters & setters sont quand même en camelCase (setValue, getValue...)

## Formatage Doxygen pour fonctions : 
```cpp
/**
 * @brief Function to sum two numbers
 *
 * This function takes two numbers and returns the resulting sum
 *
 * @param a Fisrt number
 * @param b Second number
 * @return Sum of a & b
 */
int Sum(int a, int b) {
    return a + b;
}
```
## Formatage Doxygen pour classes : 
```cpp
/**
 * @brief Class representing a student
 *
 * This class represents a student with it's name and student id.
 */
class Etudiant {
public:
    /**
     * @brief Student class constructor
     * @param nom Student name
     * @param id Student ID
     */
    Student(const std::string& name, int id);

    /**
     * @brief Get the student name
     * @return Student name
     */
    std::string getName() const;

    /**
     * @brief Get the student ID
     * @return Student ID
     */
    int getId() const;

private:
    std::string name; ///< Student Name
    int idNumber;    ///< Student ID
};
```