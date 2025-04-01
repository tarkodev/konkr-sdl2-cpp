# Konkr - Projet de Programmation Avancée

Ce projet implémente une version du jeu **Konkr** en C++ moderne avec affichage graphique via SDL2. Réalisé en binôme dans le cadre du cours de Programmation Avancée, le projet se base sur une architecture modulaire, l'utilisation intensive de la STL et les bonnes pratiques modernes (C++17 et ultérieur).

## Fonctionnalités

- **Jeu tour par tour** avec version multijoueur (sans IA ennemie, bandits autonomes à déplacement aléatoire)
- **Grille hexagonale** pour la gestion du terrain
- Chargement de cartes depuis un fichier ASCII
- Coloration par faction
- Implémentation de pathfinding / flooding pour vérifier la connectivité d'un territoire
- Gestion des bâtiments et des troupes avec :
  - Ordre de puissance
  - Protection (calculée selon la puissance)
  - Fusion pour promouvoir les troupes
  - Revenu et paiement des salaires, avec gestion de la mutinerie en cas d'impayé
- Sélection au clic et via le clavier
- **Bonus pour points supplémentaires :**
  - Animations basiques des troupes
  - Drag & Drop
  - Options de rewind
  - Éditeur de cartes pour créer/modifier/enregistrer des niveaux

## Technologies et bonnes pratiques

- **C++ moderne** (C++17 minimum) : utilisation de lambdas, smart pointers (unique/shared/weak_ptr), range-based for, etc.
- **STL** : aucun tableau C-style, utilisation des conteneurs et algorithmes STL
- **Architecture orientée objet** : classes pour encapsuler les objets du jeu et la SDL2, héritage et polymorphisme utilisés
- **Documentation** et code structuré pour une maintenance aisée

## Structure du projet

```
.
├── CMakeLists.txt         # Configuration de la construction avec CMake
├── README.md              # Ce fichier
├── assets                 # Ressources (images, sons, etc.)
├── build                  # Répertoire de build généré par CMake
├── include                # Fichiers d'en-tête (Game, GameMap, HexagonGrid, Window, etc.)
├── lib                    # Bibliothèques externes (SDL2, SDL2_gfx, etc.)
└── src                    # Code source (implémentations des classes et main)
```

## Construction et exécution

### Prérequis
- CMake (version 3.10 ou supérieure)
- Un compilateur C++ supportant C++17 (ou plus récent)
- SDL2 et SDL2_gfx (fournis dans le dossier `lib/`)

### Instructions de build

1. **Générer les fichiers de build :**

   ```bash
   mkdir build
   cd build
   cmake ..
   ```

2. **Compiler le projet :**

   ```bash
   make
   ```

3. **Exécuter l'application :**

   ```bash
   ./konkr
   ```
