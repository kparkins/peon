#include "GLVideoMode.h"

Peon::GLVideoMode::GLVideoMode(unsigned int w, unsigned int h, int refresh)
    : width(w), height(h), refreshRate(refresh) {}

Peon::GLVideoMode::~GLVideoMode() {}