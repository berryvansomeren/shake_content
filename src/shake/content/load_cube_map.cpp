#include "load_cube_map.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <array>
#include <string>
#include <vector>

#include "shake/core/std/map.hpp"
#include "shake/content/content_manager.hpp"
#include "shake/io/file_json.hpp"
#include "shake/graphics/material/texture_parameters.hpp"

namespace shake {
namespace content {
namespace load {

namespace { // anonymous

} // namespace anonymous

std::shared_ptr<graphics::CubeMap> load_cube_map( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto json = io::file::json::read( path );

    const auto cube_face_keys = std::vector<std::string> { "right", "left", "top", "bottom", "front", "back" };

    const auto image_format_str         = io::file::json::read_as<std::string>  ( json, { "image_format"        } );
    const auto texture_format_str       = io::file::json::read_as<std::string>  ( json, { "texture_format"      } );
    const auto interpolation_mode_str   = io::file::json::read_as<std::string>  ( json, { "interpolation_mode"  } );
    const auto generate_mipmaps         = io::file::json::read_as<bool>         ( json, { "generate_mip_maps"   } );

    // get data from file in memory
    int bytes_per_pixel_forced  { 3 };
    auto image_data = graphics::CubeMap::ImageData { };
    auto bytes_per_pixel_values = std::array<int, graphics::CubeMap::n_cube_faces> { };

    for ( std::size_t cube_face_index = 0; cube_face_index < graphics::CubeMap::n_cube_faces; ++cube_face_index )
    {
        const auto cube_face_texture_path = io::Path( io::file::json::read_as<std::string>( json, { cube_face_keys[ cube_face_index ] } ) );
        const auto cube_face_texture_full_path = content_manager->get_full_path( cube_face_texture_path );

        auto& image_info = image_data[ cube_face_index ];

        image_info.ptr = static_cast<uint8_t*>( stbi_load
        (
            cube_face_texture_full_path.c_str(),
            &image_info.width,
            &image_info.height,
            &bytes_per_pixel_values [ cube_face_index ],
            bytes_per_pixel_forced
        ) );
    }

    // load texture on gpu
    const auto texture = std::make_shared<graphics::CubeMap>
    (
        image_data,
        graphics::to_texture_format( texture_format_str ),
        graphics::to_filter( interpolation_mode_str )
    );

    // clear memory from cpu
    for ( const auto& image_info : image_data )
    {
        stbi_image_free( image_info.ptr );
    }

    return texture;
}

} // namespace load
} // namespace content
} // namespace shake
