# Rapport IIOT : BIAIS Budwael PERIER Marc

## Introduction

Nous souhaitons présenter dans cette documentation les différents fichiers que nous avons créer ainsi que les fonctions qui les composent pour offrir la meilleure compréhension pour l'utilisateur.

Vous pouvez vous déplacer à travers l'arbre de navigation sur la gauche pour étudier les différents fichiers contenu dans notre code.

On peut diviser cela en 2 dossier distincts : 
- Publisher : BIAIS Budwael
- Subscriber : PERIER Marc

---

## Publisher

Dans notre cas le publisher avait pour rôle de mesurer la température et l'humidité à l'aide d'un module SHT30 pour l'afficher et l'envoyer au Subscriber

Le dossier publisher est composé des dossiers suivants :
- PubSubClient : L'extension pour les fonctions liés au serveur MQTT fourni
- Wire : L'extension Arduino pour la communication I2C avec le shield
- WEMOS_SHT3X : L'extension pour le contrôle du shield de Température/Humidité
- AP : Le fichier de contrôle de l'access Point pour récupérer les données de Température/Humidité
- WS : Le fichier de gestion d'ensemble WIFI pour l'initialisation du serveur t l'affichage des pages WEB
- MQTT : Le fichier pour l'initialisation et la commuication sur le serveur MQTT
- Publisher : Le fichier main de flash de la carte ESP8266

### Connexion au Wifi Station et Access Point avec affichage sur le port Serial

![Connexion WS et AP](file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/WS_AP.png)

On peut voir sur l'image que la ESP8266 s'est bien connectée sur la Wifi Station via l'IP du routeur sur la wifi "robotique". On peut aussi constater que la carte a configuré comme il faut les paramètres Access Point qui par ailleurs est bien visible dans la recherche de connexion Wifi :

![IIOT Buddy](file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/IIOT_Temp_Buddy.png)
![IIOT config](file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/AP_config.png)

Tout ceci est afficher via la connexion au port Serial COM et de plus on remarque la connexion au serveur MQTT en tant que USER9.

### Communication sur le serveur MQTT

![MQTT](file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/SerialSensor.png)

On peut voir l'affichage sur le port Serial les valeurs de température et d'humidité (arrondis à 2 décimales) qui sont envoyés par intervalle de 2 secondes sur les 2 topics de paramètre : Temp et Humi

Ils seront ensuite récupérés par le Subscriber pour l'ouverture/fermeture du relais.

### Affichage sur le serveur Web

<img src="file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/Website.png" width="1000" height="600">


Notre code affiche bien sur l'adresse IP la page Web qui nous indique les IP de notre configuration WS et AP. Il nous donne aussi la possibilité de regarder les paramètres envoyés par le capteur de Température et d'Humidité :

<img src="file:///C:/Users/b.biais/OneDrive%20-%20ESTIA/Documents/PlatformIO/Projects/IIOT%20Project%20BP/doc/img/WebsiteSensor.png" width="1000" height="600">

La page web nous affiche les différents paramètres avec l'unité et la page Web sera rechargée toutes les 2 secondes (correspondant aussi au rechargement pour l'envoie des paramètres sur le serveur MQTT) afin de toujours avoir des valeurs actuelles.

---

## Subscriber

Dans notre cas le subscriber avait pour rôle de récupérer les mesures de température et dh'umidité sur le serveur MQTT et d'ouvrir ou fermer le relais en conséquence suivant les données récoltées

Le dossier subscriber est composé des dossiers suivants :
- PubSubClient : L'extension pour les fonctions liés au serveur MQTT fourni
- Wire : L'extension Arduino pour la communication I2C avec le shield
- WEMOS_SHT3X : L'extension pour le contrôle du shield de Température/Humidité
- AP : Le fichier de contrôle de l'access Point pour récupérer les données de Température/Humidité
- WS : Le fichier de gestion d'ensemble WIFI pour l'initialisation du serveur t l'affichage des pages WEB
- MQTT : Le fichier pour l'initialisation et la commuication sur le serveur MQTT
- Subscriber : Le fichier main de flash de la carte ESP8266

---

## Contact

Pour toute remarque supplémentaire ou questions à propos du code (ou pour nous engager à la NASA), veuillez nous contacter par mail à l'adresse b.biais@net.estia.fr ou m.perier@net.estia.fr