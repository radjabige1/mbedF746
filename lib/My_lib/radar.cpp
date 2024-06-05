#include "radar.h"

// Déclaration des objets
lv_obj_t * title;
lv_obj_t * progress_bar;
lv_obj_t * dist_label;
lv_obj_t * btn_cont;
lv_obj_t * start_btn;
lv_obj_t * start_label;
lv_obj_t * stop_btn;
lv_obj_t * stop_label;

// Initialisation LVGL et création de l'interface utilisateur
void create_ui()
{
    // Création de l'écran
    lv_obj_t * scr = lv_scr_act();

    // Ajout du titre
    title = lv_label_create(scr);
    lv_label_set_text(title, "Projet Radar de Recul");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5); // Alignement au centre en haut

    // Création de la barre de progression
    progress_bar = lv_bar_create(scr);
    lv_bar_set_range(progress_bar,0, 60);
    lv_obj_set_size(progress_bar, 200, 20); // Dimensions de la barre de progression
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, -20); // Positionner au centre avec un décalage vers le haut

    // Création du texte pour afficher la distance
    dist_label = lv_label_create(scr);
    lv_label_set_text(dist_label, "L'obstacle se trouve a ... cm");
    lv_obj_align(dist_label, LV_ALIGN_CENTER, 0, 20); // Positionner sous la barre de progression

    // Création du conteneur pour les boutons
    btn_cont = lv_obj_create(scr);
    lv_obj_set_size(btn_cont, LV_PCT(100), 70); // Largeur 100% de l'écran, hauteur 70 pixels
    lv_obj_align(btn_cont, LV_ALIGN_BOTTOM_MID, 0, 0); // Aligner en bas au centre avec un décalage vers le haut

    // Ajuster la disposition des boutons
    lv_obj_set_layout(btn_cont, LV_LAYOUT_FLEX);
    lv_obj_set_style_pad_column(btn_cont, 20, 0); // Espacement entre les boutons
    lv_obj_set_flex_flow(btn_cont, LV_FLEX_FLOW_ROW); // Disposition en ligne
    lv_obj_set_flex_align(btn_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // Centrer les boutons horizontalement et verticalement

    // Création du bouton Start
    start_btn = lv_btn_create(btn_cont);
    lv_obj_set_size(start_btn, 100, 50);
    start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "Start");
    lv_obj_center(start_label);
    lv_obj_add_event_cb(start_btn, start_button_event_handler, LV_EVENT_CLICKED, NULL);

    // Création du bouton Stop
    stop_btn = lv_btn_create(btn_cont);
    lv_obj_set_size(stop_btn, 100, 50);
    stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "Stop");
    lv_obj_center(stop_label);
    lv_obj_add_event_cb(stop_btn, stop_button_event_handler, LV_EVENT_CLICKED, NULL);
}

// Fonction de gestion de l'événement du bouton Start
void start_button_event_handler(lv_event_t * e) 
{
    radar_active = true;
    printf("Radar started\n");
}

// Fonction de gestion de l'événement du bouton Stop
void stop_button_event_handler(lv_event_t * e) 
{
    radar_active = false;
    buzzer = 0; // Assurer que le buzzer est éteint quand le radar est arrêté
    printf("Radar stopped\n");
}

// Fonction de mise à jour de la barre de progression en fonction de la distance
void update_progress_bar(int dist)
{
    // Mise à jour de la valeur de la barre de progression
    lv_bar_set_value(progress_bar, dist, LV_ANIM_OFF);

    // Changer la couleur de la barre de progression en fonction de la distance
    if (dist<=10) 
    {
        lv_obj_set_style_bg_color(progress_bar, lv_color_make(255, 0, 0), LV_PART_MAIN); // Rouge
    } 
    else if (dist <= 25) 
    {
        lv_obj_set_style_bg_color(progress_bar, lv_color_make(255, 165, 0), LV_PART_MAIN); // Orange
    } 
    else if (dist <=40 ) 
    {
        lv_obj_set_style_bg_color(progress_bar, lv_color_make(255, 255, 0), LV_PART_MAIN); // Jaune
    } 
    else 
    {
        lv_obj_set_style_bg_color(progress_bar, lv_color_make(0, 255, 0), LV_PART_MAIN); // Vert
    }
}

// Fonction de mise à jour du texte avec la distance
void update_distance_label(int dist)
{
    char buf[32]; // Un tampon pour stocker la chaîne formatée
    sprintf(buf, "L'obstacle se trouve a %d cm", dist); // Formatage de la chaîne avec la distance
    lv_label_set_text(dist_label, buf); // Mettre à jour le texte du label avec la nouvelle chaîne
}

// Fonction de gestion du buzzer en fonction de la distance
void sound_buzzer(int dist)
{
    if (dist <= 10) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(50ms);
        buzzer = 0;
    } 
    else if (dist <= 25) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(100ms);
        buzzer = 0;
    }
    else if (dist <= 40) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(200ms);
        buzzer = 0;

    }  
    else 
    {
        buzzer = 0;
    }
}
