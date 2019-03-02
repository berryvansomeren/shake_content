#ifndef LOAD_VOXEL_GRID_HPP
#define LOAD_VOXEL_GRID_HPP

#include "shake/io/path.hpp"

#include "shake/graphics/geometry/voxel_grid.hpp"

namespace shake {
namespace content {
namespace load {

std::unique_ptr<graphics::VoxelGrid> load_voxel_grid ( const io::Path& path );

} // namespace load
} // namespace content
} // namespace shake

#endif // LOAD_VOXEL_GRID_HPP
