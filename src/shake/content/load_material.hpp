#ifndef LOAD_MATERIAL_HPP
#define LOAD_MATERIAL_HPP

#include <memory>

#include "shake/io/path.hpp"
#include "shake/graphics/material/material.hpp"

namespace shake {
namespace content {

class ContentManager;

namespace load {

std::shared_ptr<graphics::Material> load_material( shake::content::ContentManager* content_manager, const io::Path& path);

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_MATERIAL_HPP
