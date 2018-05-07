#include "rack.hpp"


using namespace rack;


// Forward-declare the Plugin, defined in Template.cpp
extern Plugin *plugin;

struct BBIOrangeKnob : RoundKnob {
  BBIOrangeKnob() {
    setSVG(SVG::load(assetPlugin(plugin, "res/orangeknob.svg")));
  }
};

struct BBIBigKnob : RoundKnob {
  BBIBigKnob() {
    setSVG(SVG::load(assetPlugin(plugin, "res/bigknob.svg")));
    shadow->opacity = 0.f;
  }
};

// Forward-declare each Model, defined in each module source file
extern Model *modelComparatah;
extern Model *modelHooptie;
extern Model *modelDividah;
extern Model *modelGetRekt;
