#include "load_shader.hpp"

#include "shake/io/file.hpp"
#include "shake/io/file_json.hpp"

#include "shake/graphics/material/shader_preprocessor.hpp"

namespace shake {
namespace content {
namespace load {

//----------------------------------------------------------------
std::unique_ptr<graphics::Shader> load_shader(const io::Path& path)
{
    const auto source = io::file::read_as_string( path );
    return graphics::Shader::create_basic( source, source );
}

} // namespace load
} // namespace content
} // namespace shake
