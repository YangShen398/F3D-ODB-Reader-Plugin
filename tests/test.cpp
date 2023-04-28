#include <f3d/engine.h>
#include <f3d/image.h>
#include <iostream>

int main() {
  std::cout << " this line reached" << std::endl;
  f3d::engine::autoloadPlugins();
  f3d::engine::loadPlugin("../f3d-plugin-ODBReader.dll");

  // Create a f3d::engine
  f3d::engine eng(f3d::window::Type::NATIVE_OFFSCREEN);

  // Add a file to the loader and load it
  eng.getLoader().loadGeometry("Job-1.odb");
  // eng.getLoader().addFile("Job-1.odb").loadFile();
  // Set the window size and render to an image
  constexpr unsigned int width = 300;
  constexpr unsigned int height = 300;
  f3d::image img = eng.getWindow().setSize(width, height).renderToImage();
  f3d::image baseline("./Job-1-baseline.png");
  if (img != baseline) {
    std::cerr << "Generated image is different from the baseline" << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
