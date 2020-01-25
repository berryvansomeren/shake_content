#include "load_font.hpp"

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shake/core/contracts/contracts.hpp"
#include "shake/core/math/math.hpp"

#include "shake/content/content_manager.hpp"

#include "shake/io/file.hpp"
#include "shake/io/file_json.hpp"

#include "shake/graphics/gl/gl_enum.hpp"
#include "shake/graphics/gl/gl_int.hpp"
#include "shake/graphics/assets/font.hpp"
#include "shake/graphics/material/material.hpp"
#include "shake/graphics/geometry/geometry_2d.hpp"

#include "shake/graphics/assets/sprite.hpp"

namespace shake {
namespace content {
namespace load {

namespace { // anonymous

FT_Library ft { };

graphics::Font::CharacterMap load_character_map( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const io::Path full_path { content_manager->get_full_path( path ) };

    FT_Face face { };
    CHECK_EQ( FT_New_Face( ft, full_path.c_str(), 0, &face ), 0, "Could not load font." );

    FT_Set_Pixel_Sizes( face, 0, 64 );

    graphics::gl::pixel_store( graphics::gl::PixelStorageMode::UnpackAlignment, graphics::gl::Size { 1 } );

    graphics::Font::CharacterMap character_map { };

    const auto program = content_manager->get_or_load<graphics::Program>( io::Path { "shaders/default_text_shader.glsl" } );
    const auto material = std::make_shared<graphics::Material>( program );

    for ( uint8_t c = 0; c < 128; c++)
    {
        // Load character glyph
        CHECK_EQ( FT_Load_Char( face, c, FT_LOAD_RENDER ), 0, "Could not load glyph." );

        // Generate texture
        const auto texture = std::make_shared<graphics::Texture>
        (
            face->glyph->bitmap.buffer,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            graphics::gl::TextureFormat::R,
            graphics::gl::Filter::Linear
        );

        const auto sprite = std::make_shared<graphics::Sprite>
        (
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            texture
        );

        const auto geometry = std::make_shared<graphics::Geometry2D>( graphics::make_rectangle_2D( face->glyph->bitmap.width, face->glyph->bitmap.rows ) );
        const auto render_pack = graphics::RenderPack2D { geometry,  };

        //render_pack.material->set_uniform( "u_sampler2", graphics::UniformTexture { texture, to_texture_unit_index( graphics::gl::NamedTextureUnit::Albedo ) } );

        // Now store character for later use
        graphics::Font::Character character
        {
            render_pack,
            glm::vec2 { texture->get_width(), texture->get_height() },
            glm::ivec2 { face->glyph->bitmap_left, face->glyph->bitmap_top },
            // advance is stored in units that are 1/64th of a pixel
            // bitshift by 6 since 2^6 = 64
            glm::ivec2( face->glyph->advance.x >> 6, face->glyph->advance.y >> 6 )
        };

        character_map.insert( { c, character } );
    }

    graphics::gl::pixel_store( graphics::gl::PixelStorageMode::UnpackAlignment, graphics::gl::Size { 4 } ); // Set back to initial value

    FT_Done_Face( face );

    return character_map;
}

} // namespace anonymous

void init_font_loader()
{
    CHECK_EQ( FT_Init_FreeType( &ft ), 0, "Could not init FreeType library." );
}

std::shared_ptr<graphics::Font> load_font( shake::content::ContentManager* content_manager, const io::Path& path )
{
    const auto content = io::file::json::read( path );

    const auto font_default_path        = io::Path{ io::file::json::read_as<std::string>( content, { "default"      } ) };
    const auto font_itallic_path        = io::Path{ io::file::json::read_as<std::string>( content, { "itallic"      } ) };
    const auto font_bold_path           = io::Path{ io::file::json::read_as<std::string>( content, { "bold"         } ) };
    const auto font_bold_itallic_path   = io::Path{ io::file::json::read_as<std::string>( content, { "bold_itallic" } ) };

    return std::make_shared<graphics::Font>
    (
        load_character_map( content_manager, font_default_path ),
        load_character_map( content_manager, font_itallic_path ),
        load_character_map( content_manager, font_bold_path ),
        load_character_map( content_manager, font_bold_itallic_path )
    );
}

void destroy_font_loader()
{
    FT_Done_FreeType( ft );
}

} // namespace load
} // namespace content
} // namespace shake
