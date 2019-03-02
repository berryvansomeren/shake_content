#ifndef LOAD_SHADER_HPP
#define LOAD_SHADER_HPP

#include <memory>

#include "shake/io/path.hpp"

#include "shake/graphics/material/shader.hpp"

namespace shake {
namespace content {
namespace load {

std::unique_ptr<graphics::Shader> load_shader ( const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_SHADER_HPP
