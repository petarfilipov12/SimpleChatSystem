OURCE_DIR="."

SERVER_OUT="cpp_out"
SERVER_OUT_DEST_DIR="../server/code/protobuf"

CLIENT_OUT="python_out"
CLIENT_OUT_DEST_DIR="client"

PROTO_SOURCES="$(find ${SOURCE_DIR} -type f -name '*.proto')"

for proto_file in "${PROTO_SOURCES[@]}"; do
    protoc --proto_path=$SOURCE_DIR --$SERVER_OUT=$SERVER_OUT_DEST_DIR $SOURCE_DIR$proto_file
done