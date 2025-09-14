# Tutoriel sur l'utilisation du spectromètre


## Installation
Pour contrôler le spectro, s'assurer que celui-ci est alimenté (12V) et branché sur le port série d'un ordinateur (cable USB).


## Contrôle du spectromètre à partir de chaines de caractère
Le spectromètre peut se contrôler avec des chaines de caractères envoyées sur le port série.
La structure d'une commande est `<verbe> <sujet> [complément]` où le verbe et le sujet sont obligatoires et le complément est optionnel (dépend du verbe et du sujet).
Les sujets peuvent être soit une variable, soit une fonction.
Lorsque plusieurs compléments sont spécifiés, ils doivent être séparés par le caractère `&`.

Les verbes disponibles pour une variable sont `"set"` et `"get`" et permettent d'initialiser ou de lire sa valeur.
Voici un exemple:
```
set laser_target_temp 25

get laser_target_temp
# Résultat: 25
```

Le verbe `"call"` sert à appeller les fonctions.
En voici quelques exemples:
```
call get_temp
# Résultat: 21.37

call allow_lasing 1&@
# Résultat:

call acquire_data 3
# Résultat: 0,924,1024, ...
```


## Interface graphique
Une interface graphique (voir la figure ci-bas) peut être utilisée pour contrôler le spectro.
Il faut d'abord choisir le nom d'alias pour une variable ou une fonction à partir de la liste déroulante.
Les arguments doivent également être entrés dans la zone réservée à cet effet.
Si plusieurs arguments sont requis, ils doivent être séparés par un retour à la ligne (`\n`).
Selon le type du `<sujet>` (variable ou fonction), les verbes `set`/`get` ou `call` peuvent être exécutés en appuyant sur les bouttons correspondants.
Le message de commande envoyé ainsi que le résultat retourné par le spectro s'afficheront dans la boite "Résultats" au bas de l'interface.
La zone à droite de l'interface décrit sommairement les variables et fonctions disponibles pour le spectro.

![GUI](gui.png "GUI")


## Initialisation des paramètres de mesure
Avant d'effectuer une première mesure avec le spectromètre, il faut initialise les paramètres de mesure.
Les paramètres disponibles sont `allow_lasing` et `set_integration_time`.


### 1) `allow_lasing(state, code)`
Par défaut, il est impossible d'activer le laser par mesure de sécurité.
Par conséquent, toute mesure de spectre est impossible.
La fonction `allow_lasing` permet d'activer (ou désactiver) cette fonction.
Le premier argument est un booléen qui permet ou non l'activation du laser.
Le second est un caractère de sécurité qui doit être entré, sans quoi la commande désactive le laser.
Ce caractère est `@`.
La figure ci-dessous présente un exemple d'appel de cette fonction.

![allow_lasing](allow_lasing.png "allow_lasing")


### 2) `set_integration_time(time)`
Un capteur CCD présent dans le spectro permet la lecture des spectres.
Ce dernier fonctionne en accumulant des charges durant un temps d'intégration `time` [us].
Plus le temps d'intégration est long, plus le capteur accumulera de charges et moins le ratio signal sur bruit (SNR) sera grand ce qui est souhaitable.
Cependant, lorsque le temps d'intégration est trop long, le CCD devient saturé ce qui invalide la mesure.
Il est donc important de trouver une valeur inférieure à ce seuil (qui dépend de la luminosité ambiante, la transparence de l'échantillon, la puissance du laser, ...).
La section [Ajustement du temps d'intégration](#ajustement-du-temps-dintégration) fournit un guide pour aider à déterminer ce temps optimal.


## Acquisition des données avec `acquire_data(n_mesures)`
L'acquisition des données se fait avec la fonction `acquire_data`.
L'argument `n_mesures` spécifie le nombre de mesures prises pour moyenner le bruit.
Par exemple, la commande `call acquire_data 10` mesure 10 spectres avec le temps d'intégration spécifié avec la fonction `set_integration_time`, puis somme les valeurs obtenues pour chaque pixel.
Une courbe de calibration est également mesurée, puis soustraite du spectre avant de retourner les données afin de supprimer le bruit de fond.
Les résultats sont retournés sous la forme d'une chaine de caractères où les valeurs des pixels sont séparées par des virgules.

Les résultats sont automatiquement affichés à l'aide de matplotlib et enregistrés dans le répertoire `Spectres_data\`.
Le nom du fichier est indiqué dans le graphique matplotlib tel que présenté ci-bas.
Plusieurs appels de `acquire_data` sans fermer le graphique superposent les spectres.

![exemple_spectre](spectre.png "exemple spectre")


## Contrôle de la température du laser
La température du laser du spectro est automatiquement régulée 10 fois par seconde par le Arduino.
Cette régulation implique d'allumer le système de refroidissement du laser lorsque sa température est trop élevée et d'allumer le ventilateur pour permettre l'évacuation de la chaleur.
Ce dernier s'éteint automatiquement 5 minutes après que la température du laser soit retombée sous la cible fixée.
La variable `laser_target_temp` [Celsius] permet à l'utilisateur de spécifier cette cible.
La valeur par défaut est de 25 degrés Celsius.


## Ajustement du temps d'intégration
Cette section se veut un guide pour aider l'utilisateur à déterminer un temps d'intégration optimal pour la lecture par le capteur CCD du spectro.


### Temps d'intégration trop faible
Si le temps d'intégration est trop faible, un graphique comme le suivant peut être obtenu.
Ceci peut également se produire si l'alimentation de 12V n'est pas fournie au spectro ou qu'un des interrupteurs de sécurité du couvercle n'est pas enclenché.
On observe du bruit autour de zéro.

![int1_n1](debugging/int1_n1.png "int1_n1")

Si le temps d'intégation est plus long, on obtient une courbe qui peut ressembler à celle qui suit.
On peut commencer à discerner un spectre, mais ce dernier est très buité.

![int10000_n1](debugging/int10000_n1.png "int10000_n1")

Ce bruit peut être réduit en augmentant le nombre de mesures.
Dans ce cas, `n_mesures` est passé de 1 à 10000.

![int10000_n10000](debugging/int10000_n10000.png "int10000_n10000")


### Temps d'intégration adéquat
Avant d'augmenter le nombre de mesure, il est recommandé d'augmenter le temps d'intégration.
Ceci permet d'avoir une plus grande amélioration sur le SNR que d'augmenter le nombre de mesures en plus de réduire le temps de mesure.
La figure suivant illustre un temps d'intégration optimal.
Le SNR est amélioré.

![int100000_n1](debugging/int100000_n1.png "int100000_n1")

Une fois le temps d'intégration optimal trouvé, augmenter le nombre de mesures améliore encore plus le SNR (figure suivante).

![int100000_n100](debugging/int100000_n100.png "int100000_n100")


### Temps d'intégration trop grand
En revanche, si le temps d'intégration est trop élevé, le capteur CCD sature et le spectre n'est plus visible.
La figures suivante présente une lecture de spectre invalide où le CCD est saturé.

![int200000_n1](debugging/int200000_n1.png "int100000_n1")
