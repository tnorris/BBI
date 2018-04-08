#include "BBI.hpp"
#include "dsp/digital.hpp"
#include <stdlib.h>

#define ROWS 8

struct Hooptie : Module {
  enum ParamIds {
    ENUMS(GATE_CHANCE, ROWS),
    MODE,
    NUM_PARAMS
  };
  enum InputIds {
    BANG_INPUT, // I came from Max/MSP (why you'll see 'bang' when I mean 'gate'/'trigger')
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

  enum sequenceDirection { forward, back, ping, random, shotgun, single };
  sequenceDirection direction = back;

  int bangCounter = 0;

  SchmittTrigger bangTrigger;
  SchmittTrigger rstTrigger;
  SchmittTrigger modeTrigger;

  bool lastHigh = false;

  Hooptie() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;

  void nextSequenceStep();
  void notify();

  float diceRoll = 0.f;

  // For more advanced Module features, read Rack's engine.hpp header file
  // - toJson, fromJson: serialization of internal data
  // - onSampleRateChange: event triggered by a change of sample rate
  // - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

void Hooptie::notify() {
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
}

void Hooptie::nextSequenceStep() {
  bangTrigger.process(inputs[BANG_INPUT].value / 10.f );

  rstTrigger.process(inputs[RST_INPUT].value / 10.f);
  if(rstTrigger.isHigh()) {
    bangCounter = 0;
  }

  bool thisHigh = bangTrigger.isHigh();

  if(thisHigh && !lastHigh) {
    switch(direction) {
      case forward:
        bangCounter += 1;
        bangCounter = bangCounter % ROWS;
        break;
      case back:
        bangCounter -= 1;
        if(bangCounter < 0) { bangCounter = ROWS-1; }
        break;
      case ping:
        break;
      case random:
        break;
      case shotgun:
        break;
      case single:
        if(bangCounter < ROWS) { bangCounter += 1; }
        break;
    }

    diceRoll = randomUniform() * 10.f;
  }

  lastHigh = thisHigh;
}

void Hooptie::step() {
  if(modeTrigger.process(params[MODE].value)) {
    switch(direction) {
      case forward:
        direction = back;
        break;
      case back:
        direction = ping;
        break;
      case ping:
        //direction = random;
        // skip a bit, brother
        direction = single;
        break;
      case random:
        direction = shotgun;
        break;
      case shotgun:
        direction = single;
        break;
      case single:
        direction = forward;
        break;
      }
  }
  nextSequenceStep();
  notify();
}

struct HooptieWidget : ModuleWidget {
  HooptieWidget(Hooptie *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/hooptie.svg")));
    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(ParamWidget::create<LEDButton>(Vec(50, 5), module, Hooptie::MODE, 0.0f, 1.0f, 0.0f));


    for (int i = 0; i < ROWS; i++) {
      // add the chance knob
      addParam(ParamWidget::create<RoundBlackKnob>(Vec(5, 30+(i*35)), module, Hooptie::GATE_CHANCE+i, 0.0f, 10.0f, 0.0f));

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
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelHooptie = Model::create<Hooptie, HooptieWidget>("BBI", "Hooptie", "Hooptie", CLOCK_MODULATOR_TAG);
