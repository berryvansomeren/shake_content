#ifndef LOAD_SPRITE_HPP
#define LOAD_SPRITE_HPP

#include <memory>

#include "shake/io/path.hpp"

#include "shake/graphics/assets/sprite.hpp"

namespace shake {
namespace content {
namespace load {

std::unique_ptr<graphics::Sprite> load_sprite ( const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_SPRITE_HPP
