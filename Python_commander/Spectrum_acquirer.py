import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime
import os


def str_to_array(msg):
    """
    Converti les données envoyées du Arduino sous forme de str en un np.array
    contenant les valeurs des pixels.
    """
    return np.fromstring(msg, dtype=int, sep=',')
    
def plot_spectrum(data, title):
    plt.plot(data)
    plt.xlabel("Numéro du pixel")
    plt.ylabel("Intensité lumineuse (u.a.)")
    plt.title(title)
    plt.show()
    
def get_data_cbk(msg):
    raw_data = msg[1].split(" ")[1]
    data = str_to_array(raw_data)
    
    date = datetime.now()
    date_str = date.strftime("%Y_%m_%d_%H_%M_%S")+".txt"
    file_name = os.path.join("..", "Spectres_data", date_str)
    with open(file_name, "w") as f:
        f.write(raw_data)
        
    plot_spectrum(data, date_str)
    