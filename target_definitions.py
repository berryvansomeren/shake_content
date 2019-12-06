from pathlib import Path
from typing import List

from cmake_generator import NewTarget, Target, TargetType

# ----------------------------------------------------------------
def get_target_definitions() -> List[ Target ]:
    shake_core = NewTarget(
        name = 'shake_content',
        target_type = TargetType.SharedLibrary,
        src_dir_path = ( Path( __file__ ).resolve().parent / 'src/' ).as_posix(),
        dependencies = [
            'freetype',
            'glm',
            'json11',
            'stb',
            'shake_core',
            'shake_graphics',
            'shake_io'
        ]
    )
    return [ shake_core ]
