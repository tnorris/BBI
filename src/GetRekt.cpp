#include "BBI.hpp"
#include "dsp/digital.hpp"
#include <stdlib.h>


struct GetRekt : Module {
  enum ParamIds {
    NUM_PARAMS
  };
  enum InputIds {
    A_IN,
    B_IN,
    C_IN,
    D_IN,
    NUM_INPUTS
  };
  enum OutputIds {
    A_OUT,
    B_OUT,
    C_OUT,
    D_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  GetRekt() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;
  void compare(InputIds, OutputIds);
};

void GetRekt::compare(InputIds in, OutputIds out) {
  if(inputs[in].value < 0 ) {
    outputs[out].value = -1 * inputs[in].value;
  } else {
    outputs[out].value = inputs[in].value;
  }
}

void GetRekt::step() {
  compare(A_IN, A_OUT);
  compare(B_IN, B_OUT);
  compare(C_IN, C_OUT);
  compare(D_IN, D_OUT);
}


struct GetRektWidget : ModuleWidget {
  GetRektWidget(GetRekt *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/getrekt.svg")));
    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    int s = 30;
    int spacing = s/2;
    addInput(Port::create<PJ301MPort>(Vec(3, s), Port::INPUT, module, GetRekt::A_IN));
    addOutput(Port::create<PJ301MPort>(Vec(3, s*2), Port::OUTPUT, module, GetRekt::A_OUT));

    addInput(Port::create<PJ301MPort>(Vec(3, s*3+spacing), Port::INPUT, module, GetRekt::B_IN));
    addOutput(Port::create<PJ301MPort>(Vec(3, s*4+spacing), Port::OUTPUT, module, GetRekt::B_OUT));

    addInput(Port::create<PJ301MPort>(Vec(3, s*5+(2*spacing)), Port::INPUT, module, GetRekt::C_IN));
    addOutput(Port::create<PJ301MPort>(Vec(3, s*6+(2*spacing)), Port::OUTPUT, module, GetRekt::C_OUT));

    addInput(Port::create<PJ301MPort>(Vec(3, s*7+(3*spacing)), Port::INPUT, module, GetRekt::D_IN));
    addOutput(Port::create<PJ301MPort>(Vec(3, s*8+(3*spacing)), Port::OUTPUT, module, GetRekt::D_OUT));

  }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelGetRekt = Model::create<GetRekt, GetRektWidget>("BBI", "GetRekt", "GetRekt", LOGIC_TAG);
