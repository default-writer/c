& git submodule init
& git submodule add -f --name deps/rexo https://github.com/christophercrouzet/rexo.git deps/rexo
& git submodule update --recursive --remote
& git pull --recurse-submodules