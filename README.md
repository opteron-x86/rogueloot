# rogueloot

Environment setup and build instructions (Windows)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

vcpkg x-update-registry --add https://raw.githubusercontent.com/Ryan-rsm-McKenzie/CommonLibSSE/main/ports/vcpkg-registry.json

# Install CommonLibSSE-NG
vcpkg install commonlibsse-ng:x64-windows-static-md

git clone https://github.com/CharmedBaryon/CommonLibSSE-NG.git
cd CommonLibSSE-NG
git submodule update --init --recursive

# Add CommonLibSSE-NG as subdirectory
add_subdirectory(path/to/CommonLibSSE-NG CommonLibSSE-NG EXCLUDE_FROM_ALL)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        CommonLibSSE::CommonLibSSE
)

# Configure
cmake --preset release

# Build
cmake --build build --config Release

# Output will be in build/Release/LootDrop.dll