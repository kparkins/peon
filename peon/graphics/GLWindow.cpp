//
// Created by Kyle on 5/11/2016.
//

#include "GLWindow.h"

Peon::GLWindow::GLWindow(const GLWindowCreateInfo & info) {

}

Peon::GLWindow::~GLWindow() {
  
}

bool Peon::GLWindow::ShouldClose() {
    return false;
}

void Peon::GLWindow::SetShouldClose(bool close) {
}

void Peon::GLWindow::SetTitle(const string & title) {
}

void Peon::GLWindow::SetIcon(const PixelData & icon) {
}

ivec2 Peon::GLWindow::GetPosition() const {
    return ivec2();
}

void Peon::GLWindow::SetPosition(const ivec2 & position) {
}

ivec2 Peon::GLWindow::GetSize() const {
    return ivec2();
}

void Peon::GLWindow::SetSize(const ivec2 & dimensions) {
}

void Peon::GLWindow::SetSizeLimits(const ivec2 & min, const ivec2 & max) {
}

void Peon::GLWindow::SetAspectRatio(unsigned int numerator, unsigned int denominator) {
}

ivec2 Peon::GLWindow::GetFramebufferSize() const {
    return ivec2();
}

void Peon::GLWindow::Iconify() {
}

void Peon::GLWindow::Restore() {
}

void Peon::GLWindow::Maximize() {
}

void Peon::GLWindow::Show() {
}

void Peon::GLWindow::Hide() {
}

void Peon::GLWindow::Focus() {
}

void Peon::GLWindow::SetFullScreen(bool on) {
}

void Peon::GLWindow::SetVerticalSync(bool on) {
}

Peon::Shared<Peon::GLDisplayDevice> Peon::GLWindow::GetDisplayDevice() {
    return Shared<GLDisplayDevice>();
}
