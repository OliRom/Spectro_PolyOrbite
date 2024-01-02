import serial
import matplotlib.pyplot as plt

# Définir le port série et la vitesse de transmission (baudrate)
port = "COM5"  # Remplacez cela par le port correct sur votre système (ex: "COM3" sur Windows)
baudrate = 9600
n_pixel  =3694

# Ouvrir la connexion série
ser = serial.Serial(port, baudrate)

# Initialiser les listes pour stocker les valeurs
val = [None] * n_pixel

try:
    while True:
        # Lire une ligne de données depuis le port série
        line = ser.readline().decode("utf-8").strip()

        # Diviser les valeurs en utilisant la virgule comme séparateur
        values = line.split(',')

        # Vérifier si le format est correct (deux valeurs séparées par une virgule)
        if len(values) == 2:
            # Convertir les valeurs en nombres flottants
            x, y = map(float, values)

            # Ajouter les valeurs aux listes
            val[int(x)] = y
        
            if x == n_pixel-1:
                plt.plot(range(n_pixel), val)
                plt.ylim(0, 4096)
                plt.pause(0.01)
                plt.clf()
        
        else:
            print("Format de données incorrect:", line)

except KeyboardInterrupt:
    # Fermer la connexion série en cas d'interruption manuelle (Ctrl+C)
    ser.close()
    print("Connexion série fermée.")



