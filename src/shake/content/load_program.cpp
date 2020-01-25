#include "load_program.hpp"

#include "shake/io/file.hpp"
#include "shake/io/file_json.hpp"

#include "shake/graphics/material/shader.hpp"
#include "shake/graphics/material/program.hpp"

namespace shake {
namespace content {
namespace load {

//----------------------------------------------------------------
std::shared_ptr< graphics::Program > load_program( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto source           = io::file::read_as_string( path );
    const auto vertex_shader    = graphics::make_shader( graphics::gl::ShaderType::Vertex,   source );
    const auto fragment_shader  = graphics::make_shader( graphics::gl::ShaderType::Fragment, source );
    return std::make_shared< graphics::Program > ( graphics::make_program( vertex_shader, fragment_shader ) );
}

} // namespace load
} // namespace content
} // namespace shake
