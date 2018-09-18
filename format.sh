#!/bin/bash
find lib/ src/  -regex '.*\.\(\|h\|cc\)' -exec clang-format -style=google -i {} \;
