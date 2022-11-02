& git config --global core.autocrlf input
& git submodule init
& git submodule add -b no_sanitize_address -f --name deps/rexo https://github.com/default-writer/rexo.git deps/rexo
& git submodule update --recursive --remote
& git pull --recurse-submodules