#include "load_sprite.hpp"

#include "shake/io/file_json.hpp"
#include "shake/content/content_manager.hpp"
#include "shake/graphics/material/texture.hpp"

namespace shake {
namespace content {
namespace load {

//----------------------------------------------------------------
std::unique_ptr<graphics::Sprite> load_sprite ( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto content      = io::file::json::read( path );

    const auto texture_path = io::file::json::read_as<std::string>( content, { "texture" } );
    const auto texture      = content_manager->get_or_load<graphics::Texture>( io::Path{ texture_path } );

    const auto width        = io::file::json::read_as<float>( content, { "width"  } );
    const auto height       = io::file::json::read_as<float>( content, { "height" } );

    return std::make_unique<graphics::Sprite> ( width, height, texture );
}

} // namespace load
} // namespace content
} // namespace shake
