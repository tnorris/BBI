#include "BBI.hpp"
#include "dsp/digital.hpp"


struct Dividah : Module {
  enum ParamIds {
    NUM_PARAMS
  };
  enum InputIds {
    BANG_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    ONE_OUTPUT,
    TWO_OUTPUT,
    THREE_OUTPUT,
    FOUR_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  float phase = 0.0;
  float blinkPhase = 0.0;
  int bangCounter = 0;
  int MAX_STEPS = 8;

  float bangStart = 0.0;
  float bangEnd = 0.0;

  SchmittTrigger bangTrigger;

  bool lastHigh = false;

  Dividah() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;

  // For more advanced Module features, read Rack's engine.hpp header file
  // - toJson, fromJson: serialization of internal data
  // - onSampleRateChange: event triggered by a change of sample rate
  // - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void Dividah::step() {
  bangTrigger.process(inputs[BANG_INPUT].value / 10.f);

  bool thisHigh = bangTrigger.isHigh();

  if(thisHigh && !lastHigh) {
    bangCounter += 1;
    bangCounter = bangCounter % MAX_STEPS;
  }

  outputs[ONE_OUTPUT].value = inputs[BANG_INPUT].value;

  if (bangCounter % 2 == 0) {
    outputs[TWO_OUTPUT].value = inputs[BANG_INPUT].value;
  } else {
    outputs[TWO_OUTPUT].value = 0.0f;
  }

  if (bangCounter % 4 == 0) {
    outputs[THREE_OUTPUT].value = inputs[BANG_INPUT].value;
  } else {
    outputs[THREE_OUTPUT].value = 0.0f;
  }

  if (bangCounter == 0) {
    outputs[FOUR_OUTPUT].value = inputs[BANG_INPUT].value;
  } else {
    outputs[FOUR_OUTPUT].value = 0.0f;
  }

  lastHigh = thisHigh;
}


struct DividahWidget : ModuleWidget {
  DividahWidget(Dividah *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/gray.svg")));
    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addOutput(Port::create<PJ301MPort>(Vec(10, 30), Port::OUTPUT, module, Dividah::ONE_OUTPUT));
    addOutput(Port::create<PJ301MPort>(Vec(10, 60), Port::OUTPUT, module, Dividah::TWO_OUTPUT));
    addOutput(Port::create<PJ301MPort>(Vec(10, 90), Port::OUTPUT, module, Dividah::THREE_OUTPUT));
    addOutput(Port::create<PJ301MPort>(Vec(10, 120), Port::OUTPUT, module, Dividah::FOUR_OUTPUT));

    addInput(Port::create<PJ301MPort>(Vec(10, 160), Port::INPUT, module, Dividah::BANG_INPUT));
  }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelDividah = Model::create<Dividah, DividahWidget>("BBI", "Dividah", "Dividah", CLOCK_MODULATOR_TAG);
