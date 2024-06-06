#include "radar.h"

// Déclaration des objets
lv_obj_t *title;
lv_obj_t *progress_bar;
lv_obj_t *dist_label;
lv_obj_t *btn_cont;
lv_obj_t *start_btn;
lv_obj_t *start_label;
lv_obj_t *stop_btn;
lv_obj_t *stop_label;
lv_obj_t *buzzer_switch;
lv_obj_t *buzzer_label;

// Styles pour la barre de progression
lv_style_t style_red;
lv_style_t style_orange;
lv_style_t style_yellow;
lv_style_t style_green;
lv_style_t style_transparent;
lv_style_t style_switch_on;
lv_style_t style_switch_off;

// Variable pour contrôler l'état du buzzer
bool buzzer_enabled = true;

void create_ui()
{
    // Création de l'écran
    lv_obj_t *scr = lv_scr_act();

    // Ajout du titre
    title = lv_label_create(scr);
    lv_label_set_text(title, "Projet Radar de Recul");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5); // Alignement au centre en haut

    // Création de la barre de progression
    progress_bar = lv_bar_create(scr);
    lv_bar_set_range(progress_bar, 0, 60);
    lv_obj_set_size(progress_bar, 200, 20); // Dimensions de la barre de progression
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, -20); // Positionner au centre avec un décalage vers le haut

    // Création du texte pour afficher la distance
    dist_label = lv_label_create(scr);
    lv_label_set_text(dist_label, "L'obstacle se trouve a ... cm");
    lv_obj_align(dist_label, LV_ALIGN_CENTER, 0, 20); // Positionner sous la barre de progression

    // Création du conteneur pour les boutons
    btn_cont = lv_obj_create(scr);
    lv_obj_set_size(btn_cont, LV_PCT(100), 100); // Largeur 100% de l'écran, hauteur 100 pixels
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

    // Création du switch Buzzer On/Off
    buzzer_switch = lv_switch_create(btn_cont);
    lv_obj_set_size(buzzer_switch, 100, 50);
    lv_obj_add_event_cb(buzzer_switch, buzzer_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Création du label pour le switch
    buzzer_label = lv_label_create(btn_cont);
    lv_label_set_text(buzzer_label, "Buzzer On");
    lv_obj_align_to(buzzer_label, buzzer_switch, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // Initialisation des styles
    lv_style_init(&style_red);
    lv_style_set_bg_color(&style_red, lv_color_make(255, 0, 0));
    lv_style_set_bg_opa(&style_red, LV_OPA_COVER);

    lv_style_init(&style_orange);
    lv_style_set_bg_color(&style_orange, lv_color_make(255, 165, 0));
    lv_style_set_bg_opa(&style_orange, LV_OPA_COVER);

    lv_style_init(&style_yellow);
    lv_style_set_bg_color(&style_yellow, lv_color_make(255, 255, 0));
    lv_style_set_bg_opa(&style_yellow, LV_OPA_COVER);

    lv_style_init(&style_green);
    lv_style_set_bg_color(&style_green, lv_color_make(0, 255, 0));
    lv_style_set_bg_opa(&style_green, LV_OPA_COVER);

    lv_style_init(&style_transparent);
    lv_style_set_bg_opa(&style_transparent, LV_OPA_TRANSP);

    lv_style_init(&style_switch_off);
    lv_style_set_bg_color(&style_switch_off, lv_color_make(255, 0, 0));
    lv_style_set_bg_opa(&style_switch_off, LV_OPA_COVER);

    lv_style_init(&style_switch_on);
    lv_style_set_bg_color(&style_switch_on, lv_color_make(0, 0, 255));
    lv_style_set_bg_opa(&style_switch_on, LV_OPA_COVER);

    // Application du style initial
    lv_obj_add_style(progress_bar, &style_green, LV_PART_INDICATOR);
}

// Fonction de gestion de l'événement du bouton Start
void start_button_event_handler(lv_event_t *e) 
{
    radar_active = true;
    printf("Radar started\n");
}

// Fonction de gestion de l'événement du bouton Stop
void stop_button_event_handler(lv_event_t *e) 
{
    radar_active = false;
    buzzer = 0; // Assurer que le buzzer est éteint quand le radar est arrêté
    printf("Radar stopped\n");
}

// Fonction de gestion de l'événement du switch Buzzer On/Off
void buzzer_switch_event_handler(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    buzzer_enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);

    if (buzzer_enabled)
    {
        lv_label_set_text(buzzer_label, "Buzzer On");
        lv_obj_add_style(sw, &style_switch_on, 0);
        lv_obj_remove_style(sw, &style_switch_off, 0);
    }
    else
    {
        lv_label_set_text(buzzer_label, "Buzzer Off");
        lv_obj_add_style(sw, &style_switch_off, 0);
        lv_obj_remove_style(sw, &style_switch_on, 0);
        buzzer = 0; // Éteindre le buzzer immédiatement
    }
    printf("Buzzer %s\n", buzzer_enabled ? "enabled" : "disabled");
}

// Fonction de mise à jour de la barre de progression en fonction de la distance
void update_progress_bar(int dist)
{
    if (dist < 5) 
    {
        lv_bar_set_value(progress_bar, 60, LV_ANIM_OFF); // Remplir complètement la barre de progression
        lv_obj_remove_style(progress_bar, NULL, LV_PART_INDICATOR);
        lv_obj_add_style(progress_bar, &style_red, LV_PART_INDICATOR);
    } 
    else 
    {
        // Mise à jour de la valeur de la barre de progression
        lv_bar_set_value(progress_bar, dist, LV_ANIM_OFF);

        // Supprimer les styles précédents
        lv_obj_remove_style(progress_bar, NULL, LV_PART_INDICATOR);

        // Changer la couleur de la barre de progression en fonction de la distance
        if (dist <= 10) 
        {
            lv_obj_add_style(progress_bar, &style_red, LV_PART_INDICATOR);
        } 
        else if (dist <= 20) 
        {
            lv_obj_add_style(progress_bar, &style_orange, LV_PART_INDICATOR);
        } 
        else if (dist <= 30) 
        {
            lv_obj_add_style(progress_bar, &style_yellow, LV_PART_INDICATOR);
        } 
        else if (dist <= 40) 
        {
            lv_obj_add_style(progress_bar, &style_green, LV_PART_INDICATOR);
        } 
        else 
        {
            lv_obj_add_style(progress_bar, &style_transparent, LV_PART_INDICATOR);
        }
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
    if (!buzzer_enabled) 
    {
        buzzer = 0;
        return;
    }

    if (dist < 5) 
    {
        buzzer = 1;
    } 
    else if (dist <= 10) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(100ms);
        buzzer = 0;
    } 
    else if (dist <= 20) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(200ms);
        buzzer = 0;
    } 
    else if (dist <= 30) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(400ms);
        buzzer = 0;
    }  
    else if (dist <= 40) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(700ms);
        buzzer = 0;
    } 
    else 
    {
        buzzer = 0;
    }
}
