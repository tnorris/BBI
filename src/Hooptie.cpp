#include "BBI.hpp"
#include "dsp/digital.hpp"
#include <stdlib.h>

#define ROWS 8


struct Hooptie : Module {
  enum ParamIds {
    ENUMS(GATE_CHANCE, ROWS),
    DEBUG_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    BANG_INPUT,
    RST_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    ENUMS(SNOWFLAKE_BANG_OUT, ROWS),
    BANG_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    ENUMS(GATE_LIGHTS, ROWS),
    NUM_LIGHTS
  };

  int bangCounter = 0;

  int knobStyle = 0;

  SchmittTrigger bangTrigger;
  SchmittTrigger rstTrigger;

  bool lastHigh = false;

  Hooptie() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;

  json_t *toJson() override {
    json_t *root = json_object();
    json_object_set_new(root, "knobStyle", json_integer(knobStyle));
    return root;
  }

  void fromJson(json_t *root) override {
    json_t *knob_style = json_object_get(root, "knobStyle");
    knobStyle = json_integer_value(knob_style);
  }

  float diceRoll = 0.f;

  // For more advanced Module features, read Rack's engine.hpp header file
  // - toJson, fromJson: serialization of internal data
  // - onSampleRateChange: event triggered by a change of sample rate
  // - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

void Hooptie::step() {
  bangTrigger.process(inputs[BANG_INPUT].value / 10.f );

  rstTrigger.process(inputs[RST_INPUT].value / 10.f);
  if(rstTrigger.isHigh()) {
    bangCounter = 0;
  }


  bool thisHigh = bangTrigger.isHigh();

  if(thisHigh && !lastHigh) {
    bangCounter += 1;
    bangCounter = bangCounter % ROWS;

    diceRoll = randomUniform() * 10.f;
  }

  // maybe send a message out
  // if the roll < chance_knob[bangCounter]: send BANG_INPUT through
  if(diceRoll < params[GATE_CHANCE+bangCounter].value) {
    outputs[SNOWFLAKE_BANG_OUT+bangCounter].value = inputs[BANG_INPUT].value;
    outputs[BANG_OUT].value = inputs[BANG_INPUT].value;
  } else {
    outputs[SNOWFLAKE_BANG_OUT+bangCounter].value = 0.f;
    outputs[BANG_OUT].value = 0.f;
  }

  for(int i=0; i < ROWS; i++) {
    // light up the bangCounter LED
    lights[GATE_LIGHTS + i].setBrightness((i == bangCounter) ? (1.f) : (0.f));
  }

  lastHigh = thisHigh;
}

struct HooptieWidget : ModuleWidget {
  HooptieWidget(Hooptie *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/hooptie.svg")));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    for (int i = 0; i < ROWS; i++) {
      // add the chance knob
      addParam(ParamWidget::create<BBIOrangeKnob>(Vec(5, 30+(i*35)), module, Hooptie::GATE_CHANCE+i, 0.0f, 10.0f, 0.0f));

      // add the LEDs
      addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(70, 40+(i*35)), module, Hooptie::GATE_LIGHTS + i));

      // add the gate out
      addOutput(Port::create<PJ301MPort>(Vec(40, 32+(i*35)), Port::OUTPUT, module, Hooptie::SNOWFLAKE_BANG_OUT+i));
    }

    // add clock in (bang)
    addInput(Port::create<PJ301MPort>(Vec(4, 30+(ROWS*35)), Port::INPUT, module, Hooptie::BANG_INPUT));

    // add the master bang
    addOutput(Port::create<PJ301MPort>(Vec(34, 30+(ROWS*35)), Port::OUTPUT, module, Hooptie::BANG_OUT));

    // reset in
    addInput(Port::create<PJ301MPort>(Vec(65, 30+(ROWS*35)), Port::INPUT, module, Hooptie::RST_INPUT));


  }

//  void appendContextMenu(Menu *menu) override { 
//    Hooptie *module = dynamic_cast<Hooptie*>(this->module);
// }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHooptie = Model::create<Hooptie, HooptieWidget>("BBI", "Hooptie", "Hooptie", CLOCK_MODULATOR_TAG);
