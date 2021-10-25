# Install dependencies
choco install strawberryperl
choco install cmake

# Build
cargo build --release --workspace

# Run
./target/release/laplace-eq-therm-server.exe
./target/release/laplace-eq-therm-client.exe