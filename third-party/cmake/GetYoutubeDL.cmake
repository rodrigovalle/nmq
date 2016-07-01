# TODO: make it so we don't download this binary every friggin time
# look into making a custom target that depends on the youtube-dl file, so we
# don't redownload more than once
file(
    DOWNLOAD "https://yt-dl.org/latest/youtube-dl" "${DEPS_INSTALL_DIR}/youtube-dl"
    SHOW_PROGRESS
)
file(
    COPY "${DEPS_INSTALL_DIR}/youtube-dl"
    DESTINATION "${DEPS_INSTALL_DIR}/bin"
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
file(REMOVE "${DEPS_INSTALL_DIR}/youtube-dl")
