"""
Programme à exécuter pour contrôler le spectromètre.
"""

import tkinter as tk
from StrCommander import StrCmd
from Spectrum_acquirer import get_data_cbk


def exec_gui(aliases_list, function_callbacks):
    cmd = StrCmd("Périphérique")  # Connecter le Arduino
        
    def rst():
        alias_dropdown.set("Sélectionner un alias")
        arguments_text.delete("1.0", tk.END)
        result_text.config(text="")
    
    def cmd_format_with_verb(verb, callback=dict()):
        def cmd_format():
            alias = alias_dropdown.get()
            if alias == "Sélectionner un alias":
                result_text.config(text="Veuillez sélectionner un alias")
                return
            
            args = arguments_text.get("1.0", tk.END)[:-1]
            args_str = "&".join(args.split("\n"))
            
            res = cmd.exec_cmd(verb, alias, args_str)
            
            fun = callback.get(alias)
            if fun is not None:
                fun(res)
            
            result_text.config(text="\n".join(res))
            
        return cmd_format
    
    x_pos = 250
    arg_width = 250
    instr_width = 600

    # Création de la fenêtre principale
    root = tk.Tk()
    root.title("Spectro commander")
    root.geometry(str(x_pos + arg_width + instr_width + 50)+"x600")

    # Liste déroulante (alias)
    alias_default_txt = tk.StringVar()
    alias_default_txt.set("Sélectionner un alias")  # Texte par défaut

    alias_dropdown = tk.ttk.Combobox(root, textvariable=alias_default_txt, state="readonly", values=aliases_list)
    alias_dropdown.place(x=20, y=20, width=200, height=25)

    # Bouton "set"
    set_button = tk.Button(root, text="set", command=cmd_format_with_verb("set"))
    set_button.place(x=50, y=70, width=100, height=25)

    # Bouton "get"
    get_button = tk.Button(root, text="get", command=cmd_format_with_verb("get"))
    get_button.place(x=50, y=110, width=100, height=25)

    # Bouton "call"
    call_button = tk.Button(root, text="call", command=cmd_format_with_verb("call", function_callbacks))
    call_button.place(x=50, y=150, width=100, height=25)

    # Bouton "reset"
    reset_button = tk.Button(root, text="reset", command=rst)
    reset_button.place(x=50, y=210, width=100, height=25)

    # Zone de texte pour les arguments
    arguments_label = tk.Label(root, text="Arguments:", anchor="w")
    arguments_label.place(x=x_pos, y=20)

    arguments_text = tk.Text(root, wrap="word")
    arguments_text.place(x=x_pos, y=50, width=arg_width, height=200)

    # Résultats
    result_label = tk.Label(root, text="Résultat:", anchor="w")
    result_label.place(x=20, y=270)
    
    result_text = tk.Label(root, text="", anchor="w", justify=tk.LEFT, relief="sunken")
    result_text.place(x=20, y=300, width=x_pos + arg_width, height=125)
    
    # Instructions
    instructions_text = \
"""Voici le contrôleur du spectromètre.

Pour envoyer une commande, sélectionnez l'alias correspondant à la variable à obtenir/initialiser \
ou à la fonction à appeler. Appuyez sur le bouton d'action respectif. Le résultat s'affichera dans \
la boite de texte dédiée à cet effet.

Alias de variables disponibles (set/get):
- laser_target_temp [int]: température maximale cible du laser (Celsius)

Alias de fonctions disponibles (call):
- get_temp():
    Retourne la température du laser [Celsius].

- allow_lasing(state=[0, 1], code=[str]):
    Permettre l'activation du laser. Pour que la commande fasse effet, le bon caractère de code \
doit être spécifié.

- set_integ_time(time=[int]):
    Temps [us] durant lequel s'effectue une mesure. La valeur par défaut est 500us.

- acquire_data(moyennage=[int]):
    Demande au spectro de faire une lecture de spectre. Le moyennage est le nombre de lectures qui \
seront effectuées pour réduire le ratio signal sur bruit.
"""
    instructions_label = tk.Label(root, text=instructions_text, anchor="w", justify=tk.LEFT,
                                  wraplength=instr_width)
    instructions_label.place(x=x_pos+arg_width+30, y=20)

    # Lancement de l'application
    root.mainloop()
    

if __name__ == "__main__":
    debug_mode = False
    
    # Alias pour les variables du Arduino
    var_aliases = [
        "laser_target_temp",
    ]

    # Alias pour les fonctions pour contrôler le spectro
    fun_aliases = [
        "get_temp",
        "allow_lasing",
        "set_integ_time",
        "acquire_data",
    ]
    
    # Alias des fonctions pour débugguer
    debug_aliases = [
        "activate_laser",
        "get_laser_state",
        "set_laser_timer",
        "reset_laser_timer",
        "activate_fan",
        "set_power",
        "get_power",
        "set_pot_value",
        "set_tec_power",
    ]

    alias = var_aliases + [i for i in fun_aliases if i not in var_aliases]
    if debug_mode:
        alias = alias + debug_aliases

    # Fonctions appelées avec les résultats de certains appels de fonction
    fn_callbacks = {
        "acquire_data": get_data_cbk,
    }
    
    # Exécuter la fenêtre principale
    exec_gui(alias, fn_callbacks)
