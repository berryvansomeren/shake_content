#include "load_material.hpp"

#include <string>

#include "shake/io/file_json.hpp"

#include "shake/content/content_manager.hpp"

namespace shake {
namespace content {
namespace load {

//----------------------------------------------------------------
std::shared_ptr<graphics::Material> load_material(const io::Path& path )
{
    auto& content_manager = ContentManager::get_instance();

    const auto json_content = io::file::json::read( path );

    const auto shader_path = io::Path{ io::file::json::read_as<std::string>( json_content, "shader" ) };
    const auto shader = content_manager.get_or_load_shader( shader_path );

    auto material = std::make_shared<graphics::Material>( shader );

    if ( io::file::json::has_key( json_content, "uniforms" ) )
    {
        const auto uniforms = json_content[ "uniforms" ];
        for ( const auto& uniform_json : uniforms.array_items() )
        {
            if ( uniform_json[ "type" ] == "texture" )
            {
                const auto voxel_model = content_manager.get_or_load_voxel_grid( io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) } );
                material->set_uniform( "u_sampler2", std::make_unique<graphics::UniformTexture>( voxel_model->get_palette(), graphics::TextureUnit::Albedo ) );


//                const auto texture_path = io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) };
//                const auto texture = content_manager.get_or_load_texture( texture_path );
//                material->set_uniform( "u_sampler2", std::make_unique<graphics::UniformTexture>( texture, graphics::Texture::Unit::Albedo ) );
            }

            else if ( uniform_json[ "type" ] == "cube_map" )
            {
                const auto cube_map = content_manager.get_or_load_cube_map( io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) } );
                material->set_uniform( "u_sampler_cube", std::make_unique<graphics::UniformCubeMap>( cube_map, graphics::TextureUnit::Skybox ) );
            }
        }
    }

    return material;
}

} // namespace load
} // namespace content
} // namespace shake
