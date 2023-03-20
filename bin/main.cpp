#include "lib/CircularBuffer.hpp"
#include "lib/CircularBufferExt.hpp"

int main() {
    CircularBuffer<int>::iterator it;
    CircularBuffer<int>::const_iterator c_it(it);
    return 0;
}
