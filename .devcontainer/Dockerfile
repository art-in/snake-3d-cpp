FROM emscripten/emsdk:3.0.0

# additional packages
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    # linter for c++
    clang-tidy

# hide "new version of npm available" notification from CLI
RUN npm config set update-notifier false
