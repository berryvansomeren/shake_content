#ifndef LOAD_FONT_HPP
#define LOAD_FONT_HPP

#include <memory>

#include "shake/graphics/assets/font.hpp"

#include "shake/io/path.hpp"

namespace shake {
namespace content {
namespace load {

void init_font_loader();

std::shared_ptr<graphics::Font> load_font( const io::Path& path );

void destroy_font_loader();

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_FONT_HPP
