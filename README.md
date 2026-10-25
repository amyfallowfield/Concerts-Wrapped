# Concerts-Wrapped
Spotify wrapped but for concerts

## Build and test

Dependencies are declared in `vcpkg.json`. Install vcpkg and set `VCPKG_ROOT` to its
installation directory before configuring:

```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
cmake --preset vcpkg-debug
cmake --build build --config Debug --target unit-tests
ctest --test-dir build -C Debug --output-on-failure
```

GitHub Actions installs the manifest dependencies, builds `unit-tests`, and runs
CTest for pushes and pull requests.
