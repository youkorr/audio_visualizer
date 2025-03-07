# Fichier external_components/audio_visualizer/audio_visualizer.h

#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/display/display.h"
#include "esphome/components/lvgl/lvgl.h"

namespace esphome {
namespace audio_visualizer {

class AudioVisualizer : public Component {
 public:
  AudioVisualizer() {}

  void setup() override {
    // Initialisation du composant
    ESP_LOGD("audio_visualizer", "Initialisation du visualiseur audio");
    
    // L'écran sera configuré lors de l'appel à set_parent_screen
  }
  
  // Méthode pour définir l'écran parent (appelée depuis la configuration LVGL)
  void set_parent_screen(lv_obj_t *parent) {
    // Créer l'objet conteneur sur l'écran parent
    this->container_ = lv_obj_create(parent);
    lv_obj_set_size(this->container_, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(this->container_, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_clear_flag(this->container_, LV_OBJ_FLAG_SCROLLABLE);
    
    // Nombre de barres pour la visualisation
    const int BAR_COUNT = 32;
    const int BAR_WIDTH = 10;
    const int BAR_GAP = 2;
    const int MAX_BAR_HEIGHT = 180;
    
    // Créer les barres
    for (int i = 0; i < BAR_COUNT; i++) {
      lv_obj_t* bar = lv_obj_create(this->container_);
      lv_obj_set_size(bar, BAR_WIDTH, 1);  // Hauteur initiale minimale
      
      // Position de la barre
      int x_pos = 20 + i * (BAR_WIDTH + BAR_GAP);
      int y_pos = 220;  // Position en bas
      lv_obj_set_pos(bar, x_pos, y_pos);
      
      // Couleur des barres - gradient de bleu à violet
      lv_color_t bar_color;
      if (i < BAR_COUNT * 0.7) {
        bar_color = lv_color_hex(0x0088FF);  // Bleu
      } else if (i < BAR_COUNT * 0.9) {
        bar_color = lv_color_hex(0x8844FF);  // Violet
      } else {
        bar_color = lv_color_hex(0xFF4488);  // Rose/Rouge
      }
      
      lv_obj_set_style_bg_color(bar, bar_color, LV_PART_MAIN);
      lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
      
      // Stocker la barre dans notre tableau
      this->bars_.push_back(bar);
      this->heights_.push_back(1);
      this->target_heights_.push_back(1);
    }
  }

  void loop() override {
    // La mise à jour est gérée par le composant interval externe
  }

  // Méthode pour mettre à jour l'animation, appelée par l'interval externe
  void update_animation() {
    this->generate_random_heights();
    this->update_bars();
  }

 private:
  lv_obj_t *container_;
  std::vector<lv_obj_t*> bars_;
  std::vector<int> heights_;
  std::vector<int> target_heights_;
  
  // Génère des hauteurs aléatoires pour simuler l'animation audio
  void generate_random_heights() {
    const int MAX_HEIGHT = 180;
    const int MIN_HEIGHT = 5;
    
    // Effet de déplacement d'onde
    for (size_t i = 0; i < target_heights_.size(); i++) {
      // Générer une nouvelle hauteur cible avec une distribution qui crée un effet d'onde
      float position = static_cast<float>(i) / target_heights_.size();
      float base_height = sinf(position * 3.14159f * 2.0f + millis() / 1000.0f) * 0.5f + 0.5f;
      
      // Ajouter un peu de randomisation
      float random_factor = static_cast<float>(esp_random() % 100) / 100.0f * 0.3f;
      float height_factor = base_height * 0.7f + random_factor;
      
      // Calculer la hauteur finale
      int new_height = MIN_HEIGHT + static_cast<int>(height_factor * (MAX_HEIGHT - MIN_HEIGHT));
      
      // Définir la hauteur cible
      target_heights_[i] = new_height;
    }
  }
  
  // Met à jour l'affichage des barres
  void update_bars() {
    const float SMOOTH_FACTOR = 0.3f;  // Facteur de lissage pour l'animation
    
    for (size_t i = 0; i < bars_.size(); i++) {
      // Interpolation lisse vers la hauteur cible
      heights_[i] = heights_[i] + (target_heights_[i] - heights_[i]) * SMOOTH_FACTOR;
      
      // Mettre à jour la taille de la barre
      lv_obj_set_height(bars_[i], heights_[i]);
      
      // Repositionner le bas de la barre
      lv_obj_set_y(bars_[i], 220 - heights_[i]);
      
      // Ajouter un petit effet de brillance au-dessus des barres hautes
      if (heights_[i] > 100) {
        int bright_intensity = (heights_[i] - 100) * 2;
        if (bright_intensity > 255) bright_intensity = 255;
        
        // Si nous avions un système pour ajouter des points brillants, nous le ferions ici
        // Pour l'instant, nous modifions simplement la couleur de la barre pour les barres hautes
        lv_color_t bright_color = lv_color_make(bright_intensity, bright_intensity, 255);
        lv_obj_set_style_bg_color(bars_[i], bright_color, LV_PART_MAIN);
      } else {
        // Restaurer la couleur normale
        lv_color_t bar_color;
        if (i < bars_.size() * 0.7) {
          bar_color = lv_color_hex(0x0088FF);  // Bleu
        } else if (i < bars_.size() * 0.9) {
          bar_color = lv_color_hex(0x8844FF);  // Violet
        } else {
          bar_color = lv_color_hex(0xFF4488);  // Rose/Rouge
        }
        lv_obj_set_style_bg_color(bars_[i], bar_color, LV_PART_MAIN);
      }
    }
  }
};

}  // namespace audio_visualizer
}  // namespace esphome
