import tkinter as tk
from StrCommander import StrCmd


var_aliases = []

fun_aliases = [
    "activate_laser",
    "get_laser_state",
    "allow_lasing",
    "set_laser_timer",
    "reset_laser_timer",
    
    "set_temp",
    "get_temp",
    
    "set_power",
    "get_power",
    
    "set_integ_time",
    
    "acquire_data",
    "get_data",
]

aliases_list = var_aliases + [i for i in fun_aliases if i not in var_aliases]


def exec_gui():
    cmd = StrCmd("Périphérique")
        
    def rst():
        alias_dropdown.set("Sélectionner un alias")
        arguments_text.delete("1.0", tk.END)
        result_text.config(text="")
    
    def cmd_format_with_verb(verb):
        def cmd_format():
            alias = alias_dropdown.get()
            if alias == "Sélectionner un alias":
                result_text.config(text="Veuillez sélectionner un alias")
                return
            
            args = arguments_text.get("1.0", tk.END)[:-1]
            args_str = "&".join(args.split("\n"))
            
            res = cmd.exec_cmd(verb, alias, args_str)
            result_text.config(text="\n".join(res))
            
        return cmd_format
    
    x_pos = 250
    arg_width = 250

    # Création de la fenêtre principale
    root = tk.Tk()
    root.title("Spectro commander")
    root.geometry(str(x_pos + arg_width + 50)+"x380")

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
    call_button = tk.Button(root, text="call", command=cmd_format_with_verb("call"))
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
    result_label.place(x=x_pos, y=270)

    result_text = tk.Label(root, text="", anchor="w", justify=tk.LEFT, relief="sunken")
    result_text.place(x=x_pos, y=300, width=arg_width, height=50)

    # Lancement de l'application
    root.mainloop()
    

if __name__ == "__main__":
    exec_gui()
