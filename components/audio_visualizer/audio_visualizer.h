#ifndef AUDIO_VISUALIZER_H
#define AUDIO_VISUALIZER_H

#include "esphome.h"
#include <lvgl.h>

namespace esphome {
namespace audio_visualizer {

class AudioVisualizer : public Component, public CustomLVGLWidget {
 public:
  AudioVisualizer();
  void set_width(int width) { this->width_ = width; }
  void set_height(int height) { this->height_ = height; }

  void setup() override;
  lv_obj_t *create_widget() override;
  void update() override;
  void draw() override;

 protected:
  int width_{200};
  int height_{100};
  lv_obj_t *obj_;
  int data[64]; // Tableau pour stocker les données du visualiseur
};

}  // namespace audio_visualizer
}  // namespace esphome

#endif

