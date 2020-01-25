#include "load_material.hpp"

#include <string>

#include "shake/io/file_json.hpp"

#include "shake/content/content_manager.hpp"
//#include "shake/graphics/geometry/voxel_grid.hpp"
#include "shake/graphics/gl/gl_enum.hpp"

namespace shake {
namespace content {
namespace load {

//----------------------------------------------------------------
std::shared_ptr<graphics::Material> load_material( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto json_content = io::file::json::read( path );

    const auto shader_path = io::Path{ io::file::json::read_as<std::string>( json_content, "shader" ) };
    const auto shader = content_manager->get_or_load<graphics::Program>( shader_path );

    auto material = std::make_shared<graphics::Material>( shader );

    if ( io::file::json::has_key( json_content, "uniforms" ) )
    {
        const auto uniforms = json_content[ "uniforms" ];
        for ( const auto& uniform_json : uniforms.array_items() )
        {
            if ( uniform_json[ "type" ] == "texture" )
            {
                const auto texture_path = io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) };
                const auto file_extension = texture_path.get_file_extension();

                //if ( file_extension == ".vox" )
                //{
                //    const auto voxel_model = content_manager->get_or_load<graphics::VoxelGrid>( io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) } );
                //    const auto texture_unit_index = to_texture_unit_index( graphics::gl::NamedTextureUnit::Albedo );
                //    material->set_uniform( "u_sampler2", std::make_unique<graphics::UniformTexture>( voxel_model->get_palette(), texture_unit_index ) );
                //}
                //else 
                if ( file_extension == ".json" )
                {
                    const auto texture = content_manager->get_or_load<graphics::Texture>( texture_path );
                    const auto texture_unit_index = to_texture_unit_index( graphics::gl::NamedTextureUnit::Albedo );
                    //material->set_uniform( "u_sampler2", graphics::UniformTexture { texture, texture_unit_index } );
                }
                else
                {
                    CHECK_FAIL( "Unrecognised texture file extension: " + file_extension );
                }               
            }

            else if ( uniform_json[ "type" ] == "cube_map" )
            {
                const auto cube_map = content_manager->get_or_load<graphics::CubeMap>( io::Path{ io::file::json::read_as<std::string>( uniform_json, "path" ) } );
                const auto texture_unit_index = to_texture_unit_index( graphics::gl::NamedTextureUnit::Skybox );
                //material->set_uniform( "u_sampler_cube", graphics::UniformCubeMap { cube_map, texture_unit_index } );
            }
        }
    }

    return material;
}

} // namespace load
} // namespace content
} // namespace shake
