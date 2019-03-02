The content module is responsible for loading game content.
It also provides a global content manager,
that manages the content cache for you,
and that makes loading content easier.
It therefore has dependencies on:
- core          | for data structures and algorithms
- io            | for reading files
- graphics      | to load graphics related content
- script_system | to load logic related content
