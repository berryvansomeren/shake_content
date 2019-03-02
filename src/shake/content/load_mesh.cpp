#include "load_mesh.hpp"

#include "shake/content/content_manager.hpp"

#include "shake/core/math/math.hpp"

#include "shake/io/file_json.hpp"

namespace shake {
namespace content {
namespace load {

////----------------------------------------------------------------
//std::shared_ptr<shake::graphics::Mesh> load_mesh(const io::Path& path)
//{
//    const auto content  = io::file::json::read( path );
//    const auto vertices = io::file::json::read_as<std::vector<float>>( content, {"vertices"} );
//    const auto indices  = math::integer_sequence<uint32_t>( vertices.size() / 6 ); // 8 floats per vertex
//    return std::make_shared<graphics::Mesh>( vertices, indices );
//}

} // namespace load
} // namespace content
} // namespace shake
