set -e
cd "$(dirname "$0")"
emcmake cmake
echo ""
echo "---"
echo ""
emmake make
