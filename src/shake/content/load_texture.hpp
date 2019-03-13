#ifndef LOAD_TEXTURE_HPP
#define LOAD_TEXTURE_HPP

#include <memory>

#include "shake/io/path.hpp"
#include "shake/graphics/material/texture.hpp"

namespace shake {
namespace content {

class ContentManager;

namespace load {

std::shared_ptr<graphics::Texture> load_texture ( shake::content::ContentManager* content_manager, const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_TEXTURE_HPP
