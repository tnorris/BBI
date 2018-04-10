#include "BBI.hpp"
#include "dsp/digital.hpp"
#include <stdlib.h>


struct Comparatah : Module {
  enum ParamIds {
    B_KNOB,
    D_KNOB,
    F_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    A,
    C,
    E,
    NUM_INPUTS
  };
  enum OutputIds {
    TOP_OUT,
    BOTTOM_OUT,
    LOWER_TOP_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  Comparatah() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;
  void compare(OutputIds, InputIds, ParamIds);
};

void Comparatah::compare(OutputIds outie, InputIds a, ParamIds b_knob) {
  if(inputs[a].value > params[b_knob].value) {
    outputs[outie].value = 10.f;
  } else {
    outputs[outie].value = 0.f;
  }
}

void Comparatah::step() {
  compare(TOP_OUT, A, B_KNOB);
  compare(BOTTOM_OUT, C, D_KNOB);
  compare(LOWER_TOP_OUT, E, F_KNOB);
}


struct ComparatahWidget : ModuleWidget {
  ComparatahWidget(Comparatah *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/comparatah.svg")));
    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    int s = 30;


    addInput(Port::create<PJ301MPort>(Vec(5, s), Port::INPUT, module, Comparatah::A));
    addParam(ParamWidget::create<RoundBlackKnob>(Vec(5, s*2), module, Comparatah::B_KNOB, 0.0f, 10.0f, 0.0f));
    addOutput(Port::create<PJ301MPort>(Vec(5, s*3), Port::OUTPUT, module, Comparatah::TOP_OUT));

    addInput(Port::create<PJ301MPort>(Vec(5, s*4.5), Port::INPUT, module, Comparatah::C));
    addParam(ParamWidget::create<RoundBlackKnob>(Vec(5, s*5.5), module, Comparatah::D_KNOB, 0.0f, 10.0f, 0.0f));
    addOutput(Port::create<PJ301MPort>(Vec(5, s*6.5), Port::OUTPUT, module, Comparatah::BOTTOM_OUT));

    addInput(Port::create<PJ301MPort>(Vec(5, s*8), Port::INPUT, module, Comparatah::E));
    addParam(ParamWidget::create<RoundBlackKnob>(Vec(5, s*9), module, Comparatah::F_KNOB, 0.0f, 10.0f, 0.0f));
    addOutput(Port::create<PJ301MPort>(Vec(5, s*10), Port::OUTPUT, module, Comparatah::LOWER_TOP_OUT));
  }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelComparatah = Model::create<Comparatah, ComparatahWidget>("BBI", "Comparatah", "Comparatah", LOGIC_TAG);
