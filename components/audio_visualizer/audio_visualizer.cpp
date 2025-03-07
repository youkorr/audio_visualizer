#include "audio_visualizer.h"

namespace esphome {
namespace audio_visualizer {

AudioVisualizer::AudioVisualizer() : PollingComponent(20ms) {}

void AudioVisualizer::setup() {
  lv_obj_t *obj = this->create_widget();
  lv_obj_set_size(obj, this->width_, this->height_);
  lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN); // Fond noir
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
  this->obj_ = obj;

  // Initialiser les données avec des valeurs aléatoires
  for (int i = 0; i < 64; ++i) {
    data[i] = rand() % (this->height_ / 2) + (this->height_ / 2); // Initialisation haute
  }
}

lv_obj_t *AudioVisualizer::create_widget() {
  return lv_obj_create(lv_scr_act(), NULL);
}

void AudioVisualizer::update() {
  // Mettre à jour les données avec des valeurs aléatoires
  for (int i = 0; i < 64; ++i) {
    int change = (rand() % 11) - 5; // Moins de changement
    data[i] += change;

    if (data[i] < this->height_ / 2) data[i] = this->height_ / 2; // Limite basse
    if (data[i] > this->height_) data[i] = this->height_; // Limite haute
  }
  lv_obj_invalidate(this->obj_);
}

void AudioVisualizer::draw() {
  int num_bars = 64;
  int bar_width = (this->width_ - num_bars + 1) / num_bars;
  int spacing = 1;

  for (int i = 0; i < num_bars; ++i) {
    lv_coord_t x1 = i * (bar_width + spacing);
    lv_coord_t y1 = this->height_ - data[i];
    lv_coord_t x2 = x1 + bar_width;
    lv_coord_t y2 = this->height_;

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);

    // Dégradé de couleur (bleu -> violet -> rouge)
    lv_color_t color;
    if (i < num_bars / 3) {
      color = lv_color_mix(LV_COLOR_BLUE, LV_COLOR_PURPLE, (i * 255) / (num_bars / 3));
    } else if (i < 2 * num_bars / 3) {
      color = lv_color_mix(LV_COLOR_PURPLE, LV_COLOR_RED, ((i - num_bars / 3) * 255) / (num_bars / 3));
    } else {
      color = lv_color_mix(LV_COLOR_RED, LV_COLOR_WHITE, ((i - 2 * num_bars / 3) * 255) / (num_bars / 3)); // Rouge -> Blanc
    }
    rect_dsc.bg_color = color;
    rect_dsc.radius = 0;

    lv_area_t coords;
    coords.x1 = x1;
    coords.y1 = y1;
    coords.x2 = x2;
    coords.y2 = y2;

    lv_draw_rect(&coords, &rect_dsc);

    // Points blancs
    if (rand() % 15 == 0) { // Moins de points
      lv_draw_rect_dsc_t point_dsc;
      lv_draw_rect_dsc_init(&point_dsc);
      point_dsc.bg_color = LV_COLOR_WHITE;
      point_dsc.radius = LV_RADIUS_CIRCLE;
      lv_area_t point_coords;
      point_coords.x1 = x1 + rand() % bar_width;
      point_coords.y1 = y1 - (rand() % (this->height_ / 10) + 3); // Plus près des barres
      point_coords.x2 = point_coords.x1 + 2; // Plus petits
      point_coords.y2 = point_coords.y1 + 2;
      lv_draw_rect(&point_coords, &point_dsc);
    }
  }
}

}  // namespace audio_visualizer
}  // namespace esphome

