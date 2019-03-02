#ifndef LOAD_CUBE_MAP_HPP
#define LOAD_CUBE_MAP_HPP

#include <memory>

#include "shake/graphics/material/cube_map.hpp"
#include "shake/io/path.hpp"

namespace shake {
namespace content {
namespace load {

std::shared_ptr<graphics::CubeMap> load_cube_map( const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_CUBE_MAP_HPP
