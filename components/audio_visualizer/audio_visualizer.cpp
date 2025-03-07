#include "audio_visualizer.h"

namespace esphome {
namespace audio_visualizer {

AudioVisualizer::AudioVisualizer() {}

void AudioVisualizer::setup() {
  lv_obj_t *obj = this->create_widget();
  lv_obj_set_size(obj, this->width_, this->height_);
  lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN); // Fond noir
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
  this->obj_ = obj;

  // Initialiser les données avec des valeurs aléatoires
  for (int i = 0; i < 64; ++i) {
    data[i] = rand() % this->height_;
  }
}

lv_obj_t *AudioVisualizer::create_widget() {
  return lv_obj_create(lv_scr_act(), NULL);
}

void AudioVisualizer::update() {
  // Mettre à jour les données avec des valeurs aléatoires
  for (int i = 0; i < 64; ++i) {
    // Générer une nouvelle valeur, mais l'adoucir un peu
    int change = (rand() % 21) - 10; // Valeur aléatoire entre -10 et 10
    data[i] += change;

    // Limiter les valeurs pour qu'elles restent dans la plage valide
    if (data[i] < 0) data[i] = 0;
    if (data[i] > this->height_) data[i] = this->height_;
  }
  lv_obj_invalidate(this->obj_); // Force le redessin
}

void AudioVisualizer::draw() {
  // Dessiner les barres en utilisant les données aléatoires
  int num_bars = 64; // Nombre de barres (correspond à la taille du tableau `data`)
  int bar_width = this->width_ / num_bars;
  int spacing = 0; // Pas d'espacement entre les barres pour ressembler à l'image

  for (int i = 0; i < num_bars; ++i) {
    // Calculer les coordonnées de la barre
    lv_coord_t x1 = i * (bar_width + spacing);
    lv_coord_t y1 = this->height_ - data[i]; // On part du bas
    lv_coord_t x2 = x1 + bar_width;
    lv_coord_t y2 = this->height_;

    // Définir le style de la barre (dégradé de couleur)
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);

    // Créer un dégradé de couleur (bleu -> violet -> rouge)
    lv_color_t color;
    if (i < num_bars / 2) {
      // Bleu -> Violet
      color = lv_color_mix(LV_COLOR_BLUE, LV_COLOR_PURPLE, (i * 255) / (num_bars));
    } else {
      // Violet -> Rouge
      color = lv_color_mix(LV_COLOR_PURPLE, LV_COLOR_RED, ((i - num_bars / 2) * 255) / (num_bars));
    }
    rect_dsc.bg_color = color;
    rect_dsc.radius = 0; // Pas d'arrondi

    // Dessiner la barre
    lv_area_t coords;
    coords.x1 = x1;
    coords.y1 = y1;
    coords.x2 = x2;
    coords.y2 = y2;

    lv_draw_rect(&coords, &rect_dsc);
  }
}

}  // namespace audio_visualizer
}  // namespace esphome
