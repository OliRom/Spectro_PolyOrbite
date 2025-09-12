/*
 Module pour exécuter des fonctions à intervalle défini.

 Exemple de fonctionnement:

  #include "TaskManager.h"

  Task task1 = { ledBlink, 2000 };  // Appeler ledBlink toutes les 2s
  Task task2 = { sayHello, 5000 };  // Appeler sayHello toutes les 5s

  void setup() {
  }

  void loop() {
    run_task(task1);
    run_task(task2);
    
    // Suite du programmme...
  }

*/

#ifndef TASK_MANAGER
#define TASK_MANAGER

#include <Arduino.h>


// Structure pour définir les tâches.
struct Task {
  void (*func)();                  // Pointeur vers la fonction à exécuter
  unsigned long interval;          // Intervalle d'exécution en ms
  unsigned long previous_call_ms = 0;  // Temps du dernier appel
};

// Fonction qui exécute les tâches lorsque le moment est venu.
void run_task(Task &task) {
  unsigned long now_ms = millis();
  if (now_ms - task.previous_call_ms >= task.interval) {
    task.func();
    task.previous_call_ms = now_ms;
  }
}

// Fonction pour réinitialiser le compte à rebours d'une fonction
void reset_task_timer(Task &task) {
  task.previous_call_ms = millis();
}

// Changer la fréquence d'appel d'une tâche
void change_task_interval(Task &task, int freq) {
  task.interval = freq;
}

#endif
